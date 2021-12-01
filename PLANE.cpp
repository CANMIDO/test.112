#include<stdio.h>
#include<stdlib.h>
#include<conio.h>
#include<windows.h>
#include<iostream>

#define High  25//设计页面的大小 
#define Width 50
#define EnemyNum 5//设计敌机个数 
using namespace std;
struct ME//我方飞机的结构体 
{
	int gx;
	int gy;
};
struct ENEMY//敌机的结构体 
{
	int ex[EnemyNum];
	int ey[EnemyNum];
};
//全局变量（飞机的一些参数）

int bulletwidth;//z子弹宽度
int EnemyMoveSpeed; //敌机的移动速度
int Life=5;
int score;
struct ME my;
struct ENEMY  Enemy;
int canvas[High][Width] = {0}; 
                          //1：飞机；2：子弹；3：敌机； 
void gotoxy(int x,int y)//光标移动到（x,y）的位置 
{
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos;
	pos.X = x;
	pos.Y = y;
	SetConsoleCursorPosition(handle,pos);
 } 
 void HideCursor() // 用于隐藏光标
{
	CONSOLE_CURSOR_INFO cursor_info = {1, 0};  // 第二个值为0表示隐藏光标
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);//函数和结构体都在windows.h中定义。
}


void show()
 {
 	gotoxy(0,0);
 //	HideCursor();
 	int i,j;
 	if(Life>0)
	{
        
    	for(i=0;i<High;i++)
 	    {
 		    for(j=0;j<Width;j++)
 		    {
 			    if(canvas[i][j]==0)
 			      cout<<' ';
 			    else if(canvas[i][j]==1)
 			      cout<<'$';
 			    else if(canvas[i][j]==2)
 			      cout<<'|';
 			    else if(canvas[i][j]==3)
 			      cout<<'*';
 			}
		cout<<endl;
	    }
	 cout<<"得分"<<score<<'\t'<<"剩余生命值"<<Life;
	 //Sleep(20);
    }
    else
    {
    	cout<<"失败";
	  	Sleep(3000);
	  	system("pause");
	  	exit(0);
	}
}
 
 void startup()//数据初始化 
 {
 	my.gx = High-1;
 	my.gy = Width/2;
 	canvas[my.gx][my.gy] = 1;//1代表我方飞机 
 	int k;
 	for(k=0;k<EnemyNum;k++)
 	{
 		Enemy.ex[k]=rand()%2;//在1，2中出来 
		Enemy.ey[k]=rand()%Width; 
		canvas[Enemy.ex[k]][Enemy.ey[k]]=3;
	 }
	 score=0;
	 EnemyMoveSpeed=20;
 }
 

void UpdateWithInput()
{
	char input;
	int i,j,k;
	if(kbhit())//判断是否为有效输入（系统定义的函数，是否有彭键盘） 
	{
		input=getch();//根据用户的不同输入来移动，不必需要回车；而且输入了东西后，屏幕不会马上显示 
		if(input=='a'&& Enemy.ey>0)
		{
			canvas[my.gx][my.gy]=0;//游戏画面的更新，老的先去掉，新的位置更新； 
			my.gy--;
			canvas[my.gx][my.gy]=1;
		 }
		else if(input=='d'&& Enemy.ey>0)
		{
			canvas[my.gx][my.gy]=0;
			my.gy++;
			canvas[my.gx][my.gy]=1;
		 }
		else if(input=='w'&& Enemy.ey>0)
		{
			canvas[my.gx][my.gy]=0;
			my.gx--;
			canvas[my.gx][my.gy]=1;
		}
		else if(input=='s'&& Enemy.ey>0)
		{
			canvas[my.gx][my.gy]=0;
			my.gx++;
			canvas[my.gx][my.gy]=1;
		}
		else if(input='k')//发射子弹 
		{
			int left=my.gy-bulletwidth;
			int right=my.gy+bulletwidth;
			if(left<0) left=0;
			if(right>Width-1) right=Width-1;
			int k;
			for(k=left;k<=right;k++)
			  canvas[my.gx-1][k]=2;
		}
		else if(input=' ')
		{
		    for(i=0;i<High;i++)
			    for(j=0;j<Width;j++)
				    canvas[i][j]=0;//飞机全部清空 
			for(k=0;k<EnemyNum;k++)
 	        {
 		        Enemy.ex[k]=rand()%2; 
		        Enemy.ey[k]=rand()%Width; 
		        canvas[Enemy.ex[k]][Enemy.ey[k]]=3;//再次生成飞机 
	        }			
		}//输入空格		  
	} //如果有输入 
}
void UpdateWithoutInput()
{
	int i,j,k,l;
	for(i=0;i<High;i++)//整个循环操控我方敌机打出的子弹 
	{
		for(j=0;j<Width;j++)
		{
			if(canvas[i][j]==2)
			{
				for(k=0;k<EnemyNum;k++)
				{
					if(Enemy.ex[k]==i&&Enemy.ey[k]==j)//子弹打中了敌机
					{
						score++;
						if(score%5==0&&EnemyMoveSpeed>4)
						   EnemyMoveSpeed--;
						if(score%5==0)
						   bulletwidth++;
						canvas[Enemy.ex[k]][Enemy.ey[k]]=0;
						Enemy.ex[k]=rand()%2;
						Enemy.ey[k]=rand()%Width;
						canvas[Enemy.ex[k]][Enemy.ey[k]]=3;
						canvas[i][j]=0;
					} 
				}
				canvas[i][j]=0;//子弹的移动 
				while(i>0)
				   canvas[i-1][j]=2;
			}
		}
    }
	
        static int speed=0;
	    if(speed<EnemyMoveSpeed)
	      speed++;
	    for(k=0;k<EnemyNum;k++)//对敌机的循环（判断是否和本机相撞，判断是否越界、敌机的自然下落） 
	    { 
	        if((my.gx==Enemy.ex[k])&&(my.gy==Enemy.ey[k]))
	        {
	  	        if(Life>0)  Life--;
	  	        else
	  	       {
	  	 	        cout<<"失败";
	  	 	        Sleep(3000);
	  	 	        system("pause");
	  	 	        exit(0);
                }
	        }
	        if(Enemy.ex[k]>High)//飞机撞到底部 
	        {
	        	canvas[Enemy.ex[k]][Enemy.ey[k]]=0;
	        	Enemy.ex[k]=rand()%2;
	        	Enemy.ey[k]=rand()%Width;
	        	canvas[Enemy.ex[k]][Enemy.ey[k]]=3;
	    	}
    	
	        if(speed==EnemyMoveSpeed)//敌机的下落 
	       {
	    		for(l=0;l<EnemyNum;l++)
	    		{
			    	canvas[Enemy.ex[k]][Enemy.ey[k]]=0;
			    	Enemy.ex[k]=Enemy.ex[k]+1;
			    	Enemy.ey[k]=-1+rand()%3;
				    canvas[Enemy.ex[k]][Enemy.ey[k]]=2;
		    		speed=0; 
			    }
	        }
        }//对每个敌机的循环 
    
}

int main()
{
	startup();//数据初始化
	while(1)
	{
		show();//显示画面 
		UpdateWithoutInput();
		UpdateWithInput();
	 } 
	return 0;
 } 
