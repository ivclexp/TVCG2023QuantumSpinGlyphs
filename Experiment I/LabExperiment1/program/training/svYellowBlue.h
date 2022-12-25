
#ifndef __SV_YELLOW_BLUE_H
#define __SV_YELLOW_BLUE_H


#include <iostream>
#include <fstream>
#include <math.h>
#include <string.h>
#include <assert.h>
#include <vector>
#include <algorithm>

#include "svType.h"
#include "svVectorMatrix.h"
#include "svArray.h"


#include "./jcUtil/MGL.h"
#include "./jcUtil/MArray.h"
#include "./jcUtil/MVectorMatrix.h"
using namespace std;

using namespace __svl_lib;

/* categorical color scale based on input data */
class svYellowBlue
{
  public: 
    svYellowBlue(svScalar *data, int dataSize);
    ~svYellowBlue();
	void RenderLegend();
	svVector4 *GetColors(){return colors;}

  protected:
	void init();
	int  getNumOfIntegerDigits(float num);

    	void cleanUp();

	svVector4Array  *brewerColors;

  private:
    	svVector4 *colors;
	int        colorNumOfCategories;
	int legendmax,legendmin,legendnum;
};

#endif //__SV_YELLOW_BLUE_H
