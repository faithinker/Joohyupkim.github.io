#include "Tetris.h"
#include <conio.h>

#define LEFT 75
#define RIGHT 77
#define UP 72
#define DOWN 80
#define ESC 27
#define BX 5
#define BY 1
#define BW 10
#define BH 20


void DrawScreen();
void DrawBoard();
BOOL ProcessKey();
void PrintBrick(BOOL Show);
int GetAround(int x,int y,int b,int r);
BOOL MoveDown();
void TestFull();


void IntroScreen();  //초기화면   
void PauseScreen();	// 정지 화면
    
struct Point {
	int x,y;
};
struct Point Shape[][4][4]={
	{ {0,0,1,0,2,0,-1,0}, {0,0,0,1,0,-1,0,-2}, {0,0,1,0,2,0,-1,0}, {0,0,0,1,0,-1,0,-2} },
	{ {0,0,1,0,0,1,1,1}, {0,0,1,0,0,1,1,1}, {0,0,1,0,0,1,1,1}, {0,0,1,0,0,1,1,1} },
	{ {0,0,-1,0,0,-1,1,-1}, {0,0,0,1,-1,0,-1,-1}, {0,0,-1,0,0,-1,1,-1}, {0,0,0,1,-1,0,-1,-1} },
	{ {0,0,-1,-1,0,-1,1,0}, {0,0,-1,0,-1,1,0,-1}, {0,0,-1,-1,0,-1,1,0}, {0,0,-1,0,-1,1,0,-1} },
	{ {0,0,-1,0,1,0,-1,-1}, {0,0,0,-1,0,1,-1,1}, {0,0,-1,0,1,0,1,1}, {0,0,0,-1,0,1,1,-1} },
	{ {0,0,1,0,-1,0,1,-1}, {0,0,0,1,0,-1,-1,-1}, {0,0,1,0,-1,0,-1,1}, {0,0,0,-1,0,1,1,1} },
	{ {0,0,-1,0,1,0,0,1}, {0,0,0,-1,0,1,1,0}, {0,0,-1,0,1,0,0,-1}, {0,0,-1,0,0,-1,0,1} },
};

enum { EMPTY, BRICK, WALL }; //FFFF열거형 선언, 빈공간, 테두리 모양, 블록 모양
char *arTile[]={"  ","★","♥"}; // 모양 바꿔줌  
int board[BW+2][BH+2]; //FFFFF 2차원 배열
int nx,ny;
int brick,rot;

void main()
{
	int nFrame, nStay; 
	int x,y;
	char restart;
	
	IntroScreen(); //함수 선언  
	    
	setcursortype(NOCURSOR);
	randomize();
	clrscr();
	for (x=0;x<BW+2;x++) {
		for (y=0;y<BH+2;y++) {
			board[x][y] = (y==0 || y==BH+1 || x==0 || x==BW+1) ? WALL:EMPTY;
		}
	}
	DrawScreen();
	nFrame=5;

	for (;1;) {
		brick=random(sizeof(Shape)/sizeof(Shape[0]));
		nx=BW/2;
		ny=3;
		rot=0;
		PrintBrick(TRUE);

		if (GetAround(nx,ny,brick,rot) != EMPTY) break;
		nStay=nFrame;
		for (;2;) {
			if (--nStay == 0) {
				nStay=nFrame;
				if (MoveDown()) break;
			}
			if (ProcessKey()) break;
			delay(1000/20);
		}
	}
	clrscr();
	gotoxy(18, 3);puts("게임 끝"); //한글로 변경 
	gotoxy(10, 5); puts("다시 시작하려면 R키를 누르세요."); //재시작 키 
	setcursortype(NORMALCURSOR);
}

void DrawScreen()
{
	int x,y;

	for (x=0;x<BW+2;x++) {
		for (y=0;y<BH+2;y++) {
			gotoxy(BX+x*2,BY+y);
			puts(arTile[board[x][y]]);
		} 
	}

//	gotoxy(50,3);puts("Tetris Ver 1.0");
	gotoxy(35, 4);   puts("┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓");
	gotoxy(35, 5);   puts("┃                                ┃");
	gotoxy(35, 6);   puts("┃ 이동: ← →, 회전: ↑, 내림: ↓┃");
	gotoxy(35, 7);   puts("┃ 한번에 내림: A 키              ┃");
	gotoxy(35, 8);	 puts("┃ 일시 중지: P 키                ┃");
	gotoxy(35, 9);   puts("┃ 다시 시작: R 키                ┃");
	gotoxy(35, 10);  puts("┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛");
}

void DrawBoard()
{
	int x,y;

	for (x=1;x<BW+1;x++) {
		for (y=1;y<BH+1;y++) {
			gotoxy(BX+x*2,BY+y);
			puts(arTile[board[x][y]]);
		}
	}
}

BOOL ProcessKey()
{
	int ch,trot;

	if (kbhit()) {
		ch=getch();
		if (ch == 0xE0 || ch == 0) {
			ch=getch();
			switch (ch) {
			case LEFT:
				if (GetAround(nx-1,ny,brick,rot) == EMPTY) {
					PrintBrick(FALSE);
					nx--;
					PrintBrick(TRUE);
				}
				break;
			case RIGHT:
				if (GetAround(nx+1,ny,brick,rot) == EMPTY) {
					PrintBrick(FALSE);
					nx++;
					PrintBrick(TRUE);
				}
				break;
			case UP:
				trot=(rot == 3 ? 0:rot+1);
				if (GetAround(nx,ny,brick,trot) == EMPTY) {
					PrintBrick(FALSE);
					rot=trot;
					PrintBrick(TRUE);
				}
				break;
			case DOWN:
				if (MoveDown()) {
					return TRUE;
				}
				break;
			}
		} else {
			switch (ch) {    // 한번에 내림  키  
			case 'a':
			case 'A':
				while(MoveDown()==FALSE) {;}
				return TRUE;
			case 'p':            // 일시 정지 키 추가 
			case 'P':
				PauseScreen();
				break;
			case 'r':            // 리셋 버튼 키 추가 
			case 'R':
				clrscr();
				main();
				break;
			}
			
		}
	}
	return FALSE;
}

void PrintBrick(BOOL Show)
{
	int i;

	for (i=0;i<4;i++) {
		gotoxy(BX+(Shape[brick][rot][i].x+nx)*2,BY+Shape[brick][rot][i].y+ny);
		puts(arTile[Show ? BRICK:EMPTY]);
		SKY;
	}
}

int GetAround(int x,int y,int b,int r)
{
	int i,k=EMPTY;

	for (i=0;i<4;i++) {
		k=max(k,board[x+Shape[b][r][i].x][y+Shape[b][r][i].y]);
	}
	return k;
}

BOOL MoveDown()
{
	if (GetAround(nx,ny+1,brick,rot) != EMPTY) {
		TestFull();
		return TRUE;
	}
	PrintBrick(FALSE);
	ny++;
	PrintBrick(TRUE);
	return FALSE;
}

void TestFull()
{
	int i,x,y,ty;

	for (i=0;i<4;i++) {
		board[nx+Shape[brick][rot][i].x][ny+Shape[brick][rot][i].y]=BRICK;
	}

	for (y=1;y<BH+1;y++) {
		for (x=1;x<BW+1;x++) {
			if (board[x][y] != BRICK) break;
		}
		if (x == BW+1) {
			for (ty=y;ty>1;ty--) {
				for (x=1;x<BW+1;x++) {
					board[x][ty]=board[x][ty-1];
				}
			}
			DrawBoard();
			delay(200);
		}
	}
}

// 추가소스 
void IntroScreen()
{
	
	int x = 10;

	 
	gotoxy(x, 3); puts("              ㅌ ㅌ");
	gotoxy(x, 4); puts("ㅌㅌㅌㅌ      ㅌ ㅌ");
	gotoxy(x, 5); puts("ㅌ            ㅌ ㅌ");
	gotoxy(x, 6); puts("ㅌㅌㅌㅌ  ㅌㅌㅌ ㅌ");
	gotoxy(x, 7); puts("ㅌ            ㅌ ㅌ");
	gotoxy(x, 8); puts("ㅌㅌㅌㅌ      ㅌ ㅌ");
	gotoxy(x, 9); puts("              ㅌ ㅌ");

	
	gotoxy(x + 25, 3); puts("ㅌㅌㅌㅌㅌ");
	gotoxy(x + 25, 4); puts("ㅌ        ");
	gotoxy(x + 25, 5); puts("ㅌㅌㅌㅌㅌ");
	gotoxy(x + 25, 6); puts("ㅌ        ");
	gotoxy(x + 25, 7); puts("ㅌㅌㅌㅌㅌ");
	gotoxy(x + 24, 9); puts("ㅌㅌㅌㅌㅌㅌ");

	
	gotoxy(x + 40, 3); puts("            ㄹ");
	gotoxy(x + 40, 4); puts("ㄹㄹㄹㄹㄹ  ㄹ");
	gotoxy(x + 40, 5); puts("        ㄹ  ㄹ");
	gotoxy(x + 40, 6); puts("ㄹㄹㄹㄹㄹ  ㄹ");
	gotoxy(x + 40, 7); puts("ㄹ          ㄹ");
	gotoxy(x + 40, 8); puts("ㄹㄹㄹㄹㄹ  ㄹ");
	gotoxy(x + 40, 9); puts("            ㄹ");


	gotoxy(x + 60, 4); puts("    ㅅ    ");
	gotoxy(x + 60, 5); puts("   ㅅ ㅅ   ");
	gotoxy(x + 60, 6); puts("  ㅅ   ㅅ  ");
	gotoxy(x + 60, 7); puts(" ㅅ     ㅅ  ");
	gotoxy(x + 59, 9); puts("ㅅㅅㅅㅅㅅㅅㅅ");


	gotoxy(30, 13); puts("> 게임을 시작하려면 아무키나 눌러주세요 <");
	gotoxy(38, 15); puts("201812001 김주협");
	getch(); // 키 입력협 
}

void PauseScreen()
{
	clrscr();
	gotoxy(19, 5); puts("일시 중지");
	gotoxy(5, 8); puts("> 다시 시작하려면 아무키나 눌러주세요 <");
	gotoxy(7, 8); puts("> 현재 상태에서 리셋은 불가합니다.    <");
	getch();
	clrscr();
	DrawScreen();
	PrintBrick(TRUE);
	
}


