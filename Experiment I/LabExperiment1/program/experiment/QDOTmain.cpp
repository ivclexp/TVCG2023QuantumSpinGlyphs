#include <stdio.h>
#include <iostream>
#include <fstream>
#include <math.h>
#include <assert.h>
#include <GL/glut.h>

#include "svrbf.h"
#include "svHyperTube.h"
#include "svCriticalPointGlyph.h"
#include "svGlyph.h"
#include "svUtil.h"
#include "ivTrackball.h"
#include "ivview.h"
#include "MGL.h"
using namespace __svl_lib;

void reshape(int w, int h);
void display(void);
void key(unsigned char key, int x, int y);

#define IMAGE_WIDTH  512
#define IMAGE_HEIGHT 512

view3d view_info;

svrbf *flow_field;
svHyperTube *hypertube;
svHyperTube *hypertubeb;
svCriticalPointGlyph *glyph;
//svGlyph *tubeglyph;

svVector3Array cord;

GLboolean bBoundingBox=true;
GLboolean bAxis;

GLuint image_width;
GLuint image_height;

GLint nx, ny, nz;
Trackball trackball;

void InitLight()
{
  //setting of lighting
  GLfloat mat_diffuse[] = { 0.8, .0, 0.0, .4};
  GLfloat mat_specular[] = { 1, 1, 1, 1 };
  GLfloat mat_shininess[] = { 20.0 };
  //GLfloat light_position[] = { 24, 24, 60, 0.0 };
  GLfloat light_position[] = { 50,50,50, 0.0 };
  GLfloat white_light[] = { 1.0, 1.0, 1.0, 1.0 };
  glClearColor(0.0, 0.0, 0.0, 0.0);
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, mat_diffuse);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
  glLightfv(GL_LIGHT0, GL_POSITION, light_position);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, white_light);
  glLightfv(GL_LIGHT0, GL_SPECULAR, white_light);
}

//***************************
// GLUT callback functions
//****************************

void reshape(int w, int h)
{

        image_width  = w;
        image_height = h;

        glViewport (0, 0, w, h);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();

        if (view_info.type == PARALLEL){
                glOrtho(view_info.left, view_info.right, view_info.bottom, view_info.top,
                        view_info.hither, view_info.yon);

                glMatrixMode(GL_MODELVIEW);
                glLoadIdentity();

        }
        else{ // perspective
                float aspect = view_info.aspect_ratio * float(w)/float(h);
                float GLfloat_fov;
                if ( aspect < 1 ){
                  // fovy is a misnomer.. we GLfloatly mean the fov applies to the
                  // smaller dimension
                  float fovx, fovy;
                  fovx = fovy = view_info.view_angle;
                  GLfloat_fov = svToDegree(2*atan(tan(svToRadian(fovx/2))/aspect));
                }
                else{
                        GLfloat_fov = view_info.view_angle;
                }

                gluPerspective(GLfloat_fov,
                                           aspect,
                                           view_info.hither,
                                           view_info.yon);

                glMatrixMode(GL_MODELVIEW);
                glLoadIdentity();
                gluLookAt(view_info.eye[0],view_info.eye[1],view_info.eye[2],
                                view_info.coi[0],view_info.coi[1],view_info.coi[2],
                                0, 1, 0);

        }

        InitLight();

}

void display(void)
{
        // set new model view and projection matrix
        glClearColor(0.3, 0.3, 0.3, 1);
//	glClear(GL_COLOR_BUFFER_BIT);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);

        glEnable(GL_COLOR_MATERIAL);
        glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

        GLfloat m[16];
        trackball.getMatrix().getValue(m);

        glPushMatrix();
        glMultMatrixf(m);

        if (bBoundingBox)
                hypertube->DrawOutline();
        //if (bAxis)
        //	draw_axis();

        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
        glEnable(GL_TEXTURE_2D);
        //glEnable(GL_BLEND);
        //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        RenderProperty p;
        p.color = svColor4(0.1, 0.1, 0.8, 1.);
        hypertube->SetRenderProperty(p);
        hypertube->Render();
        hypertubeb->SetRenderProperty(p);
        hypertubeb->Render();

        //tubeglyph->Render();

        glyph->Render();

        glDisable(GL_TEXTURE_2D);
        glDisable(GL_BLEND);
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_LIGHTING);
        glDisable(GL_LIGHT0);

        glPopMatrix();
        glFlush();
        glutSwapBuffers();
}


void key(unsigned char key, int x, int y)
{
        switch (key) {
    case 'f':
        case 'F':
hypertube->SaveToFile("aa.out");
                glutPostRedisplay();
                break;

    case 's':
        case 'S':
        //hypertube->SaveToFile("tube.out");
        hypertube->SetLineLength();
                //WriteToPNM(image_width,image_height,"tube.pnm");
                break;


    case 'b':
        case 'B':
                bBoundingBox = !bBoundingBox;
                glutPostRedisplay();
                break;

    case 'a':
        case 'A':
                bAxis = !bAxis;
                glutPostRedisplay();
                break;

        case '\033':
        case 'q':
        case 'Q':
                exit(0);
                break;
        }
}

void mouse(int button, int state, int x, int y)
{
        long s=0x00000000;
    s |= (button == GLUT_LEFT_BUTTON)   ? ((state == GLUT_DOWN) ? Trackball::LBUTTON_DOWN : Trackball::LBUTTON_UP) : 0;
    s |= (button == GLUT_MIDDLE_BUTTON) ? ((state == GLUT_DOWN) ? Trackball::MBUTTON_DOWN : Trackball::MBUTTON_UP) : 0;
    s |= (button == GLUT_RIGHT_BUTTON)  ? ((state == GLUT_DOWN) ? Trackball::RBUTTON_DOWN : Trackball::RBUTTON_UP) : 0;

    int key_state = glutGetModifiers();
    s |= (key_state & GLUT_ACTIVE_CTRL)  ? Trackball::CTRL_DOWN  : 0;
    s |= (key_state & GLUT_ACTIVE_ALT)   ? Trackball::ALT_DOWN   : 0;
    s |= (key_state & GLUT_ACTIVE_SHIFT) ? Trackball::SHIFT_DOWN : 0;

        if (s & Trackball::BUTTON_DOWN){
        trackball.mouseDown(s, x, y);
        }

        if (s & Trackball::BUTTON_UP){
        trackball.mouseUp(s, x, y);
        }
}

void motion(int x, int y)
{
        trackball.mouseMotion(x, y);
        glutPostRedisplay();
}

//******************************************
// initialization code for GL and NV_EXT
//******************************************

void init(char *rbfname, char *cpname)
{
  flow_field = new svrbf;
  flow_field->New(rbfname);
  flow_field->CalculateRoughStrength(1000);

        //define the seed particles
        svInt seed_number= 40;
        svParticle *orig_seeds = new svParticle[seed_number];

        //for streamline, timeline, and stream surface
        for(int i=0; i<seed_number; i++)
        {
          svVector3 orig_pos;
          orig_pos[0] = float(rand())/float(RAND_MAX)*2.-1.;
          orig_pos[1] = float(rand())/float(RAND_MAX)*2.-1.;
          orig_pos[2] = float(rand())/float(RAND_MAX)*2.-1.;

          svScalar tm = 0.0;
          orig_seeds[i].time = tm;
          orig_seeds[i].position = orig_pos;
          orig_seeds[i].field = flow_field;
          svVector3 spx = flow_field->GetVector(orig_seeds[i]);
          cout << "newp: " << spx[0] << ", " << spx[1] << ", " << spx[2] << endl;
        }

        svScalar min_value = flow_field->GetMinStrength();
        svScalar max_value = flow_field->GetMaxStrength();
        cerr << "field: ( " << min_value << ", " << max_value <<")" << endl;

  hypertube = new svHyperTube(flow_field, orig_seeds, seed_number, SV_STORE);
//hypertube = new svHyperTube(flow_field, orig_seeds, seed_number,0);
        hypertube->SetMaxAdvectTime(100);
        hypertube->SetTubeRadius(0.02);
        //hypertube->SetAdvectDirection(SV_BOTH_DIR);
        hypertube->EnableColor(SV_LOCS,min_value, max_value);
        //hypertube->EnableTubeRadius(0.001, 0.04);
        //hypertube->EnableLineWidth(0.001, 0.04);
        //hypertube->SetTextureSquare();
        hypertube->SetTexture();
        hypertube->SetSegmentFreq(10);
        hypertube->Generate();
        //hypertube->GetSphereLocation( 4);
        //hypertube->RenderSphere(true, 4);

  hypertubeb = new svHyperTube(flow_field, orig_seeds, seed_number, SV_STORE);
        hypertubeb->SetAdvectDirection(SV_BACKWARD);
        hypertubeb->SetMaxAdvectTime(100);
        hypertubeb->SetTubeRadius(0.02);
        //hypertubeb->SetAdvectDirection(SV_BOTH_DIR);
        hypertubeb->EnableColor(SV_LOCS, min_value, max_value);
        //hypertubeb->EnableColor(SV_BLUE_YELLOW);
        //hypertubeb->EnableTubeRadius(0.001, 0.04);
        //hypertubeb->EnableLineWidth(0.001, 0.04);
        //hypertubeb->SetTextureSquare();
        //hypertubeb->SetTexture();
        hypertubeb->SetSegmentFreq(10);
        hypertubeb->Generate();
        //hypertubeb->GetSphereLocation( 4);
        //hypertubeb->RenderSphere(true, 4);

  //tubeglyph = new svGlyph(hypertube->GetVector3Array(), flow_field,
                          //hypertube->GetSeedNum());
  //tubeglyph->EnableColor(SV_LOCS,min_value, max_value);
  //tubeglyph->Generate(hypertube->GetVector3Array());

  //cerr << "genreate tubeglyph" << endl;

  glyph = new svCriticalPointGlyph(cpname);
  glyph->GenerateFromFile();


  svVector3 center = flow_field->GetCenter();
  center.getValue(view_info.coi);

  GLfloat x, y, z;
  flow_field->GetPhysicalDimension(&x,&y, &z);

  view_info.eye[0] = x/2.0;
  view_info.eye[1] = y/2.0;
  view_info.eye[2] = z*2.0;

  trackball.setEye(view_info.eye);
  trackball.setFocus(center);
  trackball.setWindowSize(IMAGE_WIDTH, IMAGE_HEIGHT);

}

//**********************
// program entry
//**********************

int main(int argc, char** argv)
{
  srand(12345);
        glutInit(&argc, argv);
        glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

        image_width  = IMAGE_WIDTH;
        image_height = IMAGE_HEIGHT;

        glutInitWindowSize(image_width, image_height);
        glutCreateWindow("tube");
        glutInitWindowPosition(200, 200);

        if(argc == 3)
        {
            init(argv[1], argv[2]);
        }
        else
        {
            init("./data/trial7/infolog/test6info47.log", "./data/trial7/eiglog/test6eig47.log");
        }


        glutDisplayFunc(display);
        glutReshapeFunc(reshape);
        glutKeyboardFunc(key);
        glutMouseFunc(mouse);
        glutMotionFunc(motion);

        glutMainLoop();
        return 0;
}



/*#include <stdio.h>
#include <iostream>
#include <fstream>
#include <math.h>
#include <assert.h>
#include <GL/glut.h>

//#include "svrbf.h"
#include "svHyperTube.h"
#include "svCriticalPointGlyph.h"
#include "svGlyph.h"
#include "svUtil.h"
#include "ivTrackball.h"
#include "ivview.h"
#include "MGL.h"

#define IMAGE_WIDTH  512
#define IMAGE_HEIGHT 512

using namespace __svl_lib;

void reshape(int w, int h);
void display(void);
void key(unsigned char key, int x, int y);

view3d view_info;
char outimagename[256];

//svrbf *flow_field;
svHyperTube *hypertube;
svCriticalPointGlyph *glyph;

svVector3Array cord;

GLboolean bBoundingBox=true;
GLboolean bAxis;

GLuint image_width;
GLuint image_height;

GLint nx, ny, nz;
Trackball trackball;

void InitLight()
{
  //setting of lighting
  GLfloat mat_diffuse[] = { 0.8, .0, 0.0, .4};
  GLfloat mat_specular[] = { 1, 1, 1, 1 };
  GLfloat mat_shininess[] = { 20.0 };
  //GLfloat light_position[] = { 24, 24, 60, 0.0 };
  GLfloat light_position[] = { 50,50,50, 0.0 };
  GLfloat white_light[] = { 1.0, 1.0, 1.0, 1.0 };
  glClearColor(0.0, 0.0, 0.0, 0.0);
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, mat_diffuse);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
  glLightfv(GL_LIGHT0, GL_POSITION, light_position);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, white_light);
  glLightfv(GL_LIGHT0, GL_SPECULAR, white_light);
}

//***************************
// GLUT callback functions
//****************************

void reshape(int w, int h)
{

        image_width  = w;
        image_height = h;

        glViewport (0, 0, w, h);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();

        if (view_info.type == PARALLEL){
                glOrtho(view_info.left, view_info.right, view_info.bottom, view_info.top,
                        view_info.hither, view_info.yon);

                glMatrixMode(GL_MODELVIEW);
                glLoadIdentity();

        }
        else{ // perspective
                float aspect = view_info.aspect_ratio * float(w)/float(h);
                float GLfloat_fov;
                if ( aspect < 1 ){
                  // fovy is a misnomer.. we GLfloatly mean the fov applies to the
                  // smaller dimension
                  float fovx, fovy;
                  fovx = fovy = view_info.view_angle;
                  GLfloat_fov = svToDegree(2*atan(tan(svToRadian(fovx/2))/aspect));
                }
                else{
                        GLfloat_fov = view_info.view_angle;
                }

                gluPerspective(GLfloat_fov,
                                           aspect,
                                           view_info.hither,
                                           view_info.yon);

                glMatrixMode(GL_MODELVIEW);
                glLoadIdentity();
                gluLookAt(view_info.eye[0],view_info.eye[1],view_info.eye[2],
                                view_info.coi[0],view_info.coi[1],view_info.coi[2],
                                0, 1, 0);

        }

        InitLight();

}

void display(void)
{
        // set new model view and projection matrix
        glClearColor(0.3, 0.3, 0.3, 1);
//	glClear(GL_COLOR_BUFFER_BIT);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);

        glEnable(GL_COLOR_MATERIAL);
        glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

        GLfloat m[16];
        trackball.getMatrix().getValue(m);

        glPushMatrix();
        glMultMatrixf(m);

        if (bBoundingBox)
                hypertube->DrawOutline();
        //if (bAxis)
        //	draw_axis();

        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
        glEnable(GL_TEXTURE_2D);
        //glEnable(GL_BLEND);
        //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        //RenderProperty p;
        //p.color = svColor4(0.9, 0.9, 0.9, 1.);
        //hypertube->SetRenderProperty(p);
        hypertube->Render();
        glyph->Render();

        glDisable(GL_TEXTURE_2D);
        glDisable(GL_BLEND);
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_LIGHTING);
        glDisable(GL_LIGHT0);

        glPopMatrix();
        glFlush();
        glutSwapBuffers();
}


void key(unsigned char key, int x, int y)
{
        switch (key) {
    case 'f':
        case 'F':
hypertube->SaveToFile("aa.out");
                glutPostRedisplay();
                break;

    case 's':
        case 'S':
        //hypertube->SaveToFile("tube.out");
        //hypertube->SetLineLength();
                __svl_lib::WriteToPNM(image_width,image_height,outimagename);
                break;


    case 'b':
        case 'B':
                bBoundingBox = !bBoundingBox;
                glutPostRedisplay();
                break;

    case 'a':
        case 'A':
                bAxis = !bAxis;
                glutPostRedisplay();
                break;

        case '\033':
        case 'q':
        case 'Q':
                exit(0);
                break;
        }
}

void mouse(int button, int state, int x, int y)
{
        long s=0x00000000;
    s |= (button == GLUT_LEFT_BUTTON)   ? ((state == GLUT_DOWN) ? Trackball::LBUTTON_DOWN : Trackball::LBUTTON_UP) : 0;
    s |= (button == GLUT_MIDDLE_BUTTON) ? ((state == GLUT_DOWN) ? Trackball::MBUTTON_DOWN : Trackball::MBUTTON_UP) : 0;
    s |= (button == GLUT_RIGHT_BUTTON)  ? ((state == GLUT_DOWN) ? Trackball::RBUTTON_DOWN : Trackball::RBUTTON_UP) : 0;

    int key_state = glutGetModifiers();
    s |= (key_state & GLUT_ACTIVE_CTRL)  ? Trackball::CTRL_DOWN  : 0;
    s |= (key_state & GLUT_ACTIVE_ALT)   ? Trackball::ALT_DOWN   : 0;
    s |= (key_state & GLUT_ACTIVE_SHIFT) ? Trackball::SHIFT_DOWN : 0;

        if (s & Trackball::BUTTON_DOWN){
        trackball.mouseDown(s, x, y);
        }

        if (s & Trackball::BUTTON_UP){
        trackball.mouseUp(s, x, y);
        }
}

void motion(int x, int y)
{
        trackball.mouseMotion(x, y);
        glutPostRedisplay();
}

//******************************************
// initialization code for GL and NV_EXT
//******************************************

void init(char *fname, char *cpname)
{
int seed_number=120;
  hypertube = new svHyperTube(fname, seed_number,2);

        hypertube->SetTexture();
        hypertube->SetTubeRadius(0.04);
        hypertube->EnableColor();

        svVector3 lbbox = svVector3(-2,-2,-2),rbbox=svVector3(2,2,2);
        hypertube->SetBBox(lbbox,rbbox);
        hypertube->GenerateFromFile();

  glyph = new svCriticalPointGlyph(cpname);
  glyph->Generate();

  view_info.eye[0] = 2/2.0;
  view_info.eye[1] = 2/2.0;
  view_info.eye[2] = 2*2.0;

  trackball.setEye(view_info.eye);
  trackball.setFocus(svVector3(0,0,0));
  trackball.setWindowSize(IMAGE_WIDTH, IMAGE_HEIGHT);

}

//**********************
// program entry
//**********************

int main(int argc, char** argv)
{
//strcpy(outimagename,argv[3]);
strcpy(outimagename,"1.png");
  srand(12345);
        glutInit(&argc, argv);
        glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

        image_width  = IMAGE_WIDTH;
        image_height = IMAGE_HEIGHT;

        glutInitWindowSize(image_width, image_height);
        glutCreateWindow("tube");
        glutInitWindowPosition(200, 200);

        //init(argv[1], argv[2]);
        init("C:\\Users\\Hanyu\\Coding\\FAVER.simpleVisLib\\FieldVis\\data\\trial20\\vectorsout-new\\6-22-120-2.svl", "C:\\Users\\Hanyu\\Coding\\FAVER.simpleVisLib\\FieldVis\\data\\trial20\\eiglog\\mc6eig22.log");
        glutDisplayFunc(display);
        glutReshapeFunc(reshape);
        glutKeyboardFunc(key);
        glutMouseFunc(mouse);
        glutMotionFunc(motion);

        glutMainLoop();
        return 0;
}
*/
