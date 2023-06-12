#include<iostream>
#include<fstream>
#include<vector>
#include<string>
#include <direct.h>

using namespace std;

#define INCOME "����"
#define EXPAND "֧��"
#define FILENAME "AccountBook.txt"

//������Ŀ�ṹ������
struct AccountItem {
	string itemType;
	int amount;
	string detail;
};


//���Ʋ˵�
class Menu{
	public:
		void showMainMenu();  //��ʾ���˵�
		void showAccountingMenu();  //��ʾ��Ŀ�˵�
		void showQueryMenu();  //��ʾ��ѯ�˵�
};


char readMenuSelection(int options);  //��ȡ����˵�����
char readQuitConfirm();  //��ȡ�����˳�����
double readAmount();  //��ȡ��Ŀ

void loadDataFromFile(vector<AccountItem>& items);  //������Ŀ����
void accouting(vector<AccountItem>& items);  //����
void query(const vector<AccountItem>& items);  //����
void insertIntoFile(const AccountItem& item);  //����ʱ���ļ���������Ŀ
void income(vector<AccountItem>& items); //������Ŀ
void expand(vector<AccountItem>& items);  //֧����Ŀ
void queryItem(const vector<AccountItem>& items); //��ѯ��Ŀ
void queryItem(const vector<AccountItem>& items, const string itemType);



int main() {
	FILE* fp;
	fp = fopen(FILENAME, "a+");
	fclose(fp);
	//�����ļ��е���Ŀ
	vector<AccountItem> items;
	loadDataFromFile(items);
	
	Menu showMenu;

	bool quit = false;
	while (!quit) {
		//��ʾ���˵�
		showMenu.showMainMenu();

		//��ȡ����ѡ�����Ϸ��Լ���
		char key = readMenuSelection(3);

		switch (key) {
		case '1':  //����
			showMenu.showAccountingMenu();
			accouting(items);
			break;

		case '2':  //��ѯ
			showMenu.showQueryMenu();
			query(items);
			break;

		case '3': //�˳�
			cout << "\nȷ���˳���(Y/N)��";
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
	//����
	system("cls");
	cout << "----------------------------------" << endl;
	cout << "|====== ��ӭʹ�ü��׼��˱� ======|" << endl;
	cout << "| \t\t\t\t |" << endl;
	cout << "|********   1  �� ��   **********|" << endl;
	cout << "|********   2  �� ѯ   **********|" << endl;
	cout << "|********   3  �� ��   **********|" << endl;
	cout << "|--------------------------------|" << endl;

	cout << "\n��ѡ��1 - 3����";

}
//���Ƽ��˲˵�
void Menu::showAccountingMenu() {
	cout << "----------------------------------" << endl;
	cout << "|====== ��ѡ�����ļ������� ======|" << endl;
	cout << "| \t\t\t\t |" << endl;
	cout << "|********   1  �� ��   **********|" << endl;
	cout << "|********   2  ֧ ��   **********|" << endl;
	cout << "|********   3  �� ��   **********|" << endl;
	cout << "|--------------------------------|" << endl;

	cout << "\n��ѡ��1 - 3����";
}
//���Ʋ�ѯ�˵�
void Menu::showQueryMenu() {
	cout << "------------------------------------------" << endl;
	cout << "|============ ��ѡ���ѯ���� ============|" << endl;
	cout << "| \t\t\t\t \t |" << endl;
	cout << "|********   1   ͳ��������Ŀ   **********|" << endl;
	cout << "|********   2   ͳ������       **********|" << endl;
	cout << "|********   3   ͳ��֧��       **********|" << endl;
	cout << "|********   4   �������˵�     **********|" << endl;
	cout << "|----------------------------------------|" << endl;

	cout << "\n��ѡ��1 - 4����";
}


void loadDataFromFile(vector<AccountItem>& items) {
	ifstream input(FILENAME);
	//��ȡÿһ����Ŀ����װ��AccountItem����
	AccountItem item;
	while (input >> item.itemType >> item.amount >> item.detail) {
		items.push_back(item);
	}
	input.close();
}

void accouting(vector<AccountItem>& items) {
	//��ȡ����ѡ�����Ϸ��Լ���
	char key = readMenuSelection(3);

	switch (key) {
	case '1':  //����
		income(items);
		break;

	case '2':  //֧��
		expand(items);
		break;

	default:
		break;
	}
}
//��¼����
void income(vector<AccountItem>& items) {
	AccountItem item;
	item.itemType = INCOME;
	cout << "\n���������";
	item.amount = readAmount();
	cout << "\n��ע��";
	getline(cin, item.detail);

	//��ӵ�������
	items.push_back(item);
	//���浽�ļ�
	insertIntoFile(item);

	cout << "\n-----------���˳ɹ���-----------\n" << endl;
	cout << "\n�밴�س��������˵�...";
	string enter;
	getline(cin, enter);
}
//��¼֧��
void expand(vector<AccountItem>& items) {
	AccountItem item;
	item.itemType = EXPAND;
	cout << "\n����֧����";
	item.amount = -readAmount();
	cout << "\n��ע��";
	getline(cin, item.detail);

	//��ӵ�������
	items.push_back(item);
	//���浽�ļ�
	insertIntoFile(item);

	cout << "\n-----------���˳ɹ���-----------\n" << endl;
	cout << "\n�밴�س��������˵�...";
	string enter;
	getline(cin, enter);
}


//���ļ�������Ϣ
void insertIntoFile(const AccountItem& item) {
	//������ļ���׷�ӷ�ʽ��
	ofstream output(FILENAME, ios::out | ios::app);
	output << item.itemType << "\t" << item.amount << "\t" << item.detail << endl;
	output.close();
}


//��ѯ
void query(const vector<AccountItem>& items) {
	//��ȡ����ѡ�����Ϸ��Լ���
	char key = readMenuSelection(4);

	switch (key) {
	case '1':  //��ѯ������Ŀ��ͳ������Ŀ
		queryItem(items);
		break;

	case '2':  //��ѯ���룬ͳ��������
		queryItem(items, INCOME);
		break;

	case '3':  //��ѯ֧����ͳ����֧��
		queryItem(items, EXPAND);
		break; 

	default:
		break;
	}

}

void queryItem(const vector<AccountItem>& items) {
	int total = 0;
	cout << "---------- ��ѯ��� -----------" << endl;
	cout << "\n ���\t���\t��ע\n" << endl;
	for (auto item : items) {
		cout << item.itemType << "\t" << item.amount << "\t" << item.detail << endl;
		total += item.amount;
	};
	cout << "===============================";
	cout << "\n����ĿΪ��" << total << endl;

	cout << "\n�밴�س��������˵�...";
	string enter;
	getline(cin, enter);
}

void queryItem(const vector<AccountItem>& items, const string itemType) {
	int total = 0;
	cout << "---------- ��ѯ��� -----------" << endl;
	cout << "\n ���\t���\t��ע\n" << endl;
	for (auto item : items) {
		if (item.itemType == itemType) {
			cout << item.itemType << "\t" << item.amount << "\t" << item.detail << endl;
			total += item.amount;
		}
	};
	cout << "===============================";
	cout << ((itemType == INCOME) ? "\n������Ϊ��" : "\n��֧��Ϊ��") << total << endl;

	cout << "\n�밴�س��������˵�...";
	string enter;
	getline(cin, enter);
}


//��ȡ��������ѡ��
char readMenuSelection(int options) {
	string str;

	//�Ϸ���У��
	while (true) {
		getline(cin, str);
		if (str.size() != 1 || str[0] - '0' <= 0 || str[0] - '0' > options) {
			cout << "������벻�Ϸ������������룺";
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
			//���������ȫ���ַ�
			cout << "������벻�Ϸ������������룺";
		}
		else {
			break;
		}
	}
	return toupper(str[0]);
}

//��ȡ������
double readAmount() {
	double amount;
	string str;
	while (true)
	{
		getline(cin, str);
		try
		{
			amount = stod(str); //��strתΪint
			break;
		}
		catch (const exception& e)
		{
			cout << "������벻�Ϸ������������룺";
		}
	}
	return amount;
}



