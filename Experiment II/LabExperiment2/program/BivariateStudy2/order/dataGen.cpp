#include <fstream>
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <cmath>
#include <time.h>
#include <algorithm>

#define TOTALPOWER 5
#define POWER 5
#define PCOUNT 30
#define TASK1 18 
#define TASK2 18
#define TASK3 6
#define TASK4 24 
#define ENCODE 3
#define TRIAL 3
#define DATA 40
#define TASK 3
#define DISTRIBUTION 2
#define MINPOWER 3

using namespace std;

int totalcount = 54 + 54 + 50;
int randomArr1[54];
int randomArr2[54];
int randomArr3[50];

typedef struct Spin{
  double pos[3];
  double dir[3];
  double mag;
  double exp;
  double coe;
} Spin;

bool sortByXYZ(Spin a, Spin b){
  if(a.pos[2] < b.pos[2]) return true;
  else if(fabs(a.pos[2] - b.pos[2])<1e-3){
      if(a.pos[1] < b.pos[1]) return true;
      else if(fabs(a.pos[1]-b.pos[1])<1e-3){
         if(a.pos[0] < b.pos[0]) return true;
      }
  }
  return false;
}

bool sortByXZY(Spin a, Spin b){
  if(a.pos[2] < b.pos[2]) return true;
  else if(fabs(a.pos[2] - b.pos[2])<1e-3){
      if(a.pos[1] < b.pos[1]) return true;
      else if(fabs(a.pos[1]-b.pos[1])<1e-3){
         if(a.pos[0] < b.pos[0]) return true;
      }
  }
  return false;
}

bool sortByYZX(Spin a, Spin b){
  if(a.pos[2] < b.pos[2]) return true;
  else if(fabs(a.pos[2] - b.pos[2])<1e-3){
      if(a.pos[1] < b.pos[1]) return true;
      else if(fabs(a.pos[1]-b.pos[1])<1e-3){
         if(a.pos[0] < b.pos[0]) return true;
      }
  }
  return false;
}

bool sortByMinCorner(Spin a, Spin b){
  double corner[3]={-2.5,-1.5,-1.5};
  double d1 = (a.pos[0] - corner[0])*(a.pos[0] - corner[0])
            + (a.pos[1] - corner[1])*(a.pos[1] - corner[1])
            + (a.pos[2] - corner[2])*(a.pos[2] - corner[2]);
  double d2 = (b.pos[0] - corner[0])*(b.pos[0] - corner[0])
            + (b.pos[1] - corner[1])*(b.pos[1] - corner[1])
            + (b.pos[2] - corner[2])*(b.pos[2] - corner[2]);
  return (d1<d2);
}

bool sortByMaxCorner(Spin a, Spin b){
  double corner[3]={2.5,1.5,1.5};
  double d1 = (a.pos[0] - corner[0])*(a.pos[0] - corner[0])
            + (a.pos[1] - corner[1])*(a.pos[1] - corner[1])
            + (a.pos[2] - corner[2])*(a.pos[2] - corner[2]);
  double d2 = (b.pos[0] - corner[0])*(b.pos[0] - corner[0])
            + (b.pos[1] - corner[1])*(b.pos[1] - corner[1])
            + (b.pos[2] - corner[2])*(b.pos[2] - corner[2]);
  return (d1<d2);
}
int main(int argc, char **argv){
srand(time(NULL));
   for(int i=5;i<8;i++){
     for(int j=1;j<=40;j++){
        char name[40];
        sprintf(name, "../studydata/D%d%d",i,j); 
        vector<Spin> spin;
        ifstream datafile(name);
        int n; datafile>>n;
        if(i==7 || i==4) n = 448;
        spin.resize(n);
        for(int jj=0;jj<n;jj++){
           datafile>>spin[jj].pos[0]>>spin[jj].pos[1]>>spin[jj].pos[2]
                   >>spin[jj].dir[0]>>spin[jj].dir[1]>>spin[jj].dir[2]
                   >>spin[jj].mag   >>spin[jj].exp   >>spin[jj].coe;
        }
        datafile.close(); 
     
        if(i>4){
        int r = rand()%5;//r=0;cout<<i<<" "<<j<<" "<<r<<endl;
        switch(r){
           case 0: sort(spin.begin(), spin.end(), sortByXYZ);break;
           case 1: sort(spin.begin(), spin.end(), sortByXZY);break;
           case 2: sort(spin.begin(), spin.end(), sortByYZX);break;
           case 3: sort(spin.begin(), spin.end(), sortByMinCorner);break;
           case 4: sort(spin.begin(), spin.end(), sortByMaxCorner);break;
        }
        int currentPower = 0;
        int powerCount = spin.size()/i;
        int currentCount = 0;
        for(int jj=0;jj<spin.size();jj++){
           spin[jj].exp = currentPower;
           spin[jj].mag = pow(10., spin[jj].exp) * spin[jj].coe;
           currentCount++;
           if(currentPower < i-1 && currentCount == powerCount){
               currentPower++;
               currentCount = 0;
           } 
        }
        }
        sprintf(name, "../studydata2/D%d%d",i,j);
        ofstream outfile(name);
        sort(spin.begin(), spin.end(), sortByXYZ);
        outfile<<spin.size()<<endl;
        for(int jj=0;jj<spin.size();jj++)
           outfile<<spin[jj].pos[0]<<" "<<spin[jj].pos[1]<<" "<<spin[jj].pos[2]
                   <<" "<<spin[jj].dir[0]<<" "<<spin[jj].dir[1]<<" "<<spin[jj].dir[2]
                   <<" "<<spin[jj].mag <<" "<<  spin[jj].exp <<" "<<  spin[jj].coe<<endl;
        outfile.close();
        spin.clear(); 
      }
   }
   return 0;
}

