#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>
#include <conio.h>

#define frame_height 30//地图尺寸
#define frame_width 50
#define UP 'w'//移动
#define DOWN 's'
#define LEFT 'a'
#define RIGHT 'd'

int i,j,k,sp,score;
char ch=UP,state=UP,choo,n;//初始化方向
int grow=0;

struct Food//食物
{ 
   
    int x;//横坐标
    int y;//纵坐标
} food;

struct Snake//蛇
{ 
   
    /*用数组储存蛇的每一部分的坐标*/
    int x[100];
    int y[100];
    int len;//长度
    int speed;//速度
} snake;

void map(void);//地图
void update_food(void);//更新食物
void move_snake(void);//蛇的移动
int alive(void);//判断蛇是否死亡
void get_speed(void);//更新速度
void gotoxy(int x,int y);//移动光标，进行游戏界面的打印


int main()
{ 
   
    do
    { 
   
        score=0;//初始化分数为0
        /*让用户进行难度选择，有彩蛋*/
        printf("Choose the degree of difficulty:\n1:easy\t2:middle 3:difficult\n");
        n=_getch();
        switch(n)
        { 
   
        case '1':
        { 
   
            sp=300;
            break;
        }
        case '2':
        { 
   
            sp=230;
            break;
        }
        case '3':
        { 
   
            sp=180;
            break;
        }
        default:
        { 
   
            printf("Congratulations!Welcome to Devil's difficulty\n");
            sp=120;
            break;
        }
        }
        system("cls");//每次新一局游戏先清屏,包含在<stdlib.h>
        map();//打印地图
        /*开始游戏*/
        while(1)
        { 
   
            update_food();//生产食物
            get_speed();//获取速度
            move_snake();//移动
            Sleep(snake.speed);//延时函数,speed数值越大延时越长
            if(!(alive()))//判断蛇是否死亡
            { 
   
                break;//死亡则退出循环
            }
        }
        printf("Game Over!\n");
        printf("1:Restart\t2:exit");
        choo=_getch();
    }
    while(choo=='1');
    return 0;
}

void map()
{ 
   
    srand(time(NULL));
    /*打印第一个食物*/
    /*Attention!此处留了一个bug:可能食物 的位置与初始的蛇重合，然后食物就会消 失，读者可以加以改进*/
    food.x=rand()%(frame_height-2)+1;
    food.y=rand()%(frame_width-2)+1;//在框内
    gotoxy(food.x,food.y);//把光标移动到该坐标
    printf("$");//打印食物

    /*snake的初始化*/
    snake.x[0]=frame_height/2;
    snake.y[0]=frame_width/2;
    gotoxy(snake.x[0],snake.y[0]);
    printf("@");
    snake.len=3;
    snake.speed=200;
    for(k=1; k<snake.len; k++)
    { 
   
        snake.x[k]=snake.x[k-1]+1;
        snake.y[k]=snake.y[k-1];
        gotoxy(snake.x[k],snake.y[k]);
        printf("@");
    }
    /*墙壁*/
    for(j=0; j<frame_width; j++)
    { 
   
        gotoxy(0,j);
        printf("#");
        gotoxy(frame_height-1,j);
        printf("#");
    }
    for(i=0; i<frame_height-1; i++)
    { 
   
        gotoxy(i,0);
        printf("#");
        gotoxy(i,frame_width-1);
        printf("#");
    }
    gotoxy(2,frame_width+3);
    if(n=='1') printf("Difficulty: easy");
    else if(n=='2') printf("Difficulty: middle");
    else if(n=='3') printf("Difficulty: difficult");
    else printf("Welcome to the Devil's difficulty");
    gotoxy(4,frame_width+3);
    printf("UP: w");
    gotoxy(6,frame_width+3);
    printf("DOWN: s");
    gotoxy(8,frame_width+3);
    printf("LEFT: a");
    gotoxy(10,frame_width+3);
    printf("RIGHT:d");
    gotoxy(12,frame_width+3);
    printf("Your score:%d",score);
}
/*食物*/
void update_food()
{ 
   
    if(snake.x[0]==food.x&&snake.y[0]==food.y)//吃到食物
    { 
   
        score+=10;
        gotoxy(12,frame_width+3);
        printf("Your score:%d",score);
        srand(time(NULL));
        /*以下是更新食物的代码,里面排除了 食物与蛇重合的情况,读者可以参考以 下代码完成对上述bug的改进*/
        int flag=1;//标记变量
        do
        { 
   
        food.x=rand()%(frame_height-2)+1;
        food.y=rand()%(frame_width-2)+1;//在框内
        for(i=0; i<snake.len; i++)
        { 
   
            if(food.x==snake.x[i]&&food.y==snake.y[i])
            { 
   
                flag=0;//有重合
                break;
            }
        }
        }
        while(flag==0);
        /*打印食物*/
        gotoxy(food.x,food.y);
        printf("$");
        snake.len++;
        grow=1;//表明长了，在move_snake函数中有用到
    }
}
/*移动蛇*/
void move_snake()
{ 
   
    while(_kbhit())//键盘有输入
    { 
   
        ch=_getch();
    }
    if(!grow)//没有长
    { 
   
        gotoxy(snake.x[snake.len-1],snake.y[snake.len-1]);
        printf(" ");//走了，在数组的最后打印空格，清除原有的蛇尾
    }
    for(k=snake.len-1; k>0; k--)//更新蛇的坐标，除了蛇头,其余位置继承上一个点的坐标
    { 
   
        snake.x[k]=snake.x[k-1];
        snake.y[k]=snake.y[k-1];//移动位置
    }
    switch(ch)//改变方向
    { 
   
    case UP:
    { 
   
        if(state==DOWN)//如果此时方向向下，输入向上的作用要被无视
        { 
   
            snake.x[0]++;
            break;
        }
        else
        { 
   
            snake.x[0]--;
            state=UP;//其余的改变状态为向上
            break;
        }
    }
    case DOWN:
    { 
   
        if(state==UP)
        { 
   
            snake.x[0]--;
            break;
        }
        else
        { 
   
            snake.x[0]++;
            state=DOWN;
            break;
        }
    }
    case LEFT:
    { 
   
        if(state==RIGHT)
        { 
   
            snake.y[0]++;
            break;
        }
        else
        { 
   
            snake.y[0]--;
            state=LEFT;
            break;
        }
    }
    case RIGHT:
    { 
   
        if(state==LEFT)
        { 
   
            snake.y[0]--;
            break;
        }
        else
        { 
   
            snake.y[0]++;
            state=RIGHT;
            break;
        }
    }
    /*摁其余键,保持原有状态*/
    default:
    { 
   
        if(state==DOWN)
        { 
   
            snake.x[0]++;
            break;
        }
        else if(state==UP)
        { 
   
            snake.x[0]--;
            break;
        }
        else if(state==LEFT)
        { 
   
            snake.y[0]--;
            break;
        }
        else if(state==RIGHT)
        { 
   
            snake.y[0]++;
            break;
        }

    }
    }
    gotoxy(snake.x[0],snake.y[0]);
    printf("@");//打印蛇头
    grow=0;//初始成长状态为0
    gotoxy(frame_height,0);//光标移动到地图左下角下方
}
/*存活状态*/
int alive(void)
{ 
   
    if(snake.x[0]==0||snake.x[0]==frame_height-1||snake.y[0]==0||snake.y[0]==frame_width-1)//撞墙
        return 0;
    for(k=1; k<snake.len; k++) //咬到自己
    { 
   
        if(snake.x[0]==snake.x[k]&&snake.y[0]==snake.y[k])
            return 0;
    }
    return 1;
}
/*加速*/
/*speed越大,蛇的速度越小*/
void get_speed()
{ 
   
    if(snake.len<=6)
        snake.speed=sp;
    else if(snake.len<=10)
        snake.speed=sp-20;
    else if(snake.len<=20)
        snake.speed=sp-50;
    else if(snake.len<=30)
        snake.speed=sp-60;
    else
        snake.speed=sp-70;
}
/*移动光标*/
void gotoxy(int x,int y)
{ 
   
    HANDLE hout;
    COORD cor;
    /* typedef struct _COORD { SHORT X; // horizontal coordinate SHORT Y; // vertical coordinate } COORD; 用该结构体来储存坐标 */
    hout=GetStdHandle(STD_OUTPUT_HANDLE);//从标准输出设备中取得一个句柄
    /*这其中x,y的赋值对象要注意,不懂的好好想想*/
    cor.X=y;
    cor.Y=x;
    SetConsoleCursorPosition(hout,cor);//定位光标的函数
}