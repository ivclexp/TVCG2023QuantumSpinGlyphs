
#include <math.h>
#include "svQDOT.h"
#include "ludcmp.h"
#include "lubksb.h"
#include "MVectorMatrix.h"
#include <iostream>
#include <fstream>

#ifndef max
#define max(a,b)  ((a)>(b)?(a):(b))
#endif
namespace __svl_lib {

svQDOT::svQDOT()
{
}

svQDOT::~svQDOT()
{
  if(ctrpos != NULL)
    delete [] ctrpos;
  if(ctrxval != NULL)
    delete [] ctrxval;
  if(ctryval != NULL)
    delete [] ctryval;
  if(ctrzval != NULL)
    delete [] ctrzval;
}

svVector3 svQDOT::GetVector(const svParticle & p)
{
  return GetVector(p.position, p.time);
}

svVector3 svQDOT::GetVector(const svVector3& pos, svScalar t)
{
   double vx=0.,vy=0,vz=0;
   //gen_rbf(pos[0],pos[1],pos[2],&vx,&vy,&vz);
   //cerr << "----> v: (" << vx <<", " << vy<<", " << vz<<endl;
   //double s = sqrt(vx*vx+vy*vy+vz*vz);
   //cerr << "----> s: " << s <<endl;

   // here need to calculate the vector velocity at pos
   // OK. find the nearest 8 neighbors weighted by distance
   // 
   int nearestIntegerX = (int)(pos.x - 0.5);
   float diffX = pos.x-nearestIntegerX;
   svVector3 leftBound, rightBound;
   if(diffX>=0 && diffX<=0.25)
   {
     leftBound.x=nearestIntegerX;
     rightBound.x=nearestIntegerX+0.25;
   }
   else if(diffX>0.25 && diffX<=0.5)
   {
     leftBound.x=nearestIntegerX+0.25;
     rightBound.x=nearestIntegerX+0.5;
   }
   else if(diffX>0.5 && diffX<=0.75)
   {
     leftBound.x=nearestIntegerX+0.5;
     rightBound.x=nearestIntegerX+0.75;
   }
   else
   {
     leftBound.x=nearestIntegerX+0.75;
     rightBound.x=nearestIntegerX+1;
   }

   svVector3 rv(vx,vy,vz);
   return rv;
}

void svQDOT::CalculateRoughStrength(int num_seeds)
{
  svVectorField::CalculateRoughStrength(num_seeds);
}

void svQDOT::gen_rbf(double x, double y, double z,
                     double *vx, double *vy, double *vz)
{
  int i;
  distanceMatrix(x,y,z);
  rbfgaussian2();

  svFloat sumx=0., sumy=0., sumz=0.;
  for(i=0; i<nvIn; i++)
  {
    sumx+= EM[i]*wx[i];
    sumy+= EM[i]*wy[i];
    sumz+= EM[i]*wz[i];
  }
  *vx = (double)sumx; *vy = (double)sumy; *vz = (double)sumz;
}

// handle internal file format 
//  file format shall be something like
//   /map/gfx0/tools/linux/src/jchen/vtkCases/rbf/trial6/test6info17.log
void svQDOT::New(char *infname)
{ New(infname, svVector3(-1,-1,-1),svVector3(1,1,1));}

void svQDOT::New(char *infname, svVector3 lb, svVector3 rb)
{
  ifstream inf(infname);
  if(!inf)
  { 
    cerr << "svQDOT: open file " << inf << " filed.. exit.. " << endl;
    exit(-1);
  };
  dimension = 3;
  max_varing_time = 0;
  flow_type = SV_STEADY;
  
  lbbox=lb; rbbox=rb;
  svFloat sx,sy,sz;

  sx = rbbox[0]-lbbox[0];
  sy = rbbox[1]-lbbox[1];
  sz = rbbox[2]-lbbox[2];
  longest_side = max(sx,max(sy,sz));

  char dummy[256], lineptr[256];

// skip the first line
  inf.getline(lineptr,  256);

// read epIn
  inf.getline(lineptr,  256);
  sscanf(lineptr, "%s %f", dummy, &epIn);

//cerr << epIn << endl;
// read nvIn
  inf.getline(lineptr,  256);
  sscanf(lineptr, "%s %d", dummy, &nvIn);
//cerr << nvIn << endl;

// skip one line
  inf.getline(lineptr,  256);

  ctrpos  = new svVector3[nvIn];
  ctrxval = new svFloat[nvIn];
  ctryval = new svFloat[nvIn];
  ctrzval = new svFloat[nvIn];
  wx = new svFloat[nvIn];
  wy = new svFloat[nvIn];
  wz = new svFloat[nvIn];
  dis= new svFloat[nvIn];
  EM =  new svFloat[nvIn];

  // get the random field
  for(int i=0; i<nvIn; i++) {
    inf.getline(lineptr,  256);
    sscanf(lineptr, "%f %f %f %f %f %f", 
            &ctrpos[i][0],&ctrpos[i][1],&ctrpos[i][2],
            &ctrxval[i],&ctryval[i],&ctrzval[i]);
  }; 

  // skip another line
  inf.getline(lineptr,  256);
  for(int i=0; i<nvIn; i++) {
    inf.getline(lineptr,  256);
    sscanf(lineptr, "%f %f %f", &wx[i],&wy[i],&wz[i]);
    //cerr << wx[i] << " " << wy[i] << " " << wz[i] << endl;
  }

  inf.close();
}

//svScalar sx, svScalar sy, svScalar sz, 
void  svQDOT::New(int numvec, 
   svVector3 _lbbox, svVector3 _rbbox, svVector3 * pos, 
   svFloat * posxvalue, svFloat * posyvalue, svFloat * poszvalue,
   svScalar e, svScalar max_time)
{
  int i;
  float sx, sy, sz;
  nvIn = numvec; epIn = e;
  lbbox = _lbbox; rbbox = _rbbox;

  dimension = 3;
  max_varing_time = max_time;
  flow_type = max_time>0. ? SV_UNSTEADY : SV_STEADY;
  sx = _rbbox[0]-_lbbox[0];
  sy = _rbbox[1]-_lbbox[1];
  sz = _rbbox[2]-_lbbox[2];
  longest_side = max(sx,max(sy,sz));

  cerr << "svQDOT New ................. " << endl;
  ctrpos  = new svVector3[nvIn];
  ctrxval = new svFloat[nvIn];
  ctryval = new svFloat[nvIn];
  ctrzval = new svFloat[nvIn];
  wx = new svFloat[nvIn];
  wy = new svFloat[nvIn];
  wz = new svFloat[nvIn];
  dis= new svFloat[nvIn];
  EM =  new svFloat[nvIn];

  for(i=0; i<nvIn; i++) {
    ctrpos[i] = pos[i];
    ctrxval[i] = posxvalue[i];
    ctryval[i] = posyvalue[i];
    ctrzval[i] = poszvalue[i];
  }
  max_varing_time = max_time;
  flow_type = max_time > 0. ? SV_UNSTEADY : SV_STEADY;

  distanceMatrix0();
}

}
