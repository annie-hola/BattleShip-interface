//Source code
#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>
#include<string.h>
#include<time.h>
#include<windows.h>
#define KEY_LEFT 75							/* Chuyển sang trái, mũi tên sang trái*/
#define KEY_UP 72							/* Chuyển lển trên, mũi tên lên */ 
#define KEY_RIGHT 77						/* Chuyển sang phải, mũi tên phải */
#define KEY_DOWN 80							/* Chuyển xuống dưới, mũi tên xuống */
#define KEY_ENTER 13						/*Enter, xác nhận*/
#define BOARDSIZE 8							/*kích thước của tàu*/
#define BATTLESHIPSIZE 5					/*tàu kích thước lớn nhất - 5 ô*/
#define CRUISERSIZE 4						/*tàu kích thước nhì - 4 ô*/
#define FRIGATESIZE 3						/*tàu nhỏ vừa - 3 ô*/
#define MINESWEEPERSIZE 2					/*tàu nhỏ nhất - 2 ô*/
#define SHIPNUMBER 4						/*tổng số tàu được đặt*/
#define WATER 176							/*biểu thị nước*/
#define SHIP 219							/*biểu thị tàu (khi chọn)*/
#define BOMB 1								/*biểu thị phần tàu bị bắn chìm (mặt cười)*/
#define NOTBOMB 248							/*biểu thị phàn bị bắn nhưng ko có tàu (chấm tròn)*/
#define PLAYER 219							/*character khi người chơi chọn vị trí cho tàu*/
#define BLANK -52							/*character trống*/
#define PLAYERNO 2							/* 2 người chơir*/

struct board {
	char ship[SHIPNUMBER][BOARDSIZE][BOARDSIZE];
	int score;
	int mainboard[BOARDSIZE][BOARDSIZE];	/**8x8*/
	char openbomb[BOARDSIZE][BOARDSIZE];	/*the 8-by-8 board contains the position of the block that a bomb has been placed*/
	char openwater[BOARDSIZE][BOARDSIZE];	/*the 8-by-8 board contains the position of the block that has been opened*/
};
typedef struct board Board;

struct info {				/*chứa thông tin người thăng <optional>*/
	char name[100];
	int score;
	char date[80];

};
typedef struct info Info;

//Giới thiệu vào game
void intro() {
	int user;
	while (1) {
		printf("=========================================================");
		printf("\n=		   Welcome to Battleship  		=");
		printf("\n=========================================================");
		printf("\n");
		printf("                    ()\n");
		printf("                    ||q',,'\n");
		printf("                    ||d,~\n");
		printf("         (,---------------------,)\n");
		printf("          ',       q888p       ,'\n");
		printf("            \\       986       /\n");
		printf("             \\  8p, d8b ,q8  /\n");
		printf("              ) 888a888a888 (\n");
		printf("             /  8b` q8p `d8  \\              O\n");
		printf("            /       689       \\             |','\n");
		printf("           /       d888b       \\      (,---------,)\n");
		printf("         ,'_____________________',     \\   ,8,   /\n");
		printf("         (`__________|__________`)      ) a888a (  \n");
		printf("         [___________|___________]     /___`8`___\\   }*{\n");
		printf("           }:::|:::::}::|::::::{      (,=========,)  -=-\n");
		printf("            '|::::}::|:::::{:|'  .,.    \\:::|:::/    ~`~\n");
		printf("--=~(@)~=-- ' |}:::::|::{:::|'           ~\"., .\"~`~\n");
		printf("               '|:}::|::::|'~`~\"., .\" \n");
		printf("         ~`~\"., .\"~`~\"., \"~`~\"., .\"~\n\n");
		printf("\n Rule: Players place and then take turns on their own water and then start shooting into the opponent's waters. If hit, player will continue to follow. Whoever shoots all the ship in the opponent's water will win \n");
		printf("		Press ENTER to start the game\n");
		user = _getch();
		if (user == KEY_ENTER)
			break;
		system("cls");
	}
	system("cls");
}
// hiện bảng 
void displayBoard(Board* ptrBoard) {
	int option = WATER;
	printf("\nYour current board: \n\n");
	int i, j;
	for (i = 0; i < BOARDSIZE; i++) {
		printf("\t    ");
		for (j = 0; j < BOARDSIZE; j++) {
			printf(" %c ", ptrBoard->mainboard[i][j]);	
		}
		printf("\n\n");
	}
}
//gameCore function đăt vị trí thuyền 
void gameCore(Board* ptrBoard) {
	int i, j, k;
	int option = WATER;
	for (i = 0; i < BOARDSIZE; i++) {
		for (j = 0; j < BOARDSIZE; j++) {
			for (k = 0; k < SHIPNUMBER; k++) {
				if (ptrBoard->ship[k][i][j] != BLANK) {
					ptrBoard->mainboard[i][j] = ptrBoard->ship[k][i][j];	
				}
				if (ptrBoard->ship[0][i][j] == BLANK && ptrBoard->ship[1][i][j] == BLANK && ptrBoard->ship[2][i][j] == BLANK && ptrBoard->ship[3][i][j] == BLANK)
					ptrBoard->mainboard[i][j] = option;						
			}
		}
	}
}
//Kiểm tra xem thuyền có bị để trùng ko
int isOverlapped(Board* ptrBoard, int m, int n, int shipnum, int choice) {
	int k, l, i;
	for (i = 0; i < shipnum; i++) {
		for (k = 0; k < SHIPNUMBER; k++) {
			if (ptrBoard->ship[k][m][n] != BLANK)			
				return 1;
		}
		if (choice == 1)		
			n = n + 1;
		else if (choice == 2)	
			m = m + 1;
	}
	return 0;
}
// Function dùng để di chuyển thuyền qua lại
void fixPos(Board* ptrBoard, int m, int n, int shipnum, int choice, int enter) {
	// if the enter button is pressed, the value of enter is 1, otherwise it is 0;
	int option = SHIP, k, l, list[] = { BATTLESHIPSIZE, CRUISERSIZE, FRIGATESIZE, MINESWEEPERSIZE };

	for (l = 0; l < shipnum; l++) {
		ptrBoard->mainboard[m][n] = option;
		for (k = 0; k < SHIPNUMBER; k++) {
			if (enter == 1 && shipnum == list[k])							
				ptrBoard->ship[k][m][n] = ptrBoard->mainboard[m][n];		
		}
		if (choice == 1)	
			n++;				
		else if (choice == 2)
			m++;

	}
}
//selectionWindows: giao diện để người chơi dặt thuyền 
Board selectionWindow(Board board, Board* ptrBoard) {
	int i, ships[SHIPNUMBER] = { BATTLESHIPSIZE, CRUISERSIZE, FRIGATESIZE, MINESWEEPERSIZE }, j, user = 1, m = 0, n = 0;
	int count = 0, user2, enter = 0, player = 1, edge;
	char choice; // hướng của thuyền
	ptrBoard = &board;
	ptrBoard->score = 0;
	system("cls");
	for (i = 0; i < SHIPNUMBER; i++) {
		m = n = 0;
		enter = 0;
		while (1) {									
			printf("What direction will the ship be facing?\n");
			printf("\n\t");
			for (j = 0; j < ships[i]; j++) {			
				printf("%c ", SHIP);
			}
			printf("\t %c\n", SHIP);
			for (j = 0; j < ships[i] - 1; j++) {		
				if (ships[i] < 4)
					printf("\n\t\t %c\n", SHIP);
				else
					printf("\n\t\t\t %c\n", SHIP);
			}
			//Option (1) và (2) được chọn 
			if (ships[i] < 4)						
				printf("\n\t(1)\t(2)");				
			else
				printf("\n\t(1)\t\t(2)\n");			 

			if (user == 1) {
				printf("\n\t ^");					// hiện vị trí thuyên của người chơi đầu tiên
			}
			else if (user == 2) {					//hiện vị trí thuyền của người chơi 2
				if (ships[i] < 4)					//sắp xếp format cho thuyền nhiều hơn 4 ô
					printf("\n\t\t ^");				
				else                                //sắp xếp format cho thuyền nhiều hơn 3 ô
					printf("\n\t\t\t ^");			
			}

			choice = _getch();						
			if (choice == KEY_LEFT) {				
				user = 1;
			}
			else if (choice == KEY_RIGHT) {			
				user = 2;
			}
			else if (choice == KEY_ENTER) {			
				if (user == 1 || user == 2) {		// enter để kết thúc dặt thuyền và bđ chơi 
					break;
				}
			}
			system("cls");
		}
		gameCore(ptrBoard);							//gọi gameCore func để tạo mainboard
		fixPos(ptrBoard, m, n, ships[count], user, enter);
		system("cls");
		while (1) {									
			printf("Row = %d Column = %d\n", m, n);
			displayBoard(ptrBoard);
			user2 = _getch();						
			if (user2 == KEY_RIGHT) {				
				if (user == 1)
					edge = BOARDSIZE - ships[count];	
				else if (user == 2)
					edge = BOARDSIZE - 1;			
				if (n < edge) {						
					n++;
					gameCore(ptrBoard);
					fixPos(ptrBoard, m, n, ships[count], user, enter);
				}
				else {								
					printf("\nOut of the board!!!\n");
					Sleep(300);
				}
			}
			else if (user2 == KEY_LEFT) {			
				if (n > 0) {							
					n--;
					gameCore(ptrBoard);
					fixPos(ptrBoard, m, n, ships[count], user, enter);
				}
				else {								
					printf("\nOut of the board!!!\n");
					Sleep(300);
				}
			}
			else if (user2 == KEY_UP) {				
				if (m > 0) {							
					m--;
					gameCore(ptrBoard);
					fixPos(ptrBoard, m, n, ships[count], user, enter);
				}
				else {								
					printf("\nOut of the board!!!\n");
					Sleep(300);
				}
			}
			else if (user2 == KEY_DOWN) {			
				if (user == 2)						
					edge = BOARDSIZE - ships[count];
				else if (user == 1)
					edge = BOARDSIZE - 1;
				if (m < edge) {					
					m++;
					gameCore(ptrBoard);
					fixPos(ptrBoard, m, n, ships[count], user, enter);
				}
				else {								
					printf("\nOut of the board!!!\n");
					Sleep(300);
				}
			}
			else if (user2 == KEY_ENTER) {			
				if (isOverlapped(ptrBoard, m, n, ships[count], user) == 0) {	
					fixPos(ptrBoard, m, n, ships[count], user, 1);
					gameCore(ptrBoard);
					system("cls");
					break;
				}
				else if (isOverlapped(ptrBoard, m, n, ships[count], user) == 1) { 
					printf("\nThey're overlapped!!!\n");
					Sleep(750);
				}
			}
			system("cls");
		}
		count++;

	}
	displayBoard(ptrBoard);
	Sleep(1000);
	system("cls");
	return board;	
}
//displayGame dùng để hiển thị bom bứn trúng và số thuyền còn lại 
void displayGame(Board* ptrBoard) {
	char shipname[SHIPNUMBER][15] = { "Battleship", "Cruiser", "Frigate", "Minesweeper" };
	int count = 0;
	printf("\nPick the spot\n\n");
	int i, j, k;
	for (i = 0; i < BOARDSIZE; i++) {
		printf("\t    ");
		for (j = 0; j < BOARDSIZE; j++) {
			printf(" %c ", ptrBoard->openbomb[i][j]); 
		}
		printf("\n\n");
	}
	for (k = 0; k < SHIPNUMBER; k++) {					
		for (i = 0; i < BOARDSIZE; i++) {
			for (j = 0; j < BOARDSIZE; j++) {
				if (ptrBoard->ship[k][i][j] != BLANK)
					count++;
			}
		}
		printf("%s : %d\n", shipname[k], count);
		count = 0;
	}
}
//refreshBoard function: giống với gameCore Func
void refreshBoard(Board* ptrBoard) {
	int i, j;
	for (i = 0; i < BOARDSIZE; i++) {
		for (j = 0; j < BOARDSIZE; j++) {
			if (ptrBoard->openwater[i][j] == BLANK) {	
				ptrBoard->openbomb[i][j] = WATER;
			}
			if (ptrBoard->openwater[i][j] == BOMB) {		
				ptrBoard->openbomb[i][j] = BOMB;
			}
			if (ptrBoard->openwater[i][j] != BOMB && ptrBoard->openwater[i][j] != BLANK) {
				ptrBoard->openbomb[i][j] = NOTBOMB;
			}
		}
	}
}
//kiểm tra có người chơi bắn hết tàu và thắng chưa
int checkWinner(Board* ptrBoard, int player) {
	int i, j, check = 1;
	for (i = 0; i < BOARDSIZE; i++) {
		for (j = 0; j < BOARDSIZE; j++) {
			if (ptrBoard->ship[1][i][j] == BLANK && ptrBoard->ship[2][i][j] == BLANK && ptrBoard->ship[3][i][j] == BLANK && ptrBoard->ship[0][i][j] == BLANK)
				check = 1;
			else {
				return 0;
			}
		}
	}
	if (check == 1) {	
		printf("Number of shots you've missed: %d", ptrBoard->score);
		return 1;
	}
}
//placeBomb : thông báo đã trúng hay chưa đồng thời thay đổi hiển thị
int placeBomb(Board* ptrBoard, int m, int n, int enter) {
	int k;
	if (enter == 1 && ptrBoard->openwater[m][n] != BLANK) {	//kiểm tra vị trí đã được chọn chưa
		printf("You've already selected the spot!");
		Sleep(500);
		return 0;
	}
	else {
		ptrBoard->openbomb[m][n] = PLAYER;						
		if (enter == 1 && ptrBoard->mainboard[m][n] != WATER) {	// thông báo đã trúng tàu 
			printf("Bomb!");
			ptrBoard->openbomb[m][n] = BOMB;
			ptrBoard->openwater[m][n] = BOMB;
			for (k = 0; k < SHIPNUMBER; k++) {
				if (ptrBoard->ship[k][m][n] != BLANK)
					ptrBoard->ship[k][m][n] = BLANK;
			}
			Sleep(500);
			return 2;
		}
		else if (enter == 1 && ptrBoard->mainboard[m][n] == WATER) {	// vị trí là nước, thông báo đã trượt
			printf("Missed!");
			ptrBoard->score += 1;
			ptrBoard->openbomb[m][n] = NOTBOMB;			
			ptrBoard->openwater[m][n] = NOTBOMB;
			Sleep(500);
		}
	}
	return 1;
}
//Main function of te game for hitting the ships and finding the winner
Board mainGame(Board board, Board* ptrBoard, int player) {
	ptrBoard = &board;
	int m = 0, n = 0, user, enter, end;
	refreshBoard(ptrBoard);
	placeBomb(ptrBoard, m, n, 0);
	while (1) {
		enter = 0;
		printf("Player %d's turn ", player + 1);		//hiển thị đến turn của người chơi nào
		printf("\nRow = %d Column = %d", m, n);
		displayGame(ptrBoard);						
		user = _getch();							// nhận INPUT từ người chơi
		if (user == KEY_RIGHT) {						
			if (n < BOARDSIZE - 1) {					
				n++;
				refreshBoard(ptrBoard);
				placeBomb(ptrBoard, m, n, enter);
			}
			else {
				printf("Out of the board!");		// thông báo thuyền nằm bên ngoài khu vực (nếu có)
				Sleep(500);
			}
		}
		else if (user == KEY_LEFT) {					
			if (n > 0) {								
				n--;
				refreshBoard(ptrBoard);
				placeBomb(ptrBoard, m, n, enter);
			}
			else {
				printf("Out of the board!");		
				Sleep(500);
			}
		}
		else if (user == KEY_UP) {					
			if (m > 0) {								
				m--;
				refreshBoard(ptrBoard);
				placeBomb(ptrBoard, m, n, enter);
			}
			else {
				printf("Out of the board!");		
				Sleep(500);
			}
		}
		else if (user == KEY_DOWN) {				
			if (m < BOARDSIZE - 1) {					
				m++;
				refreshBoard(ptrBoard);
				placeBomb(ptrBoard, m, n, enter);
			}
			else {
				printf("Out of the board!");		
				Sleep(500);
			}
		}
		else if (user == KEY_ENTER) {			
			if (placeBomb(ptrBoard, m, n, 1) == 1) {	
				break;
			}
			if (checkWinner(ptrBoard, player) == 1) {
				system("cls");
				return board;
			}
		}
		system("cls");
	}
	system("cls");
	displayGame(ptrBoard);	
	system("cls");
	return board;
}

void main() {
	Info info;
	Info* ptrinfo;
	Board board[PLAYERNO];
	Board* ptrBoard[PLAYERNO];
	ptrinfo = &info;
	ptrBoard[0] = &board[0];
	ptrBoard[1] = &board[1];
	int player = 0, round, user, order = 1;
	intro();

	for (round = 0; round < 2; round++) {
		while (1) {
			printf("\n\n\n\n\tPlayer %d's turn...\n", round + 1);				
			printf("\n\n\tPress enter to go on...");						
			if (user = _getch() == KEY_ENTER)
				break;
			system("cls");
		}
		board[order] = selectionWindow(board[order], ptrBoard[order]);	
		displayBoard(ptrBoard[order]);									
		system("cls");
		order--;
		
	}
	while (1) {
		board[player % 2] = mainGame(board[player % 2], ptrBoard[player % 2], player % 2);	
		displayGame(ptrBoard[player % 2]);													 
		system("cls");
		if (checkWinner(ptrBoard[player % 2], player % 2) == 1) {							
			printf("\nPlayer %d wins!!\n", player % 2 + 1);
			break;
		}
		player++;
	}
	getch();
}
