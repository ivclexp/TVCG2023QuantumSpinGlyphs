#include <fstream>
#include <iostream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <cmath>

using namespace std;

int main(int argc, char **argv)
{
 char str[50];

 int exp = 1;
 
 ofstream out("task4_pointorder");
 
 for(int i=1;i<2;i++)
 {
   for(int j=1;j<7;j++)
   {
     sprintf(str, "subdataset_%d.txt", 17 + j);
	
     
     ifstream infile(str);
     
     int num;
     
     infile>>num;
     
     double den = 0;
     int index;
     for(int t=0;t<num;t++)
     {
       double input;
       double tmp;
       infile>>tmp>>tmp>>tmp
       >>tmp>>tmp>>tmp>>input;

	//cerr<<log10(input/pow(10.,8.))<<" ";
       
       if(input>den && (int)log10(input/pow(10.,8.)) == exp)
       {
	 den = input;
	 index = t;
       }
     }
     
     cerr<<str<<" "<<i<<" "<<j<<" "<<index<<" "<<den<<endl;

     out<<exp<<" "<<index<<endl;
     
     infile.close();
	exp ++;
	exp = exp % 4;
   }
 }
 
 out.close();
  
 return 0; 
}
