// ��������.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "time.h"
#include "conio.h"

//��һ�����ĵ�����
int reverseNum(int _num);
//�ж��Ƿ��ǻ�������
bool isCricle(int _num);

int main(int argc, char* argv[])
{
	long i=10;
	/*
	int t=clock();

	printf("10~9999�Ļ����������£�\n");
    while(i<9999)
	{
		if(isCricle(i))
			printf("%d\n",i);
		i++;
	}
	printf("�ܹ���ʱ��%d\n",clock()-t);
	*/
	while(true)
	{
		printf("\n\n��������Ҫ�жϵĻ������֣�");
		scanf("%d",&i);
		if(isCricle(i))
		{
			printf("�����������%d�ǻ�������.\n",i);
		}
		else
		{
			printf("���ǻ�����!\n");
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