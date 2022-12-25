#include <fstream>
#include <iostream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <cmath>

using namespace std;

int main()
{
 char str[50];
 
 int pointer[20][2];

 ifstream in("../task2_data/pointorder");

 int n;
 in >>n;in>>n;in>>n;

 for(int i=0;i<20;i++)
 {
	in >>pointer[i][0]>>pointer[i][1];
	cerr<<pointer[i][0]<<endl;
 }

 in.close();

 int count = 0;

 for(int i=1;i<6;i++)
 {
		for(int j=1;j<5;j++)
		{
			sprintf(str, "../task2_data/D%d%d", i, j);//cerr<<str<<endl;

			ifstream infile(str);

			int num;

			infile>>num;
			
			for(int t=0;t<num;t++)
			{
				double tmp;
				double den;
				infile>>tmp>>tmp>>tmp>>tmp>>tmp>>tmp>>den;
				if(t == pointer[count][0])
					cerr<<den<<" ";
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
					cerr<<den<<" ";
			}cerr<<endl;cerr<<endl;

			infile.close();
			count ++;
		}
	}
  
 return 0; 
}
