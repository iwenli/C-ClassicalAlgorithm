// ��ָ�����.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "conio.h"
#include "time.h"

unsigned long myPow(int m,int n);
unsigned long myPow1(int m,int n);

int main(int argc, char* argv[])
{
	/*
	printf("������������m��n���������m��n�η���\n");
	int m,n;
	printf("������m��");
	scanf("%d",&m);
	printf("\n������n��");
	scanf("%d",&n);
	printf("\n��Ҫ��:%d��%d���ݵĽ���ǣ�%d\n\n",m,n,myPow(m,n));
	*/

	//�Ƚϵݹ���۳˵�ʱ�临�Ӷ�

	int t=clock();
	unsigned long num=myPow(2,10);
	printf("�ݹ鷨���2��10�η����ý��Ϊ��%d \n����ʱ��Ϊ��%d\n\n\n",num,clock()-t);

	t=clock();
	num=myPow1(2,10);
	printf("�۳˷����2��10�η����ý��Ϊ��%d \n����ʱ��Ϊ��%d\n\n\n",num,clock()-t);

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
	else if(n%2==0)  //ָ����2�ı���
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

