#include"stdio.h"
#include"conio.h"  //getch()
#include"windows.h"
#include"time.h"

#include"mmsystem.h"
#pragma comment(lib,"winmm.lib")

void control2();

//�л���Ϸ��
int GAME;   //��¼��ʼʱ��Ϸ���룡1������˹���� 2�������� 3��̰�Գ�
bool GAME1=1,GAME2=1,GAME3=1;  //��Ϸ�����жϣ�Ϊ1��ʾ���� 
char K;  //���ϼ�key������Ϸ�Ƴ�
char char1[1001],char2[1001];//��Ϸ����40��2y5,��һ���ַ�����β��


//����
HANDLE hconsole=GetStdHandle(STD_OUTPUT_HANDLE);  //��ȡ����̨��׼���    behind������̨�� [bi'haind] 
int score1=0,score2=0,score3=0;  //��Ϸ������¼
int grade1=1,grade2=1,grade3=1;  //��¼��Ϸ�ȼ�
int pause_time=0;  //��¼��ͣ��ʱ


//����˹����
int x,y; //ȫ���������
int flag1[40][25];  //��Ϸ��ͼ��־
char k;  //���ڽ��ռ����ַ�
int message1=rand()%19;  //��create_brick��remind���ݲ���
int message2;  //��remind������erase_remind�������ݲ���
int status;  //��ǰ����״̬��־����crase,draw,position�������ݲ���
int interval1=1000;  //������������ʱ��
int begin_time;  //��¼��Ϸ��ʼʱ��
int game_time=0;  //��¼��Ϸ��ʱ
int save_time=0;  //������Ϸ��ʱ
int line_number;  //��¼����������check_lined�����ķ���ֵ
int Y1;  //��¼�״ο�������
int Y2[4];  //��¼����������
bool game_over;  //�ж���Ϸ�Ƿ������Ϊ1��ʾ����
char save1[1001];  //������Ϸ��

//̰�Գ�תר�б���
int flag2[40][25];  //��ͼ��־
int head_x=32,head_y=12;  //��ͷ����
int tail_x,tail_y;  //��β����
int food_x,food_y;  //ʳ������
int food[40][25];   //ʳ���־��ֵΪ1��ʳ�����
int steps[20000];   //��¼��ͷ������0�����ϣ�1������2�����ң�3������
int snake_length=3; //��¼��ǰ�泤
int food_HP;        //�ж�ʳ���Ƿ���м�Ѫ����
int food_time=0;    //��¼ʳ�����ʱ��
bool Food;          //ʳ���־��Ϊ0ʱ��ʾʳ����
int HP=100;           //��¼��ǰѪֵ
int interval2=250;  //ʱ������������ƶ��ٶ�
int Oribehind=2;    //ԭ�������������ʼ�����й�
int Curbehind=2;    //��ǰ�����
int Nstep=0;        //��¼��ͷ�ߵĲ���
char now_key=75;    //��¼��һ�ΰ�����ʹ����԰���һ���Զ��ж�
   /*
                    /^\/^\
                  _|__|  O|
         \/     /~     \_/ \
          \____|__________/  \
                 \_______      \
                         `\     \                 \
                           |     |                  \
                          /      /                    \
                         /     /                       \\
                       /      /                         \ \
                     /     /                            \  \
                    /     /             _----_            \   \
                   /     /           _-~      ~-_         |   |
                  (      (        _-~    _--_    ~-_     _/   |
                   \      ~-____-~    _-~    ~-_    ~-_-~    /
                     ~-_           _-~          ~-_       _-~   
                        ~--______-~                ~-___-~      */


void m0()  //�رձ�������
{
	PlaySound(NULL,0,SND_FILENAME);
}

void m1()
{
	PlaySound("music\/play.wav",0,SND_FILENAME|SND_ASYNC|SND_LOOP);
}

void m2()
{
	PlaySound("music\/game.wav",0,SND_FILENAME|SND_ASYNC|SND_LOOP);
}
 
//��Ϸ��ͼ{��0,0��--��38,24��}
void gotoxy(int x,int y)  //���ƹ��λ��
{
	COORD coord;
	coord.X=x;
	coord.Y=y;
	SetConsoleCursorPosition(hconsole,coord);   //cursor  n. ������ߵģ��αָ꣬�룻[��]���
                                        //position n. λ�ã���λ��ְλ����������̬��վλ��vt. ���ã��ѡ��������ʵ�λ��
} 

void key_KF()  //���̼���ת��  ��Ҫ��˫����
{
  char k1=0;
  char k2=0;
  
  gotoxy(6,5);
  printf("�밴��(Esc���˳�)...");
  for(;;)
  {
	  k1=getch();
      if(k1<=0)
	  {
	     k2=getch();
	  }
      //===�������=====
      gotoxy(15,7);
      printf("         ");
      gotoxy(15,9);
      printf("        ");
      //=================
      gotoxy(6,7);
      printf("��һ����:%d",k1);
      gotoxy(6,9);
      printf("�ڶ�����:%d",k2);
      if(27==k1)
	  {
	     break;
	  }
  }
}


void hide_cursor()   //���ع��
{
	CONSOLE_CURSOR_INFO cursor_info={1,0};
	SetConsoleCursorInfo(hconsole,&cursor_info);
}


void settextcolor(int i) //�����ı���ɫ
{
	SetConsoleTextAttribute(hconsole,i);    //attribute n. ���ԣ�����	
}

void getscreen(char Char[])  //��ȡ��Ļ�ַ�
{
	COORD coord1,coord2;//�������������
	SMALL_RECT rect={0,0,39,24};//��ȡ��Χ
    CHAR_INFO Buffer[1000];//40x25
	coord1.X=40;
	coord1.Y=25;
	coord2.X=0;
	coord2.Y=0;
	//
	ReadConsoleOutput(hconsole,Buffer,coord1,coord2,&rect);
	for(int i=0;i<1000;i++)
	{
		Char[i]=Buffer[i].Char.AsciiChar;
	}
	Char[i]='\0';
}


void show()    //��Ϣ��ʾ����
{
		//=======��ʾ��Ϣ=======
    settextcolor(240);
	gotoxy(1,0);
	printf("1=���� 2=���� F1=���� F2=��ͣ F5=�л�");
	//===============save����============
	for(int i=2;i<25;i++)
	{
		gotoxy(27,i);            
    	printf("            ");  //12���ո�  
	}
    
	gotoxy(27,3);
	printf("��һ���飺");
	
	gotoxy(27,10);
	printf("�ȼ���");
	gotoxy(27,11);
	printf("�÷֣�");
	
	settextcolor(242);
	for(int y=5;y<9;y++)  //��ʾ����ǽ��
	{
		gotoxy(27,y);
		printf("�U");
		gotoxy(37,y);
		printf("�U");
	}
    for(y=29;y<37;y+=2)
	{
		gotoxy(y,4);
		printf("�T");
		gotoxy(y,9);
		printf("�T");
	}
	//�ĸ���
	gotoxy(27,4);
	printf("�X");
	gotoxy(37,4); 
	printf("�[");
	gotoxy(27,9);
	printf("�^");
	gotoxy(37,9);
	printf("�a");
    
	gotoxy(27,12);
	printf("������������");
	gotoxy(27,14);
	printf("������������");
    //=====����ǽ��=====
	settextcolor(40);
	for(y=1;y<=24;y++)
	{
		/*/==========//
		gotoxy(40,y);
		printf(" ");
		gotoxy(65,y);
		printf(" ");
		//==========/*/
		gotoxy(0,y);
		flag1[-1][y]=1;
		printf(" ");
		gotoxy(25,y);
		flag1[25][y]=1;
		printf(" ");

		flag1[y][24]=1;
	}
	gotoxy(1,24);
	printf("                        ");//24���ո�
	/*gotoxy(41,24);
	printf("                        ");//24���ո�*/
}

//============����Ī���ո�============
void skip()   //��ת
{
	settextcolor(247);
	gotoxy(27,24);
	printf(" ");
}


void print1()//��ӡ�÷���ȼ�
{
	if(grade1>7)
	{
		settextcolor(240);
	}
    else
	{
		settextcolor(247+grade1);   //���ݵȼ���ͬ��ʾ��ͬ��ɫ����
	}
	gotoxy(33,10);
	printf("%d",grade1);
	gotoxy(33,11);
	printf("%d",score1);

	//�˴��в���Ū���ף�Ϊ����ת֮������6���ո�ֻΪ�ô���֮
//	settextcolor(247);
//	gotoxy(27,24);
//	printf(" ");
// 	skip();
}
/*
void print2()//������Ϸʱʹ��
{
	settextcolor(240);
	int x,y;
	for(x=1;x<24;x+=2)
		for(y=0;y<24;y++)
		{
			if(flag1[x][y]>0)
			{
			   gotoxy(40+x,y);
			   printf("��");
			   //printf("%d",flag1[x][y]);
			}
			else
			{
				gotoxy(40+x,y);
				printf(" ");
			}
		}
}
*/
void print3()  //�������ʱ�ı���Ϸ������ɫ
{
	settextcolor(240);
	int x,y;
	for(x=1;x<24;x+=2)
		for(y=1;y<24;y++)
		{
			if(flag1[x][y]>0)
			{
			   gotoxy(x,y);
			   printf("��");
			   //printf("%d",flag1[x][y]);
			}
			else
			{
               gotoxy(x,y);
			   printf("  ");
			}
		}
}
//=====================================================================

void Save1()  //����
{
//	getscreen(save1);
	FILE* fp;
	fp=fopen("C:\\ZhangYuLong_elsfk.dat","wb");
	int* buffer[9];//���������ֵ
//    fwrite(save1,sizeof(save1),1,fp);//������д���ļ�
	fwrite(flag1,sizeof(flag1),1,fp);
    //  for(int i=0;i<8;i++)     //Ϊʲô����ֱ����ѭ�����ٿռ�
	//  {
	//   	buffer[i]=(int*)malloc(4);//���ٿռ�
	//  }
	buffer[0]=(int*)malloc(4);
	*buffer[0]=score1;//����
	fwrite(buffer[0],4,1,fp);
	buffer[1]=(int*)malloc(4);
	*buffer[1]=grade1;//�ȼ�
	fwrite(buffer[1],4,1,fp);
	buffer[2]=(int*)malloc(4);
	*buffer[2]=interval1;//���������ٶ�
	fwrite(buffer[2],4,1,fp);
	buffer[3]=(int*)malloc(4);
	*buffer[3]=x;//��ǰ���������
	fwrite(buffer[3],4,1,fp);
	buffer[4]=(int*)malloc(4);
	*buffer[4]=y;//.......������
	fwrite(buffer[4],4,1,fp);
	buffer[5]=(int*)malloc(4);
	*buffer[5]=status;//��ǰ�����־
	fwrite(buffer[5],4,1,fp);
	buffer[6]=(int*)malloc(4);
	*buffer[6]=message1;//��һ�����־
	fwrite(buffer[6],4,1,fp);
	buffer[7]=(int*)malloc(4);
	*buffer[7]=message2;//��ʾ������־
	fwrite(buffer[7],4,1,fp);
	buffer[8]=(int*)malloc(4);
	*buffer[8]=game_time+save_time;//��Ϸ��ʱ
	fwrite(buffer[8],4,1,fp);
	for(int i=0;i<=8;i++)
	{
		free(buffer[i]);//�ͷſռ�
	}
	fclose(fp);
}


void Load1()
{
	system("cls");
	FILE *fp;
	int *buffer[9];
	if(NULL==(fp=fopen("C:\\ZhangYuLong_elsfk.dat","rb"))) //�ж��Ƿ������Ϸ����
	{
		gotoxy(9,10);
		printf("��Ϸ���Ȳ����ڣ�\n\n\n\n       ��������˳�����.......");
		getch();
		exit(0);
	}
	//���ڣ����������
	//fread(save1,sizeof(save1),1,fp);
	settextcolor(240);
//	printf("%s",save1);
	show();
    fread(flag1,sizeof(flag1),1,fp);
	print3();
    buffer[0]=(int*)malloc(4);
	fread(buffer[0],4,1,fp);
    score1=*buffer[0];
	buffer[1]=(int*)malloc(4);
	fread(buffer[1],4,1,fp);
	grade1=*buffer[1];
	buffer[2]=(int*)malloc(4);
	fread(buffer[2],4,1,fp);
	interval1=*buffer[2];
	buffer[3]=(int*)malloc(4);
	fread(buffer[3],4,1,fp);
	x=*buffer[3];
	buffer[4]=(int*)malloc(4);
	fread(buffer[4],4,1,fp);
	y=*buffer[4]-1;
    buffer[5]=(int*)malloc(4);
	fread(buffer[5],4,1,fp);
	status=*buffer[5];
	buffer[6]=(int*)malloc(4);
	fread(buffer[6],4,1,fp);
	message1=*buffer[6];
	buffer[7]=(int*)malloc(4);
	fread(buffer[7],4,1,fp);
	message2=*buffer[7];
	buffer[8]=(int*)malloc(4);
	fread(buffer[8],4,1,fp);
	save_time=*buffer[8];
	begin_time=clock();
	print1();  //��ӡ�÷�
	for(int i=8;i>=0;i--)
	{
		free(buffer[i]);
	}
	fclose(fp);
}	
	
//=============================����������==============================

//=====================================================================
//=====================����� ���� ����غ���========================== 
void draw0()                          //������
{                                     //��
	settextcolor(249);
	gotoxy(x-2,y);
	printf("������");
	gotoxy(x-2,y+1);
	printf("��");
}
void draw0(int x,int y)                          //������
{                                                //��
	settextcolor(249);
	gotoxy(x-2,y);
	printf("������");
	gotoxy(x-2,y+1);
	printf("��");
}

void draw1()
{
	settextcolor(249);
	gotoxy(x-2,y-1);
	printf("����");                   //����
	gotoxy(x,y);                      //  ��
	printf("��");                     //  ��
	gotoxy(x,y+1);
	printf("��");
}
void draw1(int x,int y)
{
	settextcolor(249);
	gotoxy(x-2,y-1);
	printf("����");                   //����
	gotoxy(x,y);                      //  ��
	printf("��");                     //  ��
	gotoxy(x,y+1);
	printf("��");
}

void draw2()
{
	settextcolor(249);              //    ��
	gotoxy(x+2,y-1);                //������
	printf("��");
	gotoxy(x-2,y);
	printf("������");
}
void draw2(int x,int y)
{
	settextcolor(249);              //    ��
	gotoxy(x+2,y-1);                //������
	printf("��");
	gotoxy(x-2,y);
	printf("������");
}

void draw3()
{
	settextcolor(249);                  //��
	gotoxy(x,y-1);                      //��
	printf("��");                       //����
	gotoxy(x,y);                
	printf("��");
	gotoxy(x,y+1);
	printf("����");
}
void draw3(int x,int y)
{
	settextcolor(249);                  //��
	gotoxy(x,y-1);                      //��
	printf("��");                       //����
	gotoxy(x,y);                
	printf("��");
	gotoxy(x,y+1);
	printf("����");
}

void draw4()
{
	settextcolor(250);                   
	gotoxy(x-2,y);                      //����
	printf("����");                     //  ���� 
	gotoxy(x,y+1);
	printf("����");
}
void draw4(int x,int y)
{
	settextcolor(250);                   
	gotoxy(x-2,y);                      //����
	printf("����");                     //  ���� 
	gotoxy(x,y+1);
	printf("����");
}

void draw5()
{
	settextcolor(250);                //  ��
	gotoxy(x,y-1);                    //����
	printf("��");                     //��
	gotoxy(x-2,y);                
	printf("����");
	gotoxy(x-2,y+1);
	printf("��");
}
void draw5(int x,int y)
{
	settextcolor(250);                //  ��
	gotoxy(x,y-1);                    //����
	printf("��");                     //��
	gotoxy(x-2,y);                
	printf("����");
	gotoxy(x-2,y+1);
	printf("��");
}

void draw6()
{
	settextcolor(246);                   
	gotoxy(x-2,y-1);                    //�� 
	printf("��");                       //������
	gotoxy(x-2,y);                
	printf("������"); 
}
void draw6(int x,int y)
{
	settextcolor(246);                   
	gotoxy(x-2,y-1);                    //�� 
	printf("��");                       //������
	gotoxy(x-2,y);                
	printf("������"); 
}

void draw7()
{
	settextcolor(246);                  //����
	gotoxy(x,y-1);                      //��
	printf("����");                     //��
	gotoxy(x,y);                
	printf("��");
	gotoxy(x,y+1);
	printf("��");
}
void draw7(int x,int y)
{
	settextcolor(246);                  //����
	gotoxy(x,y-1);                      //��
	printf("����");                     //��
	gotoxy(x,y);                
	printf("��");
	gotoxy(x,y+1);
	printf("��");
}

void draw8()
{
	settextcolor(246);                   
	gotoxy(x-2,y);                     
	printf("������");                   //������
	gotoxy(x+2,y+1);                    //    ��
	printf("��"); 
}
void draw8(int x,int y)
{
	settextcolor(246);                   
	gotoxy(x-2,y);                     
	printf("������");                   //������
	gotoxy(x+2,y+1);                    //    ��
	printf("��"); 
}

void draw9()
{
	settextcolor(246);                  //  ��
	gotoxy(x,y-1);                      //  ��
	printf("��");                       //����
	gotoxy(x,y);                
	printf("��");
	gotoxy(x-2,y+1);
	printf("����");
}
void draw9(int x,int y)
{
	settextcolor(246);                  //  ��
	gotoxy(x,y-1);                      //  ��
	printf("��");                       //����
	gotoxy(x,y);                
	printf("��");
	gotoxy(x-2,y+1);
	printf("����");
}

void draw10()
{
	settextcolor(253);                   
	gotoxy(x,y);                       //  ����
	printf("����");                    //���� 
	gotoxy(x-2,y+1);
	printf("����");
}
void draw10(int x,int y)
{
	settextcolor(253);                   
	gotoxy(x,y);                       //  ����
	printf("����");                    //���� 
	gotoxy(x-2,y+1);
	printf("����");
}

void draw11()
{
	settextcolor(253);                  //��
	gotoxy(x-2,y-1);                    //����
	printf("��");                       //  ��
	gotoxy(x-2,y);                
	printf("����");
	gotoxy(x,y+1);
	printf("��");
}
void draw11(int x,int y)
{
	settextcolor(253);                  //��
	gotoxy(x-2,y-1);                    //����
	printf("��");                       //  ��
	gotoxy(x-2,y);                
	printf("����");
	gotoxy(x,y+1);
	printf("��");
}

void draw12()
{
	settextcolor(251);                   
	gotoxy(x-2,y);                     
	printf("������");                   //������
	gotoxy(x,y+1);                      //  ��
	printf("��"); 
}
void draw12(int x,int y)
{
	settextcolor(251);                   
	gotoxy(x-2,y);                     
	printf("������");                   //������
	gotoxy(x,y+1);                      //  ��
	printf("��"); 
}

void draw13()
{
	settextcolor(251);                //  ��
	gotoxy(x,y-1);                    //����
	printf("��");                     //  ��
	gotoxy(x-2,y);                
	printf("����");
	gotoxy(x,y+1);
	printf("��");
}
void draw13(int x,int y)
{
	settextcolor(251);                //  ��
	gotoxy(x,y-1);                    //����
	printf("��");                     //  ��
	gotoxy(x-2,y);                
	printf("����");
	gotoxy(x,y+1);
	printf("��");
}

void draw14()
{
	settextcolor(251);                   
	gotoxy(x,y-1);                  //  ��
	printf("��");                   //������
	gotoxy(x-2,y);                       
	printf("������"); 
}
void draw14(int x,int y)
{
	settextcolor(251);                   
	gotoxy(x,y-1);                  //  ��
	printf("��");                   //������
	gotoxy(x-2,y);                       
	printf("������"); 
}

void draw15()
{
	settextcolor(251);                  //��
	gotoxy(x,y-1);                      //����
	printf("��");                       //��
	gotoxy(x,y);                
	printf("����");
	gotoxy(x,y+1);
	printf("��");
}
void draw15(int x,int y)
{
	settextcolor(251);                  //��
	gotoxy(x,y-1);                      //����
	printf("��");                       //��
	gotoxy(x,y);                
	printf("����");
	gotoxy(x,y+1);
	printf("��");
}

void draw16()
{
	settextcolor(240);                   
	gotoxy(x,y);                     //  ����
	printf("����");                  //  ����
	gotoxy(x,y+1);                       
	printf("����"); 
}
void draw16(int x,int y)
{
	settextcolor(240);                   
	gotoxy(x,y);                     //  ����
	printf("����");                  //  ����
	gotoxy(x,y+1);                       
	printf("����"); 
}


void draw17()
{
	settextcolor(252);                  
	gotoxy(x-2,y);                      //  ��������
	printf("��������");                       
}
void draw17(int x,int y)
{
	settextcolor(252);                  
	gotoxy(x-2,y);                      //  ��������
	printf("��������");                       
}

void draw18()
{
	settextcolor(252);                  //��
	gotoxy(x,y-1);                      //��
	printf("��");                       //��
	gotoxy(x,y);                        //��
	printf("��");
	gotoxy(x,y+1);
	printf("��");
	gotoxy(x,y+2);
	printf("��");
}
void draw18(int x,int y)
{
	settextcolor(252);                  //��
	gotoxy(x,y-1);                      //��
	printf("��");                       //��
	gotoxy(x,y);                        //��
	printf("��");
	gotoxy(x,y+1);
	printf("��");
	gotoxy(x,y+2);
	printf("��");
}


void create_brick()
{
	x=11,y=2;
	switch(message1)
	{
	case 0:   
		draw0();
		status=message1;
		break;
	case 1:   
		draw1();   
		status=message1;
		break;
	case 2:   
		draw2();   
		status=message1;
		break;
	case 3:   
		draw3();   
		status=message1;
		break;
	case 4:   
		draw4();   
		status=message1;
		break;
	case 5:   
		draw5();   
		status=message1;
		break;
	case 6:   
		draw6();   
		status=message1;
		break;
	case 7:   
		draw7();   
		status=message1;
		break;
	case 8:   
		draw8();   
		status=message1;
		break;
	case 9:   
		draw9();   
		status=message1;
		break;
	case 10:   
		draw10();   
		status=message1;
		break;
	case 11:   
		draw11();
		status=message1;
		break;
	case 12:   
		draw12();   
		status=message1;
		break;
	case 13:   
		draw13();   
		status=message1;
		break;
	case 14:   
		draw14();   
		status=message1;
		break;
	case 15:   
		draw15();   
		status=message1;
		break;
	case 16:   
		draw16();   
		status=message1;
		break;
	case 17:   
		draw17();   
		status=message1;
		break;
	case 18:   
		draw18();   
		status=message1;
		break;
	default:
		break;
	}
	message1=rand()%19;  //��ȡ��һ����
}


void remind()
{
	int x=31,y=6;
	switch(message1)
	{
	case 0:   
		draw0(x,y);
		 message2=message1;
		break;
	case 1:   
		draw1(x,y);   
		message2=message1;
		break;
	case 2:   
		draw2(x,y);   
		message2=message1;
		break;
	case 3:   
		draw3(x,y);   
		message2=message1;
		break;
	case 4:   
		draw4(x,y);   
		message2=message1;
		break;
	case 5:   
		draw5(x,y);   
		message2=message1;
		break;
	case 6:   
		draw6(x,y);   
		message2=message1;
		break;
	case 7:   
		draw7(x,y);   
		message2=message1;
		break;
	case 8:   
		draw8(x,y);   
		message2=message1;
		break;
	case 9:   
		draw9(x,y);   
		message2=message1;
		break;
	case 10:   
		draw10(x,y);   
		message2=message1;
		break;
	case 11:   
		draw11(x,y);
		message2=message1;
		break;
	case 12:   
		draw12(x,y);   
		message2=message1;
		break;
	case 13:   
		draw13(x,y);   
		message2=message1;
		break;
	case 14:   
		draw14(x,y);   
		message2=message1;
		break;
	case 15:   
		draw15(x,y);   
		message2=message1;
		break;
	case 16:   
		draw16(x,y);   
		message2=message1;
		break;
	case 17:   
		draw17(x,y);   
		message2=message1;
		break;
	case 18:   
		draw18(x,y);   
		message2=message1;
		break;
	default:
		break;
	}
}

void draw()
{
	switch(status)
	{
	case 0:   
		draw0();   
		break;
	case 1:   
		draw1();   
		break;
	case 2:   
		draw2();   
		break;
	case 3:   
		draw3();   
		break;
	case 4:   
		draw4();   
		break;
	case 5:   
		draw5();   
		break;
	case 6:   
		draw6();   
		break;
	case 7:   
		draw7();   
		break;
	case 8:   
		draw8();   
		break;
	case 9:   
		draw9();   
		break;
	case 10:   
		draw10();   
		break;
	case 11:   
		draw11();   
		break;
	case 12:   
		draw12();   
		break;
	case 13:   
		draw13();   
		break;
	case 14:   
		draw14();   
		break;
	case 15:   
		draw15();   
		break;
	case 16:   
		draw16();   
		break;
	case 17:   
		draw17();   
		break;
	case 18:   
		draw18();   
		break;
	default:
		break;
	}
//	skip();
}
//=====================================================================
//=====================����� ���� ����غ���==========================
void erase0()                          //������
{                                      //��
	gotoxy(x-2,y);
	printf("      ");
	gotoxy(x-2,y+1);
	printf("  ");
}
void erase0(int x,int y)                          //������
{                                                 //��
	gotoxy(x-2,y);
	printf("      ");
	gotoxy(x-2,y+1);
	printf("  ");
}

void erase1()
{ 
	gotoxy(x-2,y-1);
	printf("    ");                   //����
	gotoxy(x,y);                      //  ��
	printf("  ");                     //  ��
	gotoxy(x,y+1);
	printf("  ");
}
void erase1(int x,int y)
{
	gotoxy(x-2,y-1);
	printf("    ");                   //����
	gotoxy(x,y);                      //  ��
	printf("  ");                     //  ��
	gotoxy(x,y+1);
	printf("  ");
}

void erase2()
{
        gotoxy(x+2,y-1);                            //    ��
	printf("  ");                               //������
	gotoxy(x-2,y);                               
	printf("      ");
}
void erase2(int x,int y)
{
	gotoxy(x+2,y-1);                            //    ��
	printf("  ");                               //������
	gotoxy(x-2,y);                               
	printf("      ");
}

void erase3()
{                   
	gotoxy(x,y-1);                      //��
	printf("  ");                       //��
	gotoxy(x,y);                        //����
	printf("  ");
	gotoxy(x,y+1);
	printf("    ");
}
void erase3(int x,int y)
{
	gotoxy(x,y-1);                      //��
	printf("  ");                       //��
	gotoxy(x,y);                        //����
	printf("  ");
	gotoxy(x,y+1);
	printf("    ");
}

void erase4()
{
	gotoxy(x-2,y);                      //����
	printf("    ");                     //  ���� 
	gotoxy(x,y+1);
	printf("    ");
}
void erase4(int x,int y)
{                   
	gotoxy(x-2,y);                      //����
	printf("    ");                     //  ���� 
	gotoxy(x,y+1);
	printf("    ");
}

void erase5()
{               
	gotoxy(x,y-1);                    //  �� 
	printf("  ");                     //����	
        gotoxy(x-2,y);                    //��
	printf("    ");
	gotoxy(x-2,y+1);
	printf("  ");
}
void erase5(int x,int y)
{
	gotoxy(x,y-1);                    //  �� 
	printf("  ");                     //����	
        gotoxy(x-2,y);                    //��
	printf("    ");
	gotoxy(x-2,y+1);
	printf("  ");
}

void erase6()
{                   
	gotoxy(x-2,y-1);                    //�� 
	printf("  ");                       //������
	gotoxy(x-2,y);                
	printf("      "); 
}
void erase6(int x,int y)
{
	gotoxy(x-2,y-1);                    //�� 
	printf("  ");                       //������
	gotoxy(x-2,y);                
	printf("      "); 
}

void erase7()
{
	gotoxy(x,y-1);                      //����
	printf("    ");                     //��
	gotoxy(x,y);                        //��
	printf("  ");
	gotoxy(x,y+1);
	printf("  ");
}
void erase7(int x,int y)
{
	gotoxy(x,y-1);                      //����
	printf("    ");                     //��
	gotoxy(x,y);                        //��
	printf("  ");
	gotoxy(x,y+1);
	printf("  ");
}

void erase8()
{                  
	gotoxy(x-2,y);                     
	printf("      ");                   //������
	gotoxy(x+2,y+1);                    //    ��
	printf("  "); 
}
void erase8(int x,int y)
{
	gotoxy(x-2,y);                     
	printf("      ");                   //������
	gotoxy(x+2,y+1);                    //    ��
	printf("  ");
}

void erase9()
{
	gotoxy(x,y-1);                      //  ��
	printf("  ");                       //  ��
	gotoxy(x,y);                        //����
	printf("  ");
	gotoxy(x-2,y+1);
	printf("    ");
}
void erase9(int x,int y)
{
	gotoxy(x,y-1);                      //  ��
	printf("  ");                       //  ��
	gotoxy(x,y);                        //����
	printf("  ");
	gotoxy(x-2,y+1);
	printf("    ");
}

void erase10()
{                  
	gotoxy(x,y);                       //  ����
	printf("    ");                    //���� 
	gotoxy(x-2,y+1);
	printf("    ");
}
void erase10(int x,int y)
{
	gotoxy(x,y);                       //  ����
	printf("    ");                    //���� 
	gotoxy(x-2,y+1);
	printf("    ");
}

void erase11()
{
	gotoxy(x-2,y-1);                    
	printf("  ");                       //��
	gotoxy(x-2,y);                      //����
	printf("    ");                     //  ��
	gotoxy(x,y+1);
	printf("  ");
}
void erase11(int x,int y)
{
	gotoxy(x-2,y-1);                    
	printf("  ");                       //��
	gotoxy(x-2,y);                      //����
	printf("    ");                     //  ��
	gotoxy(x,y+1);
	printf("  ");
}

void erase12()
{                  
	gotoxy(x-2,y);                     
	printf("      ");                   //������
	gotoxy(x,y+1);                      //  ��
	printf("  "); 
}
void erase12(int x,int y)
{
	gotoxy(x-2,y);                     
	printf("      ");                   //������
	gotoxy(x,y+1);                      //  ��
	printf("  ");
}

void erase13()
{               
	gotoxy(x,y-1);                    
	printf("  ");                     //  ��
	gotoxy(x-2,y);                    //����
	printf("    ");                   //  ��
	gotoxy(x,y+1);
	printf("  ");
}
void erase13(int x,int y)
{
	gotoxy(x,y-1);                    
	printf("  ");                     //  ��
	gotoxy(x-2,y);                    //����
	printf("    ");                   //  ��
	gotoxy(x,y+1);
	printf("  "); 
}

void erase14()
{                  
	gotoxy(x,y-1);                  //  ��
	printf("  ");                   //������
	gotoxy(x-2,y);                       
	printf("      "); 
}
void erase14(int x,int y)
{
	gotoxy(x,y-1);                  //  ��
	printf("  ");                   //������
	gotoxy(x-2,y);                       
	printf("      "); 
}

void erase15()
{                 
	gotoxy(x,y-1);                      
	printf("  ");                       //��
	gotoxy(x,y);                        //����
	printf("    ");                     //��
	gotoxy(x,y+1);                       
	printf("  ");
}
void erase15(int x,int y)
{
	gotoxy(x,y-1);                      
	printf("  ");                       //��
	gotoxy(x,y);                        //����
	printf("    ");                     //��
	gotoxy(x,y+1);                       
	printf("  ");
}

void erase16()
{                   
	gotoxy(x,y);                     //  ����
	printf("    ");                  //  ����
	gotoxy(x,y+1);                       
	printf("    "); 
}
void erase16(int x,int y)
{
	gotoxy(x,y);                     //  ����
	printf("    ");                  //  ����
	gotoxy(x,y+1);                       
	printf("    "); 
}

void erase17()
{                  
	gotoxy(x-2,y);                      //  ��������
	printf("        ");                       
}
void erase17(int x,int y)
{
	gotoxy(x-2,y);                      //  ��������
	printf("        ");                     
}


void erase18()
{
	gotoxy(x,y-1);                      //��
	printf("  ");                       //��
	gotoxy(x,y);                        //��
	printf("  ");                       //��
	gotoxy(x,y+1);
	printf("  ");
	gotoxy(x,y+2);
	printf("  ");
}
void erase18(int x,int y)
{
	gotoxy(x,y-1);                      //��
	printf("  ");                       //��
	gotoxy(x,y);                        //��
	printf("  ");                       //��
	gotoxy(x,y+1);
	printf("  ");
	gotoxy(x,y+2);
	printf("  ");
}


void erase()
{
	settextcolor(247);
	switch(status)
	{
	case 0:   
		erase0();   
		break;
	case 1:   
		erase1();   
		break;
	case 2:   
		erase2();   
		break;
	case 3:   
		erase3();   
		break;
	case 4:   
		erase4();   
		break;
	case 5:   
		erase5();   
		break;
	case 6:   
		erase6();   
		break;
	case 7:   
		erase7();   
		break;
	case 8:   
		erase8();   
		break;
	case 9:   
		erase9();   
		break;
	case 10:   
		erase10();   
		break;
	case 11:   
		erase11();   
		break;
	case 12:   
		erase12();   
		break;
	case 13:   
		erase13();   
		break;
	case 14:   
		erase14();   
		break;
	case 15:   
		erase15();   
		break;
	case 16:   
		erase16();   
		break;
	case 17:   
		erase17();   
		break;
	case 18:   
		erase18();   
		break;
	default:
		break;
	}
}


void erase_remind()
{
    int x=31,y=6;
	settextcolor(247);
	switch(message2)
	{
	case 0:   
		erase0(x,y);   
		break;
	case 1:   
		erase1(x,y);   
		break;
	case 2:   
		erase2(x,y);   
		break;
	case 3:   
		erase3(x,y);   
		break;
	case 4:   
		erase4(x,y);   
		break;
	case 5:   
		erase5(x,y);   
		break;
	case 6:   
		erase6(x,y);   
		break;
	case 7:   
		erase7(x,y);   
		break;
	case 8:   
		erase8(x,y);   
		break;
	case 9:   
		erase9(x,y);   
		break;
	case 10:   
		erase10(x,y);   
		break;
	case 11:   
		erase11(x,y);   
		break;
	case 12:   
		erase12(x,y);   
		break;
	case 13:   
		erase13(x,y);   
		break;
	case 14:   
		erase14(x,y);   
		break;
	case 15:   
		erase15(x,y);   
		break;
	case 16:   
		erase16(x,y);   
		break;
	case 17:   
		erase17(x,y);   
		break;
	case 18:   
		erase18(x,y);   
		break;
	default:
		break;
	}
} 

//=====================================================================
//=====================����� ���� ����غ���==========================��
void position() //�������ʱ�Ա�ʾ��Ӧλ�õ�������1
{
	switch(status)
	{
	case 0:   
		flag1[x-2][y]=1;
		flag1[x][y]=1;
		flag1[x+2][y]=1;
        flag1[x-2][y+1]=1;
		break;
	case 1:   
		flag1[x-2][y-1]=1;
		flag1[x][y-1]=1;
		flag1[x][y]=1;
		flag1[x][y+1]=1;
		break;
	case 2:   
		flag1[x+2][y-1]=1;
		flag1[x-2][y]=1;
		flag1[x][y]=1;
		flag1[x+2][y]=1;
		break;
	case 3:   
		flag1[x][y-1]=1;
		flag1[x][y]=1;
		flag1[x][y+1]=1;
		flag1[x+2][y+1]=1;
		break;
	case 4:   
		flag1[x-2][y]=1;
		flag1[x][y]=1;
		flag1[x][y+1]=1;
		flag1[x+2][y+1]=1;  
		break;
	case 5:   
		flag1[x][y-1]=1;
		flag1[x-2][y]=1;
		flag1[x][y]=1;
		flag1[x-2][y+1]=1;  
		break;
	case 6:   
		flag1[x-2][y-1]=1;
		flag1[x-2][y]=1;
		flag1[x][y]=1;
		flag1[x+2][y]=1;  
		break;
	case 7:   
		flag1[x][y-1]=1;
		flag1[x+2][y-1]=1;
		flag1[x][y]=1;
		flag1[x][y+1]=1;
		break;
	case 8:   
		flag1[x-2][y]=1;
		flag1[x][y]=1;
		flag1[x+2][y]=1;
		flag1[x+2][y+1]=1;  
		break;
	case 9:   
		flag1[x][y-1]=1;
		flag1[x][y]=1;
		flag1[x-2][y+1]=1;
		flag1[x][y+1]=1; 
		break;
	case 10:   
		flag1[x][y]=1;
		flag1[x+2][y]=1;
		flag1[x-2][y+1]=1;
		flag1[x][y+1]=1;
		break;
	case 11:   
		flag1[x-2][y-1]=1;
		flag1[x-2][y]=1;
		flag1[x][y]=1;
		flag1[x][y+1]=1;   
		break;
	case 12:   
		flag1[x-2][y]=1;
		flag1[x][y]=1;
		flag1[x+2][y]=1;
		flag1[x][y+1]=1;
		break;
	case 13:   
		flag1[x][y-1]=1;
		flag1[x-2][y]=1;
		flag1[x][y]=1;
		flag1[x][y+1]=1; 
		break;
	case 14:   
		flag1[x][y-1]=1;
		flag1[x-2][y]=1;
		flag1[x][y]=1;
		flag1[x+2][y]=1;   
		break;
	case 15:   
		flag1[x][y-1]=1;
		flag1[x][y]=1;
		flag1[x+2][y]=1;
		flag1[x][y+1]=1;
		break;
	case 16:   
		flag1[x][y]=1;
		flag1[x+2][y]=1;
		flag1[x][y+1]=1;
		flag1[x+2][y+1]=1;
		break;
	case 17:
		flag1[x-2][y]=1;
		flag1[x][y]=1;
		flag1[x+2][y]=1;
		flag1[x+4][y]=1; 
		break;
	case 18:   
		flag1[x][y-1]=1;
		flag1[x][y]=1;
		flag1[x][y+1]=1;
		flag1[x][y+2]=1;  
		break;
	default:
		break;
	}
	print3();
}

int check_lined()  //�����жϿ������У�����0��ʾ���п����������ʾ����������
{
	int x,y;
	//   bool bz=1;//��ǣ�ʹY2=y���ִֻ��һ��
	int i=0;//�����ж�ĳ���Ƿ�����
	line_number=0;//������������ֵΪ0
	Y1=0;
	for(y=23;y>3;y--)
	{
		for(x=1;x<25;x+=2)
		{
			if(1==flag1[x][y])
			{
				i++;
				if(12==i)
				{
					Y2[line_number++]=y;
				}
			}
		}
	    if(0==i)//˵�����ֿ���
		{
			Y1=y;
            break;
		}
		i=0;
	}
	return line_number;
}
/*
int check_lined()
{
    int x, y;
	bool bz = 1;//������ʹ��ֵ���Y2 = yִֻ��һ��
	int i = 0;//�����ж�ĳ���Ƿ�����
	line_number = 0;
    for (y = 23; y > 3; --y)
    {
        for (x = 1; x < 25; x = x + 2)
        {
            if (flag1[x][y] == 1)
            {
				++i;
				if (i == 12)//һ�д���25������,˵����������
				{
					++line_number;
					if (bz == 1)
					{
						Y2 = y;
						bz = 0;
					}
				}
            }
        }
		if (i == 0)//˵�����ֿ���
		{
			Y1 = y;
			break;
		}
		i = 0;
    }
    return line_number; //����0��ʾ���п���,��0������Խ������д��������
}
*/
void move()  //�������еķ��鼯������
{
	int x,y;
	settextcolor(240);
	for(y=Y2[line_number-1]-1;y>Y1;y--)
	{
		for(x=1;x<25;x+=2)
		{
			if(1==flag1[x][y])
			{
				flag1[x][y]=0;
				gotoxy(x,y);
				printf("  ");

                gotoxy(x,y+1);
				printf("��");
				flag1[x][y+1]=1;
			}
		}
	}
	//===========���гɹ������´�ӡ�÷���ȼ�==========
	if(score1>=50&&score1<300)
	{
		interval1=800;
		grade1=2;
	}
	else if(score1>=300&&score1<600)
	{
		interval1=750;
		grade1=3;
	}
	 else if(score1>=600&&score1<900)
	{
		interval1=600;
		grade1=4;
	}
    else if(score1>=900&&score1<1200)
	{
		interval1=500;
		grade1=5;
	}
	else if(score1>=1200&&score1<1500)
	{
		interval1=400;
		grade1=6;
	}
	else if(score1>=1500&&score1<2000)
	{
		interval1=350;
		grade1=7;
	}
	else if(score1>=2000)
	{
		interval1=200;
		grade1=8;
	}
	print1();
}
/*
void move()
{
	int x, y;
	for (y = Y2 - line_number; y > Y1; --y)
	{
		for (x = 1; x < 25; x = x + 2)
		{
			if (flag1[x][y] == 1)//˵��(x,y)�з������
			{
				gotoxy(x, y);
				printf("  ");
				flag1[x][y] = 0;

				y = y + line_number;
				gotoxy(x, y);
				printf("��");
				flag1[x][y] = 1;
			}
		}
	}
	//���гɹ�,�����´�ӡ�����͵ȼ�
	if (score1 >= 200 && score1 < 1000)
	{
		interval1 = 800;
		grade1 = 2;
	}
	else if (score1 >= 1000 && score1 < 3000)
	{
		interval1 = 500;
		grade1 = 3;
	}
	else if (score1 >= 3000 && score1 < 8000)
	{
		interval1 = 200;
		grade1 = 4;
	}
	else if (score1 >= 8000)
	{
		interval1 = 100;
		grade1 = 5;
	}
	print1();
}*/

int fun(int a)
{
	switch(a)
	{
	case 1:
		return 1; 
		break;
	case 2: 
		return 3;
		break;
	case 3:
		return 6;
		break;
	case 4:
		return 10;
		break;
	default:
		break;
	}
}

void del_lined()
{
	settextcolor(247);
	int x;
	if(check_lined()>0)
	{
		score1+=fun(line_number)*grade1;//���´�ӡ����
		for(line_number;line_number>0;line_number--)
		{
			gotoxy(1,Y2[line_number-1]);
		    //printf("                        ");
			for(x=1;x<25;x+=2)
			{
				printf("  ");
				flag1[x][Y2[line_number-1]]=0;
			}
			move();
		}
	}
}
/*
void del_lined()
{
	int x;
	if ( check_lined() )
	{
		if (line_number == 1)
		{
			score1 = score1 + 10 * grade1;
			//�Ȱ����еĲ�����
			gotoxy(1, Y2);
			printf("                        ");//��24���ո�,��ո���
			for (x = 1; x < 25; x = x + 2)
			{
				flag1[x][Y2] = 0;
			}
			//�ٰ��������Ͽ������µķ�������
			move();
		}
		else if	(line_number == 2)
		{
			score1 = score1 + 30 * grade1;
			gotoxy(1, Y2);
			printf("                        ");//��24���ո�
			gotoxy(1, Y2 - 1);
			printf("                        ");
			for (x = 1; x < 25; x = x + 2)
			{
				flag1[x][Y2] = 0;
				flag1[x][Y2 - 1] = 0;
			}
			move();
		}
		else if (line_number == 3)
		{
			score1 = score1 + 60 * grade1;
			gotoxy(1, Y2);
			printf("                        ");//��24���ո�
			gotoxy(1, Y2 - 1);
			printf("                        ");
			gotoxy(1, Y2 - 2);
			printf("                        ");
			for (x = 1; x < 25; x = x + 2)
			{
				flag1[x][Y2] = 0;
				flag1[x][Y2 - 1] = 0;
				flag1[x][Y2 - 2] = 0;
			}
			move();
		}
		else
		{
			score1 = score1 + 100 * grade1;
			gotoxy(1, Y2);
			printf("                        ");//��24���ո�
			gotoxy(1, Y2 - 1);
			printf("                        ");
			gotoxy(1, Y2 - 2);
		    printf("                        "); 
			gotoxy(1, Y2 - 3);
			printf("                        ");
			for (x = 1; x < 25; x = x + 2)
			{
				flag1[x][Y2] = 0;
				flag1[x][Y2 - 1] = 0;
				flag1[x][Y2 - 2] = 0;
				flag1[x][Y2 - 3] = 0;
			}
			move();
		}
	}
}
*/
bool judge()  //�ж���Ϸ�Ƿ����������1��ʾ����
{
	int i;
	for(i=1;i<24;i+=2)
	{
		if(1==flag1[i][2])
		{
		    game_over=1;
			GAME1=0;
		    //========������Ϸʱ�ı䷽����ɫ=====
            settextcolor(245);
	        int x,y;
	        for(x=1;x<24;x+=2)
		    for(y=0;y<24;y++)
			{
				if(flag1[x][y]>0)
				{
			      gotoxy(x,y);
			      printf("��");
			      //printf("%d",flag1[x][y]);
				}
			}
			//===================================
			settextcolor(241);
			gotoxy(7,12);
			printf("            "); //12���ո�
			gotoxy(7,13);
			printf("  ��Ϸ����  ");//ǰ��������ո�
			gotoxy(7,14);
			printf("            ");//12���ո�
			skip();
			return 1;//na yi nian wo bu zhi dao ni zai na, wo zhi zhi dao dao chu zhao ni ,ke shi yi zhi zao bu dao
			break;
		}
	}
	create_brick();  //���û�н�����������һ����
   //������ʾ����
	erase_remind();  
	remind();
	return 0;
}

bool update_brick(char key) //���·���λ�ã�����1��ʾ��Ϸ����
{
	if(75==key)  //����
	{
		switch(status)
		{
		case 0:
			if(0==flag1[x-4][y]&&0==flag1[x-4][y+1])
			{
				erase();
				x-=2;
				draw();
			}
			break;
		case 1:
			if(0==flag1[x-4][y-1]&&0==flag1[x-2][y]&&0==flag1[x-2][y+1])
			{
				erase();
				x-=2;
				draw();
			}
			break;
		case 2:
			if(0==flag1[x][y-1]&&0==flag1[x-4][y])
			{
				erase();
				x-=2;
				draw();
			}
			break;
		case 3:
			if(0==flag1[x-2][y-1]&&0==flag1[x-2][y]&&0==flag1[x-2][y+1])
			{
				erase();
				x-=2;
				draw();
			}
			break;
		case 4:
			if(0==flag1[x-4][y]&&0==flag1[x-2][y+1])
			{
				erase();
				x-=2;
				draw();
			}
			break;
		case 5:
			if(0==flag1[x-2][y-1]&&0==flag1[x-4][y]&&0==flag1[x-4][y+1])
			{
				erase();
				x-=2;
				draw();
			}
			break;
		case 6:
			if(0==flag1[x-4][y-1]&&0==flag1[x-4][y])
			{
				erase();
				x-=2;
				draw();
			}
			break;
		case 7:
			if(0==flag1[x-2][y-1]&&0==flag1[x-2][y]&&0==flag1[x-2][y+1])
			{
				erase();
				x-=2;
				draw();
			}
			break;
		case 8:
			if(0==flag1[x-4][y]&&0==flag1[x][y+1])
			{
				erase();
				x-=2;
				draw();
			}
			break;
		case 9:
			if(0==flag1[x-2][y-1]&&0==flag1[x-2][y]&&0==flag1[x-4][y+1])
			{
				erase();
				x-=2;
				draw();
			}
			break;
		case 10:
			if(0==flag1[x-2][y]&&0==flag1[x-4][y+1])
			{
				erase();
				x-=2;
				draw();
			}
			break;
		case 11:
			if(0==flag1[x-4][y-1]&&0==flag1[x-4][y]&&0==flag1[x-2][y+1])
			{
				erase();
				x-=2;
				draw();
			}
			break;
		case 12:
			if(0==flag1[x-4][y]&&0==flag1[x-2][y+1])
			{
				erase();
				x-=2;
				draw();
			}
			break;
		case 13:
			if(0==flag1[x-2][y-1]&&0==flag1[x-4][y]&&0==flag1[x-2][y+1])
			{
				erase();
				x-=2;
				draw();
			}
			break;
		case 14:
			if(0==flag1[x-2][y-1]&&0==flag1[x-4][y])
			{
				erase();
				x-=2;
				draw();
			}
			break;
		case 15:
			if(0==flag1[x-2][y-1]&&0==flag1[x-2][y]&&0==flag1[x-2][y+1])
			{
				erase();
				x-=2;
				draw();
			}
			break;
		case 16:
			if(0==flag1[x-2][y]&&0==flag1[x-2][y+1])
			{
				erase();
				x-=2;
				draw();
			}
			break;
		case 17:
			if(0==flag1[x-4][y])
			{
				erase();
				x-=2;
				draw();
			}
			break;
		case 18:
			if(0==flag1[x-2][y-1]&&0==flag1[x-2][y]&&0==flag1[x-2][y+1]&&0==flag1[x-2][y+2])
			{
				erase();
				x-=2;
				draw();
			}
			break;
		default:
			break;
		}
	}
	else if(77==key) //���� 
	{
		switch(status)
		{
		case 0:
			if(0==flag1[x+4][y]&&0==flag1[x][y+1])
			{
				erase();
				x+=2;
				draw();
			}
			break;
		case 1:
			if(0==flag1[x+2][y-1]&&0==flag1[x+2][y]&&0==flag1[x+2][y+1])
			{
				erase();
				x+=2;
				draw();
			}
			break;
		case 2:
			if(0==flag1[x+4][y-1]&&0==flag1[x+4][y])
			{
				erase();
				x+=2;
				draw();
			}
			break;
		case 3:
			if(0==flag1[x+2][y-1]&&0==flag1[x+2][y]&&0==flag1[x+4][y+1])
			{
				erase();
				x+=2;
				draw();
			}
			break;
		case 4:
			if(0==flag1[x+2][y]&&0==flag1[x+4][y+1])
			{
				erase();
				x+=2;
				draw();
			}
			break;
		case 5:
			if(0==flag1[x+2][y-1]&&0==flag1[x+2][y]&&0==flag1[x][y+1])
			{
				erase();
				x+=2;
				draw();
			}
			break;
		case 6:
			if(0==flag1[x][y-1]&&0==flag1[x+4][y])
			{
				erase();
				x+=2;
				draw();
			}
			break;
		case 7:
			if(0==flag1[x+4][y-1]&&0==flag1[x+2][y]&&0==flag1[x+2][y+1])
			{
				erase();
				x+=2;
				draw();
			}
			break;
		case 8:
			if(0==flag1[x+4][y]&&0==flag1[x+4][y+1])
			{
				erase();
				x+=2;
				draw();
			}
			break;
		case 9:
			if(0==flag1[x+2][y-1]&&0==flag1[x+2][y]&&0==flag1[x+2][y+1])
			{
				erase();
				x+=2;
				draw();
			}
			break;
		case 10:
			if(0==flag1[x+4][y]&&0==flag1[x+2][y+1])
			{
				erase();
				x+=2;
				draw();
			}
			break;
		case 11:
			if(0==flag1[x][y-1]&&0==flag1[x+2][y]&&0==flag1[x+2][y+1])
			{
				erase();
				x+=2;
				draw();
			}
			break;
		case 12:
			if(0==flag1[x+4][y]&&0==flag1[x+2][y+1])
			{
				erase();
				x+=2;
				draw();
			}
			break;
		case 13:
			if(0==flag1[x+2][y-1]&&0==flag1[x+2][y]&&0==flag1[x+2][y+1])
			{
				erase();
				x+=2;
				draw();
			}
			break;
		case 14:
			if(0==flag1[x+2][y-1]&&0==flag1[x+4][y])
			{
				erase();
				x+=2;
				draw();
			}
			break;
		case 15:
			if(0==flag1[x+2][y-1]&&0==flag1[x+4][y]&&0==flag1[x+2][y+1])
			{
				erase();
				x+=2;
				draw();
			}
			break;
		case 16:
			if(0==flag1[x+4][y]&&0==flag1[x+4][y+1])
			{
				erase();
				x+=2;
				draw();
			}
			break;
		case 17:
			if(0==flag1[x+6][y])
			{
				erase();
				x+=2;
				draw();
			}
			break;
		case 18:
			if(0==flag1[x+2][y-1]&&0==flag1[x+2][y]&&0==flag1[x+2][y+1]&&0==flag1[x+2][y+2])
			{
				erase();
				x+=2;
				draw();
			}
			break;
		default:
			break;
		}
	}
	else if(80==key)  //����
	{
		switch(status)
		{
			case 0:
			if(0==flag1[x-2][y+2]&&0==flag1[x][y+1]&&0==flag1[x+2][y+1])
			{
				erase();
				y++;
				draw();
			}
			else  //˵���Ѿ����
			{
				position();//����ط�����ж�λ
				del_lined();//�������д���
				if(1==judge())//˵����Ϸ�Ѿ�����
				{
					return 1;
				}
			}
			break;
		case 1:
			if(0==flag1[x-2][y]&&0==flag1[x][y+2])
			{
				erase();
				y++;
				draw();
			}
			else  //˵���Ѿ����
			{
				position();//����ط�����ж�λ
				del_lined();//�������д���
				if(1==judge())//˵����Ϸ�Ѿ�����
				{
					return 1;
				}
			}
			break;
		case 2:
			if(0==flag1[x-2][y+1]&&0==flag1[x][y+1]&&0==flag1[x+2][y+1])
			{
				erase();
				y++;
				draw();
			}
			else  //˵���Ѿ����
			{
				position();//����ط�����ж�λ
				del_lined();//�������д���
				if(1==judge())//˵����Ϸ�Ѿ�����
				{
					return 1;
				}
			}
			break;
		case 3:
			if(0==flag1[x][y+2]&&0==flag1[x+2][y+2])
			{
				erase();
				y++;
				draw();
			}
			else  //˵���Ѿ����
			{
				position();//����ط�����ж�λ
				del_lined();//�������д���
				if(1==judge())//˵����Ϸ�Ѿ�����
				{
					return 1;
				}
			}
			break;
		case 4:
			if(0==flag1[x-2][y+1]&&0==flag1[x][y+2]&&0==flag1[x+2][y+2])
			{
				erase();
				y++;
				draw();
			}
			else  //˵���Ѿ����
			{
				position();//����ط�����ж�λ
				del_lined();//�������д���
				if(1==judge())//˵����Ϸ�Ѿ�����
				{
					return 1;
				}
			}
			break;
		case 5:
			if(0==flag1[x-2][y+2]&&0==flag1[x][y+1])
			{
				erase();
				y++;
				draw();
			}
			else  //˵���Ѿ����
			{
				position();//����ط�����ж�λ
				del_lined();//�������д���
				if(1==judge())//˵����Ϸ�Ѿ�����
				{
					return 1;
				}
			}
			break;
		case 6:
			if(0==flag1[x-2][y+1]&&0==flag1[x][y+1]&&0==flag1[x+2][y+1])
			{
				erase();
				y++;
				draw();
			}
			else  //˵���Ѿ����
			{
				position();//����ط�����ж�λ
				del_lined();//�������д���
				if(1==judge())//˵����Ϸ�Ѿ�����
				{
					return 1;
				}
			}
			break;
		case 7:
			if(0==flag1[x][y+2]&&0==flag1[x+2][y])
			{
				erase();
				y++;
				draw();
			}
			else  //˵���Ѿ����
			{
				position();//����ط�����ж�λ
				del_lined();//�������д���
				if(1==judge())//˵����Ϸ�Ѿ�����
				{
					return 1;
				}
			}
			break;
		case 8:
			if(0==flag1[x-2][y+1]&&0==flag1[x][y+1]&&0==flag1[x+2][y+2])
			{
				erase();
				y++;
				draw();
			}
			else  //˵���Ѿ����
			{
				position();//����ط�����ж�λ
				del_lined();//�������д���
				if(1==judge())//˵����Ϸ�Ѿ�����
				{
					return 1;
				}
			}
			break;
		case 9:
			if(0==flag1[x-2][y+2]&&0==flag1[x][y+2])
			{
				erase();
				y++;
				draw();
			}
			else  //˵���Ѿ����
			{
				position();//����ط�����ж�λ
				del_lined();//�������д���
				if(1==judge())//˵����Ϸ�Ѿ�����
				{
					return 1;
				}
			}
			break;
		case 10:
			if(0==flag1[x-2][y+2]&&0==flag1[x][y+2]&&0==flag1[x+2][y+1])
			{
				erase();
				y++;
				draw();
			}
			else  //˵���Ѿ����
			{
				position();//����ط�����ж�λ
				del_lined();//�������д���
				if(1==judge())//˵����Ϸ�Ѿ�����
				{
					return 1;
				}
			}
			break;
		case 11:
			if(0==flag1[x-2][y+1]&&0==flag1[x][y+2])
			{
				erase();
				y++;
				draw();
			}
			else  //˵���Ѿ����
			{
				position();//����ط�����ж�λ
				del_lined();//�������д���
				if(1==judge())//˵����Ϸ�Ѿ�����
				{
					return 1;
				}
			}
			break;
		case 12:
			if(0==flag1[x-2][y+1]&&0==flag1[x][y+2]&&0==flag1[x+2][y+1])
			{
				erase();
				y++;
				draw();
			}
			else  //˵���Ѿ����
			{
				position();//����ط�����ж�λ
				del_lined();//�������д���
				if(1==judge())//˵����Ϸ�Ѿ�����
				{
					return 1;
				}
			}
			break;
		case 13:
			if(0==flag1[x-2][y+1]&&0==flag1[x][y+2])
			{
				erase();
				y++;
				draw();
			}
			else  //˵���Ѿ����
			{
				position();//����ط�����ж�λ
				del_lined();//�������д���
				if(1==judge())//˵����Ϸ�Ѿ�����
				{
					return 1;
				}
			}
			break;
		case 14:
			if(0==flag1[x-2][y+1]&&0==flag1[x][y+1]&&0==flag1[x+2][y+1])
			{
				erase();
				y++;
				draw();
			}
			else  //˵���Ѿ����
			{
				position();//����ط�����ж�λ
				del_lined();//�������д���
				if(1==judge())//˵����Ϸ�Ѿ�����
				{
					return 1;
				}
			}
			break;
		case 15:
			if(0==flag1[x][y+2]&&0==flag1[x+2][y+1])
			{
				erase();
				y++;
				draw();
			}
			else  //˵���Ѿ����
			{
				position();//����ط�����ж�λ
				del_lined();//�������д���
				if(1==judge())//˵����Ϸ�Ѿ�����
				{
					return 1;
				}
			}
			break;
		case 16:
			if(0==flag1[x][y+2]&&0==flag1[x+2][y+2])
			{
				erase();
				y++;
				draw();
			}
			else  //˵���Ѿ����
			{
				position();//����ط�����ж�λ
				del_lined();//�������д���
				if(1==judge())//˵����Ϸ�Ѿ�����
				{
					return 1;
				}
			}
			break;
		case 17:
			if(0==flag1[x-2][y+1]&&0==flag1[x][y+1]&&0==flag1[x+2][y+1]&&0==flag1[x+4][y+1])
			{
				erase();
				y++;
				draw();
			}
			else  //˵���Ѿ����
			{
				position();//����ط�����ж�λ
				del_lined();//�������д���
				if(1==judge())//˵����Ϸ�Ѿ�����
				{
					return 1;
				}
			}
			break;
		case 18:
			if(0==flag1[x][y+3])
			{
				erase();
				y++;
				draw();
			}
			else  //˵���Ѿ����
			{
				position();//����ط�����ж�λ
				del_lined();//�������д���
				if(1==judge())//˵����Ϸ�Ѿ�����
				{
					return 1;
				}
			}
			break;
		default:
			break;
		}
	}
	else if(32==key)  //ֱ�����䵽�ײ�
	{   
		
	//	PlaySound("music\/zyl.wav",0,SND_FILENAME|SND_ASYNC);
		bool bz=1;
		switch(status)
		{
		case 0:
			while(1)
			{
			    if(0==flag1[x-2][y+2]&&0==flag1[x][y+1]&&0==flag1[x+2][y+1])
				{
					if(1==bz)
					{ 
						erase();
						bz=0;
					}
				    y++;
				}
				else
				{
					draw();
    				break;

				}
			}
			 //˵���Ѿ����
			
		    position();//����ط�����ж�λ
			del_lined();//�������д���
		   	if(1==judge())//˵����Ϸ�Ѿ�����
			{
				return 1;
			}	
			break;
		case 1:
			while(1)
			{
			    if(0==flag1[x-2][y]&&0==flag1[x][y+2])
				{
					if(1==bz)
					{ 
						erase();
						bz=0;
					}
				    y++;
				}
				else
				{
					draw();
    				break;
					

				}
			}
			 //˵���Ѿ����
			
		    position();//����ط�����ж�λ
			del_lined();//�������д���
		   	if(1==judge())//˵����Ϸ�Ѿ�����
			{
				return 1;
			}	
			break;
		case 2:
			while(1)
			{
			    if(0==flag1[x-2][y+1]&&0==flag1[x][y+1]&&0==flag1[x+2][y+1])
				{
					if(1==bz)
					{ 
						erase();
						bz=0;
					}
				    y++;
				}
				else
				{
					draw();
    				break;

				}
			}
			 //˵���Ѿ����
			
		    position();//����ط�����ж�λ
			del_lined();//�������д���
		   	if(1==judge())//˵����Ϸ�Ѿ�����
			{
				return 1;
			}	
			break;
			
		case 3:
			while(1)
			{
			    if(0==flag1[x][y+2]&&0==flag1[x+2][y+2])
				{
					if(1==bz)
					{ 
						erase();
						bz=0;
					}
				    y++;
				}
				else
				{
					draw();
    				break;

				}
			}
			 //˵���Ѿ����
			
		    position();//����ط�����ж�λ
			del_lined();//�������д���
		   	if(1==judge())//˵����Ϸ�Ѿ�����
			{
				return 1;
			}	
			break;
		
		case 4:
			while(1)
			{
			    if(0==flag1[x-2][y+1]&&0==flag1[x][y+2]&&0==flag1[x+2][y+2])
				{
					if(1==bz)
					{ 
						erase();
						bz=0;
					}
				    y++;
				}
				else
				{
					draw();
    				break;

				}
			}
			 //˵���Ѿ����
			
		    position();//����ط�����ж�λ
			del_lined();//�������д���
		   	if(1==judge())//˵����Ϸ�Ѿ�����
			{
				return 1;
			}	
			break;
			
		case 5:
			while(1)
			{
			    if(0==flag1[x-2][y+2]&&0==flag1[x][y+1])
				{
					if(1==bz)
					{ 
						erase();
						bz=0;
					}
				    y++;
				}
				else
				{
					draw();
    				break;

				}
			}
			 //˵���Ѿ����
			
		    position();//����ط�����ж�λ
			del_lined();//�������д���
		   	if(1==judge())//˵����Ϸ�Ѿ�����
			{
				return 1;
			}	
			break;
			
		case 6:
			while(1)
			{
			    if(0==flag1[x-2][y+1]&&0==flag1[x][y+1]&&0==flag1[x+2][y+1])
				{
					if(1==bz)
					{ 
						erase();
						bz=0;
					}
				    y++;
				}
				else
				{
					draw();
    				break;

				}
			}
			 //˵���Ѿ����
			
		    position();//����ط�����ж�λ
			del_lined();//�������д���
		   	if(1==judge())//˵����Ϸ�Ѿ�����
			{
				return 1;
			}	
			break;
			
		case 7:
			while(1)
			{
			    if(0==flag1[x][y+2]&&0==flag1[x+2][y])
				{
					if(1==bz)
					{ 
						erase();
						bz=0;
					}
				    y++;
				}
				else
				{
					draw();
    				break;

				}
			}
			 //˵���Ѿ����
			
		    position();//����ط�����ж�λ
			del_lined();//�������д���
		   	if(1==judge())//˵����Ϸ�Ѿ�����
			{
				return 1;
			}	
			break;
		
		case 8:
			while(1)
			{
			    if(0==flag1[x-2][y+1]&&0==flag1[x][y+1]&&0==flag1[x+2][y+2])
				{
					if(1==bz)
					{ 
						erase();
						bz=0;
					}
				    y++;
				}
				else
				{
					draw();
    				break;

				}
			}
			 //˵���Ѿ����
			
		    position();//����ط�����ж�λ
			del_lined();//�������д���
		   	if(1==judge())//˵����Ϸ�Ѿ�����
			{
				return 1;
			}	
			break;
			
		case 9:
			while(1)
			{
			    if(0==flag1[x-2][y+2]&&0==flag1[x][y+2])
				{
					if(1==bz)
					{ 
						erase();
						bz=0;
					}
				    y++;
				}
				else
				{
					draw();
    				break;

				}
			}
			 //˵���Ѿ����
			
		    position();//����ط�����ж�λ
			del_lined();//�������д���
		   	if(1==judge())//˵����Ϸ�Ѿ�����
			{
				return 1;
			}	
			break;
			
		case 10:
			while(1)
			{
			    if(0==flag1[x-2][y+2]&&0==flag1[x][y+2]&&0==flag1[x+2][y+1])
				{
					if(1==bz)
					{ 
						erase();
						bz=0;
					}
				    y++;
				}
				else
				{
					draw();
    				break;

				}
			}
			 //˵���Ѿ����
			
		    position();//����ط�����ж�λ
			del_lined();//�������д���
		   	if(1==judge())//˵����Ϸ�Ѿ�����
			{
				return 1;
			}	
			break;
		
		case 11:
			while(1)
			{
			    if(0==flag1[x-2][y+1]&&0==flag1[x][y+2])
				{
					if(1==bz)
					{ 
						erase();
						bz=0;
					}
				    y++;
				}
				else
				{
					draw();
    				break;

				}
			}
			 //˵���Ѿ����
			
		    position();//����ط�����ж�λ
			del_lined();//�������д���
		   	if(1==judge())//˵����Ϸ�Ѿ�����
			{
				return 1;
			}	
			break;
			
		case 12:
			while(1)
			{
			    if(0==flag1[x-2][y+1]&&0==flag1[x][y+2]&&0==flag1[x+2][y+1])
				{
					if(1==bz)
					{ 
						erase();
						bz=0;
					}
				    y++;
				}
				else
				{
					draw();
    				break;

				}
			}
			 //˵���Ѿ����
			
		    position();//����ط�����ж�λ
			del_lined();//�������д���
		   	if(1==judge())//˵����Ϸ�Ѿ�����
			{
				return 1;
			}	
			break;
			
		case 13:
			while(1)
			{
			    if(0==flag1[x-2][y+1]&&0==flag1[x][y+2])
				{
					if(1==bz)
					{ 
						erase();
						bz=0;
					}
				    y++;
				}
				else
				{
					draw();
    				break;

				}
			}
			 //˵���Ѿ����
			
		    position();//����ط�����ж�λ
			del_lined();//�������д���
		   	if(1==judge())//˵����Ϸ�Ѿ�����
			{
				return 1;
			}	
			break;
			
		case 14:
			while(1)
			{
			    if(0==flag1[x-2][y+1]&&0==flag1[x][y+1]&&0==flag1[x+2][y+1])
				{
					if(1==bz)
					{ 
						erase();
						bz=0;
					}
				    y++;
				}
				else
				{
					draw();
    				break;

				}
			}
			 //˵���Ѿ����
			
		    position();//����ط�����ж�λ
			del_lined();//�������д���
		   	if(1==judge())//˵����Ϸ�Ѿ�����
			{
				return 1;
			}	
			break;
			
		case 15:
			while(1)
			{
			    if(0==flag1[x][y+2]&&0==flag1[x+2][y+1])
				{
					if(1==bz)
					{ 
						erase();
						bz=0;
					}
				    y++;
				}
				else
				{
					draw();
    				break;

				}
			}
			 //˵���Ѿ����
			
		    position();//����ط�����ж�λ
			del_lined();//�������д���
		   	if(1==judge())//˵����Ϸ�Ѿ�����
			{
				return 1;
			}	
			break;
			
		case 16:
			while(1)
			{
			    if(0==flag1[x][y+2]&&0==flag1[x+2][y+2])
				{
					if(1==bz)
					{ 
						erase();
						bz=0;
					}
				    y++;
				}
				else
				{
					draw();
    				break;
				}
			}
			 //˵���Ѿ����
			
		    position();//����ط�����ж�λ
			del_lined();//�������д���
		   	if(1==judge())//˵����Ϸ�Ѿ�����
			{
				return 1;
			}	
			break;
			
		case 17:
			while(1)
			{
			    if(0==flag1[x-2][y+1]&&0==flag1[x][y+1]&&0==flag1[x+2][y+1]&&0==flag1[x+4][y+1])
				{
					if(1==bz)
					{ 
						erase();
						bz=0;
					}
				    y++;
				}
				else
				{
					draw();
    				break;
				}
			}
			 //˵���Ѿ����
			
		    position();//����ط�����ж�λ
			del_lined();//�������д���
		   	if(1==judge())//˵����Ϸ�Ѿ�����
			{
				return 1;
			}	
			break;
			
		case 18:
			while(1)
			{
			    if(0==flag1[x][y+3])
				{
					if(1==bz)
					{ 
						erase();
						bz=0;
					}
				    y++;
				}
				else
				{
					draw();
    				break;
				}
			}
			 //˵���Ѿ����
			
		    position();//����ط�����ж�λ
			del_lined();//�������д���
		   	if(1==judge())//˵����Ϸ�Ѿ�����
			{
				return 1;
			}	
			break;
		default:
			break;
		}

	//	PlaySound(NULL,0,SND_FILENAME);
	}
	else if(72==key)//���ϣ���������ת���� 
	{
		switch(status)
		{
		case 0:
			if(0==flag1[x-2][y-1]&&0==flag1[x][y-1]&&0==flag1[x][y+1])
			{
				erase();
				status++;
				draw();
			}
			break;
		case 1:
			if(0==flag1[x+2][y-1]&&0==flag1[x-2][y]&&0==flag1[x+2][y])
			{
				erase();
				status++;
				draw();
			}
			break;
		case 2:
			if(0==flag1[x][y-1]&&0==flag1[x][y+1]&&0==flag1[x+2][y+1])
			{
				erase();
				status++;
				draw();
			}
			break;
		case 3:
			if(0==flag1[x-2][y]&&0==flag1[x+2][y]&&0==flag1[x-2][y+1])
			{
				erase();
				status=0;
				draw();
			}
			break;
		case 4:
			if(0==flag1[x][y-1]&&0==flag1[x-2][y+1])
			{
				erase();
				status++;
				draw();
			}
			break;
		case 5:
			if(0==flag1[x][y+1]&&0==flag1[x+2][y+1])
			{
				erase();
				status=4;
				draw();
			}
			break;
		case 6:
			if(0==flag1[x][y-1]&&0==flag1[x+2][y-1]&&0==flag1[x][y+1])
			{
				erase();
				status++;
				draw();
			}
			break;
		case 7:
			if(0==flag1[x-2][y]&&0==flag1[x+2][y]&&0==flag1[x+2][y+1])
			{
				erase();
				status++;
				draw();
			}
			break;
		case 8:
			if(0==flag1[x][y-1]&&0==flag1[x-2][y+1]&&0==flag1[x][y+1])
			{
				erase();
				status++;
				draw();
			}
			break;
		case 9:
			if(0==flag1[x-2][y-1]&&0==flag1[x-2][y]&&0==flag1[x+2][y])
			{
				erase();
				status=6;
				draw();
			}
			break;
		case 10:
			if(0==flag1[x-2][y-1]&&0==flag1[x-2][y])
			{
				erase();
				status++;
				draw();
			}
			break;
		case 11:
			if(0==flag1[x+2][y]&&0==flag1[x-2][y+1])
			{
				erase();
				status--;
				draw();
			}
			break;
		case 12:
			if(0==flag1[x][y-1])
			{
				erase();
				status++;
				draw();
			}
			break;
		case 13:
			if(0==flag1[x+2][y])
			{
				erase();
				status++;
				draw();
			}
			break;
		case 14:
			if(0==flag1[x][y+1])
			{
				erase();
				status++;
				draw();
			}
			break;
		case 15:
			if(0==flag1[x-2][y])
			{
				erase();
				status=12;
				draw();
			}
			break;
		case 16:
			break;
		case 17:
			if(0==flag1[x][y-1]&&0==flag1[x][y+1]&&0==flag1[x][y+2])
			{
				erase();
				status++;
				draw();
			}
			break;
		case 18:
			if(0==flag1[x-2][y]&&0==flag1[x+2][y]&&0==flag1[x+4][y])
			{
				erase();
				status--;
				draw();
			}
			break;
		default:
			break;
		}
	}
	return 0;
}

//=====================================================================
//=====================================================================
//=====================================================================
///////////////////////////////////////////////////////////////////////
//̰�Գ�
void show2()
{
	int x,y;
	settextcolor(7);
	for(y=0;y<25;y++)
	{
		gotoxy(0,y);
		printf("�d");
		flag2[0][y]=1;
		gotoxy(28,y);
		printf("�e");
        flag2[28][y]=1;
	}
	for(x=2;x<28;x+=2)
	{
		gotoxy(x,0);
		printf("�d");
		flag2[x][0]=1;
        gotoxy(x,24);
		printf("�e");
		flag2[x][24]=1;
	}
	for(x=30;x<39;x+=2)
	{
		gotoxy(x,10);
		printf("�d");
		flag2[x][11]=1;
        gotoxy(x,14);
		printf("�e");
		flag2[x][13]=1;
	}
	//д��
	settextcolor(245);
    gotoxy(31,1);
	printf("HPֵ:100");
	gotoxy(31,2);
	printf("�÷�:0");
    gotoxy(31,3);
	printf("�ȼ�:1");
    gotoxy(31,4);
	printf("�泤:3");
	gotoxy(31,6);
	printf("����ʱ��");

    gotoxy(31,17);
	printf("Esc �˳�");
	gotoxy(31,18);
	printf("F1  ����");
	gotoxy(31,19);
	printf("F2  ��ͣ");
	gotoxy(31,20);
	printf("F5  �л�");
	gotoxy(31,21);
	printf("1   ����");
	gotoxy(31,22);
	printf("2   ����");

	settextcolor(240);
    gotoxy(31,7);
	printf("�x�x�x�x");
	gotoxy(31,9);
	printf("��������");
	//�����
	gotoxy(28,12);
	printf("  ");
	flag2[28][12]=0;
}

void barrier()   //�����ϰ�
{
	settextcolor(251);
	gotoxy(16,10);
	printf("��");
	flag2[16][10]=1;
	gotoxy(16,12);
	printf("��");
	flag2[16][12]=1;
	gotoxy(16,14);
	printf("��");
	flag2[16][14]=1;

	static bool bz=1;
	if(0==bz)
	{
		settextcolor(255);  //�հ״�������
		bz=1;
	}
	else if(1==bz)
	{
		settextcolor(251);
		bz=0;
	}
	gotoxy(12,8);
	printf("����Ϸ��");
	flag2[12][8]=1;
	flag2[14][8]=1;
	flag2[16][8]=1;
	flag2[18][8]=1;
	
	gotoxy(12,16);
	printf("Ϊ������");
	flag2[12][16]=1;
	flag2[14][16]=1;
	flag2[16][16]=1;
	flag2[18][16]=1;
}

void create_snake()   //���ɳ�
{
	settextcolor(243);
	gotoxy(head_x,head_y);
    printf("�ه��");
	flag2[head_x][head_y]=1;
	flag2[head_x+2][head_y]=1;
	flag2[head_x+4][head_y]=1;
	tail_x=head_x+4;
	tail_y=head_y;
}

void create_food()  //����ʳ��
{
	food_HP=rand()%10;
	do
	{
		food_x=(rand()%13+1)*2;
		food_y=rand()%23+1;
	}
	while(flag2[food_x][food_y]!=0);
    if(0==food_HP)
	{
		settextcolor(252);
		food[food_x][food_y]=2;
		food_time=clock();
	}
	else 
	{
		settextcolor(240);
	    food[food_x][food_y]=1;
	}
	Food=1;
	gotoxy(food_x,food_y);
	printf("��");
}

void deal_food()    //����ʳ��
{
    static bool bz=1;
	if(0==Food)
	{
	    static int pd=0;//��¼ʳ�������ʮ������
	    if(1==bz)          //�رճ���
		{
		    gotoxy(28,12);
		    settextcolor(7);
		    printf("�d");
	    	flag2[28][12]=1;
	    	bz=0;
		}
    	settextcolor(244);
    	gotoxy(36,2);
        score2=score2+grade2*10;
    	printf("%d",score2);
    	gotoxy(36,4);
    	printf("%d",++snake_length);
    	pd++;
    	if(0==pd%5)   //�ж��Ƿ�����
		{
	    	gotoxy(36,3);
     		printf("%d",++grade2);
	    	interval2-=10;
		}
	    if(0==food_HP)
		{
	    	if(HP>90)
			{
		    	HP=100;
			}
	    	else 
			{
		    	HP+=10;
			}
	    	gotoxy(36,1);
			printf("   ");
			gotoxy(36,1);
			settextcolor(252);
	    	printf("%d",HP);
			gotoxy(34,8);  //��������ʱ
			printf("   ");  
		}
		create_food();
	}
	else
	{
		if(0==food_HP)
		{
            if(1==bz)    //�رճ���
		    {
                gotoxy(28,12);
                settextcolor(7);
		        printf("�d");
	    	    flag2[28][12]=1;
	    	    bz=0;
		    }
			gotoxy(34,8);
			int t=10-(clock()-food_time)/1000;
			if(t>=0)
			{
				settextcolor(252);
				gotoxy(34,8);
				printf("    ");
				gotoxy(34,8);
		    	printf("%d",t);
			}
			else
			{
				if(!(head_x==food_x&&head_y==food_y))  //����ʱ�ѵ�����ȴû�Ե�����������ʳ��
				{
					gotoxy(food_x,food_y);
			        printf("  ");
				    food[food_x][food_y]=0;
					gotoxy(34,8);  //��������ʱ
		        	printf("    ");
				    create_food();
				}
			} 
		}
	}
}

void tail_move()  //��β�ƶ�
{
	gotoxy(tail_x,tail_y);   //����ԭ��β
	printf("  ");
	flag2[tail_x][tail_y]=0;
	if(Oribehind>0)
	{
		tail_x-=2;
		Oribehind--;
	}
	else
	{
		if(1==steps[Nstep-Curbehind])
		{
			tail_x-=2;
		}
		else if(0==steps[Nstep-Curbehind])
		{
			tail_y--;
		}
		else if(2==steps[Nstep-Curbehind])
		{
			tail_x+=2;
		}
		else if(3==steps[Nstep-Curbehind])
		{
			tail_y++;
		}
	}
	if(food[head_x][head_y]!=0)   //�жϳ��Ƿ�Ե�ʳ��
	{
		Curbehind++;
	}
}

void move(char key)  //�������ƶ�
{
	settextcolor(243);
	switch(key)
	{
	case 72:  //����
		if(0==flag2[head_x][head_y-1])   //ǰ��û���ϰ���
		{
			head_y--;
			Nstep++;
			steps[Nstep]=0;
			gotoxy(head_x,head_y);    //��ͷ�ƶ�
            printf("��");
			gotoxy(head_x,head_y+1);  //����ȡ��ԭ����ͷ
		    printf("��");
			flag2[head_x][head_y]=1;
			if(0==food[head_x][head_y+1])   //��û�гԵ�ʳ��,�ƶ���β
			{
				tail_move();
			}
			else 
			{
				food[head_x][head_y+1]=0;
				Food=0;
			}
		    now_key=key;   //��¼�ò�����
		}
		else   //ǰ�������ϰ�����Ѫ
		{
			HP-=rand()%10+1;
			if(HP<0)
			{
				HP=0;
			}
			gotoxy(36,1);
			printf("   ");
			settextcolor(252);
			gotoxy(36,1);
	    	printf("%d",HP);
		}
		break;
	case 75:    //����
		if(0==flag2[head_x-2][head_y])   //ǰ��û���ϰ���
		{
			head_x-=2;
			Nstep++;
			steps[Nstep]=1;
			gotoxy(head_x,head_y);    //��ͷ�ƶ�
            printf("��");
			gotoxy(head_x+2,head_y);  //����ȡ��ԭ����ͷ
		    printf("��");
			flag2[head_x][head_y]=1;
			if(0==food[head_x+2][head_y])   //��û�гԵ�ʳ��,�ƶ���β
			{
				tail_move();
			}
			else 
			{
				food[head_x+2][head_y]=0;
				Food=0;
			}
		    now_key=key;   //��¼�ò�����
		}
		else   //ǰ�������ϰ�����Ѫ
		{
			HP-=rand()%10+1;
			if(HP<0)
			{
				HP=0;
			}
			gotoxy(36,1);
			printf("   ");
			settextcolor(252);
			gotoxy(36,1);
	    	printf("%d",HP);
		}
		break;

		case 77:   //����
    		if(0==flag2[head_x+2][head_y])   //ǰ��û���ϰ���
			{
		    	head_x+=2;
		      	Nstep++;
		      	steps[Nstep]=2;
		    	gotoxy(head_x,head_y);    //��ͷ�ƶ�
                printf("��");
		        gotoxy(head_x-2,head_y);  //����ȡ��ԭ����ͷ
		        printf("��");
		    	flag2[head_x][head_y]=1;
		    	if(0==food[head_x-2][head_y])   //��û�гԵ�ʳ��,�ƶ���β
				{
			    	tail_move();
				}
		   	    else 
				{
			    	food[head_x-2][head_y]=0;
			    	Food=0;
				}
		        now_key=key;   //��¼�ò�����
			}
	    	else   //ǰ�������ϰ�����Ѫ
			{
		    	HP-=rand()%10+1;
		    	if(HP<0)
				{
			    	HP=0;
				}
		    	gotoxy(36,1);
		    	printf("   ");
		    	gotoxy(36,1);
				settextcolor(252);
	        	printf("%d",HP);
			}
	     	break;

		case 80:  //����

 	    	if(0==flag2[head_x][head_y+1])   //ǰ��û���ϰ���
			{
		    	head_y++;
		    	Nstep++;
		    	steps[Nstep]=3;
		    	gotoxy(head_x,head_y);    //��ͷ�ƶ�
                printf("��");
		    	gotoxy(head_x,head_y-1);  //����ȡ��ԭ����ͷ
		        printf("��");
		     	flag2[head_x][head_y]=1;
		    	if(0==food[head_x][head_y-1])   //��û�гԵ�ʳ��,�ƶ���β
				{
			    	tail_move();
				}
		    	else 
				{
			    	food[head_x][head_y-1]=0;
			    	Food=0;
				}
		        now_key=key;   //��¼�ò�����
			}
	  	    else   //ǰ�������ϰ�����Ѫ
			{
		    	HP-=rand()%10+1;
		    	if(HP<0)
				{
			    	HP=0;
				}
		     	gotoxy(36,1);
		  	    printf("   ");
		    	gotoxy(36,1);
				settextcolor(252);
	        	printf("%d",HP);
			}
	   	    break;
	  	default:
			break;
	}
}
///////////////////////////////////////////////////////////////////����˹������Ʋ���//////////////////////////////////////
void control1() //��������
{
	int time=clock();
	char key1=0;
	char key2=0;
	while((key1!=48&&K!=48)&&(key1!=27&&K!=27))
	{
		Sleep(10); //��CPU��Ϣ10���ӣ����Լ�����ʹ��Ƶ��
		game_time=clock()-begin_time; //������Ϸ��ʱ������ҳ���д�ӡ
        settextcolor(252);
		gotoxy(30,13);
		printf("%d��",(game_time+save_time)/1000);
		//===========

		if(kbhit())//�а����Ƿ���1���ް����Ƿ���0
		{
			//==========�ж�ĳһ�����Ƿ���˫���룬���������key2����
			key1=getch();
			if(key1<=0)
			{
				key2=getch();
			}
            if(80==key2)  //���յ���������ָ��
			{
				if(1==update_brick(80)) //����update_brick����ʹ����ӿ죬���ж���Ϸ�Ƿ����
				{
					break;  //��������������ѭ��
				}
				key2=0; //û�н������������ִ��
			}
			else //���յ��Ĳ�������ָ�����update_brick����ʹ������������
			{
				if(clock()-time>=interval1) //�ж���������ʱ��Ŀ������������ٶȣ����ݵȼ��趨
				{
					if(1==update_brick(80))
					{
						break;
					}
					time=clock();
				}
			}
		}
		else//û�в�������ָ��
		{
			if(clock()-time>=interval1)
			{
				if(1==update_brick(80))
				{
					break;
				}
				time=clock();
			}
		}
		//================�����밴��ѭ���ж�===================
		switch(key1)
		{
		case 27:  //����Esc���˳�
			K=27;
			break;
		case 48:  //����0���˳�
			K=48;
			break;
		case 32:
            update_brick(key1);
			key1=0;
			break;
		case 49:  //����1������
			Save1();
			key1=0;
			break;
		case 50:  //����2������
			Load1();
			key1=0;
			break;
		case 109:  //�򿪹ر�����
			{
			    static bool bz1=0;
		    	if(0==bz1)
				{
			    	m0();
			    	bz1=1;
				}
		    	else
				{
			    	m2();
			    	bz1=0;
				}
			}
			key1=0;
			break;
		case 119:  //��
            update_brick(72);
			key1=0;
			break;
		case 97:  //��
			update_brick(75);
			key1=0;
			break;
		case 100:  //��
			update_brick(77);
			key1=0;
			break;
		default:
			break;
		}
		//=====================================================
		//================˫���밴��ѭ���ж�===================
		switch(key2)
		{
		case 59://F1��-����
			{
				system("cls");  //������б�־
				system("title ����˹����");
				for(int x=1;x<25;x++)
				{
					for(int y=1;y<25;y++)
					{
						flag1[x][y]=0;
					}
				}
				message1=rand()%19;
				score1=0;
				grade1=1;
				interval1=1000;
				begin_time=clock();
				show();
				print1();
				create_brick();
				remind();
				control1();
			}
			break;
		case 60://F2��-��ͣ
			{
				int before_stime=clock(); //��ͣǰʱ��
				getch(); //�������ʼ
				int behind_stime=clock();  //��ͣ��ʱ��
				pause_time=behind_stime-before_stime;  //��ͣ��ʱ
				begin_time+=pause_time;
			}
			key2=0;
			break;
		case 63:  //F5��-�л���Ϸ
			if(1==GAME2)//��Ϸ2δ����
			{
				system("cls");
		    	system("title ̰�Գ�");
		    	int x1,y1;
		    	for(x1=0;x1<40;x1+=2)
				{
				    for(y1=0;y1<25;y1++)
					{
				      	flag2[x1][y1]=0;
					}
				}
				food[food_x][food_y]=0;
				now_key=75;
				Nstep=0;
		  		head_x=32;
				head_y=12;
				Oribehind=2;
				Curbehind=2;
				HP=100;
				score2=0;
				grade2=1;
				snake_length=3;
				interval2=250;
				show2();
				barrier();
				create_snake();
				create_food();
	     		control2();

			    GAME2=0;

			}
			else
			{
				system("cls");
				gotoxy(10,10);
				printf("�����Ǳ���ģ����ú���ϧ��ֻ�����¿�ʼඣ�");
			}
			key2=0;
			break;
		case 72:  //��
		case 75:  //��
		case 77:  //��
			update_brick(key2);
			key2=0;
		//	print2();
			break;
		default:
			break;
		}
		//=====================================================
	}
}
///////////////////////////////////
/////////////////////////////////////
//////////////////////////////////////////
/////////////////////////////////////
//////////////////////////////////
void control2()
{
	char key1=0,key2=0;
	int time=clock();
	while(key1!=27&&K!=27)
	{
		Sleep(interval2);
		if(kbhit())
		{
			key1=getch();
			if(key1<=0)
			{
				key2=getch();
			}
			if(key2==72||key2==75||key2==77||key2==80)
			{
				if(key2!=now_key||key2+now_key!=152)  //�������ԭ��������ͬ���෴
				{
		     		move(key2);  //���·����ƶ���תͷ
				}
				else if(key2+now_key==152)   //ԭ�������෴������PH
				{
					gotoxy(36,1);
		  	        printf("   ");
		    	    gotoxy(36,1);
	        	    printf("%d",--HP);
				}
				move(now_key);//��ԭ�������ƶ�
			}
			else  //���ǰ��µķ����
			{
				move(now_key);
			}	
		}
		else
		{
			move(now_key);  //û�а���
			barrier();
		}
		deal_food();
		if(0==HP)
		{
			system("cls");
			settextcolor(249);
			gotoxy(12,12);
			printf("�� Ϸ �� ��");
			break;

		}

		switch(key1)
		{
		////////////////////////////////////////////////////////////////////////////////////////	
		case 49:
			break;
		case 50:
			break;
		case 109:
			static	bool bz=1;
			if(1==bz)
			{
				m0();
				bz=1;
			}
			else
			{
				m1();
				bz=0;
			}
			key1=0;
			break;

	//	default:
	//		break;
    
		}

////////////////////////////////////////////////////////////////////

		switch(key2)
		{
		case 59:   //F1
			system("cls");
			system("title ̰�Գ�");
			int x1,y1;
			for(x1=0;x1<40;x1+=2)
			{
				for(y1=0;y1<25;y1++)
				{
					flag2[x1][y1]=0;
				}
			}
			food[food_x][food_y]=0;
			now_key=75;
			Nstep=0;
			head_x=32;
			head_y=12;
			Oribehind=2;
			Curbehind=2;
			HP=100;
			score2=0;
			grade2=1;
			snake_length=3;
			interval2=250;
			show2();
		    barrier();
		    create_snake();
		    create_food();
	     	control2();
			break;
		case 60:  //F2
			{
				pause_time=0;
				int t21=clock();
				getch();
				int t22=clock();
				pause_time=t22-t21;
				time=time+pause_time;
			}
			key2=0;
			break;
		case 63:
			if(GAME1==1)
			{
				system("cls");  //������б�־
				system("title ����˹����");
				for(int x=1;x<25;x++)
				{
					for(int y=1;y<25;y++)
					{
						flag1[x][y]=0;
					}
				}
				message1=rand()%19;
				score1=0;
				grade1=1;
				interval1=1000;
				begin_time=clock();
				show();
				print1();
				create_brick();
				remind();
				control1();
				GAME1=0;
			}
			else
			{
				system("cls");
				gotoxy(10,10);
				printf("�����Ǳ���ģ����ú���ϧ��ֻ�����¿�ʼඣ�");
			}
			key2=0;
			break;
		default:
			break;
		}
		if(GAME1==0&&GAME2==0)	
		{
			K=27;
		}
	}
	if(key1==27)
	{
		K=27;
	}
}

void main()
{
    hide_cursor();
	srand(time(NULL)); 
	system("title C��Ϸ����1.0����������");
    PlaySound("music\/play.wav",0,SND_FILENAME|SND_ASYNC|SND_LOOP);
        
	settextcolor(245);
	//�˵�����
	gotoxy(10,4);
	printf("��ѡ���ܣ�");
	gotoxy(10,6);
	printf("1������˹����");
	gotoxy(10,8);
	printf("2��̰�Գ�");
	gotoxy(10,10);
	printf("3�����̼����ѯ");
	gotoxy(10,12);
	printf("0���˳�(Esc)");
	gotoxy(6,15);
	printf("�����Ϸ����رջ�򿪱������֣��밴 M ��");
	settextcolor(252);
	gotoxy(4,18);
	printf("��������Ϸ������������ѡ���");
	gotoxy(6,20);
	printf("1.Ĭ�ϴ��ڴ�С40��25");
	gotoxy(6,22);
	printf("2.Ĭ�ϱ�����ɫ'��ɫ'");
	
	
    //����
	char key;
	do
	{
		key=getch();
		if(109==key)
		{
		    static	bool bz=1;
			if(1==bz)
			{
				m0();
				bz=1;
			}
			else
			{
				m1();
				bz=0;
			}
		}
	}
	while(key!=27&&key!=48&&key!=49&&key!=50&&key!=51);
	if(51==key)
	{
		system("cls");
		system("title �����ѯ");
		key_KF();
	}
	else if(49==key)
	{
		GAME=1;
		system("cls");
		system("title ����˹����");
	    //	PlaySound(NULL,0,SND_FILENAME);
		//PlaySound("music\/game.wav",0,SND_FILENAME|SND_ASYNC|SND_LOOP);
		begin_time=clock();
		show();
		create_brick();
		remind();
		print1();
		control1();

		if(1==game_over)
		{
			
			if(0==GAME1)
			{
				do
				{
					K=getch();
					if(K<=0)
					{
						K=getch();
						if(59==K) //F1
						{
							system("cls");
							for(int x=1;x<25;x++)
							{
								for(int y=1;y<25;y++)
								{
									flag1[x][y]=0;
								}
							}
							show();
							message1=rand()%19;
							score1=0;
							grade1=1;
							print1();
							interval1=1000;
							begin_time=clock();
							create_brick();
							remind();
							control1();
						}
					}
				}	
				while(K!=27);
			}
		}
	}
	else if(50==key)//̰�Գ�
	{
		system("cls");
		system("title ̰�Գ� 1.0");
		show2();
		barrier();
		create_snake();
		create_food();
		control2();
		if(HP==0)
		{
			if(GAME1==1&&GAME2==1)
			{
				do
				{
					K=getch();
					if(K<=0)
					{
						K=getch();
						if(K==59)
						{
							system("cls");
							system("title ̰�Գ�");
							int x1,y1;
							for(x1=0;x1<40;x1+=2)
							{
								for(y1=0;y1<25;y1++)
								{
									flag2[x1][y1]=0;
								}
							}
							food[food_x][food_y]=0;
							now_key=75;
							Nstep=0;
							head_x=32;
							head_y=12;
							Oribehind=2;
							Curbehind=2;
							HP=100;
							score2=0;
							grade2=1;
							snake_length=3;
							interval2=250;
							show2();
							barrier();
							create_snake();
							create_food();
	     					control2();
						}
					}
				}
				while(K!=27);
			}
		}
	}
}