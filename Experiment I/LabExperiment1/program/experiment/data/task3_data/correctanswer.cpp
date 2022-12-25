#include <fstream>
#include <iostream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <cmath>

using namespace std;

int main(int argc, char** argv)
{

	char *logfile = argv[1];

 char str[50];
 
 int pointer[20][2];

 ifstream in("pointorder");

 int n;
 in >>n;in>>n;in>>n;

 for(int i=0;i<20;i++)
 {
	in >>pointer[i][0]>>pointer[i][1];
//	cerr<<pointer[i][0]<<endl;
 }

 in.close();

 int count = 0;

 ifstream infile(logfile);

 int index;

 for(int i=1;i<6;i++)
 {
		for(int j=1;j<5;j++)
		{
			infile>>index;

			sprintf(str, "D%d%d", i,j);//index/4+1, index%4+1);cerr<<str<<endl;

			double den1, den2;

			ifstream infile(str);
			int num;
			infile>>num;
			for(int t=0;t<num;t++)
			{
				double tmp;
				double den;
				infile>>tmp>>tmp>>tmp>>tmp>>tmp>>tmp>>den;
				if(t == pointer[count][0])
					den1 = den;
			}
			infile.close();
			
			infile.open(str);
			infile>>num;
			
			for(int t=0;t<num;t++)
			{
				double tmp;
				double den;
				infile>>tmp>>tmp>>tmp>>tmp>>tmp>>tmp>>den;
				if(t == pointer[count][1])
					den2 = den;
			};

			if(den1 > den2) cerr<<0<<endl;
			else cerr<<1<<endl;

			cerr<<pointer[index][0]<<" "<<pointer[index][1]<<" "<<den1<<" "<<den2<<endl;

			infile.close();
			count ++;
		}
	}

infile.close();
  
 return 0; 
}
