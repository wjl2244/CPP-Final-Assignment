#include<stdio.h>
#include<easyx.h>
#include<cmath>
#include<iostream>
#include<cstring>
using namespace std;

#define GRID_W 25  //线间距
#define SPACE (3*GRID_W)  //棋盘位置与窗口边缘间距
#define ROW 15  //棋盘横线
#define COL 15  //棋盘竖线

void draw();  //绘制棋盘
void mouseMoveMsg(ExMessage* msg);  //鼠标移动消息
void mousePressMsg(ExMessage* msg);  //鼠标点击消息

enum Chess {  //棋子，用来填充棋盘局部map数组。
	None = 0,
	Blue = 1,
	White = -1,
};
//定义地图数组存储棋子，0表示无棋，1表示黑棋，-1表示白棋。
int map[ROW][COL] = {};

struct Pos {
	int row;  //棋子横坐标
	int col;  //棋子纵坐标
	bool isShow;
	int player;  //棋手
}pos;

int main() {
	//画布
	initgraph(600, 500);
	pos.player = Blue;  //初始化棋手为Blue
	while (true) {
		// 绘制棋盘
		BeginBatchDraw();
		draw();
		EndBatchDraw();
		//处理消息
		ExMessage msg;
		if (peekmessage(&msg, EX_MOUSE)) {
			switch (msg.message){
			case WM_MOUSEMOVE:
			mouseMoveMsg(&msg);

			case WM_LBUTTONDOWN:
				mousePressMsg(&msg);

			default:
				break;
			}
		}
	}

	getchar();
	return 0;
}

void draw() {
	//棋盘网格线
	setlinestyle(PS_SOLID, 1);  //线宽
	
	for (int i = 0; i < 15; i++) {
		line(SPACE, i * GRID_W + SPACE, 14 * GRID_W + SPACE, i * GRID_W + SPACE);
		line(i * GRID_W + SPACE, SPACE, i * GRID_W + SPACE, 14 * GRID_W + SPACE);
	}
	//外边框加粗
	setlinestyle(PS_SOLID, 2);
	rectangle(SPACE, SPACE, SPACE + 14 * GRID_W, SPACE + 14 * GRID_W);

	//画点
	setfillcolor(RED);
	solidcircle(SPACE + 7 * GRID_W, SPACE + 7 * GRID_W, 5);
	solidcircle(SPACE + 3 * GRID_W, SPACE + 3 * GRID_W, 5);
	solidcircle(SPACE + 11 * GRID_W, SPACE + 3 * GRID_W, 5);
	solidcircle(SPACE + 3 * GRID_W, SPACE + 11 * GRID_W, 5);
	solidcircle(SPACE + 11 * GRID_W, SPACE + 11 * GRID_W, 5);

	//绘制棋子
	for (int i = 0; i < ROW; i++) {
		for (int j = 0; j < COL; j++) {
			if (map[i][j] == Blue) {
				setfillcolor(BLUE);
				solidcircle(j * GRID_W + SPACE, i * GRID_W + SPACE, 10);
			}
			else if (map[i][j] == White) {
				setfillcolor(WHITE);
				solidcircle(j * GRID_W + SPACE, i * GRID_W + SPACE, 10);
			}
		}
	}

	
	/*if (pos.isShow) {
		rectangle(pos.col*GRID_W+SPACE-12, pos.row * GRID_W + SPACE-12, pos.col * GRID_W + SPACE + 12, pos.row * GRID_W + SPACE + 12);
	}*/
	
}

void mouseMoveMsg(ExMessage* msg) {
	pos.isShow = false;
	//坐标校准（鼠标点击与落子位置校准）
	for (int i = 0; i < ROW; i++) {
		for (int j = 0; j < COL; j++) {
			int gridx = j * GRID_W + SPACE;
			int gridy = i * GRID_W + SPACE;
			if (abs(msg->x - gridx) <= 12 && abs(msg->y - gridy) <= 12) {
				pos.isShow = true;
				pos.row = i;
				pos.col = j;
			}
		}
	}
}

void mousePressMsg(ExMessage* msg) {
	if (msg->message == WM_LBUTTONDOWN) {
		//落子
		map[pos.row][pos.col] = pos.player;
		//判断输赢
		string winPlayer = (pos.player == Blue ? "Blue" : "White");
		string winMsg = (winPlayer + "玩家恭喜您获胜！游戏结束");
		//横向
		int hcounter = 0;
		for (int i = max(pos.col - 4, 0); i <= min(pos.col + 4, COL); i++) {
				(map[pos.row][i] == pos.player)? hcounter++ : hcounter = 0;
				if (hcounter == 5) {
					draw();
					 MessageBoxA(NULL, winMsg.c_str(), ("提示"), MB_OK | MB_ICONINFORMATION);
					 exit(0);
				}
		}
		//纵向
		int vcounter = 0;
		for (int k = max(pos.row - 4, 0); k <= min(pos.row + 4, ROW); k++) {
			(map[k][pos.col] == pos.player) ? vcounter++ : vcounter = 0;
			if (vcounter == 5) {
				draw();
				MessageBoxA(NULL, winMsg.c_str(), ("提示"), MB_OK | MB_ICONINFORMATION);
				exit(0);
			}
		}
		//左上对角线
		int lcounter = 0;
		//处理阈值防止数组越界
		int lx = min(pos.col, pos.row);  //当前棋子距离上边界和左边界最近距离
		lx = min(4, lx); 
		int ly = max(COL-pos.col, ROW-pos.row);  //当前棋子距离右边界和下边界最近距离
		ly = min(ly, 4);

		for (int i = -lx; i <= ly; i++) {
			//cout << "i=  "<<i << "\tpos.col+i  "<<pos.col+i << "\tpos.row+i  " << pos.row+i << "\tmap[pos.col + i][pos.row + i]  " << map[pos.col + i][pos.row + i] <<'\t'<<pos.player << endl;
			if (map[pos.row + i][pos.col + i] == pos.player) {
				lcounter++;
			}else {
				lcounter = 0;
			}
			if (lcounter == 5) {
				draw();
				MessageBoxA(NULL, winMsg.c_str(), ("提示"), MB_OK | MB_ICONINFORMATION);
				exit(0);
			}
		}

		//左下对角线
		int rcounter = 0;
		int rx = min(pos.row, COL-pos.col);  //当前棋子距离下边界和左边界最近距离
		rx = min(4, rx);
		int ry = max(ROW - pos.row, pos.col);  //当前棋子距离右边界和上边界最近距离
		ry = min(ry, 4);

		for (int i = -rx; i < ry; i++) {
			map[pos.row + i][pos.col - i] == pos.player ? rcounter++ : rcounter = 0;
			if (rcounter == 5) {
				draw();
				MessageBoxA(NULL, winMsg.c_str(), ("提示"), MB_OK | MB_ICONINFORMATION);
				exit(0);
			}
		}
		//切换棋手
		pos.player = (pos.player == Blue ? White : Blue);
	}
}
