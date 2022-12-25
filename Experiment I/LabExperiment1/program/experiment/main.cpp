#include <stdio.h>
#include <stdlib.h>
#include <iomanip>
#ifdef __APPLE__
  #include <GLUT/glut.h>
#else
  #include <GL/glut.h>
#endif

#include <iostream>
#include <fstream>
#include <math.h>
#include <assert.h>
#include <time.h>
#include <sys/time.h>
#include <ctime>
#include <cmath>
#include <string.h>

#include "ReadData.h"
#include "svQDOTDrawing.h"

#include "./jcUtil/MArray.h"
#include "./jcUtil/MGL.h"
#include "./jcUtil/MVectorMatrix.h"
#include "ivTrackball.h"

#define number_trials 4
#define number_tasks 4
#define number_encoding 5
#define FPS 1
#include <sstream>
using std::ostringstream;

using namespace __svl_lib;
using namespace std;

int task4_answer_index = -1;

char *dataDir;
char *answername;
char *correctname;
char *timename;
char *confidentname;
char *namelog;
ofstream answerfile;

int datacount = -1;
int dataorder[ number_tasks][number_encoding][number_trials];
int binaryanswer[number_trials];
int num_page = -1;
int num_task = 0;
int num_mode = 0;
vector< vector<int> > squareorder;//[4][4]= {1,2,3,4,
			//2,1,4,3,
			//3,4,1,2,
			//4,3,2,1};
int num_data[8];
int mode_order[number_encoding];
int page_order[165]=
{0,1,2,1,2,1,2,1,2,
1,2,1,2,1,2,1,2,  
1,2,1,2,1,2,1,2,    
1,2,1,2,1,2,1,2,  
1,2,1,2,1,2,1,2, 
0,1,2,1,2,1,2,1,2, 
1,2,1,2,1,2,1,2,   
1,2,1,2,1,2,1,2,  
1,2,1,2,1,2,1,2,
1,2,1,2,1,2,1,2,
0,1,2,1,2,1,2,1,2,  
1,2,1,2,1,2,1,2,   
1,2,1,2,1,2,1,2,  
1,2,1,2,1,2,1,2,   
1,2,1,2,1,2,1,2,  
0,1,2,1,2,1,2,1,2,  
1,2,1,2,1,2,1,2,   
1,2,1,2,1,2,1,2,  
1,2,1,2,1,2,1,2,   
1,2,1,2,1,2,1,2,  
3
};
int page_count = 0;
bool page_next = false;
double timefornext;

int displaylist = -1;

vector<char> TaskKeyAnswer;
float useranswer;
float confidentlevel;
float correctanswer;

bool ifpause = false;
int ifnext = 0;
bool ifshowanswer = false;
bool ifconfident = false;
bool ifselectanswer = false;


svQDOTDrawing *myDraw;
dataSet *myData;

int image_width=1000;
int image_height=1000;
GLdouble vmat[16], pmat[16];
GLint    viewport[4];
GLdouble vmat_left[16], pmat_left[16];
GLint    viewport_left[4];
GLdouble vmat_right[16], pmat_right[16];
GLint    viewport_right[4];

Trackball trackball;
GLfloat m[16];

float movex,movey;
float mousex,mousey;
int mousebutton;
int mousestate;

float rotatefactor[4];
float zoomfactor;
float panfactor[4];
char keystate;

time_t begin_time;
time_t end_time;
double wholetime = 0;
svVector3 l,r;
time_t nowtime;
bool startclock = false;       // True if clock is running
int hour=0, min=0, sec=0;     // time variables
string timestring="00:00:00";
double GetTime()
{
  timeval tv;
  gettimeofday (&tv, NULL);
  return double (tv.tv_sec) + 0.000001 * tv.tv_usec;
}

void
ReadFile()  //setup random data 
{
   myData->SetRandom(number_tasks, number_encoding, number_trials);//cerr<<"1"<<endl;
   myData->SetTask("data/task1_data","pointorder");//cerr<<"1"<<endl;
   myData->SetTask("data/task2_data","pointorder");//cerr<<"1"<<endl;
   myData->SetTask("data/task3_data","pointorder");//cerr<<"1"<<endl;
   myData->SetTask("data/task4_data","pointorder");//cerr<<"1"<<endl;
}

void Draw_Progress()
{
 	glColor3f(1.,1.,1.);
	char str[100];
	sprintf(str,"%d/80",datacount+1);
	glRasterPos2f(image_width/40., image_height/40.);
	for(int i=0;i<strlen(str);i++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[i]);	 
}

void 
Draw_Page()
{
    float x, y;
    char str[40];
    char *tmps;
    x = viewport[2]*4.5/5.-50.;	
    y = viewport[3]*9./10.;
	glColor3f(0.,0.,0.);
    tmps = strdup("next");
	glRasterPos2f(x+50., y-20.);
	sprintf(str,"%s",tmps);
	for(int i=0;i<strlen(str);i++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[i]);	

    if(movex>=x&&movex<=x+150.
	&&movey>=y-50.&&movey<=y)
	glColor3f(1.,1.,1.);
    else
   	 glColor3f(0.5,0.5,0.5);

    glBegin(GL_QUADS);
    glVertex2f(x+5.,y-5.);
    glVertex2f(x+150.-5.,y-5.);
    glVertex2f(x+150.-5.,y-50.+5.);
    glVertex2f(x+5.,y-50.+5.);
    glEnd();	

    glColor3f(0.2,0.2,0.2);
    glBegin(GL_QUADS);
    glVertex2f(x+5.,y);
    glVertex2f(x+150.,y);
    glVertex2f(x+150,y-50.+5.);
    glVertex2f(x+5.,y-50.+5.);
    glEnd();	

    glColor3f(0.8,0.8,0.8);
    glBegin(GL_QUADS);
    glVertex2f(x,y);
    glVertex2f(x+150.,y);
    glVertex2f(x+150.,y-50.);
    glVertex2f(x,y-50.);
    glEnd();

}

void 
Draw_Done()
{
    float x, y;
    char str[40];
    char *tmps;
    x = viewport[2]*4.5/5.-50.;	
    y = viewport[3]*9./10.;

	glColor3f(0.,0.,0.);
    tmps = strdup("done");
	glRasterPos2f(x+50., y-20.);
	sprintf(str,"%s",tmps);
	for(int i=0;i<strlen(str);i++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[i]);	

    if(movex>=x&&movex<=x+150.
	&&movey>=y-50.&&movey<=y)
	glColor3f(1.,1.,1.);
    else
   	 glColor3f(0.5,0.5,0.5);

    glBegin(GL_QUADS);
    glVertex2f(x+5.,y-5.);
    glVertex2f(x+150.-5.,y-5.);
    glVertex2f(x+150.-5.,y-50.+5.);
    glVertex2f(x+5.,y-50.+5.);
    glEnd();	

    glColor3f(0.2,0.2,0.2);
    glBegin(GL_QUADS);
    glVertex2f(x+5.,y);
    glVertex2f(x+150.,y);
    glVertex2f(x+150,y-50.+5.);
    glVertex2f(x+5.,y-50.+5.);
    glEnd();	

    glColor3f(0.8,0.8,0.8);
    glBegin(GL_QUADS);
    glVertex2f(x,y);
    glVertex2f(x+150.,y);
    glVertex2f(x+150.,y-50.);
    glVertex2f(x,y-50.);
    glEnd();
}


void 
Draw_Label()
{

	glColor3f(1.,1.,1.);
	char str[40];
	char *input = strdup("Please input your answer.");
	sprintf(str,"%s",input);
	glRasterPos2f(image_width/3.,image_height/2.-image_height/18.);
	for(int i=0;i<strlen(str);i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[i]);
	}	


	glColor3f(0.,0.,0);
	glRasterPos2f(image_width/3.,image_height/2.);
	char stra[100];
	//cerr<<image_width/3.<<" "<<image_height/2.<<endl;
	for(int i=0;i<TaskKeyAnswer.size();i++)
	{
		//cerr<<TaskKeyAnswer[i]<<" ";
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, TaskKeyAnswer[i]);
		stra[i] = TaskKeyAnswer[i];
	}	

	if(TaskKeyAnswer.size()!=0)
	{	ifselectanswer=true;
		useranswer = atof(stra);
	}
	else
		useranswer = 0.;
	glColor3f(1.,1.,1.);
	glBegin(GL_QUADS);
	glVertex2f(image_width/3.,image_height/2.-image_height/60.);
	glVertex2f(image_width/3.,image_height/2.+image_height/120.);
	glVertex2f(image_width/3.+image_width/10.,image_height/2.+image_height/120.);
	glVertex2f(image_width/3.+image_width/10.,image_height/2.-image_height/60.);
	glEnd();
}

void 
Draw_End()
{

	
	glColor3f(1.,1.,1.);
    float x, y;
    char str[40];
    char *tmps = strdup("Thanks.");
    x = viewport[2]/2.;	
    y = viewport[3]/2.;
    glRasterPos2f(x, y);
    sprintf(str,"%s",tmps);
    for(int i=0;i<strlen(str);i++)
	glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[i]);	
    glRasterPos2f(x, y+100.);
    sprintf(str,"All the tasks are done.");
    for(int i=0;i<strlen(str);i++)
	glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[i]);	


	x = viewport[2]/2.;	
	y = y + 200.;
    glColor3f(0.,0.,0.);
    tmps = strdup("Exit");
	glRasterPos2f(x+50., y-20.);
	sprintf(str,"%s",tmps);
	for(int i=0;i<strlen(str);i++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[i]);	

    if(movex>=x&&movex<=x+150.
	&&movey>=y-50.&&movey<=y)
	glColor3f(1.,1.,1.);
    else
   	 glColor3f(0.5,0.5,0.5);

    glBegin(GL_QUADS);
    glVertex2f(x+5.,y-5.);
    glVertex2f(x+150.-5.,y-5.);
    glVertex2f(x+150.-5.,y-50.+5.);
    glVertex2f(x+5.,y-50.+5.);
    glEnd();	

    glColor3f(0.2,0.2,0.2);
    glBegin(GL_QUADS);
    glVertex2f(x+5.,y);
    glVertex2f(x+150.,y);
    glVertex2f(x+150,y-50.+5.);
    glVertex2f(x+5.,y-50.+5.);
    glEnd();	

    glColor3f(0.8,0.8,0.8);
    glBegin(GL_QUADS);
    glVertex2f(x,y);
    glVertex2f(x+150.,y);
    glVertex2f(x+150.,y-50.);
    glVertex2f(x,y-50.);
    glEnd();

    if(mousex>=x&&mousex<=x+150.
	&&mousey>=y-50.&&mousey<=y)
	exit(0);
}

void 
Draw_Time()
{
    float x, y;
    char str[40];
    char *tmps = strdup("pause");
    x = 50;//viewport[2]*4.5/5.-50.;	
    y = viewport[3]/10.+50;

	glColor3f(0.,0.,0.);
    if(!ifpause)
    	tmps = strdup("pause");
    else
    	tmps = strdup("resume");
	glRasterPos2f(x+50., y-20.);
	sprintf(str,"%s",tmps);
	for(int i=0;i<strlen(str);i++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[i]);	

    if(movex>=x&&movex<=x+150.
	&&movey>=y-50.&&movey<=y)
	glColor3f(1.,1.,1.);
    else
   	 glColor3f(0.5,0.5,0.5);

    glBegin(GL_QUADS);
    glVertex2f(x+5.,y-5.);
    glVertex2f(x+150.-5.,y-5.);
    glVertex2f(x+150.-5.,y-50.+5.);
    glVertex2f(x+5.,y-50.+5.);
    glEnd();	

    glColor3f(0.2,0.2,0.2);
    glBegin(GL_QUADS);
    glVertex2f(x+5.,y);
    glVertex2f(x+150.,y);
    glVertex2f(x+150,y-50.+5.);
    glVertex2f(x+5.,y-50.+5.);
    glEnd();	

    glColor3f(0.8,0.8,0.8);
    glBegin(GL_QUADS);
    glVertex2f(x,y);
    glVertex2f(x+150.,y);
    glVertex2f(x+150.,y-50.);
    glVertex2f(x,y-50.);
    glEnd();

}


void Draw_OTHER() //hard coding
{	/*glColor3f(1.,1.,1.);
	char str[40];
	char *s;
	if(mode_order[num_mode] == 0)
	{
		s = strdup("Length(exponent) Length(coefficient)");
		sprintf(str,"%s",s);
	}
	else if(mode_order[num_mode] == 1)
	{
		s = strdup("Length(exponent) Width(coefficient)");
		sprintf(str,"%s",s);
	}
	else if(mode_order[num_mode] == 2)
	{
		s = strdup("Hue(exponent) Length(coefficient)");
		sprintf(str,"%s",s);
	}
	else if(mode_order[num_mode] == 3)
	{
		s = strdup("Texture(exponent) Length(coefficient)");
		sprintf(str,"%s",s);
	}
	else if(mode_order[num_mode] == 4) 
	{
		s = strdup("Hue(exponent) Texture(coefficient)");
		sprintf(str,"%s",s);
	}
  	float  x = viewport[2]*3.7/5.;	
    	float  y = viewport[3]- viewport[3]/20.;
	glRasterPos2f(x,y);
	for(int i=0;i<strlen(str);i++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[i]);	*/
}

void Draw_Level()
{

	glColor3f(1.,1.,1.);
	char str[100];
	char *s = strdup("Please select your confidence level for this task.");
	sprintf(str,"%s",s);
	//cerr<<str<<endl;

  	float x = viewport[2]/3.;
    	float y = viewport[3]/2.+viewport[3]/20.;
	glRasterPos2f(x,y);
	for(int i=0;i<strlen(str);i++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[i]);	

	s = strdup("least");
	sprintf(str,"%s",s);
	glRasterPos2f(x+ image_width/50.,y+ image_height/10.);
	for(int i=0;i<strlen(str);i++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[i]);	

	y = y + image_height/20.;
	for(int i=0;i<7;i++)
	{
		x = x + image_width/40.;
		if(mousex>=x-image_width/160.&&mousex<=x+image_width/80.
		&&mousey>=y-image_height/80.&&mousey<=y+image_height/40.)
		{
			ifconfident = true;
			confidentlevel=i+1;//cerr<<confidentlevel<<endl;
		}

		glColor3f(0.,0.,0.);
		sprintf(str,"%d",i+1);
		glRasterPos2f(x,y);
		for(int j=0;j<strlen(str);j++)
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[j]);

		if(confidentlevel == i+1)
			glColor3f(1.,1.,1.);
		else if(movex>=x-image_width/160.&&movex<=x+image_width/80.
		&&movey>=y-image_height/80.&&movey<=y+image_height/40.)
			glColor3f(1.,1.,1.);
		else
		   	glColor3f(0.5,0.5,0.5);

		    glBegin(GL_QUADS);
			glVertex2f(x-image_width/160.+5.,y+image_height/80.-5.);
			glVertex2f(x+image_width/80.-5.,y+image_height/80.-5.);
			glVertex2f(x+image_width/80.-5.,y-image_height/40.+5.);
			glVertex2f(x-image_width/160.+5.,y-image_height/40.+5.);
		    glEnd();	

		    glColor3f(0.2,0.2,0.2);
		    glBegin(GL_QUADS);
			glVertex2f(x-image_width/160.+5.,y+image_height/80.);
			glVertex2f(x+image_width/80.,y+image_height/80.);
			glVertex2f(x+image_width/80.,y-image_height/40.+5.);
			glVertex2f(x-image_width/160.+5.,y-image_height/40.+5.);
		    glEnd();	

		    glColor3f(0.8,0.8,0.8);
		glBegin(GL_QUADS);
		glVertex2f(x-image_width/160.,y+image_height/80.);
		glVertex2f(x+image_width/80.,y+image_height/80.);
		glVertex2f(x+image_width/80.,y-image_height/40.);
		glVertex2f(x-image_width/160.,y-image_height/40.);
		glEnd();

	}
	glColor3f(1.,1.,1.);
	s = strdup("most");
	sprintf(str,"%s",s);
	glRasterPos2f(x-+ image_width/150.,viewport[3]/2.+viewport[3]/20.+ image_height/10.);
	for(int i=0;i<strlen(str);i++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[i]);	

}

void
Draw_Task() //next
{
	glColor3f(1.,1.,1.);
	char str[200];

	if(num_task==0)
		sprintf(str, "Task 1. What is the magnitude at point A?");
	else if(num_task==1)
		sprintf(str, "Task 2. What is the ratio between the vectors at points A and B?");
	else if(num_task==2)
		sprintf(str, "Task 3. What magnitude is larger, point A or point B?");
	else if(num_task==3)
		sprintf(str, "Task 4. Which point has maximum magnitude when exponent is X?");

  	float x = viewport[2]/3.;	
    	float y = viewport[3]/2.;
	glRasterPos2f(x,y);
	for(int i=0;i<strlen(str);i++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[i]);	
    x = viewport[2]*4.5/5.-50.;	
    y = viewport[3]*9./10.;
    char *tmps;
    x = viewport[2]*4.5/5.-50.;	
    y = viewport[3]*9./10.;
	glColor3f(0.,0.,0.);
    tmps = strdup("next");
	glRasterPos2f(x+50., y-20.);
	sprintf(str,"%s",tmps);
	for(int i=0;i<strlen(str);i++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[i]);	

    if(movex>=x&&movex<=x+150.
	&&movey>=y-50.&&movey<=y)
	glColor3f(1.,1.,1.);
    else
   	 glColor3f(0.5,0.5,0.5);

    glBegin(GL_QUADS);
    glVertex2f(x+5.,y-5.);
    glVertex2f(x+150.-5.,y-5.);
    glVertex2f(x+150.-5.,y-50.+5.);
    glVertex2f(x+5.,y-50.+5.);
    glEnd();	

    glColor3f(0.2,0.2,0.2);
    glBegin(GL_QUADS);
    glVertex2f(x+5.,y);
    glVertex2f(x+150.,y);
    glVertex2f(x+150,y-50.+5.);
    glVertex2f(x+5.,y-50.+5.);
    glEnd();	

    glColor3f(0.8,0.8,0.8);
    glBegin(GL_QUADS);
    glVertex2f(x,y);
    glVertex2f(x+150.,y);
    glVertex2f(x+150.,y-50.);
    glVertex2f(x,y-50.);
    glEnd();
	
}

void Draw_EachTask()
{
	glColor3f(1.,1.,1.);
	char str[200];

	if(num_task==0)
		sprintf(str, "Task 1. What is the magnitude at point A?");
	else if(num_task==1)
		sprintf(str, "Task 2. What is the ratio between the vectors at points A and B?");
	else if(num_task==2)
		sprintf(str, "Task 3. What magnitude is larger, point A or point B?");
	else if(num_task==3)
		sprintf(str, "Task 4. Which point has maximum magnitude when exponent is %d?", 
			myData->GetExponentTask4(num_task, datacount, 
									  number_encoding, number_trials));


  	float x = viewport[2]/3.;	
    	float y = viewport[3]-50;
	glRasterPos2f(x,y);
	for(int i=0;i<strlen(str);i++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[i]);	
  


}

void
DrawBinary()
{

	glColor3f(1.,1.,1.);
	char str[40];
	char *input = strdup("Please select your answer.");
	sprintf(str,"%s",input);
	glRasterPos2f(image_width*4.5/5.-80,viewport[3]*7.2/10.);
	for(int i=0;i<strlen(str);i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[i]);
	}	

	float x,y;
	x = image_width*4.5/5.;
	y = viewport[3]*7.2/10.+image_height/36.;
	for(int i=0;i<2;i++)
	{
		if(mousex>=x&&mousex<=x+image_width/30.
		&&mousey>=y&&mousey<=y+image_height/40.)
		{
			ifselectanswer = true;
			useranswer = i;
		}
		glColor3f(0.,0.,0.);
		if(i==0)
			sprintf(str,"A");
		else if(i==1)
			sprintf(str,"B");
		else if(i==2)
			sprintf(str,"Same");
		if(i!=2)
		glRasterPos2f((x + x+image_width/30.)/2.-image_width/280.,(y+ y+image_height/40.)/2.+image_height/160.);
		else
		glRasterPos2f(x+image_width/280.,(y+ y+image_height/40.)/2.+image_height/160.);
		for(int j=0;j<strlen(str);j++)
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[j]);	

		if(useranswer == i&&ifselectanswer==true)
			glColor3f(1.,1.,1.);
		else if(movex>=x&&movex<=x+image_width/30.
			&&movey>=y&&movey<=y+image_height/40.)
			glColor3f(1.,1.,1.);
		else
			glColor3f(0.5,0.5,0.5);

		    glBegin(GL_QUADS);
			glVertex2f(x+5.,y+5.);
			glVertex2f(x+5.,y+image_height/40.-5.);
			glVertex2f(x+image_width/30.-5.,y+image_height/40.-5.);
			glVertex2f(x+image_width/30.-5.,y+5.);
		    glEnd();	

		    glColor3f(0.2,0.2,0.2);
		    glBegin(GL_QUADS);
			glVertex2f(x+5.,y+5.);
			glVertex2f(x+5.,y+image_height/40.);
			glVertex2f(x+image_width/30.,y+image_height/40.);
			glVertex2f(x+image_width/30.,y+5.);
		    glEnd();	

		    glColor3f(0.8,0.8,0.8);
		glBegin(GL_QUADS);
		glVertex2f(x,y);
		glVertex2f(x,y+image_height/40.);
		glVertex2f(x+image_width/30.,y+image_height/40.);
		glVertex2f(x+image_width/30.,y);
		glEnd();
		y = y + image_height/25.;
	}
	
}

void printbitmap(const string msg, double x, double y)
{
   glRasterPos2d(x, y);
   for (string::const_iterator ii = msg.begin(); ii != msg.end(); ++ii)
   {
      glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *ii);
   }
}

void
Draw_2D()
{
	if(ifnext!=3&&ifnext!=0)
	{
		//myDraw->RenderTasks();//need to be implemented
		//myDraw->RenderLegend();//need to be implemented
		//myDraw->RenderTask();
		//myDraw->RenderLegend();
		//if(ifnext==1)Draw_Done();
		
		if(ifnext==1)
		{
		//if(num_task <3)
		  Draw_Done();
		Draw_EachTask();
		     myDraw->Large_Font_Legend();
		 	 myDraw->Large_Axis_Legend();

		  if(mode_order[num_mode] == 0)
		  {
		    myDraw->Dual_Legend();
		  }
		  else if(mode_order[num_mode] == 3 )
		  {
		    myDraw->TextureSize_Legend();
		  }
		  else if(mode_order[num_mode] == 2 )
		  {
		    myDraw->ColorSize_Legend();
		  }
		  else if(mode_order[num_mode] == 1)
		  {
		     myDraw->SizeSize_Legend();
		  }
		  else if(mode_order[num_mode] == 4)
		  {
		     myDraw->SizeSizeColor_Legend();

		  }
		}
	     
		if((num_task==0)&&ifnext==2)
		{
			{Draw_Page();}Draw_EachTask();
		}
		else if((num_task==1||num_task==2||num_task==3)&&ifnext==2)
		{
			{Draw_Page();}Draw_EachTask();
		}

		if(num_task == 3 && ifnext == 1)
		{
			char str[100];
			sprintf(str,"Please press middle button to select your answer");
			glRasterPos2f(10.,image_height-20.);
			for(int i=0;i<strlen(str);i++)
			{
				glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[i]);
			}
		}
		Draw_Time();
		Draw_OTHER();
		Draw_Progress();
		//myDraw->RenderQuestionN(viewport[2],viewport[3]);

		if(ifnext==1&&!ifpause)
		{
			if(num_task==2)
				DrawBinary();
			//else if(num_task == 3)
			  //    printbitmap(timestring, image_width/40., image_height/40.+50.); // time string
		}
		else if(ifnext==2)
		{	
			if(!ifpause)
			{
				if(num_task==0||num_task==1)
					Draw_Label();
				Draw_Level();
			}
		}
	}
	else if(ifnext==3)
		Draw_End();
	else if(ifnext==0)
	{Draw_Time();	Draw_Task();}

}

/*
void InitLight()
{
  //setting of lighting
  GLfloat mat_diffuse[] = { 0.5, 0.5, 0.5, 0.5 };
  GLfloat mat_specular[] = { 0.2,0.2,0.2, 0.5 };
  GLfloat mat_shininess[] = { 10.0 };
  GLfloat light_position[] = { 0.0,0.0, 100.0, 0.0 };
  GLfloat white_light[] = { 1.0, 1.0, 1.0, 1.0 };
  glClearColor(0.0, 0.0, 0.0, 0.0);

  glShadeModel(GL_SMOOTH);
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat_diffuse);
  glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
  glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

  glLightfv(GL_LIGHT0, GL_POSITION, light_position);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, white_light);
  glLightfv(GL_LIGHT0, GL_SPECULAR, white_light);
}*/

void InitLight()
{
  //setting of lighting
  GLfloat mat_diffuse[] = { 1.0, 0.0, 0.0, 0.2 };
  GLfloat mat_specular[] = { 0.2, 0.2, 0.2, 0.5 };
  GLfloat mat_shininess[] = { 10.0 };
  GLfloat light_position[] = { 100.0, 100.0, 100.0, 0.0 };
  GLfloat white_light[] = { 1.0, 1.0, 1.0, 1.0 };
  //glClearColor(0.0, 0.0, 0.0, 0.0);
  glShadeModel(GL_SMOOTH);
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat_diffuse);
  glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
  glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
  glLightfv(GL_LIGHT0, GL_POSITION, light_position);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, white_light);
  glLightfv(GL_LIGHT0, GL_SPECULAR, white_light);
}

void
Draw_Everything()
{
        glClearColor(0.5,0.5,0.5,1.0);
	//glClearDepth(1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	trackball.getMatrix().getValue(m);

	if(ifnext!=3&&ifnext!=0)
	{
		if(ifnext==1&&!ifpause)
		{


			
			glPushMatrix();
			glMultMatrixf(m);

				if(mode_order[num_mode] == 0)
				  myDraw->Double_Cylinder(m);
				else if(mode_order[num_mode] == 1)
				  myDraw->Size_Size(m);
				else if(mode_order[num_mode] == 2)
				  myDraw->Hue_Size(m);
				else if(mode_order[num_mode] == 3)
				  myDraw->Texture_Size(m);
				else if(mode_order[num_mode] == 4)
				  myDraw->Size_Size_Color(m);
			glPopMatrix();

			glPushMatrix();
			glMultMatrixf(m);
			//Render glyphs
			myDraw->RenderBounding();
			glPopMatrix();
			
			if(num_task < 3)
			  myDraw->RenderMarkers(myData->GetSelectedIndex(num_task, datacount, number_encoding ,
								       number_trials),m, mode_order[num_mode]);
			else if(num_task == 3)
			  myDraw->RenderAnswerMarker(task4_answer_index, m, mode_order[num_mode]);
			//if(num_task==3)
			//	myDraw->RenderTask4Marker(m);
			//if(mode_order[num_mode]+1==Text)
			//	myDraw->RenderText(m);

		}

	}

       // glutSwapBuffers();
}

void 
reshape(int width, int height)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, (GLsizei) width, (GLsizei) height);

	  	glMatrixMode(GL_PROJECTION);
	  	glLoadIdentity();
		gluPerspective(45, 1.0*width/height,0.5, 100);
		glMatrixMode(GL_MODELVIEW);
	  	glLoadIdentity();
		gluLookAt(0,0,5.5,
		      0,0,0,
		      0, 1, 0);	
	
  svVector3 eye;
  svVector3 center;
  center[0]=0.;center[1]=0.;center[2]=0.;
  eye[0]=0.;eye[1]=0.;eye[2]=5.5;
  trackball.setEye(eye);	
	image_width = width;
	image_height = height;
	myDraw->SetCamera(image_width, image_height, eye);
	 viewport[2] = image_width;
	 viewport[3] = image_height;
	InitLight();
	//myDraw->InitLight();
}


void 
display(void)
{
        glClearColor(0.6,0.6,0.6,1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	trackball.getMatrix().getValue(m);

	glEnable(GL_DEPTH_TEST);
	
	
	glPushMatrix();
	//glMultMatrixf(m);
	//myDraw->Render(displaylist);
	//myDraw->RenderBounding();
	Draw_Everything();
	glPopMatrix();

	 
	GLdouble vmat2[16], pmat2[16];
	GLint    viewport2[4];

      glGetIntegerv (GL_VIEWPORT, viewport2);
      glGetDoublev (GL_MODELVIEW_MATRIX,vmat2);
      glGetDoublev (GL_PROJECTION_MATRIX, pmat2);

	    glMatrixMode(GL_PROJECTION);
	    glLoadIdentity();
	    gluOrtho2D(0.0,(GLfloat) viewport2[2], (GLfloat) viewport2[3], 0.0);
	    glMatrixMode(GL_MODELVIEW);
	    glLoadIdentity();
	    glDisable(GL_LIGHTING);
	    
	    Draw_2D();

	glMatrixMode(GL_PROJECTION);
	glLoadMatrixd(pmat2);
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixd(vmat2);		

	glFlush();
       	glutSwapBuffers();
}

void
init(int argc, char **argv)
{
  // myData=new dataSet();
   myDraw = new svQDOTDrawing();
   myData = new dataSet();
   //myDraw->SetData("data/task1_data/D11");


//two cylinder
//size/size, hue/texture, hue/size, size/texture


   //myDraw->Double_Cylinder();
  // myDraw->Size_Size();
  //myDraw->Size_Brightness();
   //myDraw->Color_Size();

  // myDraw->Size_Brightness_Noshading();
   // myDraw->Size_Color_Noshading();
	
	//myDraw->Size_Texture();
	//myDraw->Texture_Hue();

   //myDraw->Shape_Brightness();
   //myDraw->Shape_Color();

   //myDraw->Shape_Brightness_Noshading();
   //myDraw->Shape_Color_Noshading();
   //myDraw->Brightness_Color();
   //myDraw->Color_Color();

   //myDraw->Brightness_Color_Noshading();
  // myDraw->Color_Color_Noshading();


  // myDraw->Spiral_Legend();
   //myDraw->Spiral_Legend_2D();
  // myDraw->Red_Green_Legend();
  
  
  //correctname = "file/correct1";// argv[7];
  
  int nn = atoi(argv[1]);
  correctname = new char[50];
  namelog = new char[50];
  answername = new char[50];
  timename = new char[50];
  confidentname = new char[50];
  
 // answername = "file/result1";//argv[6];
 // timename = "file/time1";//argv[8];
 // confidentname = "file/confident1";//argv[9];
  dataDir = "data";//argv[1];
  
	int block =atoi(argv[2]);
  sprintf(correctname, "file/correct%d_%d",nn,block);//cerr<<correctname<<endl;
  sprintf(namelog, "file/log%d_%d",nn,block);//cerr<<correctname<<endl;
  sprintf(answername,"file/result%d_%d",nn,block);//cerr<<correctname<<endl;
  sprintf(timename,"file/time%d_%d",nn,block);//cerr<<correctname<<endl;
  sprintf(confidentname,"file/confident1%d_%d",nn,block);//cerr<<correctname<<endl;

	if(block==1)
	{
		mode_order[0] = 0;
		mode_order[1] = 1;
		mode_order[2] = 2;
		mode_order[3] = 3;
		mode_order[4] = 4;
	}	
	else if(block==2)
	{
		mode_order[0] = 1;
		mode_order[1] = 2;
		mode_order[2] = 4;
		mode_order[3] = 0;
		mode_order[4] = 3;
	}
	else if(block==3)
	{
		mode_order[0] = 2;
		mode_order[1] = 4;
		mode_order[2] = 3;
		mode_order[3] = 1;
		mode_order[4] = 0;
	}
	else if(block==4)
	{
		mode_order[0] = 3;
		mode_order[1] = 0;
		mode_order[2] = 1;
		mode_order[3] = 4;
		mode_order[4] = 2;
	}
	else if(block==5)
	{
		mode_order[0] = 4;
		mode_order[1] = 3;
		mode_order[2] = 0;
		mode_order[3] = 2;
		mode_order[4] = 1;
	}

  //myDraw = new QDOTglyph();
  //myDraw->SetMode(mode_order[num_mode]+1);
  ReadFile();//cerr<<"done"<<endl;

  svVector3 eye;
  svVector3 center;
  center[0]=0.;center[1]=0.;center[2]=0.;
  eye[0]=0.;eye[1]=0.;eye[2]=5.5;
  trackball.setEye(eye);
  trackball.setFocus(center);
  trackball.SetLogName(namelog);//"file/log1");//argv[3]);
  //namelog = "file/log1";//argv[3];

  

  trackball.setWindowSize(image_width,image_height);
  trackball.getMatrix().getValue(m);  
  confidentlevel = -1; 

  begin_time = GetTime();
  ifnext = 0;

  svVector4 texturec;
  texturec[0]=0;
  texturec[1]=0;
  texturec[2]=0;
 // myDraw->makeTexture(texturec);
  //myDraw->inittexture();

}
//-----------------done here------------------
void
RecordLog()
{
	long double time = GetTime();
	int task;
	int mode;
	int stereo;
	int start_clock = startclock;
	int confidentoutput;
	if(ifnext==0)
	{
		task = -1;
		mode = -1;
		stereo = -1;
	}
	else
	{
		task = num_task;
		mode = mode_order[num_mode];
		stereo = false;
	}
	if(ifnext==1)
	{
		trackball.GetRotateFactor(rotatefactor);
		trackball.GetZoomFactor(zoomfactor);
		trackball.GetPanFactor(panfactor);
	}	
	else
	{
		rotatefactor[0] = 0;
		rotatefactor[1] = 0;
		rotatefactor[2] = 0;
		rotatefactor[3] = 0;
		zoomfactor = 0;
		panfactor[0] = 0;
		panfactor[1] = 0;
	}
	float task4useranswer;
	float binaryanswer = -1;
	if(num_task==2)
	{
		if(ifselectanswer)
			binaryanswer = useranswer;
	}
	
	int n,n1,n2;

	std::ofstream log(namelog, std::ios_base::app | std::ios_base::out);

	log<<std::fixed << std::setprecision(8) <<time<<" ";
	log<<ifpause<<" ";
	log<<std::fixed << std::setprecision(2) <<ifnext<<" "
			<<task<<" "<<mode<<" "<<stereo<<" "<<mousebutton<<" "<<mousestate<<" "
		<<rotatefactor[0]<<" "<<rotatefactor[1]<<" "<<rotatefactor[2]<<" "<<rotatefactor[3]<<" "
		<<zoomfactor<<" "<<panfactor[0]<<" "<<panfactor[1]<<" ";//<<keystate<<" ";

	char stra[100];
	for(int i=0;i<TaskKeyAnswer.size();i++)
	{
		stra[i] = TaskKeyAnswer[i];
	}	

	if(num_task < 2)
	{
	  if(TaskKeyAnswer.size()!=0)
	  {	
		  task4useranswer = atof(stra);
	  }
	  else
		  task4useranswer = 0.;
	}
	if(num_task == 3)
	{
	  if(task4_answer_index != -1)
	    task4useranswer = myData->GetMaxAnswer(task4_answer_index); //need to be fixed
	  else
	    task4useranswer = 0;
	}

	confidentoutput = confidentlevel;

	log<<task4useranswer<<" "<<binaryanswer<<" "<<confidentoutput<<endl;
	log.close();
}

void left_button(bool mouseinput)
{
			char name[20];
			page_count++;
			page_next = true;//cerr<<ifnext<<" "<<page_count<<endl;
			if(ifnext==2)
			{
				if(!(ifselectanswer==true&&ifconfident==true))
				{	page_count--;page_next = false;}

			}	
			else if(ifnext==1)	
			{
				if((!(ifselectanswer==true)&&(num_task == 2 || num_task == 3)))
				//|| (num_task == 3 && mouseinput))
					//||GetTime()-timefornext<3.)
				{	page_count--;page_next = false;}	
			}
			ifnext = page_order[page_count];//cerr<<ifnext<<" "<<page_count<<endl;
			if(ifnext == 1&&((ifconfident==true&&ifselectanswer==true)||page_next))
			{
				//RecordLog();
				begin_time = GetTime();
				timefornext = GetTime();
				if(page_count!=1)// && page_count != 83)
				{
					
					if(num_task == 3)
					{
					  if(task4_answer_index != -1)
					    useranswer = myData->GetMaxAnswer(task4_answer_index);
					  else
					    useranswer = 0;
					}
					answerfile.open(answername,std::ios_base::app);
					answerfile<<useranswer<<endl;
					answerfile.close();
					TaskKeyAnswer.clear();	
					task4_answer_index = -1;

					answerfile.open(confidentname,std::ios_base::app);
					answerfile<<confidentlevel<<endl;
					answerfile.close();
					
					if(page_count == 42 || page_count == 83 || page_count == 124)
					{
					  answerfile.open(correctname,std::ios_base::app);
					  answerfile<<myData->SaveCorrect(num_task-1, datacount, 
									  number_encoding, number_trials)<<endl;
					  answerfile.close();
					}
					else
					{
					  answerfile.open(correctname,std::ios_base::app);
					  answerfile<<myData->SaveCorrect(num_task, datacount, 
									  number_encoding, number_trials)<<endl;
					  answerfile.close();
					}
					RecordLog();
					
				}//cerr<<num_mode<<" "<<num_page<<endl;
				if(num_page==number_trials-1)
				{
					num_page = 0;
					//displaylist=0;
					num_mode ++;
					//myDraw->SetMode(mode_order[num_mode]+1);
				}
				else
				{
				    num_page++;
				    //displaylist ++;
				}
				//myDraw->questionnum++;//cerr<<num_mode<<endl;

				datacount++;
				//if(datacount==number_trials * number_encoding )datacount=0;

				//cerr<<num_mode<<" "<<num_page<<endl;
				int data_order = myData->GetDataIndex(datacount);//%(number_trials*number_encoding));
				data_order = data_order%(number_encoding * number_trials);
				data_order = (data_order/4 + 1) * 10 + data_order%4+1;
				if(num_task==0)
					sprintf(name,"data/task1_data/D%d",data_order);
				else if(num_task==1)
					sprintf(name,"data/task2_data/D%d",data_order);
				else if(num_task==2)
					sprintf(name,"data/task3_data/D%d",data_order);
				else if(num_task ==3)
					sprintf(name,"data/task4_data/D%d",data_order);
				
				cerr<<name<<" "<<mode_order[num_mode]<<endl;
	    
				ifselectanswer=false;
				myData->SetData(name);
				myDraw->SetData(name);
	

				svVector3 center;
  				center[0]=0.;center[1]=0.;center[2]=0.;
  				trackball.setFocus(center);
				trackball.reset();

				confidentlevel = -1;
				ifconfident = false;
				time(&nowtime);

				if(mouseinput)
				{
				  if(num_task == 3)
				  {
				    startclock = true;
				    sec = 0;
				  }
				}

			}
			else if(ifnext==0)
			{
				num_task++;
				num_mode=-1;
			}
			else if(ifnext==3)
			{		
					if(num_task == 3)
					{
					  if(task4_answer_index != -1)
					    useranswer = myData->GetMaxAnswer(task4_answer_index);
					  else
					    useranswer = 0;
					}

					answerfile.open(answername,std::ios_base::app);
					answerfile<<useranswer<<endl;
					answerfile.close();
					TaskKeyAnswer.clear();		

					answerfile.open(confidentname,std::ios_base::app);
					answerfile<<confidentlevel<<endl;
					answerfile.close();
					

					answerfile.open(correctname,std::ios_base::app);
					answerfile<<myData->SaveCorrect(num_task, datacount, 
									number_encoding, number_trials)<<endl;
					answerfile.close();
					
					
					
			}
			else if(ifnext ==2&&page_next==true)
			{
				wholetime = wholetime + GetTime() - begin_time;
				/*answerfile.open(timename,std::ios_base::app);
				answerfile<<wholetime<<endl;
				answerfile.close();*/
				wholetime = 0;
				startclock = false;
				sec = 0;
				
			}
}

void 
mouse(int button, int state, int x, int y)
{
    long s=0x00000000;

    float nextx = viewport[2]*4.5/5.-50.;	
    float nexty = viewport[3]*9./10.;
    char name[20];
    char str[20];
    mousebutton = button;
    mousestate = state;

	s |= (button == GLUT_LEFT_BUTTON)   ? ((state == GLUT_DOWN) ? Trackball::LBUTTON_DOWN : Trackball::LBUTTON_UP) : 0;
	s |= (button == GLUT_RIGHT_BUTTON)  ? ((state == GLUT_DOWN) ? Trackball::RBUTTON_DOWN : Trackball::RBUTTON_UP) : 0;

	int key_state = glutGetModifiers();
	s |= (key_state & GLUT_ACTIVE_CTRL)  ? Trackball::CTRL_DOWN  : 0;
	s |= (key_state & GLUT_ACTIVE_ALT)   ? Trackball::ALT_DOWN   : 0;
	s |= (key_state & GLUT_ACTIVE_SHIFT) ? Trackball::SHIFT_DOWN : 0;

	if((button == GLUT_LEFT_BUTTON) && (state==GLUT_DOWN))
	{
		mousex = (float)x;mousey = (float)y;

		if(x>=50&&x<=50+150.&&y>=viewport[3]/10.&&y<=viewport[3]/10.+50)//pause
		{
			if(ifpause==false)
			{
				ifpause=true;
				if(ifnext==1)
					wholetime = GetTime()-begin_time + wholetime;
				//cerr<<GetTime()-wholetime<<endl;
				
			}
			else
			{
				ifpause=false;
				if(ifnext==1)
					begin_time = GetTime();
			}
		}
		else if(x>=nextx&&x<=nextx+150.&&y<=nexty&&y>=nexty-50.)//next
		{
			left_button(true);
		}

		else
		{
				if(ifnext==1)
				{
				  if (s & Trackball::BUTTON_DOWN){
				    trackball.mouseDown(s, x, y);
				  }

				  if (s & Trackball::BUTTON_UP){
				    trackball.mouseUp(s, x, y);
			  		}
				}
		}

	}
	else
	{
		if((button == GLUT_MIDDLE_BUTTON) && (state==GLUT_DOWN) && ifnext == 1)
		{
		  task4_answer_index = myDraw -> Find_Nearest(x, y, image_height, m,mode_order[num_mode]);
		  ifselectanswer = true;
		}
		
		if((button == GLUT_RIGHT_BUTTON) && (state==GLUT_DOWN))
		{
			if(ifnext==1)
			{  if (s & Trackball::BUTTON_DOWN){
			    trackball.mouseDown(s, x, y);
			  }

			  if (s & Trackball::BUTTON_UP){
			    trackball.mouseUp(s, x, y);
		  	}
			}
		}
	}
	RecordLog();
	glutPostRedisplay();
}

void motion(int x, int y)
{
	if(mousebutton!=GLUT_MIDDLE_BUTTON)
		trackball.mouseMotion(x, y);
	RecordLog();
	glutPostRedisplay();
}

void
movement(int x, int y)
{
   	movex = (float)x;	
	movey = (float)y;	
	glutPostRedisplay();
}


void key(unsigned char key, int x, int y)
{
	keystate = key;
  switch (key) {
    case '0':
		if(ifnext==2&&(num_task<2))
		TaskKeyAnswer.push_back('0');
		break;
    case '1':
		if(ifnext==2&&(num_task<2))TaskKeyAnswer.push_back('1');
		break;	
    case '2':
		if(ifnext==2&&(num_task<2))TaskKeyAnswer.push_back('2');
		break;
    case '3':
		if(ifnext==2&&(num_task<2))TaskKeyAnswer.push_back('3');
		break;
    case '4':
		if(ifnext==2&&(num_task<2))TaskKeyAnswer.push_back('4');
		break;
    case '5':
		if(ifnext==2&&(num_task<2))TaskKeyAnswer.push_back('5');
		break;
    case '6':
		if(ifnext==2&&(num_task<2))TaskKeyAnswer.push_back('6');
		break;
    case '7':
		if(ifnext==2&&(num_task<2))TaskKeyAnswer.push_back('7');
		break;
    case '8':
		if(ifnext==2&&(num_task<2))TaskKeyAnswer.push_back('8');
		break;
    case '9':
		if(ifnext==2&&(num_task<2))TaskKeyAnswer.push_back('9');
		break;
    case 'e':
		if(ifnext==2&&(num_task<2))TaskKeyAnswer.push_back('e');
		break;
    case 'E':
		if(ifnext==2&&(num_task<2))TaskKeyAnswer.push_back('e');
		break;
    case '+':
		if(ifnext==2&&(num_task<2))TaskKeyAnswer.push_back('+');
		break;
    case '-':
		if(ifnext==2&&(num_task<2))TaskKeyAnswer.push_back('-');
		break;
    case '.':
		if(ifnext==2&&(num_task<2))TaskKeyAnswer.push_back('.');
		break;
    case 'h':
		trackball.reset();
		break;
	case 'H':
		trackball.reset();
		break;
    case '\010':
		if(ifnext==2&&(num_task<2))
			if(TaskKeyAnswer.size()!=0)
			TaskKeyAnswer.pop_back();	
		break;
    case '\033':
      exit(0);
      break;
  }
  RecordLog();
  glutPostRedisplay();
}
//https://www.cs.uaf.edu/~cs381/examples/timer.cpp
void myTimer(int v)     // timer function to update time
{
	if(num_task == 3)
	{
  ostringstream timestream;      // used to format time string
    if (startclock && !ifpause) sec++;         // increment second
    timestream << setfill('0') << "Left time 00:00:" << setw(2) << 30 - sec;
    timestring=timestream.str();  //convert stream to string
    if (sec==30) 
    {
      //ifnext = 2;
      ifselectanswer = true;
      sec = 0;
      startclock = false;
	RecordLog();
      left_button(false);
    } // increment minute
    }
    glutPostRedisplay();
    glutTimerFunc(1000/v, myTimer, v);  //repost timer 
}

int main(int argc, char **argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);//|GLUT_STEREO);//
   glutInitWindowSize (2000, 1000);
   glutInitWindowPosition (0, 0);
   glutCreateWindow (argv[0]);

   init(argc, argv);

   glutReshapeFunc(reshape);
   glutDisplayFunc(display);
   glutMouseFunc(mouse);
   glutMotionFunc(motion);
   glutPassiveMotionFunc(movement);
   glutKeyboardFunc(key);
   glutTimerFunc(1000,myTimer,FPS);
   
   glutMainLoop();
   return 0;
}
