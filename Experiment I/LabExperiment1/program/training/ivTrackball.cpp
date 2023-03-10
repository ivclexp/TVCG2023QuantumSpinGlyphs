
#include "ivTrackball.h"

#include <iostream>
#include <fstream>

#ifndef PI
#define PI		Scalar(3.14159265358979323846)
#endif

#define to_degree(x)	((x)*180.0/PI)
#define to_radian(x)	((x)*PI/180.0)

#define DEFAULT_ROTATE_SCALE  0.2F
#define DEFAULT_ZOOM_SCALE    0.05F
#define DEFAULT_DOLLY_SCALE   0.05F
#define DEFAULT_PAN_SCALE     0.01F

//
//  Trackball methods
//

#define M(row,col)  m[col*4+row]

using namespace std;



/*  This function is based on Erich Boleyn (erich@uruk.org) 
 *
 *  Arbitrary axis rotation matrix.
 *
 *  This is composed of 5 matrices, Rz, Ry, T, Ry', Rz', multiplied
 *  like so:  Rz * Ry * T * Ry' * Rz'.  T is the final rotation
 *  (which is about the X-axis), and the two composite transforms
 *  Ry' * Rz' and Rz * Ry are (respectively) the rotations necessary
 *  from the arbitrary axis to the X-axis then back.  They are
 *  all elementary rotations.
 */


void rotate_matrix(Scalar theta, Scalar x, Scalar y, Scalar z, Scalar m[])
{
	Scalar xx, yy, zz, xy, yz, zx, xs, ys, zs, one_c;
	Scalar len = Scalar(sqrt(x*x + y*y +z*z));

	if (len == 0.0) {
		M(0,0) = 1.0f;  M(0,1) = 0.0f;  M(0,2) = 0.0f;  M(0,3) = 0.0f;
		M(1,0) = 0.0f;  M(1,1) = 1.0f;  M(1,2) = 0.0f;  M(1,3) = 0.0f;
		M(2,0) = 0.0f;  M(2,1) = 0.0f;  M(2,2) = 1.0f;  M(2,3) = 0.0f;
		M(3,0) = 0.0f;  M(3,1) = 0.0f;  M(3,2) = 0.0f;  M(3,3) = 1.0f;
		return;
	}

	x /= len;  y /= len;  z /= len;

	Scalar s = Scalar(sin(to_radian(theta)));
	Scalar c = Scalar(cos(to_radian(theta)));

	xx = x * x;   yy = y * y;   zz = z * z;
	xy = x * y;   yz = y * z;   zx = z * x;
	xs = x * s;   ys = y * s;   zs = z * s;
	one_c = 1.0f - c;

	M(0,0) = (one_c * xx) + c;   M(0,1) = (one_c * xy) - zs;  M(0,2) = (one_c * zx) + ys;  M(0,3) = 0.0f;
	M(1,0) = (one_c * xy) + zs;  M(1,1) = (one_c * yy) + c;   M(1,2) = (one_c * yz) - xs;  M(1,3) = 0.0f;
	M(2,0) = (one_c * zx) - ys;  M(2,1) = (one_c * yz) + xs;  M(2,2) = (one_c * zz) + c;   M(2,3) = 0.0f;
	M(3,0) = 0.0f;               M(3,1) = 0.0f;               M(3,2) = 0.0f;               M(3,3) = 1.0f;
}


Trackball::Trackball() :
	behavior(ALL),
    action(_NONE),
	rotate_scale(DEFAULT_ROTATE_SCALE),
	zoom_scale(DEFAULT_ZOOM_SCALE),
	pan_scale(DEFAULT_PAN_SCALE),
	dolly_scale(DEFAULT_DOLLY_SCALE),
	window_width(512),
	window_height(512)
{
	// the default attach mode

    attach(PAN,    LBUTTON_DOWN | SHIFT_DOWN);
    attach(DOLLY,   LBUTTON_DOWN | CTRL_DOWN);
    attach(ZOOM,  RBUTTON_DOWN );
    attach(ROTATE, LBUTTON_DOWN);

    rotatefactor[0]=0;
    rotatefactor[1]=0;
    rotatefactor[2]=0;
    rotatefactor[3]=0;

    zoomfactor=0;

    panfactor[0]=0;
    panfactor[1]=0;
}

void Trackball::reset()
{
	mouse_mat.identity();
	zoom_mat.identity();
	action = _NONE;

    rotatefactor[0]=0;
    rotatefactor[1]=0;
    rotatefactor[2]=0;
    rotatefactor[3]=0;

    zoomfactor=0;

    panfactor[0]=0;
    panfactor[1]=0;
}
void Trackball::setx()
{
	Scalar m1[16];
	m1[0]=1;m1[4]=0;m1[8]=0;m1[12]=0;
	m1[1]=0;m1[5]=0;m1[9]=1;m1[13]=0;
	m1[2]=0;m1[6]=-1;m1[10]=0;m1[14]=0;
	m1[3]=0;m1[7]=0;m1[11]=0;m1[15]=1;
	Scalar m2[16];
	m2[0]=-1;m2[4]=0;m2[8]=0;m2[12]=0;
	m2[1]=0;m2[5]=1;m2[9]=0;m2[13]=0;
	m2[2]=0;m2[6]=0;m2[10]=-1;m2[14]=0;
	m2[3]=0;m2[7]=0;m2[11]=0;m2[15]=1;
	Scalar m3[16];
	m3[0]=0;m3[4]=0;m3[8]=1;m3[12]=0;
	m3[1]=0;m3[5]=1;m3[9]=0;m3[13]=0;
	m3[2]=-1;m3[6]=0;m3[10]=0;m3[14]=0;
	m3[3]=0;m3[7]=0;m3[11]=0;m3[15]=1;
	Matrix4f M1(m1);
	Matrix4f M2(m2);
	Matrix4f M3(m3);
	mouse_mat = M3*M2*M1;
	action = _NONE;
}
void Trackball::sety()
{
	Scalar m1[16];
	m1[0]=1;m1[4]=0;m1[8]=0;m1[12]=0;
	m1[1]=0;m1[5]=0;m1[9]=1;m1[13]=0;
	m1[2]=0;m1[6]=-1;m1[10]=0;m1[14]=0;
	m1[3]=0;m1[7]=0;m1[11]=0;m1[15]=1;
	Scalar m2[16];
	m2[0]=-1;m2[4]=0;m2[8]=0;m2[12]=0;
	m2[1]=0;m2[5]=1;m2[9]=0;m2[13]=0;
	m2[2]=0;m2[6]=0;m2[10]=-1;m2[14]=0;
	m2[3]=0;m2[7]=0;m2[11]=0;m2[15]=1;
	Matrix4f M1(m1);
	Matrix4f M2(m2);
	mouse_mat = M2*M1;
	action = _NONE;
}
void Trackball::attach(int behavior, int mouse_key_state)
{
	if (behavior == PAN) {
		action_tab[_PAN] = mouse_key_state;
	}
	else if (behavior == DOLLY) {
		action_tab[_DOLLY] = mouse_key_state;
	}
	else if (behavior == ROTATE) {
		action_tab[_ROTATE] = mouse_key_state;
	}
	else if (behavior == ZOOM) {
		action_tab[_ZOOM] = mouse_key_state;
	}
}

void  Trackball::mouseDown(int state, int x, int y)
{
	start_x = x;
	start_y = y;
	start = trackballMapping(x, y);
	
	time(&nowtime);
	char str[100];

	if (behavior & ROTATE && action_tab[_ROTATE] == state) {
		action = _ROTATE;
		/*std::ofstream log(namelog, std::ios_base::app | std::ios_base::out);
		sprintf (str, "%s ROTATE START", ctime (&nowtime));
		log<<str<<endl;
		log.close();*/
	}
	else if (behavior & PAN && action_tab[_PAN] == state) {
		action = _PAN;
		/*std::ofstream log(namelog, std::ios_base::app | std::ios_base::out);
		sprintf (str, "%s PAN START", ctime (&nowtime));
		log<<str<<endl;
		log.close();*/
	}
	else if (behavior & DOLLY && action_tab[_DOLLY] == state) {
		action = _DOLLY;
		/*std::ofstream log(namelog, std::ios_base::app | std::ios_base::out);
		sprintf (str, "%s DOLLY START", ctime (&nowtime));
		log<<str<<endl;
		log.close();*/
	}
	else if (behavior & ZOOM && action_tab[_ZOOM] == state) {
		action = _ZOOM;
		/*std::ofstream log(namelog, std::ios_base::app | std::ios_base::out);
		sprintf (str, "%s ZOOM START", ctime (&nowtime));
		log<<str<<endl;
		log.close();*/
	}
	else {
		action = _NONE;
	}
}

void  Trackball::mouseUp(int state, int x, int y)
{
	char str[100];
    if (state & BUTTON_UP) {

		action = _NONE;
		/*std::ofstream log(namelog, std::ios_base::app | std::ios_base::out);
		sprintf (str, "%s MOUSE UP", ctime (&nowtime));
		log<<str<<endl;
		log.close();*/
	}
}

void  Trackball::mouseMotion(int x, int y)
{
	char str[100];
	char value[100];
	switch(action) {
	case _ROTATE:
	{
		//
		// Rotate around the axis that is perpendicular to and centers at the great circle connecting 
		// the two mouse positions. We use the two points on the great circle to calculate the axis.
		// Then project the axis on x-y plane as the rotation axis.
		//

		// calculate the axis for the great circle
		Vector3f end = trackballMapping(x, y);
		Vector3f axis = cross(start, end);

		// project the axis on x-y plane
		axis[2] = 0.0f;  

		Scalar span = Scalar(sqrt((x-start_x)*(x-start_x) + (y-start_y)*(y-start_y)));
		Scalar rot_angle = span * rotate_scale;
		Scalar m[16];
		rotate_matrix(rot_angle, axis[0], axis[1], axis[2], m);
		Matrix4f rotate(m);
		rotate_single = rotate;
		//cerr<<m[0]<<" "<<m[1]<<" "<<m[2]<<" "<<m[3]<<endl;

		rotate_mat = rotate  *  rotate_mat;
		mouse_mat = focus2world * rotate  * world2focus * mouse_mat;
		start = end;

		rotatefactor[0] = rot_angle*180./PI;
		rotatefactor[1] = axis[0];
		rotatefactor[2] = axis[1];
		rotatefactor[3] = axis[2];

		/*std::ofstream log(namelog, std::ios_base::app | std::ios_base::out);
		sprintf (value, "angle: %f axis: (%f %f %f)",rot_angle*180./PI,axis[0],axis[1],axis[2]);
		sprintf (str, "%s ROTATE %s", ctime (&nowtime),value);
		log<<str<<endl;
		log.close();*/
		break;
	}
	case _ZOOM:
	{
		Scalar zoom = 1.0f + (start_y - y) * zoom_scale;
		if (zoom < 0.0001f)   zoom = 0.0001f;
		Matrix4f zoom_mat;
		zoom_mat[0][0] = zoom;
		zoom_mat[1][1] = zoom;
		zoom_mat[2][2] = zoom;
		mouse_mat = focus2world * zoom_mat * world2focus * mouse_mat;
		
		zoomfactor = zoom;
		/*std::ofstream log(namelog, std::ios_base::app | std::ios_base::out);
		sprintf (value, "zoom: %f",zoom);
		sprintf (str, "%s ZOOM %s", ctime (&nowtime),value);
		log<<str<<endl;
		log.close();*/
		break;
	}
	case _PAN:
	{
		Scalar pan_x = (x - start_x) * pan_scale;
		Scalar pan_y = (start_y - y) * pan_scale;
		Matrix4f pan_mat;
		pan_mat[0][3] = pan_x;
		pan_mat[1][3] = pan_y;
		mouse_mat = pan_mat * mouse_mat;
		pan_zoom_mat = pan_mat * pan_zoom_mat;
		pan_mat2 = pan_mat*pan_mat2;

		world2focus[0][3] -= pan_x;
		world2focus[1][3] -= pan_y;

		focus2world[0][3] += pan_x;
		focus2world[1][3] += pan_y;

		panfactor[0] = pan_x;
		panfactor[1] = pan_y;

		/*std::ofstream log(namelog, std::ios_base::app | std::ios_base::out);
		sprintf (value, "x: %f y: %f",pan_x,pan_y);
		sprintf (str, "%s PAN %s", ctime (&nowtime),value);
		log<<str<<endl;
		log.close();*/
		break;
	}
	case _DOLLY:
	{
		Scalar dolly_z = (start_y - y) * dolly_scale;
		Matrix4f dolly_mat;
		dolly_mat[2][3] = dolly_z;
		mouse_mat = dolly_mat * mouse_mat;

		pan_zoom_mat = dolly_mat * pan_zoom_mat;
		zoom_mat = dolly_mat * zoom_mat;

		world2focus[2][3] -= dolly_z;
		focus2world[2][3] += dolly_z;
		/*std::ofstream log(namelog, std::ios_base::app | std::ios_base::out);
		sprintf (value, "dolly: %f",dolly_z);
		sprintf (str, "%s DOLLY %s", ctime (&nowtime),value);
		log<<str<<endl;
		log.close();*/
		break;
	}
	default:
		break;
	}

	start_x = x;
	start_y = y;
}

Vector3f Trackball::trackballMapping(int x, int y)
{
	Vector3f v;
	float d;

	//
	// The point on the application window (in image space) can be viewed as the parallel projection of the 
	// point on the hemisphere center at eye.  We map it back on the sphere to get its 3D position on the sphere.
	//

	v[0] = (2.0f*x - window_width)  / window_width;
	v[1] = (window_height - 2.0f*y) / window_height;
	v[2] = 0.0f;
	
	d = v.length();
	d = (d < 1.0f) ? d : 1.0f;     
	v[2] = Scalar(sqrt(1.01f - d*d));

	v.normalize();  
	return v;
}

void Trackball::setEye(const Vector3f & e)
{  
	eye = e;  
}

void Trackball::setFocus(const Vector3f & f)
{  
	focus = f; 
	world2focus.setColVector(3, -focus);  
	focus2world.setColVector(3, focus);  
}



#undef M

