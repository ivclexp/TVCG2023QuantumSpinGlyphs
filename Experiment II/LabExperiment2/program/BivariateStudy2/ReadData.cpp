#include "ReadData.h"

dataSet::dataSet()
{
  dataSize = 1;  
  dataPos = new svVector3[dataSize];
  dataVel = new svVector3[dataSize];
  dataDen = new svScalar[dataSize];
  //random_order.resize(dataSize);

}

void dataSet::SetData(char *file)
{
  delete [] dataPos;
  delete [] dataVel;
  delete [] dataDen;
  
  ifstream infile(file);
  
  infile>>dataSize;//cerr<<dataSize<<endl;
  
  dataPos = new svVector3[dataSize];
  dataVel = new svVector3[dataSize];
  dataDen = new svScalar[dataSize];
  random_order.resize(dataSize);

  for(int i=0;i<dataSize;i++)
  {
    infile>>dataPos[i][0]>>dataPos[i][1]>>dataPos[i][2]
    >>dataVel[i][0]>>dataVel[i][1]>>dataVel[i][2]
    >>dataDen[i];
  }
  
  infile.close();
}

void dataSet::SetRandom(int number_tasks, int number_encoding, int number_trials)
{
  	srand(time(NULL));
	int n;
	int num;

	//int k= 0;
	for(int ii=0;ii<number_tasks;ii++)
	{
	  for(int j=0;j<number_encoding;j++)
	  {
	    for(int i=0;i<number_trials;i++)
	    {
		  bool flag = true;
		  num = rand() % number_trials 
			  + ii * number_encoding * number_trials + j * number_trials;	//cerr<<k<<" "<<num<<" ";	  
		  while(flag)
		  {

		    flag =false;
		      //cerr<<num<<" ";
		      for(int k=0;k< random_order.size();k++)
		      {
			if(num == random_order[k])
			{
			  flag = true;
			  num = rand() % number_trials
			    + ii * number_encoding * number_trials + j * number_trials;//cerr<<num<<" ";
			  break;
			}
		      }
		  }
		    
		    random_order.push_back(num);
		    
		   //cerr<<num<<endl;
		    //k ++;
	    }
	  }
	}
	
}

void dataSet::SetTask(svChar *dir, svChar *inf)
{
	ifstream infile;
	char name[200];
	sprintf(name,"%s/%s",dir,inf);
	infile.open(name);
	if(!infile.is_open())
	{
		cerr<<"Can not find"<<name<<endl;
		exit(0);
	}

	int num;
	int number_data;
	int task_type;
	infile>>task_type;
	
	infile>>number_data;
	infile>>num;
	
	//TaskPoint[task_type].clear();
      //cerr<<num<<" "<<number_data<<" "<<task_type<<endl;
	for(int i=0;i<num;i++)	
	{
		vector<int>  c(number_data);
		for(int j=0;j<number_data;j++)
		{
		  infile>>c[j];
		}
		TaskPoint[task_type].push_back(c);
		
	}//cerr<<endl;

	infile.close();
}

vector<int> dataSet::GetSelectedIndex(int task, int count, int number_encoding, int number_trials)//task starts from 0
{
  vector<int> index;
  int randomi = random_order[count];
  randomi = randomi - task * number_encoding * number_trials;
  
  if(task < 3)
  for(int i=0;i<TaskPoint[task][randomi].size();i++)
  {
    index.push_back(TaskPoint[task][randomi][i]);
  }
  else
  for(int i=1;i<TaskPoint[task][randomi].size();i++)
  {
    index.push_back(TaskPoint[task][randomi][i]);
  }

  
  return index;
}

int dataSet::GetDataIndex(int count)
{
  return random_order[count];
}


void dataSet::GetDualAnswer(int task, int count, int number_encoding, int number_trials, double &a1, double &a2)
{

  
  int randomi = random_order[count];
  randomi = randomi - task * number_encoding * number_trials;
  //cerr<<count<<" "<<random_order[count]<<" "<<task<<" "<<number_encoding<<" "<<number_trials<<" "<<randomi<<endl;
  vector<int> index;  
  for(int i=0;i<TaskPoint[task][randomi].size();i++)
  {
    index.push_back(TaskPoint[task][randomi][i]);
  }  
  
  if(task == 1 || task == 2)
  {
    //cerr<<index.size()<<endl;
	a1 = dataDen[index[0]]/pow(10.,8.);
	a2 = dataDen[index[1]]/pow(10.,8.);
  }

}

double dataSet::SaveCorrect(int task, int count, int number_encoding, int number_trials)
{
  double c;
  
  int randomi = random_order[count];
  randomi = randomi - task * number_encoding * number_trials;
  //cerr<<count<<" "<<random_order[count]<<" "<<task<<" "<<number_encoding<<" "<<number_trials<<" "<<randomi<<" ";
  vector<int> index;  
if(TaskPoint[task].size() > 0)
  for(int i=0;i<TaskPoint[task][randomi].size();i++)
  {
    index.push_back(TaskPoint[task][randomi][i]);
	//if(task == 3)	
	//	cerr<<task<<" "<<randomi<<" "<<TaskPoint[task][randomi][i]<<" ";
  }  //cerr<<endl;
  
  if(index.size() == 1)
  {
    c = dataDen[index[0]]/pow(10.,8.);
  }
  else if(task == 1)
  {
    //cerr<<index.size()<<endl;
    c = fabs(dataDen[index[0]] / dataDen[index[1]]);
    if(c<1)
      c = 1./c;
  }
  else if(task == 2)
  {
    if (dataDen[index[0]] > dataDen[index[1]])
    {
      c = 0;
    }
    else
    {
      c = 1;
    }

	/*c = 0;

	for(int i=0;i<dataSize;i++)
	{
		c = c + dataDen[i];
	}
	c = c/dataSize;
	c = c/pow(10.,8.);*/
  }
  else if(task == 3)
  {
    c = dataDen[index[1]]/pow(10.,8.);//cerr<<" "<<index[1]<<endl;
  }  

  index.clear();
  return c;
}

int dataSet::GetExponentTask4(int task, int count, int number_encoding, int number_trials)
{
  int c;
  
  int randomi = random_order[count];
  randomi = randomi - task * number_encoding * number_trials;

  vector<int> index;  
  for(int i=0;i<TaskPoint[task][randomi].size();i++)
  {
    index.push_back(TaskPoint[task][randomi][i]);
  }  
  

  if(task == 3)
  {
    c = index[0];//cerr<<"c "<<c<<endl;
  }  

  index.clear();

  return c;
}

void dataSet::cleanup()
{
  delete [] dataPos;
  delete [] dataVel;
  delete [] dataDen;
  
  random_order.clear();
  for(int i=0;i<3;i++)
  {
    TaskPoint[i].clear();
  }
}
