

#include <iostream>
#include <fstream>
#include <math.h>
#include <string.h>
#include <assert.h>
#include <vector>

#include "svType.h"
#include "svVectorMatrix.h"
#include "svArray.h"

#include "./jcUtil/MGL.h"
#include "./jcUtil/MArray.h"
#include "./jcUtil/MVectorMatrix.h"

using namespace __svl_lib;
using namespace std;

class dataSet {
public:
	dataSet();
	~dataSet(){cleanup();}
	
	void SetData(char *file);
	void SetRandom(int number_tasks, int number_encoding, int number_trials);
	void SetTask(svChar *dir, svChar *inf);
	
	vector<int> GetSelectedIndex(int task, int count, int number_encoding, int number_trials);
	//vector<int> GetSelectedIndex(int count);
	int GetDataIndex(int count);
	
	int GetExponentTask4(int task, int count, int number_encoding, int number_trials);
	void GetDualAnswer(int task, int count, int number_encoding, int number_trials, double &a1, double &a2);
	double SaveCorrect(int task, int count, int number_encoding, int number_trials);//need to be implemented
	double GetMaxAnswer(int n){return dataDen[n]/pow(10.,8.);}

	void cleanup();
private:
	svVector3 *dataPos;
	svVector3 *dataVel;	
	svScalar *dataDen;
	
	svInt dataSize;

	vector< vector<int> > TaskPoint[4]; //hard coding!!!!!!!!!!!!!!!!!!
	vector<int> random_order;
};


