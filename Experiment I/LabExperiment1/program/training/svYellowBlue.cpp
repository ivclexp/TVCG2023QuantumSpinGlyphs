
/* 
 * svYellowBlue.cpp
 * 
 */

#include "svYellowBlue.h"
#include <iostream>
#include <fstream>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif

#include "./jcUtil/MGL.h"

using namespace std;

svYellowBlue::svYellowBlue(svScalar *data, int dataSize)
{
  colors = new svVector4[dataSize];

  float minData=data[0], maxData=data[0];
  init();

  // get the data range
  for(int di=1; di<dataSize; di++)
  {
     if(data[di] < minData&&data[di]!=0)
	   minData = data[di];
	 if(data[di] > maxData)
	   maxData = data[di];
  }; // end for(di)

  if(minData>=1)
  {
	  // get the data range
	  for(int di=1; di<dataSize; di++)
	  {
	     if(data[di]==0)
		   minData = 0;
	  }; // end for(di)
  }

  cerr << "...  minData = " << minData;
  cerr << "...  maxData = " << maxData;

  // choose the # of categories
  int minDigit = getNumOfIntegerDigits(minData);
  int maxDigit = getNumOfIntegerDigits(maxData);

  legendmin = minDigit;
  legendmax = maxDigit;

  colorNumOfCategories = maxDigit-minDigit+1;
  cerr << "...  minDigit = " << minDigit;
  cerr << "...  maxDigit = " << maxDigit;
  cerr << "...  colorNumOfCategories = " << colorNumOfCategories << endl;

  // set the color values
  colors = new svVector4[dataSize];

  if(colorNumOfCategories>=4&&colorNumOfCategories<=12)
  {
	colorNumOfCategories = colorNumOfCategories-4;
  }
  else if(colorNumOfCategories<4)
  {
	colorNumOfCategories = 0;
  }
  else if(colorNumOfCategories>12)
  {
	colorNumOfCategories = 8;
  }

  int numofinteger;
  for(int di=0; di<dataSize; di++)
  {
	numofinteger = maxDigit - getNumOfIntegerDigits(data[di]);
	if(numofinteger>colorNumOfCategories+3)
		colors[di] = brewerColors[colorNumOfCategories][colorNumOfCategories+4-1];
	else
		colors[di] = brewerColors[colorNumOfCategories][numofinteger];
    //colors[di] = brewerColors[colorNumOfCategories-4][getNumOfIntegerDigits(data[di])-minDigit];

  }; // end for(di)
  legendnum = colorNumOfCategories;
}

svYellowBlue::~svYellowBlue()
{
  cleanUp();
}

void svYellowBlue::init()
{
  int bcolors = 9;
  brewerColors = new svVector4Array[bcolors];

  //12 colors
  brewerColors[8].add(svVector4(255, 255, 217, 255));
  brewerColors[8].add(svVector4(237, 248, 177, 255));
  brewerColors[8].add(svVector4(199, 233, 180, 255));
  brewerColors[8].add(svVector4(127, 205, 187, 255));
  brewerColors[8].add(svVector4(65, 182, 196, 255));
  brewerColors[8].add(svVector4(29, 145, 192, 255));
  brewerColors[8].add(svVector4(34, 94, 168, 255));
  brewerColors[8].add(svVector4(37, 52, 148, 255));
  brewerColors[8].add(svVector4(8, 29, 88, 255));
  brewerColors[8].add(svVector4(6, 23, 72, 255));
  brewerColors[8].add(svVector4(4, 15, 45, 255));
  brewerColors[8].add(svVector4(1, 3, 10, 255));

  //11 colors
  brewerColors[7].add(svVector4(255, 255, 217, 255));
  brewerColors[7].add(svVector4(237, 248, 177, 255));
  brewerColors[7].add(svVector4(199, 233, 180, 255));
  brewerColors[7].add(svVector4(127, 205, 187, 255));
  brewerColors[7].add(svVector4(65, 182, 196, 255));
  brewerColors[7].add(svVector4(29, 145, 192, 255));
  brewerColors[7].add(svVector4(34, 94, 168, 255));
  brewerColors[7].add(svVector4(37, 52, 148, 255));
  brewerColors[7].add(svVector4(8, 29, 88, 255));
  brewerColors[7].add(svVector4(6, 23, 72, 255));
  brewerColors[7].add(svVector4(4, 15, 45, 255));

  //10 colors
  brewerColors[6].add(svVector4(255, 255, 217, 255));
  brewerColors[6].add(svVector4(237, 248, 177, 255));
  brewerColors[6].add(svVector4(199, 233, 180, 255));
  brewerColors[6].add(svVector4(127, 205, 187, 255));
  brewerColors[6].add(svVector4(65, 182, 196, 255));
  brewerColors[6].add(svVector4(29, 145, 192, 255));
  brewerColors[6].add(svVector4(34, 94, 168, 255));
  brewerColors[6].add(svVector4(37, 52, 148, 255));
  brewerColors[6].add(svVector4(8, 29, 88, 255));
  brewerColors[6].add(svVector4(6, 23, 72, 255));

  //9 colors
  brewerColors[5].add(svVector4(255, 255, 217, 255));
  brewerColors[5].add(svVector4(237, 248, 177, 255));
  brewerColors[5].add(svVector4(199, 233, 180, 255));
  brewerColors[5].add(svVector4(127, 205, 187, 255));
  brewerColors[5].add(svVector4(65, 182, 196, 255));
  brewerColors[5].add(svVector4(29, 145, 192, 255));
  brewerColors[5].add(svVector4(34, 94, 168, 255));
  brewerColors[5].add(svVector4(37, 52, 148, 255));
  brewerColors[5].add(svVector4(8, 29, 88, 255));

  // 8 colors
  brewerColors[4].add(svVector4(255, 255, 217, 255));
  brewerColors[4].add(svVector4(237, 248, 177, 255));
  brewerColors[4].add(svVector4(199, 233, 180, 255));
  brewerColors[4].add(svVector4(127, 205, 187, 255));
  brewerColors[4].add(svVector4(65, 182, 196, 255));
  brewerColors[4].add(svVector4(29, 145, 192, 255));
  brewerColors[4].add(svVector4(34, 94, 168, 255));
  brewerColors[4].add(svVector4(12, 44, 132, 255));

  // 7 colors
  brewerColors[3].add(svVector4(255, 255, 204, 255));
  brewerColors[3].add(svVector4(199, 233, 180, 255));
  brewerColors[3].add(svVector4(127, 205, 187, 255));
  brewerColors[3].add(svVector4(65, 182, 196, 255));
  brewerColors[3].add(svVector4(29, 145, 192, 255));
  brewerColors[3].add(svVector4(34, 94, 168, 255));
  brewerColors[3].add(svVector4(12, 44, 132, 255));

  // 6 colors
  brewerColors[2].add(svVector4(255, 255, 204, 255));
  brewerColors[2].add(svVector4(199, 233, 180, 255));
  brewerColors[2].add(svVector4(127, 205, 187, 255));
  brewerColors[2].add(svVector4(65, 182, 196, 255));
  brewerColors[2].add(svVector4(44, 127, 184, 255));
  brewerColors[2].add(svVector4(37, 52, 148, 255));

  // 5 colors
  brewerColors[1].add(svVector4(255, 255, 204, 255));
  brewerColors[1].add(svVector4(161, 218, 180, 255));
  brewerColors[1].add(svVector4(65, 182, 196, 255));
  brewerColors[1].add(svVector4(44, 127, 184, 255));
  brewerColors[1].add(svVector4(37, 52, 148, 255));

  // 4 colors
  brewerColors[0].add(svVector4(255, 255, 204, 255));
  brewerColors[0].add(svVector4(161, 218, 180, 255));
  brewerColors[0].add(svVector4(65, 182, 196, 255));
  brewerColors[0].add(svVector4(34, 94, 168, 255));
}

int svYellowBlue::getNumOfIntegerDigits(float num)
{
  // assuming the data is not between 10^-12, 10^20
  int lowerBound = -12, upperBound = 20;
  int multiplier = 0;

  float numCopy = num;

  if(numCopy<1&&numCopy!=0) 
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
	multiplier=0;
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

void svYellowBlue::RenderLegend()
{
    GLdouble vmat[16], pmat[16], sx, sy, sz;
    GLint    viewport[4];


    glGetDoublev(GL_MODELVIEW_MATRIX, vmat);
    glGetDoublev(GL_PROJECTION_MATRIX, pmat);
    glGetIntegerv(GL_VIEWPORT, viewport);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0,(GLfloat) viewport[2], 0.0, (GLfloat) viewport[3]);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

  char str[10];
  float x = 150.;
  float y = viewport[3]/2.;
  float size = 30.;	
 
  int j = 0;
  int num = legendmax-legendmin+1 - (legendnum+4);
  for(int i=legendmax-legendmin;i>=0;i--)
  {
	glColor4f(brewerColors[legendnum][j][0]/255.,
		brewerColors[legendnum][j][1]/255.,
		brewerColors[legendnum][j][2]/255.,
		brewerColors[legendnum][j][3]/255.);
	if(i>num) j ++;
	glBegin(GL_QUADS);
	glVertex2f(x,y);
	glVertex2f(x,y-size);
	glVertex2f(x-size/2.,y-size);
	glVertex2f(x-size/2.,y);
	glEnd();
	glColor4f(1,1,1,1);
	glRasterPos2f(x,y-size/2.);
	if(legendmin+i-12<0)
		sprintf(str,"1e%-d",(legendmin+i-12));
	else
		sprintf(str,"1e%d",(legendmin+i-12));
	for(int ii=0;ii<strlen(str);ii++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[ii]);
	y = y - size;

  }


    glMatrixMode(GL_PROJECTION);
    glLoadMatrixd(pmat);
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixd(vmat);
}

void svYellowBlue::cleanUp()
{
  if(brewerColors != NULL)
  {
    for(int i=0; i<6; i++)
      brewerColors[i].free();
  }
  delete [] brewerColors;
  delete [] colors;
}

