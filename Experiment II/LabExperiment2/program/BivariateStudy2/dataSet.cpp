#include "dataSet.h"

#include <stdio.h>     
#include <stdlib.h>    
#include <time.h>       
#include <cmath>

/*dataSet::dataSet(svChar *dir, svChar *inf)
{
	if(dataPos==NULL)
		cleanup();

        srand (time(NULL));
	ifstream infile;
	char name[40];
	sprintf(name,"%s/%s%d",dir,inf,rand()%20+1);
	cerr<<name<<endl;
	infile.open(name);
	if(!infile.is_open())
	{
		cerr<<"Can not find "<<name<<endl;	
		exit(0);
	}
	int num;
	infile>>num;

	dataPos = new svVector3[num];
	dataDir = new svVector3[num];
	dataDen = new svScalar[num];
	dataCol = new svVector3[num];
	dataSize = num;

	svVector3 start;
	svVector3 pos;
	svVector3 vel;
	svScalar den;
	svScalar *exp = new svScalar[num];
	svScalar *coe = new svScalar[num];
	int n;
	svVector3 d;
	vector<int> index;
	int indexn;
	bool flag;

	infile>>n;
	infile>>start[0]>>start[1]>>start[2];	
	lbbox = start;
	infile>>d[0]>>d[1]>>d[2];
	rbbox[0] = start[0] + d[0] * (n-1);
	rbbox[1] = start[1] + d[1] * (n-1);
	rbbox[2] = start[2] + d[2] * (n-1);
	int j=-1;
	int dd = num/6;
	for(int i=0;i<num;)
	{
		if(i%dd==0)j++;
		exp[i] = j;
		coe[i] = rand()%9+1 + (rand()%10)/10. + (rand()%10)/100.;
		if(coe[i]!=0)i++;
	}
	for(int i=0;i<num;)
	{
		flag = true;
		indexn = rand()%num;
		for(int j=0;j<index.size();j++)
		{
			if(indexn==index[j]){flag=false;break;}
		}
		if(flag)
		{
			index.push_back(indexn);
			i++;
		}
	}
	for(int i=0;i<num;i++)
	{
		infile>>vel[0]>>vel[1]>>vel[2];
		vel=normalize(vel);
		dataDir[i] = vel;

		den = coe[index[i]] * pow(10.,exp[index[i]]);
		dataDen[i] = den;

		pos[0] = start[0] + i%n * d[0];
		pos[1] = start[1] + (i/n)%n * d[1];
		pos[2] = start[2] + i/(n*n) * d[2];

		//cerr<<pos[0]<<" "<<pos[1]<<" "<<pos[2]<<endl;
		dataPos[i] = pos;
	}

	delete [] exp;
	delete [] coe;
	index.clear();

	//cerr<<rbbox[0]<<" "<<lbbox[0]<<endl;
}*/

dataSet::dataSet(svChar *dir, svChar *inf)
{
	lbbox[0] = 9e+9;
	lbbox[1] = 9e+9;
	lbbox[2] = 9e+9;
	rbbox[0] = -9e+9;
	rbbox[1] = -9e+9;
	rbbox[2] = -9e+9;


        srand (time(NULL));
	ifstream infile;
	char name[40];
	sprintf(name,"%s/%s",dir,inf);
	cerr<<name<<endl;
	infile.open(name);
	if(!infile.is_open())
	{
		cerr<<"Can not find "<<name<<endl;	
		exit(0);
	}
	int num;
	infile>>num;

	dataPos = new svVector3[num];
	dataDir = new svVector3[num];
	dataDen = new svScalar[num];
	dataCol = new svVector3[num];
	dataSize = num;

	svVector3 start;
	svVector3 pos;
	svVector3 vel;
	svScalar den;

	for(int i=0;i<num;i++)
	{
		infile>>pos[0]>>pos[1]>>pos[2]
			>>vel[0]>>vel[1]>>vel[2]
			>>den;

		dataPos[i] = pos;
		dataDir[i] = normalize(vel);
		dataDen[i] = den/pow(10.,8.);
		
		if(dataPos[i][0]<=lbbox[0])lbbox[0]=dataPos[i][0];
		if(dataPos[i][1]<=lbbox[1])lbbox[1]=dataPos[i][1];
		if(dataPos[i][2]<=lbbox[2])lbbox[2]=dataPos[i][2];
		if(dataPos[i][0]>=rbbox[0])rbbox[0]=dataPos[i][0];
		if(dataPos[i][1]>=rbbox[1])rbbox[1]=dataPos[i][1];
		if(dataPos[i][2]>=rbbox[2])rbbox[2]=dataPos[i][2];

	}
}

void dataSet::SetData(svChar *dir, svChar *inf)
{
	lbbox[0] = 9e+9;
	lbbox[1] = 9e+9;
	lbbox[2] = 9e+9;
	rbbox[0] = -9e+9;
	rbbox[1] = -9e+9;
	rbbox[2] = -9e+9;

	if(dataSize !=0)
		cleanup();

        srand (time(NULL));
	ifstream infile;
	char name[40];
	sprintf(name,"%s/%s",dir,inf);
	cerr<<name<<endl;
	infile.open(name);
	if(!infile.is_open())
	{
		cerr<<"Can not find "<<name<<endl;	
		exit(0);
	}
	int num;
	infile>>num;

	dataPos = new svVector3[num];
	dataDir = new svVector3[num];
	dataDen = new svScalar[num];
	dataCol = new svVector3[num];
	dataSize = num;

	svVector3 start;
	svVector3 pos;
	svVector3 vel;
	svScalar den;

	for(int i=0;i<num;i++)
	{
		infile>>pos[0]>>pos[1]>>pos[2]
			>>vel[0]>>vel[1]>>vel[2]
			>>den;

		dataPos[i] = pos;
		dataDir[i] = normalize(vel);
		dataDen[i] = den/pow(10.,8.);

		if(dataPos[i][0]<=lbbox[0])lbbox[0]=dataPos[i][0];
		if(dataPos[i][1]<=lbbox[1])lbbox[1]=dataPos[i][1];
		if(dataPos[i][2]<=lbbox[2])lbbox[2]=dataPos[i][2];
		if(dataPos[i][0]>=rbbox[0])rbbox[0]=dataPos[i][0];
		if(dataPos[i][1]>=rbbox[1])rbbox[1]=dataPos[i][1];
		if(dataPos[i][2]>=rbbox[2])rbbox[2]=dataPos[i][2];
	}

}
void dataSet::SetTask1(svChar *dir, svChar *inf)
{
	ifstream infile;
	char name[100];
	sprintf(name,"%s/%s",dir,inf);
	infile.open(name);
	if(!infile.is_open())
	{
		cerr<<"Can not find"<<name<<endl;
		exit(0);
	}

	for(int i=0;i<4;i++)	
	{
		infile>>Task1Point[i];
	}

	infile.close();
}
void dataSet::SetTask2(svChar *dir, svChar *inf)
{
	ifstream infile;
	char name[100];
	sprintf(name,"%s/%s",dir,inf);
	infile.open(name);
	if(!infile.is_open())
	{
		cerr<<"Can not find"<<name<<endl;
		exit(0);
	}

	for(int i=0;i<4;i++)	
	{
		infile>>Task2Point1[i]>>Task2Point2[i];
	}

	infile.close();
}
void dataSet::SetTask3(svChar *dir, svChar *inf)
{
	ifstream infile;
	char name[100];
	sprintf(name,"%s/%s",dir,inf);
	infile.open(name);
	if(!infile.is_open())
	{
		cerr<<"Can not find"<<name<<endl;
		exit(0);
	}

	for(int i=0;i<4;i++)	
	{
		infile>>Task3Point1[i]>>Task3Point2[i];
	}

	infile.close();
}
void dataSet::SetTask4(svChar *dir, svChar *inf)
{
	ifstream infile;
	char name[100];
	sprintf(name,"%s/%s",dir,inf);
	infile.open(name);
	if(!infile.is_open())
	{
		cerr<<"Can not find"<<name<<endl;
		exit(0);
	}

	for(int i=0;i<4;i++)	
	{
		infile>>Task4Point1[i]>>Task4Point2[i];
		infile>>Task4Neighbours1[i][0]>>Task4Neighbours1[i][1]>>Task4Neighbours1[i][2]>>
			Task4Neighbours1[i][3]>>Task4Neighbours1[i][4]>>Task4Neighbours1[i][5]>>
			Task4Neighbours1[i][6]>>Task4Neighbours1[i][7]>>Task4Neighbours1[i][8]>>
			Task4Neighbours1[i][9]>>Task4Neighbours1[i][10]>>Task4Neighbours1[i][11];
		infile>>Task4Neighbours2[i][0]>>Task4Neighbours2[i][1]>>Task4Neighbours2[i][2]>>
			Task4Neighbours2[i][3]>>Task4Neighbours2[i][4]>>Task4Neighbours2[i][5]>>
			Task4Neighbours2[i][6]>>Task4Neighbours2[i][7]>>Task4Neighbours2[i][8]>>
			Task4Neighbours2[i][9]>>Task4Neighbours2[i][10]>>Task4Neighbours2[i][11];
	}

	infile.close();
}
void dataSet::readColorFile(svChar *dir, svChar *inf)
{
    srand (time(NULL));
	ifstream infile;
	char name[40];
	sprintf(name,"%s/%s%d.color",dir,inf,rand()%20+1);
	infile.open(name);
	if(!infile.is_open())
	{
		cerr<<"Can not find "<<name<<endl;	
		exit(0);
	}

	svVector3 col;

	for(int i=0;i<dataSize;i++)
	{
		infile>>col[0]>>col[1]>>col[2];
		dataCol[i] = col;
	}

	infile.close();	
}

void dataSet::cleanup()
{
	delete [] dataPos;
	delete [] dataDir;
	delete [] dataDen;
	delete [] dataCol;
	dataSize = 0;
}



