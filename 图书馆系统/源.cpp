#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<atltime.h>
#include<iostream>
#include<string>
#include<fstream>
#include <strstream> 
#include <iomanip> 
#include<windows.h>
#include<conio.h>
#include <algorithm>
#define people_max 50    //��������
#define LENGTH_PER_DAY 10    //ϵͳʱ��10���൱��ͼ���ʱ������һ��
#define book_max 100  //�鼮��Ŀ����
static int peoplenum = 0;   //������
static int booknum = 0;  //�ܵ��鼮����
static int liushuinum = 0; //�����鼮�������ˮ��
static int borrowtimes = 0;//�����鱻���ĵĴ���
using namespace std;
//�鼮��Ϣ 
typedef struct book {
	string name;  //����
	string writer;  //������
	char publisher;  //������
	char sort;  //����
	float price;   //�۸�
	string buytime;  //����ʱ��
	string ISBN;//������ISBN
	string number;//�������
	int times;//�����鱻���Ĵ���
	int num;//�����鵱ǰʣ����
	struct book *next; //����ָ��
}book, *booklist;
//ԤԼ��Ϣ
struct appointment {
	string Wantedbook;
	string Time;
};
//������Ϣ
struct role {
	string name;
	string ID;  //ѧ��֤�Ż�����
	char college;
	string password;
	char type;//A�����ʦ��B����ѧ����C�������Ա
	int borrow_max;//Ȩ�ޣ���������������ʦ6ѧ��3
	int borrow_num;//�Ѿ������
	int honesty;//����������
	int fine;//��������δ�黹����ɵķ���
	int appoint_num;//�Ѿ�ԤԼ���������
	struct appointment appointlist[3];//ԤԼ�ṹ�����飬ÿ�����ԤԼ������
};;
//������Ϣ
typedef struct time
{
	int year;
	int month;
	int day;
}date;

typedef struct borrow {
	string personname;//����������
	string bookname;//ͼ������
	date time;//����ʱ��
	struct borrow *next;
}borrow, *borrowlist;

fstream file1;//�����ļ�
fstream file2;//�鼮�ļ�
fstream file3;//�軹��Ϣ�ļ�borrowlog.txt
fstream file4;//��־��Ϣlog.txt
fstream file5;//������Ϣ�ͽ��Ĵ�����Ϣ
void createrole(struct role *p, borrowlist &b, booklist L, date time); //����ע��
int login(struct role *p, booklist L, borrowlist &b, date time); //���˵�½
void showpeopleinfo(struct role * p, int n, borrowlist &b);//չʾ������Ϣ 
void openroletxt(struct role *p); //�������ļ�
int getIDnumber(struct role *p);//��ѧ�Ź����Ž��б���
booklist openbooktext(); //���鼮�ļ�
borrowlist openborrowtext();//�򿪽�����־
void addbook(booklist &L);//��ͼ���������ͼ��
void getbooknumber(book *m);//ͼ����뺯��
int power(int x, int y);//��x��y�η�
void teacher(struct role *p, booklist L, borrowlist &b, date time);//��½ʱ�����ʦ����
void student(struct role *p, booklist L, borrowlist &b, date time);//��¼ʱ����ѧ������
void manager(struct role *p, booklist L, borrowlist &b, date time);//��¼ʱ�������Ա����
string upper(string &str);//ת����Сд
int find_str(string str, string substr);//�ַ���ƥ��
int  findbook(string a, string S);
void writerfind(booklist L);//�������߲���
void publisherfind(booklist L);//���ճ��������ͼ��
void namefind(booklist L);//������������ͼ��
void numfind(booklist L);//������ı�Ų�ѯ
void ISBNfind(booklist L);//������ı�Ų�ѯ
void sortfind(booklist L);//���շ����ѯͼ��
void  Initiate_borrow(borrowlist  &a);//��ʼ������
int  Append_borrow(borrowlist &a, borrow b);//���봴������������
booklist find_name(booklist &a, string m);//ͨ����������
void borrowbook(struct role *p, int n, borrowlist &b, booklist L, date time);//���飬�����뵽������־
void searchbook(booklist head);//��ѯͼ����Ϣ
void appointbook(struct role *p, int n, borrowlist &b, booklist L, string bookname);//ԤԼ����
void checkinbook(struct role *p, int n, borrowlist &b, booklist L, date time);//���ԤԼͼ���Ƿ���Խ���
int findbook(string bookname, booklist L);//�������Ƿ�������Ϊ0
void loading(borrowlist &b);//ÿ�ο�ʼ����ʱ�����½������¼�ĵ��е����ݶ����ڴ���
void Savebook(booklist a);//ˢ���޸ĺ���ĵ�����������
void Saveborrow(borrowlist b, date time);//���½����¼����
void returnbook(struct role *p, int n, borrowlist &b, booklist L, date time);//���鲢ˢ���ĵ�
void Saverole(struct role *p);//����role.txt
void Search_book(borrowlist &b, booklist L);//����Ա�����鼮��Ϣ
void Search_person(struct role *p, borrowlist &b);//����Ա������Ա��Ϣ
void Search_onebook(borrowlist &b, booklist L);//����Ա���ҵ����鼮��Ϣ
void Search_typebook(booklist L);//����Ա��ѯһ���鼮������
date Add_One_Day(date date);//���ݾ�������������ڵ�����
int GetMonthDays(int year, int month);//�õ�ÿ���ж�����ĺ��� 
void Savedate(date Time);//ˢ���޸ĺ��ʱ���ĵ�����������
int isLeap(int year);//�ж��Ƿ�������ĺ��� 
void checkoverdue(struct role *a, int n, borrowlist &b, booklist L, date time);//�ж���û���鼮����δ�黹���
int caldays(date t1, date t2);//����t2��t1ǰ���˶�����
void Merge(int *sr, int *tr, string *mr, string *nr, int i, int m, int n);//�鲢����
void MSort(int *sr, int *tr1, string *mr, string *mr1, int s, int t);//�鲢����
void raterank(booklist L);//����������
void get_next(int *next, char *a, int la); //��next[]��ֵ
int str_kmp(int *next, char *A, char *a, int lA, int la);
int judge(string a, string b);

void main_menu(struct role *people, booklist Head, borrowlist Borrow, date time)//���˵�����
{
	int i;
	cout << "��������Ҫע�ỹ�ǵ�½��ע��ѡ��1����½ѡ��2,�˳�ѡ��0" << endl;
	cin >> i;
	while (i < 0 || i>2)
	{
		cout << "��������������������룡";
		cout << "��������Ҫע�ỹ�ǵ�½��ע��ѡ��1����½ѡ��2,�˳�ѡ��0" << endl;
		cin >> i;
	}
	if (i >= 0 && i <= 2)
	{
		switch (i)
		{
		case 1:
			createrole(people, Borrow, Head, time);//ע��
			break;
		case 2:
			login(people, Head, Borrow, time);//�����½����
			break;
		case 0:
			i = 0;
			break;
		default:
			cout << "��������" << endl;
			break;
		}
	}
}

int main()
{
	date date;
	file5.open("configuration.txt", ios::in);
	while (!file5.eof())
	{
		file5 >> date.year >> date.month >> date.day >> borrowtimes;//�������ļ��ж�ȡʱ�䲢����ṹ��
	}
	file5.close();
	struct role people[people_max];
	booklist Head = NULL;
	Head = openbooktext();
	openroletxt(people);
	borrowlist Borrow = NULL;
	Borrow = openborrowtext();
	clock_t start, finish;
	double duration = 0, PauseTime = 0;
	if (&date != NULL)
	{
		system("cls");
		cout << date.year << " " << date.month << " " << date.day;//�����ǰ����
		cout << "��ӭ����ͼ���!" << endl;
		start = clock();//��ʼ��ʱ
		for (;;)
		{

			finish = clock();
			duration = (double)(finish - start) / CLOCKS_PER_SEC;//�����ʱ���������
			if (_kbhit() != 0)//�������������
			{
				PauseTime = PauseTime + duration;//pausetime��ʾ��֮ͣǰ��ʱ���ܺ�
				_getch();
				main_menu(people, Head, Borrow, date);//��������������
				cout << date.year << " " << date.month << " " << date.day;//�˳�ʱ���ӡ��ʱ�䣬������ϵͳʱ�������Ӧ��ʱ���ƽ�
				cout << "��ӭ����ͼ���!" << endl;
				start = clock();
				duration = 0;
			}
			if ((duration + PauseTime) > LENGTH_PER_DAY)//�����ʱ�������ͼ���ϵͳ��һ�죬��10��
			{
				int t = (duration + PauseTime) / LENGTH_PER_DAY;
				date = Add_One_Day(date);//ʱ����������һ��
				system("cls");
				cout << date.year << " " << date.month << " " << date.day;
				cout << "��ӭ����ͼ���!" << endl;
				start = clock();
				PauseTime = 0;

			}
		}
	}
	return 0;
}

void openroletxt(struct role *p)//��������Ϣ�ļ���������������Ϣ������
{
	int num;
	file1.open("role.txt", ios::in);
	if (!file1.is_open())
	{
		cout << "Error opening file"; exit(1);
	}
	while (!file1.eof())
	{
		string r;
		file1 >> r;
		if (file1.eof())
			break;
		else
			p[peoplenum].name = r;
		file1 >> p[peoplenum].ID >> p[peoplenum].college >> p[peoplenum].password >> p[peoplenum].type >> p[peoplenum].borrow_max >> p[peoplenum].borrow_num >> p[peoplenum].honesty >> p[peoplenum].fine >> p[peoplenum].appoint_num;
		if (p[peoplenum].appoint_num != 0)
		{
			for (num = 1; num <= p[peoplenum].appoint_num; num++)
				file1 >> p[peoplenum].appointlist[num - 1].Wantedbook;
		}
		peoplenum++;
	}
	file1.close();
}

booklist openbooktext()//���鼮��Ϣ�ļ��������鼮��Ϣ����������
{
	book* L = new book; book *r;
	L->next = NULL;//����ͷ���
	r = L; //rʼ��ָ������ı�β
	file2.open("book.txt", ios::in);
	if (!file2.is_open())
	{
		cout << "Error opening file"; exit(1);
	}
	while (!file2.eof())
	{
		book* s = new book;
		file2 >> s->name;
		if (file2.eof())
			break;
		file2 >> s->writer >> s->publisher >> s->sort >> s->price >> s->buytime >> s->ISBN >> s->number >> s->times >> s->num;
		booknum = booknum + s->num;
		liushuinum++;
		r->next = s;   r = s;
	}
	r->next = NULL;
	file2.close();
	return L;
}

borrowlist openborrowtext()//��������Ϣ�����������Ľṹ���Ӧ�ı�����
{
	borrowlist b = new borrow; borrow *r;
	b->next = NULL;//����ͷ���
	r = b; //rʼ��ָ������ı�β
	file3.open("borrowlog.txt", ios::in);
	if (!file3.is_open())
	{
		cout << "Error opening file"; exit(1);
	}
	while (!file3.eof())
	{
		borrow* s = new borrow;
		file3 >> s->personname;
		if (file3.eof())
			break;
		file3 >> s->bookname >> s->time.year >> s->time.month >> s->time.day;
		r->next = s;   r = s;
	}
	r->next = NULL;
	file3.close();
	return b;
}

void createrole(struct role *p, borrowlist &b, booklist L, date time)//ע���µ�����
{
	string key;
	cout << "������������Ϊ����ʦ����A��ѧ������B������Ա����C����";
	cin >> p[peoplenum].type;
	if (p[peoplenum].type == 'C')
	{
		cout << "�������������룺";//��������Ϊ111111
		//ֻ������������ȷ����ע�����Ա��ȷ��ʵ��Ӧ��ʱ�İ�ȫ��
		cin >> key;
		if (key != "111111")
		{
			cout << "�������������������,�޷�ע�����Ա��\n";
			system("pause");
			system("cls"); //��������
			main_menu(p, L, b, time);
			exit(0);
		}
		else
			cout << "����������ȷ��\n";
	}
	file1.open("role.txt", ios::app);//��ע���������Ϣд�������ļ���
	cout << "�������������Ϊ��";
	cin >> p[peoplenum].name;
	cout << "���������ѧԺ��";
	cin >> p[peoplenum].college;
	cout << "������������룺";
	cin >> p[peoplenum].password;
	if (p[peoplenum].type == 'A')
	{
		p[peoplenum].borrow_max = 6;
		p[peoplenum].borrow_num = 0;
		p[peoplenum].honesty = 100;
		p[peoplenum].appoint_num = 0;
	}
	else if (p[peoplenum].type == 'B')
	{
		p[peoplenum].borrow_max = 3;
		p[peoplenum].borrow_num = 0;
		p[peoplenum].honesty = 100;
		p[peoplenum].appoint_num = 0;
	}
	else if (p[peoplenum].type == 'C')
	{
		p[peoplenum].borrow_max = 6;
		p[peoplenum].borrow_num = 0;
		p[peoplenum].honesty = 100;
		p[peoplenum].appoint_num = 0;
	}
	p[peoplenum].fine = 0;
	p[peoplenum].ID = getIDnumber(p);
	cout << "���ѧ��/������Ϊ" << getIDnumber(p);
	file1 << p[peoplenum].name << " " << p[peoplenum].ID << " " << p[peoplenum].college << " " << p[peoplenum].password << " " << p[peoplenum].type << " " << p[peoplenum].borrow_max << " " << p[peoplenum].borrow_num << " " << p[peoplenum].honesty << " " << p[peoplenum].fine << " " << p[peoplenum].appoint_num << endl;
	file1.close();
	peoplenum++;
	system("pause");
	system("cls"); //��������
	main_menu(p, L, b, time);
}

int getIDnumber(struct role *p)
{//���ߵı�����Բ������¸�ʽ��X�����ͣ�XX��ѧԺ���룩XXXX����ˮ�ţ�
	std::strstream ss; // #include <strstream>  
	long long value = ((p[peoplenum].type - 'A' + 1)*power(10, 6) + (p[peoplenum].college - 'A' + 1)*power(10, 4) + peoplenum);
	ss << value;
	ss >> p[peoplenum].ID;
	return  value;
}

int login(struct role *p, booklist L, borrowlist &b, date time)//��¼����
{
	char type;
	cout << "�����������ݣ���ʦ����A��ѧ������B������Ա����C����";
	cin >> type;
	switch (type - 'A')
	{
	case 0:
		teacher(p, L, b, time);
		break;
	case 1:
		student(p, L, b, time);
		break;
	case 2:
		manager(p, L, b, time);
		break;
	default:
		return 0;
		break;
	}
	return 1;
}

void teacher(struct role *p, booklist L, borrowlist &b, date time)//��ʦ��¼����
{
	int func;
	string id, key;
	int n = 0;
	int flag = 1;//���Ʒ�����һ���˵�
	cout << "��ӭ�����ʦͼ��ݽ��棡" << endl;
	cout << "��������Ĺ�����:";
	cin >> id;
	while (n < peoplenum)
	{
		if (id.compare(p[n].ID) == 0 && p[n].type == 'A')
		{
			cout << "���������룺";
			cin >> key;
			while (key.compare(p[n].password) != 0)
			{
				cout << "�����������������������룡" << endl;
				cin >> key;
			}
			if (key.compare(p[n].password) == 0)
			{
				cout << p[n].name << ",���ѵ�½�ɹ�����ӭ����ͼ��ݣ�" << endl;
				checkoverdue(p, n, b, L, time);
				checkinbook(p, n, b, L, time);
				while (flag != 0)
				{
					cout << "��ѯ������Ϣ����1��ͼ���ѯ����2�������ʲ�ѯ����3����������4����������5���˳�ͼ�������0" << endl;
					cout << "��������Ҫѡ��Ĺ��ܣ�";
					cin >> func;
					while (func > 5 || func < 0)
					{
						cout << "���������������������룺";
						cin >> func;
					}
					if (func >= 0 && func <= 5)
					{
						switch (func) {
						case 1:
							showpeopleinfo(p, n, b);
							break;
						case 2:
							searchbook(L);
							break;
						case 3:
							raterank(L);
						case 4:
							borrowbook(p, n, b, L, time);
							break;
						case 5:
							returnbook(p, n, b, L, time);
							break;
						case 0:
							flag = 0;
							break;
						default:
							break;
						}
					}
					system("pause");
					system("cls");
				}
			}
			break;
		}
		else
			n++;
	}
	if (n == peoplenum)
	{
		cout << "ͼ����в����ڸù����ţ����������룡" << endl;
		login(p, L, b, time);
	}
	if (flag == 0)
	{
		system("cls");
		main_menu(p, L, b, time);//���»ص����˵�
	}
}

void student(struct role *p, booklist L, borrowlist &b, date time)//ѧ����¼����
{
	int func;
	string id, key;
	int n = 0;
	int flag = 1;//���Ʒ�����һ���˵�
	cout << "��ӭ����ѧ��ͼ��ݽ��棡" << endl;
	cout << "��������Ĺ�����:";
	cin >> id;
	while (n < peoplenum)
	{
		if (id.compare(p[n].ID) == 0 && p[n].type == 'B')
		{
			cout << "���������룺";
			cin >> key;
			while (key.compare(p[n].password) != 0)
			{
				cout << "�����������������������룡" << endl;
				cin >> key;
			}
			if (key.compare(p[n].password) == 0)
			{
				cout << p[n].name << ",���ѵ�½�ɹ�����ӭ����ͼ��ݣ�" << endl;
				checkoverdue(p, n, b, L, time);
				checkinbook(p, n, b, L, time);
				while (flag != 0)
				{
					cout << "��ѯ������Ϣ����1��ͼ���ѯ����2�������ʲ�ѯ����3����������4����������5���˳�ͼ�������0" << endl;
					cout << "��������Ҫѡ��Ĺ��ܣ�";
					cin >> func;
					while (func > 5 || func < 0)
					{
						cout << "���������������������룺";
						cin >> func;
					}
					if (func >= 0 && func <= 5)
					{
						switch (func) {
						case 1:
							showpeopleinfo(p, n, b);
							break;
						case 2:
							searchbook(L);
							break;
						case 3:
							raterank(L);
						case 4:
							borrowbook(p, n, b, L, time);
							break;
						case 5:
							returnbook(p, n, b, L, time);
							break;
						case 0:
							flag = 0;
							break;
						default:
							break;
						}
					}
					system("pause");
					system("cls");
				}
			}
			break;
		}
		else
			n++;
	}
	if (n == peoplenum)
	{
		cout << "ͼ����в����ڸù����ţ����������룡" << endl;
		login(p, L, b, time);
	}
	if (flag == 0)
	{
		system("cls");
		main_menu(p, L, b, time);
	}
}

void manager(struct role *p, booklist L, borrowlist &b, date time)//����Ա��¼����
{
	string id, key;
	int n, choice;
	int flag = 1;//���ƻص���һ��Ŀ¼
	int state = 1;
	cout << "��ӭ�������Աͼ��ݽ��棡" << endl;
	cout << "��������Ĺ�����:";
	cin >> id;
	for (n = 0; n < peoplenum && state; n++)
	{
		if (id.compare(p[n].ID) == 0 && p[n].type == 'C')
		{
			cout << "���������룺";
			cin >> key;
			while (key.compare(p[n].password) != 0)
			{
				cout << "�����������������������룡" << endl;
				cin >> key;
			}
			if (key.compare(p[n].password) == 0)
			{
				cout << p[n].name << ",���ѵ�½�ɹ�����ӭ����ͼ��ݣ�" << endl;
				checkoverdue(p, n, b, L, time);
				checkinbook(p, n, b, L, time);
				while (flag != 0)
				{
					cout << "1������ͼ��\n2����ѯ�鼮��Ϣ\n3����ѯ��Ա��Ϣ\n0���˳�ͼ���\n��������Ҫѡ��Ĺ��ܣ�";
					cin >> choice;
					while (choice < 0 || choice>3)
					{
						cout << "���벻�Ϲ淶�����������룡";
						cin >> choice;
					}
					switch (choice)
					{
					case 1:addbook(L);
						break;
					case 2:Search_book(b, L);
						break;
					case 3:Search_person(p, b);
						break;
					case 0:flag = 0;
						break;
					default:
						break;
					}
					system("pause");
					system("cls");
				}
			}
			state = 0;
		}
	}
	if (n > peoplenum)
	{
		cout << "ͼ����в����ڸù����ţ����������룡" << endl;
		login(p, L, b, time);
	}

	if (flag == 0)
	{
		system("cls");
		main_menu(p, L, b, time);
	}
}

void showpeopleinfo(struct role *p, int n, borrowlist &b)//��ӡ������Ϣ
{
	cout << "������" << p[n].name << "	" << "ѧԺ��" << p[n].college << endl;
	cout << "��ǰ�Ѿ�����������" << p[n].borrow_num << "	" << "��ǰ�пɽ���������" << p[n].borrow_max - p[n].borrow_num << endl;
	cout << "��ǰ���Ŷȣ�" << p[n].honesty << "  " << "��ǰ��Ƿ����" << p[n].fine << endl;
	cout << "��ǰ�Ѿ����ĵ��鼰�������ڣ�" << endl;
	borrowlist q = b;
	while (q != NULL)
	{
		if (q->personname == p[n].name)
		{
			cout << q->time.year << "/" << q->time.month << "/" << q->time.day << "  ";
			cout << q->bookname << endl;
			q = q->next;
		}
		else
			q = q->next;
	}
}

void getbooknumber(book *m)
//��¼��ͼ����б��룬�������Ϊ XX�����ͣ�XX�������磩XXXX����ˮ�ţ�
{
	std::strstream ss; // #include <strstream>  
	long long value = ((m->sort - 'A' + 1)*power(10, 6) + (m->publisher - 'A' + 1)*power(10, 4) + liushuinum);
	ss << value;
	ss >> m->number;
	liushuinum++;
}

int power(int x, int y)//��x��y�η������벿��
{
	int num = 1, n = 0;
	while (n < y)
	{
		num = num*x;
		n++;
	}
	return num;
}

void writerfind(booklist L)//ѧ���������߲�ѯͼ��
{
	int flag = 0;//�Ƿ���ҵ���ǰ�鼮�ı�־
	char answer;
	string writer;
	cout << "������Ҫ�����鼮�����ߣ�";
	cin >> writer;
	booklist p = L;
	int m = 1;
	while (p)
	{
		if (writer == p->writer)
		{
			if (m == 1)
			{
				cout << "����\t����\t������\t������\t������" << endl;
				cout << p->name << " " << p->writer << " " << p->publisher << " " << p->num << " " << (double)(p->times / borrowtimes) << endl;
				m++;
			}
			else
				cout << p->name << " " << p->writer << " " << p->publisher << " " << p->num << " " << (double)(p->times / borrowtimes) << endl;
			flag++;
		}
		p = p->next;
	}
	cout << "�Ƿ�����Ҫ�ҵ��鼮��Y/N" << endl;
	cin >> answer;
	if (answer == 'Y')
	{
		namefind(L);
	}
	else if (answer == 'N')
		cout << "�Բ���û���ҵ�����Ҫ���鼮��" << endl;
	if (flag == 0)
		cout << "�Բ���û���ҵ�����Ҫ���鼮��" << endl;
}

void publisherfind(booklist L)//���ճ��������ͼ��
{
	int flag = 0;//�Ƿ���ҵ���ǰ�鼮�ı�־
	char answer;
	char publisher;
	cout << "������Ҫ���ҵĳ����磺";
	cin >> publisher;
	booklist p = L;
	int m = 1;
	while (p)
	{
		if (publisher == p->publisher)
		{
			if (m == 1)
			{
				cout << "����\t����\t������\t������\t������" << endl;
				cout << p->name << " " << p->writer << " " << p->publisher << " " << p->num << " " << (double)(p->times / borrowtimes) << endl;
				m++;
			}
			else
				cout << p->name << "��" << p->writer << " " << p->publisher << " " << p->num << " " << (double)(p->times / borrowtimes) << endl;
			flag++;
		}
		p = p->next;
	}
	cout << "�Ƿ�����Ҫ�ҵ��鼮��Y/N" << endl;
	cin >> answer;
	if (answer == 'Y')
	{
		namefind(L);
	}
	else if (answer == 'N')
		cout << "�Բ���û���ҵ�����Ҫ���鼮��" << endl;

	if (flag == 0)
		cout << "�Բ���û���ҵ�����Ҫ���鼮��" << endl;
}

string upper(string &str)//ת����Сд
{
	transform(str.begin(), str.end(), str.begin(), ::toupper);
	return str;
}

int find_str(string str, string substr)//�ַ���ƥ��
{
	return upper(str).find(upper(substr));
}

int  findbook(string a, string S)//ƥ���ַ���
{
	if (find_str(a, S) != -1)
		return 1;
	else
		return 0;
}

void namefind(booklist L)//������������ͼ��
{
	int flag = 0;//�Ƿ���ҵ���ǰ�鼮�ı�־
	char answer;//�Ƿ�ѡ�����ʱ��ѡ�����
	string name;
	cout << "������Ҫ���ҵ�������";
	cin >> name;
	book *head = L;
	while (head)
	{
		if (judge(head->name, name) != 0)
		{
			cout << "����\t����\t������\t������\t������" << endl;
			double  m = (double)head->times / (double)borrowtimes;//������
			cout << head->name << "  " << head->writer << "  " << head->publisher << "  " << head->num << "  " << m * 100 << "%" << endl;
			flag++;
			break;
		}
		else
			head = head->next;
	}
	if (flag == 0)
		cout << "�Բ���û���ҵ�����Ҫ���鼮��" << endl;
}

void numfind(booklist L)//������ı�Ų�ѯͼ��
{
	int flag = 0;//�Ƿ���ҵ���ǰ�鼮�ı�־
	string num;
	cout << "������Ҫ�����鼮�ı�ţ�";
	cin >> num;
	booklist p = L;
	int m = 1;
	while (p)
	{
		if (num == p->number)
		{
			if (m == 1)
			{
				cout << "����\t����\t������\t������\t������" << endl;
				cout << p->name << "  " << p->writer << "  " << p->publisher << "  " << p->num << "  " << (double)((double)p->times / (double)borrowtimes) << endl;
				m++;
			}
			else
				cout << p->name << "  " << p->writer << "  " << p->publisher << "  " << p->num << "  " << (double)((double)p->times / (double)borrowtimes) << endl;
			flag++;
		}
		p = p->next;
	}
	if (flag == 0)
		cout << "�Բ���û���ҵ�����Ҫ���鼮��" << endl;
}

void ISBNfind(booklist L)//������ı�Ų�ѯ
{
	int flag = 0;//�Ƿ���ҵ���ǰ�鼮�ı�־
	string isbn;
	cout << "������Ҫ�����鼮��ISBN���룺";
	cin >> isbn;
	booklist p = L;
	int m = 1;
	while (p)
	{
		if (isbn == p->ISBN)
		{
			if (m == 1)
			{
				cout << "����\t����\t������\t������\t������" << endl;
				cout << p->name << "  " << p->writer << "  " << p->publisher << "  " << p->num << "  " << (double)((double)p->times / (double)borrowtimes) << endl;
				m++;
			}
			else
				cout << p->name << "  " << p->writer << "  " << p->publisher << "  " << p->num << "  " << (double)((double)p->times / (double)borrowtimes) << endl;
			flag++;
		}
		p = p->next;
	}
	if (flag == 0)
		cout << "�Բ���û���ҵ�����Ҫ���鼮��" << endl;
}

void sortfind(booklist L)//���շ����ѯͼ��
{
	int flag = 0;//�Ƿ���ҵ���ǰ�鼮�ı�־
	char ch;
	char answer;
	booklist p = L;
	cout << "������Ҫ�����鼮�ķ��ࣺ";
	cin >> ch;
	int m = 1;
	while (p)
	{
		if (p->sort == ch)
		{
			if (m == 1)
			{
				cout << "����\t����\t������\t������\t������" << endl;
				cout << p->name << "  " << p->writer << "  " << p->publisher << "  " << p->num << "  " << (double)((double)p->times / (double)borrowtimes) << endl;
				m++;
			}
			else
				cout << p->name << "  " << p->writer << "  " << p->publisher << "  " << p->num << "  " << (double)((double)p->times / (double)borrowtimes) << endl;
			flag++;
		}
		p = p->next;
	}
	cout << "�Ƿ�����Ҫ�ҵ��鼮��Y/N";
	cin >> answer;
	if (answer == 'Y')
	{
		namefind(L);
	}
	else if (answer == 'N')
		cout << "�Բ���û���ҵ�����Ҫ���鼮��" << endl;

	if (flag == 0)
		cout << "�Բ���û���ҵ�����Ҫ���鼮��" << endl;
}

void  Initiate_borrow(borrowlist  &a)//��ʼ����������
{
	a = NULL;
}

int  Append_borrow(borrowlist &a, borrow b) //�����������봴�����
{
	borrowlist  p, q;
	q = new borrow;
	if (q == NULL)
		return  0;
	q->personname = b.personname;
	q->bookname = b.bookname;
	q->time = b.time;
	q->next = NULL;
	if (a == NULL)
		a = q;
	else
	{
		for (p = a; p->next; p = p->next);
		p->next = q;
	}
	return  1;
}

booklist find_name(booklist &a, string m)//�����������Ҷ�Ӧ�鼮
{
	booklist p = a;
	for (; p; p = p->next)
		if (m == p->name)
			return p;
}

void borrowbook(struct role *a, int n, borrowlist &b, booklist L, date time) //���飬�����뵽������־
{
	fstream file1("book.txt");
	borrow m;
	booklist q = NULL;
	int c, d, e;//���ԤԼ��������
	m.personname = a[n].name;
	if (a[n].honesty > 0)
		//���Ŷȴ���0�ſ��Խ���
	{
		int num;
		cout << "������������Ŀ��";
		cin >> num;
		getchar();
		while (num > (a[n].borrow_max - a[n].borrow_num))
		{
			cout << "�㵱ǰ���ֻ�ܽ�" << (a[n].borrow_max - a[n].borrow_num) << "���飡" << endl;
			cout << "������������Ŀ��";
			cin >> num;
			getchar();
		}
		cout << "��������Ҫ����������,�û������֣�" << endl;
		string s[6];
		for (int i = 0; i < num; i++)
			getline(cin, s[i]);
		for (int j = 0; j < num; j++)
		{
			m.bookname = s[j];
			int flag = 0;
			booklist p = L;
			while (p != NULL&&flag == 0)
			{
				if (m.bookname == p->name)
				{
					cout << "����  ����  ������  ʣ������" << endl;
					cout << p->name << "  " << p->writer << "  " << p->publisher << "  " << p->num << endl;
					flag++;
				}
				p = p->next;
			}
			if (flag == 0)
				cout << "�Բ���û���ҵ�����Ҫ���鼮��" << endl;
			if (flag != 0)
			{
				q = find_name(L, m.bookname);
				if (q->num == 0)
					appointbook(a, n, b, L, m.bookname); //ͼ���ѱ����꣬����ԤԼ����
				else
				{
					m.time.year = time.year;
					m.time.month = time.month;
					m.time.day = time.day;//������ʱ��������ṹ����
					Append_borrow(b, m);
					q->num--;
					q->times++;
					borrowtimes++;//����ͼ��ݵ��ܽ����������
					Savedate(time);
					a[n].borrow_num++;
					fstream file3("borrowlog.txt", ios::app);
					file3 << m.personname << " " << m.bookname << " " << time.year << " " << time.month << " " << time.day << endl;
					file3.close();
					fstream file4("log.txt", ios::app);
					file4 << m.personname << " " << m.bookname << " " << time.year << "/" << time.month << "/" << time.day << "����" << endl;
					file4.close();
					cout << "��" << time.year << "/" << time.month << "/" << time.day << "����ɹ�������20���ڹ黹" << endl;
					for (c = 1; c <= a[n].appoint_num; c++)
					{
						e = c;
						if (m.bookname == a[n].appointlist[c - 1].Wantedbook)
						{
							for (d = e + 1; j <= a[n].appoint_num; d++, e++)
								a[n].appointlist[d - 1].Wantedbook = a[n].appointlist[e - 1].Wantedbook;
							a[n].appoint_num--;
						}
					}
					Saverole(a);
					Savebook(L);//�޸��鼮��Ϣ�ĵ�����
				}
			}
		}
	}
	else
		cout << "��ĳ��Ŷ��Ѿ���Ϊ0�������޷����飡" << endl;
}

void searchbook(booklist L)//��ѯͼ����Ϣ
{
	int choice;
	cout << "�����Ը������·�ʽ��ѯ��1.����;2.����;3.������;4.���;5.ISBN;6.����" << endl;
	cout << "����������ѡ��";
	cin >> choice;
	if (choice == 1)
		namefind(L);
	else if (choice == 2)
		writerfind(L);
	else if (choice == 3)
		publisherfind(L);
	else if (choice == 4)
		numfind(L);
	else if (choice == 5)
		ISBNfind(L);
	else if (choice == 6)
		sortfind(L);
	else
		cout << "���벻����Ҫ��" << endl;
}

void appointbook(struct role *p, int n, borrowlist &b, booklist L, string bookname)//ԤԼ����
{
	cout << "�����ĸñ����Ѿ�ȫ����������������Ƿ���ҪԤԼ��" << endl;
	cout << "����ԤԼ��һ���ñ������˹黹�����ǽ�������¼ͼ���ʱ����֪ͨ��" << endl;
	cout << "��ҪԤԼ����Y������Ҫ����N : ";
	char answer;
	cin >> answer;
	if (answer == 'Y')
	{
		if (p[n].appoint_num <= 2)
		{
			p[n].appoint_num++;
			p[n].appointlist[p[n].appoint_num - 1].Wantedbook = bookname;
			cout << "����ԤԼ�ɹ���" << endl;
			Saverole(p);
		}
		else
			cout << "��ԤԼͼ����࣬������ԤԼ��" << endl;
	}
}

int findbook(string bookname, booklist L)//�������Ƿ�������Ϊ0
{
	booklist p = L->next;
	while (p)
	{
		if (p->name == bookname)
		{
			if (p->num > 0)
				return 1;
			else
				return 0;
		}
		else
			p = p->next;
	}
	return 0;
}

void checkinbook(struct role *p, int n, borrowlist &b, booklist L, date time)
{
	int num = p[n].appoint_num;//nΪ��ǰԤԼ���������
	int i, j, m;
	char A;
	booklist k = L->next;
	for (i = 1; i <= num; i++)
	{
		m = i;
		while (k != NULL)
		{
			if (k->name == p[n].appointlist[i - 1].Wantedbook)
			{
				if (k->num > 0)
				{
					cout << p[n].appointlist[i - 1].Wantedbook << "���Խ�����" << endl;
					cout << "��Ҫ���ڽ��ĸñ�����Y / N����";
					cin >> A;
					if (A == 'Y')
					{
						if (p[n].honesty > 0)
							//���Ŷȴ���0�ſ��Խ���
						{
							if (p[n].borrow_num < p[n].borrow_max)//������Ŀû�г�������Ȩ��
							{
								k->num--;
								k->times++;
								borrowtimes++;
								p[n].borrow_num++;
								fstream file3("borrowlog.txt", ios::app);//�޸�borrowlog.txt
								file3 << p[n].name << " " << k->name << " " << time.year << " " << time.month << " " << time.day << endl;
								file3.close();
								fstream file4("log.txt", ios::app);//�޸�log.txt
								file4 << p[n].name << " " << k->name << " " << time.year << "/" << time.month << "/" << time.day << "����" << endl;
								file4.close();
								cout << "��" << time.year << "/" << time.month << "/" << time.day << "����ɹ�������20���ڹ黹" << endl;
								for (j = m + 1; j <= p[n].appoint_num; j++, m++)
									p[n].appointlist[m - 1].Wantedbook = p[n].appointlist[j - 1].Wantedbook;
								p[n].appoint_num--;
								Saverole(p);
								Savebook(L);//�޸��鼮��Ϣ�ĵ�����
							}
							else
								cout << "��Ľ�����Ŀ�Ѵﵽ��Ŀɽ���Ȩ�ޣ����ȹ黹ͼ�飡" << endl;
						}
						else
							cout << "��ĳ��Ŷ��Ѿ���Ϊ0�������޷����飡" << endl;
					}
				}
				break;
			}
			k = k->next;
		}
	}
}

void loading(borrowlist &b)//ÿ�ο�ʼ����ʱ�����½������¼�ĵ��е����ݶ����ڴ���
{
	ifstream file("borrowlog.txt");

	while (!file.eof())
	{
		borrow p;
		file >> p.bookname;
		if (file.eof())
			break;
		file >> p.personname;
		Append_borrow(b, p);
	}
	file.close();
}

void Savebook(booklist L)//ˢ���޸ĺ���ĵ�����������
{
	fstream file2("book.txt", ios::out);
	if (file2)
		for (booklist p = L->next; p; p = p->next)
			file2 << p->name << " " << p->writer << " " << p->publisher << " " << p->sort << " " << p->price << " " << p->buytime << " " << p->ISBN << " " << p->number << " " << p->times << " " << p->num << endl;
	file1.close();
}

void Saveborrow(borrowlist b, date time)//���½����¼����
{
	fstream file3("borrowlog.txt", ios::out);
	if (file3)
		for (borrowlist p = b->next; p; p = p->next)
			file3 << p->personname << " " << p->bookname << " " << p->time.year << " " << p->time.month << " " << p->time.day << endl;
	file3.close();
}

void Saverole(struct role *p)
{
	fstream file1("role.txt", ios::out);
	int i;
	int num;
	if (file1)
	{
		for (i = 0; i < peoplenum; i++)
		{
			file1 << p[i].name << " " << p[i].ID << " " << p[i].college << " " << p[i].password << " " << p[i].type << " " << p[i].borrow_max << " " << p[i].borrow_num << " " << p[i].honesty << " " << p[i].fine << " " << p[i].appoint_num;
			if (p[i].appoint_num == 0)
				file1 << endl;
			else
			{
				for (num = 1; num <= p[i].appoint_num; num++)
					file1 << " " << p[i].appointlist[num - 1].Wantedbook;
				file1 << endl;
			}
		}
	}
	file1.close();
}

void returnbook(struct role *p, int n, borrowlist &b, booklist L, date time)
{//����
	borrow m;
	m.personname = p[n].name;
	cout << "��������Ҫ�黹ͼ���������";
	int num, i;
	cin >> num;
	for (i = 0; i < num; i++)
	{
		cout << "������Ҫ�黹��ͼ��������";
		cin >> m.bookname;
		borrowlist k = b->next, temp = k;
		while ((k->bookname != m.bookname || k->personname != m.personname) && k->next)
		{
			temp = k;
			k = k->next;
		}//�ҵ�Ҫɾ�����
		if (k->bookname == m.bookname && k->personname == m.personname)
		{
			if (caldays(k->time, time) > 15)
			{//�ж���û���鼮����
				p[n].honesty = p[n].honesty - 10;//һ��������һ���飬���Ŷȼ�10
				p[n].fine = (caldays(k->time, time) - 15) * 0.5;//�������������������0.5
				cout << "�����黹��" << k->bookname << "�Ѿ�����" << (caldays(k->time, time) - 15) << "�죬�㽫������" << p[n].fine << "ԪΥԼ��ͬʱ���Ŷȼ�10��" << endl;
			}
			else
				p[n].honesty++;//����ʱ�黹��ͼ�飬���Ŷȼ�1
			if (k == b->next)
				b->next = k->next;//��ɾ������ͷ���
			else
			{
				temp->next = k->next;
				delete k;
			}
			p[n].borrow_num--;
			cout << "����ɹ���" << endl;
			fstream file4("log.txt", ios::app);
			file4 << m.personname << " " << m.bookname << " " << time.year << "/" << time.month << "/" << time.day << "����" << endl;
			file4.close();
		}
		for (booklist w = L; w; w = w->next)//�����鼮������Ϣ
		{
			if (w->name == m.bookname)
				w->num++;
		}
		Saverole(p);//����������Ϣ
		Savebook(L);//�����鼮��Ϣ���鼮������һ
		Saveborrow(b, time);//ͬʱɾ�������¼�е���ؽ����¼
	}
}

void addbook(booklist &L)//����Ա�����鼮
{
	book *p = NULL, *q = NULL;
	p = L;
	while (p->next != NULL)
	{
		p = p->next;
	}//ͨ��������ָ��book����ı�β���ڱ�β����ӽڵ�
	q = new book;
	cout << "�����������鼮���������������������硢���͡��۸񡢹���ʱ�䡢ISBN���������м��Կո�����" << endl;
	cin >> q->name >> q->writer >> q->publisher >> q->sort >> q->price >> q->buytime >> q->ISBN >> q->num;
	q->times = 0;
	getbooknumber(q);
	//��ȡ���鼮��Ϣ�󣬽��鼮��Ϣ���뵽�ļ�
	file2.open("book.txt", ios::app);
	file2 << q->name << " " << q->writer << " " << q->publisher << " " << q->sort << " " << q->price << " " << q->buytime << " " << q->ISBN << " " << q->number << " " << q->times << " " << q->num << endl;
	file2.close();
	booknum = booknum + q->num;
	p->next = q;
	p = q;
	p->next = NULL;
}

void Search_book(borrowlist &b, booklist L)
{//����Ա��ѯ�鼮��Ϣ
	int flag = 1;//���ƻص���һ��Ŀ¼
	int choice;
	while (flag)
	{
		cout << "1����ѯ�����鼮������Ϣ\n2����ѯһ���鼮������\n3���鼮������������ѯ\n0��������һ���˵�\n��������Ҫѡ��Ĺ��ܣ�";
		cin >> choice;
		while (choice < 0 || choice>3)
		{
			cout << "���벻�Ϲ淶�����������룡";
			cin >> choice;
		}
		switch (choice)
		{
		case 1:Search_onebook(b, L);
			break;
		case 2: Search_typebook(L);
			break;
		case 3:raterank(L);
			break;
		case 0:flag = 0;
			break;
		default:
			break;
		}
	}
}

void Search_onebook(borrowlist &b, booklist L)
{//��ѯ�����鼮������Ϣ
	string name;
	cout << "������Ҫ���ҵ�������";
	cin >> name;
	borrowlist q = b;
	booklist p = L->next;
	while (q != NULL)
	{
		if (q->bookname == name)
		{
			cout << "������Ϊ��" << q->personname << "  ";
			cout << "����ʱ��Ϊ��" << q->time.year << "/" << q->time.month << "/" << q->time.day << endl;
		}
		q = q->next;
	}
	while (p != NULL)
	{
		if (p->name == name)
		{
			double  m = (double)p->times / (double)borrowtimes;
			cout << "�����鼮������Ϊ��" << m * 100 << "%" << endl;
			break;
		}
		p = p->next;
	}
}

void Search_typebook(booklist L)
{//����Ա��ѯһ���鼮������
	char type;
	double x = 0;
	cout << "������Ҫ���ҵ�������ͣ�";
	cin >> type;
	booklist p = L->next;
	while (p != NULL)
	{
		if (p->sort == type)
			x = x + p->times;
		p = p->next;
	}
	double  m = (double)x / (double)borrowtimes;
	cout << "�����鼮������Ϊ��" << (m * 100) << "%" << endl;
}

void raterank(booklist L)//���ù鲢����ķ�ʽ�Խ����ʴӸߵ��ͽ�������
{
	int n;
	int times[book_max + 1] = { 0 };
	string name[book_max + 1];
	for (int k = 0; k <= book_max; k++)
		name[k] = " ";
	booklist p = L->next;
	int i, j;
	for (i = 1; p; p = p->next, i++)
	{
		times[i] = p->times;
		name[i] = p->name;
	}
	i--;
	MSort(times, times, name, name, 1, i);
	cout << "�����������ѯ����������ǰ���������飺";
	cin >> n;
	for (j = 1; j <= n; j++)
		cout << "����Ϊ��" << name[j] << "  " << "���Ĵ���Ϊ��" << times[j] << "  ������Ϊ��" << (double)((double)times[j] / (double)borrowtimes) << endl;
	system("pause");
	system("cls");
}

void MSort(int *sr, int *tr1, string *mr, string *mr1, int s, int t)
{//��sr[s..t]�鲢����Ϊtr[s..t]
	int m;
	int tr2[book_max + 1] = { 0 };
	string mr2[book_max + 1];
	for (int k = 0; k <= book_max; k++)
		mr2[k] = " ";
	if (s == t)
	{
		tr1[s] = sr[s];
		mr1[s] = mr[s];
	}
	else
	{
		m = (s + t) / 2;  //��sr[s...t]ƽ��Ϊsr[s..m]��sr[m+1..t]
		MSort(sr, tr2, mr, mr2, s, m);   //�ݹ�ؽ�sr[s..m]�鲢Ϊ�����tr2[s..m]
		MSort(sr, tr2, mr, mr2, m + 1, t);   //�ݹ�ؽ�sr[m+1..t]�鲢Ϊ�����tr2[m+1..t]
		Merge(tr2, tr1, mr2, mr1, s, m, t);  //��tr2[s..m]��tr2[m+1..t]�鲢��tr1[s..t]
	}
}//MSort

void Merge(int *sr, int *tr, string *mr, string *nr, int i, int m, int n)
{//�������sr[i..m]��sr[m+1..n]�鲢Ϊtr[i..n]
	int j, k, temp;
	for (j = m + 1, k = i; i <= m &&j <= n; ++k)
	{//��sr�м�¼��С����ز���tr��
		if (sr[i] > sr[j])
		{
			tr[k] = sr[i];
			nr[k] = mr[i];
			i++;
		}
		else
		{
			tr[k] = sr[j];
			nr[k] = mr[j];
			j++;
		}
	}
	if (i <= m)
	{//��ʣ���sr[i..m]���Ƶ�tr
		for (temp = k; temp <= n && i <= m; temp++, i++)
		{
			tr[temp] = sr[i];
			nr[temp] = mr[i];
		}
	}
	if (j <= n)
	{//��ʣ���sr[j..n]���Ƶ�tr
		for (temp = k; temp <= n && j <= n; temp++, j++)
		{
			tr[temp] = sr[j];
			nr[temp] = mr[j];
		}
	}
}//Merge

void Search_person(struct role *p, borrowlist &b)
{//����Ա��ѯ��Ա��Ϣ
	string name;
	int i;
	cout << "������Ҫ���ҵ�������";
	cin >> name;
	borrowlist q = b;
	for (i = 0; i < peoplenum; i++)
	{
		if (p[i].name == name)
		{
			cout << "ѧԺ��" << p[i].college << endl;
			cout << "�Ѿ�����������" << p[i].borrow_num << "	" << "�пɽ���������" << p[i].borrow_max - p[i].borrow_num << endl;
			cout << "��ǰ���Ŷȣ�" << p[i].honesty << "  " << "��ǰ��Ƿ����" << p[i].fine << endl;
			break;
		}
	}
	while (q != NULL)
	{

		if (q->personname == name&&p[i].borrow_num != 0)
		{
			cout << "����ͼ��Ϊ��" << q->bookname << endl;
			cout << "����ʱ��Ϊ��" << q->time.year << "/" << q->time.month << "/" << q->time.day << endl;
			q = q->next;
		}
		else
			q = q->next;
	}
}

int isLeap(int year) //�ж��Ƿ�������ĺ��� 
{
	if (year % 4 == 0 && year % 100 != 0 || year % 400 == 0)
		return 1;//�������򷵻�1 
	else
		return 0;//�������귵��0 
}

void Savedate(date T)//ˢ���޸ĺ��ʱ���ĵ�����������
{
	fstream file5("configuration.txt", ios::out);
	if (file5)
		file5 << T.year << " " << T.month << " " << T.day << " " << borrowtimes << endl;
	file5.close();
}

int GetMonthDays(int year, int month)//�õ�ÿ���ж�����ĺ��� 
{
	if (isLeap(year) == 1)//�������� 
	{
		switch (month)
		{
		case 1:
			return 31;
			break;
		case 2:
			return 29;
			break;
		case 3:
			return 31;
			break;
		case 4:
			return 30;
			break;
		case 5:
			return 31;
			break;
		case 6:
			return 30;
			break;
		case 7:
			return 31;
			break;
		case 8:
			return 31;
			break;
		case 9:
			return 30;
			break;
		case 10:
			return 31;
			break;
		case 11:
			return 30;
			break;
		case 12:
			return 31;
			break;
		}
	}
}

date Add_One_Day(date date)//���ݾ�������������ڵ�����
{
	if (date.day + 1 <= GetMonthDays(date.year, date.month))
		date.day++;//�����������ĺ�������ÿ���¶�Ӧ������
	else
	{
		date.month++;
		date.day = 1;
	}
	if (date.month > 12)
	{
		date.year++;
		date.month = 1;
	}
	Savedate(date);//���޸ĵ����ڽ��б���
	return date;
}

void checkoverdue(struct role *a, int n, borrowlist &b, booklist L, date time)
{//�����ĵ��鼮��û���������������15��Ϊ����
	borrow *p = b->next;
	int i = a[n].borrow_num, j;
	for (j = 0; j < i; j++)
	{
		while (p != NULL)
		{
			if (p->personname == a[n].name)
			{
				if (caldays(p->time, time) > 15)
					cout << "����ĵ�" << p->bookname << "�Ѿ����ڣ��뾡��黹��" << endl;
			}
			p = p->next;
		}
	}
}

int caldays(date t1, date t2)
{
	//����t2��t1ǰ���˶�����
	int i, j, m, n, x;
	if (t1.year == t2.year&&t1.month == t2.month)
		return t2.day - t1.day;
	else if (t1.year == t2.year)
	{
		i = t1.month;
		j = t2.month;
		x = GetMonthDays(t1.year, i) - t1.day;
		i++;
		for (; i < j; i++)
		{
			x = x + GetMonthDays(t1.year, i);
		}
		x = x + t2.day;
		return x;
	}
	else
	{
		i = t1.month;
		j = t2.month;
		m = t1.year;
		n = t2.year;
		x = GetMonthDays(m, i) - t1.day;
		i++;
		for (; i <= 12; i++)
		{
			x = x + GetMonthDays(m, i);
		}
		m++;
		for (; m < n; m++)
		{
			i = 1;
			for (; i <= 12; i++)
			{
				x = x + GetMonthDays(m, i);
			}
		}
		for (i = 1; i < j; i++)
		{
			x = x + GetMonthDays(n, i);
		}
		x = x + t2.day;
		return x;
	}
}

void get_next(int *next, char *a, int la) //��next[]��ֵ
{
	int i = 1, j = 0;
	next[1] = 0;

	while (i <= la)
	{
		if (a[i] == a[j] || j == 0)
		{
			j++;
			i++;
			if (a[i] == a[j])
				next[i] = next[j];
			else
				next[i] = j;
		}
		else
			j = next[j];
	}
}

int str_kmp(int *next, char *A, char *a, int lA, int la)
{
	int i, j, k;
	i = 1;
	j = 1;
	while (i <= lA && j <= la)
	{
		if (A[i] == a[j] || j == 0)
		{
			i++;
			j++;
		}
		else
			j = next[j];
	}

	if (j> la)
		return i - j + 1;
	else
		return -1;
}


int judge(string a, string b)
{
	int next[100] = { 0 };
	int k;
	char a_[256];
	strcpy_s(a_, a.c_str());//ת�����ͣ���stringת�����ַ�����
	char b_[256];
	strcpy_s(b_, b.c_str());
	int la = 0, lb = 0;
	la = strlen(a_);
	lb = strlen(b_);
	for (k = la - 1; k >= 0; k--)
		a[k + 1] = a[k];
	for (k = lb - 1; k >= 0; k--)
		b[k + 1] = b[k];
	get_next(next, b_, lb);//�õ�next����
	k = str_kmp(next, a_, b_, la, lb);
	if (k == -1)
		return 0;
	else
		return 1;

}
