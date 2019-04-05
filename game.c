#include <stdio.h>  //wejscie/wyjscie
#include <stdlib.h>  // system("<komenda>");
#include <string.h> // operacje na stringach

#include <termios.h>    //
#include <unistd.h>	// uzyskanie funkcji keypressed() oraz usleep
#include <fcntl.h>	//

//////////////////////////////////DEFINE//////////////////

#define CLEAR_LINE "\x1B[K"
#define TC_BC_NRM  "\x1B[0m"
#define TC_BLC  "\x1B[30m"
#define TC_RED  "\x1B[31m"
#define TC_GRN  "\x1B[32m"
#define TC_YEL  "\x1B[33m"
#define TC_BLU  "\x1B[34m"
#define TC_MAG  "\x1B[35m"
#define TC_CYA  "\x1B[36m"
#define TC_WHT  "\x1B[37m"
#define BC_RED  "\x1B[41m"
#define BC_GRN  "\x1B[42m"
#define BC_YEL  "\x1B[43m"
#define BC_BLU  "\x1B[44m"
#define BC_MAG  "\x1B[45m"
#define BC_CYA  "\x1B[46m"
#define BC_WHT  "\x1B[47m"

////////////////////////////////WLASNE TYPY///////////////////////////

struct type_tank{     			//czolgi
	int x;
	int y;
	int c;
	char clr[12];
	int hp;
	int licznik;
	int board[42][42]; 
};

struct type_enemy_base{			//spawny przeciwnikow
	int x;
	int y;
};

struct type_spawn{				//nasze spawny
	int x;
	int y;
};

struct type_difficulty{			//poziom trudnosci
	int time;
	int hp;
	int enemies;
};
	
struct type_bullet{				//pociski
	int x;
	int y;
	int c;
	int type;
	int is;
};

struct type_boom{				//WYBUCHYY
	int x;
	int y;
	int is;
};
	

/////////////////////////GLOBALNE ZMIENNE////////////////////


struct type_tank tank[7];  //czolgi
struct type_enemy_base enemy_base[8];  
struct type_spawn spawns[5];
struct type_difficulty dif[4];
struct type_bullet bullets[61];
struct type_boom booms[7];



char kol[12];				//kolor tekstu/tla
int board[45][45];    		//plansza
int enemies;  				//liczba przeciwnikow
int level;  				//poziom trudnosci
int zycie=3; 				//zycie
int base_hp=3;				//zycie bazy
int score=0;  				//punkty
int map_no; 				//numer mapy
int endgame=0; 				//czy gra ma sie skonczyc
char map[15][10];			//tablica z nazwami map xd
char painting[10][20];		//tablica z nazwami obrazkow
int can_i_start=0;			//czy moze zaczac gre xd



void frame(int x1,int y1,int x2, int y2);  					//ramka
void frame2(int x1,int y1,int x2, int y2);					//ramka2
int direction(int c);										//a taki tam wielomian do zamienienia reszty z dzielenia przez 4 na kierunki po kodach ascii strzalek...
int been(int no, int c);									//dla level'u 2 - czy czolg juz tam byl - zwarca wartosc tank[no].board[i][j];
int can_me_smash(int no);									//dla level'u 3 - asks if enemy can smash or not...
int spawnable(int x,int y);            						//czy mozna w danym miejscu zespawnowac przeciwnika
int keypressed(void);										//czy klawisz zostal wcisniety
void draw_tank(int no,int c);								//rysuje czolg o numerze "no" i kierunku "c"
int spawn_enemy();											//spawnuje przeciwnika
int movable(int no, int c);									//czy przeciwnik moze tam jechac
int breakable(int no, int c);								//czy przeciwnik moze zniszczyc cos przed soba
int move_tank(int no, int c);								//rusza czolg numer "no" w kierunku "c"
void draw_char(int z);										//rysuje chara - rysowanie mapy
void draw_map();											//rysuje mape
void boom(int no);											//booooooooooooom
void delete_bullet(int bullet_no);							//usuwa wizualnie pocisk
void move_bullet(int bullet_no, int x, int y);				//rusza pocisk numer "bullet_no" na pole (x,y)
void collision_bullet(int bullet_no, int xxx, int yyy);		//kolizja pocisku z czyms na polu (x,y)
void refresh_bullets();										//odswieza wszystkie pociski
void shoot(int tank_no);									//czolg numer "tank_no" strzela
void think(int no);											//NAJGLOWNIEJSZY PUNKT PROGRAMU RDZEN, OSOBLIWOSC
void refresh_enemies();										//odswieza przeciwnikow
void gameplay();											//rozgrywka
void menu();												//menu przed gra
void end_of_game();											//koniec gry, podliczenie punktow
void paint(int noumber);									//rysuje obrazki
void high_scores();											//pokazuje najlepsze wyniki
void start_up();											//ladowanie przed kazda mapa
void loading();												//ladowanie stalych i takich tam



/////////////////////////////////GOTOXY///////////////////////
void gotoxy(int xxx,int yyy){
	printf("%c[%d;%df",0x1B,yyy,xxx);
}	

//////////////////////////////////FRAME////////////////

void frame(int x1,int y1,int x2, int y2){
	int i;
	
	gotoxy(x1,y1);
	printf("%c",201);
	gotoxy(x1,y2);
	printf("%c",200);
	gotoxy(x2,y1);
	printf("%c",187);
	gotoxy(x2,y2);
	printf("%c",188);
	
	gotoxy(x1+1,y1);
	for (i=1;i<=(x2-x1-1);i++)
		printf("%c",205);
	
	gotoxy(x1+1,y2);
	for (i=1;i<=(x2-x1-1);i++)
		printf("%c",205);
	
	for (i=1;i<=(y2-y1-1);i++){
		gotoxy(x1,y1+i);
		printf("%c",186);
	}
	
	for (i=1;i<=(y2-y1-1);i++){
		gotoxy(x2,y1+i);
		printf("%c",186);
	}
}

//////////////////////////////////FRAME2////////////////

void frame2(int x1,int y1,int x2, int y2){
	int i;
	
	gotoxy(x1,y1);
	printf("%c",218);
	gotoxy(x1,y2);
	printf("%c",192);
	gotoxy(x2,y1);
	printf("%c",191);
	gotoxy(x2,y2);
	printf("%c",217);
	
	gotoxy(x1+1,y1);
	for (i=1;i<=(x2-x1-1);i++)
		printf("%c",196);
	
	gotoxy(x1+1,y2);
	for (i=1;i<=(x2-x1-1);i++)
		printf("%c",196);
	
	for (i=1;i<=(y2-y1-1);i++){
		gotoxy(x1,y1+i);
		printf("%c",179);
	}
	
	for (i=1;i<=(y2-y1-1);i++){
		gotoxy(x2,y1+i);
		printf("%c",179);
	}
}


////////////////////////////////////////////DIRECTION//////////////////////////////////////
int direction(int c){
	int x;
	x=c*c*c-4.5*c*c+5.5*c+65;
	return x;
}


///////////////////////////////////////////////BEEN///////////////////////////////////////
int been(int no, int c){
	int x1,x2,x3,y1,y2,y3,s;
	
	switch(c){
		case 65:
			x1=tank[no].x-1;
			x2=tank[no].x;
			x3=tank[no].x+1;
			y1=y2=y3=tank[no].y-2;
		break;
		case 67:
			x1=x2=x3=tank[no].x+2;
			y1=tank[no].y-1;
			y2=tank[no].y;
			y3=tank[no].y+1;
		break;
		case 66:
			x1=tank[no].x+1;
			x2=tank[no].x;
			x3=tank[no].x-1;
			y1=y2=y3=tank[no].y+2;
		break;
		case 68:
			x1=x2=x3=tank[no].x-2;
			y1=tank[no].y+1;
			y2=tank[no].y;
			y3=tank[no].y-1;
		break;
	}
	s=tank[no].board[x1][y1]+tank[no].board[x2][y2]+tank[no].board[x3][y3];
	return s;
}

/////////////////////////////////////////CAN_ME_SMASH///////////////////////////////////////
int can_me_smash(int no){
	int i=1;
	
	//65
	do{
		i++;
		if ((board[tank[no].x][tank[no].y-i]==16)||(board[tank[no].x][tank[no].y-i]==98))
			return 65;		
	} while ((board[tank[no].x][tank[no].y-i]==48)||(board[tank[no].x][tank[no].y-i]==11));
	
	//67
	i=1;
	do{
		i++;
		if ((board[tank[no].x+i][tank[no].y]==16)||(board[tank[no].x+i][tank[no].y]==98))
			return 67;		
	} while ((board[tank[no].x+i][tank[no].y]==48)||(board[tank[no].x+i][tank[no].y]==11));
	
	//66
	i=1;
	do{
		i++;
		if ((board[tank[no].x][tank[no].y+i]==16)||(board[tank[no].x][tank[no].y+i]==98))
			return 66;		
	} while ((board[tank[no].x][tank[no].y+i]==48)||(board[tank[no].x][tank[no].y+i]==11));
	
	//68
	i=1;
	do{
		i++;
		if ((board[tank[no].x-i][tank[no].y]==16)||(board[tank[no].x-i][tank[no].y]==98))
			return 68;		
	} while ((board[tank[no].x-i][tank[no].y]==48)||(board[tank[no].x-i][tank[no].y]==11));
	
	return 0;
}
///////////////////////////////////////////SPAWNABLE//////////////////////////////////////

int spawnable(int x,int y){
	if ((board[x-1][y-1]==48)&&(board[x-1][y]==48)&&(board[x-1][y+1]==48)&&(board[x][y-1]==48)&&(board[x][y+1]==48)&&(board[x+1][y-1]==48)&&(board[x+1][y]==48)&&(board[x+1][y+1]==48)) 
		return 1;
	else
		return 0;
}


/////////////////////////////////////////////KEYPRESSED//////////////////////

int keypressed(void){
	struct termios oldt, newt;
	int ch;
	int oldf;
 
	tcgetattr(STDIN_FILENO, &oldt);
	newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);
	oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
	fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
 
	ch = getchar();
 
	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
  	fcntl(STDIN_FILENO, F_SETFL, oldf);
 
	if(ch != EOF){		
    	ungetc(ch, stdin);
    	return 1;
  	}
 
  	return 0;
}

/////////////////////////////////////////////////DRAW_TANK/////////////////////////////////	

void draw_tank(int no,int c){
	int x,y,xx,yy,cc;
	x=tank[no].x;
	y=tank[no].y;
	printf("%s",tank[no].clr);
	gotoxy(x-1,y-1);
	printf("###");
	gotoxy(x-1,y);
	printf("###");
	gotoxy(x-1,y+1);
	printf("###");
	
	gotoxy(tank[no].x,tank[no].y);
	printf("%d",tank[no].hp);
	
	if (no==0)
		board[x-1][y-1]=board[x-1][y]=board[x-1][y+1]=board[x][y-1]=board[x][y]=board[x][y+1]=board[x+1][y-1]=board[x+1][y]=board[x+1][y+1]=16;
	else{
		board[x-1][y-1]=board[x-1][y]=board[x-1][y+1]=board[x][y-1]=board[x][y]=board[x][y+1]=board[x+1][y-1]=board[x+1][y]=board[x+1][y+1]=10;
		if (level>=2)
			if (tank[no].board[x-1][y-1]==0)
				if (tank[no].board[x-1][y]==0)
					if (tank[no].board[x-1][y+1]==0)
						if (tank[no].board[x][y-1]==0)
							if (tank[no].board[x][y]==0)
								if (tank[no].board[x][y+1]==0)
									if (tank[no].board[x+1][y-1]==0)
										if (tank[no].board[x+1][y]==0)
											if (tank[no].board[x+1][y+1]==0)
												tank[no].board[x-1][y-1]=tank[no].board[x-1][y]=tank[no].board[x-1][y+1]=tank[no].board[x][y-1]=tank[no].board[x][y]=tank[no].board[x][y+1]=tank[no].board[x+1][y-1]=tank[no].board[x+1][y]=tank[no].board[x+1][y+1]=1;
												
	}
	
	switch (c){
		case 65:
			xx=x;
			yy=y-1;
			cc=186;
		break;
		case 67:
			xx=x+1;
			yy=y;
			cc=205;
		break;
		case 66:
			xx=x;
			yy=y+1;
			cc=186;
		break;
		case 68:
			xx=x-1;
			yy=y;
			cc=205;
		break;
	}
	gotoxy(xx,yy);
	printf("%c",cc);
	gotoxy(1,42);
	printf(CLEAR_LINE);
	printf(TC_BC_NRM);
}

////////////////////////////////////////SPAWN_ENEMY/////////////////////////////////////////////
int spawn_enemy(){
	int i,j,no;
	for (i=1;i<=6;i++){
		if (spawnable(enemy_base[i].x,enemy_base[i].y)){
			for (j=1;j<=5;j++){
				if (tank[j].hp==0){
					no=j;
					//break;
				}
			}			
			
			//printf("%s",tank[no].clr);
			tank[no].x=enemy_base[i].x;
			tank[no].y=enemy_base[i].y;
			tank[no].hp=dif[level].hp;
			tank[no].c=66;
			draw_tank(no,66);
			printf(TC_BC_NRM);
			return 1;
		}
	}
	return 0;
}


//////////////////////////////////////////////////SPAWN_ALLY//////////////////////////////////
int spawn_ally(){
	int i;
	
	for (i=1;i<=4;i++){
		if (spawnable(spawns[i].x,spawns[i].y)){
			printf(TC_YEL);
			tank[0].x=spawns[i].x;
			tank[0].y=spawns[i].y;
			tank[0].hp=3;
			tank[0].c=66;
			draw_tank(0,tank[0].c);
			printf(TC_BC_NRM);
			return 1;
		}
	}
	return 0;
}

//////////////////////////////////////////////////MOVABLE/////////////////////////////////////
int movable(int no, int c){
	int xx1,xx2,xx3,yy1,yy2,yy3;
	
	switch (c){
		case 65:
			xx1=tank[no].x-1;
			xx2=tank[no].x;
			xx3=tank[no].x+1;
			yy1=tank[no].y-2;
			yy2=tank[no].y-2;
			yy3=tank[no].y-2;
		break;
		case 67:
			xx1=tank[no].x+2;
			xx2=tank[no].x+2;
			xx3=tank[no].x+2;
			yy1=tank[no].y-1;
			yy2=tank[no].y;
			yy3=tank[no].y+1;
		break;
		case 66:
			xx1=tank[no].x-1;
			xx2=tank[no].x;
			xx3=tank[no].x+1;
			yy1=tank[no].y+2;
			yy2=tank[no].y+2;
			yy3=tank[no].y+2;
		break;
		case 68:
			xx1=tank[no].x-2;
			xx2=tank[no].x-2;
			xx3=tank[no].x-2;
			yy1=tank[no].y-1;
			yy2=tank[no].y;
			yy3=tank[no].y+1;
		break;
	}
	
	if ((board[xx1][yy1]==48)&&(board[xx2][yy2]==48)&&(board[xx3][yy3]==48))
		return 1;
	else
		return 0;
}


//////////////////////////////////////////////////BREAKABLE/////////////////////////////////////
int breakable(int no, int c){
	int xx1,xx2,xx3,yy1,yy2,yy3;
	
	switch (c){
		case 65:
			xx1=tank[no].x-1;
			xx2=tank[no].x;
			xx3=tank[no].x+1;
			yy1=tank[no].y-2;
			yy2=tank[no].y-2;
			yy3=tank[no].y-2;
		break;
		case 67:
			xx1=tank[no].x+2;
			xx2=tank[no].x+2;
			xx3=tank[no].x+2;
			yy1=tank[no].y-1;
			yy2=tank[no].y;
			yy3=tank[no].y+1;
		break;
		case 66:
			xx1=tank[no].x-1;
			xx2=tank[no].x;
			xx3=tank[no].x+1;
			yy1=tank[no].y+2;
			yy2=tank[no].y+2;
			yy3=tank[no].y+2;
		break;
		case 68:
			xx1=tank[no].x-2;
			xx2=tank[no].x-2;
			xx3=tank[no].x-2;
			yy1=tank[no].y-1;
			yy2=tank[no].y;
			yy3=tank[no].y+1;
		break;
	}
	
	if (((board[xx1][yy1]==48)||(board[xx1][yy1]==49))&&(board[xx2][yy2]==49)&&((board[xx3][yy3]==48)||(board[xx3][yy3]==49)))
		return 1;
	else if (((board[xx1][yy1]==48)||(board[xx1][yy1]==49)||(board[xx1][yy1]==50))&&(board[xx2][yy2]==50)&&((board[xx3][yy3]==48)||(board[xx3][yy3]==49)||(board[xx1][yy1]==50)))
		return 2;
	else if (((board[xx1][yy1]==48)||(board[xx1][yy1]==49)||(board[xx1][yy1]==50)||(board[xx1][yy1]==53))&&(board[xx2][yy2]==53)&&((board[xx3][yy3]==48)||(board[xx3][yy3]==49)||(board[xx1][yy1]==50)||(board[xx1][yy1]==53)))
		return 3;
	else
		return 0;
}



///////////////////////////////////////////////////MOVE_TANK//////////////////////////////////

int move_tank(int no, int c){
	int xx1,xx2,xx3,yy1,yy2,yy3,cc,lx,ly,tx1,tx2,tx3,ty1,ty2,ty3;

	if (tank[no].c!=c){
		draw_tank(no,c);
		tank[no].c=c;
		return 3;
	}
	
	switch (c){
		case 65:
			xx1=tank[no].x-1;
			xx2=tank[no].x;
			xx3=tank[no].x+1;
			yy1=tank[no].y-2;
			yy2=tank[no].y-2;
			yy3=tank[no].y-2;
			lx=xx2;
			ly=yy2+1;
			tx1=xx1;
			tx2=xx2;
			tx3=xx3;
			ty1=yy1+3;
			ty2=yy2+3;
			ty3=yy3+3;
			cc=186;
		break;
		case 67:
			xx1=tank[no].x+2;
			xx2=tank[no].x+2;
			xx3=tank[no].x+2;
			yy1=tank[no].y-1;
			yy2=tank[no].y;
			yy3=tank[no].y+1;
			lx=xx2-1;
			ly=yy2;
			tx1=xx1-3;
			tx2=xx2-3;
			tx3=xx3-3;
			ty1=yy1;
			ty2=yy2;
			ty3=yy3;
			cc=205;
		break;
		case 66:
			xx1=tank[no].x-1;
			xx2=tank[no].x;
			xx3=tank[no].x+1;
			yy1=tank[no].y+2;
			yy2=tank[no].y+2;
			yy3=tank[no].y+2;
			lx=xx2;
			ly=yy2-1;
			tx1=xx1;
			tx2=xx2;
			tx3=xx3;
			ty1=yy1-3;
			ty2=yy2-3;
			ty3=yy3-3;
			cc=186;
		break;
		case 68:
			xx1=tank[no].x-2;
			xx2=tank[no].x-2;
			xx3=tank[no].x-2;
			yy1=tank[no].y-1;
			yy2=tank[no].y;
			yy3=tank[no].y+1;
			lx=xx2+1;
			ly=yy2;
			tx1=xx1+3;
			tx2=xx2+3;
			tx3=xx3+3;
			ty1=yy1;
			ty2=yy2;
			ty3=yy3;
			cc=205;
		break;
	}
	
	if ((board[xx1][yy1]==48)&&(board[xx2][yy2]==48)&&(board[xx3][yy3]==48)){
		printf("%s",tank[no].clr);
		gotoxy(xx1,yy1);
		printf("#");
		gotoxy(xx2,yy2);
		printf("%c",cc);
		gotoxy(xx3,yy3);
		printf("#");
		gotoxy(lx,ly);
		printf("#");
		gotoxy(tx1,ty1);
		printf(" ");
		gotoxy(tx2,ty2);
		printf(" ");
		gotoxy(tx3,ty3);
		printf(" ");
		
		gotoxy(tank[no].x,tank[no].y);
		printf("#");
		
		tank[no].x=lx;
		tank[no].y=ly;
	
		
		gotoxy(tank[no].x,tank[no].y);
		printf("%d",tank[no].hp);
		
		if (no==0)
			board[xx1][yy1]=board[xx2][yy2]=board[xx3][yy3]=16;
		else
			board[xx1][yy1]=board[xx2][yy2]=board[xx3][yy3]=10;
		
		board[tx1][ty1]=board[tx2][ty2]=board[tx3][ty3]=48;
		
		if (level>=2){			
			//tank[no].board[lx][ly]=tank[no].board[lx][ly]+1;
			tank[no].board[xx1][yy1]=tank[no].board[xx1][yy1]+1;
			tank[no].board[xx2][yy2]=tank[no].board[xx2][yy2]+1;
			tank[no].board[xx3][yy3]=tank[no].board[xx3][yy3]+1;
		}
		
		printf(TC_BC_NRM);
		//printf("Poszlo");
		gotoxy(1,42);
		printf(CLEAR_LINE);
		return 1;
	}
	return 0;
}
	

/////////////////////////////////////////DRAW_CHAR////////////////////////////////////

void draw_char(int z){
	int x;
	
	switch (z){
		case 48:
			x=32;
			strcpy(kol,TC_BC_NRM);
		break;
		case 49:
			x=37;
			strcpy(kol,TC_RED);
		break;
		case 50:
			x=37;
			strcpy(kol,TC_CYA);
		break;
		case 51:
			x=32;
			strcpy(kol,BC_BLU);
		break;
		case 52:
			x=32;
			strcpy(kol,BC_WHT);
		break;
		case 53:
			x=37;
			strcpy(kol,TC_BLC);
		break;
		case 56:
			x=32;
			strcpy(kol,TC_BC_NRM);
		break;
		case 57:
			x=32;
			strcpy(kol,TC_BC_NRM);
		break;
		case 98:
			x=66;
			strcpy(kol,TC_MAG);
		break;
	}

	//	printf("z=%d x=%d %s kol\n",z,x,kol);
	printf("%s%c",kol,x);
	printf(TC_BC_NRM);
}
			
	
//////////////////////////////////////////////////DRAW_MAP////////////////////////////////

void draw_map(){

	//fgets(map_name,3,stdin);
	//scanf("%5s",map_name);
	//FILE *t=fopen(map[map_no],"rt");
	
	system("clear");

	FILE *t;
	if ((t=fopen(map[map_no],"rt"))==NULL){
    	printf ("Nie moge otworzyc mapy \"%s\"! Sprawdz, czy nie brakuje jej w katalogu gry!\n",map[map_no]);
		exit(1);
    }
	
	char s[50];
	int i,j;
	int enemy_base_noumber=1;
	int spawn_noumber=1;
	
	for (i=1;i<=39;i++){		
		fgets(s, 50, t);
		gotoxy(2,i+1);
		
		for (j=0;j<=38;j++){
			draw_char(s[j]);
			board[j+2][i+1]=s[j];
			if (s[j]==56){
				spawns[spawn_noumber].x=j+2;
				spawns[spawn_noumber].y=i+1;
				spawn_noumber=spawn_noumber+1;
				board[j+2][i+1]=48;
			}
			if (s[j]==57){
				enemy_base[enemy_base_noumber].x=j+2;
				enemy_base[enemy_base_noumber].y=i+1;
				enemy_base_noumber=enemy_base_noumber+1;
				board[j+2][i+1]=48;
			}
		}
	}
	
	//int u;
	//u=getchar();
	
	fclose(t);
}
///////////////////////////////////////////REFRESH_BOOMS//////////////////////////////////////
void refresh_booms(){
	int i;
	
	for (i=1;i<=6;i++){
		if (booms[i].is==1){
			printf(TC_BC_NRM);
			gotoxy(booms[i].x-1,booms[i].y-1);
			printf("   ");
			gotoxy(booms[i].x-1,booms[i].y);
			printf("   ");
			gotoxy(booms[i].x-1,booms[i].y+1);
			printf("   ");
			board[booms[i].x-1][booms[i].y-1]=board[booms[i].x-1][booms[i].y]=board[booms[i].x-1][booms[i].y+1]=board[booms[i].x][booms[i].y-1]=board[booms[i].x][booms[i].y]=board[booms[i].x][booms[i].y+1]=board[booms[i].x+1][booms[i].y-1]=board[booms[i].x+1][booms[i].y]=board[booms[i].x+1][booms[i].y+1]=48;
			booms[i].is=0;
			if (enemies>=5)
				if (spawn_enemy()==0)
					booms[i].is=1;
		}
		else if (booms[i].is>1)
			booms[i].is=booms[i].is-1;
	}
	
	
	i=0;
		
	if (booms[i].is==1){
		printf(TC_BC_NRM);
		gotoxy(booms[i].x-1,booms[i].y-1);
		printf("   ");
		gotoxy(booms[i].x-1,booms[i].y);
		printf("   ");
		gotoxy(booms[i].x-1,booms[i].y+1);
		printf("   ");
		board[booms[i].x-1][booms[i].y-1]=board[booms[i].x-1][booms[i].y]=board[booms[i].x-1][booms[i].y+1]=board[booms[i].x][booms[i].y-1]=board[booms[i].x][booms[i].y]=board[booms[i].x][booms[i].y+1]=board[booms[i].x+1][booms[i].y-1]=board[booms[i].x+1][booms[i].y]=board[booms[i].x+1][booms[i].y+1]=48;
		booms[i].is=0;
		if (zycie>0)
			if (spawn_ally()==0)
				booms[i].is=1;

		}
	else if (booms[i].is>1)
		booms[i].is=booms[i].is-1;
	
	gotoxy(1,42);
	printf(CLEAR_LINE);
		
}
////////////////////////////////////////////BOOM//////////////////////////////////////////////
void boom(int no){
	int i,j,boom_no;
	
	gotoxy(tank[no].x-1,tank[no].y-1);
	printf(TC_YEL);
	printf("B");
	printf(TC_RED);
	printf("O");
	printf(TC_YEL);
	printf("O");
	gotoxy(tank[no].x-1,tank[no].y);
	printf(TC_RED);
	printf("O");
	printf(TC_YEL);
	printf("O");
	printf(TC_RED);
	printf("O");
	gotoxy(tank[no].x-1,tank[no].y+1);
	printf(TC_YEL);
	printf("O");
	printf(TC_RED);
	printf("O");
	printf(TC_YEL);
	printf("M");
	printf(TC_BC_NRM);
	gotoxy(70,8);
	board[tank[no].x-1][tank[no].y-1]=board[tank[no].x][tank[no].y-1]=board[tank[no].x+1][tank[no].y-1]=board[tank[no].x-1][tank[no].y]=board[tank[no].x][tank[no].y]=board[tank[no].x+1][tank[no].y]=board[tank[no].x-1][tank[no].y+1]=board[tank[no].x][tank[no].y+1]=board[tank[no].x+1][tank[no].y+1]=15;
	
	for (i=1;i<=6;i++)
		if (booms[i].is==0){
			boom_no=i;
			break;
		}
	
	if (no==0){
		boom_no=0;
		zycie=zycie-1;
		gotoxy(45,7);
		printf(CLEAR_LINE);
		printf("Ilosc zyc: %d",zycie);
		if (zycie==0)
			endgame=1;
	}
	else{
		enemies=enemies-1;
		if (level>=2)
			for (i=0;i<=41;i++)
				for (j=0;j<=41;j++)
					tank[no].board[i][j]=0;
		gotoxy(45,5);
		printf(CLEAR_LINE);
		printf("Ilosc wrogow: %d",enemies);
	}

	booms[boom_no].is=5;
	booms[boom_no].x=tank[no].x;
	booms[boom_no].y=tank[no].y;
	
	if (no!=0){
		score=score+level;
		gotoxy(45,9);
		printf(CLEAR_LINE);
		printf("Ilosc punktow: %d",score);
	}
	//niszczy czolg o numerze no, piszac w jego miejsce "BOOOOOOOOOOM" i robi podwaliny pod refresh_booms, ktora bedzie sprzatac i respwanowac enemiesow
	//jezeli enemies>0
	gotoxy(1,42);
	printf(CLEAR_LINE);
}

////////////////////////////////////////DELETE_BULLET////////////////////////////////////////
void delete_bullet(int bullet_no){
	if (board[bullets[bullet_no].x][bullets[bullet_no].y]==11){
		printf(TC_BC_NRM);
		gotoxy(bullets[bullet_no].x,bullets[bullet_no].y);
		printf(" ");
		board[bullets[bullet_no].x][bullets[bullet_no].y]=48;
	}
	else if (board[bullets[bullet_no].x][bullets[bullet_no].y]==12){
		printf(BC_BLU);
		gotoxy(bullets[bullet_no].x,bullets[bullet_no].y);
		printf(" ");
		board[bullets[bullet_no].x][bullets[bullet_no].y]=51;
	}
	else if (board[bullets[bullet_no].x][bullets[bullet_no].y]==13)
		board[bullets[bullet_no].x][bullets[bullet_no].y]=10;
		
	printf(TC_BC_NRM);
	gotoxy(1,42);
	printf(CLEAR_LINE);
}
	
	

	//rysuje spacje w miejscu wspolrzednych xd ZMIANA WARTOSCI BOARD



///////////////////////////////////////MOVE_BULLET///////////////////////////////////////////
void move_bullet(int bullet_no, int x, int y){
	gotoxy(x,y);
	printf(TC_WHT);
	if (board[x][y]==51) 
		printf(BC_BLU);
	printf("%c",207);
	printf(TC_BC_NRM);
	delete_bullet(bullet_no);
	if (board[x][y]==48||(board[x][y]==56)||(board[x][y]==57))
		board[x][y]=11;
	else
		board[x][y]=12;
	bullets[bullet_no].x=x;
	bullets[bullet_no].y=y;
	gotoxy(1,42);
	printf(CLEAR_LINE);
}
	
	//juz wie, ze jest tam przelatywalne pole, jeszcze musi sie upewnic, czy nie woda, i rysuje nowy pocisk w x,y, a usuwa tego z bullet[bullet_no].x/y.
	//w przypadku wody musi chyba wprowadzic nowa wartosc pola, albo cos takiego... I NIE ZAPOMNIJ ZMIENIAC WARTOSC BOARD


///////////////////////////////////////COLLISION_BULLET//////////////////////////////////////
void collision_bullet(int bullet_no, int xxx, int yyy){
	int x[4],y[4],i,no,no2;
	no=6;
	x[1]=xxx;
	y[1]=yyy;
	x[2]=y[2]=x[3]=y[3]=1;
	if ((board[xxx][yyy]==49)||(board[xxx][yyy]==50)||(board[xxx][yyy]==53)){
		switch (bullets[bullet_no].c){
			case 65:
				x[2]=xxx-1;
				y[2]=y[3]=yyy;
				x[3]=xxx+1;
			break;
			case 67:
				x[2]=x[3]=xxx;
				y[2]=yyy-1;
				y[3]=yyy+1;
			break;
			case 66:
				x[2]=xxx+1;
				y[2]=y[3]=yyy;
				x[3]=xxx-1;
			break;
			case 68:
				x[2]=x[3]=xxx;
				y[2]=yyy+1;
				y[3]=yyy-1;
			break;
		}
	}

	
	for (i=1;i<=3;i++){		
		if (board[x[i]][y[i]]==49){
			printf(TC_BC_NRM);
			gotoxy(x[i],y[i]);
			printf(" ");
			board[x[i]][y[i]]=48;
		}
			
		if (board[x[i]][y[i]]==50){
			printf(TC_RED);
			gotoxy(x[i],y[i]);
			printf("%c",37);
			board[x[i]][y[i]]=49;
		}
		
		if (board[x[i]][y[i]]==53){
			printf(TC_CYA);
			gotoxy(x[i],y[i]);
			printf("%c",37);
			board[x[i]][y[i]]=50;
		}
	}
	
	if ((board[xxx][yyy]==10)&&(bullets[bullet_no].type==0)){	
		switch (bullets[bullet_no].c){
			case 65:
				x[1]=xxx-1;
				x[2]=xxx;
				x[3]=xxx+1;
				y[1]=y[2]=y[3]=yyy-1;
			break;
			case 67:
				y[1]=yyy-1;
				y[2]=yyy;
				y[3]=yyy+1;
				x[1]=x[2]=x[3]=xxx+1;
			break;
			case 66:
				x[1]=xxx+1;
				x[2]=xxx;
				x[3]=xxx-1;
				y[1]=y[2]=y[3]=yyy+1;
			break;
			case 68:
				y[1]=yyy+1;
				y[2]=yyy;
				y[3]=yyy-1;
				x[1]=x[2]=x[3]=xxx-1;
			break;
		}
		
		for (i=1;i<=5;i++)
			if (((tank[i].x==x[1])||(tank[i].x==x[2])||(tank[i].x==x[3]))&&((tank[i].y==y[1])||(tank[i].y==y[2])||(tank[i].y==y[3])))
				no=i;                      //znalazl trafiony czolg
		if (tank[no].hp>0)
			tank[no].hp=tank[no].hp-1;
		if (tank[no].hp==0)
			boom(no);
			//trafienie w czolg;	
	}
	
	if ((board[xxx][yyy]==16)&&(bullets[bullet_no].type==1)){										//trafienie naszego czolgu
		if (tank[0].hp>0){
			tank[0].hp=tank[0].hp-1;
			gotoxy(tank[0].x,tank[0].y);
			printf(TC_YEL);
			printf("%d",tank[0].hp);
			printf(TC_BC_NRM);
			gotoxy(1,42);
			printf(CLEAR_LINE);
		}
		if (tank[0].hp==0)
			boom(0);
	}
	
	if ((board[xxx][yyy]==98)&&(bullets[bullet_no].type==1)){                       			//trafienie w baze przez przeciwnika - zmienic na 1;                
		base_hp=base_hp-1;
		if (base_hp==0)
			endgame=1;
	}	
	
	if ((board[xxx][yyy]==11)||(board[xxx][yyy]==12)){  						//trafienie pocisku w pocisk
		no2=0;
		for (i=1;i<=30;i++)
			if ((bullets[i].x==xxx)&&(bullets[i].y==yyy)){				
				no2=i;
				break;
			}
		
		if (bullets[no2].type==0)
			tank[0].licznik=tank[0].licznik-1;
		delete_bullet(no2);
		bullets[no2].is=0;
	}
	
	//no to jakis switch w zaleznosci co tam spotyka, zawsze eliminuje na koncu pocisk, ale czasem robi jeszcze troszke rozpierduchy xd powodzenia
	gotoxy(1,42);
	printf(CLEAR_LINE);
	if (bullets[bullet_no].type==0)
		tank[0].licznik=tank[0].licznik-1;
	delete_bullet(bullet_no);
	bullets[bullet_no].is=0;
}

////////////////////////////////////////REFRESH_BULLETS//////////////////////////////////////
void refresh_bullets(){
	int i,x,y;
	
	for (i=1;i<=60;i++){
		if (bullets[i].is==1){
			switch (bullets[i].c){
				case 65:
					x=bullets[i].x;
					y=bullets[i].y-1;
				break;
				case 67:
					x=bullets[i].x+1;
					y=bullets[i].y;
				break;
				case 66:
					x=bullets[i].x;
					y=bullets[i].y+1;
				break;
				case 68:
					x=bullets[i].x-1;
					y=bullets[i].y;
				break;
			}
			if ((board[x][y]==48)||(board[x][y]==51)||(board[x][y]==56)||(board[x][y]==57)) 
				move_bullet(i,x,y);
			else				
				collision_bullet(i,x,y);			
		}
	}
}
			


/////////////////////////////////////////SHOOT//////////////////////////////////
void shoot(int tank_no){
	int i,no,x,y,x2,y2;
	switch(tank[tank_no].c){
		case 65:
			x=x2=tank[tank_no].x;
			y=tank[tank_no].y-2;		
			y2=y+1;
		break;
		case 67:		
			x=tank[tank_no].x+2;
			x2=x-1;
			y=y2=tank[tank_no].y;
		break;
		case 66:
			x=x2=tank[tank_no].x;
			y=tank[tank_no].y+2;
			y2=y-1;
		break;
		case 68:
			x=tank[tank_no].x-2;
			x2=x+1;
			y=y2=tank[tank_no].y;
		break;
	}
	
	if ((board[x][y]==48)||(board[x][y]==51)||(board[x][y]==57)){	
		no=0;
		for (i=1;i<=60;i++)
			if (bullets[i].is==0){				
				no=i;
				break;
			}
		bullets[no].x=x;
		bullets[no].y=y;
		bullets[no].c=tank[tank_no].c;
		bullets[no].is=1;
		
		if (tank_no==0){
			bullets[no].type=0;
			tank[0].licznik=tank[0].licznik+1;
		}
		else if (tank_no>0)
			bullets[no].type=1;
			
		if ((board[x][y]==48)||(board[x][y]==10))			
			board[x][y]=11;
		else if (board[x][y]==51)
			board[x][y]=12;
		
		gotoxy(bullets[no].x,bullets[no].y);
		
		printf(TC_WHT);
		if (board[x][y]==51)
			printf(BC_BLU);
		printf("%c",207);
		
		printf(TC_BC_NRM);		
		gotoxy(1,42);
		printf(CLEAR_LINE);
	}
	else if ((board[x][y]==10)||(board[x][y]==49)||(board[x][y]==50)||(board[x][y]==53)||(board[x][y]==16)||(board[x][y]==98)||(board[x][y]==11)||(board[x][y]==12)){
		no=0;
		for (i=1;i<=60;i++)
			if (bullets[i].is==0){
				no=i;
				break;
			}
		bullets[no].x=x2;
		bullets[no].y=y2;
		bullets[no].c=tank[tank_no].c;
		bullets[no].is=1;	
		
		if (tank_no==0){
			bullets[no].type=0;
			tank[0].licznik=tank[0].licznik+1;
		}
		else if (tank_no>0)
			bullets[no].type=1;
		
		board[x2][y2]=13;
		gotoxy(1,42);
		printf(CLEAR_LINE);
	}
		
		
}

/////////////////////////////////////////////THINK////////////////////////////////////////
void think(int no){
	int cc,cc2,b;
	
	
	if (level==1){
		if (tank[no].licznik%6==0){
			shoot(no);
			tank[no].licznik=0;
		}
		else{
			cc=tank[no].c;
			//if ((tank[no].licznik==7)||(tank[no].licznik==13))
			//	cc=68;
			if (move_tank(no,cc)==0){
				switch(tank[no].c){
					case 65:
						cc=67;
					break;
					case 67:
						cc=66;
					break;
					case 66:
						cc=68;
					break;
					case 68:
						cc=65;
					break;
				}
				move_tank(no,cc);
			}	
		}
		tank[no].licznik=tank[no].licznik+1;
		return;	
	}
					
	
	if (level==2){
		if (tank[no].licznik%5==0){
			shoot(no);
			tank[no].licznik=0;
		}
		else{
			if (movable(no,tank[no].c)){
				if (been(no,tank[no].c)==0)
					move_tank(no,tank[no].c);
				else{
					b=been(no,tank[no].c);
				
					switch(tank[no].c){
						case 65:
							cc=68;
						break;
						case 67:
							cc=65;
						break;
						case 66:
							cc=67;
							break;
						case 68:
							cc=66;
						break;
					}
				
					if ((movable(no,cc))&&(been(no,cc)<b))
						move_tank(no,cc);					
					else{
						switch(tank[no].c){
						case 65:
							cc=67;
						break;
						case 67:
							cc=66;
						break;
						case 66:
							cc=68;
						break;
						case 68:
							cc=65;
						break;
					}
					
						if ((movable(no,cc))&&(been(no,cc)<b))
							move_tank(no,cc);						
						else
							move_tank(no,tank[no].c);						
					}
				}
			}
			else{
				switch(tank[no].c){
					case 65:
						cc=68;
						cc2=67;
					break;
					case 67:
						cc=65;
						cc2=66;
					break;
					case 66:
						cc=67;
						cc2=68;
					break;
					case 68:
						cc=66;
						cc2=65;
					break;
				}
				
				if (movable(no,cc))
					move_tank(no,cc);				
				else
					move_tank(no,cc2);				
			}
		}	
		
		tank[no].licznik=tank[no].licznik+1;
		return;
	}
	
	
			//no. tutaj mozemy zrobic niezly rozpierdziel, ale proponuje jakies NAPROWADZANIE xd 	//taa...

	if (level==3){
	//	int cms;
	///	cms=can_me_smash(no);
		//if (cms!=0){
		//	if (cms!=tank[no].c){
		//		move_tank(no,cms);
		//	}
		//	else{
				//if (tank[no].licznik>=0)
			//		tank[no].licznik=-4;
			//	if (tank[no].licznik==-4)
//					shoot(no);	
//				tank[no].licznik=0;
//			}
		if (tank[no].licznik%5==0){
			int cms;
			cms=can_me_smash(no);
			if ((cms!=0)&&(cms!=tank[no].c)){
				move_tank(no,cms);
				return;
			}
			shoot(no);
			tank[no].licznik=0;
		}
		else{
			if (movable(no,tank[no].c)){
				if (been(no,tank[no].c)==0)
					move_tank(no,tank[no].c);
				else{
					b=been(no,tank[no].c);
				
					switch(tank[no].c){
						case 65:
							cc=68;
						break;
						case 67:
							cc=65;
						break;
						case 66:
							cc=67;
							break;
						case 68:
							cc=66;
						break;
					}
				
					if ((movable(no,cc))&&(been(no,cc)<b))
						move_tank(no,cc);					
					else{
						switch(tank[no].c){
						case 65:
							cc=67;
						break;
						case 67:
							cc=66;
						break;
						case 66:
							cc=68;
						break;
						case 68:
							cc=65;
						break;
					}
					
						if ((movable(no,cc))&&(been(no,cc)<b))
							move_tank(no,cc);						
						else
							move_tank(no,tank[no].c);						
					}
				}
			}
			else{
				switch(tank[no].c){
					case 65:
						cc=68;
						cc2=67;
					break;
					case 67:
						cc=65;
						cc2=66;
					break;
					case 66:
						cc=67;
						cc2=68;
					break;
					case 68:
						cc=66;
						cc2=65;
					break;
				}
				
				if (movable(no,cc))
					move_tank(no,cc);				
				else
					move_tank(no,cc2);				
			}
		}	
		
		tank[no].licznik=tank[no].licznik+1;
		return;
				
			
	}
		
	
	
}

/////////////////////////////////////////////REFRESH_ENEMIES//////////////////////////////
void refresh_enemies(){
	int i;
	for (i=1;i<=5;i++)
		if (tank[i].hp>0)
			think(i);
}
/////////////////////////////////////////////GAMEPLAY/////////////////////////

void gameplay(){	
	int i,c,moved;
	
	while ((enemies>0)&&(endgame==0)){
		refresh_bullets();
		refresh_enemies();
		refresh_booms();
		moved=0;
		
		gotoxy(1,42);
		printf(CLEAR_LINE);
		
		for (i=1;i<=300;i++){
			usleep(dif[level].time);
			if (i==150)
				refresh_bullets();
			if ((keypressed())&&(moved==0)){
				while (keypressed())
					c=getchar();
				if (((c==65)||(c==66)||(c==67)||(c==68))&&(tank[0].hp>0)){
					move_tank(0,c);
					moved=1;
				}
				if ((c==32)&&(tank[0].hp>0)&&(tank[0].licznik<5)){
					shoot(0);
					moved=1;
				}
			}
			else{
				while (keypressed())
					c=getchar();
				}
		}
	}
	system("clear");
	gotoxy(10,20);
	printf("Koniec.\n\n");
	if (endgame==1)
		end_of_game();
	else if (enemies==0){
		score=score+10*level;
		map_no=map_no+1;
		start_up();
		gameplay();
	}
}

////////////////////////////////////SET_LEVEL/////////////////////////////////////
void set_level(){
	int in,is,xn1,xn2,yn1,yn2,xs1,xs2,ys1,ys2;
	
	system("clear");
	
	printf(TC_RED);
	frame(18,20,57,22);
	printf(TC_WHT);
	gotoxy(29,21);
	printf("Can I play, Daddy?");
	
	printf(TC_BC_NRM);
	frame(18,23,57,25);
	printf(TC_WHT);
	gotoxy(31,24);
	printf("Hurt Me Plenty");
	
	printf(TC_BC_NRM);
	frame(18,26,57,28);
	printf(TC_WHT);
	gotoxy(34,27);
	printf("Serious");
	
	printf(TC_BC_NRM);
	
	in=is=1;
	xs1=xn1=18;
	ys1=yn1=20;
	xs2=xn2=57;
	ys2=yn2=22;
	
	char c=0;

	do{
		gotoxy(1,42);
		if (is!=in){
			switch (in){
				case 1:
					xn1=18;
					yn1=20;
					xn2=57;
					yn2=22;
				break;
				case 2:
					xn1=18;
					yn1=23;
					xn2=57;
					yn2=25;
				break;
				case 3:
					xn1=18;
					yn1=26;
					xn2=57;
					yn2=28;
				break;
			}
			printf(TC_BC_NRM);
			frame(xs1,ys1,xs2,ys2);
			printf(TC_RED);
			frame(xn1,yn1,xn2,yn2);
			is=in;
			xs1=xn1;
			xs2=xn2;
			ys1=yn1;
			ys2=yn2;
		}
		if (keypressed()){
			while (keypressed())
				c=getchar();
			if ((c==65)&&(in>1))
				in=in-1;
			if ((c==66)&&(in<3))
				in=in+1;
		}
	} while (c!=10);
	
	printf(TC_BC_NRM);
	
	if (in==1)
		level=1;
	if (in==2)
		level=2;
	if (in==3)
		level=3;

}
////////////////////////////////MENU///////////////////////////////////////////

void menu(){
	int in,is,xn1,xn2,yn1,yn2,xs1,xs2,ys1,ys2;
	system("clear");
	/*	frame(20,10,60,30);
	gotoxy(21,20);
	printf(" Jezdzisz strzalkami, strzelasz spacja");
	gotoxy(25,21);
	printf("Podaj poziom trudnosci (1-3): "); */
	printf(TC_RED);
	frame(13,13,58,23);
	
	printf(TC_RED);
	gotoxy(21,14);
	printf("           _______             ");
	gotoxy(21,15);
	printf("  /\\    /  |        \\        /");
	gotoxy(21,16);
	printf(" /  \\  /   |====     \\  /\\  / ");
	gotoxy(21,17);
	printf("/    \\/    |______    \\/  \\/ ");
	gotoxy(19,18);
	printf("                           _______");
	gotoxy(19,19);
	printf(" ---    /\\      /\\  /\\     |      ");
	gotoxy(19,20);
	printf("/  _   /__\\    /  \\/  \\    |==== ");
	gotoxy(19,21);
	printf("\\__|  /    \\  /        \\   |______");
	
	printf(TC_BC_NRM);
	frame(13,24,58,33);
	
	printf(TC_YEL);
	gotoxy(18,25);
	printf("           _____   ___          ");
	gotoxy(18,26);
	printf("   |___|     |    /  _   |___|   ");
	gotoxy(18,27);
	printf("   |   |   __|__  \\__|   |   |  ");
	gotoxy(16,29);
	printf(" ___   ____   _____  ___    ____    ___");
	gotoxy(16,30);
	printf("/___   |      |   |  |__|   |___   /___");
	gotoxy(16,31);
	printf("___/   |___   |___|  |  \\   |___   ___/");
	
	printf(TC_BC_NRM);
	frame(13,34,58,41);
	
	printf(TC_BLU);
	gotoxy(19,35);
	printf("_______          _______   __________");
	gotoxy(19,36);
	printf("|         \\  /      |          |");
	gotoxy(19,37);
	printf("|_____     \\/       |          |");
	gotoxy(19,38);
	printf("|          /\\       |          |");
	gotoxy(19,39);
	printf("|______   /  \\   ___|___       |");
	printf(TC_BC_NRM);
	
	in=is=1;
	xs1=xn1=13;
	ys1=yn1=13;
	xs2=xn2=58;
	ys2=yn2=23;
	
	char c=0;

	do{
		gotoxy(1,42);
		if (is!=in){
			switch (in){
				case 1:
					xn1=13;
					yn1=13;
					xn2=58;
					yn2=23;
				break;
				case 2:
					xn1=13;
					yn1=24;
					xn2=58;
					yn2=33;
				break;
				case 3:
					xn1=13;
					yn1=34;
					xn2=58;
					yn2=41;
				break;
			}
			printf(TC_BC_NRM);
			frame(xs1,ys1,xs2,ys2);
			printf(TC_RED);
			frame(xn1,yn1,xn2,yn2);
			is=in;
			xs1=xn1;
			xs2=xn2;
			ys1=yn1;
			ys2=yn2;
		}
		if (keypressed()){
			while (keypressed())
				c=getchar();
			if ((c==65)&&(in>1))
				in=in-1;
			if ((c==66)&&(in<3))
				in=in+1;
		}
	} while (c!=10);
	
	printf(TC_BC_NRM);

	if (in==1){
		set_level();
		can_i_start=1;
	}
	if (in==2)
		high_scores();
	if (in==3){
		system("clear");
		printf(BC_WHT);
		printf(TC_BLC);
		paint(3);
		gotoxy(20,40);
		printf(TC_RED);
		printf(BC_WHT);
		printf("Thank you for playing.\n");
		printf(TC_BC_NRM);
		exit(1);
	}
	
}

////////////////////////////////END_OF_GAME//////////////////////////////////////////////

void end_of_game(){
	system("clear");
	frame(20,10,60,30);
	gotoxy(21,20);
	
	int c;
	if (keypressed()){
			while (keypressed())
				c=getchar();
	}
		
	printf("     Przegrales, podaj swoje imie: ");
	gotoxy(35,21);
	printf("__________");
	gotoxy(35,21);
	
	char imie[11];
	int i;
	for (i=0;i<=10;i++)
		imie[i]=0;
	i=0;
	c=0;
	
	while ((c!=10)&&(i<10)){
		if (keypressed()){
			while (keypressed())
				c=getchar();
			if (c!=10){
				if (c!=127){
					printf("%c",c);
					imie[i]=c;
					i=i+1;
				}
				else if (i>0){
					i=i-1;
					gotoxy(35+i,21);
					printf("_");
					gotoxy(35+i,21);
					imie[i]=0;
				}
					
			}
		}
	}
	printf(TC_YEL);
	gotoxy(25,24);
	printf("%s, zdobyles %d punktow",imie,score);
	printf(TC_BC_NRM);
	
	FILE *t;
	if ((t=fopen("dane/score","rt"))==NULL){
		system("clear");
    	printf ("Nie moge otworzyc pliku \"score\"! Sprawdz, czy nie brakuje go w katalogu gry!\n");
    	exit(1);
    }
	
	char names[12][14];
	int scores[12];
	scores[0]=100000;
	for (i=1;i<=11	;i++)
		scores[i]=-2;
	
	int k=0;
	
	while (feof(t)==0){
		k++;
		fgets(names[k],13,t);
		fscanf(t,"%d",&scores[k]);
		fgets(names[0],2,t);
		//printf("%s - %d",names[k],scores[k]);
	}
		
	fclose(t);
	
	int z=0;
	
	for (i=1;i<=10;i++)
		if ((score<scores[i-1])&&(score>=scores[i])){
			z=i;
		//	printf("%d %d",scores[i-1],scores[i]);
			break;
		}
	
	for (i=11;i<=12;i++)
		scores[i]=-1;
	
	//c=getchar();
	if ((t=fopen("dane/score","wt"))==NULL){
		system("clear");
    	printf ("Nie moge otworzyc pliku \"score\"! Sprawdz, czy nie brakuje go w katalogu gry!\n");
    	exit(1);
    }		
	
	for (i=1;i<=10;i++){
		if (i==z)
			fprintf(t,"%s\n%d\n",imie,score);
		if (scores[i]>-1)
			fprintf(t,"%s%d\n",names[i],scores[i]);
	}
	
	fclose(t);
	
	gotoxy(25,26);
	printf("Wcisnij enter, aby zakonczyc gre.");
	char p=0;
	while (p!=10)
		if (keypressed())
			while (keypressed())
				p=getchar();
	
	if (z==1){
		system("clear");
		printf(TC_RED);
		gotoxy(22,40);
		printf("Pierwsze miejsce! Gratulacje!");
		printf(TC_WHT);
		paint(4);
	}
	
	if (z==2){
		system("clear");
		printf(TC_WHT);
		paint(5);
		printf(TC_RED);
		gotoxy(10,41);
		printf("Drugie miejsce! Tak blisko!");
	}
	
	if (z==3){
		system("clear");
		printf(TC_RED);
		gotoxy(20,40);
		printf("Trzecie miejsce! Moglo byc lepiej!");
		printf(TC_WHT);
		paint(1);
	}
	
	if (score==0){
		system("clear");
		printf(TC_RED);
		gotoxy(24,40);
		printf("Zero?... Naprawde?...");
		printf(TC_WHT);
		paint(2);
	}
}

////////////////////////////////////PAINT///////////////////////////////////////
void paint(int noumber){
	int x1,y1,k;
	
	FILE *t;
	if ((t=fopen(painting[noumber],"rt"))==NULL){
		system("clear");
    	printf ("Nie moge otworzyc pliku \"%s\"! Sprawdz, czy nie brakuje go w katalogu gry!\n",painting[noumber]);
    	exit(1);
    }
	char c[5];
	fscanf(t,"%d%d",&x1,&y1);
	fgets(c,2,t);
	
	k=0;
	char line[75];
	
	system("clear");
	
	while (feof(t)==0){
		gotoxy(x1,y1+k);
		k++;
		fgets(line,72,t);
		printf("%s",line);
	}
		
	fclose(t);
	gotoxy(1,41);
}

////////////////////////////////////HIGH_SCORES///////////////////////////////////////
void high_scores(){
	int i;
	
	FILE *t;
	if ((t=fopen("dane/score","rt"))==NULL){
		system("clear");
    	printf ("Nie moge otworzyc pliku \"dane/scores\"! Sprawdz, czy nie brakuje go w katalogu gry!\n");
    	exit(1);
    }

	char names[5][14];
	int scores[5];
	for (i=1;i<=3;i++)
		scores[i]=-2;
	
	int k=0;

	while ((feof(t)==0)&&(k<3)){
		k++;
		fgets(names[k],13,t);
		fscanf(t,"%d",&scores[k]);
		fgets(names[0],5,t);
	}
	fclose(t);
	

	
	paint(6);

	printf(TC_RED);
	if (scores[1]>-1){
		gotoxy(32,17);
		printf("%s",names[1]);
		gotoxy(36,28);
		printf("%d",scores[1]);
	}
	
	if (scores[2]>-1){
		gotoxy(47,19);
		printf("%s",names[2]);
		gotoxy(51,28);
		printf("%d",scores[2]);
	}

	if (scores[3]>-1){
		gotoxy(19,21);	
		printf("%s",names[3]);
		gotoxy(23,28);
		printf("%d",scores[3]);
	}
	
	gotoxy(1,42);
	char p;
	p=getchar();
	menu();
}


////////////////////////////////////STARTUP//////////////////////////////////////
void start_up(){
	int i,j,k;
	enemies=dif[level].enemies;
	for (i=0;i<=43;i++)
		for (j=0;j<=43;j++)
			board[i][j]=150;
	
	for (i=0;i<=41;i++)
		for (j=0;j<=41;j++)
			for(k=1;k<=5;k++)
				tank[k].board[i][j]=0;
	
	for (i=1;i<=5;i++){
		tank[i].hp=0;
		tank[i].licznik=i-1;
		strcpy(tank[i].clr,TC_GRN);
	}
	for (i=1;i<=6;i++){
		booms[i].is=0;
	}
	for (i=1;i<=29;i++){
		bullets[i].is=0;
	}
	
	char p;	
	while (keypressed())
		p=getchar();
	system("clear");
	gotoxy(31,19);
	printf(TC_RED);
	printf("Mapa: %s",map[map_no]);
	printf(TC_BC_NRM);
	gotoxy(25,21);
	printf("Wcisnij enter, aby zaczac.");
	
	p=0;
	while (p!=10)
		if (keypressed())
			while (keypressed())
				p=getchar();
	
	draw_map();
	frame(1,1,41,41);
	
	gotoxy(45,5);
	printf("Ilosc wrogow: %d",enemies);
	
	gotoxy(45,7);
	printf("Ilosc zyc: %d",zycie);
	
	gotoxy(45,9);
	printf("Ilosc punktow: %d",score);
	if (tank[0].hp>0){
		tank[0].x=spawns[1].x;
		tank[0].y=spawns[1].y;
		tank[0].c=65;
		tank[0].licznik=0;
		printf(TC_BC_NRM);
		draw_tank(0,tank[0].c);
	}

	for (i=1;i<=5;i++) { 
		spawn_enemy();
	}
}

///////////////////////////////////////LOADING////////////////////////////////////
void loading(){
	//int i;
	map_no=1;
	strcpy(tank[0].clr,TC_YEL);
	tank[0].hp=3;
	
	
	strcpy(map[1],"dane/m1");
	strcpy(map[2],"dane/m2");
	strcpy(map[3],"dane/m3");
	
	strcpy(painting[1],"dane/notbad.txt");
	strcpy(painting[2],"dane/fail.txt");
	strcpy(painting[3],"dane/kiss.txt");
	strcpy(painting[4],"dane/epicwin.txt");
	strcpy(painting[5],"dane/soclose.txt");
	strcpy(painting[6],"dane/scoretable");
	
	
	dif[1].hp=1;
	dif[2].hp=2;
	dif[3].hp=3;
	dif[1].time=450;
	dif[2].time=400;
	dif[3].time=350;
	dif[1].enemies=20;
	dif[2].enemies=15;
	dif[3].enemies=10;
	
}
///////////////////////////////////////MAIN///////////////////////////////////////////

int main(){
	loading();
	//do{
	//	menu();
	//} while(can_i_start==0);
	menu();
	start_up();
	gameplay();
	gotoxy(1,42);
	return 0;
}