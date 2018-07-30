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


void IntroScreen();  //�ʱ�ȭ��   
void PauseScreen();	// ���� ȭ��
    
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

enum { EMPTY, BRICK, WALL }; //FFFF������ ����, �����, �׵θ� ���, ��� ���
char *arTile[]={"  ","��","��"}; // ��� �ٲ���  
int board[BW+2][BH+2]; //FFFFF 2���� �迭
int nx,ny;
int brick,rot;

void main()
{
	int nFrame, nStay; 
	int x,y;
	char restart;
	
	IntroScreen(); //�Լ� ����  
	    
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
	gotoxy(18, 3);puts("���� ��"); //�ѱ۷� ���� 
	gotoxy(10, 5); puts("�ٽ� �����Ϸ��� RŰ�� ��������."); //����� Ű 
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
	gotoxy(35, 4);   puts("��������������������������������������������������������������������");
	gotoxy(35, 5);   puts("��                                ��");
	gotoxy(35, 6);   puts("�� �̵�: �� ��, ȸ��: ��, ����: �馭");
	gotoxy(35, 7);   puts("�� �ѹ��� ����: A Ű              ��");
	gotoxy(35, 8);	 puts("�� �Ͻ� ����: P Ű                ��");
	gotoxy(35, 9);   puts("�� �ٽ� ����: R Ű                ��");
	gotoxy(35, 10);  puts("��������������������������������������������������������������������");
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
			switch (ch) {    // �ѹ��� ����  Ű  
			case 'a':
			case 'A':
				while(MoveDown()==FALSE) {;}
				return TRUE;
			case 'p':            // �Ͻ� ���� Ű �߰� 
			case 'P':
				PauseScreen();
				break;
			case 'r':            // ���� ��ư Ű �߰� 
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

// �߰��ҽ� 
void IntroScreen()
{
	
	int x = 10;

	 
	gotoxy(x, 3); puts("              �� ��");
	gotoxy(x, 4); puts("��������      �� ��");
	gotoxy(x, 5); puts("��            �� ��");
	gotoxy(x, 6); puts("��������  ������ ��");
	gotoxy(x, 7); puts("��            �� ��");
	gotoxy(x, 8); puts("��������      �� ��");
	gotoxy(x, 9); puts("              �� ��");

	
	gotoxy(x + 25, 3); puts("����������");
	gotoxy(x + 25, 4); puts("��        ");
	gotoxy(x + 25, 5); puts("����������");
	gotoxy(x + 25, 6); puts("��        ");
	gotoxy(x + 25, 7); puts("����������");
	gotoxy(x + 24, 9); puts("������������");

	
	gotoxy(x + 40, 3); puts("            ��");
	gotoxy(x + 40, 4); puts("����������  ��");
	gotoxy(x + 40, 5); puts("        ��  ��");
	gotoxy(x + 40, 6); puts("����������  ��");
	gotoxy(x + 40, 7); puts("��          ��");
	gotoxy(x + 40, 8); puts("����������  ��");
	gotoxy(x + 40, 9); puts("            ��");


	gotoxy(x + 60, 4); puts("    ��    ");
	gotoxy(x + 60, 5); puts("   �� ��   ");
	gotoxy(x + 60, 6); puts("  ��   ��  ");
	gotoxy(x + 60, 7); puts(" ��     ��  ");
	gotoxy(x + 59, 9); puts("��������������");


	gotoxy(30, 13); puts("> ������ �����Ϸ��� �ƹ�Ű�� �����ּ��� <");
	gotoxy(38, 15); puts("201812001 ������");
	getch(); // Ű �Է��� 
}

void PauseScreen()
{
	clrscr();
	gotoxy(19, 5); puts("�Ͻ� ����");
	gotoxy(5, 8); puts("> �ٽ� �����Ϸ��� �ƹ�Ű�� �����ּ��� <");
	gotoxy(7, 8); puts("> ���� ���¿��� ������ �Ұ��մϴ�.    <");
	getch();
	clrscr();
	DrawScreen();
	PrintBrick(TRUE);
	
}


