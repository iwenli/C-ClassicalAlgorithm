#include"stdio.h"
#include"conio.h"  //getch()
#include"windows.h"
#include"time.h"

#include"mmsystem.h"
#pragma comment(lib,"winmm.lib")

void control2();

//切换游戏用
int GAME;   //记录开始时游戏编码！1：俄罗斯方块 2：连连看 3：贪吃虫
bool GAME1=1,GAME2=1,GAME3=1;  //游戏结束判断，为1表示结束 
char K;  //联合键key控制游戏推出
char char1[1001],char2[1001];//游戏坐标40×2y5,加一个字符串结尾符


//公用
HANDLE hconsole=GetStdHandle(STD_OUTPUT_HANDLE);  //获取控制台标准句柄    behind（控制台） [bi'haind] 
int score1=0,score2=0,score3=0;  //游戏分数记录
int grade1=1,grade2=1,grade3=1;  //记录游戏等级
int pause_time=0;  //记录暂停用时


//俄罗斯方块
int x,y; //全局坐标变量
int flag1[40][25];  //游戏地图标志
char k;  //由于接收键盘字符
int message1=rand()%19;  //由create_brick向remind传递参数
int message2;  //由remind函数向erase_remind函数传递参数
int status;  //当前方块状态标志，向crase,draw,position函数传递参数
int interval1=1000;  //方块自由下落时间
int begin_time;  //记录游戏开始时间
int game_time=0;  //记录游戏用时
int save_time=0;  //保存游戏用时
int line_number;  //记录可消行数，check_lined函数的返回值
int Y1;  //记录首次空行坐标
int Y2[4];  //记录可消行坐标
bool game_over;  //判断游戏是否结束，为1表示结束
char save1[1001];  //保存游戏用

//贪吃虫转专有变量
int flag2[40][25];  //地图标志
int head_x=32,head_y=12;  //虫头坐标
int tail_x,tail_y;  //虫尾坐标
int food_x,food_y;  //食物坐标
int food[40][25];   //食物标志，值为1的食物存在
int steps[20000];   //记录虫头步伐，0代表上，1代表左，2代表右，3代表下
int snake_length=3; //记录当前虫长
int food_HP;        //判断食物是否具有加血功能
int food_time=0;    //记录食物出现时间
bool Food;          //食物标志，为0时表示食被吃
int HP=100;           //记录当前血值
int interval2=250;  //时间间隔，即虫的移动速度
int Oribehind=2;    //原落后步数，与虫的起始长度有关
int Curbehind=2;    //当前落后步数
int Nstep=0;        //记录虫头走的步数
char now_key=75;    //记录上一次按键，使虫可以按上一次自动行动
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


void m0()  //关闭背景音乐
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
 
//游戏地图{（0,0）--（38,24）}
void gotoxy(int x,int y)  //控制光标位置
{
	COORD coord;
	coord.X=x;
	coord.Y=y;
	SetConsoleCursorPosition(hconsole,coord);   //cursor  n. （计算尺的）游标，指针；[计]光标
                                        //position n. 位置，方位；职位，工作；姿态；站位；vt. 安置；把……放在适当位置
} 

void key_KF()  //键盘键码转换  主要是双键码
{
  char k1=0;
  char k2=0;
  
  gotoxy(6,5);
  printf("请按键(Esc键退出)...");
  for(;;)
  {
	  k1=getch();
      if(k1<=0)
	  {
	     k2=getch();
	  }
      //===清除部分=====
      gotoxy(15,7);
      printf("         ");
      gotoxy(15,9);
      printf("        ");
      //=================
      gotoxy(6,7);
      printf("第一键码:%d",k1);
      gotoxy(6,9);
      printf("第二键码:%d",k2);
      if(27==k1)
	  {
	     break;
	  }
  }
}


void hide_cursor()   //隐藏光标
{
	CONSOLE_CURSOR_INFO cursor_info={1,0};
	SetConsoleCursorInfo(hconsole,&cursor_info);
}


void settextcolor(int i) //设置文本颜色
{
	SetConsoleTextAttribute(hconsole,i);    //attribute n. 属性；特质	
}

void getscreen(char Char[])  //截取屏幕字符
{
	COORD coord1,coord2;//定义坐标变量、
	SMALL_RECT rect={0,0,39,24};//截取范围
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


void show()    //信息显示函数
{
		//=======显示信息=======
    settextcolor(240);
	gotoxy(1,0);
	printf("1=保存 2=载入 F1=重玩 F2=暂停 F5=切换");
	//===============save更正============
	for(int i=2;i<25;i++)
	{
		gotoxy(27,i);            
    	printf("            ");  //12个空格  
	}
    
	gotoxy(27,3);
	printf("下一方块：");
	
	gotoxy(27,10);
	printf("等级：");
	gotoxy(27,11);
	printf("得分：");
	
	settextcolor(242);
	for(int y=5;y<9;y++)  //提示方块墙壁
	{
		gotoxy(27,y);
		printf("║");
		gotoxy(37,y);
		printf("║");
	}
    for(y=29;y<37;y+=2)
	{
		gotoxy(y,4);
		printf("═");
		gotoxy(y,9);
		printf("═");
	}
	//四个角
	gotoxy(27,4);
	printf("╔");
	gotoxy(37,4); 
	printf("╗");
	gotoxy(27,9);
	printf("╚");
	gotoxy(37,9);
	printf("╝");
    
	gotoxy(27,12);
	printf("——————");
	gotoxy(27,14);
	printf("——————");
    //=====设置墙壁=====
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
	printf("                        ");//24个空格
	/*gotoxy(41,24);
	printf("                        ");//24个空格*/
}

//============消除莫名空格============
void skip()   //跳转
{
	settextcolor(247);
	gotoxy(27,24);
	printf(" ");
}


void print1()//打印得分与等级
{
	if(grade1>7)
	{
		settextcolor(240);
	}
    else
	{
		settextcolor(247+grade1);   //根据等级不同显示不同颜色文字
	}
	gotoxy(33,10);
	printf("%d",grade1);
	gotoxy(33,11);
	printf("%d",score1);

	//此处尚不能弄明白，为何跳转之余会出现6个空格，只为用此消之
//	settextcolor(247);
//	gotoxy(27,24);
//	printf(" ");
// 	skip();
}
/*
void print2()//整理游戏时使用
{
	settextcolor(240);
	int x,y;
	for(x=1;x<24;x+=2)
		for(y=0;y<24;y++)
		{
			if(flag1[x][y]>0)
			{
			   gotoxy(40+x,y);
			   printf("■");
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
void print3()  //方块落地时改变游戏方块颜色
{
	settextcolor(240);
	int x,y;
	for(x=1;x<24;x+=2)
		for(y=1;y<24;y++)
		{
			if(flag1[x][y]>0)
			{
			   gotoxy(x,y);
			   printf("■");
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

void Save1()  //保存
{
//	getscreen(save1);
	FILE* fp;
	fp=fopen("C:\\ZhangYuLong_elsfk.dat","wb");
	int* buffer[9];//保存各变量值
//    fwrite(save1,sizeof(save1),1,fp);//将参数写入文件
	fwrite(flag1,sizeof(flag1),1,fp);
    //  for(int i=0;i<8;i++)     //为什么不能直接用循环开辟空间
	//  {
	//   	buffer[i]=(int*)malloc(4);//开辟空间
	//  }
	buffer[0]=(int*)malloc(4);
	*buffer[0]=score1;//分数
	fwrite(buffer[0],4,1,fp);
	buffer[1]=(int*)malloc(4);
	*buffer[1]=grade1;//等级
	fwrite(buffer[1],4,1,fp);
	buffer[2]=(int*)malloc(4);
	*buffer[2]=interval1;//自由下落速度
	fwrite(buffer[2],4,1,fp);
	buffer[3]=(int*)malloc(4);
	*buffer[3]=x;//当前方块横坐标
	fwrite(buffer[3],4,1,fp);
	buffer[4]=(int*)malloc(4);
	*buffer[4]=y;//.......纵坐标
	fwrite(buffer[4],4,1,fp);
	buffer[5]=(int*)malloc(4);
	*buffer[5]=status;//当前方块标志
	fwrite(buffer[5],4,1,fp);
	buffer[6]=(int*)malloc(4);
	*buffer[6]=message1;//下一方块标志
	fwrite(buffer[6],4,1,fp);
	buffer[7]=(int*)malloc(4);
	*buffer[7]=message2;//提示擦除标志
	fwrite(buffer[7],4,1,fp);
	buffer[8]=(int*)malloc(4);
	*buffer[8]=game_time+save_time;//游戏用时
	fwrite(buffer[8],4,1,fp);
	for(int i=0;i<=8;i++)
	{
		free(buffer[i]);//释放空间
	}
	fclose(fp);
}


void Load1()
{
	system("cls");
	FILE *fp;
	int *buffer[9];
	if(NULL==(fp=fopen("C:\\ZhangYuLong_elsfk.dat","rb"))) //判断是否存在游戏进度
	{
		gotoxy(9,10);
		printf("游戏进度不存在！\n\n\n\n       按任意键退出程序.......");
		getch();
		exit(0);
	}
	//存在，载入各变量
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
	print1();  //打印得分
	for(int i=8;i>=0;i--)
	{
		free(buffer[i]);
	}
	fclose(fp);
}	
	
//=============================保存与载入==============================

//=====================================================================
//=====================方块的 生成 的相关函数========================== 
void draw0()                          //■■■
{                                     //■
	settextcolor(249);
	gotoxy(x-2,y);
	printf("■■■");
	gotoxy(x-2,y+1);
	printf("■");
}
void draw0(int x,int y)                          //■■■
{                                                //■
	settextcolor(249);
	gotoxy(x-2,y);
	printf("■■■");
	gotoxy(x-2,y+1);
	printf("■");
}

void draw1()
{
	settextcolor(249);
	gotoxy(x-2,y-1);
	printf("■■");                   //■■
	gotoxy(x,y);                      //  ■
	printf("■");                     //  ■
	gotoxy(x,y+1);
	printf("■");
}
void draw1(int x,int y)
{
	settextcolor(249);
	gotoxy(x-2,y-1);
	printf("■■");                   //■■
	gotoxy(x,y);                      //  ■
	printf("■");                     //  ■
	gotoxy(x,y+1);
	printf("■");
}

void draw2()
{
	settextcolor(249);              //    ■
	gotoxy(x+2,y-1);                //■■■
	printf("■");
	gotoxy(x-2,y);
	printf("■■■");
}
void draw2(int x,int y)
{
	settextcolor(249);              //    ■
	gotoxy(x+2,y-1);                //■■■
	printf("■");
	gotoxy(x-2,y);
	printf("■■■");
}

void draw3()
{
	settextcolor(249);                  //■
	gotoxy(x,y-1);                      //■
	printf("■");                       //■■
	gotoxy(x,y);                
	printf("■");
	gotoxy(x,y+1);
	printf("■■");
}
void draw3(int x,int y)
{
	settextcolor(249);                  //■
	gotoxy(x,y-1);                      //■
	printf("■");                       //■■
	gotoxy(x,y);                
	printf("■");
	gotoxy(x,y+1);
	printf("■■");
}

void draw4()
{
	settextcolor(250);                   
	gotoxy(x-2,y);                      //■■
	printf("■■");                     //  ■■ 
	gotoxy(x,y+1);
	printf("■■");
}
void draw4(int x,int y)
{
	settextcolor(250);                   
	gotoxy(x-2,y);                      //■■
	printf("■■");                     //  ■■ 
	gotoxy(x,y+1);
	printf("■■");
}

void draw5()
{
	settextcolor(250);                //  ■
	gotoxy(x,y-1);                    //■■
	printf("■");                     //■
	gotoxy(x-2,y);                
	printf("■■");
	gotoxy(x-2,y+1);
	printf("■");
}
void draw5(int x,int y)
{
	settextcolor(250);                //  ■
	gotoxy(x,y-1);                    //■■
	printf("■");                     //■
	gotoxy(x-2,y);                
	printf("■■");
	gotoxy(x-2,y+1);
	printf("■");
}

void draw6()
{
	settextcolor(246);                   
	gotoxy(x-2,y-1);                    //■ 
	printf("■");                       //■■■
	gotoxy(x-2,y);                
	printf("■■■"); 
}
void draw6(int x,int y)
{
	settextcolor(246);                   
	gotoxy(x-2,y-1);                    //■ 
	printf("■");                       //■■■
	gotoxy(x-2,y);                
	printf("■■■"); 
}

void draw7()
{
	settextcolor(246);                  //■■
	gotoxy(x,y-1);                      //■
	printf("■■");                     //■
	gotoxy(x,y);                
	printf("■");
	gotoxy(x,y+1);
	printf("■");
}
void draw7(int x,int y)
{
	settextcolor(246);                  //■■
	gotoxy(x,y-1);                      //■
	printf("■■");                     //■
	gotoxy(x,y);                
	printf("■");
	gotoxy(x,y+1);
	printf("■");
}

void draw8()
{
	settextcolor(246);                   
	gotoxy(x-2,y);                     
	printf("■■■");                   //■■■
	gotoxy(x+2,y+1);                    //    ■
	printf("■"); 
}
void draw8(int x,int y)
{
	settextcolor(246);                   
	gotoxy(x-2,y);                     
	printf("■■■");                   //■■■
	gotoxy(x+2,y+1);                    //    ■
	printf("■"); 
}

void draw9()
{
	settextcolor(246);                  //  ■
	gotoxy(x,y-1);                      //  ■
	printf("■");                       //■■
	gotoxy(x,y);                
	printf("■");
	gotoxy(x-2,y+1);
	printf("■■");
}
void draw9(int x,int y)
{
	settextcolor(246);                  //  ■
	gotoxy(x,y-1);                      //  ■
	printf("■");                       //■■
	gotoxy(x,y);                
	printf("■");
	gotoxy(x-2,y+1);
	printf("■■");
}

void draw10()
{
	settextcolor(253);                   
	gotoxy(x,y);                       //  ■■
	printf("■■");                    //■■ 
	gotoxy(x-2,y+1);
	printf("■■");
}
void draw10(int x,int y)
{
	settextcolor(253);                   
	gotoxy(x,y);                       //  ■■
	printf("■■");                    //■■ 
	gotoxy(x-2,y+1);
	printf("■■");
}

void draw11()
{
	settextcolor(253);                  //■
	gotoxy(x-2,y-1);                    //■■
	printf("■");                       //  ■
	gotoxy(x-2,y);                
	printf("■■");
	gotoxy(x,y+1);
	printf("■");
}
void draw11(int x,int y)
{
	settextcolor(253);                  //■
	gotoxy(x-2,y-1);                    //■■
	printf("■");                       //  ■
	gotoxy(x-2,y);                
	printf("■■");
	gotoxy(x,y+1);
	printf("■");
}

void draw12()
{
	settextcolor(251);                   
	gotoxy(x-2,y);                     
	printf("■■■");                   //■■■
	gotoxy(x,y+1);                      //  ■
	printf("■"); 
}
void draw12(int x,int y)
{
	settextcolor(251);                   
	gotoxy(x-2,y);                     
	printf("■■■");                   //■■■
	gotoxy(x,y+1);                      //  ■
	printf("■"); 
}

void draw13()
{
	settextcolor(251);                //  ■
	gotoxy(x,y-1);                    //■■
	printf("■");                     //  ■
	gotoxy(x-2,y);                
	printf("■■");
	gotoxy(x,y+1);
	printf("■");
}
void draw13(int x,int y)
{
	settextcolor(251);                //  ■
	gotoxy(x,y-1);                    //■■
	printf("■");                     //  ■
	gotoxy(x-2,y);                
	printf("■■");
	gotoxy(x,y+1);
	printf("■");
}

void draw14()
{
	settextcolor(251);                   
	gotoxy(x,y-1);                  //  ■
	printf("■");                   //■■■
	gotoxy(x-2,y);                       
	printf("■■■"); 
}
void draw14(int x,int y)
{
	settextcolor(251);                   
	gotoxy(x,y-1);                  //  ■
	printf("■");                   //■■■
	gotoxy(x-2,y);                       
	printf("■■■"); 
}

void draw15()
{
	settextcolor(251);                  //■
	gotoxy(x,y-1);                      //■■
	printf("■");                       //■
	gotoxy(x,y);                
	printf("■■");
	gotoxy(x,y+1);
	printf("■");
}
void draw15(int x,int y)
{
	settextcolor(251);                  //■
	gotoxy(x,y-1);                      //■■
	printf("■");                       //■
	gotoxy(x,y);                
	printf("■■");
	gotoxy(x,y+1);
	printf("■");
}

void draw16()
{
	settextcolor(240);                   
	gotoxy(x,y);                     //  ■■
	printf("■■");                  //  ■■
	gotoxy(x,y+1);                       
	printf("■■"); 
}
void draw16(int x,int y)
{
	settextcolor(240);                   
	gotoxy(x,y);                     //  ■■
	printf("■■");                  //  ■■
	gotoxy(x,y+1);                       
	printf("■■"); 
}


void draw17()
{
	settextcolor(252);                  
	gotoxy(x-2,y);                      //  ■■■■
	printf("■■■■");                       
}
void draw17(int x,int y)
{
	settextcolor(252);                  
	gotoxy(x-2,y);                      //  ■■■■
	printf("■■■■");                       
}

void draw18()
{
	settextcolor(252);                  //■
	gotoxy(x,y-1);                      //■
	printf("■");                       //■
	gotoxy(x,y);                        //■
	printf("■");
	gotoxy(x,y+1);
	printf("■");
	gotoxy(x,y+2);
	printf("■");
}
void draw18(int x,int y)
{
	settextcolor(252);                  //■
	gotoxy(x,y-1);                      //■
	printf("■");                       //■
	gotoxy(x,y);                        //■
	printf("■");
	gotoxy(x,y+1);
	printf("■");
	gotoxy(x,y+2);
	printf("■");
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
	message1=rand()%19;  //获取下一方块
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
//=====================方块的 擦除 的相关函数==========================
void erase0()                          //■■■
{                                      //■
	gotoxy(x-2,y);
	printf("      ");
	gotoxy(x-2,y+1);
	printf("  ");
}
void erase0(int x,int y)                          //■■■
{                                                 //■
	gotoxy(x-2,y);
	printf("      ");
	gotoxy(x-2,y+1);
	printf("  ");
}

void erase1()
{ 
	gotoxy(x-2,y-1);
	printf("    ");                   //■■
	gotoxy(x,y);                      //  ■
	printf("  ");                     //  ■
	gotoxy(x,y+1);
	printf("  ");
}
void erase1(int x,int y)
{
	gotoxy(x-2,y-1);
	printf("    ");                   //■■
	gotoxy(x,y);                      //  ■
	printf("  ");                     //  ■
	gotoxy(x,y+1);
	printf("  ");
}

void erase2()
{
        gotoxy(x+2,y-1);                            //    ■
	printf("  ");                               //■■■
	gotoxy(x-2,y);                               
	printf("      ");
}
void erase2(int x,int y)
{
	gotoxy(x+2,y-1);                            //    ■
	printf("  ");                               //■■■
	gotoxy(x-2,y);                               
	printf("      ");
}

void erase3()
{                   
	gotoxy(x,y-1);                      //■
	printf("  ");                       //■
	gotoxy(x,y);                        //■■
	printf("  ");
	gotoxy(x,y+1);
	printf("    ");
}
void erase3(int x,int y)
{
	gotoxy(x,y-1);                      //■
	printf("  ");                       //■
	gotoxy(x,y);                        //■■
	printf("  ");
	gotoxy(x,y+1);
	printf("    ");
}

void erase4()
{
	gotoxy(x-2,y);                      //■■
	printf("    ");                     //  ■■ 
	gotoxy(x,y+1);
	printf("    ");
}
void erase4(int x,int y)
{                   
	gotoxy(x-2,y);                      //■■
	printf("    ");                     //  ■■ 
	gotoxy(x,y+1);
	printf("    ");
}

void erase5()
{               
	gotoxy(x,y-1);                    //  ■ 
	printf("  ");                     //■■	
        gotoxy(x-2,y);                    //■
	printf("    ");
	gotoxy(x-2,y+1);
	printf("  ");
}
void erase5(int x,int y)
{
	gotoxy(x,y-1);                    //  ■ 
	printf("  ");                     //■■	
        gotoxy(x-2,y);                    //■
	printf("    ");
	gotoxy(x-2,y+1);
	printf("  ");
}

void erase6()
{                   
	gotoxy(x-2,y-1);                    //■ 
	printf("  ");                       //■■■
	gotoxy(x-2,y);                
	printf("      "); 
}
void erase6(int x,int y)
{
	gotoxy(x-2,y-1);                    //■ 
	printf("  ");                       //■■■
	gotoxy(x-2,y);                
	printf("      "); 
}

void erase7()
{
	gotoxy(x,y-1);                      //■■
	printf("    ");                     //■
	gotoxy(x,y);                        //■
	printf("  ");
	gotoxy(x,y+1);
	printf("  ");
}
void erase7(int x,int y)
{
	gotoxy(x,y-1);                      //■■
	printf("    ");                     //■
	gotoxy(x,y);                        //■
	printf("  ");
	gotoxy(x,y+1);
	printf("  ");
}

void erase8()
{                  
	gotoxy(x-2,y);                     
	printf("      ");                   //■■■
	gotoxy(x+2,y+1);                    //    ■
	printf("  "); 
}
void erase8(int x,int y)
{
	gotoxy(x-2,y);                     
	printf("      ");                   //■■■
	gotoxy(x+2,y+1);                    //    ■
	printf("  ");
}

void erase9()
{
	gotoxy(x,y-1);                      //  ■
	printf("  ");                       //  ■
	gotoxy(x,y);                        //■■
	printf("  ");
	gotoxy(x-2,y+1);
	printf("    ");
}
void erase9(int x,int y)
{
	gotoxy(x,y-1);                      //  ■
	printf("  ");                       //  ■
	gotoxy(x,y);                        //■■
	printf("  ");
	gotoxy(x-2,y+1);
	printf("    ");
}

void erase10()
{                  
	gotoxy(x,y);                       //  ■■
	printf("    ");                    //■■ 
	gotoxy(x-2,y+1);
	printf("    ");
}
void erase10(int x,int y)
{
	gotoxy(x,y);                       //  ■■
	printf("    ");                    //■■ 
	gotoxy(x-2,y+1);
	printf("    ");
}

void erase11()
{
	gotoxy(x-2,y-1);                    
	printf("  ");                       //■
	gotoxy(x-2,y);                      //■■
	printf("    ");                     //  ■
	gotoxy(x,y+1);
	printf("  ");
}
void erase11(int x,int y)
{
	gotoxy(x-2,y-1);                    
	printf("  ");                       //■
	gotoxy(x-2,y);                      //■■
	printf("    ");                     //  ■
	gotoxy(x,y+1);
	printf("  ");
}

void erase12()
{                  
	gotoxy(x-2,y);                     
	printf("      ");                   //■■■
	gotoxy(x,y+1);                      //  ■
	printf("  "); 
}
void erase12(int x,int y)
{
	gotoxy(x-2,y);                     
	printf("      ");                   //■■■
	gotoxy(x,y+1);                      //  ■
	printf("  ");
}

void erase13()
{               
	gotoxy(x,y-1);                    
	printf("  ");                     //  ■
	gotoxy(x-2,y);                    //■■
	printf("    ");                   //  ■
	gotoxy(x,y+1);
	printf("  ");
}
void erase13(int x,int y)
{
	gotoxy(x,y-1);                    
	printf("  ");                     //  ■
	gotoxy(x-2,y);                    //■■
	printf("    ");                   //  ■
	gotoxy(x,y+1);
	printf("  "); 
}

void erase14()
{                  
	gotoxy(x,y-1);                  //  ■
	printf("  ");                   //■■■
	gotoxy(x-2,y);                       
	printf("      "); 
}
void erase14(int x,int y)
{
	gotoxy(x,y-1);                  //  ■
	printf("  ");                   //■■■
	gotoxy(x-2,y);                       
	printf("      "); 
}

void erase15()
{                 
	gotoxy(x,y-1);                      
	printf("  ");                       //■
	gotoxy(x,y);                        //■■
	printf("    ");                     //■
	gotoxy(x,y+1);                       
	printf("  ");
}
void erase15(int x,int y)
{
	gotoxy(x,y-1);                      
	printf("  ");                       //■
	gotoxy(x,y);                        //■■
	printf("    ");                     //■
	gotoxy(x,y+1);                       
	printf("  ");
}

void erase16()
{                   
	gotoxy(x,y);                     //  ■■
	printf("    ");                  //  ■■
	gotoxy(x,y+1);                       
	printf("    "); 
}
void erase16(int x,int y)
{
	gotoxy(x,y);                     //  ■■
	printf("    ");                  //  ■■
	gotoxy(x,y+1);                       
	printf("    "); 
}

void erase17()
{                  
	gotoxy(x-2,y);                      //  ■■■■
	printf("        ");                       
}
void erase17(int x,int y)
{
	gotoxy(x-2,y);                      //  ■■■■
	printf("        ");                     
}


void erase18()
{
	gotoxy(x,y-1);                      //■
	printf("  ");                       //■
	gotoxy(x,y);                        //■
	printf("  ");                       //■
	gotoxy(x,y+1);
	printf("  ");
	gotoxy(x,y+2);
	printf("  ");
}
void erase18(int x,int y)
{
	gotoxy(x,y-1);                      //■
	printf("  ");                       //■
	gotoxy(x,y);                        //■
	printf("  ");                       //■
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
//=====================方块的 控制 的相关函数==========================　
void position() //方块落地时对表示相应位置的数组置1
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

int check_lined()  //用于判断可消的行，返回0表示无行可消，否则表示可消的行数
{
	int x,y;
	//   bool bz=1;//标记，使Y2=y语句只执行一次
	int i=0;//用于判断某行是否已满
	line_number=0;//可消行数赋初值为0
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
	    if(0==i)//说明出现空行
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
	bool bz = 1;//作用是使赋值语句Y2 = y只执行一次
	int i = 0;//用于判断某行是否已满
	line_number = 0;
    for (y = 23; y > 3; --y)
    {
        for (x = 1; x < 25; x = x + 2)
        {
            if (flag1[x][y] == 1)
            {
				++i;
				if (i == 12)//一行存在25个方块,说明出现满行
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
		if (i == 0)//说明出现空行
		{
			Y1 = y;
			break;
		}
		i = 0;
    }
    return line_number; //返回0表示无行可消,非0代表可以进行消行处理的行数
}
*/
void move()  //将非满行的方块集体下移
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
				printf("■");
				flag1[x][y+1]=1;
			}
		}
	}
	//===========消行成功后重新打印得分与等级==========
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
			if (flag1[x][y] == 1)//说明(x,y)有方块存在
			{
				gotoxy(x, y);
				printf("  ");
				flag1[x][y] = 0;

				y = y + line_number;
				gotoxy(x, y);
				printf("■");
				flag1[x][y] = 1;
			}
		}
	}
	//消行成功,则重新打印分数和等级
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
		score1+=fun(line_number)*grade1;//重新打印积分
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
			//先把满行的擦除掉
			gotoxy(1, Y2);
			printf("                        ");//共24个空格,清空该行
			for (x = 1; x < 25; x = x + 2)
			{
				flag1[x][Y2] = 0;
			}
			//再把满行以上空行以下的方块下移
			move();
		}
		else if	(line_number == 2)
		{
			score1 = score1 + 30 * grade1;
			gotoxy(1, Y2);
			printf("                        ");//共24个空格
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
			printf("                        ");//共24个空格
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
			printf("                        ");//共24个空格
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
bool judge()  //判断游戏是否结束，返回1表示结束
{
	int i;
	for(i=1;i<24;i+=2)
	{
		if(1==flag1[i][2])
		{
		    game_over=1;
			GAME1=0;
		    //========结束游戏时改变方块颜色=====
            settextcolor(245);
	        int x,y;
	        for(x=1;x<24;x+=2)
		    for(y=0;y<24;y++)
			{
				if(flag1[x][y]>0)
				{
			      gotoxy(x,y);
			      printf("■");
			      //printf("%d",flag1[x][y]);
				}
			}
			//===================================
			settextcolor(241);
			gotoxy(7,12);
			printf("            "); //12个空格
			gotoxy(7,13);
			printf("  游戏结束  ");//前后各两个空格
			gotoxy(7,14);
			printf("            ");//12个空格
			skip();
			return 1;//na yi nian wo bu zhi dao ni zai na, wo zhi zhi dao dao chu zhao ni ,ke shi yi zhi zao bu dao
			break;
		}
	}
	create_brick();  //如果没有结束则生成下一方块
   //更新提示方块
	erase_remind();  
	remind();
	return 0;
}

bool update_brick(char key) //更新方块位置，返回1表示游戏结束
{
	if(75==key)  //向左
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
	else if(77==key) //向右 
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
	else if(80==key)  //向下
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
			else  //说明已经落地
			{
				position();//对落地方块进行定位
				del_lined();//进行消行处理
				if(1==judge())//说明游戏已经结束
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
			else  //说明已经落地
			{
				position();//对落地方块进行定位
				del_lined();//进行消行处理
				if(1==judge())//说明游戏已经结束
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
			else  //说明已经落地
			{
				position();//对落地方块进行定位
				del_lined();//进行消行处理
				if(1==judge())//说明游戏已经结束
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
			else  //说明已经落地
			{
				position();//对落地方块进行定位
				del_lined();//进行消行处理
				if(1==judge())//说明游戏已经结束
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
			else  //说明已经落地
			{
				position();//对落地方块进行定位
				del_lined();//进行消行处理
				if(1==judge())//说明游戏已经结束
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
			else  //说明已经落地
			{
				position();//对落地方块进行定位
				del_lined();//进行消行处理
				if(1==judge())//说明游戏已经结束
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
			else  //说明已经落地
			{
				position();//对落地方块进行定位
				del_lined();//进行消行处理
				if(1==judge())//说明游戏已经结束
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
			else  //说明已经落地
			{
				position();//对落地方块进行定位
				del_lined();//进行消行处理
				if(1==judge())//说明游戏已经结束
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
			else  //说明已经落地
			{
				position();//对落地方块进行定位
				del_lined();//进行消行处理
				if(1==judge())//说明游戏已经结束
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
			else  //说明已经落地
			{
				position();//对落地方块进行定位
				del_lined();//进行消行处理
				if(1==judge())//说明游戏已经结束
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
			else  //说明已经落地
			{
				position();//对落地方块进行定位
				del_lined();//进行消行处理
				if(1==judge())//说明游戏已经结束
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
			else  //说明已经落地
			{
				position();//对落地方块进行定位
				del_lined();//进行消行处理
				if(1==judge())//说明游戏已经结束
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
			else  //说明已经落地
			{
				position();//对落地方块进行定位
				del_lined();//进行消行处理
				if(1==judge())//说明游戏已经结束
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
			else  //说明已经落地
			{
				position();//对落地方块进行定位
				del_lined();//进行消行处理
				if(1==judge())//说明游戏已经结束
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
			else  //说明已经落地
			{
				position();//对落地方块进行定位
				del_lined();//进行消行处理
				if(1==judge())//说明游戏已经结束
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
			else  //说明已经落地
			{
				position();//对落地方块进行定位
				del_lined();//进行消行处理
				if(1==judge())//说明游戏已经结束
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
			else  //说明已经落地
			{
				position();//对落地方块进行定位
				del_lined();//进行消行处理
				if(1==judge())//说明游戏已经结束
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
			else  //说明已经落地
			{
				position();//对落地方块进行定位
				del_lined();//进行消行处理
				if(1==judge())//说明游戏已经结束
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
			else  //说明已经落地
			{
				position();//对落地方块进行定位
				del_lined();//进行消行处理
				if(1==judge())//说明游戏已经结束
				{
					return 1;
				}
			}
			break;
		default:
			break;
		}
	}
	else if(32==key)  //直接下落到底部
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
			 //说明已经落地
			
		    position();//对落地方块进行定位
			del_lined();//进行消行处理
		   	if(1==judge())//说明游戏已经结束
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
			 //说明已经落地
			
		    position();//对落地方块进行定位
			del_lined();//进行消行处理
		   	if(1==judge())//说明游戏已经结束
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
			 //说明已经落地
			
		    position();//对落地方块进行定位
			del_lined();//进行消行处理
		   	if(1==judge())//说明游戏已经结束
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
			 //说明已经落地
			
		    position();//对落地方块进行定位
			del_lined();//进行消行处理
		   	if(1==judge())//说明游戏已经结束
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
			 //说明已经落地
			
		    position();//对落地方块进行定位
			del_lined();//进行消行处理
		   	if(1==judge())//说明游戏已经结束
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
			 //说明已经落地
			
		    position();//对落地方块进行定位
			del_lined();//进行消行处理
		   	if(1==judge())//说明游戏已经结束
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
			 //说明已经落地
			
		    position();//对落地方块进行定位
			del_lined();//进行消行处理
		   	if(1==judge())//说明游戏已经结束
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
			 //说明已经落地
			
		    position();//对落地方块进行定位
			del_lined();//进行消行处理
		   	if(1==judge())//说明游戏已经结束
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
			 //说明已经落地
			
		    position();//对落地方块进行定位
			del_lined();//进行消行处理
		   	if(1==judge())//说明游戏已经结束
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
			 //说明已经落地
			
		    position();//对落地方块进行定位
			del_lined();//进行消行处理
		   	if(1==judge())//说明游戏已经结束
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
			 //说明已经落地
			
		    position();//对落地方块进行定位
			del_lined();//进行消行处理
		   	if(1==judge())//说明游戏已经结束
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
			 //说明已经落地
			
		    position();//对落地方块进行定位
			del_lined();//进行消行处理
		   	if(1==judge())//说明游戏已经结束
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
			 //说明已经落地
			
		    position();//对落地方块进行定位
			del_lined();//进行消行处理
		   	if(1==judge())//说明游戏已经结束
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
			 //说明已经落地
			
		    position();//对落地方块进行定位
			del_lined();//进行消行处理
		   	if(1==judge())//说明游戏已经结束
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
			 //说明已经落地
			
		    position();//对落地方块进行定位
			del_lined();//进行消行处理
		   	if(1==judge())//说明游戏已经结束
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
			 //说明已经落地
			
		    position();//对落地方块进行定位
			del_lined();//进行消行处理
		   	if(1==judge())//说明游戏已经结束
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
			 //说明已经落地
			
		    position();//对落地方块进行定位
			del_lined();//进行消行处理
		   	if(1==judge())//说明游戏已经结束
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
			 //说明已经落地
			
		    position();//对落地方块进行定位
			del_lined();//进行消行处理
		   	if(1==judge())//说明游戏已经结束
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
			 //说明已经落地
			
		    position();//对落地方块进行定位
			del_lined();//进行消行处理
		   	if(1==judge())//说明游戏已经结束
			{
				return 1;
			}	
			break;
		default:
			break;
		}

	//	PlaySound(NULL,0,SND_FILENAME);
	}
	else if(72==key)//向上，继续即旋转方块 
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
//贪吃虫
void show2()
{
	int x,y;
	settextcolor(7);
	for(y=0;y<25;y++)
	{
		gotoxy(0,y);
		printf("卍");
		flag2[0][y]=1;
		gotoxy(28,y);
		printf("卐");
        flag2[28][y]=1;
	}
	for(x=2;x<28;x+=2)
	{
		gotoxy(x,0);
		printf("卍");
		flag2[x][0]=1;
        gotoxy(x,24);
		printf("卐");
		flag2[x][24]=1;
	}
	for(x=30;x<39;x+=2)
	{
		gotoxy(x,10);
		printf("卍");
		flag2[x][11]=1;
        gotoxy(x,14);
		printf("卐");
		flag2[x][13]=1;
	}
	//写字
	settextcolor(245);
    gotoxy(31,1);
	printf("HP值:100");
	gotoxy(31,2);
	printf("得分:0");
    gotoxy(31,3);
	printf("等级:1");
    gotoxy(31,4);
	printf("虫长:3");
	gotoxy(31,6);
	printf("倒计时：");

    gotoxy(31,17);
	printf("Esc 退出");
	gotoxy(31,18);
	printf("F1  重玩");
	gotoxy(31,19);
	printf("F2  暂停");
	gotoxy(31,20);
	printf("F5  切换");
	gotoxy(31,21);
	printf("1   保存");
	gotoxy(31,22);
	printf("2   载入");

	settextcolor(240);
    gotoxy(31,7);
	printf("▁▁▁▁");
	gotoxy(31,9);
	printf("▔▔▔▔");
	//打开入口
	gotoxy(28,12);
	printf("  ");
	flag2[28][12]=0;
}

void barrier()   //设置障碍
{
	settextcolor(251);
	gotoxy(16,10);
	printf("张");
	flag2[16][10]=1;
	gotoxy(16,12);
	printf("玉");
	flag2[16][12]=1;
	gotoxy(16,14);
	printf("龙");
	flag2[16][14]=1;

	static bool bz=1;
	if(0==bz)
	{
		settextcolor(255);  //空白处忽悠人
		bz=1;
	}
	else if(1==bz)
	{
		settextcolor(251);
		bz=0;
	}
	gotoxy(12,8);
	printf("本游戏由");
	flag2[12][8]=1;
	flag2[14][8]=1;
	flag2[16][8]=1;
	flag2[18][8]=1;
	
	gotoxy(12,16);
	printf("为您制作");
	flag2[12][16]=1;
	flag2[14][16]=1;
	flag2[16][16]=1;
	flag2[18][16]=1;
}

void create_snake()   //生成虫
{
	settextcolor(243);
	gotoxy(head_x,head_y);
    printf("①囧囧");
	flag2[head_x][head_y]=1;
	flag2[head_x+2][head_y]=1;
	flag2[head_x+4][head_y]=1;
	tail_x=head_x+4;
	tail_y=head_y;
}

void create_food()  //生成食物
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
	printf("●");
}

void deal_food()    //处理食物
{
    static bool bz=1;
	if(0==Food)
	{
	    static int pd=0;//记录食物次数，十次升级
	    if(1==bz)          //关闭出口
		{
		    gotoxy(28,12);
		    settextcolor(7);
		    printf("卍");
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
    	if(0==pd%5)   //判断是否升级
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
			gotoxy(34,8);  //擦除倒计时
			printf("   ");  
		}
		create_food();
	}
	else
	{
		if(0==food_HP)
		{
            if(1==bz)    //关闭出口
		    {
                gotoxy(28,12);
                settextcolor(7);
		        printf("卍");
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
				if(!(head_x==food_x&&head_y==food_y))  //倒计时已到，虫却没吃到则重新生成食物
				{
					gotoxy(food_x,food_y);
			        printf("  ");
				    food[food_x][food_y]=0;
					gotoxy(34,8);  //擦除倒计时
		        	printf("    ");
				    create_food();
				}
			} 
		}
	}
}

void tail_move()  //虫尾移动
{
	gotoxy(tail_x,tail_y);   //擦除原虫尾
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
	if(food[head_x][head_y]!=0)   //判断虫是否吃掉食物
	{
		Curbehind++;
	}
}

void move(char key)  //处理虫的移动
{
	settextcolor(243);
	switch(key)
	{
	case 72:  //向上
		if(0==flag2[head_x][head_y-1])   //前方没有障碍物
		{
			head_y--;
			Nstep++;
			steps[Nstep]=0;
			gotoxy(head_x,head_y);    //虫头移动
            printf("①");
			gotoxy(head_x,head_y+1);  //虫身取代原来虫头
		    printf("囧");
			flag2[head_x][head_y]=1;
			if(0==food[head_x][head_y+1])   //虫没有吃到食物,移动虫尾
			{
				tail_move();
			}
			else 
			{
				food[head_x][head_y+1]=0;
				Food=0;
			}
		    now_key=key;   //记录该步按键
		}
		else   //前方存在障碍物，则减血
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
	case 75:    //向左
		if(0==flag2[head_x-2][head_y])   //前方没有障碍物
		{
			head_x-=2;
			Nstep++;
			steps[Nstep]=1;
			gotoxy(head_x,head_y);    //虫头移动
            printf("①");
			gotoxy(head_x+2,head_y);  //虫身取代原来虫头
		    printf("囧");
			flag2[head_x][head_y]=1;
			if(0==food[head_x+2][head_y])   //虫没有吃到食物,移动虫尾
			{
				tail_move();
			}
			else 
			{
				food[head_x+2][head_y]=0;
				Food=0;
			}
		    now_key=key;   //记录该步按键
		}
		else   //前方存在障碍物，则减血
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

		case 77:   //向右
    		if(0==flag2[head_x+2][head_y])   //前方没有障碍物
			{
		    	head_x+=2;
		      	Nstep++;
		      	steps[Nstep]=2;
		    	gotoxy(head_x,head_y);    //虫头移动
                printf("①");
		        gotoxy(head_x-2,head_y);  //虫身取代原来虫头
		        printf("囧");
		    	flag2[head_x][head_y]=1;
		    	if(0==food[head_x-2][head_y])   //虫没有吃到食物,移动虫尾
				{
			    	tail_move();
				}
		   	    else 
				{
			    	food[head_x-2][head_y]=0;
			    	Food=0;
				}
		        now_key=key;   //记录该步按键
			}
	    	else   //前方存在障碍物，则减血
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

		case 80:  //向下

 	    	if(0==flag2[head_x][head_y+1])   //前方没有障碍物
			{
		    	head_y++;
		    	Nstep++;
		    	steps[Nstep]=3;
		    	gotoxy(head_x,head_y);    //虫头移动
                printf("①");
		    	gotoxy(head_x,head_y-1);  //虫身取代原来虫头
		        printf("囧");
		     	flag2[head_x][head_y]=1;
		    	if(0==food[head_x][head_y-1])   //虫没有吃到食物,移动虫尾
				{
			    	tail_move();
				}
		    	else 
				{
			    	food[head_x][head_y-1]=0;
			    	Food=0;
				}
		        now_key=key;   //记录该步按键
			}
	  	    else   //前方存在障碍物，则减血
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
///////////////////////////////////////////////////////////////////俄罗斯方块控制部分//////////////////////////////////////
void control1() //控制中枢
{
	int time=clock();
	char key1=0;
	char key2=0;
	while((key1!=48&&K!=48)&&(key1!=27&&K!=27))
	{
		Sleep(10); //让CPU休息10秒钟，可以减少其使用频率
		game_time=clock()-begin_time; //计算游戏用时，并在页面中打印
        settextcolor(252);
		gotoxy(30,13);
		printf("%d秒",(game_time+save_time)/1000);
		//===========

		if(kbhit())//有按键是返回1，无按键是返回0
		{
			//==========判断某一按键是否是双键码，如果是则用key2接收
			key1=getch();
			if(key1<=0)
			{
				key2=getch();
			}
            if(80==key2)  //接收到的是向下指令
			{
				if(1==update_brick(80)) //调用update_brick函数使下落加快，并判断游戏是否结束
				{
					break;  //结束则跳出本次循环
				}
				key2=0; //没有结束则继续向下执行
			}
			else //接收到的不是向下指令，调用update_brick函数使方块自由下落
			{
				if(clock()-time>=interval1) //判断自由下落时间的快慢，即下落速度，根据等级设定
				{
					if(1==update_brick(80))
					{
						break;
					}
					time=clock();
				}
			}
		}
		else//没有产生按键指令
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
		//================单键码按键循环判断===================
		switch(key1)
		{
		case 27:  //按键Esc，退出
			K=27;
			break;
		case 48:  //按键0，退出
			K=48;
			break;
		case 32:
            update_brick(key1);
			key1=0;
			break;
		case 49:  //按键1，保存
			Save1();
			key1=0;
			break;
		case 50:  //按键2，载入
			Load1();
			key1=0;
			break;
		case 109:  //打开关闭音乐
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
		case 119:  //上
            update_brick(72);
			key1=0;
			break;
		case 97:  //左
			update_brick(75);
			key1=0;
			break;
		case 100:  //右
			update_brick(77);
			key1=0;
			break;
		default:
			break;
		}
		//=====================================================
		//================双键码按键循环判断===================
		switch(key2)
		{
		case 59://F1键-重玩
			{
				system("cls");  //清空所有标志
				system("title 俄罗斯方块");
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
		case 60://F2键-暂停
			{
				int before_stime=clock(); //暂停前时间
				getch(); //任意键开始
				int behind_stime=clock();  //暂停后时间
				pause_time=behind_stime-before_stime;  //暂停用时
				begin_time+=pause_time;
			}
			key2=0;
			break;
		case 63:  //F5键-切换游戏
			if(1==GAME2)//游戏2未结束
			{
				system("cls");
		    	system("title 贪吃虫");
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
				printf("机会是宝贵的，不好好珍惜，只能重新开始喽！");
			}
			key2=0;
			break;
		case 72:  //上
		case 75:  //左
		case 77:  //右
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
				if(key2!=now_key||key2+now_key!=152)  //如果不是原来按键相同或相反
				{
		     		move(key2);  //按新方向移动，转头
				}
				else if(key2+now_key==152)   //原来方向相反，减少PH
				{
					gotoxy(36,1);
		  	        printf("   ");
		    	    gotoxy(36,1);
	        	    printf("%d",--HP);
				}
				move(now_key);//按原来方向移动
			}
			else  //不是按下的方向键
			{
				move(now_key);
			}	
		}
		else
		{
			move(now_key);  //没有按键
			barrier();
		}
		deal_food();
		if(0==HP)
		{
			system("cls");
			settextcolor(249);
			gotoxy(12,12);
			printf("游 戏 结 束");
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
			system("title 贪吃虫");
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
				system("cls");  //清空所有标志
				system("title 俄罗斯方块");
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
				printf("机会是宝贵的，不好好珍惜，只能重新开始喽！");
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
	system("title C游戏开发1.0（张玉龙）");
    PlaySound("music\/play.wav",0,SND_FILENAME|SND_ASYNC|SND_LOOP);
        
	settextcolor(245);
	//菜单部分
	gotoxy(10,4);
	printf("请选择功能：");
	gotoxy(10,6);
	printf("1：俄罗斯方块");
	gotoxy(10,8);
	printf("2：贪吃虫");
	gotoxy(10,10);
	printf("3：键盘键码查询");
	gotoxy(10,12);
	printf("0：退出(Esc)");
	gotoxy(6,15);
	printf("如果游戏中想关闭或打开背景音乐，请按 M 键");
	settextcolor(252);
	gotoxy(4,18);
	printf("……本游戏在如下条件最佳……");
	gotoxy(6,20);
	printf("1.默认窗口大小40×25");
	gotoxy(6,22);
	printf("2.默认背景颜色'白色'");
	
	
    //主体
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
		system("title 键码查询");
		key_KF();
	}
	else if(49==key)
	{
		GAME=1;
		system("cls");
		system("title 俄罗斯方块");
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
	else if(50==key)//贪吃虫
	{
		system("cls");
		system("title 贪吃虫 1.0");
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
							system("title 贪吃虫");
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