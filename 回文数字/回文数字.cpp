// 回文数字.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "time.h"
#include "conio.h"

//求一个数的倒置数
int reverseNum(int _num);
//判断是否是回文数字
bool isCricle(int _num);

int main(int argc, char* argv[])
{
	long i=10;
	/*
	int t=clock();

	printf("10~9999的回文数字如下：\n");
    while(i<9999)
	{
		if(isCricle(i))
			printf("%d\n",i);
		i++;
	}
	printf("总共耗时：%d\n",clock()-t);
	*/
	while(true)
	{
		printf("\n\n请输入您要判断的回文数字：");
		scanf("%d",&i);
		if(isCricle(i))
		{
			printf("您输入的数字%d是回文数字.\n",i);
		}
		else
		{
			printf("不是回文数!\n");
		}
	}
	return 0;
}

bool isCricle(int _num)
{
	int m=_num;
	bool flag=false;

	if(m == reverseNum(m))
		flag=true;
	else
		flag=false;

	return flag;
}

int reverseNum(int _num)
{
	int m,n=0;
	m=_num;
	
	while(m)
	{
		n=n * 10 + m % 10;
		m /= 10;
	}

	return n;
}