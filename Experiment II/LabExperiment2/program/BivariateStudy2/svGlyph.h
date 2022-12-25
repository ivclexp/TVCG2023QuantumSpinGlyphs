
/* 
 * Author: Jian Chen
 * Nov 30 2013
 * 
 * idea:
 */
#ifndef __SV_GLYPH_H
#define __SV_GLYPH_H

#ifdef __APPLE__
  #include <GLUT/glut.h>
#else
  #include <GL/glut.h>
#endif

#include <algorithm>
#include <vector>
#include <time.h>

//#include "svType.h"
//#include "svVectorMatrix.h"
//#include "svArray.h"
#include "QDOTglyph.h"
#include "./jcUtil/MGL.h"
#include "./jcUtil/MArray.h"
#include "./jcUtil/MVectorMatrix.h"

//#include "QDOTPrimitive.h"
using namespace __svl_lib;
using namespace std;

class svGlyph{
public:
  svGlyph(){}
  ~ svGlyph(){}
void makeTexture(svVector4 c);
void inittexture();

	svVector3 svGetPerpendicularVector(const svVector3 & v);
	svVector3 svGetRotatePoint(const svVector3& start, const svVector3& org, 
			const svVector3& axis, svScalar rotate_degree);
	svVector3 svGetNorm(const svVector3& v1, const svVector3& v0,
                    const svVector3& v2);
	svVector3 svAverage(const svVector3& v1, const svVector3& v2);

	svVector3 GetVertical(svVector3 pos, svVector3 v, GLfloat *tm);

  void RenderBounding(svVector3 lbbox, svVector3 rbbox);

  void Draw_2DFishBone(svVector3 pos, svVector3 vel, svScalar length);

  void Draw_InnerLegend(svVector3 pos, svVector3 vel, svScalar l,svScalar length, GLfloat *tm);
  void Draw_WidthLegend(svVector3 pos, svVector3 vel, svScalar l,svScalar length, GLfloat *tm);
  void Draw_Cylinder(svVector3 pos, svVector3 vel,
				svScalar l,svScalar w,
			      svScalar length,
			      svScalar radius,
			      svVector4 col,
			      bool base, bool light, GLfloat *tm);
  void Draw_Cylinder_Bottom(svVector3 pos, svVector3 vel,
				svScalar l,svScalar w,
			      svScalar length,
			      svScalar radius,
			      svVector4 col);
  void Draw_Cylinder_withtexture(svVector3 pos, svVector3 vel,
				svScalar l,svScalar n,
			      svScalar length,
			      svScalar radius,
			      svVector4 col, svVector4 texture_col, svScalar max, GLfloat *tm);
  void Draw_Cylinder_withtexture_horizontal(svVector3 pos, svVector3 vel,
				svScalar l,svScalar n,
			      svScalar length,
			      svScalar radius,
			      svVector4 col);
  void Draw_Arrow(svVector3 pos, svVector3 vel,
			      svScalar length,
			      svScalar radius,
			      svScalar top_length,
			      svVector4 col);
  void Draw_Shape(svVector3 pos, svVector3 vel,
			      svScalar A,
			      svScalar r,
			      svVector4 col,
				bool light);
	//void SetTransform(GLfloat *t, int n);

	
	GLboolean invert_matrix( const GLfloat *m, GLfloat *out );



}; // end class QDOTglyph


#endif //__QDOT_GLYPH_H
