#include <fstream>
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <cmath>
#include <time.h>

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

typedef struct dataInfo{
  int pID; //participant;
  int powerID[3];
  int encodeID[3]; // encodeID: 1color, 2texture, 3splitvectors
  int dataID[2]; // data distribution: 1regular, 2random
  int task1Power[30];
  int task2Power[30];
  int task3Power[30];
  int task1Index[30];
  int task2Index[30];
  int task3Index[30];
  int task1X[30];
  int task2X[30];
  int task1Answer[30];
  int task2Answer[30];  
} dataInfo; 

int main(int argc, char **argv){
   dataInfo data;

   data.encodeID[0]=1;   
   data.encodeID[1]=1;   
   data.encodeID[2]=1;   

   data.dataID[0] = 1;
   data.dataID[1] = 2;
   
   int index = 34;
   for(int i=0;i<30;i++){
      data.task1Power[i] = i%5+3;
      data.task2Power[i] = i%5+3;
      data.task3Power[i] = i%5+3;
      data.task1Index[i] = index;
      data.task2Index[i] = index+1;
      data.task3Index[i] = index+2;
      data.task1X[i] = rand()%data.task1Power[i];
      data.task2X[i] = rand()%data.task2Power[i];
   }

 //Answers for task 1 and task 2
     for(int j=0;j<30;j++){
        char name[40];
        if((j/5)%2==0)//<3)//data.dataID[j%2]==1)//data[i].dataID[j] == 1)
        sprintf(name, "../studydata2/D%d%d", 
                        data.task1Power[j], data.task1Index[j]);
        else
        sprintf(name, "../studydata2/R%d%d", 
                        data.task1Power[j], data.task1Index[j]);
        ifstream datafile(name);
        int n; datafile>>n;
        vector<Spin> spin;
        spin.resize(n);
        for(int jj=0;jj<n;jj++){
           datafile>>spin[jj].pos[0]>>spin[jj].pos[1]>>spin[jj].pos[2]
                   >>spin[jj].dir[0]>>spin[jj].dir[1]>>spin[jj].dir[2]
                   >>spin[jj].mag>>spin[jj].exp>>spin[jj].coe;
        }
        datafile.close(); 

        double answer = 1;
        double x = data.task1X[j];
        int ycount = 0;
        vector<int> tmpcount;
        for(int jj=0;jj<n;jj++){
          if(fabs(x - spin[jj].exp)<1e-3){
              ycount++;
              tmpcount.push_back(jj);
          }
        }
        int r = rand()%ycount;
        data.task1Answer[j] = tmpcount[r];// spin[tmpcount[r]].mag; 
        spin.clear();
     } 
 
     for(int j=0;j<30;j++){
        char name[40];
        if((j/5)%2==0)//j%5<3)//data.dataID[j%2]==1)//data[i].dataID[0] == 1)
        sprintf(name, "../studydata2/D%d%d", 
                        data.task2Power[j], data.task2Index[j]);
        else
          sprintf(name, "../studydata2/R%d%d", 
                        data.task2Power[j], data.task2Index[j]);
        ifstream datafile(name);
        int n; datafile>>n;
        vector<Spin> spin;
        spin.resize(n);
        for(int jj=0;jj<n;jj++){
           datafile>>spin[jj].pos[0]>>spin[jj].pos[1]>>spin[jj].pos[2]
                   >>spin[jj].dir[0]>>spin[jj].dir[1]>>spin[jj].dir[2]
                   >>spin[jj].mag>>spin[jj].exp>>spin[jj].coe;
        }
        datafile.close(); 

        double answer = -1;
        double x = data.task2X[j];
        for(int jj=0;jj<n;jj++){
          if(fabs(x - spin[jj].exp)<1e-3){
              if(answer < spin[jj].mag){
                 answer = spin[jj].mag;
                 data.task2Answer[j] = jj;
              }
          }
        }
        spin.clear();
     } 

     //Print
     ofstream orderfile("order1");
     orderfile<<1<<endl;
     orderfile<<30<<endl;
     for(int j=0;j<30;j++)
         orderfile<<j/10+1<<" "<<(((j/5)%2==0)?1:2)<<" "//data.dataID[j%2]<<" "
                 <<data.task1Power[j]<<" "<<data.task1Index[j]<<" "
                 <<data.task1X[j]<<" "<<data.task1Answer[j]<<endl;
     orderfile<<30<<endl;
     for(int j=0;j<30;j++)
         orderfile<<j/10+1<<" "<<(((j/5)%2==0)?1:2)<<" "//data.dataID[j%2]<<" "
                 <<data.task2Power[j]<<" "<<data.task2Index[j]<<" "
                 <<data.task2X[j]<<" "<<data.task2Answer[j]<<endl; 
     orderfile<<30<<endl;
     for(int j=0;j<30;j++)
         orderfile<<j/10+1<<" "<<(((j/5)%2==0)?1:2)<<" "//data.dataID[j%2]<<" "
                 <<data.task3Power[j]<<" "<<data.task3Index[j]<<" "
                 <<-1<<" "<<data.task3Power[j]<<endl;

     orderfile.close();
 
      //test
      //cout<<data[i].pID<<" ";
      //for(int j=0;j<3;j++) 
      //    cout<<data[i].powerID[j]<<" ";
      //for(int j=0;j<3;j++)
      //    cout<<data[i].encodeID[j]<<" ";
      //cout<<data[i].dataID[0]<<" "<<data[i].dataID[1]<<endl;
      //power 1
   /*   for(int j=0;j<2;j++){
         for(int e=0;e<ENCODE;e++){
           int taskPowerRandom[9];
       for(int ii=0;ii<2;ii++){ 
           int count=0;
           while(powerCount[0]<TRIAL 
              || powerCount[1]<TRIAL 
              || powerCount[2]<TRIAL){
              int r = rand()%3;
              while(powerCount[r]>=TRIAL){
                  r = rand()%3;
              }
              taskPowerRandom[count] = r;
              powerCount[r]++;
              count++;
          }
          //test
          //for(int jj=0;jj<count;jj++)
          //     cout<<taskPowerRandom[jj]<<" ";cout<<endl;
         }
      }*/

   /*double digitrange[2] = {1, 9};

   char *folder = strdup(argv[1]);
   sampleSize = atoi(argv[2]);
   int count = 0;
   int powercount[9]={0,0,0,0,0,0,0,0,0};
   for(int i=0;i<sampleSize;i++)
   {
      char tmpcount[20];
      sprintf(tmpcount, "%d", i);
      string name =  string(folder) + "D" + string(tmpcount);     
      cout<<name<<endl;
      ifstream infile(name.c_str());
      int n;
      double minmag = 9e+9;
      double maxmag = -9e+9;
      vector<Spin> spin;
      infile>>n; spin.resize(n);

      int expcount[9]= {0,0,0,0,0,0,0,0,0};      
      for(int j=0;j<n;j++)
      {
         infile>>spin[j].pos[0]>>spin[j].pos[1]>>spin[j].pos[2]
               >>spin[j].dir[0]>>spin[j].dir[1]>>spin[j].dir[2]
               >>spin[j].mag>>spin[j].exp>>spin[j].coe;
         if(spin[j].exp > maxmag) maxmag = spin[j].exp;
         if(spin[j].exp < minmag) minmag = spin[j].exp;
      }
      infile.close();      

      for(int j=0;j<n;j++)
      {
        int exp = spin[j].exp - minmag;
        expcount[exp]++;
      }
      for(int j=0;j<9;j++)
         cout<<expcount[j]<<" ";
      cout<<endl;

      int power = maxmag - minmag + 1;
      powercount[power]++;

      char powerchar[20];
      sprintf(powerchar, "%d%d",power, powercount[power]);
      string powername = "powerData/D" + string(powerchar);
      //if(power==5) cout<<maxmag<<" "<<minmag<<endl;
      ofstream dataout(powername.c_str());
      //cout<<powername<<endl;
      dataout<<n<<endl;
      for(int j=0;j<n;j++)
      {
         dataout<<spin[j].pos[0]<<" "<<spin[j].pos[1]<<" "<<spin[j].pos[2]<<" "
               <<spin[j].dir[0]<<" "<<spin[j].dir[1]<<" "<<spin[j].dir[2]<<" "
               <<spin[j].mag<<" "<<spin[j].exp<<" "<<spin[j].coe<<endl;
      }
      dataout.close();
      spin.clear();       
   }*/
   return 0;
}

