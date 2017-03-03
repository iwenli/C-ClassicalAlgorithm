// 幂指数求解.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "conio.h"
#include "time.h"

unsigned long myPow(int m,int n);
unsigned long myPow1(int m,int n);

int main(int argc, char* argv[])
{
	/*
	printf("请输入两个数m和n，用来求解m的n次方：\n");
	int m,n;
	printf("请输入m：");
	scanf("%d",&m);
	printf("\n请输入n：");
	scanf("%d",&n);
	printf("\n请要求:%d的%d次幂的结果是：%d\n\n",m,n,myPow(m,n));
	*/

	//比较递归和累乘的时间复杂度

	int t=clock();
	unsigned long num=myPow(2,10);
	printf("递归法求解2的10次方所得结果为：%d \n所用时间为：%d\n\n\n",num,clock()-t);

	t=clock();
	num=myPow1(2,10);
	printf("累乘法求解2的10次方所得结果为：%d \n所用时间为：%d\n\n\n",num,clock()-t);

	getch();
	return 0;
}

unsigned long myPow(int m,int n)
{
	unsigned long temp=0;

	if(n==0)
		return 1;
	else if(n==1)
		return m;
	else if(n%2==0)  //指数是2的倍数
	{
		temp=myPow(m,n/2);
		return temp*temp;
	}
	else if(n%2 != 0)
	{
		temp=myPow(m,n-1);
		return temp*m;
	}
}

unsigned long myPow1(int m,int n)
{
	int t=n;
	long temp=1;
	while(t>0)
	{
		temp*=m;
		t--;
	}
	return temp;
}

