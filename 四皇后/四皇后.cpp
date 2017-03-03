// 四皇后.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "conio.h"
#include "time.h"

#define I 8   //皇后数目
#define J 8


void print(int(*Q)[I]);                     //打印棋盘
void Queen(int j,int (*Q)[I]);				//递归深度优先遍历确定皇后
bool isCorrect(int i,int j,int(*Q)[I]);     //判断当前位置是否可以放置皇后   规则

int count=0;

int main(int argc, char* argv[])
{
	int Q[I][J];  
	//初始化棋盘数组
	for(int i=0;i<I;i++)
		for(int j=0;j<J;j++)
			Q[i][j]=0;
 	
	int t=clock();
	Queen(0,Q);

	printf("%d 皇后的解的个数是：%d\n",I,count);
	printf("耗时：%d ms",clock()-t);
	getch();
	return 0;
}

void Queen(int j,int (*Q)[I])
{
	int i,k;
	
	if(j==J)
	{
		//递归结束条件/*
		count++;   //解个数
		print(Q);  //打印当前状态
		printf("\n\n");

		return; //终止本次递归*/
	}

	for(i=0;i<I;i++)
	{
		if(isCorrect(i,j,Q))
		{
			Q[i][j]=1;
			Queen(j+1,Q);
			Q[i][j]=0;
		}
	}

}

bool isCorrect(int i,int j,int(*Q)[I])
{
	int m,n;
	bool flag=true;  
	//默认可以放置皇后  有冲突修改为不可放置
	//判断行
	for(m=i,n=0;n<J;n++)
		if(Q[m][n]==1 && n!=j)
			flag=false;
	//判断列
	for(m=0,n=j;m<I;m++)
		if(Q[m][n]==1 && m!=i)
			flag=false;
	//判断左上方
	for(m=i-1,n=j-1;m>=0&&n>=0;m--,n--)
		if(Q[m][n]==1)
			flag=false;
	//判断右上方
	for(m=i-1,n=j+1;m>=0&&n<J;m--,n++)
		if(Q[m][n]==1)
			flag=false;
	//左下方
	for(m=i+1,n=j-1;m<I&&n>=0;m++,n--)
		if(Q[m][n]==1)
			flag=false;
	//右下方
	for(m=i+1,n=j+1;m<I&&n<J;m++,n++)
		if(Q[m][n]==1)
			flag=false;

	return flag;
}

void print(int (*Q)[I])
{
	printf("第 %d 个解为：\n",count);
	for(int i=0;i<I;i++)
	{
		for(int j=0;j<J;j++)
		{
			printf("Q(%d,%d)=%d  ",i,j,Q[i][j]);
		}
		printf("\n");
	} 
}
