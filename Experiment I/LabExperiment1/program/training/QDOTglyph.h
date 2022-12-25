
/* 
 * Author: Jian Chen
 * Nov 30 2013
 * 
 * idea:
 */
#ifndef __QDOT_GLYPH_H
#define __QDOT_GLYPH_H

#ifdef __APPLE__
  #include <GLUT/glut.h>
#else
  #include <GL/glut.h>
#endif

#include <algorithm>
#include <vector>
#include <time.h>

#include "svType.h"
#include "svVectorMatrix.h"
#include "svArray.h"

#include "./jcUtil/MGL.h"
#include "./jcUtil/MArray.h"
#include "./jcUtil/MVectorMatrix.h"

//#include "QDOTPrimitive.h"
using namespace __svl_lib;
using namespace std;

class QDOTglyph{
public:
  QDOTglyph(){glyphArrowPos=NULL;glyphArrowNorm=NULL;glyphFrequency=1;NUM_SEG=10;minExp = 9e+9;
	maxExp = -9e+9;ni=-1;taskarrow[0]=-1;taskarrow[1]=-1;taskarrow_task4[0]=-1;taskarrow_task4[1]=-1;questionnum=0;}

 ~QDOTglyph();
  void cleanup();

  void SetColors(svVector3 *c);

  void Render();
  void RenderCylindernocolor();
  void RenderLines();
  void GenerateLines();
  void RenderMarker(GLfloat *m);
  void RenderTask4Marker(GLfloat *m);
  void RenderText( GLfloat *m);

  void RenderBounding();

  void RenderTask1(int num,int w,int h);
  void RenderTask2(int w, int h);
  void RenderTask3(int num1,int num2,int w,int h);
  void RenderTask4(int num1,int num2,int w,int h); 

  void RenderDistributeLegend();

  void RenderQuestionN(int w, int h);

  void SetData(svVector3 *pos, svVector3 *dir, svScalar *den, svVector3 *col,int num,
		svVector3 l, svVector3 r);
  void SetEndPos();
  void SetArrows();

  void SetMode(int n){ENCODE_MODE=n;}

  void ResetArrow(){taskarrow[0]=-1;taskarrow[1]=-1;
		taskarrow_task4[0]=-1;taskarrow_task4[1]=-1;correctflag = 0;}

  bool select_task2;//???
  int image_width;
  int image_height;

  void SetEye(svVector3 e){eye = e;}

  void SetTask1(int a){task1_index = a;}
  void SetTask2(int a, int b){task2_index[0]=a;task2_index[1]=b;}
  void SetTask3(int a, int b){task3_index[0]=a;task3_index[1]=b;}
  void SetTask4(int a, int b){task4_index[0]=a;task4_index[1]=b;}

  void SetTask4Neighbours(int *n1,int *n2);

  bool FindShot(double &x, double &y);

  int GetAnswers();

  int questionnum;

  void GetCorrect(int num,int task,char *name);
  float GetCorrectAnswer(int num1,int num2,int task);

  float GetTask3Den1(){return task3_den[0];}
  float GetTask3Den2(){return task3_den[1];}

protected:

  svVector3 *dataPos;
  svVector3 *dataEnd;
  svScalar *dataDen;
  svVector3 *dataDir;
  svVector3 *dataPoint;
  svVector3 *dataEnd2;
  svScalar *exp;
  svScalar *coe;  
  svVector3 *dataCol;
  
  int dataSize;

  svVector3Array  *glyphArrowPos;
  svVector3Array  *glyphArrowNorm;

  int correctflag;

  svVector3 lbbox;
  svVector3 rbbox;
 
  float minExp;
  float maxExp;

  int glyphFrequency;
  int NUM_SEG;

  int ENCODE_MODE;

  int taskarrow[2];
  int taskarrow_task4[2];

  int ni;

  float minl_distribute;
  float minl_direct;
  float minl_text;
  float minl_log;
  float arrow_radius;
  float arrow_length;

  svVector3 eye;
  vector<bool> ifshot;

  int shot_w;
  int shot_h;

  float correctanswer[64];

  int task1_index;
  int task2_index[2];
  int task3_index[2];
  int task4_index[2];

  float task3_den[2];

  int task4N1[12];
  int task4N2[12];
  int task4N1L;
  int task4N2L;

  int listIndex;

private:
}; // end class QDOTglyph


#endif //__QDOT_GLYPH_H
