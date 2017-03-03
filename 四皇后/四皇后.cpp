// �Ļʺ�.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "conio.h"
#include "time.h"

#define I 8   //�ʺ���Ŀ
#define J 8


void print(int(*Q)[I]);                     //��ӡ����
void Queen(int j,int (*Q)[I]);				//�ݹ�������ȱ���ȷ���ʺ�
bool isCorrect(int i,int j,int(*Q)[I]);     //�жϵ�ǰλ���Ƿ���Է��ûʺ�   ����

int count=0;

int main(int argc, char* argv[])
{
	int Q[I][J];  
	//��ʼ����������
	for(int i=0;i<I;i++)
		for(int j=0;j<J;j++)
			Q[i][j]=0;
 	
	int t=clock();
	Queen(0,Q);

	printf("%d �ʺ�Ľ�ĸ����ǣ�%d\n",I,count);
	printf("��ʱ��%d ms",clock()-t);
	getch();
	return 0;
}

void Queen(int j,int (*Q)[I])
{
	int i,k;
	
	if(j==J)
	{
		//�ݹ��������/*
		count++;   //�����
		print(Q);  //��ӡ��ǰ״̬
		printf("\n\n");

		return; //��ֹ���εݹ�*/
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
	//Ĭ�Ͽ��Է��ûʺ�  �г�ͻ�޸�Ϊ���ɷ���
	//�ж���
	for(m=i,n=0;n<J;n++)
		if(Q[m][n]==1 && n!=j)
			flag=false;
	//�ж���
	for(m=0,n=j;m<I;m++)
		if(Q[m][n]==1 && m!=i)
			flag=false;
	//�ж����Ϸ�
	for(m=i-1,n=j-1;m>=0&&n>=0;m--,n--)
		if(Q[m][n]==1)
			flag=false;
	//�ж����Ϸ�
	for(m=i-1,n=j+1;m>=0&&n<J;m--,n++)
		if(Q[m][n]==1)
			flag=false;
	//���·�
	for(m=i+1,n=j-1;m<I&&n>=0;m++,n--)
		if(Q[m][n]==1)
			flag=false;
	//���·�
	for(m=i+1,n=j+1;m<I&&n<J;m++,n++)
		if(Q[m][n]==1)
			flag=false;

	return flag;
}

void print(int (*Q)[I])
{
	printf("�� %d ����Ϊ��\n",count);
	for(int i=0;i<I;i++)
	{
		for(int j=0;j<J;j++)
		{
			printf("Q(%d,%d)=%d  ",i,j,Q[i][j]);
		}
		printf("\n");
	} 
}
