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
#include <unistd.h>
#include "ReadData.h"
#include "svQDOTDrawing.h"

#include "./jcUtil/MArray.h"
#include "./jcUtil/MGL.h"
#include "./jcUtil/MVectorMatrix.h"
#include "ivTrackball.h"

#define FPS 10

#include <sstream>
using std::ostringstream;


#define TOTALTRIAL 158

svQDOTDrawing *myGlyphs;
dataSet *myData;
Trackball trackball;
bool showRest = false; 
bool showTime = true;
bool showGlyphs = true;
bool showAnswers = false;
bool giveAnswer = false;

string correctFile;
string logFile;
string answerFile;
string timeFile;
string confidenceFile;

float timeLimit[4]={20, 20, 1, 1};

typedef struct dataInfo{
  int pID; //participant;
  int task1trial;
  int task2trial;
  int task3trial;
  int task4trial;
  int powerID[3];
  int *encodeID;//[366]; // encodeID: 1color, 2texture, 3splitvectors
  int *dataID;//[366]; // data distribution: 1regular, 2random
  int *task1Power;//[108];
  int *task2Power;//[108];
  int *task3Power;//[150];
  int *task4Power;//[120];
  int *task1Index;//[108];
  int *task2Index;//[108];
  int *task3Index;//[150];
  int *task4Index;//[120];
  int *task1X;//[108];
  int *task2X;//[108];
  int *task1Answer;//[108];
  int *task2Answer;//[108];
  int *task3Answer;//[108];
  int *task4Answer;//[108];
  int *task1User;
  int *task2User;
  int *task3User;
  int *task4User;
  double *userAnswer; 
  int *visConfi;
  int *dataConfi[2];
  bool *isBack;

  dataInfo(){
        visConfi = new int[366];
        dataConfi[0] = new int[366];
        dataConfi[1] = new int[366];
        isBack = new bool[366];
        for(int i=0;i<366;i++) isBack[i] = false;
        userAnswer = new double[366];
        for(int i=0;i<366;i++){
            userAnswer[i] = -1;
            visConfi[i] = -1;
            dataConfi[0][i]=-1;
            dataConfi[1][i]=-1;
        }
        encodeID = new int[366]; 
        dataID = new int[366]; // data distribution: 1regular, 2random
        task1Power = new int[108];
        task2Power = new int[108];
        task3Power = new int[150];
        task4Power = new int[120];
        task1Index = new int[108];
        task2Index = new int[108];
        task3Index = new int[150];
        task4Index = new int[120];
        task1X = new int[108];
        task2X = new int[108];
        task1Answer = new int[108];
        task2Answer = new int[108];
        task3Answer = new int[150];
	task4Answer = new int[108];
	task1User = new int[108];
	task2User = new int[150];
	task3User = new int[120];
        task4User = new int[120];
  }

  ~dataInfo(){
       delete [] userAnswer;
       delete [] encodeID; 
       delete [] dataID;
       delete [] task1Power;
       delete [] task2Power;
       delete [] task3Power;
       delete [] task4Power;
       delete [] task1Index;
       delete [] task2Index;
       delete [] task3Index;
       delete [] task4Index;
       delete [] task1X;
       delete [] task2X;
       delete [] task1Answer;    
       delete [] task2Answer;    
       delete [] task3Answer;    
       delete [] task4Answer;    
  }

} dataInfo;

typedef struct myTime{
  double beginTime;
  double endTime;
  double wholeTime;
  time_t nowTime;
  bool startClock;
  string timeString;
  int hour;
  int min;
  int sec;

  double GetTime(){
     timeval tv;
     gettimeofday (&tv, NULL);
     return double (tv.tv_sec) + 0.000001 * tv.tv_usec;

  };
} myTime;
myTime *timeObj;


typedef struct myState{
//  int participantID;
//  int blockID;
  int taskID;
//  int encodeID;
  double remainingTime;
//  double userAnswer;
//  double confidentLevel;
//  double groundTruth;
  bool pause;
  bool showAnswer;
  bool isConfident;
  bool isSelect;
}myState;
myState state;

class myOrder{   
public:
    myOrder(){
        trialPointer = -1;
        dataPointer = -1;
        pagePointer=0;
        dataMax[0] = 108;
        dataMax[1] = 108;
        dataMax[2] = 150;
        dataMax[3] = 120;
        datainfo = new dataInfo();
    }
    ~myOrder(){
        delete [] pageOrder;
        delete datainfo;
    }

   void readPageOrder(string file);  
   void readOrder(string file);

   dataInfo *datainfo; 
   int dataPointer;
   int pagePointer; 
   int trialPointer;
   int *pageOrder; 
   int pageNum;
   int dataMax[4];
   //int encodeOrder[3];
   //string *dataOrder;
   //int *powerOrder; 
   //int *answerOrder;
};
myOrder *order;

void myOrder::readPageOrder(string file){
  ifstream infile(file.c_str());
  int n;
  infile>>n; //cout<<n<<endl;
  int m;
  infile>>m;
  pageNum = 263; 
// + m*2/3 * 2 - 60/5 + n * 7 + 2;//m*2 + n + n * 3;
  //cout<<pageNum<<endl;
  pageOrder = new int[pageNum];
  //cout<<pageNum<<endl;
  for(int i=0;i<pageNum;i++){
     infile>>pageOrder[i]; //cout<<pageOrder[i]<<" ";
  }
  infile.close();
}

/*
void myOrder::readOrder(string file){
  ifstream infile(file.c_str());
  dataOrder = new string[TOTALTRIAL];
  powerOrder = new int[TOTALTRIAL];
  answerOrder = new int[TOTALTRIAL];
  infile>>encodeOrder[0];
  infile>>encodeOrder[1];
  infile>>encodeOrder[2];
  for(int i=0;i<TOTALTRIAL;i++){
    int power, index;
    infile>>power>>index;
    char numstr[10];
    sprintf(numstr,"%d%d",power,index);
    dataOrder[i] = "data/D"+string(numstr); 
    infile>>powerOrder[i]>>answerOrder[i];
  }
  infile.close();
}
*/

void myOrder::readOrder(string file){
  ifstream infile(file.c_str());
  infile>>datainfo->pID;
  infile>>datainfo->task1trial; //# of task1
  int count = 0;
  for(int i=0;i<datainfo->task1trial;i++){
     infile>>datainfo->encodeID[count];
     infile>>datainfo->dataID[count];
     infile>>datainfo->task1Power[i];
     infile>>datainfo->task1Index[i];
     infile>>datainfo->task1X[i];
     infile>>datainfo->task1Answer[i];
     //double tt; infile>>tt;
     count++;
  } 
  infile>>datainfo->task2trial; //# of task2
  for(int i=0;i<datainfo->task2trial;i++){
     infile>>datainfo->encodeID[count];
     infile>>datainfo->dataID[count];
     infile>>datainfo->task2Power[i];
     infile>>datainfo->task2Index[i];
     infile>>datainfo->task2X[i];
     infile>>datainfo->task2Answer[i];
     count++;
  }
  int tmp; 
  infile>>datainfo->task3trial; //# of task3
  for(int i=0;i<datainfo->task3trial;i++){
     infile>>datainfo->encodeID[count];
     infile>>datainfo->dataID[count];
     infile>>datainfo->task3Power[i];
     infile>>datainfo->task3Index[i];
     infile>>tmp;
     infile>>datainfo->task3Answer[i];
     count++;
  } 
  infile>>datainfo->task4trial; //# of task3
  for(int i=0;i<datainfo->task4trial;i++){
     infile>>datainfo->encodeID[count];
     infile>>datainfo->dataID[count];
     infile>>datainfo->task4Power[i];
     infile>>datainfo->task4Index[i];
     infile>>tmp;
     infile>>datainfo->task4Answer[i];
//cout<<datainfo->encodeID[count]<<endl;
     count++;
  }
  infile.close(); 
}

int mousebutton;
int mousestate;

typedef struct viewProperty{
  svVector3 eye;
  svVector3 center;
  int image_width;
  int image_height;
  GLfloat m[16];
  GLfloat im[16];
  GLdouble vmat[16];
  GLdouble pmat[16];
  GLint    viewport[4];
  GLdouble vmat_left[16];
  GLdouble pmat_left[16];
  GLint    viewport_left[4];
  GLdouble vmat_right[16];
  GLdouble pmat_right[16];
  GLint    viewport_right[4];
}viewProperty;
viewProperty viewproperty;

void recordAnswer(){
  if(order->trialPointer<0) return;

  ofstream answerfile(answerFile.c_str(),std::ios_base::app | std::ios_base::out);
  answerfile<<order->trialPointer<<" ";
  if(state.taskID == 0)
  {
   answerfile<<myGlyphs->getMag(order->datainfo->task1Answer[order->dataPointer])<<" ";
   answerfile<<myGlyphs->getMag(order->datainfo->userAnswer[order->trialPointer])<<endl;
  }
  else if(state.taskID == 1)
  {
   answerfile<<myGlyphs->getMag(order->datainfo->task2Answer[order->dataPointer])<<" ";
     answerfile<<myGlyphs->getMag(order->datainfo->userAnswer[order->trialPointer])<<endl;
  }
  else
  {
     answerfile<<order->datainfo->task3Answer[order->dataPointer]<<" ";
     answerfile<<order->datainfo->userAnswer[order->trialPointer]<<endl;
  } 
  answerfile.close();
}

void recordConfidence(){
  if(order->trialPointer<0) return;

  ofstream confifile(confidenceFile.c_str(),std::ios_base::app | std::ios_base::out);
  confifile<<order->trialPointer<<" ";
  confifile<<order->datainfo->visConfi[order->trialPointer]+1<<" ";
  confifile<<order->datainfo->dataConfi[0][order->trialPointer]+1<<" ";
  confifile<<order->datainfo->dataConfi[1][order->trialPointer]+1<<endl;
  confifile.close();
}

void recordTime(){
  ofstream timefile(timeFile.c_str(),std::ios_base::app | std::ios_base::out);
  timeObj->wholeTime = timeObj->endTime - timeObj->beginTime;  

  timefile<<std::fixed << std::setprecision(8) <<order->trialPointer<<" "
          <<timeObj->wholeTime<<endl;

  timefile.close();
}

void recordLog(){
  int dataPointer = order->dataPointer;
  int trialPointer = order->trialPointer;
  int pagePointer = order->pagePointer;

  if(trialPointer<0) return;

  ofstream log(logFile.c_str(),std::ios_base::app | std::ios_base::out);

  dataInfo *tmpdata = order->datainfo;

  log<<std::fixed << std::setprecision(8) <<timeObj->GetTime()<<" ";
  log<<state.taskID<<" ";
  log<<order->pageOrder[pagePointer]<<" ";
  log<<tmpdata->dataID[trialPointer]<<" ";
  log<<tmpdata->encodeID[trialPointer]<<" ";

//  float rotatefactor[4];
//  float zoomfactor;
//  float panfactor[4];
//  trackball.GetRotateFactor(rotatefactor);
//  trackball.GetZoomFactor(zoomfactor);
//  trackball.GetPanFactor(panfactor);
  log<<std::fixed << std::setprecision(2) <<" "
     <<mousebutton<<" "<<mousestate<<" "
     <<viewproperty.m[0]<<" "<<viewproperty.m[1]<<" "<<viewproperty.m[2]<<" "
     <<viewproperty.m[3]<<" "<<viewproperty.m[4]<<" "<<viewproperty.m[5]<<" "
     <<viewproperty.m[6]<<" "<<viewproperty.m[7]<<" "<<viewproperty.m[8]<<" "
     <<viewproperty.m[9]<<" "<<viewproperty.m[10]<<" "<<viewproperty.m[11]<<" "
     <<viewproperty.m[12]<<" "<<viewproperty.m[13]<<" "<<viewproperty.m[14]<<" "
     <<viewproperty.m[15]<<" ";
//                  <<mousebutton<<" "<<mousestate<<" "
//                  <<rotatefactor[0]<<" "<<rotatefactor[1]<<" "
//                  <<rotatefactor[2]<<" "<<rotatefactor[3]<<" "
//                  <<zoomfactor<<" "<<panfactor[0]<<" "<<panfactor[1]<<" ";

  if(state.taskID == 0){
     log<<tmpdata->task1Power[dataPointer]<<" ";
     log<<tmpdata->task1Index[dataPointer]<<" ";
     log<<tmpdata->task1X[dataPointer]<<" ";
     log<<tmpdata->task1Answer[dataPointer]<<" ";
  }
  else if(state.taskID == 1){
     log<<tmpdata->task2Power[dataPointer]<<" ";
     log<<tmpdata->task2Index[dataPointer]<<" ";
     log<<tmpdata->task2X[dataPointer]<<" ";
     log<<tmpdata->task2Answer[dataPointer]<<" ";
  }
  else if(state.taskID == 2){
     log<<tmpdata->task3Power[dataPointer]<<" ";
     log<<tmpdata->task3Index[dataPointer]<<" ";
     log<<-1<<" ";
     log<<tmpdata->task3Answer[dataPointer]<<" ";
  }
  else if(state.taskID == 3){
     log<<tmpdata->task4Power[dataPointer]<<" ";
     log<<tmpdata->task4Index[dataPointer]<<" ";
     log<<-1<<" ";
     log<<tmpdata->task4Answer[dataPointer]<<" ";
  }
  log<<tmpdata->userAnswer[trialPointer]<<" ";
  log<<tmpdata->visConfi[trialPointer]<<" ";
  log<<tmpdata->dataConfi[0][trialPointer]<<" ";
  log<<tmpdata->dataConfi[1][trialPointer]<<" ";
  log<<endl;

  log.close();
}


class myDraw{
public:
  myDraw(){};

  void setParameters(){
     largeButton[0] = 100;//150.;
     largeButton[1] = 50;//50.;
     largeBorder = 5.;
     isNextSelected = false;
     isNextClicked = false;
     nextPos[0] = viewproperty.viewport[2]*4.7/5.-10;
     nextPos[1] = viewproperty.viewport[3]*14./15.;

     isBackSelected = false;
     isBackClicked = false;
     backPos[0] = 50.;
     backPos[1] = viewproperty.viewport[3]*14./15.;

     isAnswerSelected = false;
     isAnswerClicked = false;
     answerPos[0] = 50.;
     answerPos[1] = viewproperty.viewport[3]*14./15.-100.;

     isExitSelected = false;
     isExitClicked = false;
     exitPos[0] = viewproperty.viewport[2]/2.;
     exitPos[1] = viewproperty.viewport[3]/2.+50;//+200;

     isConfiVisSelected = -1;
     isConfiVisClicked = -1;
     confidenceButton[0] = 50;
     confidenceButton[1] = 40;
     confidenceBorder = 2.5;
     confidenceCount = 7;
     float x = viewproperty.viewport[2]/3. + viewproperty.image_width/40.;
     float y = viewproperty.viewport[3]/3.
             + viewproperty.viewport[3]/20.;
     //        + viewproperty.image_height/20.;
     for(int i=0;i<7;i++){
        confiVisPos[i][0] = x;
        x += confidenceButton[0] * 1.5;
        confiVisPos[i][1] = y; 
     } 

   for(int j=0;j<2;j++){
     isConfiDataSelected[j] = -1;
     isConfiDataClicked[j] = -1;
     x = viewproperty.viewport[2]/3. + viewproperty.image_width/40.;
     y = viewproperty.viewport[3]/3.
             + viewproperty.viewport[3]/20.
             + j*(viewproperty.image_height/5.);
     for(int i=0;i<7;i++){
        confiDataPos[j][i][0] = x;
        x += confidenceButton[0] * 1.5;
        confiDataPos[j][i][1] = y; 
     } 
   }

     optionButton[0] = 75;
     optionButton[1] = 30;
     optionBorder = 3.;
     isOptionSelected = -1;
     isOptionClicked = -1;
     optionButtonCount = 5;
     x = nextPos[0]+10;//-10;// - 20;
     y = nextPos[1] - optionButton[1] * (float)optionButtonCount*1.6;
     for(int i=0;i<5;i++){
        optionPos[i][0]=x;
        optionPos[i][1]=y;
        y = y + optionButton[1] * 1.5;
     }
  };

  void drawBitmap(const string msg, double x, double y){
     glLineWidth(2.);
     glColor3f(1,1,1);//252./255.,141./255.,89./255.);//1,1,1);
     //glRasterPos2d(x, y);
     glPushMatrix();
     glTranslatef(x,y,0);
     glRotatef(180,0,1,0);
     glRotatef(180,0,0,1);
     glScalef(0.5,0.5,0.5);
     for (string::const_iterator ii = msg.begin(); ii != msg.end(); ++ii){
         //glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *ii);
         glutStrokeCharacter(GLUT_STROKE_ROMAN,*ii);
     }
     glPopMatrix();
     glLineWidth(1.);
  }
  void drawButton(char *name,
                        double textPosX,
                        double textPosY,
                        double buttonPosX,
                        double buttonPosY,
                        double buttonW,
                        double buttonH,
                        double border,
                        bool select);
  void drawProgress();
  void drawPage();
  void drawDone();
  void drawAnswer();
  void drawFeedback();
  void drawPrevious();
  void drawEncode();
  void drawRest();
  //void drawLabel();
  void drawEnd();
  void drawTime();
  void drawDataLevel();
  void drawVisLevel();
  void drawTask();
  void drawInstruction();
  void drawEachTask();
  void drawOptions();
  void draw2D();  

  bool isNextSelected;
  bool isNextClicked;
  float largeButton[2];
  float largeBorder;
  float nextPos[2];
  bool isExitSelected;
  bool isExitClicked;
  bool isBackSelected;
  bool isBackClicked;
  float backPos[2];
  bool isAnswerSelected;
  bool isAnswerClicked;
  float answerPos[2];
  float exitPos[2];
  int optionButtonCount;
  float optionPos[5][2];
  float optionBorder;
  float optionButton[2];
  int isOptionSelected;
  int isOptionClicked;
  int isConfiVisSelected;
  int isConfiVisClicked;
  int isConfiDataSelected[2];
  int isConfiDataClicked[2];
  float confiVisPos[7][2];
  float confiDataPos[2][7][2];
  int confidenceCount;
  float confidenceButton[2];
  float confidenceBorder;
}; 
myDraw *draw;

void myDraw::drawButton(char *name,
                        double textPosX,
                        double textPosY,
                        double buttonPosX,
                        double buttonPosY,
                        double buttonW,
                        double buttonH,
                        double border,
                        bool select)
{
           if(select)
               glColor3f(0.5,0.5,0.5);
           else
               glColor3f(1,1,1);
           glRasterPos2f(textPosX, textPosY);
           for(int j=0;j<strlen(name);j++)
               glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, name[j]);
         double x = buttonPosX;
         double y = buttonPosY;
           if(!select)
               glColor3f(0.5,0.5,0.5);
           else
               glColor3f(1,1,1);
          glBegin(GL_QUADS);
          glVertex2f(x+border,y+border);
          glVertex2f(x+border,y+buttonH-border);
          glVertex2f(x+buttonW-border, y+buttonH-border);
          glVertex2f(x+buttonW-border, y+border);
          glEnd();

          glColor3f(0.2,0.2,0.2);
          glBegin(GL_QUADS);
          glVertex2f(x+border,y+border);
          glVertex2f(x+border,y+buttonH);
          glVertex2f(x+buttonW, y+buttonH);
          glVertex2f(x+buttonW, y+border);
          glEnd();

          glColor3f(0.8,0.8,0.8);
          glBegin(GL_QUADS);
          glVertex2f(x,y);
          glVertex2f(x,y+buttonH);
          glVertex2f(x+buttonW, y+buttonH);
          glVertex2f(x+buttonW, y);
          glEnd();
}

void myDraw::drawProgress(){
        glColor3f(1.,1.,1.);
        char str[20];
        if(state.taskID == 0)
             sprintf(str,"Task 1: %d/30",order->dataPointer+1);
        else if(state.taskID == 1)
             sprintf(str,"Task 2: %d/30",order->dataPointer+1);
        else if(state.taskID == 2)
             sprintf(str,"Task 3: %d/30",order->dataPointer+1);
        else if(state.taskID == 3)
             sprintf(str,"%d/120",order->dataPointer+1);
        glRasterPos2f(viewproperty.image_width/40., viewproperty.image_height/40.);
        for(int i=0;i<strlen(str);i++)
                glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[i]);
}

void myDraw::drawEncode(){
     glColor3f(1.,1.,1.);
     char str[50];
     int encodeID;
     int dataID;
     if(order->trialPointer<0){  
       encodeID = order->datainfo->encodeID[0];
       dataID = order->datainfo->dataID[0];
     }
     else{
       encodeID = order->datainfo->encodeID[order->trialPointer+1];
       dataID = order->datainfo->dataID[order->trialPointer+1];
     }

     //cout<<order->trialPointer<<" "<<encodeID<<endl;
     if(encodeID==1)
         sprintf(str, "color-length");
     else if(encodeID==2)
         sprintf(str, "texture-length");
     else if(encodeID==3)
         sprintf(str, "length-length");

    float x = viewproperty.viewport[2]/3+200;//.+125;
    float y = viewproperty.viewport[3]/2;//*5/12.;
/*
    glRasterPos2f(x,y);
    for(int i=0;i<strlen(str);i++)
         glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[i]);
 */
     char str2[100];
     if(dataID==1)
         sprintf(str2, "Structured; %s", str);
     else if(dataID==2)
         sprintf(str2, "Random; %s", str);

    //x = x - 100;//viewproperty.viewport[2]/4.+25.;
    glRasterPos2f(x,y);
    for(int i=0;i<strlen(str2);i++)
         glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str2[i]);
}
void myDraw::drawTask(){
     glColor3f(1.,1.,1.);
     char str[200];

     if(state.taskID==0)
         sprintf(str, "Task 1. Quickly within 20 seconds, find the vector magnitude of X (X will be provided in each task).");
     else if(state.taskID==1)
        sprintf(str, "Task 2. Quickly within 20 seconds, which vector has the maximum magnitude when power is X (X will be provided in each task)?");   
     else if(state.taskID==2)
         sprintf(str, "Task 3. Quickly within 1 second, estimate the total number of different powers in the vector field.");
     else if(state.taskID==3)
         sprintf(str, "Task 3. Group 2: How many different powers in the data?");

    float x = viewproperty.viewport[2]/4.+25;
    float y = viewproperty.viewport[3]/2.;
    glRasterPos2f(x,y);
    for(int i=0;i<strlen(str);i++)
         glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[i]);
  //  x = viewproperty.viewport[2]*4.5/5.-50.;
  //  y = viewproperty.viewport[3]*9./10.;

}
void myDraw::drawInstruction(){
     glColor3f(1,1.,1.);
     char str[200];

     sprintf(str, "Click your answer vector using middle mouse button.");

    float x = viewproperty.viewport[2]/3.;
    float y = viewproperty.viewport[3]-25.;
    glRasterPos2f(x,y);
    for(int i=0;i<strlen(str);i++)
         glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[i]);
}
void myDraw::drawRest(){
     glColor3f(1.,1.,1.);
     char str[200];

     sprintf(str, "Please take a 1-minute break. You can resume anytime for the next task by clicking \"Next\".");
     // TODO: Timer
     // "Please click Next button to begin."

   drawBitmap(timeObj->timeString, 
              nextPos[0] - largeButton[0], 
              nextPos[1] + largeButton[1]);
//    drawBitmap(timeObj->timeString, 
//               viewproperty.viewport[2]/3,
//               viewproperty.viewport[3]/3.+50);

    float x = viewproperty.viewport[2]/3.;
    float y = viewproperty.viewport[3]/2.;
    glRasterPos2f(x,y);
    for(int i=0;i<strlen(str);i++)
         glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[i]);
    x = viewproperty.viewport[2]*4.5/5.-50.;
    y = viewproperty.viewport[3]*9./10.;
}

void myDraw::drawPrevious(){
  char name[20];
  sprintf(name, "Back");
  double buttonPosX = backPos[0];
  double buttonPosY = backPos[1];
  double buttonW = largeButton[0];
  double buttonH = largeButton[1];
  double border = largeBorder;
  double textPosX = buttonPosX + 20.;
  double textPosY = buttonPosY + 30.;
  bool select = isBackSelected || isBackClicked;

  drawButton(name,
             textPosX,
             textPosY,
             buttonPosX,
             buttonPosY,
             buttonW,
             buttonH,
             border,
             select);
}
void scientificNotation(int index,int x, int y){
    char str[20];
    sprintf(str,"%0.2fx10", myGlyphs->getCoe(index));
    glRasterPos2f(x, y);
    for(int j=0;j<strlen(str);j++)
      glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, str[j]);
    sprintf(str,"%d", (int)myGlyphs->getExp(index));
    glRasterPos2f(x+80,
                  y-10);
    for(int j=0;j<strlen(str);j++)
      glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[j]);
}
void myDraw::drawFeedback(){
   float x= 50; 
   float y =75;

   char str[40]; char str2[200]; char str3[40]; char str4[40]; 
   glColor3f(255./255.,237./255.,111./255.);//252./255.,205./255.,229./255.);//0,0,1);//253./255., 180./255., 98./255.);

   if(state.taskID<2){
      int index1 = order->datainfo->userAnswer[order->trialPointer];
      int index2;
      if(state.taskID==0) 
           index2 = order->datainfo->task1Answer[order->dataPointer];
      else 
           index2 = order->datainfo->task2Answer[order->dataPointer];
      svScalar exp1;
      svScalar exp2;
      svScalar coe1;
      svScalar coe2;
      if(index1>=0){
        exp1 = myGlyphs->getExp(index1);
        exp2 = myGlyphs->getExp(index2);
        coe1 = myGlyphs->getCoe(index1);
        coe2 = myGlyphs->getCoe(index2);
      }
      if(index1==index2){
        sprintf(str,"Correct.");
        sprintf(str2,"Your answer: "); 
        scientificNotation(index1, x+140, y+50);
      }
      else if(index1<0){
        sprintf(str,"Please try harder.");
        sprintf(str2,"No answer is given within 20 seconds."); 
      }
      else{
        if(fabs(exp1-exp2)>0.01){
            sprintf(str,"Please try harder.");
        }
        else{
          if(fabs(coe1-coe2)<2){
            sprintf(str,"Very close.");
          }
          else if(fabs(coe1-coe2)<5){
            sprintf(str,"Close.");
          }
          else {
            sprintf(str,"Please try a little bit harder.");
          }
        }
        sprintf(str2,"Your answer: "); 
        scientificNotation(index1, x+140, y+50);
      }
      sprintf(str3, "Correct answer: ");
      scientificNotation(index2, x+160, y+100); 
      sprintf(str4,"The answer is marked by two blue triangles.");
   }
   else if(state.taskID==2){
  //     cout<<order->trialPointer<<" "<<order->datainfo->userAnswer[order->trialPointer]<<endl;
      if(3+order->datainfo->userAnswer[order->trialPointer]==order->datainfo->task3Answer[order->dataPointer]){
        sprintf(str,"Correct.");
      }
      else {
        sprintf(str,"Incorret.");
      }
      sprintf(str2,"Your answer: %d.",3+(int)order->datainfo->userAnswer[order->trialPointer]);
      sprintf(str3,"Correct answer: %d.",order->datainfo->task3Answer[order->dataPointer]);
    }

    glRasterPos2f(x, y);
    for(int j=0;j<strlen(str);j++)
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, str[j]);
    glRasterPos2f(x, y+50);
    for(int j=0;j<strlen(str2);j++)
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, str2[j]);
    glRasterPos2f(x, y+100);
    for(int j=0;j<strlen(str3);j++)
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, str3[j]);
    if(state.taskID<2){
      glColor3f(0,0,1);
      glRasterPos2f(x, y+150);
      for(int j=0;j<strlen(str4);j++)
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, str4[j]);
    }
}
void myDraw::drawAnswer(){
  char name[20];
  sprintf(name, "Answer");
  double buttonPosX = answerPos[0];
  double buttonPosY = answerPos[1];
  double buttonW = largeButton[0];
  double buttonH = largeButton[1];
  double border = largeBorder;
  double textPosX = buttonPosX + 20.;
  double textPosY = buttonPosY + 30.;
  bool select = isAnswerSelected || isAnswerClicked;

  drawButton(name,
             textPosX,
             textPosY,
             buttonPosX,
             buttonPosY,
             buttonW,
             buttonH,
             border,
             select);

  if(order->pageOrder[order->pagePointer] == 1
    && showAnswers){
   char str[40]; 
   if(state.taskID==0){
      glColor3f(0,0,1);
      sprintf(str,"The answer is marked by two blue triangles");
    }
   else if(state.taskID==1){
      glColor3f(0,0,1);
      sprintf(str,"The answer is marked by two blue triangles");
    }
   else if(state.taskID==2){
      glColor3f(1,1,1);
      sprintf(str,"The number of powers is %d",order->datainfo->task3Answer[order->dataPointer]);
    }

    glRasterPos2f(draw->answerPos[0]+draw->largeButton[0]*1.2, 
                  draw->answerPos[1]+draw->largeButton[1]);
    for(int j=0;j<strlen(str);j++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[j]);
  }
}

void myDraw::drawPage(){
  char name[20];
  sprintf(name, "Next");
  double buttonPosX = nextPos[0];
  double buttonPosY = nextPos[1];
  double buttonW = largeButton[0];
  double buttonH = largeButton[1];
  double border = largeBorder;
  double textPosX = buttonPosX + 30.;
  double textPosY = buttonPosY + 30.;
  bool select = isNextSelected || isNextClicked;  

  drawButton(name,
             textPosX,
             textPosY,
             buttonPosX,
             buttonPosY,
             buttonW,
             buttonH,
             border,
             select);
}

void myDraw::drawDone(){
  char name[20];
  sprintf(name, "Done");
  double buttonPosX = nextPos[0];
  double buttonPosY = nextPos[1];
  double buttonW = largeButton[0];
  double buttonH = largeButton[1];
  double border = largeBorder;
  double textPosX = buttonPosX + 30.;
  double textPosY = buttonPosY + 30.;
  bool select = isNextSelected || isNextClicked;  

  drawButton(name,
             textPosX,
             textPosY,
             buttonPosX,
             buttonPosY,
             buttonW,
             buttonH,
             border,
             select);
}

void myDraw::drawTime(){
   drawBitmap(timeObj->timeString, 
              nextPos[0] - largeButton[0], 
              nextPos[1] + largeButton[1]);
//viewproperty.image_width/40.,viewproperty.image_height/40.+100.);
}

void myDraw::drawVisLevel(){
        glColor3f(1.,1.,1.);
        char str[100];
        int encode = order->datainfo->encodeID[order->trialPointer];
        if(encode == 1){
          sprintf(str,"How confident were you in accomplishing what you were asked to do using the color-length method?");
        }
        else if(encode == 2){
          sprintf(str,"How confident were you in accomplishing what you were asked to do using the texture-length method?");
        }
        else if(encode == 3){
          sprintf(str,"How confident were you in accomplishing what you were asked to do using the length-length method?");
        }
        float x = viewproperty.viewport[2]/3.;
        float y = viewproperty.viewport[3]/3.;//+viewproperty.viewport[3]/20.;
        glRasterPos2f(x,y);
        for(int i=0;i<strlen(str);i++)
           glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[i]);

        sprintf(str,"Very low");
        glRasterPos2f(confiVisPos[0][0],confiVisPos[0][1]+confidenceButton[1]*1.5);
        for(int i=0;i<strlen(str);i++)
                glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[i]);

        for(int i=0;i<confidenceCount;i++){
           char name[20];
           sprintf(name, "%d", i+1); //cout<<name<<endl;
           double buttonPosX = confiVisPos[i][0];
           double buttonPosY = confiVisPos[i][1];
           double buttonW = confidenceButton[0];
           double buttonH = confidenceButton[1];
           double border = confidenceBorder;
           double textPosX = buttonPosX+confidenceButton[0]/3.;// + 50.;
           double textPosY = buttonPosY+confidenceButton[1]*2./3.;//- 20.;
           bool select;
           select = order->datainfo->visConfi[order->trialPointer] == i
                   || isConfiVisSelected == i; 
//           select = (isConfiVisSelected == i)||(isConfiVisClicked == i);     
           drawButton(name,
             textPosX,
             textPosY,
             buttonPosX,
             buttonPosY,
             buttonW,
             buttonH,
             border,
             select);
        }

        glColor3f(1.,1.,1.);
        sprintf(str,"Neutral");
        glRasterPos2f(confiVisPos[3][0], confiVisPos[0][1]+confidenceButton[1]*1.5);
        for(int i=0;i<strlen(str);i++)
                glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[i]);
        glColor3f(1.,1.,1.);
        sprintf(str,"Very high");
        glRasterPos2f(confiVisPos[6][0], confiVisPos[0][1]+confidenceButton[1]*1.5);
        for(int i=0;i<strlen(str);i++)
                glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[i]);
}
void myDraw::drawDataLevel(){
        glColor3f(1.,1.,1.);
        char str[100];
        int dataid = order->datainfo->dataID[order->trialPointer];

     for(int j=0;j<2;j++){
      if(j==0)
        sprintf(str,"How confident were you in accomplishing what you were asked to do using the STRUCTURED data?");
      else
        sprintf(str,"How confident were you in accomplishing what you were asked to do using the RANDOM data?");
        float x = viewproperty.viewport[2]/3.;
        float y = viewproperty.viewport[3]/3.+viewproperty.viewport[3]/5.*j;
        glRasterPos2f(x,y);
        for(int i=0;i<strlen(str);i++)
           glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[i]);

        sprintf(str,"Very low");
        glRasterPos2f(confiDataPos[j][0][0],confiDataPos[j][0][1]+confidenceButton[1]*1.5);
        for(int i=0;i<strlen(str);i++)
                glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[i]);

        for(int i=0;i<confidenceCount;i++){
           char name[20];
           sprintf(name, "%d", i+1); //cout<<name<<endl;
           double buttonPosX = confiDataPos[j][i][0];
           double buttonPosY = confiDataPos[j][i][1];
           double buttonW = confidenceButton[0];
           double buttonH = confidenceButton[1];
           double border = confidenceBorder;
           double textPosX = buttonPosX+confidenceButton[0]/3.;// + 50.;
           double textPosY = buttonPosY+confidenceButton[1]*2./3.;//- 20.;
           bool select;
           select = order->datainfo->dataConfi[j][order->trialPointer] == i
                   || isConfiDataSelected[j] == i; 
          // select = (isConfiDataSelected == i)||(isConfiDataClicked == i);     
           drawButton(name,
             textPosX,
             textPosY,
             buttonPosX,
             buttonPosY,
             buttonW,
             buttonH,
             border,
             select);
        }

        glColor3f(1.,1.,1.);
        sprintf(str,"Neutral");
        glRasterPos2f(confiDataPos[j][3][0], confiDataPos[j][0][1]+confidenceButton[1]*1.5);
        for(int i=0;i<strlen(str);i++)
                glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[i]);
        glColor3f(1.,1.,1.);
        sprintf(str,"Very high");
        glRasterPos2f(confiDataPos[j][6][0], confiDataPos[j][0][1]+confidenceButton[1]*1.5);
        for(int i=0;i<strlen(str);i++)
                glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[i]);
      }
}

void myDraw::drawEnd(){
    glColor3f(1.,1.,1.);
    float x, y;
    char str[40];
    char *tmps = strdup("The end.");
    x = viewproperty.viewport[2]/2.; 
    y = viewproperty.viewport[3]/2.;
    glRasterPos2f(x, y);
    sprintf(str,"%s",tmps);
    for(int i=0;i<strlen(str);i++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[i]);  
/*    glRasterPos2f(x, y+100.);
    sprintf(str,"All the tasks are done.");
    for(int i=0;i<strlen(str);i++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[i]);  
*/
/*
  char name[20];
  sprintf(name, "Exit");
  double buttonPosX = exitPos[0];
  double buttonPosY = exitPos[1];
  double buttonW = largeButton[0];
  double buttonH = largeButton[1];
  double border = largeBorder;
  double textPosX = buttonPosX + largeButton[0]/3.;
  double textPosY = buttonPosY + largeButton[1]*2/3.;
  bool select = isExitSelected || isExitClicked;  

  drawButton(name,
             textPosX,
             textPosY,
             buttonPosX,
             buttonPosY,
             buttonW,
             buttonH,
             border,
             select);
*/
}

void myDraw::drawEachTask(){
     glColor3f(1.,1.,1.);
     char str[200];

     char dd[100];
/*     if(order->datainfo->dataID[order->trialPointer]==1)
         sprintf(dd,"   Data distribution: Group");
     else
         sprintf(dd,"   Data distribution: Random");
*/
     if(state.taskID==0){
        sprintf(str, "Find: ");
        float x = draw->optionPos[0][0]-75;//viewproperty.viewport[2]/3.;
        float y = draw->optionPos[0][1]+50;//+50;//viewproperty.viewport[3]-50;
        glRasterPos2f(x,y);
        for(int i=0;i<strlen(str);i++)
           glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[i]);
     }
     else if(state.taskID==1){
       sprintf(str, "Find the vector with max magnitude");
        float x = draw->optionPos[0][0]-190;//-10;//viewproperty.viewport[2]/3.;
        float y = draw->optionPos[0][1]+50;//viewproperty.viewport[3]-50;
        glRasterPos2f(x,y);
        for(int i=0;i<strlen(str);i++)
           glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[i]);
        sprintf(str, "when power is %d",order->datainfo->task2X[order->dataPointer]);
        glRasterPos2f(x,y+25);
        for(int i=0;i<strlen(str);i++)
           glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[i]);

        float translatexy[2] = {140, 35};
        if(order->datainfo->encodeID[order->trialPointer]==3){
           translatexy[1]-=5;
        }
/*
           int power = order->datainfo->task2X[order->dataPointer];
           if(power==0){
              translatexy[1]-=14;
           }
           else if(power>2){
              translatexy[1] += (power-2) * 14;
           }
        }*/
        glPushMatrix();
        glTranslatef(x+translatexy[0],y+translatexy[1],0);
        glScalef(1.5,1.5,1.5);
        if(order->datainfo->encodeID[order->trialPointer]==1)
         myGlyphs->Single_ColorSize(order->datainfo->task2X[order->dataPointer]);
        else if(order->datainfo->encodeID[order->trialPointer]==2)
         myGlyphs->Single_TextureSize(order->datainfo->task2X[order->dataPointer]);
        else if(order->datainfo->encodeID[order->trialPointer]==3){
         double exp = myGlyphs->getExp(order->datainfo->task2Answer[order->dataPointer]);
         if(exp > 0.9)
             myGlyphs->Single_Dual(order->datainfo->task2X[order->dataPointer]);
        }
        glPopMatrix();

     }
     else if(state.taskID==2)
     {    sprintf(str, "Number of different powers:");
  //   else if(state.taskID==3)
  //     sprintf(str, "Task 3. Group 2: How many different exponents in the data? %s", dd);

     float x = draw->optionPos[0][0]-130;//viewproperty.viewport[2]/3.;
     float y = draw->optionPos[0][1]-20;//viewproperty.viewport[3]-50;
     glRasterPos2f(x,y);
     for(int i=0;i<strlen(str);i++)
       glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[i]);
    }
}

void myDraw::drawOptions(){
        glColor3f(1.,1.,1.);
        char str[40];
        char *input = strdup("Please select your answer.");
        sprintf(str,"%s",input);
        //glRasterPos2f(optionPos[0][0]-15, optionPos[0][1]-10);
        for(int i=0;i<strlen(str);i++){
          //      glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[i]);
        }
        for(int i=0;i<optionButtonCount;i++){
           char name[20];
           sprintf(name, "%d", i+3);
           double buttonPosX = optionPos[i][0];
           double buttonPosY = optionPos[i][1];
           double buttonW = optionButton[0];
           double buttonH = optionButton[1];
           double border = optionBorder;
           double textPosX = buttonPosX+optionButton[0]/2.5;// + 50.;
           double textPosY = buttonPosY+optionButton[1]*2./3.;// - 20.;
           bool select;
           select = (isOptionSelected == i)||(isOptionClicked == i);     
           drawButton(name,
             textPosX,
             textPosY,
             buttonPosX,
             buttonPosY,
             buttonW,
             buttonH,
             border,
             select);
      }
}

void myDraw::draw2D(){
  if(order->pagePointer == order->pageNum){
        drawEnd();
        return;
  }
  int pageState = order->pageOrder[order->pagePointer];
  int task =state.taskID; 
  switch(pageState){
     case 0:{
               drawTask();
               drawPage();
               break;
            }
     case 1:{
               drawDone();
               drawEachTask();
               drawProgress();
               if(showTime)drawTime();
               if(task == 2 || task == 3){
                 drawOptions();
               }
               break;
            }
     case 2:{
               //drawPage();
               //drawTime();
               //drawEachTask();
               //drawProgress();
               break;
            }
     case 3:{
               drawPage();
               drawEncode();
               //       drawProgress();
               //drawTask();
               break;
            }
     case 4:{
               drawVisLevel();
               drawDone();
               break;
            }
     case 5:{
               drawRest();
               drawPage();
               break;
            }
     case 6:{
               drawDataLevel();
               drawDone();
               break;
            }
     case 7:{
               drawPage();
               drawEachTask();
               drawProgress();
               drawFeedback();
               if(task == 2 || task == 3){
                 drawOptions();
               }
               break;
            }
  }
}

class myMouse{
public:
  void leftMouse(int x, int y);
  void leftMovement(int x, int y);
  void middleMouse(int x, int y);  
  void release();

  svQDOTDrawing *glyph;
  myDraw *draw;
  int mousex;
  int mousey;
};
myMouse *mouseObj;

void myMouse::release(){
  draw->isNextSelected = false;
  draw->isNextClicked = false;
//  draw->isExitSelected=false;
//  draw->isExitClicked=false;
  draw->isOptionSelected=-1;
  draw->isOptionClicked=-1;
  draw->isConfiVisSelected=-1;
  draw->isConfiVisClicked=-1;
  draw->isConfiDataSelected[0]=-1;
  draw->isConfiDataClicked[0]=-1;
  draw->isConfiDataSelected[1]=-1;
  draw->isConfiDataClicked[1]=-1;
}

void SetData(){
   char dataname[20];
   int task = state.taskID;
   int power;
   int index;
   switch(task){
         case 0:{
              power = order->datainfo->task1Power[order->dataPointer];
              index = order->datainfo->task1Index[order->dataPointer];
              break;
         }
         case 1:{
              power = order->datainfo->task2Power[order->dataPointer];
              index = order->datainfo->task2Index[order->dataPointer];
              break;
         }
         case 2:{
              power = order->datainfo->task3Power[order->dataPointer];
              index = order->datainfo->task3Index[order->dataPointer];
              break;
         }
         case 3:{
              power = order->datainfo->task4Power[order->dataPointer];
              index = order->datainfo->task4Index[order->dataPointer];
              break;
         }
   }
   if(order->datainfo->dataID[order->trialPointer] == 1)
   sprintf(dataname, "studydata2/D%d%d", power, index);
   else
   sprintf(dataname, "studydata2/R%d%d", power, index);
   //cout<<dataname<<endl;
   myGlyphs->SetData(dataname);
}

void reloadLastData(){
   if(order->pagePointer < 3) return;
   bool newdata = (order->pageOrder[order->pagePointer]==1)?true:false;
   bool newtask = (order->pageOrder[order->pagePointer]==0)?true:false;
   if(order->pageOrder[order->pagePointer]==0){
           state.taskID--;
           order->dataPointer = 29;
   }
   order->pagePointer--;
   while(order->pageOrder[order->pagePointer]!=1){
        if(order->pageOrder[order->pagePointer]==0){ 
           state.taskID--;
           order->dataPointer = 30;
        }
	order->pagePointer--;
   }
   //cout<<order->dataPointer<<endl;
//   cout<<state.taskID<<endl;
   mouseObj->release();
   if(newdata){
     order->trialPointer--;
     order->dataPointer--;
   }
   SetData();
   order->datainfo->isBack[order->trialPointer] = true;
   //out<<order->datainfo->task1Answer[order->dataPointer]<<endl;
   trackball.reset();
   showGlyphs = true;
   //showAnswers = true;
   giveAnswer = true;
   ostringstream timestream;  // used to format time string
   timestream << setfill('0') <<  setw(2) 
              << timeLimit[state.taskID] ;
   timeObj->timeString=timestream.str(); 
   timeObj->sec = 0;
   timeObj->startClock = true;//false;
}

void reloadData(){
                     //timeObj->endTime = timeObj->GetTime();
                     if(order->trialPointer>=0 && order->dataPointer>=0){
                          recordLog();
                          recordAnswer();
                          recordTime();
                          recordConfidence();
                     }
                     mouseObj->release();
                     order->trialPointer++;
                     order->dataPointer++;
                     SetData();
                     //cout<<order->datainfo->task1Answer[order->dataPointer]<<endl;
                     timeObj->beginTime = timeObj->GetTime();
                     trackball.reset();
                     showGlyphs = true;
                     showTime = true;
                     showAnswers = false;
                     giveAnswer = false;
                     ostringstream timestream;  // used to format time string
                     timestream << setfill('0') <<  setw(2) 
                                << timeLimit[state.taskID] ;
                     timeObj->timeString=timestream.str(); 
                     timeObj->sec = 0;
                     timeObj->startClock = true;
                     if(order->datainfo->isBack[order->trialPointer]){
                        //timeObj->startClock = false;
                     }
                     else{
                        //timeObj->startClock = true;
                     }
}

void myMouse::leftMouse(int x, int y){
  /*if(x > draw->answerPos[0] && x < draw->answerPos[0] + draw->largeButton[0]
  && y > draw->answerPos[1] && y < draw->answerPos[1] + draw->largeButton[1]
  && order->pageOrder[order->pagePointer]==1){
    if(draw->isAnswerClicked == false){
     draw->isAnswerClicked = true;
     showGlyphs = true;
     showAnswers = true;
    }
  }
  else if(x > draw->backPos[0] && x < draw->backPos[0] + draw->largeButton[0]
  && y > draw->backPos[1] && y < draw->backPos[1] + draw->largeButton[1]
  && order->pagePointer>2){
     reloadLastData();
  }
  else
  */ 
  if(x > draw->nextPos[0] && x < draw->nextPos[0] + draw->largeButton[0]
  && y > draw->nextPos[1] && y < draw->nextPos[1] + draw->largeButton[1]
  && order->pageOrder[order->pagePointer]!=2){
     draw->isNextClicked = true;
     order->pagePointer++;
     
    // cout<<order->pagePointer<<" "<<order->pageOrder[order->pagePointer]<<endl;
     if(order->pagePointer == order->pageNum){
        if((order->datainfo->visConfi[order->trialPointer]<0)
                   || (order->datainfo->dataConfi[0][order->trialPointer]<0)
                   || (order->datainfo->dataConfi[1][order->trialPointer]<0)){
           order->pagePointer--;
        }
        else{
          recordLog();
          recordAnswer();
          recordTime();
          recordConfidence();
        }
     }
     else if(order->pagePointer < order->pageNum){
        switch(order->pageOrder[order->pagePointer]){
           case 0:{
                   if(0){
                   //order->pagePointer>1
                   //&& !giveAnswer
                   //&& !order->datainfo->isBack[order->trialPointer]){ 
                     order->pagePointer--;
                   }
                   else{
                     showRest = false;
                     if(order->trialPointer>=0){
                          recordLog();
                          recordAnswer();
                          recordTime();
                          recordConfidence();
                     }
                     order->dataPointer = -1;
                     state.taskID++; 
                   }
                   //cout<<"task ID "<<state.taskID<<endl;
                   break;
                  }
           case 1:{
                   if(state.taskID==2//order->pagePointer>1
                   && !giveAnswer){
                   //&& !order->datainfo->isBack[order->trialPointer]){//order->datainfo->userAnswer[order->trialPointer]<0){
                     order->pagePointer--;
                   }
                   else{
                     reloadData();
                   }
                   break;
                  }
           case 2:{
                   if(giveAnswer){
                     timeObj->startClock = true;
                     timeObj->endTime = timeObj->GetTime();
       	             recordLog();
                     timeObj->sec = timeLimit[state.taskID]*10;
                   }
                   else
                     order->pagePointer--;
                     //order->pageOrder++;
                     break;
                  }
           case 7:{
                    if(!giveAnswer){
                       order->pagePointer--;
                    }
                    else{
                      showGlyphs = true;
                      timeObj->startClock = false;
                    }
                    break;
                  }
           case 5:
                  {
                   //cout<<order->datainfo->userAnswer[order->trialPointer]<<endl;
                   //if(order->pagePointer>1
                   //&& (//!(//giveAnswer
                       //order->datainfo->isBack[order->trialPointer])
                    //(order->datainfo->visConfi[order->trialPointer]<0)
                   //|| (order->datainfo->dataConfi[order->trialPointer]<0))
                   //){//order->datainfo->userAnswer[order->trialPointer]<0){
                   if(order->datainfo->dataConfi[0][order->trialPointer]<0
                    ||order->datainfo->dataConfi[1][order->trialPointer]<0){
                      order->pagePointer--;
                   }
                   else{
                      showRest = true;
                      timeObj->startClock = true;
                      timeObj->sec = 0;
                   }
                   break;
                  }
           case 3:{
                     if(order->datainfo->visConfi[order->trialPointer]<0
                     && order->pageOrder[order->pagePointer-1]==4)
                           order->pagePointer--;
                     break;
                  }
           case 6:{
                   if(order->datainfo->visConfi[order->trialPointer]<0){
                      order->pagePointer--;
                   }
                   break;
                  }
        }
     } 
  }
//confidence 
  else if(order->pageOrder[order->pagePointer] == 4){
     for(int i=0;i<draw->confidenceCount;i++){
         if(x > draw->confiVisPos[i][0]
         && x < draw->confiVisPos[i][0] + draw->confidenceButton[0]
         && y > draw->confiVisPos[i][1]
         && y < draw->confiVisPos[i][1] + draw->confidenceButton[1]){
            draw->isConfiVisClicked = i;
            order->datainfo->visConfi[order->trialPointer] = i;
            break;
         }
     }
  }
  else if(order->pageOrder[order->pagePointer] == 6){
    for(int j=0;j<2;j++){
     for(int i=0;i<draw->confidenceCount;i++){
         if(x > draw->confiDataPos[j][i][0]
         && x < draw->confiDataPos[j][i][0] + draw->confidenceButton[0]
         && y > draw->confiDataPos[j][i][1]
         && y < draw->confiDataPos[j][i][1] + draw->confidenceButton[1]){
            draw->isConfiDataClicked[j] = i;
            order->datainfo->dataConfi[j][order->trialPointer] = i;
            break;
         }
     }
    }
  } 
//task 3
  else if(order->pageOrder[order->pagePointer] == 1
       && state.taskID >= 2){
     for(int i=0;i<draw->optionButtonCount;i++){
         if(x > draw->optionPos[i][0]
         && x < draw->optionPos[i][0] + draw->optionButton[0]
         && y > draw->optionPos[i][1]
         && y < draw->optionPos[i][1] + draw->optionButton[1]){
            draw->isOptionClicked = i;
            giveAnswer = true;
            order->datainfo->userAnswer[order->trialPointer] = i;
            break;
         }
     }
  } 
}

void myMouse::leftMovement(int x, int y){
  /*if(order->pagePointer == order->pageNum){
      if(x > draw->exitPos[0] && x < draw->exitPos[0] + draw->largeButton[0]
      && y > draw->exitPos[1] && y < draw->exitPos[1] + draw->largeButton[1]){
          draw->isExitSelected = true;
      }
      else{
          draw->isExitSelected = false;
      }
  }*/

//  cout<<"Test "<<order->pageOrder[order->pagePointer]<<" "<<state.taskID<<endl;
  /*if(x > draw->backPos[0] && x < draw->backPos[0] + draw->largeButton[0]
  && y > draw->backPos[1] && y < draw->backPos[1] + draw->largeButton[1]){
     draw->isBackSelected = true;
  }
  else{
     draw->isBackSelected = false;
  }
  if(x > draw->answerPos[0] && x < draw->answerPos[0] + draw->largeButton[0]
  && y > draw->answerPos[1] && y < draw->answerPos[1] + draw->largeButton[1]){
     draw->isAnswerSelected = true;
  }
  else{
     draw->isAnswerSelected = false;
  }*/
  if(x > draw->nextPos[0] && x < draw->nextPos[0] + draw->largeButton[0]
  && y > draw->nextPos[1] && y < draw->nextPos[1] + draw->largeButton[1]){
     draw->isNextSelected = true;
  }
  else{
     draw->isNextSelected = false;
  }
//confidence 
  if(order->pageOrder[order->pagePointer] == 4){
     draw->isConfiVisSelected = -1;
     for(int i=0;i<draw->confidenceCount;i++){
         if(x > draw->confiVisPos[i][0]
         && x < draw->confiVisPos[i][0] + draw->confidenceButton[0]
         && y > draw->confiVisPos[i][1]
         && y < draw->confiVisPos[i][1] + draw->confidenceButton[1]){
            draw->isConfiVisSelected = i;break;
         }
     }
  }
  if(order->pageOrder[order->pagePointer] == 6){
   for(int j=0;j<2;j++){
     draw->isConfiDataSelected[j] = -1;
     for(int i=0;i<draw->confidenceCount;i++){
         if(x > draw->confiDataPos[j][i][0]
         && x < draw->confiDataPos[j][i][0] + draw->confidenceButton[0]
         && y > draw->confiDataPos[j][i][1]
         && y < draw->confiDataPos[j][i][1] + draw->confidenceButton[1]){
            draw->isConfiDataSelected[j] = i;break;
         }
     }
   }
  } 
//task 3
  if(order->pageOrder[order->pagePointer] == 1
       && state.taskID >= 2){
     draw->isOptionSelected = -1;
     for(int i=0;i<draw->optionButtonCount;i++){
         if(x > draw->optionPos[i][0]
         && x < draw->optionPos[i][0] + draw->optionButton[0]
         && y > draw->optionPos[i][1]
         && y < draw->optionPos[i][1] + draw->optionButton[1]){
            draw->isOptionSelected = i;break;
         }
     }
  } 
}

void timer(int v){
  if(showRest){
   ostringstream timestream;      // used to format time string
   if (timeObj->startClock){
    timeObj->sec++;         // increment second
    timestream << setfill('0') <<  setw(2) << 60 - timeObj->sec/10;
    timeObj->timeString=timestream.str();  //convert stream to string
    if(timeObj->sec/10 == 60){
       timeObj->startClock = false;
    }
   }
  }
  else if(state.taskID<2){
   //cout<<timeObj->startClock<<endl;
   ostringstream timestream;      // used to format time string
   if (timeObj->startClock){
    timeObj->sec++;         // increment second
    timestream << setfill('0') <<  setw(2) << timeLimit[state.taskID] 
             - timeObj->sec/10;
    timeObj->timeString=timestream.str();  //convert stream to string
    if(order->pageOrder[order->pagePointer]==1
    && timeObj->sec/10 == timeLimit[state.taskID]){
       timeObj->endTime = timeObj->GetTime();
       recordLog();
       order->pagePointer++;
     //  timeObj->startClock = false;
     //  showGlyphs = false;
       giveAnswer = true;
       //showAnswers = true;
    }
    else if(order->pageOrder[order->pagePointer]==2
         && timeObj->sec == timeLimit[state.taskID]*10+5){
       timeObj->startClock = false;
       order->pagePointer++;
       reloadData();
    }
    /*else if(order->pageOrder[order->pagePointer]==7){
       timeObj->startClock = false;
       order->pagePointer++;
       reloadData();
    }
    else if(order->pageOrder[order->pagePointer]==3){
       timeObj->startClock = false;
    }
    else if(order->pageOrder[order->pagePointer]==4){
       timeObj->startClock = false;
    }*/
    /*else if(showGlyphs && timeObj->sec == 1){
       recordLog();
       showGlyphs = true;
       timeObj->startClock = false;
       order->pagePointer++;
       reloadData();
    }*/
   }
    /*else if(order->pageOrder[order->pagePointer]==2
    && timeObj->sec == timeLimit[state.taskID]+1){

    }*/
  }
  else {
   ostringstream timestream;      // used to format time string
   if (timeObj->startClock){
    timeObj->sec++;         // increment second
    timestream << setfill('0') <<  setw(2) << timeLimit[state.taskID] 
             - timeObj->sec/10;
    timeObj->timeString=timestream.str();  //convert stream to string
    if(timeObj->sec/10 == timeLimit[state.taskID]){
       recordLog();
       showGlyphs = false;
       timeObj->startClock = false;
       showTime = false;
    }
   }
  }
/*
  else if(order->pageOrder[order->pagePointer]==2){
   if (timeObj->startClock) timeObj->sec++;         // increment second
    cout<<timeObj->sec<<endl;
    if(timeObj->sec == 1){//timeLimit[state.taskID]){
       timeObj->startClock = false;
       recordLog();
    }
    order->pagePointer++;
    reloadData();
  }
*/
  glutPostRedisplay();
  glutTimerFunc(1000/v, timer, v);  //repost timer 
}

void key(unsigned char key, int x, int y){
  switch (key) {
    case 'h':
                trackball.reset();
                break;
    case 'H':
                trackball.reset();
                break;
  }
  recordLog();
  glutPostRedisplay();
}

void motion(int x, int y){
  trackball.mouseMotion(x, y);
  recordLog();
  glutPostRedisplay();
}

void movement(int x, int y){
  mouseObj->leftMovement(x,y);
  glutPostRedisplay();
}

void mouse(int button, int mstate, int x, int y){
    long s=0x00000000;
    mousebutton = button;
    mousestate = mstate;

        s |= (button == GLUT_LEFT_BUTTON)   ? ((mstate == GLUT_DOWN) ? Trackball::LBUTTON_DOWN : Trackball::LBUTTON_UP) : 0;
        s |= (button == GLUT_RIGHT_BUTTON)  ? ((mstate == GLUT_DOWN) ? Trackball::RBUTTON_DOWN : Trackball::RBUTTON_UP) : 0;

        int key_state = glutGetModifiers();
        s |= (key_state & GLUT_ACTIVE_CTRL)  ? Trackball::CTRL_DOWN  : 0;
//        s |= (key_state & GLUT_ACTIVE_ALT)   ? Trackball::ALT_DOWN   : 0;
//        s |= (key_state & GLUT_ACTIVE_SHIFT) ? Trackball::SHIFT_DOWN : 0;


            if(order->pagePointer < order->pageNum){
               if(order->pageOrder[order->pagePointer]==1
               || order->pageOrder[order->pagePointer]==7){
                  if (s & Trackball::BUTTON_DOWN){
                    trackball.mouseDown(s, x, y);
                  }
               }
            }
                                  if (s & Trackball::BUTTON_UP){
                                    trackball.mouseUp(s, x, y);
                                        }

        if((button == GLUT_LEFT_BUTTON) && (mstate==GLUT_DOWN)){
            mouseObj->leftMouse(x, y);     
        }
        else if((button == GLUT_LEFT_BUTTON) && (mstate==GLUT_UP)){
            //mouseObj->release();
          draw->isNextSelected = false;
          draw->isNextClicked = false;
          draw->isAnswerSelected =false;
          draw->isAnswerClicked =false;
          //draw->isAnswerSelected =false;
          //draw->isBackClicked = false;
          //draw->isBackClicked = false;
          //draw->isExitSelected=false;
          //draw->isExitClicked=false;
        }
        else if(button == GLUT_MIDDLE_BUTTON && mstate==GLUT_DOWN
       && (state.taskID < 2) && order->pageOrder[order->pagePointer] == 1){
          order->datainfo->userAnswer[order->trialPointer] =  
                     myGlyphs -> Find_Nearest(x,y,//viewproperty.image_height,
                                order->datainfo->encodeID[order->trialPointer],
                                viewproperty.m);
          if(order->datainfo->userAnswer[order->trialPointer]>=0)
               giveAnswer = true;
        }
        recordLog();
        glutPostRedisplay();
}

void InitLight()
{
  GLfloat mat_diffuse[] = { 1.0, 0.0, 0.0, 0.2 };
  GLfloat mat_specular[] = { 0.2, 0.2, 0.2, 0.5 };
  GLfloat mat_shininess[] = { 10.0 };
  GLfloat light_position[] = { 100.0, 100.0, 100.0, 0.0 };
  GLfloat white_light[] = { 1.0, 1.0, 1.0, 1.0 };
  glShadeModel(GL_SMOOTH);
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat_diffuse);
  glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
  glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
  glLightfv(GL_LIGHT0, GL_POSITION, light_position);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, white_light);
  glLightfv(GL_LIGHT0, GL_SPECULAR, white_light);
}

void reshape(int width, int height){
  viewproperty.image_width = width;
  viewproperty.image_height = height;


    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, (GLsizei) width, (GLsizei) height);

                glMatrixMode(GL_PROJECTION);
                glLoadIdentity();
                gluPerspective(45, 1.0*width/height,0.5, 100);
                glMatrixMode(GL_MODELVIEW);
                glLoadIdentity();
                gluLookAt(0,0,viewproperty.eye[2],
                      0,0,0,
                      0, 1, 0);
  trackball.setEye(viewproperty.eye);
  myGlyphs->SetCamera(width, height, viewproperty.eye);
  viewproperty.viewport[2] = width;
  viewproperty.viewport[3] = height;

  draw->setParameters();

  InitLight();
}

void drawLegend(){
   if(order->pagePointer == order->pageNum) return;

   int task = state.taskID;
   int encode = order->datainfo->encodeID[order->trialPointer];
   if((order->pageOrder[order->pagePointer] == 1
     ||order->pageOrder[order->pagePointer] == 7)
   && task<2){
        glDisable(GL_DEPTH_TEST);
        glPushMatrix();
        myGlyphs->Large_Font_Legend();
        if(task == 1)
        myGlyphs->Large_Axis_Legend(order->datainfo->task2X[order->dataPointer]);
        else
        myGlyphs->Large_Axis_Legend(-1);
        if(encode == 1)
            myGlyphs->ColorSize_Legend();//ize(viewproperty.m);
        else if(encode == 2)
            myGlyphs->TextureSize_Legend();//(viewproperty.m);
        else if(encode == 3)
            myGlyphs->Dual_Legend();
        glPopMatrix();
        glEnable(GL_DEPTH_TEST); 

     if(task == 0){
        glColor3f(1,1,1);
        char str[10];
        //sprintf(str, "%6.2e", myGlyphs->getMag(order->datainfo->task1Answer[order->dataPointer]));
        sprintf(str,"%0.2fx10", myGlyphs->getCoe(order->datainfo->task1Answer[order->dataPointer]));
        glRasterPos2f(draw->optionPos[0][0]-30,
                      draw->optionPos[0][1]+80);
        for(int j=0;j<strlen(str);j++)
               glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[j]);
        sprintf(str,"%d", (int)myGlyphs->getExp(order->datainfo->task1Answer[order->dataPointer]));
        glRasterPos2f(draw->optionPos[0][0]+35,
                      draw->optionPos[0][1]+75);
        for(int j=0;j<strlen(str);j++)
               glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[j]);
     }
   }
}
void draw3D(){
   if(order->pagePointer == order->pageNum) return;

   int encode = order->datainfo->encodeID[order->trialPointer];
   if(order->pageOrder[order->pagePointer] == 1
   || order->pageOrder[order->pagePointer] == 7){
    if(showGlyphs){//cout<<showGlyphs<<endl;
        glPushMatrix();
        glMultMatrixf(viewproperty.m);
        glRotatef(90,1,0,0);
        if(encode == 1)
            myGlyphs->Hue_Size(viewproperty.m);
        else if(encode == 2)
            myGlyphs->Texture_Size(viewproperty.m);
        else if(encode == 3)
            myGlyphs->Double_Cylinder(viewproperty.m);
        myGlyphs->RenderBounding();
        glPopMatrix();
    }

       if(state.taskID == 0){
         int locationx = viewproperty.image_width*7./8.-30.;
         int locationy = viewproperty.image_height/6-25;
         int widthx = viewproperty.image_width/8.+30.;
         int widthy = viewproperty.image_height/4.;
         glViewport(locationx, locationy, widthx, widthy);
         glMatrixMode(GL_PROJECTION);
         glLoadIdentity();
         gluPerspective(45, 1.0*widthx/widthy,0.5, 100);
         glMatrixMode(GL_MODELVIEW);
         glLoadIdentity();
         gluLookAt(0,0,1,
                      0,0,0,
                      0, 1, 0);
         glPushMatrix();
         glRotatef(90,1,0,0);
         if(encode == 1)
          myGlyphs->Hue_Size_Single(viewproperty.im,order->datainfo->task1Answer[order->dataPointer]);
         else if(encode == 2)
          myGlyphs->Texture_Size_Single(viewproperty.im,order->datainfo->task1Answer[order->dataPointer]);
         else if(encode == 3)
          myGlyphs->Double_Cylinder_Single(viewproperty.im,order->datainfo->task1Answer[order->dataPointer]);
          glPopMatrix();
        }        
   }
}

void display(void){
      glClearColor(0.6,0.6,0.6,1.0);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

 
      trackball.getMatrix().getValue(viewproperty.m);
      glEnable(GL_DEPTH_TEST);

      draw3D();

      glViewport(0, 0, (GLsizei) viewproperty.image_width, (GLsizei) viewproperty.image_height);
      glMatrixMode(GL_PROJECTION);
      glLoadIdentity();
      gluPerspective(45, 1.0*viewproperty.image_width/viewproperty.image_height,0.5, 100);
      glMatrixMode(GL_MODELVIEW);
      glLoadIdentity();
      gluLookAt(0,0,viewproperty.eye[2],
                      0,0,0,
                      0, 1, 0);
      glGetIntegerv (GL_VIEWPORT, viewproperty.viewport);
      glGetDoublev (GL_MODELVIEW_MATRIX,viewproperty.vmat);
      glGetDoublev (GL_PROJECTION_MATRIX, viewproperty.pmat);

            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            gluOrtho2D(0.0,(GLfloat) viewproperty.viewport[2], 
                       (GLfloat) viewproperty.viewport[3], 0.0);
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            glDisable(GL_LIGHTING);

        draw->draw2D();
        drawLegend();


        glMatrixMode(GL_PROJECTION);
        glLoadMatrixd(viewproperty.pmat);
        glMatrixMode(GL_MODELVIEW);
        glLoadMatrixd(viewproperty.vmat);
        if((order->pageOrder[order->pagePointer] == 1 
        || order->pageOrder[order->pagePointer] == 7)
        && showGlyphs
        && state.taskID<2)
           myGlyphs->RenderAnswerMarker(
               (int)order->datainfo->userAnswer[order->trialPointer],
               viewproperty.m,
               order->datainfo->encodeID[order->trialPointer], false);
        if(order->pageOrder[order->pagePointer] == 7
       // && showGlyphs 
       // && showAnswers 
           && state.taskID==0)
           myGlyphs->RenderAnswerMarker(
               (int)order->datainfo->task1Answer[order->dataPointer],
               viewproperty.m,
               order->datainfo->encodeID[order->trialPointer],true);
        else if(order->pageOrder[order->pagePointer] == 7
       // && showGlyphs 
        //&& showAnswers 
           && state.taskID==1)
           myGlyphs->RenderAnswerMarker(
               (int)order->datainfo->task2Answer[order->dataPointer],
               viewproperty.m,
               order->datainfo->encodeID[order->trialPointer],true);

        glFlush();
        glutSwapBuffers();
}

//-------------------------------------------------------------//

void init(int argc, char **argv){
  myGlyphs = new svQDOTDrawing();
  order = new myOrder();
  draw = new myDraw(); 
  timeObj = new myTime();
  mouseObj = new myMouse();
  mouseObj->draw = draw;

  state.taskID = 0; 
  order->datainfo->pID  = atoi(argv[1]);

  char numstr[10]; // enough to hold all numbers up to 64-bits
  sprintf(numstr, "%d", order->datainfo->pID); 
  correctFile = "training/correct" + string(numstr);
  logFile = "training/log" + string(numstr);
  ofstream log(logFile.c_str(),fstream::app);
  log<<order->datainfo->pID<<endl;
  log.close();
  answerFile = "training/result" + string(numstr);
  timeFile = "training/time" + string(numstr);
  confidenceFile = "training/confidence" + string(numstr);

  string blockFile = "trainingOrder/order1";//+ string(numstr);
  order->readOrder(blockFile);
  string pageFile("trainingOrder/pageOrder");
  order->readPageOrder(pageFile);

  viewproperty.center[0]=0.;
  viewproperty.center[1]=0.;
  viewproperty.center[2]=0.;
  viewproperty.eye[0]=0.;
  viewproperty.eye[1]=0.;
  viewproperty.eye[2]=5.5;
  trackball.setEye(viewproperty.eye);
  trackball.setFocus(viewproperty.center);
  trackball.SetLogName((char*)logFile.c_str());
  trackball.getMatrix().getValue(viewproperty.im);

  timeObj->startClock = false;
  timeObj->wholeTime = 0;
  timeObj->hour = 0;
  timeObj->min = 0;
  timeObj->sec = 0;
  string str("00");
  (timeObj->timeString) = str;//.append(str);
  timeObj->beginTime = timeObj->GetTime();

}

int main(int argc, char **argv){
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE);//|GLUT_STEREO);//

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
   glutTimerFunc(1000,timer,FPS);

   glutMainLoop();
   return 0;
}
