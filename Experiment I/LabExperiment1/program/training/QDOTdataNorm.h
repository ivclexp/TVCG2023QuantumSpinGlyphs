/**------------------------------------------------------------------
 Author: Jian Chen (jichen@umbc.edu) 
 Description: NIST QDOT data parser
 Data: Nov 2013
 Version 1.0:
---------------------------------------------------------------------
*/

#ifndef __QDOT_DATA_NORM_H
#define __QDOT_DATA_NORM_H

#include <iostream>
#include <fstream>
#include <math.h>
#include <string.h>
#include <assert.h>
#include <vector>

#include "svType.h"
#include "svVectorMatrix.h"
#include "svArray.h"

#include "./jcUtil/MGL.h"
#include "./jcUtil/MArray.h"
#include "./jcUtil/MVectorMatrix.h"

using namespace __svl_lib;
using namespace std;

class QDOTdataNorm {
public:
  QDOTdataNorm(svChar *dir, svChar *inf);
  // read data

  QDOTdataNorm(svChar *dir, svChar *inf, int layers);
  // separate data by layers only

  QDOTdataNorm(svChar *dir, svChar *inf, int layers, int quantile);
  // separate data by layers and quatile
  QDOTdataNorm(svChar *dir, svChar *filename, svChar *outDir);
  virtual ~QDOTdataNorm();

  virtual void SaveToVTK(svChar *outDir);
  // save by layers and quantile data

  // virtual void SaveToVTK(svChar *outDir, double zmin, double zmax);
  // TODO: save by layers and quantile data; only save certain z values

  virtual void GetPosition();
  virtual void Print();

  int GetPowNum(svScalar numCopy);

  void SortData();
  void SetAverage();
  void SetStandard();
  void splitDen();

  void SetSamples();

  float DotProduct(svVector3 p1, svVector3 p2);
  void LineInterpolation(int s, int e, int &index1, int &index2, svVector3 p,
			float xd);
  int PlaneInterpolation(int s, svVector3 p, float yd);
  bool comparedistance(int index1, int index2, svVector3 p);

  svVector3Array * GetLayerDataPos(){return dataLayerPos;}
  svScalarArray *GetLayerDataDir(){return dataLayerDir;}
  int GetLayerNum(){return zlayernum;}
  svScalar GetLayerMaxData(){return dataLayerMax;}
  svScalar GetLayerMinData(){return dataLayerMin;}
  float Getxl(){return xl;}
  float Getyl(){return yl;}
  float Getxd(){return xd;}
  float Getyd(){return yd;}

  svVector3 *GetSamplePos(){return samplePos;}
  svVector3 *GetSampleVel(){return sampleVel;}
  svScalar *GetSampleDen(){return sampleDen;}
  int GetSampleNum(){return sampleNum;}
  svVector3 GetLeftBounding(){return lbbox;}
  svVector3 GetRightBounding(){return rbbox;}

protected:
  virtual void cleanData();
  virtual void cleanSplitData();

  virtual void readData();
  virtual void splitData();

  virtual void Find0Data();

private:
  svChar *inDir;     // input dir name
  svChar *inf;       // input file name

  svInt  layerSize;
  // the number of layers we are dividing the data
  // when BY_LAYER = false: layerSize=1;
  // otherwise, layerSize > 1;

  svInt quatileSize;
  // the number of quatiles we need to split the data
  // when BY_QUATILE=false; quatileSize=1;
  // otherwise, quatileSize = 2; // split by 0/0.5 and 0.25/0.75

  svVector3Array *dataPos; // sampling sites
  svVector3Array *dataVel; // velocity info at the sampling sites
  svScalarArray  dataDen; // added 3/15/14 for the additional scalar column
  
  svVector3Array *dataSplitPos; // store the data by layer and quatile
  svVector3Array *dataSplitVel; // store the velocity by layer and quatile
  svScalarArray *dataSplitDen;

  svVector3 *samplePos;
  svScalar *sampleDen;
  svVector3 *sampleVel;
  int sampleNum;

  svVector3Array *zerodataPos;
  svVector3Array *zerodataDir;
  svScalarArray *zerodataDen;

  svIntArray *splitMag;
  int splitdennum;


  svVector3 lbbox, rbbox; // left and right bounding box size
  svScalarArray zValue, denMinBox, denMaxBox; // get min/max density on each layer

   vector<svVector3> sortpos;
   vector<int> sortindex;
   float zdistance;

   vector<int> zindex;
   vector<bool> zformat;
   vector<int> yindex;
   vector<int> xindex;

   vector<float> ydistance;
   vector<float> xdistance;

   vector<int> yinz;
   vector<int> xinz;
   vector<int> xiny;
   
   int zlayernum; 
   svVector3 *xyplane;
   int xysample;
   svVector3 *yzplane;
   int yzsample;
   svVector3 *xzplane;
   int xzsample;

   svVector3Array *dataLayerPos;
   svIntArray *dataLayerIndex;
   svScalarArray *dataLayerDir; 
   float xl;
   float yl;
   float xd;
   float yd;
   svScalar dataLayerMax;
   svScalar dataLayerMin;



  bool BY_QUATILE;
  bool BY_LAYER;
};

#endif // _QDOT_DATA_NORM_H

