
/** 
 * Author: Jian Chen
 * Date: Nov 30 2013
 * Description: glyphs
 *   calculate the angle of those vectors and map them
*/


#ifdef __APPLE__
  #include <GLUT/glut.h>
#else
  #include <GL/glut.h>
#endif

#include <iostream>
#include <fstream>
#include <string.h>
#include <stdio.h>     
#include <stdlib.h>    
#include <time.h>       
#include <cmath>

#include "QDOTglyph.h"

#define SWAP(T, a, b)   { T t; t=a; a=b; b=t;  }

#define SV_PI	  (3.14159265358979323846)
#define SV_2_PI   (3.14159265358979323846 * 2.0)
#define SV_SMALL  (1e-6)

#define svToDegree(x)             ((x)*(180.)/SV_PI)
#define svToRadian(x)	           ((x)*SV_PI/(180.))
#define svMax(a,b)                (((a) > (b)) ? (a) : (b))
#define svMin(a,b)                (((a) < (b)) ? (a) : (b))
#define svClamp(v, min_v, max_v)  { v = svMax(svMin(v, max_v), min_v); }

	
template <class T> inline
  void svSwap(T & a, T & b)
{  T t = a; a = b; b = t;  }

svVector3 svGetPerpendicularVector(const svVector3 & v)
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

svVector3 svGetRotatePoint(const svVector3& start, const svVector3& org, 
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

// get the normal vector
//      v1-------------v0
//                    /
//                   /
//                  / v2
//
svVector3 svGetNorm(const svVector3& v1, const svVector3& v0,
                    const svVector3& v2)
{
  return normalize(cross((v1-v0), (v2-v0)));
}

svVector3 svAverage(const svVector3& v1, const svVector3& v2)
{
  return normalize(v1+v2);
}


void QDOTglyph::SetData(svVector3 *pos, svVector3 *dir, svScalar *den, 
			svVector3 *col, int num,
			svVector3 l, svVector3 r)
{
	if(glyphArrowPos!=NULL)
	{	cleanup();}
	
	NUM_SEG=10;
	dataPos = new svVector3[num];
	dataEnd = new svVector3[num];
	dataDir = new svVector3[num];
	dataDen = new svScalar[num];
	dataEnd2 = new svVector3[num];
	dataPoint = new svVector3[num];

	ni = -1;
	
	exp = new svScalar[num];
	coe = new svScalar[num];
	dataCol = new svVector3[num];
	glyphArrowPos = new svVector3Array[NUM_SEG];
	glyphArrowNorm = new svVector3Array[NUM_SEG+1];

	float numCopy;
  	int multiplier = 0;
	bool flag;

	for(int i=0;i<num;i++)
	{
		flag = false;
		dataPos[i] = pos[i];
		dataDir[i] = dir[i];
		dataDen[i] = den[i];

		if(col==NULL)
		{
			dataCol[i][0]=1.;
			dataCol[i][1]=1.;
			dataCol[i][2]=1.;
		}
		else
			dataCol[i] = col[i];

		for(int j=0;j<NUM_SEG;j++)
			glyphArrowPos[j].add(pos[i]);
		for(int j=0;j<NUM_SEG+1;j++)
			glyphArrowNorm[j].add(pos[i]);
		multiplier = 0;

		//cerr<<dataPos[i][0]<<" "<<dataPos[i][1]<<" "<<dataPos[i][2]<<" "<<endl;

		numCopy = den[i];
		
		  if(numCopy<1&&numCopy>0) 
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
		{multiplier=0;}
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
	
		exp[i] = multiplier;
		coe[i] = den[i]/(pow(10.,exp[i]));

		if(exp[i]>maxExp)maxExp = exp[i];
		if(exp[i]<minExp)minExp = exp[i];
		
	}

	lbbox= l;
	rbbox= r;
	dataSize = num;

	glyphFrequency = 1;
	//cerr<<minExp<<" "<<maxExp<<endl;

	minl_distribute = 0.05/2.;
	minl_log = 0.05/2.;
	minl_text = 0.05/2.;	
	minl_direct = 0.05/2.;
	arrow_radius = 0.05;
	arrow_length = 0.2;
}

void QDOTglyph::SetEndPos()
{

	svScalar scale;

	for(int i=0;i<dataSize;i++)
	{
		if(ENCODE_MODE == 1)
		{
			
			scale = dataDen[i] * minl_direct;
		}
		else if(ENCODE_MODE==2)
		{
			scale = coe[i]*minl_distribute;
		}
		else if(ENCODE_MODE==3)
		{
			scale = minl_text;
		}
		else if(ENCODE_MODE==4)
		{

			scale = log10(dataDen[i])*minl_log;
		}
		dataEnd[i] = dataPos[i] + scale*dataDir[i];
		dataEnd2[i] = dataPos[i] + exp[i]*minl_distribute*dataDir[i];
		dataPoint[i] = dataEnd[i] + arrow_length*dataDir[i];
	}
}


void QDOTglyph::SetColors(svVector3 *c)
{
	if(c==NULL)
	for(int i=0;i<dataSize;i++)
	{
		dataCol[i][0] = 1.;
		dataCol[i][1] = 1.;
		dataCol[i][2] = 1.;
	}
	else
	for(int i=0;i<dataSize;i++)
	{
		dataCol[i] = c[i];
	}
}


void QDOTglyph::SetArrows()
{
        svVector3 head;
	svVector3 norm;
	svVector3 segment_dir;	
	svVector3 point;
	svVector3 end;
	int i;
	float scale;
	svVector3 tmp;
	tmp[0]=0.;tmp[1]=0.;tmp[2]=0.;

        for(i=0;i<dataSize;i++)
        {
		end = dataPos[i];
		point= dataEnd[i];//end+arrow_length*dataDir[i];

		segment_dir = normalize(dataDir[i]);
		norm = svGetPerpendicularVector(segment_dir);
		head = end + arrow_radius * norm;

		for(int j=0;j<NUM_SEG;j++)
		{
			glyphArrowPos[j][i] = svGetRotatePoint(head, end, segment_dir, j * 360/NUM_SEG);
		}
		for(int j=0;j<NUM_SEG;j++)
		{
			if(j!=NUM_SEG-1)
				glyphArrowNorm[j+1][i] = svGetNorm(glyphArrowPos[j][i],point,glyphArrowPos[j+1][i]);
			else
				glyphArrowNorm[j+1][i] = svGetNorm(glyphArrowPos[j][i],point,glyphArrowPos[0][i]);
			glyphArrowNorm[j+1][i] = normalize(glyphArrowNorm[j+1][i]);
		}
		for(int j=1;j<NUM_SEG+1;j++)
		{
			tmp = tmp + glyphArrowNorm[j][i];
		}
		tmp = tmp + segment_dir;
		tmp = tmp/(float)(NUM_SEG+1);
		tmp = normalize(tmp);
		glyphArrowNorm[0][i] = tmp;
		for(int j=1;j<NUM_SEG+1;j++)
		{
			if(j!=1)
				glyphArrowNorm[j][i] = (glyphArrowNorm[j][i] + glyphArrowNorm[j-1][i])/2.;
			else
				glyphArrowNorm[j][i] = (glyphArrowNorm[j][i] + glyphArrowNorm[NUM_SEG][i])/2.;
			glyphArrowNorm[j][i] = normalize(glyphArrowNorm[j][i]);
		}

        }
}
void QDOTglyph::Render()
{
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

	svVector3 point;
	svVector3 end;
	float scale;
	float length = 0.05;
	float minl = 0.05;

	int s,e;
	int i;

	int times;
	svVector3 linestart;
	float dot;	
	float dot2;

        for(int ii =0;ii<dataSize;)
        {
		i = ii;
		if(coe[i]!=0)
		{	
			glDisable(GL_LIGHTING);
			glDisable(GL_LIGHT0);

			if(ENCODE_MODE!=3)
			{
				glLineWidth(1.);
				linestart = dataPos[i] + 5*minl_distribute*dataDir[i];
				glColor4f(0.3,0.3,0.3,1.);
				glBegin(GL_LINES);
				glVertex3f(dataPos[i][0],dataPos[i][1],dataPos[i][2]);
				glVertex3f(linestart[0],linestart[1],linestart[2]);
				glEnd();
			}

			glEnable(GL_LIGHTING);
			glEnable(GL_LIGHT0);

			if(i==taskarrow[0])
				glColor4f(1.,0.,0.,0.5);
			else if(i==taskarrow[1])
				glColor4f(1.,1.,0.,0.5);
			else if(i==taskarrow_task4[0])
				glColor4f(1.,0.,0.,0.5);
			else if(i==taskarrow_task4[1])
				glColor4f(1.,1.,0.,0.5);
			else	
				glColor4f(dataCol[i][0],dataCol[i][1],dataCol[i][2],0.5);
		
			if(ENCODE_MODE==2)
			{
				glPushMatrix();
				glTranslatef(dataEnd2[i][0]+arrow_radius*dataDir[i][0],
					dataEnd2[i][1]+arrow_radius*dataDir[i][1],
					dataEnd2[i][2]+arrow_radius*dataDir[i][2]);
				glutSolidSphere(arrow_radius,20,20);
				glPopMatrix();
			}
	
			for(int j=0;j<NUM_SEG;j++)
			{
				glBegin(GL_TRIANGLES);
				glNormal3f(glyphArrowNorm[0][i][0],glyphArrowNorm[0][i][1],glyphArrowNorm[0][i][2]);
				glVertex3f(dataEnd[i][0],dataEnd[i][1],dataEnd[i][2]);
				glNormal3f(glyphArrowNorm[j+1][i][0],glyphArrowNorm[j+1][i][1],glyphArrowNorm[j+1][i][2]);
				glVertex3f(glyphArrowPos[j][i][0],glyphArrowPos[j][i][1],glyphArrowPos[j][i][2]);
				if(j!=NUM_SEG-1)
		                {
					glNormal3f(glyphArrowNorm[j+2][i][0],glyphArrowNorm[j+2][i][1],glyphArrowNorm[j+2][i][2]);
		                        glVertex3f(glyphArrowPos[j+1][i][0],glyphArrowPos[j+1][i][1],glyphArrowPos[j+1][i][2]);
				}
				else
				{
		                        glNormal3f(glyphArrowNorm[1][i][0],glyphArrowNorm[1][i][1],glyphArrowNorm[1][i][2]);
		                        glVertex3f(glyphArrowPos[0][i][0],glyphArrowPos[0][i][1],glyphArrowPos[0][i][2]);
				}
				glEnd();
			}
			glBegin(GL_POLYGON);
			glNormal3f(-glyphArrowNorm[0][i][0],-glyphArrowNorm[0][i][1],-glyphArrowNorm[0][i][2]);
			for(int j=0;j<NUM_SEG;j++)
			{
				glVertex3f(glyphArrowPos[j][i][0],glyphArrowPos[j][i][1],
					glyphArrowPos[j][i][2]);				
			}
			glEnd();

		}
			ii = ii + glyphFrequency;

        }
	/*glBegin(GL_POINTS);
	for(i=0;i<dataPointPos[0].size();i++)
		glVertex3f(dataPointPos[0][i][0],dataPointPos[0][i][1],dataPointPos[0][i][2]);
	glEnd();*/

        glDisable(GL_LIGHTING);
        glDisable(GL_LIGHT0);
        glDisable(GL_BLEND);
	glLineWidth(1.);
}

void QDOTglyph::RenderCylindernocolor()
{
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

	float expr = minl_distribute*1.7;//0.06/1.5;
	float coer = minl_distribute;//0.035/1.5;
	float exeh;
	float coeh;

	svVector3 segment_dir,norm,head,o,linestart;
	int NUM = 15;
	svVector3 p1[NUM];
	svVector3 n1[NUM];
	svVector3 p2[NUM];
	svVector3 n2[NUM];

	float length;

        for(int ii =0;ii<dataSize;)
        {
		if(coe[ii]!=0)
		{
			exeh = exp[ii] * minl_distribute;

				glDisable(GL_LIGHTING);
				glDisable(GL_LIGHT0);
				glDisable(GL_TEXTURE_2D); 
				glLineWidth(1.);
				glColor4f(0.3,0.3,0.3,1.);
				glBegin(GL_LINES);
				glVertex3f(dataPos[ii][0],dataPos[ii][1],dataPos[ii][2]);
				glVertex3f(dataPos[ii][0]+5.*minl_distribute*dataDir[ii][0],
					dataPos[ii][1]+5.*minl_distribute*dataDir[ii][1],
					dataPos[ii][2]+5.*minl_distribute*dataDir[ii][2]);
				glEnd();				


			glEnable(GL_LIGHTING);
			glEnable(GL_LIGHT0);
			glEnable(GL_TEXTURE_2D); 

			glColor4f(1.,1.,1.,0.5);
			segment_dir = normalize(dataDir[ii]);
			norm = svGetPerpendicularVector(segment_dir);
			head = dataPos[ii] + coer * norm;
			o = dataPos[ii];
			for(int i=0;i<NUM;i++)
			{
				p1[i]=svGetRotatePoint(head, o, segment_dir, (float)i * 360./(NUM-1));
				n1[i]=p1[i]-o;
				n1[i]=normalize(n1[i]);
			}

			if(ENCODE_MODE!=2)
			for(int i=0;i<NUM-1;i++)
			{
				glNormal3f(-segment_dir[0],-segment_dir[1],-segment_dir[2]);
				glBegin(GL_TRIANGLES);
				glVertex3f(p1[i][0],p1[i][1],p1[i][2]);
				glVertex3f(p1[i+1][0],p1[i+1][1],p1[i+1][2]);		
				glVertex3f(o[0],o[1],o[2]);	
				glEnd();
			}			

			o = dataEnd[ii];
			head = o + coer * norm;
			for(int i=0;i<NUM;i++)
			{
				p2[i]=svGetRotatePoint(head, o, segment_dir, (float)i * 360./(NUM-1));
				n2[i] = p2[i] - o;
				n2[i]=normalize(n2[i]);
			}

			for(int i=0;i<NUM-1;i++)
			{
				glBegin(GL_QUADS);
				glNormal3f(n1[i][0],n1[i][1],n1[i][2]);
				glVertex3f(p1[i][0],p1[i][1],p1[i][2]);
				glNormal3f(n1[i+1][0],n1[i+1][1],n1[i+1][2]);
				glVertex3f(p1[i+1][0],p1[i+1][1],p1[i+1][2]);
				glNormal3f(n2[i+1][0],n2[i+1][1],n2[i+1][2]);
				glVertex3f(p2[i+1][0],p2[i+1][1],p2[i+1][2]);		
				glNormal3f(n2[i][0],n2[i][1],n2[i][2]);
				glVertex3f(p2[i][0],p2[i][1],p2[i][2]);			
				glEnd();
			}

			for(int i=0;i<NUM-1;i++)
			{
				glNormal3f(segment_dir[0],segment_dir[1],segment_dir[2]);
				glBegin(GL_TRIANGLES);
				glVertex3f(p2[i][0],p2[i][1],p2[i][2]);
				glVertex3f(p2[i+1][0],p2[i+1][1],p2[i+1][2]);		
				glVertex3f(o[0],o[1],o[2]);	
				glEnd();
			}	

			if(ENCODE_MODE==2)
			{
				segment_dir = normalize(dataDir[ii]);
				norm = svGetPerpendicularVector(segment_dir);
				head = dataPos[ii] + expr * norm;
				o = dataPos[ii];
				for(int i=0;i<NUM;i++)
				{
					p1[i]=svGetRotatePoint(head, o, segment_dir, (float)i * 360./(NUM-1));
					n1[i]=p1[i]-o;
					n1[i]=normalize(n1[i]);
				}
				for(int i=0;i<NUM-1;i++)
				{
					glNormal3f(-segment_dir[0],-segment_dir[1],-segment_dir[2]);
					glBegin(GL_TRIANGLES);
					glVertex3f(p1[i][0],p1[i][1],p1[i][2]);
					glVertex3f(p1[i+1][0],p1[i+1][1],p1[i+1][2]);		
					glVertex3f(o[0],o[1],o[2]);	
					glEnd();
				}				
				o = dataEnd2[ii];
				head = o + expr * norm;
				for(int i=0;i<NUM;i++)
				{
					p2[i]=svGetRotatePoint(head, o, segment_dir, (float)i * 360./(NUM-1));
					n2[i] = p2[i] - o;
					n2[i]=normalize(n2[i]);
				}
				glColor4f(1.,1.,1.,0.5);
				for(int i=0;i<NUM-1;i++)
				{
					glBegin(GL_QUADS);
					glNormal3f(n1[i][0],n1[i][1],n1[i][2]);
					glVertex3f(p1[i][0],p1[i][1],p1[i][2]);
					glNormal3f(n1[i+1][0],n1[i+1][1],n1[i+1][2]);
					glVertex3f(p1[i+1][0],p1[i+1][1],p1[i+1][2]);
					glNormal3f(n2[i+1][0],n2[i+1][1],n2[i+1][2]);
					glVertex3f(p2[i+1][0],p2[i+1][1],p2[i+1][2]);		
					glNormal3f(n2[i][0],n2[i][1],n2[i][2]);
					glVertex3f(p2[i][0],p2[i][1],p2[i][2]);			
					glEnd();
				}

				if(exp[ii]>coe[ii])
				{
					head = dataPos[ii] + coer * norm;
					for(int i=0;i<NUM;i++)
					{
						p1[i]=svGetRotatePoint(head, o, segment_dir, (float)i * 360./(NUM-1));
						n1[i]=p1[i]-o;
						n1[i]=normalize(n1[i]);
					}	
					for(int i=0;i<NUM-1;i++)
					{
						glNormal3f(segment_dir[0],segment_dir[1],segment_dir[2]);
						glBegin(GL_QUADS);
						glVertex3f(p1[i][0],p1[i][1],p1[i][2]);
						glVertex3f(p1[i+1][0],p1[i+1][1],p1[i+1][2]);
						glVertex3f(p2[i+1][0],p2[i+1][1],p2[i+1][2]);		
						glVertex3f(p2[i][0],p2[i][1],p2[i][2]);			
						glEnd();
					}				
				}
				else
				{
					for(int i=0;i<NUM-1;i++)
					{
						glNormal3f(segment_dir[0],segment_dir[1],segment_dir[2]);
						glBegin(GL_TRIANGLES);
						glVertex3f(p2[i][0],p2[i][1],p2[i][2]);
						glVertex3f(p2[i+1][0],p2[i+1][1],p2[i+1][2]);		
						glVertex3f(o[0],o[1],o[2]);	
						glEnd();
					}
				}
				for(int i=0;i<NUM;i++)
				{
					p1[i] = p2[i];n1[i]=n2[i];
				}
			}
			
		}

		ii = ii + glyphFrequency;
        }
        glDisable(GL_LIGHTING);
        glDisable(GL_LIGHT0);
        glDisable(GL_BLEND);
	glLineWidth(1.);
}
void QDOTglyph::RenderLines()
{
	glCallList(listIndex);
}
void QDOTglyph::GenerateLines()
{
	listIndex = glGenLists(1);
	if (listIndex != 0) {
	   glNewList(listIndex,GL_COMPILE);

	glDisable(GL_TEXTURE_2D); 
	glEnable( GL_LINE_SMOOTH );
	glHint( GL_LINE_SMOOTH_HINT, GL_NICEST );
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_DEPTH_TEST);

	float expr = minl_distribute*1.7;//0.06/1.5;
	float coer = minl_distribute;//0.035/1.5;
	float exeh;
	float coeh;

	svVector3 segment_dir,norm,head,o,linestart;
	int NUM = 15;
	svVector3 p1[NUM];
	svVector3 n1[NUM];
	svVector3 p2[NUM];
	svVector3 n2[NUM];
	svVector3 point;

	float length;
	int num;

	glLineWidth(1.);
        for(int ii =0;ii<dataSize;)
        {
		if(coe[ii]!=0)
		{

			glDisable(GL_LIGHTING);
			glDisable(GL_LIGHT0);

			if(ENCODE_MODE==1)
				num = dataDen[ii];
			else if(ENCODE_MODE==2)
				num = coe[ii];
			else if(ENCODE_MODE==4)
				num = log10(dataDen[ii]);
			else if(ENCODE_MODE==3)
				num = minl_text/minl_direct;

			segment_dir = normalize(dataDir[ii]);
			norm = svGetPerpendicularVector(segment_dir);	

			glColor4f(0.2,0.2,0.2,1);
		
			for(int j=0;j<num;j++)
			{
				point = dataPos[ii] + dataDir[ii] * (j+1) * minl_direct;	
				head =  point + coer * norm;
				o = point;
				glBegin(GL_LINE_STRIP);
				for(int i=0;i<NUM;i++)
				{
					p1[i]=svGetRotatePoint(head, o, segment_dir, (float)i * 360./(NUM-1));
					glVertex3f(p1[i][0],p1[i][1],p1[i][2]);
				}
				glEnd();
			}

			if(ENCODE_MODE==2)
			{
				num = exp[ii];
				segment_dir = normalize(dataDir[ii]);
				norm = svGetPerpendicularVector(segment_dir);	

				glColor4f(0.2,0.2,0.2,1);
		
				for(int j=0;j<num;j++)
				{
					point = dataPos[ii] + dataDir[ii] * (j+1) * minl_direct;;	
					head =  point + expr * norm;
					o = point;
					glBegin(GL_LINE_STRIP);
					for(int i=0;i<NUM;i++)
					{
						p1[i]=svGetRotatePoint(head, o, segment_dir, (float)i * 360./(NUM-1));
						glVertex3f(p1[i][0],p1[i][1],p1[i][2]);
					}
					glEnd();
				}
			}
			
		}

		ii = ii + glyphFrequency;
        }
        glDisable(GL_LIGHTING);
        glDisable(GL_LIGHT0);
        glDisable(GL_BLEND);
	glLineWidth(1.);
	   glEndList();
	}
}

void QDOTglyph::RenderMarker(GLfloat *m)
{

    GLdouble vmat[16], pmat[16], sx[3], sy[3], sz[3];
    GLint    viewport[4];
	glEnable(GL_BLEND);

    glGetDoublev(GL_MODELVIEW_MATRIX, vmat);
    glGetDoublev(GL_PROJECTION_MATRIX, pmat);
    glGetIntegerv(GL_VIEWPORT, viewport);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0,(GLfloat) viewport[2], (GLfloat) viewport[3],0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

	svVector3 p[2];
	svVector3 tmp1,tmp2;
	svVector3 dir,dir2;
	int i;
	svVector3 end;
	float scale;

	for(int k=0;k<2;k++)
	{
		for(int j =0;j<2;j++)
		{
			p[j][0]=0.;p[j][1]=0.;p[j][2]=0.;	
		}
		tmp1[0]=0.;tmp1[1]=0.;tmp1[2]=0.;
		//tmp2[0]=0.;tmp2[1]=0.;tmp2[2]=0.;
		if(taskarrow[k]!=-1)
		{
			i = taskarrow[k];
			p[0][0]  = dataPos[i][0] * m[0] +
				dataPos[i][1] * m[4] +
				dataPos[i][2] * m[8] +
				m[12] + p[0][0];
			p[0][1]  = dataPos[i][0] * m[1] +
				  dataPos[i][1] * m[5] +
				  dataPos[i][2] * m[9] +
				  m[13] + p[0][1];
			p[0][2]  = dataPos[i][0] * m[2] +
				   dataPos[i][1] * m[6] +
				   dataPos[i][2] * m[10] +
				   m[14] + p[0][2];
			gluProject((double)(p[0][0]), 
				(double)(p[0][1]), 
				(double)(p[0][2]), 
				vmat, pmat, viewport, &sx[0],&sy[0],&sz[0]);
			sy[0] = (float)viewport[3]-(float)sy[0];

			scale =minl_distribute*(exp[i]);
			if(ENCODE_MODE==2&&scale>minl_distribute*coe[i])
			{
				end = dataPos[i] + scale*dataDir[i];
			}
			else
			{
				end = dataEnd[i];	
			}

			p[1][0]  = end[0] * m[0] +
				end[1] * m[4] +
				end[2] * m[8] +
				m[12] + p[1][0];
			p[1][1]  = end[0] * m[1] +
				  end[1] * m[5] +
				  end[2] * m[9] +
				  m[13] + p[1][1];
			p[1][2]  = end[0] * m[2] +
				   end[1] * m[6] +
				   end[2] * m[10] +
				   m[14] + p[1][2];

			gluProject((double)(p[1][0]), 
				(double)(p[1][1]), 
				(double)(p[1][2]), 
				vmat, pmat, viewport, &sx[1],&sy[1],&sz[1]);
			sy[1] = (float)viewport[3]-(float)sy[1];

			if(k==1)glColor4f(0.,0.,1.,0.75);	
			else glColor4f(1.,0.,0.,0.75);	
			if(sx[0]>=0&&sx[0]<=image_width
			&&sy[0]>=0&&sy[0]<=image_height&&p[0][2]<=eye[2])
			{	
				tmp2 = dataPos[i] + arrow_length*dataDir[i];
				tmp1[0]  = tmp2[0] * m[0] +
					tmp2[1] * m[4] +
					tmp2[2] * m[8] +
					m[12] + tmp1[0];
				tmp1[1]  = tmp2[0] * m[1] +
					  tmp2[1] * m[5] +
					  tmp2[2] * m[9] +
					  m[13] + tmp1[1];
				tmp1[2]  = tmp2[0] * m[2] +
					   tmp2[1] * m[6] +
					   tmp2[2] * m[10] +
					   m[14] + tmp1[2];
				gluProject((double)(tmp1[0]), 
					(double)(tmp1[1]), 
					(double)(tmp1[2]), 
					vmat, pmat, viewport, &sx[2],&sy[2],&sz[2]);
				sy[2] = (float)viewport[3]-(float)sy[2];
				dir[0] = sx[2]-sx[0];
				dir[1] = sy[2]-sy[0];
				dir[2] = 0;
				dir =  normalize(dir);
				if(dir[0]!=0)
				{
					dir2[1]=sqrt(1/(1+(dir[1]*dir[1])/(dir[0]*dir[0])));
					dir2[0]=-dir2[1] * dir[1] / dir[0];dir2[2]=0;
				}
				else
				{dir2[0]=1;dir[1]=0;dir[2]=0;}
				glBegin(GL_TRIANGLES);
				glVertex2f(sx[0],sy[0]);
				glVertex2f(sx[0]+dir2[0]*17-10.*dir[0],sy[0]+dir2[1]*17-10.*dir[1]);
				glVertex2f(sx[0]+dir2[0]*17+10.*dir[0],sy[0]+dir2[1]*17+10.*dir[1]);		
				glEnd();
				if(k==0)
				{
					glRasterPos2f(sx[0]+20.,sy[0]+20.);
					glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'A');
				}
				else
				{
					if(k==1)
					{
						glRasterPos2f(sx[0]+20.,sy[0]+20.);
						glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'B');
					}
				}
				tmp1[0]=0.;tmp1[1]=0.;tmp1[2]=0.;
			}
			if(sx[1]>=0&&sx[1]<=image_width
			&&sy[1]>=0&&sy[1]<=image_height&&p[1][2]<=eye[2])
			{	
				tmp2 = end- arrow_length*dataDir[i];
				tmp1[0]  = tmp2[0] * m[0] +
					tmp2[1] * m[4] +
					tmp2[2] * m[8] +
					m[12] + tmp1[0];
				tmp1[1]  = tmp2[0] * m[1] +
					  tmp2[1] * m[5] +
					  tmp2[2] * m[9] +
					  m[13] + tmp1[1];
				tmp1[2]  = tmp2[0] * m[2] +
					   tmp2[1] * m[6] +
					   tmp2[2] * m[10] +
					   m[14] + tmp1[2];
				gluProject((double)(tmp1[0]), 
					(double)(tmp1[1]), 
					(double)(tmp1[2]), 
					vmat, pmat, viewport, &sx[2],&sy[2],&sz[2]);
				sy[2] = (float)viewport[3]-(float)sy[2];
				dir[0] = sx[2]-sx[1];
				dir[1] = sy[2]-sy[1];
				dir[2] = 0;
				dir =  normalize(dir);
				if(dir[0]!=0)
				{
					dir2[1]=sqrt(1/(1+(dir[1]*dir[1])/(dir[0]*dir[0])));
					dir2[0]=-dir2[1] * dir[1] / dir[0];dir2[2]=0;
				}
				else
				{dir2[0]=1;dir[1]=0;dir[2]=0;}
				glBegin(GL_TRIANGLES);
				glVertex2f(sx[1],sy[1]);
				glVertex2f(sx[1]+dir2[0]*17-10.*dir[0],sy[1]+dir2[1]*17-10.*dir[1]);
				glVertex2f(sx[1]+dir2[0]*17+10.*dir[0],sy[1]+dir2[1]*17+10.*dir[1]);		
				glEnd();
			}
		}
	}

    glMatrixMode(GL_PROJECTION);
    glLoadMatrixd(pmat);
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixd(vmat);
}
void QDOTglyph::RenderTask4Marker(GLfloat *m)
{

    GLdouble vmat[16], pmat[16], sx[3], sy[3], sz[3];
    GLint    viewport[4];
	glEnable(GL_BLEND);

    glGetDoublev(GL_MODELVIEW_MATRIX, vmat);
    glGetDoublev(GL_PROJECTION_MATRIX, pmat);
    glGetIntegerv(GL_VIEWPORT, viewport);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0,(GLfloat) viewport[2], (GLfloat) viewport[3],0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

	svVector3 p[2];
	svVector3 tmp1,tmp2;
	svVector3 dir,dir2;
	int i;
	svVector3 end;
	float scale;

	for(int k=0;k<2;k++)
	{
		for(int j =0;j<2;j++)
		{
			p[j][0]=0.;p[j][1]=0.;p[j][2]=0.;	
		}
		tmp1[0]=0.;tmp1[1]=0.;tmp1[2]=0.;
		//tmp2[0]=0.;tmp2[1]=0.;tmp2[2]=0.;
		if(taskarrow_task4[k]!=-1)
		{
			i = taskarrow_task4[k];
			p[0][0]  = dataPos[i][0] * m[0] +
				dataPos[i][1] * m[4] +
				dataPos[i][2] * m[8] +
				m[12] + p[0][0];
			p[0][1]  = dataPos[i][0] * m[1] +
				  dataPos[i][1] * m[5] +
				  dataPos[i][2] * m[9] +
				  m[13] + p[0][1];
			p[0][2]  = dataPos[i][0] * m[2] +
				   dataPos[i][1] * m[6] +
				   dataPos[i][2] * m[10] +
				   m[14] + p[0][2];
			gluProject((double)(p[0][0]), 
				(double)(p[0][1]), 
				(double)(p[0][2]), 
				vmat, pmat, viewport, &sx[0],&sy[0],&sz[0]);
			sy[0] = (float)viewport[3]-(float)sy[0];

			scale =minl_distribute*(exp[i]);
			if(ENCODE_MODE==2&&scale>minl_distribute*coe[i])
			{
				end = dataPos[i] + scale*dataDir[i];
			}
			else
			{
				end = dataEnd[i];	
			}

			p[1][0]  = end[0] * m[0] +
				end[1] * m[4] +
				end[2] * m[8] +
				m[12] + p[1][0];
			p[1][1]  = end[0] * m[1] +
				  end[1] * m[5] +
				  end[2] * m[9] +
				  m[13] + p[1][1];
			p[1][2]  = end[0] * m[2] +
				   end[1] * m[6] +
				   end[2] * m[10] +
				   m[14] + p[1][2];

			gluProject((double)(p[1][0]), 
				(double)(p[1][1]), 
				(double)(p[1][2]), 
				vmat, pmat, viewport, &sx[1],&sy[1],&sz[1]);
			sy[1] = (float)viewport[3]-(float)sy[1];

			if(k==1)glColor4f(1.,1.,0.,0.5);	
			else glColor4f(1.,0.,0.,0.5);	
			if(sx[0]>=0&&sx[0]<=image_width
			&&sy[0]>=0&&sy[0]<=image_height&&p[0][2]<=eye[2])
			{	
				tmp2 = dataPos[i] + arrow_length*dataDir[i];
				tmp1[0]  = tmp2[0] * m[0] +
					tmp2[1] * m[4] +
					tmp2[2] * m[8] +
					m[12] + tmp1[0];
				tmp1[1]  = tmp2[0] * m[1] +
					  tmp2[1] * m[5] +
					  tmp2[2] * m[9] +
					  m[13] + tmp1[1];
				tmp1[2]  = tmp2[0] * m[2] +
					   tmp2[1] * m[6] +
					   tmp2[2] * m[10] +
					   m[14] + tmp1[2];
				gluProject((double)(tmp1[0]), 
					(double)(tmp1[1]), 
					(double)(tmp1[2]), 
					vmat, pmat, viewport, &sx[2],&sy[2],&sz[2]);
				sy[2] = (float)viewport[3]-(float)sy[2];
				dir[0] = sx[2]-sx[0];
				dir[1] = sy[2]-sy[0];
				dir[2] = 0;
				dir =  normalize(dir);
				if(dir[0]!=0)
				{
					dir2[1]=sqrt(1/(1+(dir[1]*dir[1])/(dir[0]*dir[0])));
					dir2[0]=-dir2[1] * dir[1] / dir[0];dir2[2]=0;
				}
				else
				{dir2[0]=1;dir[1]=0;dir[2]=0;}
				glBegin(GL_TRIANGLES);
				glVertex2f(sx[0],sy[0]);
				glVertex2f(sx[0]+dir2[0]*17-10.*dir[0],sy[0]+dir2[1]*17-10.*dir[1]);
				glVertex2f(sx[0]+dir2[0]*17+10.*dir[0],sy[0]+dir2[1]*17+10.*dir[1]);		
				glEnd();
				tmp1[0]=0.;tmp1[1]=0.;tmp1[2]=0.;
			}
			if(sx[1]>=0&&sx[1]<=image_width
			&&sy[1]>=0&&sy[1]<=image_height&&p[1][2]<=eye[2])
			{	
				tmp2 = end- arrow_length*dataDir[i];
				tmp1[0]  = tmp2[0] * m[0] +
					tmp2[1] * m[4] +
					tmp2[2] * m[8] +
					m[12] + tmp1[0];
				tmp1[1]  = tmp2[0] * m[1] +
					  tmp2[1] * m[5] +
					  tmp2[2] * m[9] +
					  m[13] + tmp1[1];
				tmp1[2]  = tmp2[0] * m[2] +
					   tmp2[1] * m[6] +
					   tmp2[2] * m[10] +
					   m[14] + tmp1[2];
				gluProject((double)(tmp1[0]), 
					(double)(tmp1[1]), 
					(double)(tmp1[2]), 
					vmat, pmat, viewport, &sx[2],&sy[2],&sz[2]);
				sy[2] = (float)viewport[3]-(float)sy[2];
				dir[0] = sx[2]-sx[1];
				dir[1] = sy[2]-sy[1];
				dir[2] = 0;
				dir =  normalize(dir);
				if(dir[0]!=0)
				{
					dir2[1]=sqrt(1/(1+(dir[1]*dir[1])/(dir[0]*dir[0])));
					dir2[0]=-dir2[1] * dir[1] / dir[0];dir2[2]=0;
				}
				else
				{dir2[0]=1;dir[1]=0;dir[2]=0;}
				glBegin(GL_TRIANGLES);
				glVertex2f(sx[1],sy[1]);
				glVertex2f(sx[1]+dir2[0]*17-10.*dir[0],sy[1]+dir2[1]*17-10.*dir[1]);
				glVertex2f(sx[1]+dir2[0]*17+10.*dir[0],sy[1]+dir2[1]*17+10.*dir[1]);		
				glEnd();
			}
		}
	}

    glMatrixMode(GL_PROJECTION);
    glLoadMatrixd(pmat);
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixd(vmat);
}
void QDOTglyph::RenderText(GLfloat *m)
{
    GLdouble vmat[16], pmat[16], sx, sy, sz;
	GLdouble tmpsx,tmpsy,tmpsz;
    GLint    viewport[4];

    glGetDoublev(GL_MODELVIEW_MATRIX, vmat);
    glGetDoublev(GL_PROJECTION_MATRIX, pmat);
    glGetIntegerv(GL_VIEWPORT, viewport);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0,(GLfloat) viewport[2], (GLfloat) viewport[3],0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

	shot_w = image_width/(7.*10.);
	shot_h = image_height/15.;

	if(ifshot.size()!=0)
		ifshot.clear();
	for(int t=0;t<shot_h;t++)
		for(int k=0;k<shot_w;k++)
			ifshot.push_back(false);

	char str[20];
	float scale = 0.01;
	svVector3 end;
	svVector3 middle;
	int i;
	int s,e;
	int index;
	bool flag=false;

	svVector3 tmpp;


		s = 0;
		e = dataSize; 


	svVector3 *p = new svVector3[e];
        for(int j =s;j<e;j++)
        {
		p[j][0]=0.;p[j][1]=0.;p[j][2]=0.;	
	}

		for(int j =0;j<dataSize;j++)
		{
			p[j][0]=0.;p[j][1]=0.;p[j][2]=0.;	
		}
		s=0;e=dataSize;
		for(int j =s;j<e;)
		{

			i=j;flag=true;

			glColor4f(1.,1.,1.,1);

			middle = dataPos[i];
		
			p[j][0]  = middle[0] * m[0] +
				    middle[1] * m[4] +
				    middle[2] * m[8] +
				    m[12] + p[j][0];
		        p[j][1]  = middle[0] * m[1] +
		                    middle[1] * m[5] +
		                    middle[2] * m[9] +
		                    m[13] + p[j][1];
		        p[j][2]  = middle[0] * m[2] +
		                    middle[1] * m[6] +
		                    middle[2] * m[10] +
		                    m[14] + p[j][2];
			gluProject((double)(p[j][0]), 
				  (double)(p[j][1]), 
				  (double)(p[j][2]), 
				  vmat, pmat, viewport, &sx,&sy,&sz);
			 sy = (float)viewport[3]-(float)sy;
			sprintf(str, "%.2fe+%.0f",coe[i], exp[i]);
			if(sx>=0&&sx<=image_width&&sy>=0&&sy<=image_height&&p[j][2]<=eye[2])
			{
				tmpsx = sx;tmpsy=sy;tmpsz=sz;
				if(FindShot(sx,sy))
				{
					glRasterPos2f(sx, sy);
					for(int ii=0;ii<strlen(str);ii++)
						glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[ii]);	
					glBegin(GL_LINES);
					glVertex2f(sx,sy);
					glVertex2f(tmpsx,tmpsy);
					glEnd();
				}
			}
			j++;
		}

	delete [] p;

    glMatrixMode(GL_PROJECTION);
    glLoadMatrixd(pmat);
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixd(vmat);
}

bool QDOTglyph::FindShot(double &x, double &y)
{
	int xi,yi;
	yi = y/15;
	xi = x/(7.*10.);
	int index = (int)(yi * shot_w) + (int)(xi);
	float indexx,indexy;

	indexy = (yi+1) * 15.;
	indexx = xi* (7.*10.);

	if(index<0||index>=ifshot.size())
		return false;	
	if(ifshot[index]==false)
	{
		x = indexx;
		y = indexy;
		ifshot[index]=true;
		return true;
	}
	else
	{
		if(x>=image_width/2.)
		{
			if(index-1>=0)
			{
				if(ifshot[index-1]==false)
				{x=indexx-7.*10.;y=indexy;ifshot[index-1]=true;return true;}
				else return false;
			}
			/*else if(ifshot[index-1-shot_w]==false)
			{x=x-8*15;y=y-15;ifshot[index-1-shot_w]=true;return true;}
			else if(ifshot[index-1+shot_w]==false)
			{x=x-8*15;y=y+15;ifshot[index-1+shot_w]=true;return true;}*/
			else return false;
		}
		else
		{
			if(index+1<ifshot.size())
			{
				if(ifshot[index+1]==false)
				{x=indexx+7.*10.;y=indexy;ifshot[index+1]=true;return true;}
				else return false;
			}
			/*else if(ifshot[index+1-shot_w]==false)
			{x=x+8*15;y=y-15;ifshot[index+1-shot_w]=true;return true;}
			else if(ifshot[index+1+shot_w]==false)
			{x=x+8*15;y=y+15;ifshot[index+1+shot_w]=true;return true;}*/
			else return false;
		}
	}
}

void QDOTglyph::RenderBounding()
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
	svVector3 arrow[3][NUM_SEG+1];
	svVector3 arrownorm[3][NUM_SEG+1];
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
		point[i]=end+arrow_length/3*dir;
		norm = svGetPerpendicularVector(dir);
		head = end + arrow_radius/3 * norm;

		for(int j=0;j<NUM_SEG;j++)
		{
			arrow[i][j] = svGetRotatePoint(head, end, dir, j * 360/NUM_SEG);
		}
		for(int j=0;j<NUM_SEG;j++)
		{
			if(j!=NUM_SEG-1)
				arrownorm[i][j+1] = svGetNorm(arrow[i][j],point[i],arrow[i][j+1]);
			else
				arrownorm[i][j+1] = svGetNorm(arrow[i][j],point[i],arrow[i][0]);
			arrownorm[i][j+1] = normalize(arrownorm[i][j+1]);
		}
		arrownorm[i][0] = dir;
		for(int j=1;j<NUM_SEG+1;j++)
		{
			if(j!=1)
				arrownorm[i][j] = (arrownorm[i][j] + arrownorm[i][j-1])/2.;
			else
				arrownorm[i][j] = (arrownorm[i][j] + arrownorm[i][NUM_SEG])/2.;
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
	for(int i=0;i<NUM_SEG;i++)
	{
		glBegin(GL_TRIANGLES);
		glNormal3f(arrownorm[0][0][0],arrownorm[0][0][1],arrownorm[0][0][2]);
		glVertex3f(point[0][0],point[0][1],point[0][2]);
		glNormal3f(arrownorm[0][i+1][0],arrownorm[0][i+1][1],arrownorm[0][i+1][2]);
		glVertex3f(arrow[0][i][0],arrow[0][i][1],arrow[0][i][2]);
		if(i!=NUM_SEG-1)
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
	for(int i=0;i<NUM_SEG;i++)
	{
		glBegin(GL_TRIANGLES);
		glNormal3f(arrownorm[1][0][0],arrownorm[1][0][1],arrownorm[1][0][2]);
		glVertex3f(point[1][0],point[1][1],point[1][2]);
		glNormal3f(arrownorm[1][i+1][0],arrownorm[1][i+1][1],arrownorm[1][i+1][2]);
		glVertex3f(arrow[1][i][0],arrow[1][i][1],arrow[1][i][2]);
		if(i!=NUM_SEG-1)
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
	for(int i=0;i<NUM_SEG;i++)
	{
		glBegin(GL_TRIANGLES);
		glNormal3f(arrownorm[2][0][0],arrownorm[2][0][1],arrownorm[2][0][2]);
		glVertex3f(point[2][0],point[2][1],point[2][2]);
		glNormal3f(arrownorm[2][i+1][0],arrownorm[2][i+1][1],arrownorm[2][i+1][2]);
		glVertex3f(arrow[2][i][0],arrow[2][i][1],arrow[2][i][2]);
		if(i!=NUM_SEG-1)
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

void QDOTglyph::RenderDistributeLegend()
{
    GLdouble vmat[16], pmat[16], sx[3], sy[3], sz[3];
    GLint    viewport[4];
	glEnable(GL_BLEND);
    glGetDoublev(GL_MODELVIEW_MATRIX, vmat);
    glGetDoublev(GL_PROJECTION_MATRIX, pmat);
    glGetIntegerv(GL_VIEWPORT, viewport);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0,(GLfloat) viewport[2], (GLfloat) viewport[3],0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();	

  char str[10];
  float x = viewport[2]-150.;
  float y = viewport[3]-100.;
  float size = 10.;	

	x = viewport[2]*4.5/5.;
	y = y + size*3;

	glColor3f(1.,1.,1.);
	glBegin(GL_LINE_LOOP);
	glVertex2f(x,y);
	glVertex2f(x+size*1.7,y);
	glVertex2f(x+size*1.7,y+size*5);
	glVertex2f(x,y+size*5);
	glEnd();
	glRasterPos2f(x+size*1.8,y);
	sprintf(str,"exponent");
	for(int ii=0;ii<strlen(str);ii++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[ii]);

	glBegin(GL_LINE_LOOP);
	glVertex2f(x+size*0.35,y-size);
	glVertex2f(x+size*1.35,y-size);
	glVertex2f(x+size*1.35,y+size*5);
	glVertex2f(x+size*0.35,y+size*5);
	glEnd();
	glRasterPos2f(x+size*1.45,y-size);
	sprintf(str,"coefficient");
	for(int ii=0;ii<strlen(str);ii++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[ii]);
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixd(pmat);
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixd(vmat);
}

void QDOTglyph::SetTask4Neighbours(int *n1, int *n2)
{
	for(int i=0;i<12;i++)
	{
		task4N1[i] = n1[i];//cerr<<n1[i]<<" ";
		task4N2[i] = n2[i];//cerr<<n2[i]<<" ";
	}

	float td;
	float dd = 9e+9;
	for(int i=0;i<12;i++)
	{
		td=dataDir[task4_index[0]][0] * dataDir[n1[i]][0]
		+ dataDir[task4_index[0]][1] * dataDir[n1[i]][1]
		+ dataDir[task4_index[0]][2] * dataDir[n1[i]][2];
		if(td<dd)
		{
			task4N1L = n1[i];dd = td;
		}
	}
	dd = 9e+9;
	for(int i=0;i<12;i++)
	{
		td=dataDir[task4_index[1]][0] * dataDir[n2[i]][0]
		+ dataDir[task4_index[1]][1] * dataDir[n2[i]][1]
		+ dataDir[task4_index[1]][2] * dataDir[n2[i]][2];
		if(td<dd)
		{
			task4N2L = n2[i];dd = td;
		}
	}

	//cerr<<task4N1L<<" "<<task4N2L<<endl;
}

void QDOTglyph::RenderTask1(int num,int w,int h)
{
	taskarrow[0] = task1_index;
	taskarrow[1] = -1;
	//cerr<<dataDen[taskarrow[0]]<<endl;
	glPushMatrix();
	glColor3f(1.,1.,1.);
	char *question = strdup("What is the magnitude at point A?");
	char str[100];
	sprintf(str,"Task 1. %s",question);
	glRasterPos2f(w/3., h-h/20.);
	for(int i=0;i<strlen(str);i++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[i]);	
	glPopMatrix();
}

void QDOTglyph::RenderTask2(int w,int h)
{
	glColor3f(1.,1.,1.);
	char str[100];
	char *question;
	question=strdup("What is the ratio between the vectors at points A and B?");
	sprintf(str,"Task 2. %s", question);
	glRasterPos2f(w/3., h-h/20.);
	for(int i=0;i<strlen(str);i++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[i]);	
	taskarrow[0]=task2_index[0];
	taskarrow[1]=task2_index[1];
}

void QDOTglyph::RenderTask3(int num1,int num2,int w,int h) 
{
	//cerr<<num1<<" "<<num2<<endl;
	taskarrow[0]=task3_index[num1];
	taskarrow[1]=task3_index[num2];	
	//cerr<<"task "<<taskarrow[0]<<" "<<taskarrow[1]<<endl;
	glColor3f(1.,1.,1.);
	char str[100];
	sprintf(str, "Task 3. Which magnitude is larger, point A or point B?");
	glRasterPos2f(w/3., h-h/20.);
	for(int i=0;i<strlen(str);i++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[i]);	
}

void QDOTglyph::RenderTask4(int num1,int num2,int w,int h) 
{
	taskarrow[0]=task4_index[num1];
	taskarrow[1]=task4_index[num2];	
	if(num1==0)
	{
		taskarrow_task4[0]=task4N1L;
		taskarrow_task4[1]=task4N2L;
	}
	else
	{
		taskarrow_task4[0]=task4N2L;
		taskarrow_task4[1]=task4N1L;
	}
	glColor3f(1.,1.,1.);
	char str[100];
	sprintf(str, "Task 4. Which angle is larger,  between the two angles marked by two vectors at each of A and B?");
	glRasterPos2f(w/4., h-h/20.);
	for(int i=0;i<strlen(str);i++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[i]);	
}


void QDOTglyph::RenderQuestionN(int w, int h)
{
	glColor3f(1.,1.,1.);
	char str[100];
	sprintf(str,"%d/12",questionnum);
	glRasterPos2f(w/40., h/40.);
	for(int i=0;i<strlen(str);i++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[i]);	
}

void QDOTglyph::GetCorrect(int num,int task,char *name)
{
	ofstream outfile(name,std::ios_base::app | std::ios_base::out);
	if(task==1)
	{
		outfile<<dataDen[task1_index]<<endl;
	}
	else if(task==2)
	{
		outfile<<abs(dataDen[task2_index[0]]-dataDen[task2_index[1]])<<endl;
	}
	else if(task==3)
	{
		//outfile<<dataDen[randomsample[num-minExp]]<<endl;
	}
	outfile.close();
}
float QDOTglyph::GetCorrectAnswer(int num1,int num2,int task)
{
	correctflag = 0;
	float a;
	int n;

	if(task==0)
	{
		a = dataDen[task1_index];
	}
	else if(task==1)
	{
		a = abs(dataDen[task2_index[0]]/dataDen[task2_index[1]]);
		task3_den[num1] = dataDen[task2_index[0]];
		task3_den[1-num1] = dataDen[task2_index[1]]; 
	}
	else if(task==2)
	{
		if(num1==0)
			a = 0;
		else
			a = 1;
		task3_den[num1] = dataDen[task3_index[0]];
		task3_den[1-num1] = dataDen[task3_index[1]]; 
	}
	else if(task==3)
	{
		if(num1==0)
			a = 0;
		else a = 1;
	}

	//cerr<<a<<endl;
	return a;
}
void QDOTglyph::cleanup()
{
	delete [] dataPos;
	delete [] dataDir;
	delete [] dataDen;
	delete [] exp;
	delete [] coe;
	delete [] dataCol;
	delete [] dataEnd;
	delete [] dataEnd2;
	delete [] dataPoint;

	for(int i=0;i<NUM_SEG;i++)
		glyphArrowPos[i].free();	
	delete [] glyphArrowPos;
	glyphArrowPos = NULL;

	for(int i=0;i<NUM_SEG+1;i++)
		glyphArrowNorm[i].free();	
	delete [] glyphArrowNorm;
	glyphArrowNorm = NULL;
}

QDOTglyph::~QDOTglyph()
{
cleanup();
	
}


