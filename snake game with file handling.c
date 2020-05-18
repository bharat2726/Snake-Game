#include<stdio.h>
#include<conio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>
#define M 20
#define N 30
int freq[M][N]={0};
struct food{
	
	int x;
	int y;
	char value;
	struct food *next;
};
struct snake{
	int x;
	int y;
	char value;
	struct snake *next;
};
void delay(int ms){
	clock_t starttime=clock();
	while(clock()<starttime+ms)
	{
		if(kbhit())
		{
			break;
		}
	}
	
}
void object(struct food *n){
	int x,y;
	do{
		x=rand()%M;
		y=rand()%N;
	}while(freq[x][y]==1);
	n->x=x;
	n->y=y;
	n->value='$';
	n->next=NULL;
}
struct snake* loadgame(struct snake *start,struct food *start1){
	FILE *fp;
	fp=fopen("save.txt","r");
	int i,j,f=0,temp=0,temp1=0,flag=0;
	char v;
	struct snake *n,*ptr;
	struct food *n1;
	char s[100];
	fgets(s,100,fp); 
	printf("%s\n",s);int ff=0;
	for(i=0;s[i]!='\0';i++)
	{
		if(ff==2)
		{
			ff=0;
		}
		if(s[i]==',')
		{
			if(f==1)
			{
				start1->x=temp;
				start1->y=temp1;
				temp=temp1=0;
				f=0;
			}
			else if(f==2)
			{
				n->x=temp;
				n->y=temp1;
				temp=temp1=0;
				ptr=start;
				if(start!=NULL)
				{
					while(ptr->next!=NULL)
					{
						ptr=ptr->next;
					}
					ptr->next=n;
				}
				else
				{
					start=n;
				}
				
				n->next=NULL;
				f=0;
			}
		}
		else if(s[i]!=',' || s[i]!=' ')
		{
			//printf("*\n");
			if(s[i]=='*' && s[i+1]!='*' && flag==0)
			{
				n=(struct snake*)malloc(sizeof(struct snake));
				//temp=temp1=0;
			}
			else if(s[i]=='*' && s[i+1]=='*')
			{
				flag=1;
				//n1=(struct food*)malloc(sizeof(struct food));
				i++;
				continue;
			}
			if(s[i]=='O' || s[i]=='#' || s[i]=='$' || s[i]=='X')
			{
				if(s[i]=='$')
				{
					start1->value=s[i];
					f=1;
				}
				else
				{
					n->value=s[i];
					f=2;
				}
			}
			else if(s[i]>='0' && s[i]<='9')
			{
				if(ff==1)
				{
					temp1=temp1*10 + (s[i]-48);
				}
				if(ff==0)
				{
					temp=temp*10 + (s[i]-48);
				}
			}
		}
		if(s[i]==' ')
		{
			ff++;
		}
	}
	return start;
}
struct snake* sgame(struct snake *start,struct food *start1,char a[M][N]){
	struct snake *n,*ptr;
	n=(struct snake*)malloc(sizeof(struct snake));
	n->x=1;
	n->y=1;
	n->value='O';
	start=n;
	n->next=NULL;
	a[n->x][n->y]=n->value;
	freq[n->x][n->y]=1;
	ptr=start; 
	n=(struct snake*)malloc(sizeof(struct snake));
	n->x=2;
	n->y=1;
	n->value='#'; 
	ptr->next=n;
	n->next=NULL;
	a[n->x][n->y]=n->value;
	freq[n->x][n->y]=1;
	ptr=start;
	while(ptr->next!=NULL)
	{
		ptr=ptr->next;
	}
	n=(struct snake*)malloc(sizeof(struct snake));
	n->x=3;
	n->y=1;
	n->value='#'; 
	ptr->next=n;
	n->next=NULL;
	a[n->x][n->y]=n->value;
	freq[n->x][n->y]=1;
	ptr=start;
	while(ptr->next!=NULL)
	{
		ptr=ptr->next;
	}
	n=(struct snake*)malloc(sizeof(struct snake));
	n->x=4;
	n->y=1;
	n->value='X'; 
	ptr->next=n;
	n->next=NULL;
	a[n->x][n->y]=n->value;
	freq[n->x][n->y]=1;
	srand(time(0)); ////////////////// generates food at same point when prog is opened after closing it////////////////
	object(start1);
	
	return start;
}
void print(char a[M][N]){
	int i,j;
	for(i=0;i<N+2;i++)
	{
		printf("+");
	}
	printf("\n");
	for(i=0;i<M;i++)
	{
		printf("+");
		for(j=0;j<N;j++)
		{
			printf("%c",a[i][j]);
		}
		printf("+\n");
	}
	for(i=0;i<N+2;i++)
	{
		printf("+");
	}
	printf("\n");
}
void swap(int *a,int *b){
	int t;
	t=*a;
	*a=*b;
	*b=t;
}
struct snake* insert(struct snake *start,int t1,int t2){
	struct snake *ptr=start,*n;
	n=(struct snake*)malloc(sizeof(struct snake));
	while(ptr->next!=NULL)
	{
		ptr=ptr->next;
	}
	ptr->value='#';
	n->value='X';
	n->x=t1;
	n->y=t2;
	ptr->next=n;
	n->next=NULL;
	
	freq[t1][t2]=1;
	return start;
}
void caughtfood(struct snake *start,struct food *start1,int t1,int t2){
	struct snake *ptr;
	struct food *ptr1;
	ptr=start;
	ptr1=start1;
	if(ptr->x==ptr1->x && ptr->y==ptr1->y)
	{
		start=insert(start,t1,t2);
		object(start1);
	}
}
void movement(struct snake *start,struct food *start1,int x,int y){
	struct snake *ptr=start;
	int t1=x,t2=y;
	freq[t1][t2]++;
	while(ptr!=NULL)
	{
		swap(&t1,&(ptr->x));
		swap(&t2,&(ptr->y));
		ptr=ptr->next;
	}
	freq[t1][t2]--;
	caughtfood(start,start1,t1,t2);
}
char getdir(struct snake *start){
	char ch;
	struct snake *ptr;
	int x=0,x1=0;int y=0,y1=0;
	ptr=start;
	x=ptr->x;
	y=ptr->y;
	ptr=ptr->next;
	x1=ptr->x;
	y1=ptr->y;
//	printf("x=%d\ny=%d\nx1=%d\ny1=%d\n",x,y,x1,y1);
	if(abs(x-x1)!=1 && x!=x1)
	{
		if(x==0 && x1==M-1)
		x1=-1;
		else
		x=-1;
	}
	else if(abs(y-y1)!=1 && y!=y1)
	{
		if(y==0 && y1==N-1)
		y1=-1;
		else
		y=-1;
	}
//	printf("x=%d\ny=%d\nx1=%d\ny1=%d\n",x,y,x1,y1);
	if(x==x1 && y<y1)
	ch='a';
	else if(x==x1 && y>y1)
	ch='d';
	else if(y==y1 && x>x1)
	ch='s';
	else if(y==y1 && x<x1)
	ch='w';
	//printf("ch===%c\n",ch);
	return ch;
}
int validmove(char move,struct snake *start){
	if(move!='w' && move!='a' && move!='s' && move!='d')
	return 0;
	char dir;
	dir=getdir(start);
//	printf("%c\n",dir);
	if(dir=='w' && move!='s')
	return 1;
	if(dir=='s' && move!='w')
	return 1;
	if(dir=='a' && move!='d')
	return 1;
	if(dir=='d' && move!='a')
	return 1;
	
	return 0;	
}
void collision(struct snake *start){
	if(freq[start->x][start->y]>1)///////////////////////////////////////******************
	{
		printf("GAME OVER");
		exit(0);
	}
}
void savegame(struct snake *start,struct food *start1){
//	printf("Save Game\n");
	FILE *fp;
	fp=fopen("save.txt","w");
	int x,y,q=1;
	char h,s=' ',ss='*',sss=',',h1[3],h2[3];
	struct snake *ptr=start;
	while(ptr!=NULL)
	{
		//printf("*\n");
		x=ptr->x;
		y=ptr->y;
		h=ptr->value;
		
		fprintf(fp,"*%d %d %c,",x,y,h);

		ptr=ptr->next;
	}
		x=y=0;
		x=start1->x;
		y=start1->y;
		h=start1->value;
		fprintf(fp,"**%d %d %c,",x,y,h);
		
		if(fp!=NULL)
		printf("Game Saved\n");
	fclose(fp);
	exit(0);
}
struct snake* game(struct snake *start,struct food *start1){
	struct snake *ptr;
	ptr=start;
	int x=0,y=0;
	x=ptr->x;
	y=ptr->y;
	delay(100);
	char move;
	if(kbhit()){
	//////keyboard hit
		move=getch();
	}
	else
	{
		move=getdir(start);
	}
	char save;
	if(move=='q')
	{
		printf("Do you want to save the game?y/n\n");
		move=getch();
		while((getchar())!='\n');
		if(move=='y')
		{
			savegame(start,start1);
		}
		else
		{
			printf("GAME OVER");
			exit(0);
		}
	}
	else
	{
		if(validmove(move,start))
		{
			if(move=='w')   ///////////UPWARD MOVEMENT////////////
			{	 
				x--;
				if(x<0)
				{
					x=M-1;
				}
			
			}
			else if(move=='s')     /////////////DOWNWARD MOVEMENT /////////////////
			{
				x++;
				if(x==M)
				{
					x=0;
				}
			}
			else if(move=='a')
			{
				y--;
				if(y<0)
				{
					y=N-1;
				}
			}
			else if(move=='d')
			{
				y++;
				if(y==N)
				{
					y=0;
				}
			}
			movement(start,start1,x,y);
			collision(start);
		}
		else
		{
			printf("Invalid Move\n");
			game(start,start1);
		}
			return start;
	}
}
void setboard(char a[20][30],struct snake *start,struct food *obj){
	struct snake *ptr=start;
	int i,j;
	for(i=0;i<M;i++)
	{
		for(j=0;j<N;j++)
		{
			a[i][j]=' ';
		}
	}
	while(ptr!=NULL)
	{
		a[ptr->x][ptr->y]=ptr->value;
		ptr=ptr->next;
	}
	struct food *ptr1=obj;
	ptr1=obj;
	a[ptr1->x][ptr1->y]=ptr1->value;
	
}
int main(){
	char g;
	static char a[M][N];
	struct snake *start=NULL;
	struct food obj;
	printf("Press N to start a new game\nPress L to load game\nPress 'q' while playing to save game");
	scanf("%c",&g);
	if(g=='n')
	start=sgame(start,&obj,a);
	else if(g=='l')
	start=loadgame(start,&obj);
	while(1)
	{
		setboard(a,start,&obj);
		print(a);
		start=game(start,&obj);
		system("cls");   
	}
}
