#include<stdio.h>
#include<easyx.h>
#include<cmath>
#include<iostream>
#include<cstring>
using namespace std;

#define GRID_W 25  //�߼��
#define SPACE (3*GRID_W)  //����λ���봰�ڱ�Ե���
#define ROW 15  //���̺���
#define COL 15  //��������

void draw();  //��������
void mouseMoveMsg(ExMessage* msg);  //����ƶ���Ϣ
void mousePressMsg(ExMessage* msg);  //�������Ϣ

enum Chess {  //���ӣ�����������ֲ̾�map���顣
	None = 0,
	Blue = 1,
	White = -1,
};
//�����ͼ����洢���ӣ�0��ʾ���壬1��ʾ���壬-1��ʾ���塣
int map[ROW][COL] = {};

struct Pos {
	int row;  //���Ӻ�����
	int col;  //����������
	bool isShow;
	int player;  //����
}pos;

int main() {
	//����
	initgraph(600, 500);
	pos.player = Blue;  //��ʼ������ΪBlue
	while (true) {
		// ��������
		BeginBatchDraw();
		draw();
		EndBatchDraw();
		//������Ϣ
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
	//����������
	setlinestyle(PS_SOLID, 1);  //�߿�
	
	for (int i = 0; i < 15; i++) {
		line(SPACE, i * GRID_W + SPACE, 14 * GRID_W + SPACE, i * GRID_W + SPACE);
		line(i * GRID_W + SPACE, SPACE, i * GRID_W + SPACE, 14 * GRID_W + SPACE);
	}
	//��߿�Ӵ�
	setlinestyle(PS_SOLID, 2);
	rectangle(SPACE, SPACE, SPACE + 14 * GRID_W, SPACE + 14 * GRID_W);

	//����
	setfillcolor(RED);
	solidcircle(SPACE + 7 * GRID_W, SPACE + 7 * GRID_W, 5);
	solidcircle(SPACE + 3 * GRID_W, SPACE + 3 * GRID_W, 5);
	solidcircle(SPACE + 11 * GRID_W, SPACE + 3 * GRID_W, 5);
	solidcircle(SPACE + 3 * GRID_W, SPACE + 11 * GRID_W, 5);
	solidcircle(SPACE + 11 * GRID_W, SPACE + 11 * GRID_W, 5);

	//��������
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
	//����У׼�������������λ��У׼��
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
		//����
		map[pos.row][pos.col] = pos.player;
		//�ж���Ӯ
		string winPlayer = (pos.player == Blue ? "Blue" : "White");
		string winMsg = (winPlayer + "��ҹ�ϲ����ʤ����Ϸ����");
		//����
		int hcounter = 0;
		for (int i = max(pos.col - 4, 0); i <= min(pos.col + 4, COL); i++) {
				(map[pos.row][i] == pos.player)? hcounter++ : hcounter = 0;
				if (hcounter == 5) {
					draw();
					 MessageBoxA(NULL, winMsg.c_str(), ("��ʾ"), MB_OK | MB_ICONINFORMATION);
					 exit(0);
				}
		}
		//����
		int vcounter = 0;
		for (int k = max(pos.row - 4, 0); k <= min(pos.row + 4, ROW); k++) {
			(map[k][pos.col] == pos.player) ? vcounter++ : vcounter = 0;
			if (vcounter == 5) {
				draw();
				MessageBoxA(NULL, winMsg.c_str(), ("��ʾ"), MB_OK | MB_ICONINFORMATION);
				exit(0);
			}
		}
		//���϶Խ���
		int lcounter = 0;
		//������ֵ��ֹ����Խ��
		int lx = min(pos.col, pos.row);  //��ǰ���Ӿ����ϱ߽����߽��������
		lx = min(4, lx); 
		int ly = max(COL-pos.col, ROW-pos.row);  //��ǰ���Ӿ����ұ߽���±߽��������
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
				MessageBoxA(NULL, winMsg.c_str(), ("��ʾ"), MB_OK | MB_ICONINFORMATION);
				exit(0);
			}
		}

		//���¶Խ���
		int rcounter = 0;
		int rx = min(pos.row, COL-pos.col);  //��ǰ���Ӿ����±߽����߽��������
		rx = min(4, rx);
		int ry = max(ROW - pos.row, pos.col);  //��ǰ���Ӿ����ұ߽���ϱ߽��������
		ry = min(ry, 4);

		for (int i = -rx; i < ry; i++) {
			map[pos.row + i][pos.col - i] == pos.player ? rcounter++ : rcounter = 0;
			if (rcounter == 5) {
				draw();
				MessageBoxA(NULL, winMsg.c_str(), ("��ʾ"), MB_OK | MB_ICONINFORMATION);
				exit(0);
			}
		}
		//�л�����
		pos.player = (pos.player == Blue ? White : Blue);
	}
}
