#include "svQDOTDrawing.h"

#define cylinder_radius 0.05/2*1.7
#define exponent_num 4
#define checkImageWidth 128
#define checkImageHeight 128
static GLubyte checkImage[checkImageHeight][checkImageWidth][4];
static GLuint texName;

static GLubyte checkImageVertical[128][128][4];
static GLuint texNameVertical;
static GLubyte checkImageHorizontal[64][64][4];
static GLuint texNameHorizontal;

//double color[4][3] ={ {0., 68., 27.}, {35., 139., 69.}, {116., 196., 118.}, {199., 233., 192.}};

double color[5][7][3]={
{
   {141.,211, 199},
   {190.,186, 218},
   {255.,255, 179}
},
{
   {251.,128, 114},
   {141.,211, 199},
   {190.,186, 218},
   {255.,255, 179}
},
{
   {251.,128, 114},
   {128.,177, 211},
   {141.,211, 199},
   {190.,186, 218},
   {255.,255, 179}
},
{
   {251.,128, 114},
   {128.,177, 211},
   {141.,211, 199},
   {190.,186, 218},
   {253.,180, 98},
   {255.,255, 179}
},
{
   {251.,128, 114},
   {128.,177, 211},
   {141.,211, 199},
   {190.,186, 218},
   {253.,180, 98},
   {252,205,229},
   {255.,255, 179}
}
};

/*
double color[5][7][3]={
{
{145,191,219},
{255,255,191},
{252,141,89}
},
{
{44,123,182},
{171,217,233},
{253,174,97},
{215,25,28}
},
{
{44,123,182},
{171,217,233},
{255,255,191},
{253,174,97},
{215,25,28}
},

{
{69,117,180},
{145,191,219},
{224,243,248},
{254,224,144},
{252,141,89},
{215,48,39}
},

{
{49,54,149},
{69,117,180},
{171,217,233},
{254,224,144},
{253,174,97},
{244,109,67},
{215,48,39}
}*/


/*{
{69,117,180},
{145,191,219},
{224,243,248},
{255,255,191},
{254,224,144},
{252,141,89},
{215,48,39},
}*/
/*
double color3[3][3] ={
{145,191,219},
{255,255,191},
{252,141,89}
};
double color4[4][3] ={ {34., 94., 168.}, {65.,182.,196.}, {161., 218., 180.}, {255,237,160}};
double color5[5][3] = {
{44,123,182},
{171,217,233},
{255,255,191},
{253,174,97},
{215,25,28}
};
double color6[6][3] ={
{69,117,180},
{145,191,219},
{224,243,248},
{254,224,144},
{252,141,89},
{215,48,39}
};
double color7[7][3] ={
{69,117,180},
{145,191,219},
{224,243,248},
{255,255,191},
{254,224,144},
{252,141,89},
{215,48,39},
};
*/

//double color[4][3] ={ {35., 132., 67.}, {120.,198.,121.}, {194., 230., 153.}, {255., 255., 204.}};
double texture_color[4][3] = { {0., 29., 89.}, {0.,78.,91.}, {58., 82., 56.}, {117., 118., 75.}};
int texture_vertical_index[10][4] = {{0, 1, 0, 6}, {0, 2, 2, 6}, {2, 3, 2, 6}, {6, 4, 6, 6},
				    {1, 7, 1, 6}, {5, 10, 5, 6}, {8, 15, 8, 6}, {13, 26, 13, 6},
				    {29, 58, 29, 6},{64,0,64,0}};//128
				    /*{{1, 1, 2, 6}, {0, 2, 2, 6}, {2, 3, 3, 6}, {0, 5, 1, 6},
				    {2, 7, 3, 6}, {5, 10, 5, 6}, {8, 15, 9, 6}, {13, 26, 13, 6},
				    {29, 58, 29, 6},{64,0,64,0}};//128*/
int texture_horizontal_index[10][4] = {{1, 1, 1, 6}, {1, 2, 1, 6}, {2, 3, 2, 6}, {4, 4, 4, 6},
				    {5, 6, 5, 6}, {5, 10, 5, 6}, {8, 15, 8, 6}, {13, 26, 13, 6},
				    {29, 0, 29, 6},{32,0,32,0}};//64


//153 0 0  (170)
// 153 153 0
// 0 153 0
// 0 153 153

void svQDOTDrawing::InitLight()
{
  //setting of lighting

 /* //GLfloat light_position[] = { 24, 24, 60, 0.0 };
  GLfloat light_position[] = { 50,50,50, 0.0 };
  GLfloat white_light[] = { 1.0, 1.0, 1.0, 1.0 };
  //glClearColor(0.0, 0.0, 0.0, 0.0);
  GLfloat mat_diffuse[] = { 0.5, .5, 0.5, 1};
  GLfloat mat_specular[] = { 0.2, 0.2, 0.2, 1. };
  GLfloat mat_shininess[] = { 5.0 };
  GLfloat mat_ambient[] = {0.05, 0.05, 0.05,1};
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
  glLightfv(GL_LIGHT0, GL_POSITION, light_position);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, white_light);
  glLightfv(GL_LIGHT0, GL_SPECULAR, white_light);*/
}


void svQDOTDrawing::makeVerticalTexture(int n)
{
   int i, j;
   int count = 0;
   n = 9 - n;
    for (i = 0; i < 128; i++) 
    {
      for (j = 0; j < 128; j++) 
      {
	if(i<texture_vertical_index[n][0])
	{
	  checkImageVertical[i][j][0] = (GLubyte) 255;
	  checkImageVertical[i][j][1] = (GLubyte) 255;
	  checkImageVertical[i][j][2] = (GLubyte) 255;
	  checkImageVertical[i][j][3] = (GLubyte) 255;	  
	}
	else if(i>127 - texture_vertical_index[n][2])
	{
	  checkImageVertical[i][j][0] = (GLubyte) 255;
	  checkImageVertical[i][j][1] = (GLubyte) 255;
	  checkImageVertical[i][j][2] = (GLubyte) 255;
	  checkImageVertical[i][j][3] = (GLubyte) 255;	 	  
	}
	else
	{
	 if((i-texture_vertical_index[n][0])%(texture_vertical_index[n][1] + texture_vertical_index[n][3]) 
	    < texture_vertical_index[n][3])
	  {
	    checkImageVertical[i][j][0] = (GLubyte) 0;
	    checkImageVertical[i][j][1] = (GLubyte) 0;
	    checkImageVertical[i][j][2] = (GLubyte) 0;
	    checkImageVertical[i][j][3] = (GLubyte) 255;
	    
	  }
	  else
	  {
	    checkImageVertical[i][j][0] = (GLubyte) 255;
	    checkImageVertical[i][j][1] = (GLubyte) 255;
	    checkImageVertical[i][j][2] = (GLubyte) 255;
	    checkImageVertical[i][j][3] = (GLubyte) 255;	   
	  }
	}
      }
   } 
  // cerr<<n<<" "<<count<<endl;
}
void svQDOTDrawing::inittexture_vertical()
{
  // glShadeModel(GL_FLAT);
   glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
   glGenTextures(1, &texNameVertical);
   glBindTexture(GL_TEXTURE_2D, texNameVertical);

   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, 
     //              GL_NEAREST);
   //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, 
     //              GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MIN_FILTER,
                    GL_LINEAR_MIPMAP_LINEAR);
   //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 128, 
         //       128, 0, GL_RGBA, GL_UNSIGNED_BYTE, 
         //       checkImageVertical);
	gluBuild2DMipmaps(GL_TEXTURE_2D,
                      GL_RGBA,
                      128,128,
                      GL_RGBA,
                      GL_UNSIGNED_BYTE,
                      checkImageVertical);
}

void svQDOTDrawing::makeHorizontalTexture(int n)
{
   int i, j;
    for (i = 0; i < 64; i++) 
    {
      for (j = 0; j < 64; j++) 
      {
	if(i<texture_horizontal_index[n][0])
	{
	  checkImageHorizontal[i][j][0] = (GLubyte) 255;
	  checkImageHorizontal[i][j][1] = (GLubyte) 255;
	  checkImageHorizontal[i][j][2] = (GLubyte) 255;
	  checkImageHorizontal[i][j][3] = (GLubyte) 255;	  
	}
	else if(i>63 - texture_horizontal_index[n][3])
	{
	  checkImageHorizontal[i][j][0] = (GLubyte) 255;
	  checkImageHorizontal[i][j][1] = (GLubyte) 255;
	  checkImageHorizontal[i][j][2] = (GLubyte) 255;
	  checkImageHorizontal[i][j][3] = (GLubyte) 255;	 	  
	}
	else
	{
	  if((i-texture_horizontal_index[n][0])%(texture_horizontal_index[n][2] + texture_horizontal_index[n][3]) 
	    < texture_horizontal_index[n][2])
	  {
	    checkImageHorizontal[i][j][0] = (GLubyte) 0;
	    checkImageHorizontal[i][j][1] = (GLubyte) 0;
	    checkImageHorizontal[i][j][2] = (GLubyte) 0;
	    checkImageHorizontal[i][j][3] = (GLubyte) 255;
	  }
	  else
	  {
	    checkImageHorizontal[i][j][0] = (GLubyte) 255;
	    checkImageHorizontal[i][j][1] = (GLubyte) 255;
	    checkImageHorizontal[i][j][2] = (GLubyte) 255;
	    checkImageHorizontal[i][j][3] = (GLubyte) 255;	   
	  }
	}

      }
   } 
}
void svQDOTDrawing::inittexture_horizontal()
{
  // glShadeModel(GL_FLAT);
   glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
   glGenTextures(1, &texNameHorizontal);
   glBindTexture(GL_TEXTURE_2D, texNameHorizontal);

   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, 
                   GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, 
                   GL_NEAREST);
/*glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MIN_FILTER,
                    GL_LINEAR_MIPMAP_LINEAR);*/
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 64, 
                64, 0, GL_RGBA, GL_UNSIGNED_BYTE, 
                checkImageHorizontal);
	/*gluBuild2DMipmaps(GL_TEXTURE_2D,
                      GL_RGBA,
                      checkImageWidth, checkImageHeight,
                      GL_RGBA,
                      GL_UNSIGNED_BYTE,
                      checkImage);*/
}

void svQDOTDrawing::makeTexture(svVector4 c)
{
  int i, j;
    for (i = 0; i < checkImageHeight; i++) 
    {
      for (j = 0; j < checkImageWidth; j++) 
      {
         if(i%2 ==1)
	 {
	  checkImage[i][j][0] = (GLubyte) c[0]*255;
	  checkImage[i][j][1] = (GLubyte) c[1]*255;
	  checkImage[i][j][2] = (GLubyte) c[2]*255;
	  checkImage[i][j][3] = (GLubyte) 255;
	 }
	 else
	 {
	  checkImage[i][j][0] = (GLubyte) 255;
	  checkImage[i][j][1] = (GLubyte) 255;
	  checkImage[i][j][2] = (GLubyte) 255;
	  checkImage[i][j][3] = (GLubyte) 255;	   
	 }
      }
   }
int width, height;
//unsigned char* image = 

//SOIL_load_image("twocylinder_legend.png", &width, &height, 0, SOIL_LOAD_RGB);
//cerr<<width<<" "<<height<<endl;
//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
           //   GL_UNSIGNED_BYTE, image);

  // glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
   //glGenTextures(1, &amp;&texName);
   glBindTexture(GL_TEXTURE_2D, texName);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

   // glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);

   // glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    ///glTexParameteri(GL_TEXTURE_2D,
                 //   GL_TEXTURE_MIN_FILTER,
                //    GL_LINEAR_MIPMAP_LINEAR);
  // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, 
   //             height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 
   //             image);
	//gluBuild2DMipmaps(GL_TEXTURE_2D,
        //              GL_RGBA,
         //             checkImageWidth, checkImageHeight,
          //            GL_RGBA,
          //            GL_UNSIGNED_BYTE,
           //           checkImage);
}

void svQDOTDrawing::inittexture()
{
  // glShadeModel(GL_FLAT);
   glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
   glGenTextures(1, &texName);
   glBindTexture(GL_TEXTURE_2D, texName);

   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, 
                   GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, 
                   GL_NEAREST);
/*glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MIN_FILTER,
                    GL_LINEAR_MIPMAP_LINEAR);*/
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, checkImageWidth, 
                checkImageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, 
                checkImage);
	/*gluBuild2DMipmaps(GL_TEXTURE_2D,
                      GL_RGBA,
                      checkImageWidth, checkImageHeight,
                      GL_RGBA,
                      GL_UNSIGNED_BYTE,
                      checkImage);*/
}

svQDOTDrawing::svQDOTDrawing()
{
  dataSize = 1;
  
 dataPos = new svVector3[dataSize];
 dataVel = new svVector3[dataSize];
 dataDen = new svScalar[dataSize];
 dataExp = new svScalar[dataSize];
 dataCoe = new svScalar[dataSize];

 drawing = new svGlyph();
 
 TaskPoint.resize(4);//hard coding!!!!!!!!!!!!!!!
 
 dataDisplayList = 1;
}

int svQDOTDrawing::GetExponent(svScalar den)
{
	  int multiplier = 0;

	  svScalar numCopy = den;

	  if(numCopy<1&&numCopy!=0) 
	  {
	    while ((fabs(numCopy-1.)>0.000001)&&(numCopy-1.)<0.000001)
		{

		  multiplier--;//cerr<<num<<" "<<multiplier<<endl;
		  numCopy *= 10;
		}; // end while(1)
	  }
	  else if(numCopy==0)
		multiplier=0;
	  else if(numCopy>=1)
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

void svQDOTDrawing::SetData(char *file)
{
  cleanup();
  
 ifstream in(file);
 
 in>>dataSize;
 
 dataPos = new svVector3[dataSize];
 dataVel = new svVector3[dataSize];
 dataDen = new svScalar[dataSize];
 dataExp = new svScalar[dataSize];
 dataCoe = new svScalar[dataSize];
 
 lbbox[0] = 9e+9;
 lbbox[1] = 9e+9;
 lbbox[2] = 9e+9;
 rbbox[0] = -9e+9;
 rbbox[1] = -9e+9;
 rbbox[2] = -9e+9;
 maxExp = 0;
 //cout<<file<<" "<<dataSize<<endl;
 for(int i=0;i<dataSize;i++)
 {
   in >> dataPos[i][0] >> dataPos[i][1] >> dataPos[i][2]
	  >> dataVel[i][0] >> dataVel[i][1] >> dataVel[i][2]
	  >> dataDen[i] >> dataExp[i] >> dataCoe[i];
	
	dataVel[i] = normalize(dataVel[i]);	
	//dataDen[i] = dataDen[i];// / pow(10.,8.);

        if(dataExp[i] > maxExp) maxExp = dataExp[i];
	//dataExp[i] = (svScalar)GetExponent(dataDen[i]);
	//dataCoe[i] = dataDen[i]/pow(10., dataExp[i]);

	if(dataPos[i][0]<=lbbox[0])lbbox[0]=dataPos[i][0];
	if(dataPos[i][1]<=lbbox[1])lbbox[1]=dataPos[i][1];
	if(dataPos[i][2]<=lbbox[2])lbbox[2]=dataPos[i][2];
	if(dataPos[i][0]>=rbbox[0])rbbox[0]=dataPos[i][0];
	if(dataPos[i][1]>=rbbox[1])rbbox[1]=dataPos[i][1];
	if(dataPos[i][2]>=rbbox[2])rbbox[2]=dataPos[i][2];	

        //if(i==321) cout<<dataDen[i]<<endl;
 }
 //cout<<maxExp<<endl; 
//dataSize = 1;

 in.close();
}
/*
void svQDOTDrawing::SetTransform()
{
	GLfloat tm[16];
	int k =0;
    for(int i=0; i<4; i++)
    {
	    for(int j=0; j<4; j++)
            {
		tm[k]=0;
		k++;
	   }
   }

	drawing->SetTransform(tm,16);

	
}*/

void svQDOTDrawing::SetCamera(int w, int h, svVector3 e)
{
 
  image_width = w;
  image_height = h;
  eye =e ;
}

/*void svQDOTDrawing::SetTask(svChar *dir, svChar *inf)
{
	ifstream infile;
	char name[100];
	sprintf(name,"%s/%s",dir,inf);
	infile.open(name);
	if(!infile.is_open())
	{
		cerr<<"Can not find"<<name<<endl;
		exit(0);
	}

	int num;
	int number_data;
	int task_type;
	infile>>num;
	infile>>number_data;
	infile>>task_type;
	
	TaskPoint[task_type].clear();

	for(int i=0;i<num;i++)	
	{
		vector<int>  c(number_data);
		for(int j=0;j<number_data;j++)
		{
		  infile>>c[j];
		  TaskPoint[task_type].push_back(c[j]);
		}
	}

	infile.close();
}
*/
void svQDOTDrawing::Render(int list)
{
	glCallList(list);

}


void svQDOTDrawing::RenderBounding()
{
	drawing->RenderBounding(lbbox, rbbox);
}


void svQDOTDrawing::RenderInnerLegend()
{
	glNewList(dataDisplayList, GL_COMPILE);
/*
	for(int i=0;i<dataSize;i++)
	{
		drawing->Draw_InnerLegend(dataPos[i], dataVel[i], 
					  5,
					cylinder_radius);

	}*/
	glEndList();

	//dataDisplayList++;  
}

void svQDOTDrawing::Double_Cylinder(GLfloat *m)
{
	//glNewList(dataDisplayList, GL_COMPILE);
	svVector4 col;
	col[0] = 1; col[1] = 1; col[2] = 1; col[3] = 0.5;
	/*for(int i=0;i<dataSize;i++)
	{
		drawing->Draw_InnerLegend(dataPos[i], dataVel[i], 
					  5,
					      cylinder_radius);
	}*/

 /* for(int t=0;t<5;t++)
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
		alpha = f * 0.5;
	}
	else if(t == 2)
	{
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);
		glDepthFunc(GL_LEQUAL);
		alpha = (0.5 - f*0.5)/(1. - f*0.5);
	}
	else if(t == 3)
	{
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glDepthFunc(GL_ALWAYS);
		alpha = f * 0.5;
	}
	else if(t == 4)
	{
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glDepthFunc(GL_LEQUAL);
		alpha = (0.5 - f*0.5)/(1. - f*0.5);
	} 
	col[3]= alpha;*/
		//glEnable(GL_DEPTH_TEST); 
	for(int i=0;i<dataSize;i++)
	{
		glColor4f(col[0], col[1], col[2], col[3]);

		drawing->Draw_InnerLegend(dataPos[i], dataVel[i], 
					  5,
					      cylinder_radius,m);

		drawing->Draw_Cylinder(dataPos[i], dataVel[i], 
					dataCoe[i], 1.,
					cylinder_radius, cylinder_radius/2.,
					col, false, true,m);
		drawing->Draw_Cylinder(dataPos[i], dataVel[i], 
					dataExp[i], 1.,
					cylinder_radius, cylinder_radius,
					col, true, true,m);
		/*drawing->Draw_Arrow(dataPos[i], dataVel[i], 
					dataCoe[i] * cylinder_radius, cylinder_radius/1.7,
					cylinder_radius,col);*/
	}
  //}
	  

	
	//glEndList();	

	//dataDisplayList++;  


	//dataDisplayList++;
}

void svQDOTDrawing::Double_Cylinder_Single(GLfloat *m, int dataIndex){
        int i = dataIndex;
	svVector4 col;
	col[0] = 1; col[1] = 1; col[2] = 1; col[3] = 0.5;
        svVector3 pos;
        //pos[0] = 0; pos[1]=-2; pos[2]=0;
       // pos[0] = -0.3; pos[1]=-2.025; pos[2]=0;
        pos[0] = -0.025; pos[1]=0; pos[2]=0;
        svVector3 vel;
        vel[0] = 1; vel[1]=0; vel[2] = 0; 
	drawing->Draw_InnerLegend(pos, vel, 
				  5,
			          cylinder_radius,m);
	drawing->Draw_Cylinder(pos, vel, 
					dataCoe[i], 1.,
					cylinder_radius, cylinder_radius/2.,
					col, false, true,m);
	drawing->Draw_Cylinder(pos,vel, 
					dataExp[i], 1.,
					cylinder_radius, cylinder_radius,
					col, true, true,m);
}
void svQDOTDrawing::Size_Size(GLfloat *m)
{
	//glNewList(dataDisplayList, GL_COMPILE);
	svVector4 col;
	col[0] = 1; col[1] = 1; col[2] = 1; col[3] = 0.75;
	/*for(int i=0;i<dataSize;i++)
	{
		drawing->Draw_InnerLegend(dataPos[i], dataVel[i], 
					  5,
					cylinder_radius);
	}*/
  /*for(int t=0;t<5;t++)
  {
  	double alpha = 0.5;
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
		alpha = f * 0.5;
	}
	else if(t == 2)
	{
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);
		glDepthFunc(GL_LEQUAL);
		alpha = (0.5 - f*0.5)/(1. - f*0.5);
	}
	else if(t == 3)
	{
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glDepthFunc(GL_ALWAYS);
		alpha = f * 0.5;
	}
	else if(t == 4)
	{
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glDepthFunc(GL_LEQUAL);
		alpha = (0.5 - f*0.5)/(1. - f*0.5);
	} 
	col[3] = alpha;*/
	for(int i=0;i<dataSize;i++)
	{
		drawing->Draw_InnerLegend(dataPos[i], dataVel[i], 
					  5,
					      cylinder_radius,m);
		drawing->Draw_WidthLegend(dataPos[i], dataVel[i], 
					  dataExp[i],
					cylinder_radius,m);
		drawing->Draw_Cylinder(dataPos[i], dataVel[i], 
					dataCoe[i], dataExp[i],
					cylinder_radius, cylinder_radius,
					col, true, true,m);
	}
  //}
	glDisable(GL_CULL_FACE);
	glDepthFunc(GL_LESS);
	
	//glEndList();	



	//dataDisplayList++;
}

void svQDOTDrawing::Size_Size_Color(GLfloat *m)
{
	//glNewList(dataDisplayList, GL_COMPILE);
	//zsvVector4 col;
	//col[0] = 1; col[1] = 1; col[2] = 1; col[3] = 0.5;
	/*for(int i=0;i<dataSize;i++)
	{
		drawing->Draw_InnerLegend(dataPos[i], dataVel[i], 
					  5,
					cylinder_radius);
	}*/
  /*for(int t=0;t<5;t++)
  {
  	double alpha = 0.5;
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
		alpha = f * 0.5;
	}
	else if(t == 2)
	{
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);
		glDepthFunc(GL_LEQUAL);
		alpha = (0.5 - f*0.5)/(1. - f*0.5);
	}
	else if(t == 3)
	{
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glDepthFunc(GL_ALWAYS);
		alpha = f * 0.5;
	}
	else if(t == 4)
	{
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glDepthFunc(GL_LEQUAL);
		alpha = (0.5 - f*0.5)/(1. - f*0.5);
	} 
	col[3] = alpha;*/
	for(int i=0;i<dataSize;i++)
	{
		svVector4 col;
                int index = dataExp[i];
		col[0] = color[(int)maxExp-2][index][0]/255.;
		col[1] = color[(int)maxExp-2][index][1]/255.;
		col[2] = color[(int)maxExp-2][index][2]/255.;
		col[3] = 0.75;

		drawing->Draw_InnerLegend(dataPos[i], dataVel[i], 
					  5,
					      cylinder_radius,m);
		drawing->Draw_WidthLegend(dataPos[i], dataVel[i], 
					  dataExp[i],
					cylinder_radius,m);
		drawing->Draw_Cylinder(dataPos[i], dataVel[i], 
					dataCoe[i], dataExp[i],
					cylinder_radius, cylinder_radius,
					col, true, true,m);
	}
  //}
	glDisable(GL_CULL_FACE);
	glDepthFunc(GL_LESS);
	
	//glEndList();	



	//dataDisplayList++;
}

void svQDOTDrawing::Hue_Size(GLfloat *m)
{

	//glNewList(dataDisplayList, GL_COMPILE);

	/*for(int i=0;i<dataSize;i++)
	{
		svVector4 col;
		int index = (int)dataExp[i];//cerr<<index<<endl;
		col[0] = color[index][0]/255.;
		col[1] = color[index][1]/255.;
		col[2] = color[index][2]/255.;
		col[3] = 0.5;
	  
		
	}
  for(int t=0;t<5;t++)
  {
  	double alpha = 0.5;
	double f =0.75;
	if(t==0)
	{
		glDisable(GL_CULL_FACE);
		glDepthFunc(GL_LESS);
		alpha = 0.;
		
  	}
	else if(t == 1)
	{
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);
		glDepthFunc(GL_ALWAYS);
		alpha = f * 0.5;
	}
	else if(t == 2)
	{
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);
		glDepthFunc(GL_LESS);
		alpha = (0.5 - f*0.5)/(1. - f*0.5);
	}
	else if(t == 3)
	{
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glDepthFunc(GL_ALWAYS);
		alpha = f * 0.5;
	}
	else if(t == 4)
	{
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glDepthFunc(GL_LEQUAL);
		alpha = (0.5 - f*0.5)/(1. - f*0.5);
	} */
	for(int i=0;i<dataSize;i++)
	{
		svVector4 col;
		int index = (int)dataExp[i];//cerr<<index<<endl;
		col[0] = color[(int)maxExp-2][index][0]/255.;
		col[1] = color[(int)maxExp-2][index][1]/255.;
		col[2] = color[(int)maxExp-2][index][2]/255.;
		col[3] = 0.95;//0.75;
	
		//if(t != 0)
		drawing->Draw_InnerLegend(dataPos[i], dataVel[i], 
					  5,
					      cylinder_radius,m);
		drawing->Draw_Cylinder(dataPos[i], dataVel[i], 
					dataCoe[i], 1,
					cylinder_radius, cylinder_radius,
					col,true,true,m);//, true, true);
		
		
	}
  //}
	glDisable(GL_CULL_FACE);
	glDepthFunc(GL_LESS);
	
	//glEndList();	

	//dataDisplayList++;  

	//dataDisplayList++;
}
void svQDOTDrawing::Hue_Size_Single(GLfloat *m, int dataIndex){
        int i = dataIndex;
	svVector4 col;
	int index = (int)dataExp[i];//cerr<<index<<endl;
	col[0] = color[(int)maxExp-2][index][0]/255.;
	col[1] = color[(int)maxExp-2][index][1]/255.;
	col[2] = color[(int)maxExp-2][index][2]/255.;
	col[3] = 0.95;
        svVector3 pos;
//        pos[0]=2;pos[1]=0;po 
//        pos[0] = -0.3; pos[1]=-2.025; pos[2]=0;
//        pos[0] = -0.75; pos[1]=-2.15; pos[2]=0.;
        pos[0]=-0.025;pos[1]=0;pos[2]=0;
        svVector3 vel;
        vel[0] = 1; vel[1]=0; vel[2] = 0; 
	drawing->Draw_InnerLegend(pos, vel, 
				  5,
			          cylinder_radius,m);
	drawing->Draw_Cylinder(pos, vel, 
				dataCoe[i], 1,
				cylinder_radius, cylinder_radius,
				col,true,true,m);//, true, true);
}
void svQDOTDrawing::Texture_Size(GLfloat *m)
{

	//glNewList(dataDisplayList, GL_COMPILE);
  
	svVector4 col;
	col[0] = 1; col[1] = 1; col[2] = 1; col[3] = 0.95;
	svVector4 col2;
	col2[0] = 1; col2[1] = 1; col2[2] = 1; col2[3] = 0.95;
	//drawing->makeTexture(col2);
	//drawing->inittexture();
	/*for(int i=0;i<dataSize;i++)
	{
		drawing->Draw_InnerLegend(dataPos[i], dataVel[i], 
					  5,
					cylinder_radius);
	}*/
  /*for(int t=0;t<5;t++)
  {
  	double alpha = 0.5;
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
		alpha = f * 0.5;
	}
	else if(t == 2)
	{
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);
		glDepthFunc(GL_LEQUAL);
		alpha = (0.5 - f*0.5)/(1. - f*0.5);
	}
	else if(t == 3)
	{
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glDepthFunc(GL_ALWAYS);
		alpha = f * 0.5;
	}
	else if(t == 4)
	{
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glDepthFunc(GL_LEQUAL);
		alpha = (0.5 - f*0.5)/(1. - f*0.5);
	} 
	col2[3] = alpha;*/
	for(int i=0;i<dataSize;i++)
	{
		//if(dataExp[i]>0)
		svVector4 texture_col;
		texture_col[0] = 0;
		texture_col[1] = 0;
		texture_col[2] = 0;
		texture_col[3] = 0.95;//0.75;
		drawing->Draw_InnerLegend(dataPos[i], dataVel[i], 
					  5,
					      cylinder_radius,m);
		drawing->Draw_Cylinder_withtexture(dataPos[i], dataVel[i], 
					dataCoe[i], dataExp[i],
					cylinder_radius, cylinder_radius,
				   col, texture_col, maxExp, m);
// 10./3.,m);
		/*else
		drawing->Draw_Cylinder(dataPos[i], dataVel[i], 
					dataCoe[i], 1,
					cylinder_radius, cylinder_radius,
					col, true, true);*/
	}
	
  //}
		/*drawing->Draw_Cylinder(dataPos[i], dataVel[i], 
					4, 1,
					cylinder_radius, cylinder_radius,
					col2, true, true);*/

	glDisable(GL_CULL_FACE);
	glDepthFunc(GL_LESS);
	
	//glEndList();	

	//dataDisplayList++;  
}

void svQDOTDrawing::Texture_Size_Single(GLfloat *m, int dataIndex){
        int i = dataIndex;
	svVector4 col;
	col[0] = 1; col[1] = 1; col[2] = 1; col[3] = 0.95;
        svVector3 pos;
        //pos[0] = 0; pos[1]=-2; pos[2]=0;
       // pos[0] = -0.3; pos[1]=-2.025; pos[2]=0;
        pos[0] = -0.025; pos[1]=0; pos[2]=0;
        svVector3 vel;
        vel[0] = 1; vel[1]=0; vel[2] = 0; 
	svVector4 texture_col;
	texture_col[0] = 0;
	texture_col[1] = 0;
	texture_col[2] = 0;
	texture_col[3] = 0.95;
	drawing->Draw_InnerLegend(pos, vel, 
				  5,
				  cylinder_radius,m);
	drawing->Draw_Cylinder_withtexture(pos, vel, 
					dataCoe[i], dataExp[i],
					cylinder_radius, cylinder_radius,
				   col, texture_col, maxExp, m);
}
void svQDOTDrawing::Hue_Texture(GLfloat *m)
{

	//glNewList(dataDisplayList, GL_COMPILE);
	svVector4 col2;
	col2[0] = 1; col2[1] = 1; col2[2] = 1; col2[3] = 0.5;
	/*for(int i=0;i<dataSize;i++)
	{
	  
		svVector4 col;
		int index = (int)dataExp[i];
		col[0] = color[index][0]/255.;
		col[1] = color[index][1]/255.;
		col[2] = color[index][2]/255.;
		col[3] = 0.5;
	  
	drawing->Draw_InnerLegend(dataPos[i], dataVel[i], 
					  5,
					cylinder_radius);
	}
  for(int t=0;t<5;t++)
  {
  	double alpha = 0.5;
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
		alpha = f * 0.5;
	}
	else if(t == 2)
	{
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);
		glDepthFunc(GL_LEQUAL);
		alpha = (0.5 - f*0.5)/(1. - f*0.5);
	}
	else if(t == 3)
	{
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glDepthFunc(GL_ALWAYS);
		alpha = f * 0.5;
	}
	else if(t == 4)
	{
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glDepthFunc(GL_LEQUAL);
		alpha = (0.5 - f*0.5)/(1. - f*0.5);
	} 				
	*/
	for(int i=0;i<dataSize;i++)
	{
	  	svVector4 col;
		int index = (int)dataExp[i];
		col[0] = color[(int)maxExp-2][index][0]/255.;
		col[1] = color[(int)maxExp-2][index][1]/255.;
		col[2] = color[(int)maxExp-2][index][2]/255.;
		col[3] = 0.75;
		svVector4 texture_col;
		texture_col[0] = texture_color[index][0]/255.;
		texture_col[1] = texture_color[index][1]/255.;
		texture_col[2] = texture_color[index][2]/255.;
		texture_col[3] = 0.75;
		drawing->Draw_InnerLegend(dataPos[i], dataVel[i], 
					  5,
					      cylinder_radius,m);
		drawing->Draw_Cylinder_withtexture(dataPos[i], dataVel[i], 
					4, dataCoe[i],
					cylinder_radius, cylinder_radius,
						   col,texture_col,10,m);
	}
  //}
		/*drawing->Draw_Cylinder(dataPos[i], dataVel[i], 
					4, 1,
					cylinder_radius, cylinder_radius,
					col2, true, true);*/

	glDisable(GL_CULL_FACE);
	glDepthFunc(GL_LESS);
	
	//glEndList();	


	//dataDisplayList++;  
}
void svQDOTDrawing::Single_Dual(int exp)
{
      //glDisable(GL_DEPTH_TEST);

	float x,y;
	float size = 7;
	x = 0.;
	y = 0;

	//glDisable(GL_BLEND);
//	glDisable( GL_LINE_SMOOTH );

	glColor3f(0,0,0);//0.4,0.4,0.4);
	glBegin(GL_LINE_LOOP);
	glVertex2f(x,y);
	glVertex2f(x+size*exp,y);
	glVertex2f(x+size*exp,y-size*2);
	glVertex2f(x,y-size*2);
	glEnd();
	for(int i=1;i<exp;i++)
	{
		glBegin(GL_LINES);
		glVertex2f(x+size*i, y);//x,y-size*(i));
		glVertex2f(x+size*i, y-size*2);//+2*size,y-size*(i));
		glEnd();
	}	
	glEnable( GL_LINE_SMOOTH );
	glHint( GL_LINE_SMOOTH_HINT, GL_NICEST );
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(1.,1.,1.,0.5);
	glBegin(GL_QUADS);
	glVertex2f(x,y);
	glVertex2f(x+size*exp,y);
	glVertex2f(x+size*exp,y-size*2);
	glVertex2f(x,y-size*2);
	glEnd();

/*	glBegin(GL_LINE_LOOP);
	glVertex2f(x,y);
	glVertex2f(x+size*2,y);
	glVertex2f(x+size*2,y-size*exp);
	glVertex2f(x,y-size*exp);
	glEnd();
	for(int i=1;i<exp;i++)
	{
		glBegin(GL_LINES);
		glVertex2f(x,y-size*(i));
		glVertex2f(x+2*size,y-size*(i));
		glEnd();
	}	
	glEnable( GL_LINE_SMOOTH );
	glHint( GL_LINE_SMOOTH_HINT, GL_NICEST );
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glColor4f(1.,1.,1.,0.5);
	glBegin(GL_QUADS);
	glVertex2f(x,y);
	glVertex2f(x+size*2,y);
	glVertex2f(x+size*2,y-size*exp);
	glVertex2f(x,y-size*exp);
	glEnd();
*/
   //glEnable(GL_DEPTH_TEST);
}
void svQDOTDrawing::Single_Dual(int exp, int coe)
{
      //glDisable(GL_DEPTH_TEST);

	float x,y;
	float size = 7;

	x = 0.;
	y = 0;

	//glDisable(GL_BLEND);
//	glDisable( GL_LINE_SMOOTH );

	glColor3f(0.,0.,0.);
	glBegin(GL_LINE_LOOP);
	glVertex2f(x+0.5*size,y);
	glVertex2f(x+1.5*size,y);
	glVertex2f(x+1.5*size,y-size*coe);
	glVertex2f(x+0.5*size,y-size*coe);
	glEnd();

	glBegin(GL_LINE_LOOP);
	glVertex2f(x,y);
	glVertex2f(x+size*2,y);
	glVertex2f(x+size*2,y-size*exp);
	glVertex2f(x,y-size*exp);
	glEnd();

	glColor3f(0,0,0);//0.4,0.4,0.4);
//	glLineWidth(0.1);

	glBegin(GL_LINES);
	glVertex2f(x+size,y-size*5);
	glVertex2f(x+size,y);
	glEnd();
        svVector3 pos;
        pos[0] = x+2*size/2;pos[1]=y;pos[2]=0;
        svVector3 vel;
        vel[0] =0;vel[1]=-1;vel[2]=0;
        svScalar length = size;
        drawing->Draw_2DFishBone(pos, vel, length);

	glEnable( GL_LINE_SMOOTH );
	glHint( GL_LINE_SMOOTH_HINT, GL_NICEST );
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glColor4f(1.,1.,1.,0.5);
	glBegin(GL_QUADS);
	glVertex2f(x+0.5*size,y);
	glVertex2f(x+1.5*size,y);
	glVertex2f(x+1.5*size,y-size*coe);
	glVertex2f(x+0.5*size,y-size*coe);
	glEnd();	

	glColor4f(1.,1.,1.,0.5);
	glBegin(GL_QUADS);
	glVertex2f(x,y);
	glVertex2f(x+size*2,y);
	glVertex2f(x+size*2,y-size*exp);
	glVertex2f(x,y-size*exp);
	glEnd();

	glColor4f(0,0,0,1);
	for(int i=1;i<coe;i++)
	{
		glBegin(GL_LINES);
		glVertex2f(x+0.5*size,y-size*(i));
		glVertex2f(x+1.5*size,y-size*(i));
		glEnd();
	}

	for(int i=1;i<exp;i++)
	{
		glBegin(GL_LINES);
		glVertex2f(x,y-size*(i));
		glVertex2f(x+2*size,y-size*(i));
		glEnd();
	}	

   //glEnable(GL_DEPTH_TEST);
}
void svQDOTDrawing::RenderDistributeLegend()
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
  float y = viewport[3]/12;//viewport[3]-200.;
  float size = 7;//viewport[3]/50;	

	x = viewport[2]*4.5/5.;
	y = y + size*3;

	glColor3f(0.,0.,0.);
	glBegin(GL_LINE_LOOP);
	glVertex2f(x,y);
	glVertex2f(x+size*1,y);
	glVertex2f(x+size*1,y+size*2);
	glVertex2f(x,y+size*2);
	glEnd();
	glColor3f(1.,1.,1.);
	glBegin(GL_LINES);
	glVertex2f(x,y-size);
	glVertex2f(x+size*1,y-size);
	glEnd();
	glBegin(GL_LINES);
	glVertex2f(x,y-size-size*0.2);
	glVertex2f(x,y-size+size*0.2);
	glEnd();
	glBegin(GL_LINES);
	glVertex2f(x,y-size);
	glVertex2f(x+size*0.2,y-size-size*0.2);
	glEnd();
	glBegin(GL_LINES);
	glVertex2f(x,y-size);
	glVertex2f(x+size*0.2,y-size+size*0.2);
	glEnd();
	glBegin(GL_LINES);
	glVertex2f(x+size*1,y-size-size*0.2);
	glVertex2f(x+size*1,y-size+size*0.2);
	glEnd();
	glBegin(GL_LINES);
	glVertex2f(x+size*1,y-size);
	glVertex2f(x+size*1-size*0.2,y-size-size*0.2);
	glEnd();
	glBegin(GL_LINES);
	glVertex2f(x+size*1,y-size);
	glVertex2f(x+size*1-size*0.2,y-size+size*0.2);
	glEnd();
	glRasterPos2f(x+size*1.2,y-size*0.6);
	sprintf(str,"exponent");
	for(int ii=0;ii<strlen(str);ii++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[ii]);

	glColor3f(0.,0.,0.);
	glBegin(GL_LINE_LOOP);
	glVertex2f(x,y+0.5*size);
	glVertex2f(x+size*3,y+0.5*size);
	glVertex2f(x+size*3,y+1.5*size);
	glVertex2f(x,y+1.5*size);
	glEnd();
	glColor3f(1.,1.,1.);
	glBegin(GL_LINES);
	glVertex2f(x,y+4*size-size);
	glVertex2f(x+size*3,y+4*size-size);
	glEnd();
	glBegin(GL_LINES);
	glVertex2f(x,y+4*size-size-size*0.2);
	glVertex2f(x,y+4*size-size+size*2);
	glEnd();
	glBegin(GL_LINES);
	glVertex2f(x,y+4*size-size);
	glVertex2f(x+size*0.2,y+4*size-size-size*0.2);
	glEnd();
	glBegin(GL_LINES);
	glVertex2f(x,y+4*size-size);
	glVertex2f(x+size*0.2,y+4*size-size+size*0.2);
	glEnd();
	glBegin(GL_LINES);
	glVertex2f(x+size*3,y+4*size-size-size*0.2);
	glVertex2f(x+size*3,y+4*size-size+size*0.2);
	glEnd();
	glBegin(GL_LINES);
	glVertex2f(x+size*3,y+4*size-size);
	glVertex2f(x+size*3-size*0.2,y+4*size-size-size*0.2);
	glEnd();
	glBegin(GL_LINES);
	glVertex2f(x+size*3,y+4*size-size);
	glVertex2f(x+size*3-size*0.2,y+4*size-size+size*0.2);
	glEnd();
	glRasterPos2f(x+size*0.8,y+3.7*size);
	sprintf(str,"digit");
	for(int ii=0;ii<strlen(str);ii++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[ii]);

	glColor3f(0.,0.,0.);
	glLineWidth(0.2);

	glBegin(GL_LINES);
	glVertex2f(x,y+size);
	glVertex2f(x+size*5,y+size);
	glEnd();
	glBegin(GL_LINES);
	glVertex2f(x+size*2,y+0.5*size);
	glVertex2f(x+size*2,y+1.5*size);
	glEnd();	

	glColor3f(1.,1.,1.);
	glLineWidth(1.);
	glBegin(GL_LINES);
	glVertex2f(x+size*5,y+4*size-size-size*0.2);
	glVertex2f(x+size*5,y+4*size-size+size*2);
	glEnd();
	glBegin(GL_LINES);
	glVertex2f(x,y+4*size-size+size*2-size*0.5);
	glVertex2f(x+size*5,y+4*size-size+size*2-size*0.5);
	glEnd();
	glBegin(GL_LINES);
	glVertex2f(x,y+4*size-size+size*2-size*0.5);
	glVertex2f(x+size*0.2,y+4*size-size+size*2-size*0.5-size*0.2);
	glEnd();
	glBegin(GL_LINES);
	glVertex2f(x,y+4*size-size+size*2-size*0.5);
	glVertex2f(x+size*0.2,y+4*size-size+size*2-size*0.5+size*0.2);
	glEnd();
	glBegin(GL_LINES);
	glVertex2f(x+size*5,y+4*size-size+size*2-size*0.5);
	glVertex2f(x+size*5-size*0.2,y+4*size-size+size*2-size*0.5-size*0.2);
	glEnd();
	glBegin(GL_LINES);
	glVertex2f(x+size*5,y+4*size-size+size*2-size*0.5);
	glVertex2f(x+size*5-size*0.2,y+4*size-size+size*2-size*0.5+size*0.2);
	glEnd();
	glRasterPos2f(x+size*2.45,y+4*size-size+size*2+size*0.25);
	sprintf(str,"5");
	for(int ii=0;ii<strlen(str);ii++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[ii]);

	glLineWidth(0.2);

    glMatrixMode(GL_PROJECTION);
    glLoadMatrixd(pmat);
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixd(vmat);
}
void svQDOTDrawing::RenderMarkers(vector<int> index, GLfloat *m, int ENCODE_MODE, bool answer)
{
  //cerr<<index[0]<<endl;
    GLdouble vmat[16], pmat[16], sx[3], sy[3], sz[3];
    GLint    viewport[4];
    
    glEnable(GL_BLEND);
    //glDisable(GL_LIGHTING);

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
	//cerr<<index.size()<<" !"<<endl;
	for(int k=0;k<index.size();k++)
	{
		for(int j =0;j<2;j++)
		{
			p[j][0]=0.;p[j][1]=0.;p[j][2]=0.;	
		}
		tmp1[0]=0.;tmp1[1]=0.;tmp1[2]=0.;
		
		i = index[k];

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
		
		if(ENCODE_MODE == 0)
		{
		  double exp = cylinder_radius * dataExp[i];
		  double coe = cylinder_radius * dataCoe[i];
		  if(exp > coe)
		  {
		    scale = exp;
		  }
		  else
		  {
		    scale = coe;
		  }
		}
		else if(ENCODE_MODE == 1 || ENCODE_MODE == 4)
		{
		  scale = cylinder_radius * dataCoe[i];
		}
		else if(ENCODE_MODE == 2)
		{
		  scale = cylinder_radius * dataCoe[i];
		}
		else if(ENCODE_MODE == 3)
		{
		  scale = cylinder_radius * dataCoe[i];
		}

		
		end = dataPos[i] + scale * dataVel[i];


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

		
                if(!answer)
		  glColor4f(1.,0.,0.,0.75);	
                else		
		  glColor4f(1.,1.,0.,0.75);	
		if(sx[0]>=0&&sx[0]<=image_width
		&&sy[0]>=0&&sy[0]<=image_height&&p[0][2]<=eye[2])
		{	
			tmp2 = dataPos[i] +  (svScalar)0.2*dataVel[i];
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
				tmp2 = end- (svScalar)0.2*dataVel[i];
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


    glMatrixMode(GL_PROJECTION);
    glLoadMatrixd(pmat);
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixd(vmat);  
}
void svQDOTDrawing::RenderSelectionTest(int index){
   if(index <0)return;
   
   glPushMatrix();
   glTranslatef(dataPos[index][0], dataPos[index][1], dataPos[index][2]);
   glutSolidSphere(0.1,5,5);
   glPopMatrix();
}

void svQDOTDrawing::RenderAnswerMarker(int index, GLfloat *m, int ENCODE_MODE, bool answer)
{
  //cerr<<index[0]<<endl;
    GLdouble vmat[16], pmat[16], sx[3], sy[3], sz[3];
    GLint    viewport[4];
    
    glEnable(GL_BLEND);
    //glDisable(GL_LIGHTING);

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
        //cout<<index<<endl;
	//cerr<<index.size()<<" !"<<endl;
	for(int k=0;k<1;k++)
	{
		for(int j =0;j<2;j++)
		{
			p[j][0]=0.;p[j][1]=0.;p[j][2]=0.;	
		}
		tmp1[0]=0.;tmp1[1]=0.;tmp1[2]=0.;
		
		i = index;
                if(index < 0) break;

		p[0][0]  = dataPos[i][0] * m[0] +
				-dataPos[i][2] * m[4] +
				dataPos[i][1] * m[8] +
				m[12] + p[0][0];
		p[0][1]  = dataPos[i][0] * m[1] +
				  -dataPos[i][2] * m[5] +
				  dataPos[i][1] * m[9] +
				  m[13] + p[0][1];
		p[0][2]  = dataPos[i][0] * m[2] +
				   -dataPos[i][2] * m[6] +
				   dataPos[i][1] * m[10] +
				   m[14] + p[0][2];
		gluProject((double)(p[0][0]), 
				(double)(p[0][1]), 
				(double)(p[0][2]), 
				vmat, pmat, viewport, &sx[0],&sy[0],&sz[0]);
		sy[0] = (float)viewport[3]-(float)sy[0];
		
		if(ENCODE_MODE == 3)//0)
		{
		  double exp = cylinder_radius * dataExp[i];
		  double coe = cylinder_radius * dataCoe[i];
		  if(exp > coe)
		  {
		    scale = exp;
		  }
		  else
		  {
		    scale = coe;
		  }
		}
		else //if(ENCODE_MODE == 1 || ENCODE_MODE == 4)
		{ 
		  scale = cylinder_radius * dataCoe[i];
		}/*
		else if(ENCODE_MODE == 2)
		{
		  scale = cylinder_radius * dataCoe[i];
		}
		else if(ENCODE_MODE == 3)
		{
		  scale = cylinder_radius * dataCoe[i];
		}*/

		
		end = dataPos[i] + scale * dataVel[i];


		p[1][0]  = end[0] * m[0] +
				-end[2] * m[4] +
				end[1] * m[8] +
				m[12] + p[1][0];
		p[1][1]  = end[0] * m[1] +
				  -end[2] * m[5] +
				  end[1] * m[9] +
				  m[13] + p[1][1];
		p[1][2]  = end[0] * m[2] +
				   -end[2] * m[6] +
				   end[1] * m[10] +
				   m[14] + p[1][2];

		gluProject((double)(p[1][0]), 
				(double)(p[1][1]), 
				(double)(p[1][2]), 
				vmat, pmat, viewport, &sx[1],&sy[1],&sz[1]);
		sy[1] = (float)viewport[3]-(float)sy[1];

		
		if(answer)//k==1)
		{
		  glColor4f(0.,0.,1.,0.75);	
		}
		else
		{
		  glColor4f(1.,0.,0.,0.75);	
		}
		if(sx[0]>=0&&sx[0]<=image_width
		&&sy[0]>=0&&sy[0]<=image_height&&p[0][2]<=eye[2])
		{	
			tmp2 = dataPos[i] +  (svScalar)0.2*dataVel[i];
			tmp1[0]  = tmp2[0] * m[0] +
					-tmp2[2] * m[4] +
					tmp2[1] * m[8] +
				  m[12] + tmp1[0];
			tmp1[1]  = tmp2[0] * m[1] +
					  -tmp2[2] * m[5] +
					  tmp2[1] * m[9] +
					  m[13] + tmp1[1];
			tmp1[2]  = tmp2[0] * m[2] +
					   -tmp2[2] * m[6] +
					   tmp2[1] * m[10] +
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
		
                        if(answer){
                            dir2=-dir2;
                        }
		
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
				tmp2 = end- (svScalar)0.2*dataVel[i];
				tmp1[0]  = tmp2[0] * m[0] +
					-tmp2[2] * m[4] +
					tmp2[1] * m[8] +
					m[12] + tmp1[0];
				tmp1[1]  = tmp2[0] * m[1] +
					  -tmp2[2] * m[5] +
					  tmp2[1] * m[9] +
					  m[13] + tmp1[1];
				tmp1[2]  = tmp2[0] * m[2] +
					   -tmp2[2] * m[6] +
					   tmp2[1] * m[10] +
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
                        if(answer){
                            dir2=-dir2;
                        }
				glBegin(GL_TRIANGLES);
				glVertex2f(sx[1],sy[1]);
				glVertex2f(sx[1]+dir2[0]*17-10.*dir[0],sy[1]+dir2[1]*17-10.*dir[1]);
				glVertex2f(sx[1]+dir2[0]*17+10.*dir[0],sy[1]+dir2[1]*17+10.*dir[1]);		
				glEnd();
		}
	}


    glMatrixMode(GL_PROJECTION);
    glLoadMatrixd(pmat);
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixd(vmat);  
}
void svQDOTDrawing::RenderCorrectMarker(vector<int> index, GLfloat *m, int ENCODE_MODE)
{
  //cerr<<index[0]<<endl;
    GLdouble vmat[16], pmat[16], sx[3], sy[3], sz[3];
    GLint    viewport[4];
    
    glEnable(GL_BLEND);
    //glDisable(GL_LIGHTING);

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
	//cerr<<index.size()<<" !"<<endl;
	for(int k=0;k<1;k++)
	{
		for(int j =0;j<2;j++)
		{
			p[j][0]=0.;p[j][1]=0.;p[j][2]=0.;	
		}
		tmp1[0]=0.;tmp1[1]=0.;tmp1[2]=0.;
		
		i = index[0];

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
		
		if(ENCODE_MODE == 0)
		{
		  double exp = cylinder_radius * dataExp[i];
		  double coe = cylinder_radius * dataCoe[i];
		  if(exp > coe)
		  {
		    scale = exp;
		  }
		  else
		  {
		    scale = coe;
		  }
		}
		else if(ENCODE_MODE == 1 || ENCODE_MODE == 4)
		{
		  scale = cylinder_radius * dataCoe[i];
		}
		else if(ENCODE_MODE == 2)
		{
		  scale = cylinder_radius * dataCoe[i];
		}
		else if(ENCODE_MODE == 3)
		{
		  scale = cylinder_radius * dataCoe[i];
		}

		
		end = dataPos[i] + scale * dataVel[i];


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

		
		if(k==1)
		{
		  glColor4f(1.,1.,0.,0.75);	
		}
		else
		{
		  glColor4f(1.,1.,0.,0.75);	
		}
		if(sx[0]>=0&&sx[0]<=image_width
		&&sy[0]>=0&&sy[0]<=image_height&&p[0][2]<=eye[2])
		{	
			tmp2 = dataPos[i] +  (svScalar)0.2*dataVel[i];
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
				tmp2 = end- (svScalar)0.2*dataVel[i];
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


    glMatrixMode(GL_PROJECTION);
    glLoadMatrixd(pmat);
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixd(vmat);  
}

//Single Hue Legend
/*void svQDOTDrawing::Spiral_Legend()
{
	CIEColor2 *spiral = new CIEColor2();

	glNewList(dataDisplayList, GL_COMPILE);

	
	svVector3 pos;
	svVector3 col;
	svScalar size = 1;
	pos[0] = 0;
	pos[1] = 0;
	pos[2] = 0;


	for(int i=0;i<exponent_num;i++)
	{
		double t = 0.5;
		double h = (i/6.) * 360;

		vector<double> rgb = spiral -> SinglehueSequential(t, 1., 0., 1., h);

		glColor3f(rgb[0], rgb[1], rgb[2]);

		glBegin(GL_QUADS);			
		glVertex2f(pos[0], pos[1]);
		glVertex2f(pos[0] + size, pos[1]);
		pos[1] = pos[1] + size;
		glVertex2f(pos[0] + size, pos[1]);
		glVertex2f(pos[0], pos[1]);
		glEnd();

	}


	glEndList();	

	delete spiral;
	spiral = NULL;	

	dataDisplayList++;
}


void svQDOTDrawing::Spiral_Legend_2D()
{
	CIEColor2 *spiral = new CIEColor2();

	glNewList(dataDisplayList, GL_COMPILE);

	
	svVector3 pos;
	svVector3 col;
	svScalar size = 1;
	pos[0] = 0;
	pos[1] = 0;
	pos[2] = 0;


	for(int i=0;i<exponent_num;i++)
	{
		double h = i * 60;

		glBegin(GL_QUAD_STRIP);
		for(int j=0;j<=100;j++)
		{
			double t = 0.1 + j * 0.007;
			vector<double> rgb = spiral -> SinglehueSequential(t, 0.5, 0., 1., h);
			
			glColor3f(rgb[0], rgb[1], rgb[2]);
			glVertex2f(pos[0], pos[1]);
			glColor3f(rgb[0], rgb[1], rgb[2]);
			glVertex2f(pos[0] + size, pos[1]);

			pos[1] = pos[1] + size;
		}
		glEnd();

		pos[1] = 0;
		pos[0] = pos[0] + size;
	}


	glEndList();	

	delete spiral;
	spiral = NULL;	

	dataDisplayList++;
}

void svQDOTDrawing::Red_Green_Legend()
{
	glNewList(dataDisplayList, GL_COMPILE);
	
	svVector3 pos;
	svVector3 col;
	svScalar size = 1;
	pos[0] = 0;
	pos[1] = 0;
	pos[2] = 0;


	for(int i=0;i<exponent_num;i++)
	{
		double h = i * 90;
	glBegin(GL_QUAD_STRIP);
		for(int j=0;j<=100;j++)
		{
			svVector3 rgb;

			rgb[0] = i/4.;
			rgb[1] = j/100.;
			rgb[2] = 0;
			
			glColor3f(rgb[0], rgb[1], rgb[2]);
			glVertex2f(pos[0], pos[1]);
			glColor3f(rgb[0], rgb[1], rgb[2]);
			glVertex2f(pos[0] + size, pos[1]);

			pos[1] = pos[1] + size;
		}
	glEnd();
		pos[1] = 0;
		pos[0] = pos[0] + size;
	}


	glEndList();
	
	dataDisplayList++;	
}*/

void svQDOTDrawing::Single_ColorSize(int exp, int coe)
{
	float x =0;
	float y =0;
	float size = 7;

	//exp = exp * 3;

	glDisable(GL_BLEND);
	glDisable( GL_LINE_SMOOTH );

	glColor3f(0.3,0.3,0.3);
//	glLineWidth(0.1);

//	glLineWidth(0.2);
	glBegin(GL_LINES);
	glVertex2f(x+2*size/2,y);
	glVertex2f(x+2*size/2,y-size*5);
	glEnd();
  
        svVector3 pos;
        pos[0] = x+2*size/2;pos[1]=y;pos[2]=0;
        svVector3 vel;
        vel[0] =0;vel[1]=-1;vel[2]=0;
        svScalar length = size;
        drawing->Draw_2DFishBone(pos, vel, length);
	/*glBegin(GL_LINE_LOOP);
	glVertex2f(x,y);
	glVertex2f(x+2*size,y);
	glVertex2f(x+size*2,y-size*coe);
	glVertex2f(x,y-size*coe);
	glEnd();*/


	glColor3f(0.3,0.3,0.3);
	glBegin(GL_LINE_LOOP);
	glVertex2f(x,y);
	glVertex2f(x+2*size,y);
	glVertex2f(x+size*2,y-size*coe);
	glVertex2f(x,y-size*coe);
	glEnd();

	glEnable( GL_LINE_SMOOTH );
	glHint( GL_LINE_SMOOTH_HINT, GL_NICEST );
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glColor4f(color[(int)maxExp-2][exp][0]/255.,color[(int)maxExp-2][exp][1]/255.,color[(int)maxExp-2][exp][2]/255, 0.95);
	glBegin(GL_QUADS);
	glVertex2f(x,y);
	glVertex2f(x+2*size,y);
	glVertex2f(x+size*2,y-size*coe);
	glVertex2f(x,y-size*coe);
	glEnd();
	glColor3f(0,0,0);
	for(int i=1;i<coe;i++)
	{
		glBegin(GL_LINES);
		glVertex2f(x+size*2,y-size*i);
		glVertex2f(x,y-size*i);
		glEnd();
	}	

}
void svQDOTDrawing::Single_ColorSize(int exp)
{
	float x =0;
	float y =0;
	float size = 7;

	glDisable(GL_BLEND);
	glDisable( GL_LINE_SMOOTH );

        int coe = 2;

	glColor3f(0.3,0.3,0.3);
	glBegin(GL_LINE_LOOP);
	glVertex2f(x,y);
	glVertex2f(x+2*size,y);
	glVertex2f(x+size*2,y-size*coe);
	glVertex2f(x,y-size*coe);
	glEnd();

	glEnable( GL_LINE_SMOOTH );
	glHint( GL_LINE_SMOOTH_HINT, GL_NICEST );
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glColor4f(color[(int)maxExp-2][exp][0]/255.,color[(int)maxExp-2][exp][1]/255.,color[(int)maxExp-2][exp][2]/255, 0.95);
	glBegin(GL_QUADS);
	glVertex2f(x,y);
	glVertex2f(x+2*size,y);
	glVertex2f(x+size*2,y-size*coe);
	glVertex2f(x,y-size*coe);
	glEnd();
}

void svQDOTDrawing::Axis_Legend()
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
	
    float x, y;
    float width = (7 * 10 + 5)*4 + 50;
    float height = (7*2 + 5)*10+50;

 	x = viewport[2]-300-60;
	y = 300+50;     

	glColor3f(1,1,1);

	glBegin(GL_LINES);
	glVertex2f(x + width,y);
	glVertex2f(x,y);
	glEnd();
	glBegin(GL_LINES);
	glVertex2f(x, y- height);
	glVertex2f(x,y);
	glEnd();

	glBegin(GL_LINES);
	glVertex2f(x + width,y);
	glVertex2f(x + width - 10,y-10/sqrt(3));
	glEnd();
	glBegin(GL_LINES);
	glVertex2f(x + width,y);
	glVertex2f(x + width - 10,y+10/sqrt(3));
	glEnd();

	glBegin(GL_LINES);
	glVertex2f(x, y- height);
	glVertex2f(x-10/sqrt(3),y-height+10);
	glEnd();
	glBegin(GL_LINES);
	glVertex2f(x, y- height);
	glVertex2f(x+10/sqrt(3),y-height+10);
	glEnd();

	char str[20];
	sprintf(str, "Exponent");
	glRasterPos2f(x + width-7*10,y+20);
	for(int i=0;i<strlen(str);i++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[i]);	
	sprintf(str, "Digit");
	glRasterPos2f(x,y-height-10);
	for(int i=0;i<strlen(str);i++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[i]);	


	for(int i=1;i<11;i++)
	{
		sprintf(str, "%d", i);
		glRasterPos2f(x+3,y - 19.1 * (i-1)-50+12);
		for(int i=0;i<strlen(str);i++)
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[i]);	
	}

	for(int i=0;i<4;i++)
	{
		sprintf(str, "%d", i);
		glRasterPos2f(x+75*i+60,y-5);
		for(int i=0;i<strlen(str);i++)
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[i]);	
	}

    glMatrixMode(GL_PROJECTION);
    glLoadMatrixd(pmat);
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixd(vmat);
}

void svQDOTDrawing::Large_Axis_Legend(int X)
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
	
    float x, y;
    float width = 49*(maxExp+1)+20;//(7*7 + 12 )*(maxExp+1);//60 + 7 * 7 * (maxExp+1);//4;
    float height = 5 * 7 *6 + 7*7 + 11*7 + 8*7 + 9*7 + 10*7 + 50;

    float origin =viewport[2]-49* (maxExp+1)-25; 
    float originy = 550+35;//550 + 30;

 	x = origin;//viewport[2]-280;
	y = originy;//640+30;     

	glColor3f(1,1,1);

	glBegin(GL_LINES);
	glVertex2f(x + width,y);
	glVertex2f(x,y);
	glEnd();
	glBegin(GL_LINES);
	glVertex2f(x, y- height);
	glVertex2f(x,y);
	glEnd();

	glBegin(GL_LINES);
	glVertex2f(x + width,y);
	glVertex2f(x + width - 10,y-10/sqrt(3));
	glEnd();
	glBegin(GL_LINES);
	glVertex2f(x + width,y);
	glVertex2f(x + width - 10,y+10/sqrt(3));
	glEnd();

	glBegin(GL_LINES);
	glVertex2f(x, y- height);
	glVertex2f(x-10/sqrt(3),y-height+10);
	glEnd();
	glBegin(GL_LINES);
	glVertex2f(x, y- height);
	glVertex2f(x+10/sqrt(3),y-height+10);
	glEnd();

	char str[20];
	sprintf(str, "Power");
	glRasterPos2f(viewport[2]-50,y+30);//x + width - 35,y+20);//-(maxExp+2)*10,y+20);
	for(int i=0;i<strlen(str);i++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[i]);	
	sprintf(str, "Digit");
	glRasterPos2f(x-35,y-height+10);//-10);
	for(int i=0;i<strlen(str);i++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[i]);	
 
        double vy = y + 30.;
	for(int i=1;i<11;i++)
	{
		if(i<10)
		sprintf(str, "%d", i);
		else
		sprintf(str, "9.99");
		if(i<=5)
			glRasterPos2f(x-10,vy - 7*7*i);
		else if(i<10)
			glRasterPos2f(x-10,vy - 7.25*7*5 - (7*7 + i*7)*(i-5)/2);
                else if(i==10)
			glRasterPos2f(x-25,vy - 7.25*7*5 - (7*7 + i*7)*(i-5)/2);
 
		for(int j=0;j<strlen(str);j++)
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[j]);	
	}

	for(int i=0;i<maxExp+1;i++)
	{
              if(X == i)
                glColor3f(0.5,0.,0.);
              else
                glColor3f(1,1,1);
		sprintf(str, "%d", i);
		glRasterPos2f(x+25 + i * 7*7 + 3,y+15);//9*7
		for(int i=0;i<strlen(str);i++)
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[i]);	
                /*if(X >= 0 && X < maxExp + 1 && X==i){
                   glColor3f(0.5,0.,0.);
                   glBegin(GL_TRIANGLES);
                   glVertex2f(x+30 + i * 8*7 + 3,y+11);
                   glVertex2f(x+30 + i * 8*7 + 3-5,y+20);
                   glVertex2f(x+30 + i * 8*7 + 3+5,y+20);
                   glEnd();
                }*/
	}

    glMatrixMode(GL_PROJECTION);
    glLoadMatrixd(pmat);
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixd(vmat);
}

void svQDOTDrawing::Font_Legend()
{
	glColor3f(1,1,1);

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
	
    float x, y;
    float width = 7 * 10 + 5;
    float height = 7*2 + 8;

 	x = viewport[2]-300-5;
	y = 300+24;     

	char str[20];

	for(int i=1;i<11;i++)
	{
		if(i==1 ||i == 10)
		{
			for(int j=0;j<4;j++)
			{
				sprintf(str, "%de+%d", i, j);
				glRasterPos2f(x,y);
				for(int i=0;i<strlen(str);i++)
					glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, str[i]);	

				x = x + width;
			}
		}
		x = viewport[2]-300-10;
		y = y - height;
	}

    glMatrixMode(GL_PROJECTION);
    glLoadMatrixd(pmat);
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixd(vmat);
}


void svQDOTDrawing::Large_Font_Legend()
{

	glColor3f(1,1,1);

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
	
    float x, y;
    float width = 7*7;
    float height = 9*7;//8.5*7 ;
    float origin =viewport[2]- width * (maxExp+1); 
    float originy = 550;//550 + 30;

 	x = origin-5;//viewport[2]-252;
	y = originy+30;//27.5;//650;     

	char str[20]; char powstr[10];

	for(int i=1;i<11;i++)
	{
		if(i==1 ||i == 10)
		{
		for(int j=0;j<maxExp+1;j++)
		{
			if(i==1){
                          sprintf(str, "%dx10",i);//%d", i, j);	
                          sprintf(powstr, "%d",j);//%d", i, j);	
			  glRasterPos2f(x+25,y-2);
			  for(int i=0;i<strlen(powstr);i++)
				glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, powstr[i]);	
			  glRasterPos2f(x,y);
			  for(int i=0;i<strlen(str);i++)
				glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, str[i]);	
			   x = x + width;
                        }
			else{
		       	  sprintf(str, "9.99x10");//%d", j);	
                          sprintf(powstr, "%d",j);//%d", i, j);	
			  glRasterPos2f(x+29,y-2);
			  for(int i=0;i<strlen(powstr);i++)
				glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, powstr[i]);	
			  glRasterPos2f(x-10,y);
 			  for(int i=0;i<strlen(str);i++)
				glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, str[i]);	
			   x = x + width;
                        }
		}
		}
		x = origin-2;//viewport[2]-257;
		y = y - height;
	}

    glMatrixMode(GL_PROJECTION);
    glLoadMatrixd(pmat);
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixd(vmat);
}

void svQDOTDrawing::ColorSize_Legend()
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
	
    float x, y;
    float width = 7*7;
    float height = 6;
    float originx =viewport[2]- width * (maxExp+1); 
    float originy = 570;

 	x = originx;//viewport[2]-60. * (maxExp+1);//250;
	y = originy;//640;   

	for(int i=1;i<11;i++)
	{
		for(int j=0;j<maxExp+1;j++)
		{
			glPushMatrix();

			glTranslatef(x, y, 0);
			Single_ColorSize(j, i);

			glPopMatrix();

			x = x + width;
		}
		x = originx;//viewport[2]-250;
		if(i<=5)
		y = y - 7*(height) - 7;
		else
		y = y - 7*(i+1);
	}

    glMatrixMode(GL_PROJECTION);
    glLoadMatrixd(pmat);
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixd(vmat);
}

void svQDOTDrawing::ColorTexture_Legend()
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
	
    float x, y;
    float width = 8*7;
    float height = 5;

 	x = viewport[2]-250;
	y = 640;   
  

	for(int i=1;i<11;i++)
	{
		for(int j=0;j<4;j++)
		{
			glPushMatrix();

			glTranslatef(x, y, 0);
			Single_ColorTexture(j, i);

			glPopMatrix();

			x = x + width;
		}
		x = viewport[2]-250;
		if(i<=5)
		y = y - 7*(5) - 7;
		else
		y = y - 7*(i+1);
	}

    glMatrixMode(GL_PROJECTION);
    glLoadMatrixd(pmat);
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixd(vmat);
}

void svQDOTDrawing::TextureSize_Legend()
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
	
    float x, y;
    float width = 7*7;
    float height = 6;
     
    float origin =viewport[2]-width * (maxExp+1); 
    float originy = 570;//+30;//550 + 30;

 	x = origin;//viewport[2]-250;
	y = originy;//640;   

	for(int i=1;i<11;i++)
	{
		for(int j=0;j<maxExp+1;j++)
		{
			glPushMatrix();

			glTranslatef(x, y, 0);
			Single_TextureSize(j, i);

			glPopMatrix();

			x = x + width;
		}
		x = origin;//viewport[2]-250;
		if(i<=5)
		y = y - 7*(height) - 7;
		else
		y = y - 7*(i+1);
	}

    glMatrixMode(GL_PROJECTION);
    glLoadMatrixd(pmat);
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixd(vmat);
}

void svQDOTDrawing::SizeSize_Legend()
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
	
    float x, y;
    float width = 8*7;
    float height = 6;

 	x = viewport[2]-250;
	y = 640;   

	for(int i=1;i<11;i++)
	{
		for(int j=0;j<4;j++)
		{
			glPushMatrix();

			glTranslatef(x, y, 0);
			Single_SizeSize(j, i);

			glPopMatrix();

			x = x + width;
		}
		x = viewport[2]-250;
		if(i<=5)
		y = y - 7*(5) - 7;
		else
		y = y - 7*(i+1);
	}

    glMatrixMode(GL_PROJECTION);
    glLoadMatrixd(pmat);
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixd(vmat);
}

void svQDOTDrawing::SizeSizeColor_Legend()
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
	
    float x, y;
    float width = 8*7;
    float height = 5;

 	x = viewport[2]-250;
	y = 640;   

	for(int i=1;i<11;i++)
	{
		for(int j=0;j<4;j++)
		{
			glPushMatrix();

			glTranslatef(x, y, 0);
			Single_SizeSizeColor(j, i);

			glPopMatrix();

			x = x + width;
		}
		x = viewport[2]-250;
		if(i<=5)
		y = y - 7*(5) - 7;
		else
		y = y - 7*(i+1);
	}

    glMatrixMode(GL_PROJECTION);
    glLoadMatrixd(pmat);
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixd(vmat);
}

void svQDOTDrawing::Dual_Legend()
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
	
    float x, y;
    float width = 7*7;
    float height = 6;

    float origin =viewport[2]-width * (maxExp+1); 
    float originy = 570;//+30;//550 + 30;

 	x = origin;//viewport[2]-250;
	y = originy;//640;   
   // float origin = viewport[2]-250;
   // float originy = 550+30;//550 + 30;
 //	x = origin;//viewport[2]-250;
//	y = originy;//640;   
  
	for(int i=1;i<11;i++)
	{
		for(int j=0;j<maxExp+1;j++)
		{
			glPushMatrix();

			glTranslatef(x, y, 0);
			Single_Dual(j, i);

			glPopMatrix();

			x = x + width;
		}
		x = origin;//viewport[2]-250;
		if(i<=5)
		y = y - 7*(height) - 7;
		else
		y = y - 7*(i+1);
	}

    glMatrixMode(GL_PROJECTION);
    glLoadMatrixd(pmat);
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixd(vmat);
}

void svQDOTDrawing::Coesize_Legend()
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



/*
  char str[10];
  float x = viewport[2]-150.;
  float y = viewport[3]/20;//viewport[3]-200.;
  float size = viewport[3]/50;	

	x = viewport[2]*0.93;
	y = y + size*3;


	glColor3f(0.,0.,0.);
	glBegin(GL_LINE_LOOP);
	glVertex2f(x,y+0.5*size);
	glVertex2f(x+size*3,y+0.5*size);
	glVertex2f(x+size*3,y+2.5*size);
	glVertex2f(x,y+2.5*size);
	glEnd();
	glColor3f(1.,1.,1.);
	glBegin(GL_LINES);
	glVertex2f(x,y+4*size-size);
	glVertex2f(x+size*3,y+4*size-size);
	glEnd();
	glBegin(GL_LINES);
	glVertex2f(x,y+4*size-size);
	glVertex2f(x+size*0.2,y+4*size-size-size*0.2);
	glEnd();
	glBegin(GL_LINES);
	glVertex2f(x,y+4*size-size);
	glVertex2f(x+size*0.2,y+4*size-size+size*0.2);
	glEnd();
	glBegin(GL_LINES);
	glVertex2f(x+size*3,y+4*size-size-size*0.2);
	glVertex2f(x+size*3,y+4*size-size+size*0.2);
	glEnd();
	glBegin(GL_LINES);
	glVertex2f(x+size*3,y+4*size-size);
	glVertex2f(x+size*3-size*0.2,y+4*size-size-size*0.2);
	glEnd();
	glBegin(GL_LINES);
	glVertex2f(x+size*3,y+4*size-size);
	glVertex2f(x+size*3-size*0.2,y+4*size-size+size*0.2);
	glEnd();
	glColor3f(1.,1.,1.);
	glRasterPos2f(x+size*0.8,y+3.7*size);
	sprintf(str,"digit");
	for(int ii=0;ii<strlen(str);ii++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[ii]);

	glColor3f(0.,0.,0.);
	glLineWidth(0.2);
	glBegin(GL_LINES);
	glVertex2f(x,y+size*1.5);
	glVertex2f(x+size*5,y+size*1.5);
	glEnd();
	glBegin(GL_LINES);
	glVertex2f(x+size*2,y+0.5*size);
	glVertex2f(x+size*2,y+2.5*size);
	glEnd();	
	glBegin(GL_LINES);
	glVertex2f(x+size,y+0.5*size);
	glVertex2f(x+size,y+2.5*size);
	glEnd();

	glColor3f(1.,1.,1.);
	glLineWidth(1.);
	glBegin(GL_LINES);
	glVertex2f(x+size*5,y+4*size-size+size*2-size*0.5-size*0.2);
	glVertex2f(x+size*5,y+4*size-size+size*2-size*0.5+size*0.2);
	glEnd();
	glBegin(GL_LINES);
	glVertex2f(x,y+4*size-size-size*0.2);
	glVertex2f(x,y+4*size-size+size*2);
	glEnd();
	glBegin(GL_LINES);
	glVertex2f(x,y+4*size-size+size*2-size*0.5);
	glVertex2f(x+size*5,y+4*size-size+size*2-size*0.5);
	glEnd();
	glBegin(GL_LINES);
	glVertex2f(x,y+4*size-size+size*2-size*0.5);
	glVertex2f(x+size*0.2,y+4*size-size+size*2-size*0.5-size*0.2);
	glEnd();
	glBegin(GL_LINES);
	glVertex2f(x,y+4*size-size+size*2-size*0.5);
	glVertex2f(x+size*0.2,y+4*size-size+size*2-size*0.5+size*0.2);
	glEnd();
	glBegin(GL_LINES);
	glVertex2f(x+size*5,y+4*size-size+size*2-size*0.5);
	glVertex2f(x+size*5-size*0.2,y+4*size-size+size*2-size*0.5-size*0.2);
	glEnd();
	glBegin(GL_LINES);
	glVertex2f(x+size*5,y+4*size-size+size*2-size*0.5);
	glVertex2f(x+size*5-size*0.2,y+4*size-size+size*2-size*0.5+size*0.2);
	glEnd();
	glRasterPos2f(x+size*2.45,y+4*size-size+size*2+size*0.25);

	sprintf(str,"5");
	for(int ii=0;ii<strlen(str);ii++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[ii]);

	glLineWidth(0.2);*/

    glMatrixMode(GL_PROJECTION);
    glLoadMatrixd(pmat);
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixd(vmat);
}

void svQDOTDrawing::Single_SizeSizeColor(int exp, int coe)
{
	float x =0;
	float y =0;
	float size = 7;

	glDisable(GL_BLEND);
	glDisable( GL_LINE_SMOOTH );

	glColor3f(0.3,0.3,0.3);
	glLineWidth(0.1);


	if(exp != 0)
	{
		glBegin(GL_LINES);
		glVertex2f(x+exp*size*2/2,y);
		glVertex2f(x+exp*size*2/2,y-size*5);
		glEnd();
	}
	else
	{
		glBegin(GL_LINES);
		glVertex2f(x+0.25*size,y);
		glVertex2f(x+0.25*size,y-size*5);
		glEnd();
	}


	glBegin(GL_LINES);
	glVertex2f(x+exp*size*2/2,y);
	glVertex2f(x+5*size+exp*size*2/2,y);
	glEnd();

	if(exp != 0)
	{
		for(int i=1;i<coe;i++)
		{
			glBegin(GL_LINES);
			glVertex2f(x,y-size*i);
			glVertex2f(x+exp*size*2,y-size*i);
			glEnd();
		}
	}
	else
	{
		for(int i=1;i<coe;i++)
		{
			glBegin(GL_LINES);
			glVertex2f(x,y-size*i);
			glVertex2f(x+0.25*size,y-size*i);
			glEnd();
		}
	}


	if(exp == 0)
	{
		glBegin(GL_LINE_LOOP);
		glVertex2f(x,y);
		glVertex2f(x+0.5*size,y);
		glVertex2f(x+0.5*size,y-size*coe);
		glVertex2f(x,y-size*coe);
		glEnd();
	}
	else
	{
		glBegin(GL_LINE_LOOP);
		glVertex2f(x,y);
		glVertex2f(x+exp*size*2,y);
		glVertex2f(x+exp*size*2,y-size*coe);
		glVertex2f(x,y-size*coe);
		glEnd();
	}

	glEnable( GL_LINE_SMOOTH );
	glHint( GL_LINE_SMOOTH_HINT, GL_NICEST );
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glColor4f(color[(int)maxExp-2][exp][0]/255.,color[(int)maxExp-2][exp][1]/255.,color[(int)maxExp-2][exp][2]/255.,0.75);
	
	if(exp == 0)
	{
		glBegin(GL_QUADS);
		glVertex2f(x,y);
		glVertex2f(x+0.5*size,y);
		glVertex2f(x+0.5*size,y-size*coe);
		glVertex2f(x,y-size*coe);
		glEnd();
	}
	else
	{
		glBegin(GL_QUADS);
		glVertex2f(x,y);
		glVertex2f(x+exp*size*2,y);
		glVertex2f(x+exp*size*2,y-size*coe);
		glVertex2f(x,y-size*coe);
		glEnd();
	}
	
	glLineWidth(1.);
}
void svQDOTDrawing::Single_SizeSize(int exp, int coe)
{
	float x =0;
	float y =0;
	float size = 7;

	glDisable(GL_BLEND);
	glDisable( GL_LINE_SMOOTH );

	glColor3f(0.3,0.3,0.3);
	glLineWidth(0.1);


	if(exp != 0)
	{
		glBegin(GL_LINES);
		glVertex2f(x+exp*size*2/2,y);
		glVertex2f(x+exp*size*2/2,y-size*5);
		glEnd();
	}
	else
	{
		glBegin(GL_LINES);
		glVertex2f(x+0.25*size,y);
		glVertex2f(x+0.25*size,y-size*5);
		glEnd();
	}


	glBegin(GL_LINES);
	glVertex2f(x+exp*size*2/2,y);
	glVertex2f(x+5*size+exp*size*2/2,y);
	glEnd();

	if(exp != 0)
	{
		for(int i=1;i<coe;i++)
		{
			glBegin(GL_LINES);
			glVertex2f(x,y-size*i);
			glVertex2f(x+exp*size*2,y-size*i);
			glEnd();
		}
	}
	else
	{
		for(int i=1;i<coe;i++)
		{
			glBegin(GL_LINES);
			glVertex2f(x,y-size*i);
			glVertex2f(x+0.25*size,y-size*i);
			glEnd();
		}
	}


	if(exp == 0)
	{
		glBegin(GL_LINE_LOOP);
		glVertex2f(x,y);
		glVertex2f(x+0.5*size,y);
		glVertex2f(x+0.5*size,y-size*coe);
		glVertex2f(x,y-size*coe);
		glEnd();
	}
	else
	{
		glBegin(GL_LINE_LOOP);
		glVertex2f(x,y);
		glVertex2f(x+exp*size*2,y);
		glVertex2f(x+exp*size*2,y-size*coe);
		glVertex2f(x,y-size*coe);
		glEnd();
	}

	glEnable( GL_LINE_SMOOTH );
	glHint( GL_LINE_SMOOTH_HINT, GL_NICEST );
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glColor4f(1,1,1.,0.75);
	
	if(exp == 0)
	{
		glBegin(GL_QUADS);
		glVertex2f(x,y);
		glVertex2f(x+0.5*size,y);
		glVertex2f(x+0.5*size,y-size*coe);
		glVertex2f(x,y-size*coe);
		glEnd();
	}
	else
	{
		glBegin(GL_QUADS);
		glVertex2f(x,y);
		glVertex2f(x+exp*size*2,y);
		glVertex2f(x+exp*size*2,y-size*coe);
		glVertex2f(x,y-size*coe);
		glEnd();
	}
	
	glLineWidth(1.);
}
void svQDOTDrawing::Size_Legend()
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
  float y = viewport[3]/15;//viewport[3]-200.;
  float size = viewport[3]/54;	

	x = viewport[2]*4.5/5.;
	y = y + size*3;

	glColor3f(1.,1.,1.);
	glBegin(GL_LINES);
	glVertex2f(x-size/2,y+0.5*size);
	glVertex2f(x-size/2,y+2.5*size);
	glEnd();
	glBegin(GL_LINES);
	glVertex2f(x-size/2-size*0.2,y+0.5*size);
	glVertex2f(x-size/2+size*0.2,y+0.5*size);
	glEnd();
	glBegin(GL_LINES);
	glVertex2f(x-size/2,y+0.5*size);
	glVertex2f(x-size/2+size*0.2,y+0.5*size+size*0.2);
	glEnd();
	glBegin(GL_LINES);
	glVertex2f(x-size/2,y+0.5*size);
	glVertex2f(x-size/2-size*0.2,y+0.5*size+size*0.2);
	glEnd();
	glBegin(GL_LINES);
	glVertex2f(x-size/2-size*0.2,y+2.5*size);
	glVertex2f(x-size/2+size*0.2,y+2.5*size);
	glEnd();
	glBegin(GL_LINES);
	glVertex2f(x-size/2,y+2.5*size);
	glVertex2f(x-size/2-size*0.2,y+2.5*size-size*0.2);
	glEnd();
	glBegin(GL_LINES);
	glVertex2f(x-size/2,y+2.5*size);
	glVertex2f(x-size/2+size*0.2,y+2.5*size-size*0.2);
	glEnd();
	glColor3f(1.,1.,1.);
	glPushMatrix();
	glRasterPos2f(x-size,y);
	sprintf(str,"exponent");
	for(int ii=0;ii<strlen(str);ii++)
	{	glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[ii]);
	}
	glPopMatrix();

	glColor3f(1.,1.,1.);
	glBegin(GL_LINES);
	glVertex2f(x-size,y+1.5*size);
	glVertex2f(x-size,y+6.5*size);
	glEnd();
	glBegin(GL_LINES);
	glVertex2f(x-size-size*0.2,y+1.5*size);
	glVertex2f(x-size+size*0.2,y+1.5*size);
	glEnd();
	glBegin(GL_LINES);
	glVertex2f(x-size,y+1.5*size);
	glVertex2f(x-size+size*0.2,y+1.5*size+size*0.2);
	glEnd();
	glBegin(GL_LINES);
	glVertex2f(x-size,y+1.5*size);
	glVertex2f(x-size-size*0.2,y+1.5*size+size*0.2);
	glEnd();
	glBegin(GL_LINES);
	glVertex2f(x-size-size*0.2,y+6.5*size);
	glVertex2f(x-size+size*0.2,y+6.5*size);
	glEnd();
	glBegin(GL_LINES);
	glVertex2f(x-size,y+6.5*size);
	glVertex2f(x-size-size*0.2,y+6.5*size-size*0.2);
	glEnd();
	glBegin(GL_LINES);
	glVertex2f(x-size,y+6.5*size);
	glVertex2f(x-size+size*0.2,y+6.5*size-size*0.2);
	glEnd();
	glColor3f(1.,1.,1.);
	glPushMatrix();
	glRasterPos2f(x-size*1.5,y + size*3.5);
	sprintf(str,"5");
	for(int ii=0;ii<strlen(str);ii++)
	{	glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[ii]);
	}
	glPopMatrix();

	glColor3f(0.,0.,0.);
	glBegin(GL_LINE_LOOP);
	glVertex2f(x,y+0.5*size);
	glVertex2f(x+size*3,y+0.5*size);
	glVertex2f(x+size*3,y+2.5*size);
	glVertex2f(x,y+2.5*size);
	glEnd();
	glColor3f(1.,1.,1.);
	glBegin(GL_LINES);
	glVertex2f(x,y+4*size-size);
	glVertex2f(x+size*3,y+4*size-size);
	glEnd();
	glBegin(GL_LINES);
	glVertex2f(x,y+4*size-size);
	glVertex2f(x+size*0.2,y+4*size-size-size*0.2);
	glEnd();
	glBegin(GL_LINES);
	glVertex2f(x,y+4*size-size);
	glVertex2f(x+size*0.2,y+4*size-size+size*0.2);
	glEnd();
	glBegin(GL_LINES);
	glVertex2f(x+size*3,y+4*size-size-size*0.2);
	glVertex2f(x+size*3,y+4*size-size+size*0.2);
	glEnd();
	glBegin(GL_LINES);
	glVertex2f(x+size*3,y+4*size-size);
	glVertex2f(x+size*3-size*0.2,y+4*size-size-size*0.2);
	glEnd();
	glBegin(GL_LINES);
	glVertex2f(x+size*3,y+4*size-size);
	glVertex2f(x+size*3-size*0.2,y+4*size-size+size*0.2);
	glEnd();
	glColor3f(1.,1.,1.);
	glRasterPos2f(x+size*0.8,y+3.7*size);
	sprintf(str,"digit");
	for(int ii=0;ii<strlen(str);ii++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[ii]);

	glColor3f(0.,0.,0.);
	glLineWidth(0.2);
	glBegin(GL_LINES);
	glVertex2f(x,y+1.5*size);
	glVertex2f(x+size*5,y+1.5*size);
	glEnd();
	glBegin(GL_LINES);
	glVertex2f(x,y+1.5*size);
	glVertex2f(x,y+6.5*size);
	glEnd();
	glBegin(GL_LINES);
	glVertex2f(x+size*2,y+0.5*size);
	glVertex2f(x+size*2,y+2.5*size);
	glEnd();	
	glBegin(GL_LINES);
	glVertex2f(x+size,y+0.5*size);
	glVertex2f(x+size,y+2.5*size);
	glEnd();

	glColor3f(1.,1.,1.);
	glLineWidth(1.);
	glBegin(GL_LINES);
	glVertex2f(x+size*5,y+4*size-size+size*2-size*0.5-size*0.2);
	glVertex2f(x+size*5,y+4*size-size+size*2-size*0.5+size*0.2);
	glEnd();
	glBegin(GL_LINES);
	glVertex2f(x,y+4*size-size+size*2-size*0.5);
	glVertex2f(x+size*5,y+4*size-size+size*2-size*0.5);
	glEnd();
	glBegin(GL_LINES);
	glVertex2f(x,y+4*size-size+size*2-size*0.5);
	glVertex2f(x+size*0.2,y+4*size-size+size*2-size*0.5-size*0.2);
	glEnd();
	glBegin(GL_LINES);
	glVertex2f(x,y+4*size-size+size*2-size*0.5);
	glVertex2f(x+size*0.2,y+4*size-size+size*2-size*0.5+size*0.2);
	glEnd();
	glBegin(GL_LINES);
	glVertex2f(x+size*5,y+4*size-size+size*2-size*0.5);
	glVertex2f(x+size*5-size*0.2,y+4*size-size+size*2-size*0.5-size*0.2);
	glEnd();
	glBegin(GL_LINES);
	glVertex2f(x+size*5,y+4*size-size+size*2-size*0.5);
	glVertex2f(x+size*5-size*0.2,y+4*size-size+size*2-size*0.5+size*0.2);
	glEnd();
	glRasterPos2f(x+size*2.45,y+4*size-size+size*2+size*0.25);

	sprintf(str,"5");
	for(int ii=0;ii<strlen(str);ii++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[ii]);

	glLineWidth(0.2);

    glMatrixMode(GL_PROJECTION);
    glLoadMatrixd(pmat);
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixd(vmat);
}


void svQDOTDrawing::Single_ColorTexture(int exp, int coe)
{
	float x =0;
	float y =0;
	float size = 7;

	glDisable(GL_BLEND);
	glDisable( GL_LINE_SMOOTH );

	glColor3f(0.3,0.3,0.3);
	glLineWidth(0.1);


	glLineWidth(0.2);
	glBegin(GL_LINES);
	glVertex2f(x+2*size/2,y);
	glVertex2f(x+2*size/2,y-size*5);
	glEnd();

	for(int i=1;i<4;i++)
	{
		glBegin(GL_LINES);
		glVertex2f(x+size*2,y-size*i);
		glVertex2f(x,y-size*i);
		glEnd();
	}	

	/*glBegin(GL_LINE_LOOP);
	glVertex2f(x,y);
	glVertex2f(x+2*size,y);
	glVertex2f(x+size*2,y-size*4);
	glVertex2f(x,y-size*4);
	glEnd();
*/

	glEnable( GL_LINE_SMOOTH );
	glHint( GL_LINE_SMOOTH_HINT, GL_NICEST );
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glColor4f(texture_color[exp][0]/255.,texture_color[exp][1]/255.,
		texture_color[exp][2]/255.,0.75);

	glBegin(GL_QUADS);
	glVertex2f(x,y);
	glVertex2f(x+2*size * coe/10.,y);
	glVertex2f(x+2*size * coe/10.,y-size*4);
	glVertex2f(x,y-size*4);
	glEnd();

	glColor4f(color[(int)maxExp-2][exp][0]/255.,color[(int)maxExp-2][exp][1]/255.,
		color[(int)maxExp-2][exp][2]/255.,0.75);

	glBegin(GL_QUADS);
	glVertex2f(x+2*size * coe/10.,y);
	glVertex2f(x+2*size * coe/10.,y-size*4);
	glVertex2f(x+2*size ,y-size*4);
	glVertex2f(x+2*size,y);
	glEnd();


	
}

void svQDOTDrawing::Color_Legend(int shift)
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
 // float x = viewport[2]-450.;
  //float y = viewport[3]-100.;
  float size = 10.;
  
  
  glEnable(GL_BLEND);
  double x, y;
  x = 0;y=0;
  glColor3f(0,0,0);
     glColor3f(1,1,1);
   if(shift != 4)
    glRasterPos2f(viewport[2]-200.,viewport[3]/8);
   else
     glRasterPos2f(viewport[2] - 220,viewport[3]/8);
   sprintf(str,"exponent");
   for(int ii=0;ii<strlen(str);ii++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[ii]);
   
 for(int i=0;i<maxExp;i++)
 {
   glColor4f(color[(int)maxExp-2][3-i][0]/255., color[(int)maxExp-2][3-i][1]/255., color[(int)maxExp-2][3-i][2]/255., 0.5);
   glPushMatrix();
   if(shift != 4)
    glTranslatef(viewport[2]-50., viewport[3]/7.5 + 50*i, 0);
   else
     glTranslatef(viewport[2]-200., viewport[3]/7.5 + 50*i, 0);
   glScalef(32, 32, 1);

   glBegin(GL_QUADS);
   glVertex2f(x,y);
   glVertex2f(x+1,y);
   glVertex2f(x+1,y+1);
   glVertex2f(x,y+1);
   glEnd();  
   
   glPopMatrix();
   
   glColor3f(1,1,1);
   if(shift != 4)
    glRasterPos2f(viewport[2]-60,viewport[3]/6.5 + 50*i);
   else
     glRasterPos2f(viewport[2]-210,viewport[3]/6.5 + 50*i);
   sprintf(str,"%d", 3-i);
   for(int ii=0;ii<strlen(str);ii++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[ii]);
 //  x = x + 2;
 }
 
     glMatrixMode(GL_PROJECTION);
    glLoadMatrixd(pmat);
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixd(vmat);  
}

void svQDOTDrawing::Single_TextureSize(int exp)
{
        int coe = 2;
	float x =0;
	float y =0;
	float size = 7;


	glEnable( GL_LINE_SMOOTH );
	glHint( GL_LINE_SMOOTH_HINT, GL_NICEST );
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glColor4f(0.,0,0,0.95);
	glBegin(GL_QUADS);
	glVertex2f(x,y);
	glVertex2f(x+2*size * exp/maxExp,y);//exp/10.,y);
	glVertex2f(x+2*size * exp/maxExp, y-size*coe);//exp/10.,y-size*coe);
	glVertex2f(x,y-size*coe);
	glEnd();

	glColor4f(1.,1,1,0.95);
	glBegin(GL_QUADS);
	glVertex2f(x+2*size * exp/maxExp,y);//exp/10.,y);
	glVertex2f(x+2*size * exp/maxExp, y-size*coe);//exp/10.,y-size*coe);
	glVertex2f(x+2*size ,y-size*coe);
	glVertex2f(x+2*size,y);
	glEnd();
}
void svQDOTDrawing::Single_TextureSize(int exp, int coe)
{
	float x =0;
	float y =0;
	float size = 7;

	//exp = exp * 3;

	glDisable(GL_BLEND);
	glDisable( GL_LINE_SMOOTH );

	glColor3f(0.3,0.3,0.3);
//	glLineWidth(0.1);

//	glLineWidth(0.2);
	glBegin(GL_LINES);
	glVertex2f(x+2*size/2,y);
	glVertex2f(x+2*size/2,y-size*5);
	glEnd();
  
        svVector3 pos;
        pos[0] = x+2*size/2;pos[1]=y;pos[2]=0;
        svVector3 vel;
        vel[0] =0;vel[1]=-1;vel[2]=0;
        svScalar length = size;
        drawing->Draw_2DFishBone(pos, vel, length);
	/*glBegin(GL_LINE_LOOP);
	glVertex2f(x,y);
	glVertex2f(x+2*size,y);
	glVertex2f(x+size*2,y-size*coe);
	glVertex2f(x,y-size*coe);
	glEnd();*/

	glEnable( GL_LINE_SMOOTH );
	glHint( GL_LINE_SMOOTH_HINT, GL_NICEST );
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glColor4f(0.,0,0,0.95);
	glBegin(GL_QUADS);
	glVertex2f(x,y);
	glVertex2f(x+2*size * exp/maxExp,y);//exp/10.,y);
	glVertex2f(x+2*size * exp/maxExp, y-size*coe);//exp/10.,y-size*coe);
	glVertex2f(x,y-size*coe);
	glEnd();

	glColor4f(1.,1,1,0.95);
	glBegin(GL_QUADS);
	glVertex2f(x+2*size * exp/maxExp,y);//exp/10.,y);
	glVertex2f(x+2*size * exp/maxExp, y-size*coe);//exp/10.,y-size*coe);
	glVertex2f(x+2*size ,y-size*coe);
	glVertex2f(x+2*size,y);
	glEnd();

	glColor3f(0,0,0);
	for(int i=1;i<coe;i++)
	{
		glBegin(GL_LINES);
		glVertex2f(x+size*2,y-size*i);
		glVertex2f(x,y-size*i);
		glEnd();
	}	

}



void svQDOTDrawing::VerticalTexture_Legend(bool shift)
{
    glDisable(GL_LIGHTING);
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
 // float x = viewport[2]-450.;
  //float y = viewport[3]-100.;
  float size = 10.;
 // glShadeModel(GL_FLAT);
  //glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
  glEnable(GL_BLEND);
  double x, y;
  x = 0;y=0;
  glColor3f(0,0,0);
  int end;
  int s;
  if(shift)
  {
    s = 0;
    end = 4;
  }
  else
  {
    s = 1;
    end = 10;
  }
  int ii;
  glColor3f(1.,1.,1.);
   if(shift)
   {
   glRasterPos2f(viewport[2]-220.,viewport[3]/8);
   sprintf(str,"exponent");
   for(int ii=0;ii<strlen(str);ii++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[ii]);
   }
   else
   {
     glRasterPos2f(viewport[2]-110.,viewport[3]/8);
   sprintf(str,"coefficient");
   for(int ii=0;ii<strlen(str);ii++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[ii]);
   }



 for(int i=0;i<end;)//i++)
 {
   if(!shift)
   {
     ii =end-i;
   }
   else
   {
     ii = end-1-i;
     
       
   } 

   glPushMatrix();
   if(!shift)
    glTranslatef(viewport[2]-100., viewport[3]/7.5 + 40*i, 0);
   else
     glTranslatef(viewport[2]-220., viewport[3]/7.5 +40*i, 0);
   
   if(!shift)
   {
     Single_Texture((double)ii);
   }
   else
   {
     Single_Texture((double)ii*3);
     
   } 
   
   glPopMatrix();


   glColor3f(1,1,1);

   if(!shift)
    glRasterPos2f(viewport[2]-115.,viewport[3]/6.5 + 40*i);
   else
     glRasterPos2f(viewport[2]-230,viewport[3]/6.5 + 40*i);
   sprintf(str,"%d", ii);
   for(int j=0;j<strlen(str);j++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[j]);
   if(!shift)
   {
     i++;
   }
   else
   {
     i++;
     
       
   } 
 //  x = x + 2;
 }/*
 if(!shift)
{
	glBegin(GL_LINES);
	glVertex2f(viewport[2]-50., viewport[3]/7.5);
	glVertex2f(viewport[2]-50. , viewport[3]/7.5 + 40*9);
	glEnd();
}
 */
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixd(pmat);
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixd(vmat);  

	
}
void svQDOTDrawing::HorizontalTexture_Legend(bool shift)
{
    glDisable(GL_LIGHTING);
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
 // float x = viewport[2]-450.;
  //float y = viewport[3]-100.;
  float size = 10.;
 // glShadeModel(GL_FLAT);
  //glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
  glEnable(GL_BLEND);
  double x, y;
  x = 0;y=0;
  glColor3f(0,0,0);
  int end;
  int s;
  if(shift)
  {
    s = 0;
    end = 4;
  }
  else
  {
    s = 1;
    end = 9;
  }
  int ii;
  glColor3f(1.,1.,1.);
   if(shift)
   {
   glRasterPos2f(viewport[2]-180.,viewport[3]/8);
   sprintf(str,"exponent");
   for(int ii=0;ii<strlen(str);ii++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[ii]);
   }
   else
   {
     glRasterPos2f(viewport[2]-80.,viewport[3]/8);
   sprintf(str,"coefficient");
   for(int ii=0;ii<strlen(str);ii++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[ii]);
   }
 for(int i=0;i<end;i++)
 {
   if(!shift)
   {
     ii =end-i;
   }
   else
   {
     ii = end-1-i;
   } 
   
   makeHorizontalTexture(ii);
   inittexture_horizontal();
   
   glEnable(GL_TEXTURE_2D);
   glPushMatrix();
   if(!shift)
    glTranslatef(viewport[2]-50., viewport[3]/7.5 + 50*i, 0);
   else
     glTranslatef(viewport[2]-150., viewport[3]/7.5 + 50*i, 0);
   
   glScalef(30, 30, 1);
   glBegin(GL_QUADS);
   glTexCoord2f(1,0);
   glVertex2f(x,y);
   glTexCoord2f(1,1);
   glVertex2f(x+1,y);
   glTexCoord2f(0,1);
   glVertex2f(x+1,y+1);
   glTexCoord2f(0,0);
   glVertex2f(x,y+1);
   glEnd(); 
   
   glPopMatrix();
   glDisable(GL_TEXTURE_2D);
   glColor3f(1,1,1);

   if(!shift)
    glRasterPos2f(viewport[2]-60,viewport[3]/6.5 + 50*i);
   else
     glRasterPos2f(viewport[2]-160,viewport[3]/6.5 + 50*i);
   sprintf(str,"%d", ii);
   for(int j=0;j<strlen(str);j++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[j]);
 //  x = x + 2;
 }
 
     glMatrixMode(GL_PROJECTION);
    glLoadMatrixd(pmat);
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixd(vmat);  

	
}
void svQDOTDrawing::Texture_Legend(bool shift)
{
glDisable(GL_LIGHTING);
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
 // float x = viewport[2]-450.;
  //float y = viewport[3]-100.;
  float size = 10.;
 //make
 //glShadeModel(GL_FLAT);
  //glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
  glEnable(GL_BLEND);
  double x, y;
  x = 0;y=0;
  glColor3f(0,0,0);
  int end;
  int s;
  if(shift)
  {
    s = 0;
    end = 4;
  }
  else
  {
    s = 1;
    end = 9;
  }
  int ii;
  glColor3f(1.,1.,1.);
   if(shift)
   {
   glRasterPos2f(viewport[2]-180.,viewport[3]/8);
   sprintf(str,"exponent");
   for(int ii=0;ii<strlen(str);ii++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[ii]);
   }
   else
   {
     glRasterPos2f(viewport[2]-80.,viewport[3]/8);
   sprintf(str,"coefficient");
   for(int ii=0;ii<strlen(str);ii++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[ii]);
   }
 for(int i=0;i<end;i++)
 {
   if(!shift)
   {
     ii =end-i;
   }
   else
   {
     ii = end-1-i;
   } 
   glEnable(GL_TEXTURE_2D);
   glPushMatrix();
   if(!shift)
    glTranslatef(viewport[2]-50., viewport[3]/7.5 + 26*i, 0);
   else
     glTranslatef(viewport[2]-150., viewport[3]/7.5 + 26*i, 0);
   glScalef(20, 20, 1);

   glBegin(GL_QUADS);
   glTexCoord2f(0,0.5 - 0.05 * ii);
   glVertex2f(x,y);
   glTexCoord2f(1,0.5 - 0.05 * ii);
   glVertex2f(x+1,y);
   glTexCoord2f(1,0.5 + 0.05 * ii);
   glVertex2f(x+1,y+1);
   glTexCoord2f(0,0.5 + 0.05 * ii);
   glVertex2f(x,y+1);
   glEnd(); 
   
   glPopMatrix();
   glDisable(GL_TEXTURE_2D);
   glColor3f(1,1,1);

   if(!shift)
    glRasterPos2f(viewport[2]-60,viewport[3]/6.5 + 26*i);
   else
     glRasterPos2f(viewport[2]-160,viewport[3]/6.5 + 26*i);
   sprintf(str,"%d", ii);
   for(int j=0;j<strlen(str);j++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[j]);
 //  x = x + 2;
 }
 
     glMatrixMode(GL_PROJECTION);
    glLoadMatrixd(pmat);
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixd(vmat);  

	
}
void svQDOTDrawing::Single_Texture(double n)
{  
  double min_length = n;
  double white_length = 10. - min_length;
  int num = 5;

  double x,y;
  x = 0; y = 0;
   
  for(int j=0;j<num;j++)
  {
    glColor4f(1,1,1,0.5);
    glBegin(GL_QUADS);    
    glVertex2f(x,y);
    glVertex2f(x+white_length/2.,y);
    glVertex2f(x+white_length/2.,y+10.*num/2.);
    glVertex2f(x,y+10.*num/2.);
    glEnd();

    x = x + white_length/2.;

    glColor4f(0,0,0,0.5);
    glBegin(GL_QUADS);    
    glVertex2f(x,y);
    glVertex2f(x+min_length,y);
    glVertex2f(x+min_length,y+10.*num/2.);
    glVertex2f(x,y+10.*num/2.);
    glEnd();

    x = x + min_length;

    glColor4f(1,1,1,0.5);
    glBegin(GL_QUADS);    
    glVertex2f(x,y);
    glVertex2f(x+white_length/2.,y);
    glVertex2f(x+white_length/2.,y+10.*num/2.);
    glVertex2f(x,y+10.*num/2.);
    glEnd();
    
    x = x + white_length/2.;
  }//cerr<<y<<endl;

}
/*
void svQDOTDrawing::Single_Texture(double n)
{

  
  int num;
  int int_n = (int)n;
  
  double min_length = ((1./ 2.) * ((double)int_n/n))/int_n;//cerr<<min_length<<endl;
  double white_length;
  if(int_n<n)
  {
    num = n+1;
  }
  else
  {
    num = n;
  }
  if(num == 1)
  {
    white_length = (1./2.);
  }
  else
  {
    white_length = (1./2.) / (num-1);//cerr<<min_length<<" "<<white_length<<endl;
  }//cerr<<white_length<<endl;
  
  double x,y;
  x = 0; y = 0;
  
  glColor3f(0,0,0);
  for(int j=0;j<int_n;j++)
  {
    glBegin(GL_QUADS);    
    glVertex2f(x,y);
    glVertex2f(x+1,y);
    glVertex2f(x+1,(j+1) * min_length + j * white_length);
    glVertex2f(x, (j+1) * min_length + j * white_length);
    glEnd();
    
    y = (j+1) * min_length + (j+1) * white_length;
  }//cerr<<y<<endl;
  if(int_n<n)
  {
    glBegin(GL_QUADS);    
    glVertex2f(x,y);
    glVertex2f(x+1,y);
    glVertex2f(x+1,1);
    glVertex2f(x, 1);
    glEnd();    
  }
}*/

bool LessAngle(svVector3 p1, svVector3 p2, svVector3 p)
{
  svVector3 n1 = normalize(p - p1);
  svVector3 n2 = normalize(p2 - p1);
  
  double dot = n1[0] * n2[0] + n1[1] * n2[1] + n1[2] * n2[2];
  
  if(dot >0)
    return true;
  else
    return false;
}
float GetDot(svVector3 a, svVector3 b)
{
	float c;
	c = a[0]*b[0]+a[1]*b[1]+a[2]*b[2];
	return c;
}
float PointtoLine(svVector3 p1, svVector3 p2, svVector3 p)
{
	//distance(x=a+tn, p) = ||(a-p)-((a-p)*n)n||
	//wiki http://en.wikipedia.org/wiki/Distance_from_a_point_to_a_line

	svVector3 n;
	n = p1-p2;
	n = normalize(n);
	svVector3 a=p2;
	svVector3 ap=a - p;
	float apdot=GetDot(ap,n);
	svVector3 apn = apdot*n;
	svVector3 d = ap - apn;

	float distance = sqrt(d[0]*d[0]+d[1]*d[1]
				+d[2]*d[2]);	

	return distance;
}

int svQDOTDrawing::Find_Nearest(int x, int y, int encodeID, GLfloat *m){

    GLint viewport[4];
    GLdouble mvmatrix[16];
    GLdouble projmatrix[16];
    GLdouble Z = -9E+9; 
    int index = -1;
 
   glGetIntegerv (GL_VIEWPORT, viewport);
    glGetDoublev (GL_MODELVIEW_MATRIX, mvmatrix);
    glGetDoublev (GL_PROJECTION_MATRIX, projmatrix);

    for(int i=0;i<dataSize;i++){
       svVector3 p[2], end;
       p[0][0]=0;p[0][1]=0;p[0][2]=0;
       p[1][0]=0;p[1][1]=0;p[1][2]=0;
       if(encodeID == 3){
         if(dataExp[i] > dataCoe[i]){
           end = dataPos[i] + (svScalar)(cylinder_radius * dataExp[i]) * dataVel[i];
         }
         else{
           end = dataPos[i] + (svScalar)(cylinder_radius * dataCoe[i])*dataVel[i];
         }
       }
       else{
           end = dataPos[i] + (svScalar)(cylinder_radius * dataCoe[i]) * dataVel[i];
       }

       p[0][0]  = dataPos[i][0] * m[0] +
                   -dataPos[i][2] * m[4] +
                   dataPos[i][1] * m[8] +
                   m[12] + p[0][0];
       p[0][1]  = dataPos[i][0] * m[1] +
                   -dataPos[i][2] * m[5] +
                   dataPos[i][1] * m[9] +
                  m[13] + p[0][1];
       p[0][2]  = dataPos[i][0] * m[2] +
                   -dataPos[i][2] * m[6] +
                   dataPos[i][1] * m[10] +
                   m[14] + p[0][2];
       p[1][0]  = end[0] * m[0] +
                   -end[2] * m[4] +
                   end[1] * m[8] +
                   m[12] + p[1][0];
       p[1][1]  = end[0] * m[1] +
                   -end[2] * m[5] +
                   end[1] * m[9] +
                  m[13] + p[1][1];
       p[1][2]  = end[0] * m[2] +
                   -end[2] * m[6] +
                   end[1] * m[10] +
                   m[14] + p[1][2];
       GLdouble wx[2], wy[2], wz[2];
       for(int j=0;j<2;j++){
		gluProject((GLdouble)p[j][0],
                        (GLdouble)p[j][1],
                        (GLdouble)p[j][2],
                        mvmatrix, projmatrix, viewport,
                        &(wx[j]), &(wy[j]), &(wz[j]));
		wy[j] = (GLdouble)viewport[3]-wy[j];
       }
       GLdouble objX, objY, objZ;
       gluUnProject((GLdouble)x,
                     (GLdouble)y,
                     wz[0],
                     mvmatrix, projmatrix, viewport,
                     &objX, &objY, &objZ);                   
       double d2D = (wx[0]-wx[1])*(wx[0]-wx[1])
                   +(wy[0]-wy[1])*(wy[0]-wy[1])
                   +(wz[0]-wz[1])*(wz[0]-wz[1]);
       double d3D = (p[0][0]-p[1][0])*(p[0][0]-p[1][0])
                  + (p[0][1]-p[1][1])*(p[0][1]-p[1][1])
                  + (p[0][2]-p[1][2])*(p[0][2]-p[1][2]);
       double ratio = sqrt(d3D)/sqrt(d2D);
       svVector3 dir2D;
       dir2D[0] = wx[1] - wx[0];
       dir2D[1] = wy[1] - wy[0];
       dir2D[2] = 0;//wx[1] - wx[0];
       dir2D = normalize(dir2D);
       svVector3 v2D;
       v2D[0] = -dir2D[1];
       v2D[1] = dir2D[0];
       v2D[2] = 0;
       v2D = normalize(v2D);
       double dV3D = cylinder_radius;
       double dV2D = dV3D/ratio;

      //cout<<dV2D<<" "<<v2D[0]<<" "<<v2D[1]<<endl;
       svVector3 p2D[4];
       p2D[0][0]=wx[0]-v2D[0]*dV2D;
       p2D[0][1]=wy[0]-v2D[1]*dV2D;
       p2D[0][2]=0;//wz[0]-v2D[2]*dV2D;
       p2D[1][0]=wx[0]+v2D[0]*dV2D;
       p2D[1][1]=wy[0]+v2D[1]*dV2D;
       p2D[1][2]=0;//wz[0]+v2D[2]*dV2D;
       p2D[2][0]=wx[1]-v2D[0]*dV2D;
       p2D[2][1]=wy[1]-v2D[1]*dV2D;
       p2D[2][2]=0;//wz[1]-v2D[2]*dV2D;
       p2D[3][0]=wx[1]+v2D[0]*dV2D;
       p2D[3][1]=wy[1]+v2D[1]*dV2D;
       p2D[3][2]=0;//wz[1]+v2D[2]*dV2D;
      
      // cout<<wx[0]<<" "<<wy[0]<<" "<<wz[0]<<endl;
     //  cout<<wx[1]<<" "<<wy[1]<<" "<<wz[1]<<endl; 
     //  for(int j=0;j<4;j++)
      //     cout<<p2D[j][0]<<" "<<p2D[j][1]<<" "<<p2D[j][2]<<endl;
       //cout<<x<<" "<<y<<endl;
       svVector3 tmpp; tmpp[0]=x; tmpp[1]=y; tmpp[2]=0; 
       bool in11 = LessAngle(p2D[0], p2D[1], tmpp);
       bool in12 = LessAngle(p2D[1], p2D[0], tmpp);
       bool in21 = LessAngle(p2D[3], p2D[1], tmpp);
       bool in22 = LessAngle(p2D[1], p2D[3], tmpp);
       bool in31 = LessAngle(p2D[3], p2D[2], tmpp);
       bool in32 = LessAngle(p2D[2], p2D[3], tmpp);
       bool in41 = LessAngle(p2D[0], p2D[2], tmpp);
       bool in42 = LessAngle(p2D[2], p2D[0], tmpp);
       if(in11 && in12 && in21 && in22 && in31 && in32 && in41 && in42){
          if(objZ>Z){
                 Z = objZ;
                 index = i;
          }
       }

   } 

   //cout<<"index"<<" "<<index<<endl;

   return index;
}

int svQDOTDrawing::Find_Nearest(int x, int y, int width, GLfloat *tb_transform, int ENCODE_MODE)
{
    GLdouble td1, dd1, td2, dd2;
    GLdouble eyedd, eyetd;   
 
    int num = dataSize;
    
    svVector3 *p1 = new svVector3 [num];
    svVector3 *p2 = new svVector3 [num];
    svVector3 *v = new svVector3[num];
    svVector3 *new_pos = new svVector3 [num];
    
    GLdouble *wx1 = new GLdouble[num];
    GLdouble *wy1 = new GLdouble[num];
    GLdouble *wz1 = new GLdouble[num];
    GLdouble *wx2 = new GLdouble[num];
    GLdouble *wy2 = new GLdouble[num];
    GLdouble *wz2 = new GLdouble[num];

    svScalar scale;
    for(int i=0;i<num;i++)
    {
    		if(ENCODE_MODE == 3)//splivectors
		{
		  double exp = cylinder_radius * dataExp[i];
		  double coe = cylinder_radius * dataCoe[i];
		  if(exp > coe)
		  {
		    scale = exp;
		  }
		  else
		  {
		    scale = coe;
		  }
		}
		else //if(ENCODE_MODE == 1 ||ENCODE_MODE == 4)
		{
		  scale = cylinder_radius * dataCoe[i];
		}/*
		else if(ENCODE_MODE == 2)
		{
		  scale = cylinder_radius * dataCoe[i];
		}
		else if(ENCODE_MODE == 3)
		{
		  scale = cylinder_radius * dataCoe[i];
		}*/

      new_pos[i] = dataPos[i] + scale * dataVel[i];
    }
    GLint viewport[4];
    GLdouble mvmatrix[16];
    GLdouble projmatrix[16];
  
    glGetIntegerv (GL_VIEWPORT, viewport);
    glGetDoublev (GL_MODELVIEW_MATRIX, mvmatrix);
    glGetDoublev (GL_PROJECTION_MATRIX, projmatrix);

	for(int i=0;i<num;i++)
	{
		p1[i][0]=0;p1[i][1]=0;p1[i][2]=0;	
		p2[i][0]=0;p2[i][1]=0;p2[i][2]=0;	
	}

	for(int i=0;i<num;i++)
	{
		p1[i][0]  = dataPos[i][0] * tb_transform[0] +
			    dataPos[i][1] * tb_transform[4] +
			    dataPos[i][2] * tb_transform[8] +
			    tb_transform[12] + p1[i][0];
                p1[i][1]  = dataPos[i][0] * tb_transform[1] +
                            dataPos[i][1] * tb_transform[5] +
                            dataPos[i][2] * tb_transform[9] +
                            tb_transform[13] + p1[i][1];
                p1[i][2]  = dataPos[i][0] * tb_transform[2] +
                            dataPos[i][1] * tb_transform[6] +
                            dataPos[i][2] * tb_transform[10] +
                            tb_transform[14] + p1[i][2];
	}
	for(int i=0;i<num;i++)
	{
		p2[i][0]  = new_pos[i][0] * tb_transform[0] +
			    new_pos[i][1] * tb_transform[4] +
			    new_pos[i][2] * tb_transform[8] +
			    tb_transform[12] + p2[i][0];
                p2[i][1]  = new_pos[i][0] * tb_transform[1] +
                            new_pos[i][1] * tb_transform[5] +
                            new_pos[i][2] * tb_transform[9] +
                            tb_transform[13] + p2[i][1];
                p2[i][2]  = new_pos[i][0] * tb_transform[2] +
                            new_pos[i][1] * tb_transform[6] +
                            new_pos[i][2] * tb_transform[10] +
                            tb_transform[14] + p2[i][2];
	}
	
	for(int i=0;i<num;i++)
	{
		gluProject((GLdouble)p1[i][0],
                        (GLdouble)p1[i][1],
                        (GLdouble)p1[i][2],
                        mvmatrix, projmatrix, viewport,
                        &(wx1[i]), &(wy1[i]), &(wz1[i]));
		wy1[i] = (GLdouble)viewport[3]-wy1[i];
	}
	for(int i=0;i<num;i++)
	{
		gluProject((GLdouble)p2[i][0],
                        (GLdouble)p2[i][1],
                        (GLdouble)p2[i][2],
                        mvmatrix, projmatrix, viewport,
                        &(wx2[i]), &(wy2[i]), &(wz2[i]));
		wy2[i] = (GLdouble)viewport[3]-wy2[i];
	}

        eyedd = 9e+9;	
	eyetd = 9e+9;
        dd1 = 9e+9;
	dd2 = 9e+9;
	int select_index;
	for(int i=0;i<num;i++)
	{
		td1 = sqrt(((GLdouble)x-wx1[i]) *  ((GLdouble)x-wx1[i]) +
		      ((GLdouble)y-wy1[i]) * ((GLdouble)y-wy1[i]));
		svVector3 tmpp1, tmpp2, tmpp;
                svVector3 mid;
		tmpp1[0] = wx1[i];tmpp1[1] = wy1[i];tmpp1[2]=0;
		tmpp2[0] = wx2[i];tmpp2[1] = wy2[i];tmpp2[2]=0;
                mid[0] = (p1[i][0] + p2[i][0])/2.;
                mid[1] = (p1[i][1] + p2[i][1])/2.;
                mid[2] = (p1[i][2] + p2[i][2])/2.;
		tmpp[0] = x; tmpp[1] = y; tmpp[2] = 0;
		bool in1 = LessAngle(tmpp1, tmpp2, tmpp);
		bool in2 = LessAngle(tmpp2, tmpp1, tmpp);
		td2 = PointtoLine(tmpp2, tmpp1, tmpp);
                eyetd = (mid[0]-eye[0])*(mid[0]-eye[0])
                      + (mid[1]-eye[1])*(mid[1]-eye[1])
                      + (mid[2]-eye[2])*(mid[2]-eye[2]);
		/*if(td1 < dd1)
		{
			dd1 = td1;
			dd2 = td2;
			select_index = i;
		}
		else if(td1 >= dd1 &&fabs(td1 - dd1) < 100)
		{
		  if(td2 > dd2)
		  {
			dd1 = td1;
			dd2 = td2;
			select_index = i;
		  }
		}*/
		if(in1&&in2&&td2<dd2&&eyetd<eyedd)
		{
		  dd1 = td1;
                  eyedd =  eyetd;
			dd2 = td2;
			select_index = i;
		}
	}

        //cout<<dd2<<" "<<td2<<" "<<eyedd<<" "<<eyetd<<" "<<select_index<<endl;

	delete [] p1;
	delete [] p2;
	delete [] v;
	delete [] new_pos;
	delete [] wx1;
	delete [] wy1;
	delete [] wz1;
	delete [] wx2;
	delete [] wy2;
	delete [] wz2;

	return select_index;
}

void svQDOTDrawing::cleanup()
{
  delete [] dataPos;
  delete [] dataVel;
  delete [] dataDen;
  delete [] dataExp;
  delete [] dataCoe;
  
  delete drawing;
  drawing = NULL;
}
