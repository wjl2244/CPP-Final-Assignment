#include<iostream>
#include<fstream>
#include<vector>
#include<string>
#include <direct.h>

using namespace std;

#define INCOME "收入"
#define EXPAND "支出"
#define FILENAME "AccountBook.txt"

//定义账目结构体类型
struct AccountItem {
	string itemType;
	int amount;
	string detail;
};


//绘制菜单
class Menu{
	public:
		void showMainMenu();  //显示主菜单
		void showAccountingMenu();  //显示账目菜单
		void showQueryMenu();  //显示查询菜单
};


char readMenuSelection(int options);  //读取输入菜单命令
char readQuitConfirm();  //读取输入退出命令
double readAmount();  //读取账目

void loadDataFromFile(vector<AccountItem>& items);  //加载账目数据
void accouting(vector<AccountItem>& items);  //记账
void query(const vector<AccountItem>& items);  //查账
void insertIntoFile(const AccountItem& item);  //记账时向文件加入新帐目
void income(vector<AccountItem>& items); //收入账目
void expand(vector<AccountItem>& items);  //支出账目
void queryItem(const vector<AccountItem>& items); //查询账目
void queryItem(const vector<AccountItem>& items, const string itemType);



int main() {
	FILE* fp;
	fp = fopen(FILENAME, "a+");
	fclose(fp);
	//加载文件中的账目
	vector<AccountItem> items;
	loadDataFromFile(items);
	
	Menu showMenu;

	bool quit = false;
	while (!quit) {
		//显示主菜单
		showMenu.showMainMenu();

		//读取键盘选择，做合法性检验
		char key = readMenuSelection(3);

		switch (key) {
		case '1':  //记账
			showMenu.showAccountingMenu();
			accouting(items);
			break;

		case '2':  //查询
			showMenu.showQueryMenu();
			query(items);
			break;

		case '3': //退出
			cout << "\n确认退出？(Y/N)：";
			if (readQuitConfirm() == 'Y') {
				quit = true;
			}
			break;

		default:
			break;
		}
		cout << endl;
	}
}


void Menu::showMainMenu() {
	//清屏
	system("cls");
	cout << "----------------------------------" << endl;
	cout << "|====== 欢迎使用简易记账本 ======|" << endl;
	cout << "| \t\t\t\t |" << endl;
	cout << "|********   1  记 账   **********|" << endl;
	cout << "|********   2  查 询   **********|" << endl;
	cout << "|********   3  退 出   **********|" << endl;
	cout << "|--------------------------------|" << endl;

	cout << "\n请选择（1 - 3）：";

}
//绘制记账菜单
void Menu::showAccountingMenu() {
	cout << "----------------------------------" << endl;
	cout << "|====== 请选择您的记账种类 ======|" << endl;
	cout << "| \t\t\t\t |" << endl;
	cout << "|********   1  收 入   **********|" << endl;
	cout << "|********   2  支 出   **********|" << endl;
	cout << "|********   3  返 回   **********|" << endl;
	cout << "|--------------------------------|" << endl;

	cout << "\n请选择（1 - 3）：";
}
//绘制查询菜单
void Menu::showQueryMenu() {
	cout << "------------------------------------------" << endl;
	cout << "|============ 请选择查询类型 ============|" << endl;
	cout << "| \t\t\t\t \t |" << endl;
	cout << "|********   1   统计所有账目   **********|" << endl;
	cout << "|********   2   统计收入       **********|" << endl;
	cout << "|********   3   统计支出       **********|" << endl;
	cout << "|********   4   返回主菜单     **********|" << endl;
	cout << "|----------------------------------------|" << endl;

	cout << "\n请选择（1 - 4）：";
}


void loadDataFromFile(vector<AccountItem>& items) {
	ifstream input(FILENAME);
	//读取每一条账目并包装成AccountItem数据
	AccountItem item;
	while (input >> item.itemType >> item.amount >> item.detail) {
		items.push_back(item);
	}
	input.close();
}

void accouting(vector<AccountItem>& items) {
	//读取键盘选择，做合法性检验
	char key = readMenuSelection(3);

	switch (key) {
	case '1':  //收入
		income(items);
		break;

	case '2':  //支出
		expand(items);
		break;

	default:
		break;
	}
}
//记录收入
void income(vector<AccountItem>& items) {
	AccountItem item;
	item.itemType = INCOME;
	cout << "\n本次收入金额：";
	item.amount = readAmount();
	cout << "\n备注：";
	getline(cin, item.detail);

	//添加到容器中
	items.push_back(item);
	//保存到文件
	insertIntoFile(item);

	cout << "\n-----------记账成功！-----------\n" << endl;
	cout << "\n请按回车返回主菜单...";
	string enter;
	getline(cin, enter);
}
//记录支出
void expand(vector<AccountItem>& items) {
	AccountItem item;
	item.itemType = EXPAND;
	cout << "\n本次支出金额：";
	item.amount = -readAmount();
	cout << "\n备注：";
	getline(cin, item.detail);

	//添加到容器中
	items.push_back(item);
	//保存到文件
	insertIntoFile(item);

	cout << "\n-----------记账成功！-----------\n" << endl;
	cout << "\n请按回车返回主菜单...";
	string enter;
	getline(cin, enter);
}


//向文件插入信息
void insertIntoFile(const AccountItem& item) {
	//输出到文件（追加方式）
	ofstream output(FILENAME, ios::out | ios::app);
	output << item.itemType << "\t" << item.amount << "\t" << item.detail << endl;
	output.close();
}


//查询
void query(const vector<AccountItem>& items) {
	//读取键盘选择，做合法性检验
	char key = readMenuSelection(4);

	switch (key) {
	case '1':  //查询所有账目，统计总账目
		queryItem(items);
		break;

	case '2':  //查询收入，统计总收入
		queryItem(items, INCOME);
		break;

	case '3':  //查询支出，统计总支出
		queryItem(items, EXPAND);
		break; 

	default:
		break;
	}

}

void queryItem(const vector<AccountItem>& items) {
	int total = 0;
	cout << "---------- 查询结果 -----------" << endl;
	cout << "\n 类别\t金额\t备注\n" << endl;
	for (auto item : items) {
		cout << item.itemType << "\t" << item.amount << "\t" << item.detail << endl;
		total += item.amount;
	};
	cout << "===============================";
	cout << "\n总帐目为：" << total << endl;

	cout << "\n请按回车返回主菜单...";
	string enter;
	getline(cin, enter);
}

void queryItem(const vector<AccountItem>& items, const string itemType) {
	int total = 0;
	cout << "---------- 查询结果 -----------" << endl;
	cout << "\n 类别\t金额\t备注\n" << endl;
	for (auto item : items) {
		if (item.itemType == itemType) {
			cout << item.itemType << "\t" << item.amount << "\t" << item.detail << endl;
			total += item.amount;
		}
	};
	cout << "===============================";
	cout << ((itemType == INCOME) ? "\n总收入为：" : "\n总支出为：") << total << endl;

	cout << "\n请按回车返回主菜单...";
	string enter;
	getline(cin, enter);
}


//读取键盘输入选项
char readMenuSelection(int options) {
	string str;

	//合法性校验
	while (true) {
		getline(cin, str);
		if (str.size() != 1 || str[0] - '0' <= 0 || str[0] - '0' > options) {
			cout << "你的输入不合法，请重新输入：";
		}
		else {
			break;
		}
	}

	return str[0];
}

char readQuitConfirm() {
	string str;

	while (true) {
		getline(cin, str);
		if (str.size() != 1 || toupper(str[0]) != 'Y' && toupper(str[0]) != 'N') {
			//保留输入的全部字符
			cout << "你的输入不合法，请重新输入：";
		}
		else {
			break;
		}
	}
	return toupper(str[0]);
}

//读取输入金额
double readAmount() {
	double amount;
	string str;
	while (true)
	{
		getline(cin, str);
		try
		{
			amount = stod(str); //将str转为int
			break;
		}
		catch (const exception& e)
		{
			cout << "你的输入不合法，请重新输入：";
		}
	}
	return amount;
}



