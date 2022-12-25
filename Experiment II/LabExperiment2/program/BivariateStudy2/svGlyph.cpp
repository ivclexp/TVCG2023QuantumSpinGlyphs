#include "svGlyph.h"


#define SEGMENT_NUM 20

#define SWAP(T, a, b)   { T t; t=a; a=b; b=t;  }

#define SV_PI	  (3.14159265358979323846)
#define SV_2_PI   (3.14159265358979323846 * 2.0)
#define SV_SMALL  (1e-6)

#define svToDegree(x)             ((x)*(180.)/SV_PI)
#define svToRadian(x)	           ((x)*SV_PI/(180.))
#define svMax(a,b)                (((a) > (b)) ? (a) : (b))
#define svMin(a,b)                (((a) < (b)) ? (a) : (b))
#define svClamp(v, min_v, max_v)  { v = svMax(svMin(v, max_v), min_v); }

#define checkImageWidth 20
#define checkImageHeight 20
static GLubyte checkImage[checkImageHeight][checkImageWidth][4];
	static GLuint texName;

	
template <class T> inline
  void svSwap(T & a, T & b)
{  T t = a; a = b; b = t;  }

svVector3 svGlyph::svGetPerpendicularVector(const svVector3 & v)
{
	svVector3 r;
	if (fabs(v[2]) < 0.1){
		r[0] = v[1];
		r[1] = -v[0];
	}
	else if (fabs(v[1]) < 0.1){
		r[0] = v[2];
		r[2] = -v[0];
	}
	else{
		r[1] = v[2];
		r[2] = -v[1];
	}

	return normalize(cross(v, r));
}


svVector3 svGlyph::svGetRotatePoint(const svVector3& start, const svVector3& org, 
			const svVector3& axis, svScalar rotate_degree)
{
   svVector3 p, q, r;
   svScalar theta = svToRadian(rotate_degree);
   svScalar costheta,sintheta;

   // calculate the roatation axis
   r = normalize(axis);
   p = start - org;

   //cerr << "r: "; r.print();
   //cerr << "p: "; p.print();

   costheta = svScalar(cos(theta));
   sintheta = svScalar(sin(theta));

   q[0] += (costheta + (1 - costheta) * r[0] * r[0]) * p[0];
   q[0] += ((1 - costheta) * r[0] * r[1] - r[2] * sintheta) * p[1];
   q[0] += ((1 - costheta) * r[0] * r[2] + r[1] * sintheta) * p[2];

   q[1] += ((1 - costheta) * r[0] * r[1] + r[2] * sintheta) * p[0];
   q[1] += (costheta + (1 - costheta) * r[1] * r[1]) * p[1];
   q[1] += ((1 - costheta) * r[1] * r[2] - r[0] * sintheta) * p[2];

   q[2] += ((1 - costheta) * r[0] * r[2] - r[1] * sintheta) * p[0];
   q[2] += ((1 - costheta) * r[1] * r[2] + r[0] * sintheta) * p[1];
   q[2] += (costheta + (1 - costheta) * r[2] * r[2]) * p[2];
   //cerr << "q: "; q.print();

   q += org;
   //cerr << "org: "; org.print();
   //cerr << "q: "; q.print();
   return q;
}


svVector3 svGlyph::svGetNorm(const svVector3& v1, const svVector3& v0,
                    const svVector3& v2)
{
  return normalize(cross((v1-v0), (v2-v0)));
}

svVector3 svGlyph::svAverage(const svVector3& v1, const svVector3& v2)
{
  return normalize(v1+v2);
}

double function_c(double omiga, double m)
{
  double cos_omiga = cos(omiga);
  double sgn_cos_omiga;
  
  if(cos_omiga < 0)
  {
    sgn_cos_omiga = -1;
  }
  else if(fabs(cos_omiga) < 0.00001)
  {
    sgn_cos_omiga = 0;
  }
  else
  {
    sgn_cos_omiga = 1;
  }
  
 
  

  double c = sgn_cos_omiga * pow( fabs(cos_omiga), m);
  return c;
}

double function_s(double omiga, double m)
{
  double sin_omiga = sin(omiga);
  double sgn_sin_omiga;

  
  if(sin_omiga < 0)
  {
    sgn_sin_omiga = -1;
  } 
  else if(fabs(sin_omiga) < 0.00001)
  {
    sgn_sin_omiga = 0;
  }
  else
  {
    sgn_sin_omiga = 1;
  }
  

  double s = sgn_sin_omiga * pow( fabs(sin_omiga), m);
  
  return s;
}


svVector3 svGlyph::GetVertical(svVector3 pos, svVector3 v, GLfloat *tm)
{

    GLfloat *invert_tb= new GLfloat[16];
    svVector3 p,p2;
    svVector3 eye;
    eye[0]=0;eye[1]=0;eye[2]=5.5;

    svVector3 y;
    y[0]=0;y[1]=1;y[2]=0;
    svVector3 x;
    x[0]=1;x[1]=0;x[2]=0;

		p2[0]  = pos[0] * tm[0] +
			    -pos[2] * tm[4] +
			    pos[1] * tm[8] +
			    tm[12] ;
                p2[1]  = pos[0] * tm[1] +
                            -pos[2] * tm[5] +
                            pos[1] * tm[9] +
                            tm[13];
                p2[2]  = pos[0] * tm[2] +
                            -pos[2] * tm[6] +
                            pos[1] * tm[10] +
                            tm[14] ;

		p[0]  = v[0] * tm[0] +
			    -v[2] * tm[4] +
			    v[1] * tm[8] +
			    tm[12] ;
                p[1]  = v[0] * tm[1] +
                            -v[2] * tm[5] +
                            v[1] * tm[9] +
                            tm[13];
                p[2]  = v[0] * tm[2] +
                            -v[2] * tm[6] +
                            v[1] * tm[10] +
                            tm[14] ;

	p = normalize(p);
    eye = eye - p2;
    eye = normalize(eye);

	svVector3 v2 = cross(eye, p);

	/*if(fabs(dot(p,x))<0.01)
	{
		v2[0]=0;
		v2[1]=0;
		v2[2]=1;
	}*/


	svVector3 r2;

	//if(fabs(v2[2]) < fabs(v2[0]))
	//	r2 = v2;
	//else
	// 	r2= cross(p, v2);

    if(!invert_matrix(tm, invert_tb)) {
        printf("Erron in inverting transform matrix\n");
        //exit(0);
    }

	r2 = v2;
	svVector3 r;

    r[0] = r2[0] * invert_tb[0] +
            r2[1] * invert_tb[4] +
            r2[2] * invert_tb[8] +
            invert_tb[12];

   r[1] = r2[0] * invert_tb[1] +
            r2[1] * invert_tb[5] +
            r2[2] * invert_tb[9] +
            invert_tb[13];

    r[2] = r2[0] * invert_tb[2] +
            r2[1] * invert_tb[6] +
            r2[2] * invert_tb[10] +
            invert_tb[14];

   svVector3 tmpr =r ;
//   r[1] = -tmpr[2];
//   r[2] =  tmpr[1];

   r = normalize(r);
	//delete [] tm;
	delete [] invert_tb;
	return r;
}

void svGlyph::Draw_2DFishBone(svVector3 pos, svVector3 vel, svScalar length)
{
 svVector3 end;
 
 end = pos + (svScalar)(5.*length) * vel;
 
 glLineWidth(1);
 
 glColor4f(0.3,0.3,0.3,1.);
 svScalar fishbone_l = length*2/3.;

 svVector3 v;

 for(int i=0;i<5;i++)
 {
	svVector3 head = pos + vel * (svScalar)(i+1) * length;
	v[0] = -vel[1];
        v[1] = vel[0];// svGetPerpendicularVector(vel);//GetVertical(head, vel,tm);
	v[2] = 0;
 	v = normalize(v);
	svVector3 p = head - fishbone_l * vel;
	svVector3 p1 = p + v * fishbone_l*0.3;
	svVector3 p2 = p - v * fishbone_l*0.3;

	glBegin(GL_LINES);
	glVertex3f(head[0], head[1], head[2]);
	glVertex3f(p1[0], p1[1], p1[2]);
	glEnd();

	glBegin(GL_LINES);
	glVertex3f(head[0], head[1], head[2]);
	glVertex3f(p2[0], p2[1], p2[2]);
	glEnd();

 }
 
}
void svGlyph::Draw_InnerLegend(svVector3 pos, svVector3 vel, svScalar l,svScalar length, GLfloat *tm)
{
	glEnable( GL_LINE_SMOOTH );
	glHint( GL_LINE_SMOOTH_HINT, GL_NICEST );
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

 glDisable(GL_LIGHTING);
 glDisable(GL_LIGHT0);
 
 svVector3 end;
 
 end = pos + (l*length) * vel;
 
 glLineWidth(1);
 
 glColor4f(0.3,0.3,0.3,1.);
 glBegin(GL_LINES);
 
 glVertex3f(pos[0], pos[1], pos[2]);
 glVertex3f(end[0], end[1], end[2]);
 
 glEnd();

 svScalar fishbone_l = length*2/3.;

 svVector3 v;

 for(int i=0;i<5;i++)
 {
	svVector3 head = pos + vel * (i+1) * length;
	v =  svGetPerpendicularVector(vel);//GetVertical(head, vel,tm);
	svVector3 p = head - fishbone_l * vel;
	svVector3 p1 = p + v * fishbone_l*0.3;
	svVector3 p2 = p - v * fishbone_l*0.3;

	glBegin(GL_LINES);
	glVertex3f(head[0], head[1], head[2]);
	glVertex3f(p1[0], p1[1], p1[2]);
	glEnd();

	glBegin(GL_LINES);
	glVertex3f(head[0], head[1], head[2]);
	glVertex3f(p2[0], p2[1], p2[2]);
	glEnd();

 }
 
}
void svGlyph::Draw_WidthLegend(svVector3 pos, svVector3 vel, svScalar l,svScalar length, GLfloat *tm)
{
	glEnable( GL_LINE_SMOOTH );
	glHint( GL_LINE_SMOOTH_HINT, GL_NICEST );
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


 glDisable(GL_LIGHTING);
 glDisable(GL_LIGHT0);
 
 svVector3 v = svGetPerpendicularVector(vel);
 svVector3 end1; 
 svVector3 end2;
 
 end1 = pos;
 end2 = pos +vel*0.001 + (svScalar)(5.*length) * v;

 glLineWidth(1);

 glColor4f(0.3,0.3,0.3,1);
 glBegin(GL_LINES);
 
 glVertex3f(end1[0], end1[1], end1[2]);
 glVertex3f(end2[0], end2[1], end2[2]);
 
 glEnd();
 
 svVector3 top = pos + vel;

 svScalar fishbone_l = length*2/3.;
 svVector3 v2 = normalize(svGetRotatePoint(top, pos, v, 90)); 

 for(int i=0;i<5;i++)
 {
	svVector3 head = pos + v * (i+1) * length;
	v2 =  svGetPerpendicularVector(v);//GetVertical(head, v,tm);
	svVector3 p = head - fishbone_l * v;
	svVector3 p1 = p + v2 * fishbone_l*0.3;
	svVector3 p2 = p - v2 * fishbone_l*0.3;

	glBegin(GL_LINES);
	glVertex3f(head[0], head[1], head[2]);
	glVertex3f(p1[0], p1[1], p1[2]);
	glEnd();

	glBegin(GL_LINES);
	glVertex3f(head[0], head[1], head[2]);
	glVertex3f(p2[0], p2[1], p2[2]);
	glEnd();
 }

 /*
 int segment = SEGMENT_NUM;
 
 svVector3 head;
 svVector3 rotate_base;
 double min_degree = 180./ segment; 
 
 for(int j=1;j<(int)l+1;j++)
 {
  head = pos + (svScalar)(j * length/2) * v;
  glBegin(GL_LINE_STRIP);
  for(int i=0;i<segment;i++)
  {
    rotate_base = svGetRotatePoint(head, pos, vel, (double)i * min_degree); 
    glVertex3f(rotate_base[0], rotate_base[1], rotate_base[2]);
  }
  glEnd();
  glBegin(GL_LINE_STRIP);
  for(int i=0;i<segment;i++)
  {
    rotate_base = svGetRotatePoint(head, pos, vel, -(double)i * min_degree); 
    glVertex3f(rotate_base[0], rotate_base[1], rotate_base[2]);
  }
  glEnd();
  
  head = pos - (svScalar)(j * length/2) * v;
  glBegin(GL_LINE_STRIP);
  for(int i=0;i<segment;i++)
  {
    rotate_base = svGetRotatePoint(head, pos, vel, (double)i * min_degree); 
    glVertex3f(rotate_base[0], rotate_base[1], rotate_base[2]);
  }
  glEnd();
  glBegin(GL_LINE_STRIP);
  for(int i=0;i<segment;i++)
  {
    rotate_base = svGetRotatePoint(head, pos, vel, -(double)i * min_degree); 
    glVertex3f(rotate_base[0], rotate_base[1], rotate_base[2]);
  }
  glEnd();
 }*/
 
}
void svGlyph::Draw_Cylinder(svVector3 pos, svVector3 vel,
				svScalar l,svScalar w,
			      svScalar length,
			      svScalar radius,
			      svVector4 col,
			      bool base,
				bool light, GLfloat *tm)
{
  //if(light)
  //{
  
    if (fabs(w)<1e-10)
    {
     w = 0.05; 
    }

 
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
        glEnable(GL_COLOR_MATERIAL);
        glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

	glEnable( GL_LINE_SMOOTH );
	glHint( GL_LINE_SMOOTH_HINT, GL_NICEST );
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
  
  svVector3 vel_vertical = svGetPerpendicularVector(vel);//GetVertical(pos, vel, tm);//svGetPerpendicularVector(vel);
  svVector3 head_base;
  head_base = pos + vel_vertical * radius * w;
  svVector3 head_top;
  head_top = pos + vel * length * l + vel_vertical * radius * w;
  svVector3 top = pos + vel * length * l;
  
  double min_degree = 360./ SEGMENT_NUM; 
  svVector3 rotate_base[SEGMENT_NUM];
  svVector3 rotate_top[SEGMENT_NUM];
  svVector3 rotate_normal[SEGMENT_NUM];
  for(int i=0;i<SEGMENT_NUM;i++)
  {
    rotate_base[i] = svGetRotatePoint(head_base, pos, vel, (double)i * min_degree); 
    rotate_top[i] = svGetRotatePoint(head_top, top, vel, (double)i * min_degree); 
	svVector3 n = rotate_base[i] - pos;

    rotate_normal[i] = normalize(n);
	//cerr<<rotate_base[i][0]<<" "<<rotate_base[i][1]<<" "<<(double)i * min_degree<<endl;
  } 
  
  //glColor4f(col[0], col[1], col[2], col[3]);
 // for(int t=0;t<5;t++)
 // {
  	double alpha = col[3];

	glColor4f(col[0], col[1], col[2], alpha);
	glBegin(GL_QUAD_STRIP);  
	for(int i=0;i<SEGMENT_NUM;i++)
	{
	  svVector3 normal = rotate_normal[i];
	  glNormal3f(normal[0], normal[1], normal[2]);
	  glVertex3f(rotate_base[i][0], rotate_base[i][1], rotate_base[i][2]);
	  glNormal3f(normal[0], normal[1], normal[2]);
	  glVertex3f(rotate_top[i][0], rotate_top[i][1], rotate_top[i][2]);
	}  
	glNormal3f(rotate_normal[0][0], rotate_normal[0][1], rotate_normal[0][2]);
	glVertex3f(rotate_base[0][0], rotate_base[0][1], rotate_base[0][2]);
	glNormal3f(rotate_normal[0][0], rotate_normal[0][1], rotate_normal[0][2]);
	glVertex3f(rotate_top[0][0], rotate_top[0][1], rotate_top[0][2]);
	glEnd();


	
	glBegin(GL_POLYGON);
	glNormal3f(vel[0], vel[1], vel[2]);
	for(int i=1;i<SEGMENT_NUM;i++)
	{
	  glVertex3f(rotate_top[i][0], rotate_top[i][1], rotate_top[i][2]);
	}
	glVertex3f(rotate_top[0][0], rotate_top[0][1], rotate_top[0][2]);
	glEnd();
	
	if(base)
	{
	  glBegin(GL_POLYGON);
	  glNormal3f(-vel[0], -vel[1], -vel[2]);
	  for(int i=1;i<SEGMENT_NUM;i++)
	  {
	    glVertex3f(rotate_base[i][0], rotate_base[i][1], rotate_base[i][2]);
	  }
	  glVertex3f(rotate_base[0][0], rotate_base[0][1], rotate_base[0][2]);
	  glEnd();    
	}
  //}

 // glDisable(GL_CULL_FACE);
  //glDepthFunc(GL_LESS);
///-------band-----------
  glLineWidth(0.5);
  glColor3f(0.2,0.2,0.2);
  glDisable(GL_LIGHTING);
  glDisable(GL_LIGHT0);
  glDisable(GL_NORMALIZE);

  for(int i=0;i<SEGMENT_NUM;i++)
  {
	rotate_base[i] = rotate_base[i] + rotate_normal[i] * 0.0001;
  }

  for(int j=1;j<=(int)l;j++)
  {
	  glBegin(GL_LINE_LOOP);
	  for(int i=0;i<SEGMENT_NUM;i++)
	  {
		rotate_base[i] = rotate_base[i] +  vel * length ;
		glVertex3f(rotate_base[i][0], rotate_base[i][1], rotate_base[i][2]);
	  }
	  glEnd();
  }
}

void svGlyph::Draw_Cylinder_Bottom(svVector3 pos, svVector3 vel,
				svScalar l,svScalar w,
			      svScalar length,
			      svScalar radius,
			      svVector4 col)
{
/*  Draw_Cylinder(pos, vel,
				1, w,
			      length,
			      radius,
			      col,
			       true,
				true);
  svVector3 new_pos = pos + length * vel;
  svVector4 new_col;
  new_col[0] = 1;
  new_col[1] = 1;
  new_col[2] = 1;
  new_col[3] = col[3];
  
  Draw_Cylinder(new_pos, vel,
				l-1, w,
			      length,
			      radius,
			      new_col,
			       false,
				true);  */
}
/*
void svGlyph::Draw_Cylinder_withtexture(svVector3 pos, svVector3 vel,
				svScalar l,svScalar n,
			      svScalar length,
			      svScalar radius,
			      svVector4 col)
{
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_COLOR_MATERIAL);
  glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
  glEnable(GL_NORMALIZE);

  glEnable(GL_LINE_SMOOTH);
  glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  
  glEnable(GL_DEPTH_TEST); 
  
  svScalar w = n;

	int int_w = (int)( w * 2);
	int num;
	if((svScalar)int_w < w*2.)
	{
		num = int_w + 1;
	}
	else
	{
		num = int_w;
	}

	svVector3 rotate_base;
	svVector3 rotate_top;
	svVector3 rotate_normal;
	svVector3 head;

	if(num > 0)
	{
	svScalar bar_degree;
	bar_degree = 360./(svScalar)SEGMENT_NUM;
	svScalar white_degree;
	white_degree = (360. - bar_degree * (svScalar)num)/(svScalar)num;

	
	svVector3 v;
	v = svGetPerpendicularVector(vel);
	head = pos + radius * v;
	int segment;
	segment = (SEGMENT_NUM - num)/num; 
	if(segment == 0)
	  segment = 1;
	for(int i=0;i< int_w;i++)
	{
		svVector3 tmphead = svGetRotatePoint(head, pos, vel, (svScalar)i * (white_degree + bar_degree));
		svVector3 tmptop = tmphead + l*length * vel;
		svVector3 normal;
		glColor4f(col[0], col[1], col[2], col[3]);
		glBegin(GL_QUAD_STRIP);
		for(int j=0;j<=segment;j++)
		{
			
			rotate_base = svGetRotatePoint(tmphead, pos, vel, (svScalar)j * white_degree/(svScalar)segment);
			rotate_top = rotate_base + l*length * vel;
			rotate_normal = normalize(rotate_base - pos);
			glNormal3f(rotate_normal[0], rotate_normal[1], rotate_normal[2]);
			glVertex3f(rotate_base[0], rotate_base[1], rotate_base[2]);
			glNormal3f(rotate_normal[0], rotate_normal[1], rotate_normal[2]);
			glVertex3f(rotate_top[0], rotate_top[1], rotate_top[2]);
		}		
		glEnd();
		tmphead = svGetRotatePoint(head, pos, vel, i * (white_degree + bar_degree) + white_degree);
		tmptop = tmphead + l*length * vel;
		rotate_base = svGetRotatePoint(tmphead, pos, vel, bar_degree);
		rotate_top = rotate_base + l*length * vel;
		rotate_normal = normalize(rotate_base - pos);
		normal = normalize(tmphead - pos);
		
		glColor4f(0,0,0,col[3]);
		glBegin(GL_QUADS);
		glNormal3f(normal[0],normal[1],normal[2]);
		glVertex3f(tmphead[0],tmphead[1],tmphead[2]);
		glNormal3f(rotate_normal[0],rotate_normal[1],rotate_normal[2]);
		glVertex3f(rotate_base[0],rotate_base[1],rotate_base[2]);
		glNormal3f(rotate_normal[0],rotate_normal[1],rotate_normal[2]);
		glVertex3f(rotate_top[0],rotate_top[1],rotate_top[2]);
		glNormal3f(normal[0],normal[1],normal[2]);
		glVertex3f(tmptop[0],tmptop[1],tmptop[2]);
		glEnd();
	}
	if(int_w < 2*w)
	{
		svScalar left_white_degree = white_degree + bar_degree - (w*2 - int_w ) * bar_degree;
		svVector3 tmphead = svGetRotatePoint(head, pos, vel,  int_w  * (white_degree + bar_degree));
		svVector3 tmptop = tmphead + l*length * vel;
		svVector3 normal;
		
		glColor4f(col[0], col[1], col[2], col[3]);
		glBegin(GL_QUAD_STRIP);
		for(int j=0;j<=segment;j++)
		{
			rotate_base = svGetRotatePoint(tmphead, pos, vel, j * left_white_degree/segment);
			rotate_top = rotate_base + l*length * vel;
			rotate_normal = normalize(rotate_base - pos);
			glNormal3f(rotate_normal[0], rotate_normal[1], rotate_normal[2]);
			glVertex3f(rotate_base[0], rotate_base[1], rotate_base[2]);
			glNormal3f(rotate_normal[0], rotate_normal[1], rotate_normal[2]);
			glVertex3f(rotate_top[0], rotate_top[1], rotate_top[2]);
		}		
		glEnd();

		tmphead = svGetRotatePoint(head, pos, vel,  int_w  * (white_degree + bar_degree) + left_white_degree);
		tmptop = tmphead + l*length * vel;
		rotate_base = svGetRotatePoint(tmphead, pos, vel,(w*2 - int_w ) * bar_degree);
		rotate_top = rotate_base + l*length * vel;
		normal = normalize(tmphead - pos);
		
		glColor4f(0,0,0,col[3]);
		glBegin(GL_QUADS);
		glNormal3f(normal[0],normal[1],normal[2]);
		glVertex3f(tmphead[0],tmphead[1],tmphead[2]);
		glNormal3f(rotate_normal[0],rotate_normal[1],rotate_normal[2]);
		glVertex3f(rotate_base[0],rotate_base[1],rotate_base[2]);
		glNormal3f(rotate_normal[0],rotate_normal[1],rotate_normal[2]);
		glVertex3f(rotate_top[0],rotate_top[1],rotate_top[2]);
		glNormal3f(normal[0],normal[1],normal[2]);
		glVertex3f(tmptop[0],tmptop[1],tmptop[2]);
		glEnd();

	}
	
	glColor4f(col[0],col[1],col[2],col[3]);
	glBegin(GL_POLYGON);
	glNormal3f(-vel[0],-vel[1],-vel[2]);
	for(int i=0;i<SEGMENT_NUM;i++)
	{
	  svVector3 base = svGetRotatePoint(rotate_base, pos, vel, i * 360./SEGMENT_NUM);
	  glVertex3f(base[0],base[1],base[2]);
	}
	glEnd();


	glColor4f(col[0],col[1],col[2],col[3]);
	glBegin(GL_POLYGON);
	glNormal3f(vel[0],vel[1],vel[2]);
	for(int i=0;i<SEGMENT_NUM;i++)
	{
	  svVector3 base = svGetRotatePoint(rotate_base, pos, vel, i * 360./SEGMENT_NUM);
	  base = base + l*length * vel;
	  glVertex3f(base[0],base[1],base[2]);
	}
	glEnd();
	
  glLineWidth(0.1);
  glColor3f(0.3,0.3,0.3);
  glDisable(GL_LIGHTING);
  glDisable(GL_LIGHT0);
  glDisable(GL_NORMALIZE);

	

  rotate_base= rotate_base + rotate_normal * 0.001;
  for(int j=1;j<=(int)l;j++)
  {
	  glBegin(GL_LINE_LOOP);
	  for(int i=0;i<SEGMENT_NUM;i++)
	  {
		 svVector3 base = svGetRotatePoint(rotate_base, pos, vel, i * 360./SEGMENT_NUM);
		 svVector3 top = base + l*length * vel;
		 glVertex3f(top[0],top[1],top[2]);
	  }
	  glEnd();
  }

  }
 
  svVector4 new_col;
 
  new_col[0]=1;
  new_col[1]=1;
  new_col[2]=1;
  new_col[3]=col[3];
  svVector3 new_pos;
 
  new_pos = pos + length * vel;
 if(num > 0)
      Draw_Cylinder(new_pos, vel,
				l-1.,1,
			      length,
			      radius,
			      new_col,
			      false,
				true);
 else
      Draw_Cylinder(pos, vel,
				l,1,
			      length,
			      radius,
			      new_col,
			      true,
				true);

}
*/
void svGlyph::Draw_Cylinder_withtexture(svVector3 pos, svVector3 vel,
				svScalar l,svScalar n,
			      svScalar length,
			      svScalar radius,
			      svVector4 col,svVector4 texture_col,
				svScalar max, GLfloat *tm)
{
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_COLOR_MATERIAL);
  glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
  glEnable(GL_NORMALIZE);

  glEnable(GL_LINE_SMOOTH);
  glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  
  glEnable(GL_DEPTH_TEST); 
  
	int num = 5;
	svScalar whole_degree = 360./num;
	int bar_segment, white_segment;

	svScalar bar_degree;
	bar_degree = whole_degree * n / max;
	svScalar white_degree;
	white_degree = whole_degree - bar_degree;
	bar_segment = (SEGMENT_NUM/num) * (n/max);
	if(bar_segment == 0)
		bar_segment = 1;
	white_segment = (SEGMENT_NUM/num) - bar_segment;
	
	svVector3 v;
	svVector3 rotate_base, rotate_top, rotate_normal;
	v = svGetPerpendicularVector(vel);//GetVertical(pos, vel, tm);//svGetPerpendicularVector(vel);
	svVector3 head = pos + radius * v;
	svVector3 top = pos + length * l * vel;

	for(int i=0;i<num;i++)
	{
		svVector3 tmphead = pos + radius * v;
		head = svGetRotatePoint(tmphead, pos, vel, i * whole_degree);

		
		//glColor4f(1, 1, 1, col[3]);
		glColor4f(col[0], col[1], col[2], col[3]);
		glBegin(GL_QUAD_STRIP);
		for(int j=0;j<=white_segment;j++)
		{
			rotate_base = svGetRotatePoint(head, pos, vel, (svScalar)j * white_degree/(svScalar)white_segment);
			rotate_top = rotate_base + l*length * vel;
			rotate_normal = normalize(rotate_base - pos);
			
			glNormal3f(rotate_normal[0], rotate_normal[1], rotate_normal[2]);
			glVertex3f(rotate_base[0], rotate_base[1], rotate_base[2]);
			glNormal3f(rotate_normal[0], rotate_normal[1], rotate_normal[2]);
			glVertex3f(rotate_top[0], rotate_top[1], rotate_top[2]);
		}
		glEnd();

		glColor4f(texture_col[0],texture_col[1],texture_col[2],texture_col[3]); 
		glBegin(GL_QUAD_STRIP);
		for(int j=0;j<=bar_segment;j++)
		{
			rotate_base = svGetRotatePoint(head, pos, vel, (svScalar)j * bar_degree/(svScalar)bar_segment + white_degree);
			
			rotate_normal = normalize(rotate_base - pos);
			//rotate_base = rotate_base + (svScalar)0.0001 * rotate_normal;
			rotate_top = rotate_base + l*length * vel;

			glNormal3f(rotate_normal[0], rotate_normal[1], rotate_normal[2]);
			glVertex3f(rotate_base[0], rotate_base[1], rotate_base[2]);
			glNormal3f(rotate_normal[0], rotate_normal[1], rotate_normal[2]);
			glVertex3f(rotate_top[0], rotate_top[1], rotate_top[2]);
		}
		glEnd();

	}	
	
	glColor4f(col[0], col[1], col[2], col[3]);
	glBegin(GL_POLYGON);
	glNormal3f(-vel[0],-vel[1],-vel[2]);
	for(int i=0;i<SEGMENT_NUM;i++)
	{
	  svVector3 base = svGetRotatePoint(head, pos, vel, i * 360./SEGMENT_NUM);
	  glVertex3f(base[0],base[1],base[2]);
	}
	glEnd();


	glColor4f(col[0], col[1], col[2], col[3]);
	glBegin(GL_POLYGON);
	glNormal3f(vel[0],vel[1],vel[2]);
	for(int i=0;i<SEGMENT_NUM;i++)
	{
	  svVector3 base = svGetRotatePoint(head, pos, vel, i * 360./SEGMENT_NUM);
	  base = base + l*length * vel;
	  glVertex3f(base[0],base[1],base[2]);
	}
	glEnd();
	
	  glLineWidth(0.5);
	  glColor3f(0.2,0.2,0.2);
	  glDisable(GL_LIGHTING);
	  glDisable(GL_LIGHT0);
	  glDisable(GL_NORMALIZE);
	  rotate_base= rotate_base + rotate_normal * 0.001;
	  for(int j=1;j<=(int)l;j++)
	  {
		  glBegin(GL_LINE_LOOP);
		  for(int i=0;i<SEGMENT_NUM;i++)
		  {
			 svVector3 base = svGetRotatePoint(rotate_base, pos, vel, i * 360./SEGMENT_NUM);
			 svVector3 top = base + j*length * vel;
			 glVertex3f(top[0],top[1],top[2]);
		  }
		  glEnd();
	  }

  

}
void svGlyph::Draw_Cylinder_withtexture_horizontal(svVector3 pos, svVector3 vel,
				svScalar l,svScalar n,
			      svScalar length,
			      svScalar radius,
			      svVector4 col)
{

  
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_COLOR_MATERIAL);
  glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
  glEnable(GL_NORMALIZE);
  

  glEnable(GL_LINE_SMOOTH);
  glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  
  glEnable(GL_DEPTH_TEST); 

  
  svScalar w = n;

	int int_w = (int) w;
	int num;
	
	if((svScalar)int_w < w)
	{
		num = int_w + 1;
	}
	else
	{
		num = int_w;
	}
	
	svVector3 rotate_base;
	svVector3 rotate_top;
	svVector3 rotate_normal;
	svScalar length_num = 10;
	svScalar bar_length = length/length_num;
	svScalar white_length = (length - bar_length*num)/(num+1);
	svVector3 v;
	v = svGetPerpendicularVector(vel);
	svVector3 head = pos + v * radius;
	
	for(int i=0;i< int_w;i++)
	{
	  svVector3 base = pos + vel *(i * (bar_length + white_length));
	  svVector3 tmphead = head + vel *(i * (bar_length + white_length));
	  
	  glColor4f(col[0],col[1],col[2],col[3]);
	  glBegin(GL_QUAD_STRIP);
	  for(int j=0;j<=SEGMENT_NUM;j++)
	  {
	    svVector3 draw_v;
	    svVector3 draw_n;
	    draw_v = svGetRotatePoint(tmphead, base, vel, j * 360./SEGMENT_NUM);
	    draw_n = normalize(draw_v - base);
	    glNormal3f(draw_n[0], draw_n[1], draw_n[2]);
	    glVertex3f(draw_v[0], draw_v[1], draw_v[2]);
	    svVector3 draw_v2 = draw_v + vel * white_length;
	    glNormal3f(draw_n[0], draw_n[1], draw_n[2]);
	    glVertex3f(draw_v2[0], draw_v2[1], draw_v2[2]);
	  }
	  glEnd();
	  
	  base = pos + vel *(i * (bar_length + white_length) + white_length);
	  tmphead = head + vel *(i * (bar_length + white_length) + white_length);
	  
	  glColor4f(0,0,0,col[3]);
	  glBegin(GL_QUAD_STRIP);
	  for(int j=0;j<=SEGMENT_NUM;j++)
	  {
	    svVector3 draw_v;
	    svVector3 draw_n;
	    draw_v = svGetRotatePoint(tmphead, base, vel, j * 360./SEGMENT_NUM);
	    draw_n = normalize(draw_v - base);
	    glNormal3f(draw_n[0], draw_n[1], draw_n[2]);
	    glVertex3f(draw_v[0], draw_v[1], draw_v[2]);
	    svVector3 draw_v2 = draw_v + vel * bar_length;
	    glNormal3f(draw_n[0], draw_n[1], draw_n[2]);
	    glVertex3f(draw_v2[0], draw_v2[1], draw_v2[2]);
	  }
	  glEnd();
	  
	  
	  
	  
	}
	
	if(int_w < w)
	{
	  svVector3 base = pos + vel *((int_w) * (bar_length + white_length));
	  svVector3 tmphead = head + vel *((int_w) * (bar_length + white_length));	  
	  glColor4f(col[0],col[1],col[2],col[3]);
	  glBegin(GL_QUAD_STRIP);
	  for(int j=0;j<=SEGMENT_NUM;j++)
	  {
	    svVector3 draw_v;
	    svVector3 draw_n;
	    draw_v = svGetRotatePoint(tmphead, base, vel, j * 360./SEGMENT_NUM);
	    draw_n = normalize(draw_v - base);
	    glNormal3f(draw_n[0], draw_n[1], draw_n[2]);
	    glVertex3f(draw_v[0], draw_v[1], draw_v[2]);
	    svVector3 draw_v2 = draw_v + vel * white_length;
	    glNormal3f(draw_n[0], draw_n[1], draw_n[2]);
	    glVertex3f(draw_v2[0], draw_v2[1], draw_v2[2]);
	  }
	  glEnd();
	  
	  svScalar left_bar_length = (w - int_w) * bar_length;
	  base = pos + vel *((int_w) * (bar_length + white_length) + white_length);
	  tmphead = head + vel *((int_w) * (bar_length + white_length) + white_length);	  
	  glColor4f(0,0,0,col[3]);
	  glBegin(GL_QUAD_STRIP);
	  for(int j=0;j<=SEGMENT_NUM;j++)
	  {
	    svVector3 draw_v;
	    svVector3 draw_n;
	    draw_v = svGetRotatePoint(tmphead, base, vel, j * 360./SEGMENT_NUM);
	    draw_n = normalize(draw_v - base);
	    glNormal3f(draw_n[0], draw_n[1], draw_n[2]);
	    glVertex3f(draw_v[0], draw_v[1], draw_v[2]);
	    svVector3 draw_v2 = draw_v + vel * left_bar_length;
	    glNormal3f(draw_n[0], draw_n[1], draw_n[2]);
	    glVertex3f(draw_v2[0], draw_v2[1], draw_v2[2]);
	  }
	  glEnd();

	  base = pos + vel *((int_w) * (bar_length + white_length)+ white_length + left_bar_length);
	  tmphead = head + vel *((int_w) * (bar_length + white_length)+ white_length + left_bar_length);	  
	  glColor4f(col[0],col[1],col[2],col[3]);
	  glBegin(GL_QUAD_STRIP);
	  for(int j=0;j<=SEGMENT_NUM;j++)
	  {
	      svVector3 draw_v;
	      svVector3 draw_n;
	      draw_v = svGetRotatePoint(tmphead, base, vel, j * 360./SEGMENT_NUM);
	      draw_n = normalize(draw_v - base);
	      glNormal3f(draw_n[0], draw_n[1], draw_n[2]);
	      glVertex3f(draw_v[0], draw_v[1], draw_v[2]);
	      svVector3 draw_v2 = head + vel * l * length;
	      glNormal3f(draw_n[0], draw_n[1], draw_n[2]);
	      glVertex3f(draw_v2[0], draw_v2[1], draw_v2[2]);
	    }
	    glEnd();
	}
	else
	{
	  svVector3 base = pos + vel *((int_w) * (bar_length + white_length));
	  svVector3 tmphead = head + vel *((int_w) * (bar_length + white_length));	  
	  glColor4f(col[0],col[1],col[2],col[3]);
	  glBegin(GL_QUAD_STRIP);
	  for(int j=0;j<=SEGMENT_NUM;j++)
	  {
	      svVector3 draw_v;
	      svVector3 draw_n;
	      draw_v = svGetRotatePoint(tmphead, base, vel, j * 360./SEGMENT_NUM);
	      draw_n = normalize(draw_v - base);
	      glNormal3f(draw_n[0], draw_n[1], draw_n[2]);
	      glVertex3f(draw_v[0], draw_v[1], draw_v[2]);
	      svVector3 draw_v2 = draw_v + vel * white_length;
	      glNormal3f(draw_n[0], draw_n[1], draw_n[2]);
	      glVertex3f(draw_v2[0], draw_v2[1], draw_v2[2]);
	    }
	    glEnd();
	}
	
	glColor4f(col[0],col[1],col[2],col[3]);
	glBegin(GL_POLYGON);
	glNormal3f(-vel[0],-vel[1],-vel[2]);
	for(int i=0;i<SEGMENT_NUM;i++)
	{
	  svVector3 base = svGetRotatePoint(head, pos, vel, i * 360./SEGMENT_NUM);
	  glVertex3f(base[0],base[1],base[2]);
	}
	glEnd();
	
  glLineWidth(0.1);
  glColor3f(0.3,0.3,0.3);
  glDisable(GL_LIGHTING);
  glDisable(GL_LIGHT0);
  glDisable(GL_NORMALIZE);


  head= head + vel*length + v * 0.001;
  for(int j=1;j<=1;j++)
  {
	  glBegin(GL_LINE_LOOP);
	  for(int i=0;i<SEGMENT_NUM;i++)
	  {
		 svVector3 base = svGetRotatePoint(head, pos, vel, i * 360./SEGMENT_NUM);
		 glVertex3f(base[0],base[1],base[2]);
	  }
	  glEnd();
  }
  
  svVector4 new_col;
 
  new_col[0]=1;
  new_col[1]=1;
  new_col[2]=1;
  new_col[3]=col[3];
  svVector3 new_pos;
  new_pos = pos + length * vel;
     // Draw_Cylinder(new_pos, vel,
	//			l-1.,1,
	//		      length,
	//		      radius,
	//		      new_col,
		//	      false,
				//true);
//
}
/*void svGlyph::Draw_Cylinder_withtexture(svVector3 pos, svVector3 vel,
				svScalar l,svScalar n,
			      svScalar length,
			      svScalar radius,
			      svVector4 col)
{
  

  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_COLOR_MATERIAL);
  glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
  glEnable(GL_NORMALIZE);

  glEnable(GL_LINE_SMOOTH);
  glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  
  glEnable(GL_DEPTH_TEST); 


  
  int num;
  int int_n = (int)n;

  
  double min_length = ((length * l / 2.) * ((double)int_n/n))/int_n;//cerr<<min_length<<endl;
  
  double min_degree = 360./ SEGMENT_NUM; 
  svVector3 rotate_base[SEGMENT_NUM];
  svVector3 rotate_top[SEGMENT_NUM];
  svVector3 rotate_normal[SEGMENT_NUM];
  svVector3 mid_base[SEGMENT_NUM];
  
  svVector3 mid = pos + (length * l)/2 * vel ; 
  svVector3 vel_vertical = svGetPerpendicularVector(vel);    

  float w = 1;
  svVector3 head_base;
  head_base[0] = pos[0] + vel_vertical[0] * radius * w;
  head_base[1] = pos[1] + vel_vertical[1] * radius * w;
  head_base[2] = pos[2] + vel_vertical[2] * radius * w;
  svVector3 head_top;
  head_top[0] = pos[0] + vel[0] * length * l + vel_vertical[0] * radius * w;
  head_top[1] = pos[1] + vel[1] * length * l + vel_vertical[1] * radius * w;
  head_top[2] = pos[2] + vel[2] * length * l + vel_vertical[2] * radius * w;
  svVector3 head_mid;
  head_mid[0] = mid[0] + vel_vertical[0] * radius * w;
  head_mid[1] = mid[1] + vel_vertical[1] * radius * w;
  head_mid[2] = mid[2] + vel_vertical[2] * radius * w;
  svVector3 top = pos + vel * length * l;  

      svVector3 tmp_rotate_base[SEGMENT_NUM];
    svVector3 tmp_rotate_top[SEGMENT_NUM];
    svVector3 tmp_rotate_normal[SEGMENT_NUM];
    
    svVector3 tmp_head_base;
    svVector3 tmp_head_top;
  for(int i=0;i<SEGMENT_NUM;i++)
  {
    rotate_base[i] = svGetRotatePoint(head_base, pos, vel, (double)i * min_degree); 
    mid_base[i] = svGetRotatePoint(head_mid, mid, vel, (double)i * min_degree);
    rotate_top[i] = svGetRotatePoint(head_top, top, vel, (double)i * min_degree); 
    rotate_normal[i] = normalize(rotate_base[i] - pos);
  } 
 

  	double alpha = col[3];	
	for(int j=0;j<int_n;j++)
	{
	  svVector3 top = mid + (svScalar)((j+1) * min_length) * vel;
	  svVector3 base = mid + (svScalar)((j) * min_length) * vel;
	  
	  tmp_head_base[0] = base[0] + vel_vertical[0] * radius ;
	  tmp_head_base[1] = base[1] + vel_vertical[1] * radius ;
	  tmp_head_base[2] = base[2] + vel_vertical[2] * radius ;
	  
	  tmp_head_top[0] = top[0] + vel_vertical[0] * radius ;
	  tmp_head_top[1] = top[1] + vel_vertical[1] * radius ;
	  tmp_head_top[2] = top[2] + vel_vertical[2] * radius ;    
	  
	  for(int i=0;i<SEGMENT_NUM;i++)
	  {
	    tmp_rotate_base[i] = svGetRotatePoint(tmp_head_base, base, vel, (double)i * min_degree); 
	    tmp_rotate_top[i] = svGetRotatePoint(tmp_head_top, top, vel, (double)i * min_degree); 
	    tmp_rotate_normal[i] = normalize(tmp_rotate_top[i] - top);
	  } 
	  
	  if(j%2==1)
	  {
	    glColor4f(col[0], col[1], col[2], alpha);
	  }
	  else
	  {
	    glColor4f(1.,1.,1.,alpha);
	  }
	  
	  glBegin(GL_QUAD_STRIP);  
	  for(int i=0;i<SEGMENT_NUM;i++)
	  {
	    svVector3 normal = tmp_rotate_normal[i];
	    glNormal3f(normal[0], normal[1], normal[2]);
	    glVertex3f(tmp_rotate_base[i][0], tmp_rotate_base[i][1], tmp_rotate_base[i][2]);
	    glNormal3f(normal[0], normal[1], normal[2]);
	    glVertex3f(tmp_rotate_top[i][0], tmp_rotate_top[i][1], tmp_rotate_top[i][2]);
	  }  
	    glNormal3f(tmp_rotate_normal[0][0], tmp_rotate_normal[0][1], tmp_rotate_normal[0][2]);
	    glVertex3f(tmp_rotate_base[0][0], tmp_rotate_base[0][1], tmp_rotate_base[0][2]);
	    glNormal3f(tmp_rotate_normal[0][0], tmp_rotate_normal[0][1], tmp_rotate_normal[0][2]);
	    glVertex3f(tmp_rotate_top[0][0], tmp_rotate_top[0][1], tmp_rotate_top[0][2]);
	  glEnd();  
      }
      
      if(int_n < n)
      {
	if(int_n%2 == 1)
	{
	  glColor4f(col[0], col[1], col[2], alpha);
	}
	else
	{
	  glColor4f(1.,1.,1.,alpha);
	}
  	  for(int i=0;i<SEGMENT_NUM;i++)
	  {
	    tmp_rotate_base[i] = tmp_rotate_top[i]; 
	    tmp_rotate_top[i] = rotate_top[i];
	  }     
	glBegin(GL_QUAD_STRIP);  
	for(int i=0;i<SEGMENT_NUM;i++)
	{
	  svVector3 normal = tmp_rotate_normal[i];
	  glNormal3f(normal[0], normal[1], normal[2]);
	  glVertex3f(tmp_rotate_base[i][0], tmp_rotate_base[i][1], tmp_rotate_base[i][2]);
	  glNormal3f(normal[0], normal[1], normal[2]);
	  glVertex3f(tmp_rotate_top[i][0], tmp_rotate_top[i][1], tmp_rotate_top[i][2]);
	}  
	  glNormal3f(tmp_rotate_normal[0][0], tmp_rotate_normal[0][1], tmp_rotate_normal[0][2]);
	  glVertex3f(tmp_rotate_base[0][0], tmp_rotate_base[0][1], tmp_rotate_base[0][2]);
	  glNormal3f(tmp_rotate_normal[0][0], tmp_rotate_normal[0][1], tmp_rotate_normal[0][2]);
	  glVertex3f(tmp_rotate_top[0][0], tmp_rotate_top[0][1], tmp_rotate_top[0][2]);
	glEnd();  
      }
      
	//-----------------------------------------------------------------------
      for(int j=0;j<int_n;j++)
      {
	svVector3 base = mid - (svScalar)((j+1) * min_length) * vel;
	svVector3 top = mid - (svScalar)((j) * min_length) * vel;
	
	tmp_head_base[0] = base[0] + vel_vertical[0] * radius ;
	tmp_head_base[1] = base[1] + vel_vertical[1] * radius ;
	tmp_head_base[2] = base[2] + vel_vertical[2] * radius ;
	
	tmp_head_top[0] = top[0] + vel_vertical[0] * radius ;
	tmp_head_top[1] = top[1] + vel_vertical[1] * radius ;
	tmp_head_top[2] = top[2] + vel_vertical[2] * radius ;    
	
	for(int i=0;i<SEGMENT_NUM;i++)
	{
	  tmp_rotate_base[i] = svGetRotatePoint(tmp_head_base, base, vel, (double)i * min_degree); 
	  tmp_rotate_top[i] = svGetRotatePoint(tmp_head_top, top, vel, (double)i * min_degree); 
	  tmp_rotate_normal[i] = normalize(tmp_rotate_top[i] - top);
	} 
	
	if(j%2==0)
	{
	  glColor4f(col[0], col[1], col[2], alpha);
	}
	else
	{
	  glColor4f(1.,1.,1.,alpha);
	}
	
	glBegin(GL_QUAD_STRIP);  
	for(int i=0;i<SEGMENT_NUM;i++)
	{
	  svVector3 normal = tmp_rotate_normal[i];
	  glNormal3f(normal[0], normal[1], normal[2]);
	  glVertex3f(tmp_rotate_base[i][0], tmp_rotate_base[i][1], tmp_rotate_base[i][2]);
	  glNormal3f(normal[0], normal[1], normal[2]);
	  glVertex3f(tmp_rotate_top[i][0], tmp_rotate_top[i][1], tmp_rotate_top[i][2]);
	}  
	  glNormal3f(tmp_rotate_normal[0][0], tmp_rotate_normal[0][1], tmp_rotate_normal[0][2]);
	  glVertex3f(tmp_rotate_base[0][0], tmp_rotate_base[0][1], tmp_rotate_base[0][2]);
	  glNormal3f(tmp_rotate_normal[0][0], tmp_rotate_normal[0][1], tmp_rotate_normal[0][2]);
	  glVertex3f(tmp_rotate_top[0][0], tmp_rotate_top[0][1], tmp_rotate_top[0][2]);
	glEnd();  
      }
      
      if(int_n<n)
      {
	  if(int_n%2 == 0)
	  {
	    glColor4f(col[0], col[1], col[2], alpha);
	  }
	  else
	  {
	    glColor4f(1.,1.,1.,alpha);
	  }
      
  	  for(int i=0;i<SEGMENT_NUM;i++)
	  {
	    tmp_rotate_top[i] = tmp_rotate_base[i]; 
	    tmp_rotate_base[i] = rotate_base[i];
	  }    
	glBegin(GL_QUAD_STRIP);  
	for(int i=0;i<SEGMENT_NUM;i++)
	{
	  svVector3 normal = tmp_rotate_normal[i];
	  glNormal3f(normal[0], normal[1], normal[2]);
	  glVertex3f(tmp_rotate_base[i][0], tmp_rotate_base[i][1], tmp_rotate_base[i][2]);
	  glNormal3f(normal[0], normal[1], normal[2]);
	  glVertex3f(tmp_rotate_top[i][0], tmp_rotate_top[i][1], tmp_rotate_top[i][2]);
	}  
	  glNormal3f(tmp_rotate_normal[0][0], tmp_rotate_normal[0][1], tmp_rotate_normal[0][2]);
	  glVertex3f(tmp_rotate_base[0][0], tmp_rotate_base[0][1], tmp_rotate_base[0][2]);
	  glNormal3f(tmp_rotate_normal[0][0], tmp_rotate_normal[0][1], tmp_rotate_normal[0][2]);
	  glVertex3f(tmp_rotate_top[0][0], tmp_rotate_top[0][1], tmp_rotate_top[0][2]);
	glEnd(); 
      }
      
      
	glColor4f(1.,1.,1.,alpha);
	glBegin(GL_POLYGON);
	glNormal3f(vel[0], vel[1], vel[2]);
	for(int i=1;i<SEGMENT_NUM;i++)
	{
	  glVertex3f(rotate_top[i][0], rotate_top[i][1], rotate_top[i][2]);
	}
	glVertex3f(rotate_top[0][0], rotate_top[0][1], rotate_top[0][2]);
	glEnd();
	
	
	  glBegin(GL_POLYGON);
	  glNormal3f(-vel[0], -vel[1], -vel[2]);
	  for(int i=1;i<SEGMENT_NUM;i++)
	  {
	    glVertex3f(rotate_base[i][0], rotate_base[i][1], rotate_base[i][2]);
	  }
	  glVertex3f(rotate_base[0][0], rotate_base[0][1], rotate_base[0][2]);
	  glEnd();   
  //}
  
   // glDisable(GL_CULL_FACE);
 // glDepthFunc(GL_LEQUAL);
///-------band-----------
  glLineWidth(0.1);
  glColor3f(0.3,0.3,0.3);
  glDisable(GL_LIGHTING);
  glDisable(GL_LIGHT0);
  glDisable(GL_NORMALIZE);

  for(int i=0;i<SEGMENT_NUM;i++)
  {
	rotate_base[i] = rotate_base[i] + rotate_normal[i] * 0.001;
  }

  for(int j=1;j<=(int)l;j++)
  {
	  glBegin(GL_LINE_LOOP);
	  for(int i=0;i<SEGMENT_NUM;i++)
	  {
		rotate_base[i] = rotate_base[i] +  vel * length ;
		glVertex3f(rotate_base[i][0], rotate_base[i][1], rotate_base[i][2]);
	  }
	  glEnd();
  }
	
}*/
void svGlyph::RenderBounding(svVector3 lbbox, svVector3 rbbox)
{
	glEnable(GL_BLEND);
	glColor3f(1.,1.,1.);
	
	glLineWidth(0.2);
	glBegin(GL_LINES);
	glVertex3f(lbbox[0],lbbox[1],lbbox[2]);
	glVertex3f(rbbox[0],lbbox[1],lbbox[2]);
	glEnd();
	glBegin(GL_LINES);
	glVertex3f(lbbox[0],lbbox[1],lbbox[2]);
	glVertex3f(lbbox[0],rbbox[1],lbbox[2]);
	glEnd();
	glBegin(GL_LINES);
	glVertex3f(lbbox[0],lbbox[1],lbbox[2]);
	glVertex3f(lbbox[0],lbbox[1],rbbox[2]);
	glEnd();

	glBegin(GL_LINES);
	glVertex3f(rbbox[0],rbbox[1],lbbox[2]);
	glVertex3f(rbbox[0],lbbox[1],lbbox[2]);
	glEnd();
	glBegin(GL_LINES);
	glVertex3f(rbbox[0],rbbox[1],lbbox[2]);
	glVertex3f(lbbox[0],rbbox[1],lbbox[2]);
	glEnd();
	glBegin(GL_LINES);
	glVertex3f(rbbox[0],rbbox[1],lbbox[2]);
	glVertex3f(rbbox[0],rbbox[1],rbbox[2]);
	glEnd();

	glBegin(GL_LINES);
	glVertex3f(rbbox[0],rbbox[1],rbbox[2]);
	glVertex3f(rbbox[0],lbbox[1],rbbox[2]);
	glEnd();
	glBegin(GL_LINES);
	glVertex3f(rbbox[0],rbbox[1],rbbox[2]);
	glVertex3f(lbbox[0],rbbox[1],rbbox[2]);
	glEnd();
	glBegin(GL_LINES);
	glVertex3f(lbbox[0],lbbox[1],rbbox[2]);
	glVertex3f(lbbox[0],rbbox[1],rbbox[2]);
	glEnd();
	glBegin(GL_LINES);
	glVertex3f(lbbox[0],lbbox[1],rbbox[2]);
	glVertex3f(rbbox[0],lbbox[1],rbbox[2]);
	glEnd();

	glBegin(GL_LINES);
	glVertex3f(rbbox[0],lbbox[1],rbbox[2]);
	glVertex3f(rbbox[0],lbbox[1],lbbox[2]);
	glEnd();
	glBegin(GL_LINES);
	glVertex3f(lbbox[0],rbbox[1],rbbox[2]);
	glVertex3f(lbbox[0],rbbox[1],lbbox[2]);
	glEnd();

	svVector3 end;
	svVector3 dir;
	svVector3 point[3];
	svVector3 arrow[3][SEGMENT_NUM+1];
	svVector3 arrownorm[3][SEGMENT_NUM+1];
	svVector3 norm;
	svVector3 head;
	svVector3 tmp;
        for(int i=0;i<3;i++)
        {
		if(i==0)
		{
			end[0]=0;end[1]=0;end[2]=rbbox[2]*1.1;
			dir[0]=0;dir[1]=0;dir[2]=1;
		}
		else if(i==1)
		{
			end[0]=0;end[2]=0;end[1]=rbbox[1]*1.1;
			dir[0]=0;dir[2]=0;dir[1]=1;
		}
		else if(i==2)
		{
			end[2]=0;end[1]=0;end[0]=rbbox[0]*1.1;
			dir[2]=0;dir[1]=0;dir[0]=1;
		}
		tmp[0]=0;tmp[1]=0;tmp[2]=0;
		svScalar arrow_length = 0.17;
		svScalar arrow_radius = 0.03;
		point[i]=end+arrow_length*dir;
		norm = svGetPerpendicularVector(dir);
		head = end + arrow_radius * norm;

		for(int j=0;j<SEGMENT_NUM;j++)
		{
			arrow[i][j] = svGetRotatePoint(head, end, dir, j * 360/SEGMENT_NUM);
		}
		for(int j=0;j<SEGMENT_NUM;j++)
		{
			if(j!=SEGMENT_NUM-1)
				arrownorm[i][j+1] = svGetNorm(arrow[i][j],point[i],arrow[i][j+1]);
			else
				arrownorm[i][j+1] = svGetNorm(arrow[i][j],point[i],arrow[i][0]);
			arrownorm[i][j+1] = normalize(arrownorm[i][j+1]);
		}
		arrownorm[i][0] = dir;
		for(int j=1;j<SEGMENT_NUM+1;j++)
		{
			if(j!=1)
				arrownorm[i][j] = (arrownorm[i][j] + arrownorm[i][j-1])/2.;
			else
				arrownorm[i][j] = (arrownorm[i][j] + arrownorm[i][SEGMENT_NUM])/2.;
			arrownorm[i][j] = normalize(arrownorm[i][j]);
		}

        }

	glColor3f(67./255.,162./255.,202./255.);
	glBegin(GL_LINES);
	glVertex3f(0.,0.,0.);
	glVertex3f(0.,0.,rbbox[2]*1.1);
	glEnd();
	glRasterPos3f(0.1,.1,rbbox[2]*1.1);
	glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'z');
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	for(int i=0;i<SEGMENT_NUM;i++)
	{
		glBegin(GL_TRIANGLES);
		glNormal3f(arrownorm[0][0][0],arrownorm[0][0][1],arrownorm[0][0][2]);
		glVertex3f(point[0][0],point[0][1],point[0][2]);
		glNormal3f(arrownorm[0][i+1][0],arrownorm[0][i+1][1],arrownorm[0][i+1][2]);
		glVertex3f(arrow[0][i][0],arrow[0][i][1],arrow[0][i][2]);
		if(i!=SEGMENT_NUM-1)
		{
			glNormal3f(arrownorm[0][i+2][0],arrownorm[0][i+2][1],arrownorm[0][i+2][2]);
			glVertex3f(arrow[0][i+1][0],arrow[0][i+1][1],arrow[0][i+1][2]);
		}
		else
		{
			glNormal3f(arrownorm[0][1][0],arrownorm[0][1][1],arrownorm[0][1][2]);
			glVertex3f(arrow[0][0][0],arrow[0][0][1],arrow[0][0][2]);
		}
		glEnd();
	}
	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT0);

	glColor3f(44./255.,162./255.,95./255.);
	glBegin(GL_LINES);
	glVertex3f(0.,0.,0.);
	glVertex3f(0.,rbbox[1]*1.1,0.);
	glEnd();
	glRasterPos3f(.1,rbbox[1]*1.1,.1);
	glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'y');
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	for(int i=0;i<SEGMENT_NUM;i++)
	{
		glBegin(GL_TRIANGLES);
		glNormal3f(arrownorm[1][0][0],arrownorm[1][0][1],arrownorm[1][0][2]);
		glVertex3f(point[1][0],point[1][1],point[1][2]);
		glNormal3f(arrownorm[1][i+1][0],arrownorm[1][i+1][1],arrownorm[1][i+1][2]);
		glVertex3f(arrow[1][i][0],arrow[1][i][1],arrow[1][i][2]);
		if(i!=SEGMENT_NUM-1)
		{
			glNormal3f(arrownorm[1][i+2][0],arrownorm[1][i+2][1],arrownorm[1][i+2][2]);
			glVertex3f(arrow[1][i+1][0],arrow[1][i+1][1],arrow[1][i+1][2]);
		}
		else
		{
			glNormal3f(arrownorm[1][1][0],arrownorm[1][1][1],arrownorm[1][1][2]);
			glVertex3f(arrow[1][0][0],arrow[1][0][1],arrow[1][0][2]);
		}
		glEnd();
	}
	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT0);

	glColor3f(227./255.,74./255.,51./255.);
	glBegin(GL_LINES);
	glVertex3f(0.,0.,0.);
	glVertex3f(rbbox[0]*1.1,0.,0.);
	glEnd();
	glRasterPos3f(rbbox[0]*1.1,.1,.1);
	glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'x');
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	for(int i=0;i<SEGMENT_NUM;i++)
	{
		glBegin(GL_TRIANGLES);
		glNormal3f(arrownorm[2][0][0],arrownorm[2][0][1],arrownorm[2][0][2]);
		glVertex3f(point[2][0],point[2][1],point[2][2]);
		glNormal3f(arrownorm[2][i+1][0],arrownorm[2][i+1][1],arrownorm[2][i+1][2]);
		glVertex3f(arrow[2][i][0],arrow[2][i][1],arrow[2][i][2]);
		if(i!=SEGMENT_NUM-1)
		{
			glNormal3f(arrownorm[2][i+2][0],arrownorm[2][i+2][1],arrownorm[2][i+2][2]);
			glVertex3f(arrow[2][i+1][0],arrow[2][i+1][1],arrow[2][i+1][2]);
		}
		else
		{
			glNormal3f(arrownorm[2][1][0],arrownorm[2][1][1],arrownorm[2][1][2]);
			glVertex3f(arrow[2][0][0],arrow[2][0][1],arrow[2][0][2]);
		}
		glEnd();
	}
	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT0);

	glLineWidth(1.);
}

void svGlyph::Draw_Arrow(svVector3 pos, svVector3 vel,
			      svScalar length,
			      svScalar radius,
			      svScalar top_length,
			      svVector4 col)
{

	  glEnable(GL_LIGHTING);
	  glEnable(GL_LIGHT0);
	  glEnable(GL_COLOR_MATERIAL);
	  glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
 	  glEnable(GL_NORMALIZE);
  

  glEnable(GL_LINE_SMOOTH);
  glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  
  glEnable(GL_DEPTH_TEST); 

  svVector3 vel_vertical = svGetPerpendicularVector(vel);
  svVector3 head_base = pos + vel * length +  vel_vertical * radius;
  svVector3 top = pos + vel * (length + top_length);
  svVector3 center = pos + vel * length;
  
  double min_degree = 360./ SEGMENT_NUM ;
  
  svVector3 rotate_base[SEGMENT_NUM];
  svVector3 rotate_normal[SEGMENT_NUM];
  for(int i=0;i<SEGMENT_NUM;i++)
  {
    rotate_base[i] = svGetRotatePoint(head_base, center, vel, (double)i * min_degree);  
  } 
  for(int i=0;i<SEGMENT_NUM;i++)
  {
    svVector3 v1, v2, v3;
    v3 = -vel;
    if(i==0)
    {
      v1 = cross(top - rotate_base[i],
		 rotate_base[SEGMENT_NUM-1] - rotate_base[i]);
      v2 = cross(rotate_base[i+1] - rotate_base[i],
		 top - rotate_base[i]);
    }
    else if(i==SEGMENT_NUM-1)
    {
      v1 = cross(top - rotate_base[i],
		 rotate_base[i-1] - rotate_base[i]);
      v2 = cross(rotate_base[i]-rotate_base[SEGMENT_NUM-1], 
		 top - rotate_base[i]);
    }
    else
    {
      v1 = cross(top - rotate_base[i],
		 rotate_base[i-1] - rotate_base[i]);
      v2 = cross(rotate_base[i+1] - rotate_base[i],
		 top - rotate_base[i]);
    }
    
    rotate_normal[i] = normalize(v1 + v2 + v3);
  }
  
  glColor4f(col[0], col[1], col[2], col[3]);
  
  glBegin(GL_TRIANGLE_FAN); 
  glNormal3f(vel[0], vel[1], vel[2]);
  glVertex3f(top[0], top[1], top[2]);
  for(int i=0;i<SEGMENT_NUM;i++)
  {
    glNormal3f(rotate_normal[i][0], rotate_normal[i][1], rotate_normal[i][2]);
    glVertex3f(rotate_base[i][0], rotate_base[i][1], rotate_base[i][2]);
  }  
  glNormal3f(rotate_normal[0][0], rotate_normal[0][1], rotate_normal[0][2]);
  glVertex3f(rotate_base[0][0], rotate_base[0][1], rotate_base[0][2]);
  glEnd();
  
  glBegin(GL_POLYGON);
  glNormal3f(-vel[0], -vel[1], -vel[2]);
  for(int i=1;i<SEGMENT_NUM;i++)
  {
    glVertex3f(rotate_base[i][0], rotate_base[i][1], rotate_base[i][2]);
  }
  glEnd();


  glLineWidth(3.);
  glBegin(GL_LINES);
  glVertex3f(pos[0], pos[1], pos[2]);
  glVertex3f(center[0], center[1], center[2]);
  glEnd();
  
  glColor3f(0.,0.,0.);
  glLineWidth(5.);
  glBegin(GL_LINES);
  glVertex3f(pos[0], pos[1], pos[2]);
  glVertex3f(center[0], center[1], center[2]);
  glEnd();
}

void svGlyph::Draw_Shape(svVector3 pos, svVector3 vel,
			      svScalar A,
			      svScalar r,
			      svVector4 col, bool light)
{
  glDisable(GL_LIGHTING);
  glDisable(GL_LIGHT0);
  glDisable(GL_NORMALIZE);

  svVector3 end;
  svScalar scalar = A * 1.5;
  end[0] = pos[0]+  scalar * vel[0];
  end[1] = pos[1] + scalar * vel[1];
  end[2] = pos[2] + scalar * vel[2];

  glLineWidth(2.);
  glColor3f(col[0], col[1], col[2]);
  glBegin(GL_LINES);
  glVertex3f(pos[0], pos[1], pos[2]);
  glVertex3f(end[0], end[1], end[2]);
  glEnd();
  glLineWidth(1.);

  if(light)
  {
	  glEnable(GL_LIGHTING);
	  glEnable(GL_LIGHT0);
	  glEnable(GL_COLOR_MATERIAL);
	  glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	  glEnable(GL_NORMALIZE);
  }
  
  glEnable(GL_LINE_SMOOTH);
  glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  
  glEnable(GL_DEPTH_TEST); 


  double u;
  double v;
  
  int num = SEGMENT_NUM * (SEGMENT_NUM + 1);
  double c, s;
  double x[num], y[num], z[num];
  svVector3 normal[num];
  double v_segment = SV_PI/SEGMENT_NUM;
  double u_segment = SV_2_PI/SEGMENT_NUM;

  double angle_x = acos(vel[2]);
  if(vel[1] > 0)
  {
	angle_x = - angle_x;
  }
  double xy_project = vel[0] * vel[0] + vel[1] * vel[1];
  xy_project = sqrt(xy_project);
  double angle_z = acos(vel[1]/xy_project);
  if(angle_x < 0)
  {	
	if (vel[0] > 0)
	 {
		angle_z =  -angle_z;
	  }
  }
  else
  {
	angle_z = SV_PI - angle_z;
	if(vel[0] < 0)
		angle_z =  -angle_z;		
  }
  
  for(int i=0;i<=SEGMENT_NUM;i++)
  {
    v = -SV_PI/2. + i * v_segment;
    for(int j=0;j<SEGMENT_NUM;j++)
    {
      u = -SV_PI + j * u_segment;
      x[i * SEGMENT_NUM + j] = A * function_c(v, 2./r)
				* function_c(u, 2./r) ;
      y[i * SEGMENT_NUM + j] = A * function_c(v, 2./r)
				* function_s(u, 2./r);
      z[i * SEGMENT_NUM + j] = A * function_s(v, 2./r);
    }
  }
  
/*svVector3 zz;
zz[0] = 0; zz[1] = 0; zz[2] = 1;
zz[0] = (sin(angle_x))* sin(angle_z)+pos[0];
zz[1] =  (-sin(angle_x))* cos(angle_z)+ pos[1];
zz[2] =  cos(angle_x) + pos[2];
zz[0] = pos[0];
zz[1] = -sin(angle_x) + pos[1];
zz[2] = cos(angle_x) + pos[2];
cerr<<vel[0]<<" "<<vel[1]<<" "<<vel[2]<<endl;
cerr<<(sin(angle_x))* sin(angle_z)<<" "<<(-sin(angle_x))* cos(angle_z)<<" "<<cos(angle_x)<<endl;
cerr<< -sin(angle_x) <<" "<<cos(angle_x) <<endl;*/
  for(int i=0;i<num;i++)
  {
	double tempx, tempy, tempz;
	tempx = x[i]; tempy = y[i]; tempz = z[i];

	
	x[i] = tempx;
	y[i] = tempy * cos(angle_x) + tempz * (-sin(angle_x));
	z[i] = tempy * sin(angle_x) + tempz * cos(angle_x);

	tempx = x[i]; tempy = y[i]; tempz = z[i];
	x[i] = tempx * cos(angle_z) - tempy * sin(angle_z)+ pos[0];
	y[i] = tempx * sin(angle_z) + tempy * cos(angle_z)+ pos[1];
	z[i] = tempz+ pos[2];


	normal[i][0] = x[i] - pos[0];
	normal[i][1] = y[i] - pos[1];
	normal[i][2] = z[i] - pos[2];
  }

  //----Solve Depth Buffer Problems Here----
  //http://www.alecjacobson.com/weblog/?p=2750
  for(int t=0;t<5;t++)
  {
	double alpha;
	double f =0.75;
	if(t==0)
	{
		glDisable(GL_CULL_FACE);
		glDepthFunc(GL_LESS);
		alpha = 0;
  	}
	else if(t == 1)
	{
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);
		glDepthFunc(GL_ALWAYS);
		alpha = f * col[3];
	}
	else if(t == 2)
	{
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);
		glDepthFunc(GL_LEQUAL);
		alpha = (col[3] - f*col[3])/(1. - f*col[3]);
	}
	else if(t == 3)
	{
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glDepthFunc(GL_ALWAYS);
		alpha = f * col[3];
	}
	else if(t == 4)
	{
		glDisable(GL_CULL_FACE);
		//glCullFace(GL_BACK);
		glDepthFunc(GL_LEQUAL);
		alpha = (col[3] - f*col[3])/(1. - f*col[3]);
	}
   glColor4f(col[0], col[1], col[2], alpha);
  for(int i=0;i<SEGMENT_NUM;i++)
  {
    glBegin(GL_QUAD_STRIP);
    for(int j=0;j<SEGMENT_NUM;j++)
    {
	glNormal3f(normal[i * SEGMENT_NUM + j][0], 
			normal[i * SEGMENT_NUM + j][1], 
			normal[i * SEGMENT_NUM + j][2]);
      	glVertex3f(x[i * SEGMENT_NUM + j],
		 y[i * SEGMENT_NUM + j],
		 z[i * SEGMENT_NUM + j]);
	glNormal3f(normal[(i+1) * SEGMENT_NUM + j][0], 
			normal[(i+1) * SEGMENT_NUM + j][1], 
			normal[(i+1) * SEGMENT_NUM + j][2]);
      	glVertex3f(x[(i+1) * SEGMENT_NUM + j],
		 y[(i+1) * SEGMENT_NUM + j],
		 z[(i+1) * SEGMENT_NUM + j]);
    }
    glNormal3f(normal[i * SEGMENT_NUM ][0], 
			normal[i * SEGMENT_NUM ][1], 
			normal[i * SEGMENT_NUM ][2]);
    glVertex3f(x[i * SEGMENT_NUM ],
		 y[i * SEGMENT_NUM ],
		 z[i * SEGMENT_NUM ]);
    glNormal3f(normal[(i+1) * SEGMENT_NUM ][0], 
			normal[(i+1) * SEGMENT_NUM ][1], 
			normal[(i+1) * SEGMENT_NUM ][2]);
    glVertex3f(x[(i+1) * SEGMENT_NUM ],
		 y[(i+1) * SEGMENT_NUM ],
		 z[(i+1) * SEGMENT_NUM ]);
    glEnd();
  }

  }

  glDisable(GL_CULL_FACE);
/*  glDisable(GL_LIGHTING);
  glDisable(GL_LIGHT0);
  glDisable(GL_NORMALIZE);


  glLineWidth(2.);
  glColor3f(0,0,0);
  glBegin(GL_LINES);
  glVertex3f(pos[0], pos[1], pos[2]);
  glVertex3f(zz[0], zz[1], zz[2]);
  glEnd();
  glLineWidth(1.);*/

}


GLboolean svGlyph::invert_matrix( const GLfloat *m, GLfloat *out )
{
/* NB. OpenGL Matrices are COLUMN major. */
#define SWAP_ROWS(a, b) { GLfloat *_tmp = a; (a)=(b); (b)=_tmp; }
#define MAT(m,r,c) (m)[(c)*4+(r)]

 GLfloat wtmp[4][8];
 GLfloat m0, m1, m2, m3, s;
 GLfloat *r0, *r1, *r2, *r3;

 r0 = wtmp[0], r1 = wtmp[1], r2 = wtmp[2], r3 = wtmp[3];

 r0[0] = MAT(m,0,0), r0[1] = MAT(m,0,1),
 r0[2] = MAT(m,0,2), r0[3] = MAT(m,0,3),
 r0[4] = 1.0, r0[5] = r0[6] = r0[7] = 0.0,

 r1[0] = MAT(m,1,0), r1[1] = MAT(m,1,1),
 r1[2] = MAT(m,1,2), r1[3] = MAT(m,1,3),
 r1[5] = 1.0, r1[4] = r1[6] = r1[7] = 0.0,

 r2[0] = MAT(m,2,0), r2[1] = MAT(m,2,1),
 r2[2] = MAT(m,2,2), r2[3] = MAT(m,2,3),
 r2[6] = 1.0, r2[4] = r2[5] = r2[7] = 0.0,

 r3[0] = MAT(m,3,0), r3[1] = MAT(m,3,1),
 r3[2] = MAT(m,3,2), r3[3] = MAT(m,3,3),
 r3[7] = 1.0, r3[4] = r3[5] = r3[6] = 0.0;

 /* choose pivot - or die */
 if (fabs(r3[0])>fabs(r2[0])) SWAP_ROWS(r3, r2);
 if (fabs(r2[0])>fabs(r1[0])) SWAP_ROWS(r2, r1);
 if (fabs(r1[0])>fabs(r0[0])) SWAP_ROWS(r1, r0);
 if (0.0 == r0[0])  return GL_FALSE;

 /* eliminate first variable     */
 m1 = r1[0]/r0[0]; m2 = r2[0]/r0[0]; m3 = r3[0]/r0[0];
 s = r0[1]; r1[1] -= m1 * s; r2[1] -= m2 * s; r3[1] -= m3 * s;
 s = r0[2]; r1[2] -= m1 * s; r2[2] -= m2 * s; r3[2] -= m3 * s;
 s = r0[3]; r1[3] -= m1 * s; r2[3] -= m2 * s; r3[3] -= m3 * s;
 s = r0[4];
 if (s != 0.0) { r1[4] -= m1 * s; r2[4] -= m2 * s; r3[4] -= m3 * s; }
 s = r0[5];
 if (s != 0.0) { r1[5] -= m1 * s; r2[5] -= m2 * s; r3[5] -= m3 * s; }
 s = r0[6];
 if (s != 0.0) { r1[6] -= m1 * s; r2[6] -= m2 * s; r3[6] -= m3 * s; }
 s = r0[7];
 if (s != 0.0) { r1[7] -= m1 * s; r2[7] -= m2 * s; r3[7] -= m3 * s; }

 /* choose pivot - or die */
 if (fabs(r3[1])>fabs(r2[1])) SWAP_ROWS(r3, r2);
 if (fabs(r2[1])>fabs(r1[1])) SWAP_ROWS(r2, r1);
 if (0.0 == r1[1])  return GL_FALSE;

 /* eliminate second variable */
 m2 = r2[1]/r1[1]; m3 = r3[1]/r1[1];
 r2[2] -= m2 * r1[2]; r3[2] -= m3 * r1[2];
 r2[3] -= m2 * r1[3]; r3[3] -= m3 * r1[3];
 s = r1[4]; if (0.0 != s) { r2[4] -= m2 * s; r3[4] -= m3 * s; }
 s = r1[5]; if (0.0 != s) { r2[5] -= m2 * s; r3[5] -= m3 * s; }
 s = r1[6]; if (0.0 != s) { r2[6] -= m2 * s; r3[6] -= m3 * s; }
 s = r1[7]; if (0.0 != s) { r2[7] -= m2 * s; r3[7] -= m3 * s; }

 /* choose pivot - or die */
 if (fabs(r3[2])>fabs(r2[2])) SWAP_ROWS(r3, r2);
 if (0.0 == r2[2])  return GL_FALSE;

 /* eliminate third variable */
 m3 = r3[2]/r2[2];
 r3[3] -= m3 * r2[3], r3[4] -= m3 * r2[4],
 r3[5] -= m3 * r2[5], r3[6] -= m3 * r2[6],
 r3[7] -= m3 * r2[7];
 /* last check */
 if (0.0 == r3[3]) return GL_FALSE;

 s = 1.0/r3[3];              /* now back substitute row 3 */
 r3[4] *= s; r3[5] *= s; r3[6] *= s; r3[7] *= s;

 m2 = r2[3];                 /* now back substitute row 2 */
 s  = 1.0/r2[2];
 r2[4] = s * (r2[4] - r3[4] * m2), r2[5] = s * (r2[5] - r3[5] * m2),
 r2[6] = s * (r2[6] - r3[6] * m2), r2[7] = s * (r2[7] - r3[7] * m2);
 m1 = r1[3];
 r1[4] -= r3[4] * m1, r1[5] -= r3[5] * m1,
 r1[6] -= r3[6] * m1, r1[7] -= r3[7] * m1;
 m0 = r0[3];
 r0[4] -= r3[4] * m0, r0[5] -= r3[5] * m0,
 r0[6] -= r3[6] * m0, r0[7] -= r3[7] * m0;

 m1 = r1[2];                 /* now back substitute row 1 */
 s  = 1.0/r1[1];
 r1[4] = s * (r1[4] - r2[4] * m1), r1[5] = s * (r1[5] - r2[5] * m1),
 r1[6] = s * (r1[6] - r2[6] * m1), r1[7] = s * (r1[7] - r2[7] * m1);
 m0 = r0[2];
 r0[4] -= r2[4] * m0, r0[5] -= r2[5] * m0,
 r0[6] -= r2[6] * m0, r0[7] -= r2[7] * m0;

 m0 = r0[1];                 /* now back substitute row 0 */
 s  = 1.0/r0[0];
 r0[4] = s * (r0[4] - r1[4] * m0), r0[5] = s * (r0[5] - r1[5] * m0),
 r0[6] = s * (r0[6] - r1[6] * m0), r0[7] = s * (r0[7] - r1[7] * m0);

 MAT(out,0,0) = r0[4]; MAT(out,0,1) = r0[5],
 MAT(out,0,2) = r0[6]; MAT(out,0,3) = r0[7],
 MAT(out,1,0) = r1[4]; MAT(out,1,1) = r1[5],
 MAT(out,1,2) = r1[6]; MAT(out,1,3) = r1[7],
 MAT(out,2,0) = r2[4]; MAT(out,2,1) = r2[5],
 MAT(out,2,2) = r2[6]; MAT(out,2,3) = r2[7],
 MAT(out,3,0) = r3[4]; MAT(out,3,1) = r3[5],
 MAT(out,3,2) = r3[6]; MAT(out,3,3) = r3[7];

 return GL_TRUE;

#undef MAT
#undef SWAP_ROWS
}
