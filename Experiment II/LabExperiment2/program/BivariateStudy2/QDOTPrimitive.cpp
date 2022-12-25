
/**
 * Author: Jian Chen
 * Date: Nov 30, 2013
 * Description: base class, QDOTPrimitive
 */

#ifdef __APPLE__
  #include <GLUT/glut.h>
#else
  #include <GL/glut.h>
#endif

#include "QDOTPrimitive.h"

namespace __qdot_lib {

QDOTPrimitive::QDOTPrimitive()
{
  field=NULL;

  svColor4 color(0.1, 0.1, 0.9, 0.5);
  render_property.color = color;
  render_property.line_width = 1.;

  ENABLE_COLOR=false;
}

QDOTPrimitive::QDOTPrimitive(QDOTdata *data)
{
  field = data;
}

QDOTPrimitive::~QDOTPrimitive()
{
}

} // end namespace
