/**------------------------------------------------------------------
 ** Author: Jian Chen (jichen@umbc.edu)
 * Nov 2013
 *
 * QDOTdataNorm.cpp
---------------------------------------------------------------------
*/

#include "QDOTdataNorm.h"
#include <vector>
#include <algorithm>

//#define VEL_SCALAR 1000000000000  // 10^12 scale up the velocity field
#define VEL_SCALAR 1 // the velocity field has been normalized
#define DEN_SCALAR 10000000000000000 //10^12 scale up the density

#define LARGE_DUMMY 10000
#define SMALL_DUMMY -10000

#define FLOATING_POINT_ERROR 0.00001

// read data
QDOTdataNorm::QDOTdataNorm(svChar *dir, svChar *filename)
{
  inDir = strdup(dir);
  inf = strdup(filename);

  layerSize = 1;
  quatileSize=1;

  lbbox=svVector3(SMALL_DUMMY, SMALL_DUMMY, SMALL_DUMMY);
  rbbox=svVector3(LARGE_DUMMY, LARGE_DUMMY, LARGE_DUMMY);

  dataPos = NULL;
  dataVel = NULL;
  dataSplitPos=NULL;
  dataSplitVel=NULL;

  BY_QUATILE=false;
  BY_LAYER=false;

  readData();
}

// not used by now.. splitData() will split by quatile as well
// separate data by layers only
QDOTdataNorm::QDOTdataNorm(svChar *dir, svChar *filename, int layers)
{
  inDir = strdup(dir);
  inf = strdup(filename);
  layerSize = layers;
  quatileSize=1;

  dataPos = NULL;
  dataVel = NULL;
  dataSplitPos=NULL;
  dataSplitVel=NULL;

  BY_QUATILE=false;
  BY_LAYER=true;

  readData();
  splitData();
}

// separate data by layers and quatile
QDOTdataNorm::QDOTdataNorm(svChar *dir, svChar *filename, int layers, int quantile)
{
  inDir = strdup(dir);
  inf = strdup(filename);
  layerSize = layers;
  //if(quantile!=2)  // should use 2 only; two splits: 0/0.5 and 0.25/0.75
  quatileSize=2; 

  dataPos = NULL;
  dataVel = NULL;
  dataSplitPos=NULL;
  dataSplitVel=NULL;

  BY_QUATILE=true;
  BY_LAYER=true;

  readData();
  splitData();
}

QDOTdataNorm::QDOTdataNorm(svChar *dir, svChar *filename, svChar *outDir)
{
  inDir = strdup(dir);
  inf = strdup(filename);

  dataPos = NULL;
  dataVel = NULL;
  dataSplitPos=NULL;
  dataSplitVel=NULL;
  dataLayerPos = NULL;
  dataLayerDir = NULL;
  splitMag = NULL;

  BY_QUATILE=true;
  BY_LAYER=true;

  readData();
  SortData();
  //SetAverage();
  SetSamples();
/*
  vector<svVector3> *splitpos = new vector<svVector3>[zValue.size()];
  vector<int> *splitden = new vector<int>[zValue.size()];
  vector<svVector3> *splitvel = new vector<svVector3>[zValue.size()];

  int size = dataPos[0].size();

  int j;
  for(int i=0;i<size;i++)
  {
	for(j=0;j<zValue.size();j++)
	{
		if(zValue[j]==dataPos[0][i][2])
		{
			splitpos[j].push_back(dataPos[0][i]);
			splitden[j].push_back(dataDen[i]);
			splitvel[j].push_back(dataVel[0][i]);
			break;
		}
	}		
  }
  for(int i=0;i<zValue.size();i++)
  {
	  // output filename
	  svChar vtkOutFileName[2056];
	  sprintf(vtkOutFileName, "%s/%s_TL%dl%d.vtk", 
	          outDir, inf, zValue.size(), i);

	  ofstream vtkOutFile(vtkOutFileName, ios::out | ios::trunc);
	  if(!vtkOutFile)
	  {
	    cerr << "==> Write to file " << vtkOutFileName << " failed. Program exits."<< endl;
	    exit(-1);
	  }
	  else
	    cerr << "Saving to vtk: " << vtkOutFileName << endl;

      // output to vtk

	  vtkOutFile<<"# vtk DataFile Version 2.0"<<endl;
	  vtkOutFile<<vtkOutFileName<<endl;
	  vtkOutFile<<"ASCII"<<endl;
	  vtkOutFile<<"DATASET UNSTRUCTURED_GRID"<<endl;
	  vtkOutFile<<"POINTS " << splitpos[i].size() << " float"<< endl;
	for(j=0;j<splitpos[i].size();j++)
	{

	      vtkOutFile<<splitpos[i][j][0]<<" "
		            <<splitpos[i][j][1]<<" "
		            <<splitpos[i][j][2]<<endl;
	}
      	  vtkOutFile<<endl;
	  vtkOutFile<<"POINT_DATA " << splitpos[i].size() << endl;
	  vtkOutFile<<"VECTORS velocity float" << endl;
	for(j=0;j<splitpos[i].size();j++)
	{

	      vtkOutFile<<splitvel[i][j][0]<<" "
		            <<splitvel[i][j][1]<<" "
		            <<splitvel[i][j][2]<<endl;
	}

	  vtkOutFile<<endl;
	  vtkOutFile<<"SCALARS density float " << 1 << endl;
	  vtkOutFile<<"LOOKUP_TABLE default" << endl;
	for(j=0;j<splitpos[i].size();j++)
	{

	      vtkOutFile<<splitden[i][j]<<endl;
	}

	  vtkOutFile.close();
	  cerr << "Saved......  " << endl;			
  }*/
}

/*
QDOTdataNorm::QDOTdataNorm(svChar *dir, svChar *filename, float zmin, float zmax)
{
  inDir = strdup(dir);
  inf = strdup(filename);
  layerSize=1;
  quatileSize=1;

  dataPos = NULL;
  dataVel = NULL;
  dataSplitPos=NULL;
  dataSplitVel=NULL;

  BY_QUATILE=false;
  BY_LAYER=false;

  readData();
  splitData(zmin, zmax);
}
*/

// separate data by layers and quatile

void QDOTdataNorm::Print()
{
  int ds = dataPos[0].size(); // data size in that layer
  cerr << "------Print() -----------------------------"<<endl;
  cerr << "Print: dataPos size = " << ds << endl;
  for(int j=0; j<ds; j++)
    cerr << j << ": " 
	     << dataPos[0][j][0]<<" "<<dataPos[0][j][1]<<" "<<dataPos[0][j][2]<<", "
	 	 << dataVel[0][j][0]<<" "<<dataVel[0][j][1]<<" "<<dataVel[0][j][2]<<endl;
  cerr << "------end of Print()------------------------"<<endl;

  /*
  for(int i=0; i<layerSize; i++)
  {
    for(int j=0; j<ds; j++)
      cerr << j << ": " 
	    << dataSplitPos[0][j][0]<<" "<<dataSplitPos[0][j][1]<<" "<<dataSplitPos[0][j][2]<<", "
		<< dataSplitVel[0][j][0]<<" "<<dataSplitVel[0][j][1]<<" "<<dataSplitVel[0][j][2]<<endl;
  }; // end for i
  */
}

QDOTdataNorm::~QDOTdataNorm()
{
  cleanData();
  cleanSplitData();
}

int QDOTdataNorm::GetPowNum(svScalar numCopy)
{
	int multiplier = 0;
		  if(numCopy<1&&numCopy>0) 
		  {
		    while (1)
			{
		      if(numCopy>=1)
			  {
			    break;
			  }; // end if(num)
			  multiplier--;
			  numCopy *= 10;
			}; // end while(1)
		  }
		else if(numCopy==0)
		{multiplier=0;}
		  else
		  {
		    while(1)
			{
			  if(numCopy>=0 && numCopy <10)
			  {
			    break;
			  };
			  multiplier++;
			  numCopy /= 10.;
			}; // end while (1)
		  }; // end if(numCopy)	
	return multiplier;
}

void QDOTdataNorm::SaveToVTK(svChar *outDir)
{
  for(int li=0; li<layerSize; li++)
  {
    for(int qi=0; qi<quatileSize; qi++)
    {
	  int dataIndex = 2*li+qi;
	  int samplingSize=dataSplitPos[dataIndex].size();

	  // output filename
	  svChar vtkOutFileName[2056];
	  sprintf(vtkOutFileName, "%s/%s_TL%dl%dq%d.vtk", 
	          outDir, inf, layerSize, li, qi);

	  ofstream vtkOutFile(vtkOutFileName, ios::out | ios::trunc);
	  if(!vtkOutFile)
	  {
	    cerr << "==> Write to file " << vtkOutFileName << " failed. Program exits."<< endl;
	    exit(-1);
	  }
	  else
	    cerr << "Saving to vtk: " << vtkOutFileName << endl;

      // output to vtk

	  vtkOutFile<<"# vtk DataFile Version 2.0"<<endl;
	  vtkOutFile<<vtkOutFileName<<endl;
	  vtkOutFile<<"ASCII"<<endl;
	  vtkOutFile<<"DATASET UNSTRUCTURED_GRID"<<endl;
	  vtkOutFile<<"POINTS " << samplingSize << " float"<< endl;
	  for(int si=0; si<samplingSize; si++)
	  {
	      vtkOutFile<<dataSplitPos[dataIndex][si][0]<<" "
		            <<dataSplitPos[dataIndex][si][1]<<" "
		            <<dataSplitPos[dataIndex][si][2]<<endl;
	  }; // end for(si)

      vtkOutFile<<endl;
	  vtkOutFile<<"POINT_DATA " << samplingSize << endl;
	  vtkOutFile<<"VECTORS velocity float" << endl;
	  for(int si=0; si<samplingSize; si++)
	  {
	      vtkOutFile<<dataSplitVel[dataIndex][si][0]<<" "
		            <<dataSplitVel[dataIndex][si][1]<<" "
		            <<dataSplitVel[dataIndex][si][2]<< endl;
	  }; // end for(si)

	  vtkOutFile<<endl;
	  vtkOutFile<<"SCALARS density float " << 1 << endl;
	  vtkOutFile<<"LOOKUP_TABLE default" << endl;
	  for(int si=0; si<samplingSize; si++)
	  {
	    vtkOutFile<<dataSplitDen[dataIndex][si]<<endl;
	  }

	  vtkOutFile.close();
	  cerr << "Saved......  " << endl;
    }; // end for(qi)
  }; // end for(li)
}

void QDOTdataNorm::GetPosition()
{
}

/** only split the data by z values
 *  no layer, no quatile
 *  Not implemented for now (Dec 10 2013) - TODO: running out of time..
 */
/*
void QDOTdataNorm::splitData(float zmin, float zmax)
{
  cerr<<"Spliting data ... "<<dataPos[0].size()<<" points......"<<endl;
  cerr << "\tNumber of layers: 1" << endl;
  cerr << "\tNumber of quatiles (fixed): 1" << endl;
  cerr << "\tSampling sites (zmin, zmax): (" << zmin <<", " << zmax << ")"<<endl;

  dataSplitPos = new svVector3Array[1];
  dataSplitVel = new svVector3Array[1];

  int ds = dataPos[0].size(); // data size in that layer
  for(int dj=0; dj<ds; dj++)
  {
    if(dataPos[0][di][2]>=zmin &&
	   dataPos[0][di][2]<=zmax)
	{
	  dataSplitPos[dataIndex].add(dataPos[0][dj]);
	  dataSplitVel[dataIndex].add(dataVel[0][dj]);
	}; // end if(dataPos)
  }; // end for(dj)
}
*/

void QDOTdataNorm::splitData()
{
  double layerZOffset = (rbbox[2]-lbbox[2])/ layerSize;

  cerr<<"Spliting data ... "<<dataPos[0].size()<<" points......"<<endl;
  cerr << "\tNumber of layers: " << layerSize << endl;
  cerr << "\tNumber of quatiles (fixed): 2" << endl;
  cerr << "\tLayer z offset: " << layerZOffset << endl;

  // OK. on the safe side, clean the splitdata first
  // allows multiple calls to split the data differently.
  cleanSplitData();

  // allocate memory
  dataSplitPos = new svVector3Array[layerSize * quatileSize];
  dataSplitVel = new svVector3Array[layerSize * quatileSize];
  dataSplitDen = new svScalarArray[layerSize * quatileSize];

  for(int ls=0; ls<layerSize*quatileSize; ls++) // initialize the density sampling values
  {
    denMinBox.add(LARGE_DUMMY);
	denMaxBox.add(SMALL_DUMMY);
  }; // end for(ls)

  int  dataIndex=0;

  // here is the order the data being saved...
  // dataSplitPos[0]=l0q0; dataSplitPos[1]=l0q1; 
  // dataSplitPos[2]=l1q0; dataSplitPos[3]=l1q1;
  // dataSplitPos[4]=l2q0; dataSplitPos[5]=l2q1;
  // dataSplitPos[6]=l3q0; dataSplitPos[7]=l3q1;
  // dataSplitPos[8]=l4q0; dataSplitPos[9]=l4q1;
  // evenly divide the z values based on bounding box size
  int ds = dataPos[0].size(); // data size in that layer

  for(int dj=0; dj<ds; dj++) // split the data into two quatile
  {
	int currentLayer = (int) ((dataPos[0][dj][2]-lbbox[2]) / layerZOffset );

	if(currentLayer>=layerSize) // deal with the last number
	  currentLayer-=1;

    // integer and half integer sites
    if((fabs (dataPos[0][dj][0] - (int) dataPos[0][dj][0]) < FLOATING_POINT_ERROR) ||
	   (fabs (dataPos[0][dj][0]*2 - (int)(dataPos[0][dj][0]*2)) < FLOATING_POINT_ERROR) )
	{
	  dataIndex = 2*currentLayer;
#ifdef __MY_DUBUG__
	  cerr << "quatile type 0: dataIndex = " << dataIndex << " layer:" << currentLayer << " x: "  
	       << dataPos[0][dj][0]<< " z: " << dataPos[0][dj][2]<<endl;
#endif
	}
	else  // quatile and three-fouth quatile sites
	{
	  dataIndex = 2*currentLayer + 1;
#ifdef __MY_DUBUG__
	  cerr << "quatile type 1: dataIndex = " << dataIndex << " layer:" << currentLayer << " x: "  
	       << dataPos[0][dj][0]<<" z: " << dataPos[0][dj][2]<< endl;
#endif
	}; // end if(fabs)

	dataSplitPos[dataIndex].add(dataPos[0][dj]);
	dataSplitVel[dataIndex].add(dataVel[0][dj]);
	dataSplitDen[dataIndex].add(dataDen[dj]);

    if(denMinBox[dataIndex] > dataDen[dj])
	  denMinBox[dataIndex]= dataDen[dj];
	if(denMaxBox[dataIndex] < dataDen[dj])
	  denMaxBox[dataIndex]= dataDen[dj];
  }; // end for(dj)

  cerr << "... Done spliting data into "<< layerSize * 2 << " datasets."<<endl;
}

void QDOTdataNorm::readData()
{
  long double vx, vy, vz;
  long double px, py, pz;
  long double dummy;

  svVector3 pos;
  svVector3 vel;
  svScalar  density;

  svChar filename[2056];
  sprintf(filename, "%s/%s", inDir, inf);

  ifstream infile(filename);
  if(infile.fail())
  {
    cerr << inf << "not found.\n";
	cerr << "Load QDOT data failed. prog exit\n" <<endl;
	exit(-1);
  }
  else
  {
	cerr << "Loading data: " << filename << endl;
  }

  // in this case, all data are displayed in a single layer
  dataPos = new svVector3Array[1];
  dataVel = new svVector3Array[1];

  splitdennum = 0;
  vector<int> tmppow;
  int exp;
  bool flag = true;

  while(infile>>pos[0] >> pos[1] >> pos[2] >> dummy >> dummy >> vel[0] >> vel[1] >> vel[2] >> density)
  {
	flag = true;

   	vel*=VEL_SCALAR;
	dataPos[0].add(pos);  // dealing with a single layer only
	dataVel[0].add(vel);

	density*=DEN_SCALAR;
	dataDen.add(density);

	// set the bounding box
	if(pos[0]<lbbox[0]) lbbox[0]=pos[0];
	if(pos[0]>rbbox[0]) rbbox[0]=pos[0];
	if(pos[1]<lbbox[1]) lbbox[1]=pos[1];
	if(pos[1]>rbbox[1]) rbbox[1]=pos[1];
	if(pos[2]<lbbox[2]) lbbox[2]=pos[2];
	if(pos[2]>rbbox[2]) rbbox[2]=pos[2];

    // split the data layers using the z value 
    // layerSize = # of z values
	// compared to the zValue saved
	int zi;
	for(zi=0; zi<zValue.size(); zi++)
	{
	  if(pos[2]==zValue[zi])
	    break;
	}; // end for(zi)
	if(zi==zValue.size()) // not found
	{
	  zValue.add(pos[2]);
	  cerr << "--> zValue: insert " << pos[2]<<endl;
	};  // end if(zi)

	exp = GetPowNum(density);
	for(int kk=0;kk<tmppow.size();kk++)
	{
		if(exp==tmppow[kk])
		{flag = false;break;}
	}
	if(flag)
	{tmppow.push_back(exp);}
  };
  infile.close();

	for(int i=0;i<tmppow.size();i++)
	{
		cerr<<tmppow[i]<<" ";
	}cerr<<endl;

  cerr<<"... Done loading sampling sites of "<<dataPos[0].size()<<" points......"<<endl;
  cerr << "\tBounding box size: ("
       <<lbbox[0] <<", "<<lbbox[1]<<", "<<lbbox[2]<<"), (" 
       <<rbbox[0] <<", "<<rbbox[1]<<", "<<rbbox[2]<<")" 
       <<endl;
  cerr << "\tNumber of layers (z values): " << zValue.size() << endl;
  for(int zi=0; zi<zValue.size(); zi++)
  {
    cerr << zValue[zi]<<" ";
  }; // end for(zi)
  cerr << endl;

  xl = rbbox[0] - lbbox[0];
  yl = rbbox[1] - lbbox[1];
}

void QDOTdataNorm::Find0Data()
{
	zerodataPos = new svVector3Array[1];
	zerodataDir = new svVector3Array[1];
	zerodataDen = new svScalarArray[1];

	int samplenum = dataPos[0].size();

	for(int i=0;i<samplenum;i++)
	{
		if((float)(dataPos[0][i][2]-(int)dataPos[0][i][2])==0.)
		{
			zerodataPos[0].add(dataPos[0][i]);
			zerodataDir[0].add(dataVel[0][i]);
			zerodataDen[0].add(dataDen[i]);
		}
	}
}

vector<float> xvector;
vector<float> yvector;
vector<float> zvector;
vector<int> in;

bool comparez (int a, int b) { return zvector[a] < zvector[b]; }
bool comparey (int a, int b) { return yvector[a] < yvector[b]; }
bool comparex (int a, int b) { return xvector[a] < xvector[b]; }

bool compare(int a, int b)
{
	if(zvector[a]!=zvector[b])
		return zvector[a]<zvector[b];
	else if(yvector[a]!=yvector[b])
		return yvector[a]<yvector[b];
	else if(xvector[a]!=xvector[b])
		return xvector[a]<xvector[b];
}

bool comparexy(int a, int b)
{
	if(yvector[a]!=yvector[b])
		return yvector[a]<yvector[b];
	else if(xvector[a]!=xvector[b])
		return xvector[a]<xvector[b];	
}
bool comparexz(int a, int b)
{
	if(zvector[a]!=zvector[b])
		return zvector[a]<zvector[b];
	else if(xvector[a]!=xvector[b])
		return xvector[a]<xvector[b];	
}
bool compareyz(int a, int b)
{
	if(zvector[a]!=zvector[b])
		return zvector[a]<zvector[b];
	else if(yvector[a]!=yvector[b])
		return yvector[a]<yvector[b];	
}
void QDOTdataNorm::SortData()
{	
	cerr<<"Start sorting data. Please wait...."<<endl;
	int i;
	int dataSize = dataPos[0].size();

	for(i=0;i<dataSize;i++)
	{
		zvector.push_back(dataPos[0][i][2]);
		xvector.push_back(dataPos[0][i][0]);
		yvector.push_back(dataPos[0][i][1]);
		in.push_back(i);
	}

	sort(in.begin(),in.end(), compare);
	sort(in.begin(),in.end(), compare);
	sort(in.begin(),in.end(), compare);

	svVector3 tmp;
	int countx = 0;
	int county = 0;
	int countxy = 0;
	xindex.push_back(0);
	yindex.push_back(0);
	zindex.push_back(0);
	xinz.push_back(0);
	yinz.push_back(0);
	xiny.push_back(0);

	zlayernum = 1;
	float yd,xd;
	yd = 0;xd = 0;

	for(i=0;i<dataSize;i++)
	{
		tmp = dataPos[0][in[i]];
		sortpos.push_back(tmp);
		sortindex.push_back(in[i]);
		if(i!=0)
		{
			if(sortpos[i][2]!=sortpos[i-1][2])
			{
				zindex.push_back(i);
				yinz.push_back(county);
				xinz.push_back(countx);
				zlayernum++;
				ydistance.push_back(yd);
				yd = 0;
				zdistance = sortpos[i][2] -sortpos[i-1][2];
			}
			if(sortpos[i][1]!=sortpos[i-1][1])
			{
				yindex.push_back(i);
				xiny.push_back(countx);
				if(sortpos[i][2]==sortpos[i-1][2])
					yd = sortpos[i][1]-sortpos[i-1][1];
				county++;
				xdistance.push_back(xd);
				xd = 0;
			}
			if(sortpos[i][0]!=sortpos[i-1][0])
			{
				xindex.push_back(i);
				if(sortpos[i][2]==sortpos[i-1][2]
					&&sortpos[i][1]==sortpos[i-1][1])
					xd = sortpos[i][0]-sortpos[i-1][0];
				countx++;
			}
		}
	}
	ydistance.push_back(yd);
	xdistance.push_back(xd);
	cerr<<"Sorting is done"<<endl;
}

void QDOTdataNorm::splitDen()
{
	
}

void QDOTdataNorm::SetSamples()
{
	vector<int> tmpi;
	int i,j,k;
	int datasize = sortpos.size();
	int s1,s2,e1,e2;

	for(i=0;i<zindex.size();)
	{
		//cerr<<i<<endl;
		s1 = yinz[i]+1;
		if(i+1>=yinz.size())
			e1 = yinz.size();
		else
			e1 = yinz[i+1]+1;
		for(j=s1;j<e1;)
		{
			if(j+1>=xiny.size())
				e2 = xiny.size();	
			else
				e2 = xiny[j+1]+1;
			s2 = xiny[j]+1;	
			for(k=s2;k<e2;)
			{
				tmpi.push_back(k);
				k = k + 10;
			}
			j = j + 10;
		}
		i = i + 10;
	}

	datasize = tmpi.size();
	cerr<<datasize<<endl;
	sampleNum = datasize;
	samplePos = new svVector3[datasize];
	sampleDen = new svScalar[datasize];
	sampleVel = new svVector3[datasize];

	for(i=0;i<datasize;i++)
	{
		samplePos[i] = dataPos[0][sortindex[tmpi[i]]];
		sampleVel[i] = dataVel[0][sortindex[tmpi[i]]];
		sampleDen[i] = dataDen[sortindex[tmpi[i]]];
		//cerr<<sampleDen[i]<<" ";
	}
	//cerr<<endl;
}

void QDOTdataNorm::LineInterpolation(int s, int e, int &index1, int &index2, svVector3 p,
		float xd)
{
	int i;
	float tmpi = (p[0] - sortpos[s][0])/xd;
	if(tmpi>=0)
	{
		i = tmpi;
		i = i + s;
	
		index1 = i;
		if(i<e-1)
			index2 = i+1;
		else
			index2 = -1;
	}
	else
	{
		index1 = -1;index2 = s;
	}
}

int QDOTdataNorm::PlaneInterpolation(int s,svVector3 p, float yd)
{
	int i;
	i = (p[1] - sortpos[s][1])/yd;
	
	return i;	
}

bool QDOTdataNorm::comparedistance(int index1, int index2, svVector3 p)
{
	float d1, d2;

	if(index1==-1)return false;
	if(index2==-1)return true;

	d1 = pow(sortpos[index1][0]-p[0],2.)
	+ pow(sortpos[index1][1]-p[1],2.)
	+ pow(sortpos[index1][2]-p[2],2.);

	d2 = pow(sortpos[index2][0]-p[0],2.)
	+ pow(sortpos[index2][1]-p[1],2.)
	+ pow(sortpos[index2][2]-p[2],2.);

	if(d1<d2)
		return true;
	else
		return false;
}

float QDOTdataNorm::DotProduct(svVector3 p1, svVector3 p2)
{
	float r;
	r = p1[0]*p2[0] + p1[1]*p2[1] + p1[2]*p2[2];
	
	return r; 
}

void QDOTdataNorm::SetAverage()
{
	dataLayerPos = new svVector3Array[zlayernum];
	dataLayerDir = new svScalarArray[zlayernum];

	int datasize = sortpos.size();
	int i;

	int j=0;	
	int t=0;
	svVector3 p;
	vector<int> sz;
	vector<int> sy;
	vector<float> dz;
	vector<float> dy;
	vector<int> yz;
	int ni[6];
	t = 0;
	float tmpdz;
	float tmpdy;
	svScalar d;

	for(i=0;i<datasize;i++)
	{
		if(i==0)
		{
			dataLayerPos[0].add(sortpos[i]);
			sz.push_back(0);
			yz.push_back(-1);
		}
		else if(sortpos[i][2]!=sortpos[i-1][2])
		{
			j++;
			sz.push_back(i);
			yz.push_back(t);
			dz.push_back(tmpdz);
			dataLayerPos[j].add(sortpos[i]);
			//cerr<<sz[j]<<" ";
		}
		else
		{
			dataLayerPos[j].add(sortpos[i]);
		}

		if(i==0)
			{sy.push_back(0);}
		else if(sortpos[i][1]!=sortpos[i-1][1])
		{
			t++;
			dy.push_back(tmpdy);
			tmpdz = sortpos[i][1]-sortpos[i-1][1];
			sy.push_back(i);
		}	

		if(i!=0)
			tmpdy = sortpos[i][0] - sortpos[i-1][0];
	}
	//cerr<<endl;
	dy.push_back(tmpdy);
	dz.push_back(tmpdz);

	xd = tmpdy;
	yd = tmpdz;

	int tmp;
	int tmp1, tmp2;
	int tmp3, tmp4;
	int count = 0;
	svScalar dir;
	svScalar dd1 = 9e+9;
	svScalar dd2 = -9e+9;
	for(i=0;i<datasize;i++)
	{
		if(i==0)
		{
			j=0;
		}
		else if(sortpos[i][2]!=sortpos[i-1][2])
		{
			j++;
		}

		if(i==0)
		{t=0;}
		else if(sortpos[i][1]!=sortpos[i-1][1])
		{
			t++;
		}		

		p = sortpos[i];
		
		//-----------------
		if(sy[t]!=sz[j])
			LineInterpolation(sy[t-1], sy[t], ni[0], ni[1], p,dy[t-1] );
		else
			{ni[0]=-1;ni[1]=-1;}

		//-------------------
		if(j+1<sz.size())
		{
			if(sy[t+1]!=sz[j+1])
				LineInterpolation(sy[t+1], sy[t+2], ni[2], ni[3], p,dy[t+1] );
			else
				{ni[2]=-1;ni[3]=-1;}
		}
		else
		{
			if(t+2<sy.size())
				LineInterpolation(sy[t+1], sy[t+2], ni[2], ni[3], p,dy[t+1] );
			else if(t+1<sy.size())
				LineInterpolation(sy[t+1], sortpos.size(), ni[2], ni[3], p,dy[t+1] );
			else {ni[2]=-1;ni[3]=-1;}			
		}

		//------------------
		if(j!=0)
		{
			tmp = PlaneInterpolation(sz[j-1],p, dz[j-1]);
			if(tmp>=0)
			{
				LineInterpolation(sy[yz[j-1]+tmp+1], sy[yz[j-1]+tmp+2], tmp1, tmp2, p, dy[yz[j-1]+tmp+1]);
			}
			else
			{tmp1=-1;tmp2=-1;}
			//if(tmp<=sz[j]-sz[j-1]-1)
			if(sy[yz[j-1]+tmp+2]!=sz[j])
			{
				LineInterpolation(sy[yz[j-1]+tmp+2], sy[yz[j-1]+tmp+3], tmp3, tmp4, p, dy[yz[j-1]+tmp+2]);
			}
			else
			{tmp3=-1;tmp4=-1;}	
			if(comparedistance(tmp1,tmp2,p))
				tmp1 = tmp1;
			else
				tmp2 = tmp2;	
			if(comparedistance(tmp3,tmp4,p))
				tmp3 = tmp3;
			else
				tmp3 = tmp4;	
			if(comparedistance(tmp1,tmp3,p))
				ni[4] = tmp1;
			else
				ni[4] = tmp3;
		}
		else
			ni[4] = -1;

		//-------------------
		if(j+1<sz.size())
		{		
			tmp = PlaneInterpolation(sz[j+1],p, dz[j+1]);
			if(tmp>=0)
			{
				if(yz[j+1]+tmp+2<sy.size())
					LineInterpolation(sy[yz[j+1]+tmp+1], sy[yz[j+1]+tmp+2], tmp1, tmp2, p, dy[yz[j+1]+tmp+1]);
				else
					LineInterpolation(sy[yz[j+1]+tmp+1], sortpos.size(), tmp1, tmp2, p, dy[yz[j+1]+tmp+1]);
			}
			else
			{tmp1=-1;tmp2=-1;}
			if(j+2<sz.size())
			{
				if(sy[yz[j+1]+tmp+2]!=sz[j+2])
				{
					if(yz[j+1]+tmp+3<sy.size())
						LineInterpolation(sy[yz[j+1]+tmp+2], sy[yz[j+1]+tmp+3], tmp3, tmp4, p, dy[yz[j+1]+tmp+2]);
					else
						LineInterpolation(sy[yz[j+1]+tmp+2], sortpos.size(), tmp3, tmp4, p, dy[yz[j+1]+tmp+2]);
				}
				else
				{tmp3=-1;tmp4=-1;}
			}
			else 
			{
				if(yz[j+1]+tmp+2<sy.size())
                                {
                                        if(yz[j+1]+tmp+3<sy.size())
                                                LineInterpolation(sy[yz[j+1]+tmp+2], sy[yz[j+1]+tmp+3], tmp3, tmp4, p, dy[yz[j+1]+tmp+2]);
                                        else
                                                LineInterpolation(sy[yz[j+1]+tmp+2], sortpos.size(), tmp3, tmp4, p, dy[yz[j+1]+tmp+2]);
                                }
                                else
                                {tmp3=-1;tmp4=-1;}

			}

			if(comparedistance(tmp1,tmp2,p))
				tmp1 = tmp1;
			else
				tmp2 = tmp2;	
			if(comparedistance(tmp3,tmp4,p))
				tmp3 = tmp3;
			else
				tmp3 = tmp4;	
			if(comparedistance(tmp1,tmp3,p))
				ni[5] = tmp1;
			else
				ni[5] = tmp3;
		}
		else
			ni[5] = -1;	

		/*if(i==0)
		{
			cerr<<sortpos[0][0]<<" "<<sortpos[0][1]<<" "<<sortpos[0][2]<<" "<<endl;
			for(int ii=0;ii<6;ii++)
			{if(ni[ii]!=-1)
				cerr<<ii<<" "<<sortpos[ni[ii]][0]<<" "<<sortpos[ni[ii]][1]<<" "<<sortpos[ni[ii]][2]<<" "<<endl;}
		}		
		if(i==255771)
		{
			cerr<<sortpos[i][0]<<" "<<sortpos[i][1]<<" "<<sortpos[i][2]<<" "<<endl;
			for(int ii=0;ii<6;ii++)
			{if(ni[ii]!=-1)
				cerr<<ii<<" "<<sortpos[ni[ii]][0]<<" "<<sortpos[ni[ii]][1]<<" "<<sortpos[ni[ii]][2]<<" "<<endl;}
		}*/
		count = 0;
		dir = 0.;
		for(int ii=0;ii<6;ii++)
		{
			if(ni[ii]!=-1)
			{
				dir =  dir + DotProduct(dataVel[0][sortindex[i]],dataVel[0][sortindex[ni[ii]]]);
				count++;
			}
		}
		/*if(count==2)	
		{
			cerr<<i<<" "<<sortpos[i][0]<<" "<<sortpos[i][1]<<" "<<sortpos[i][2]<<endl;
			for(int ii=0;ii<6;ii++)
				if(ni[ii]!=-1)
				cerr<<ii<<" "<<sortpos[ni[ii]][0]<<" "<<sortpos[ni[ii]][1]<<" "<<sortpos[ni[ii]][2]<<endl;
			cerr<<"------------------"<<endl;
		}*/
		d = (svScalar)dir/count;
		if(d<dd1) dd1 = d;
		if(d>dd2) dd2 = d; 

		dataLayerDir[j].add(d);
	}

	dataLayerMax = dd2;
	dataLayerMin = dd1;

	//cerr<<dd1 <<" "<<dd2<<endl;
}

void QDOTdataNorm::cleanData()
{
  if(dataPos!=NULL)
  {
    cerr << "... Cleaning up dataPos and dataVel. "<< endl;
	dataPos[0].free();
	dataVel[0].free();
	dataDen.free();

	delete [] dataPos;
	delete [] dataVel;
  };

	if(dataLayerPos!=NULL)
	{
		for(int i=0;i<zlayernum;i++)
			dataLayerPos[i].free();
		delete [] dataLayerPos;
		dataLayerPos = NULL;
	}

	if(dataLayerDir!=NULL)
	{
		for(int i=0;i<zlayernum;i++)
			dataLayerDir[i].free();
		delete [] dataLayerDir;
		dataLayerDir = NULL;
	}
}

void QDOTdataNorm::cleanSplitData()
{
  if(dataSplitPos!=NULL)
  {
    int size = layerSize * quatileSize;
	cerr << "... Cleaning up Split data of size " << size << endl;

    for(int i=0; i<size; i++)
	{
	  dataSplitPos[i].free();
	  dataSplitVel[i].free();
	  dataSplitDen[i].free();
	}

	delete [] dataSplitPos;
	delete [] dataSplitVel;
	delete [] dataSplitDen;
  };
}

