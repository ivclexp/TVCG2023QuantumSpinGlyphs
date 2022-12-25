#include <iostream> 
#include <fstream>
#include <string.h>
using namespace std;

int main()
{
   ifstream infile("pointorder");
   int n;
   infile>>n;
   infile>>n;
   infile>>n;

   for(int i=0;i<n;i++)
   {
         int index1, index2;
         infile>>index1>>index2;
       char str[20];
       sprintf(str, "D%d%d",i/4+1, i%4+1);
       ifstream in(str);
       int N;
       in>>N;

       double d1,d2;
       for(int j=0;j<N;j++)
        {
              double p[7];
              in>>p[0]>>p[1]>>p[2]>>p[3]>>p[4]>>p[5]>>p[6];

              if(j == index1)
                    d1 = p[6];
              if(j == index2)
                    d2 = p[6];
       } 
       if(d1>d2) cerr<<d1<<" "<<d2<<" "<<d1/d2<<endl;
       else cerr<<d2<<" "<<d1<<" "<<d2/d1<<endl;

       in.close();
   }

   infile.close();

   return 0;
}
