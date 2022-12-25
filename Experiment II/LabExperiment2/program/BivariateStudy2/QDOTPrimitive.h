
/**
 * Author: Jian Chen
 * Date: Nov 30, 2013
 * Description: base class, QDOTPrimitive
 */

#ifndef  __QDOT_PRIMITIVE_H
#define  __QDOT_PRIMITIVE_H

#include "QDOTdata.h"

namespace __qdot_lib{

typedef struct _RenderProperty {
  svColor4 color;
  svFloat line_width;
} RenderProperty;

class QDOTPrimitive
{
public:
  QDOTPrimitive();
  QDOTPrimitive(QDOTdata *data);
  virtual ~QDOTPrimitive();

  //---------------------------------------------
  virtual void Render()=0;
  virtual void Generate()=0;

  QDOTdata *GetField()
  {
    return field;
  }

  void SetField(QDOTdata * d)
  {
    field = d;
  }

protected:
  svUint  display_list;  // OpenGL display list

private:
  QDOTdata *field; // the field to be processed 

  //--------------------------------------------
  RenderProperty render_property;
  //svLut   lut; // color lookup table
  bool  ENABLE_COLOR;

}; // end class 

}

#endif //__QDOT_PRIMITIVE_H
