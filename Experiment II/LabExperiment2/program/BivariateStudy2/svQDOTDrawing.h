#include "svGlyph.h"
#include "CIEColor2.h"
#include <fstream>
#include <string.h>

#define DISPLAYLIST_NUM 13

using namespace std;

class svQDOTDrawing{
public:
  
  svQDOTDrawing();
  ~svQDOTDrawing();
  
  void SetData(char *file);
  int GetExponent(svScalar den);
  
  void InitLight();
  void SetTask(svChar *dir, svChar *inf);
  
  void cleanup();
  void makeTexture(svVector4 c);
  void inittexture();
  void makeVerticalTexture(int n);
  void inittexture_vertical();
  void makeHorizontalTexture(int n);
  void inittexture_horizontal();
  
  void RenderBounding();
  void Render(int list);
  
  void Arrow();

  void Double_Cylinder(GLfloat *m); //size legend1
  void Size_Size(GLfloat *m); // size legend2
  void Size_Size_Color(GLfloat *m); // size legend2
  //void Size_Brightness(); //brightness legend + size legend3
  void Hue_Size(GLfloat *m); //singlehue legend + size legend3
  void Texture_Size(GLfloat *m);
  void Hue_Texture(GLfloat *m);

  void Hue_Size_Single(GLfloat *m, int dataIndex);
  void Texture_Size_Single(GLfloat *m, int dataIndex);
  void Double_Cylinder_Single(GLfloat *m, int dataIndex);

  void RenderInnerLegend();
  void RenderDistributeLegend();
  void SetCamera(int w, int h, svVector3 e);
  void RenderMarkers(vector<int> index, GLfloat *m, int ENCODE_MODE, bool answer);
  void RenderAnswerMarker(int index, GLfloat *m, int ENCODE_MODE, bool answer);
  void RenderCorrectMarker(vector<int> index, GLfloat *m, int ENCODE_MODE);
  void ReSetDisplaylist(){dataDisplayList=0;}
  

	void Single_SizeSize(int exp, int coe);
	void Single_SizeSizeColor(int exp, int coe);
	void Single_ColorSize(int exp, int coe);
	void Single_TextureSize(int exp, int coe);
	void Single_Dual(int exp, int coe);
	void Single_ColorTexture(int exp, int coe);

        void Single_ColorSize(int exp);
        void Single_TextureSize(int exp);
        void Single_Dual(int exp);

	void ColorSize_Legend();
	void ColorTexture_Legend();
	void TextureSize_Legend();
	void SizeSize_Legend();
	void SizeSizeColor_Legend();
	void Dual_Legend();
	void Axis_Legend();
	void Font_Legend();
	void Large_Font_Legend();
	void Large_Axis_Legend(int X);

    void Texture_Legend(bool shift);
    void VerticalTexture_Legend(bool shift);
    void HorizontalTexture_Legend(bool shift);
    void Single_Texture(double n);
    void Color_Legend(int shift);
    void TwoCylinder_Legend();
    void Size_Legend();
    void Coesize_Legend();

    void RenderSelectionTest(int index);	
    int Find_Nearest(int x, int y, int encodeID, GLfloat *m); 
    int Find_Nearest( int x, int y, int width, GLfloat *tb_transform, int ENCODE_MODE);

   // void SetTransform();

    GLfloat tb_transform[4][4];

    svScalar getMag(int index){return dataDen[index];}
    svScalar getCoe(int index){return dataCoe[index];}
    svScalar getExp(int index){return dataExp[index];}
protected:

private:
  svVector3 *dataPos;
  svVector3 *dataVel;
  svScalar *dataDen;
  svScalar *dataExp;
  svScalar *dataCoe;
  svScalar maxExp;

  svVector3 rbbox;
  svVector3 lbbox;
  
  svInt dataSize;
  svInt dataDisplayList;
  svGlyph *drawing;
  
  double image_width;
  double image_height;
  svVector3 eye;
  
  vector< vector<int> > TaskPoint;
};
