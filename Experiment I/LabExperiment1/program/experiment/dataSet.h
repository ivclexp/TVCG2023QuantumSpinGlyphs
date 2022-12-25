/**------------------------------------------------------------------
 Author: Jian Chen (jichen@umbc.edu) 
 Description: NIST QDOT data parser
 Data: Nov 2013
 Version 1.0:
---------------------------------------------------------------------
*/

#ifndef __DATA_SET_H
#define __DATA_SET_H

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

class dataSet {
public:
	dataSet(){dataSize=0;}
	dataSet(svChar *dir, svChar *inf);
	void SetData(svChar *dir, svChar *inf);
	void SetTask1(svChar *dir, svChar *inf);
	void SetTask2(svChar *dir, svChar *inf);
	void SetTask3(svChar *dir, svChar *inf);
	void SetTask4(svChar *dir, svChar *inf);
	~dataSet();
	
	void readColorFile(svChar *dir, svChar *inf);

	svVector3 * GetPos(){return dataPos;}
	svVector3 * GetDir(){return dataDir;}
	svScalar * GetDen(){return dataDen;}
	svVector3 *GetCol(){return dataCol;}
	svInt GetNum(){return dataSize;}
	svVector3 GetLeftBounding(){return lbbox;}
	svVector3 GetRightBounding(){return rbbox;}

	int GetTask1(int n){return Task1Point[n];}
	int GetTask21(int n){return Task2Point1[n];}
	int GetTask22(int n){return Task2Point2[n];}
	int GetTask31(int n){return Task3Point1[n];}
	int GetTask32(int n){return Task3Point2[n];}
	int GetTask41(int n){return Task4Point1[n];}
	int GetTask42(int n){return Task4Point2[n];}
	void GetTask4n1(int n, int *nn){for(int i=0;i<12;i++)nn[i]=Task4Neighbours1[n][i];}
	void GetTask4n2(int n, int *nn){for(int i=0;i<12;i++)nn[i]=Task4Neighbours2[n][i];}

	void cleanup();
private:
	svVector3 *dataPos;
	svVector3 *dataDir;	
	svScalar *dataDen;
	svVector3 *dataCol;
	svInt dataSize;

	int Task1Point[32];
	int Task2Point1[32];
	int Task2Point2[32];
	int Task3Point1[32];
	int Task3Point2[32];
	int Task4Point1[32];
	int Task4Point2[32];
	int Task4Neighbours1[32][12];
	int Task4Neighbours2[32][12];

	svVector3 lbbox;
	svVector3 rbbox;
};

#endif // _DATA_SET_H

