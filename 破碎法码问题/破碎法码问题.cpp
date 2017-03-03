// 破碎法码问题.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "conio.h"

bool justify(int i,int j,int k,int s);
bool getWeight(int i,int j,int k,int s,int weight);
void fm();
void printNum(int _i,int _j,int _k,int _s);

int main(int argc, char* argv[])
{
	printf("可能的解有：");
	fm();
	

	getch();
	return 0;
}

void fm()
{
	int i,j,k,s;
	for(i=1;i<=40;i++)
		for(j=i+1;j<=40-i;j++)
			for(k=j+1;k<=40-i-j;k++)
				for(s=k+1;s<=40-i-j-k;s++)
					if(i+j+k+s==40)
					{
						if(justify(i,j,k,s))
						{
							printf("%d,%d,%d,%d\n",i,j,k,s);
							printf("\n\n组合如下：\n");
							printNum(i,j,s,k);
						}
					}
}

bool justify(int _i,int _j,int _k,int _s)
{
	bool flag=true;
	int weight;
	for(weight=1;weight<=40;weight++)
	{
		if(!getWeight(_i,_j,_k,_s,weight))
			flag=false;
	}

	return flag;
}
 
bool getWeight(int i,int j,int k,int s,int weight)
{
	bool flag=false;
	int x1,x2,x3,x4;
	for(x1=-1;x1<=1;x1++)
		for(x2=-1;x2<=1;x2++)
			for(x3=-1;x3<=1;x3++)
				for(x4=-1;x4<=1;x4++)
					if(x1*i+x2*j+x3*k+x4*s==weight)
						flag=true;
	return flag;	
}

void printNum(int _i,int _j,int _k,int _s)
{
	int weight;
	
	for(weight=1;weight<=40;weight++)
	{ 
		printf("重量%d:\n",weight);
		int x1,x2,x3,x4;
		for(x1=-1;x1<=1;x1++)
			for(x2=-1;x2<=1;x2++)
				for(x3=-1;x3<=1;x3++)
					for(x4=-1;x4<=1;x4++)
						if(x1*_i+x2*_j+x3*_k+x4*_s==weight)
							printf("%d*%d+%d*%d+%d*%d+%d*%d=%d\n",x1,_i,x2,_j,x3,_k,x4,_s,weight);
	}
}