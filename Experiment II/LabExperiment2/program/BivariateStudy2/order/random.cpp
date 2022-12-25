#include <fstream>
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <cmath>


using namespace std;

typedef struct Spin{
  double pos[3];
  double dir[3];
  double mag;
  double exp;
  double coe;
} Spin;

double box[3]={
  6,
  4,
  4,
};


int sampleSize = 99;//100;

int main(int argc, char **argv)
{
   double digitrange[2] = {1, 9};

  // char *folder = strdup(argv[1]);
   int count = 0;
   for(int i=3;i<=7;i++)
   {
     for(int jj=1;jj<40;jj++)
     {
      char powercount[20];
      char tmpcount[20];
      sprintf(powercount, "%d",i);
      sprintf(tmpcount, "%d", jj);
      string name = "../studydata2/D" + string(powercount) + string(tmpcount);
      string randomname = "../studydata2/R" + string(powercount)+string(tmpcount);

      ifstream infile(name.c_str());
      int n;
      vector<Spin> spin;
      infile>>n; spin.resize(n);
      vector<int> index;
      srand(1);
      for(int j=0;j<n;j++)
      {
         infile>>spin[j].pos[0]>>spin[j].pos[1]>>spin[j].pos[2]
               >>spin[j].dir[0]>>spin[j].dir[1]>>spin[j].dir[2]
               >>spin[j].mag>>spin[j].exp>>spin[j].coe;
         bool exist = true;
         int in;
         while(exist)
         {
           exist = false;
           in =rand()%spin.size();
           for(int t=0;t<index.size();t++)
           {
              if(in == index[t])
              { exist = true;break;}
           }
         } 
         index.push_back(in);
      }
      infile.close();      

      ofstream dataout(randomname.c_str());
      cout<<randomname<<endl;
      dataout<<n<<endl;
      for(int j=0;j<n;j++)
      {
         int in = index[j]; //cout<<j<<" ";
         dataout<<spin[j].pos[0]<<" "<<spin[j].pos[1]<<" "<<spin[j].pos[2]<<" "
               <<spin[in].dir[0]<<" "<<spin[in].dir[1]<<" "<<spin[in].dir[2]<<" "
               <<spin[in].mag<<" "<<spin[in].exp<<" "<<spin[in].coe<<endl;
      }
      dataout.close();
      spin.clear();       
      index.clear();
     }
   }
}

