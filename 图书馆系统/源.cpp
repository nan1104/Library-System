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
#define people_max 50    //人数上限
#define LENGTH_PER_DAY 10    //系统时间10秒相当于图书馆时间增加一天
#define book_max 100  //书籍数目上限
static int peoplenum = 0;   //总人数
static int booknum = 0;  //总的书籍总数
static int liushuinum = 0; //用于书籍编码的流水号
static int borrowtimes = 0;//所有书被借阅的次数
using namespace std;
//书籍信息 
typedef struct book {
	string name;  //书名
	string writer;  //作者名
	char publisher;  //出版社
	char sort;  //类型
	float price;   //价格
	string buytime;  //购买时间
	string ISBN;//该种书ISBN
	string number;//该书编码
	int times;//该类书被借阅次数
	int num;//该类书当前剩余数
	struct book *next; //链表指针
}book, *booklist;
//预约信息
struct appointment {
	string Wantedbook;
	string Time;
};
//人物信息
struct role {
	string name;
	string ID;  //学生证号或工作号
	char college;
	string password;
	char type;//A代表教师，B代表学生，C代表管理员
	int borrow_max;//权限，最大借书数量，教师6学生3
	int borrow_num;//已经借书的
	int honesty;//借书诚信情况
	int fine;//借书逾期未归还需缴纳的罚金
	int appoint_num;//已经预约的书的数量
	struct appointment appointlist[3];//预约结构体数组，每人最多预约三本书
};;
//借书信息
typedef struct time
{
	int year;
	int month;
	int day;
}date;

typedef struct borrow {
	string personname;//借书人姓名
	string bookname;//图书姓名
	date time;//借书时间
	struct borrow *next;
}borrow, *borrowlist;

fstream file1;//人物文件
fstream file2;//书籍文件
fstream file3;//借还信息文件borrowlog.txt
fstream file4;//日志信息log.txt
fstream file5;//日期信息和借阅次数信息
void createrole(struct role *p, borrowlist &b, booklist L, date time); //个人注册
int login(struct role *p, booklist L, borrowlist &b, date time); //个人登陆
void showpeopleinfo(struct role * p, int n, borrowlist &b);//展示个人信息 
void openroletxt(struct role *p); //打开人物文件
int getIDnumber(struct role *p);//对学号工作号进行编码
booklist openbooktext(); //打开书籍文件
borrowlist openborrowtext();//打开借书日志
void addbook(booklist &L);//往图书馆中增加图书
void getbooknumber(book *m);//图书编码函数
int power(int x, int y);//求x的y次方
void teacher(struct role *p, booklist L, borrowlist &b, date time);//登陆时进入教师界面
void student(struct role *p, booklist L, borrowlist &b, date time);//登录时进入学生界面
void manager(struct role *p, booklist L, borrowlist &b, date time);//登录时进入管理员界面
string upper(string &str);//转换大小写
int find_str(string str, string substr);//字符串匹配
int  findbook(string a, string S);
void writerfind(booklist L);//按照作者查找
void publisherfind(booklist L);//按照出版社查找图书
void namefind(booklist L);//按照书名查找图书
void numfind(booklist L);//按照书的编号查询
void ISBNfind(booklist L);//按照书的编号查询
void sortfind(booklist L);//按照分类查询图书
void  Initiate_borrow(borrowlist  &a);//初始化链表
int  Append_borrow(borrowlist &a, borrow b);//插入创建借书链表结点
booklist find_name(booklist &a, string m);//通过书名查找
void borrowbook(struct role *p, int n, borrowlist &b, booklist L, date time);//借书，并输入到借书日志
void searchbook(booklist head);//查询图书信息
void appointbook(struct role *p, int n, borrowlist &b, booklist L, string bookname);//预约借书
void checkinbook(struct role *p, int n, borrowlist &b, booklist L, date time);//检查预约图书是否可以借阅
int findbook(string bookname, booklist L);//查找书是否数量不为0
void loading(borrowlist &b);//每次开始运行时都重新将借书记录文档中的内容读到内存中
void Savebook(booklist a);//刷新修改后的文档并更新内容
void Saveborrow(borrowlist b, date time);//更新借书记录内容
void returnbook(struct role *p, int n, borrowlist &b, booklist L, date time);//还书并刷新文档
void Saverole(struct role *p);//更新role.txt
void Search_book(borrowlist &b, booklist L);//管理员查找书籍信息
void Search_person(struct role *p, borrowlist &b);//管理员查找人员信息
void Search_onebook(borrowlist &b, booklist L);//管理员查找单本书籍信息
void Search_typebook(booklist L);//管理员查询一类书籍借阅率
date Add_One_Day(date date);//根据具体情况进行日期的推移
int GetMonthDays(int year, int month);//得到每月有多少天的函数 
void Savedate(date Time);//刷新修改后的时间文档并更新内容
int isLeap(int year);//判断是否是闰年的函数 
void checkoverdue(struct role *a, int n, borrowlist &b, booklist L, date time);//判断有没有书籍逾期未归还情况
int caldays(date t1, date t2);//计算t2比t1前进了多少天
void Merge(int *sr, int *tr, string *mr, string *nr, int i, int m, int n);//归并排序
void MSort(int *sr, int *tr1, string *mr, string *mr1, int s, int t);//归并排序
void raterank(booklist L);//借阅率排序
void get_next(int *next, char *a, int la); //求next[]的值
int str_kmp(int *next, char *A, char *a, int lA, int la);
int judge(string a, string b);

void main_menu(struct role *people, booklist Head, borrowlist Borrow, date time)//主菜单界面
{
	int i;
	cout << "请问你是要注册还是登陆？注册选择1，登陆选择2,退出选择0" << endl;
	cin >> i;
	while (i < 0 || i>2)
	{
		cout << "你的输入有误，请重新输入！";
		cout << "请问你是要注册还是登陆？注册选择1，登陆选择2,退出选择0" << endl;
		cin >> i;
	}
	if (i >= 0 && i <= 2)
	{
		switch (i)
		{
		case 1:
			createrole(people, Borrow, Head, time);//注册
			break;
		case 2:
			login(people, Head, Borrow, time);//进入登陆界面
			break;
		case 0:
			i = 0;
			break;
		default:
			cout << "输入有误！" << endl;
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
		file5 >> date.year >> date.month >> date.day >> borrowtimes;//从配置文件中读取时间并存入结构体
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
		cout << date.year << " " << date.month << " " << date.day;//输出当前日期
		cout << "欢迎进入图书馆!" << endl;
		start = clock();//开始计时
		for (;;)
		{

			finish = clock();
			duration = (double)(finish - start) / CLOCKS_PER_SEC;//计算计时间隔的秒数
			if (_kbhit() != 0)//如果键盘有输入
			{
				PauseTime = PauseTime + duration;//pausetime表示暂停之前的时间总和
				_getch();
				main_menu(people, Head, Borrow, date);//进入具体操作部分
				cout << date.year << " " << date.month << " " << date.day;//退出时候打印出时间，并根据系统时间进行相应的时间推进
				cout << "欢迎进入图书馆!" << endl;
				start = clock();
				duration = 0;
			}
			if ((duration + PauseTime) > LENGTH_PER_DAY)//如果计时间隔大于图书馆系统的一天，即10秒
			{
				int t = (duration + PauseTime) / LENGTH_PER_DAY;
				date = Add_One_Day(date);//时间往后推移一天
				system("cls");
				cout << date.year << " " << date.month << " " << date.day;
				cout << "欢迎进入图书馆!" << endl;
				start = clock();
				PauseTime = 0;

			}
		}
	}
	return 0;
}

void openroletxt(struct role *p)//打开人物信息文件，并存入人物信息数组中
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

booklist openbooktext()//打开书籍信息文件，并将书籍信息存入链表中
{
	book* L = new book; book *r;
	L->next = NULL;//链表头结点
	r = L; //r始终指向链表的表尾
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

borrowlist openborrowtext()//将借书信息存入借阅链表的结构体对应的变量中
{
	borrowlist b = new borrow; borrow *r;
	b->next = NULL;//链表头结点
	r = b; //r始终指向链表的表尾
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

void createrole(struct role *p, borrowlist &b, booklist L, date time)//注册新的人物
{
	string key;
	cout << "请输入你的身份为（老师输入A，学生输入B，管理员输入C）：";
	cin >> p[peoplenum].type;
	if (p[peoplenum].type == 'C')
	{
		cout << "请输入网关密码：";//网关密码为111111
		//只有网关密码正确才能注册管理员，确保实际应用时的安全性
		cin >> key;
		if (key != "111111")
		{
			cout << "你输入的网关密码有误,无法注册管理员！\n";
			system("pause");
			system("cls"); //清屏函数
			main_menu(p, L, b, time);
			exit(0);
		}
		else
			cout << "网关密码正确！\n";
	}
	file1.open("role.txt", ios::app);//将注册的人物信息写入人物文件中
	cout << "请输入你的姓名为：";
	cin >> p[peoplenum].name;
	cout << "请输入你的学院：";
	cin >> p[peoplenum].college;
	cout << "请输入你的密码：";
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
	cout << "你的学号/工作号为" << getIDnumber(p);
	file1 << p[peoplenum].name << " " << p[peoplenum].ID << " " << p[peoplenum].college << " " << p[peoplenum].password << " " << p[peoplenum].type << " " << p[peoplenum].borrow_max << " " << p[peoplenum].borrow_num << " " << p[peoplenum].honesty << " " << p[peoplenum].fine << " " << p[peoplenum].appoint_num << endl;
	file1.close();
	peoplenum++;
	system("pause");
	system("cls"); //清屏函数
	main_menu(p, L, b, time);
}

int getIDnumber(struct role *p)
{//读者的编码可以采用如下格式：X（类型）XX（学院编码）XXXX（流水号）
	std::strstream ss; // #include <strstream>  
	long long value = ((p[peoplenum].type - 'A' + 1)*power(10, 6) + (p[peoplenum].college - 'A' + 1)*power(10, 4) + peoplenum);
	ss << value;
	ss >> p[peoplenum].ID;
	return  value;
}

int login(struct role *p, booklist L, borrowlist &b, date time)//登录界面
{
	char type;
	cout << "请输入你的身份（教师输入A，学生输入B，管理员输入C）：";
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

void teacher(struct role *p, booklist L, borrowlist &b, date time)//教师登录界面
{
	int func;
	string id, key;
	int n = 0;
	int flag = 1;//控制返回上一级菜单
	cout << "欢迎进入教师图书馆界面！" << endl;
	cout << "请输入你的工作号:";
	cin >> id;
	while (n < peoplenum)
	{
		if (id.compare(p[n].ID) == 0 && p[n].type == 'A')
		{
			cout << "请输入密码：";
			cin >> key;
			while (key.compare(p[n].password) != 0)
			{
				cout << "你输入的密码错误，请重新输入！" << endl;
				cin >> key;
			}
			if (key.compare(p[n].password) == 0)
			{
				cout << p[n].name << ",你已登陆成功，欢迎进入图书馆！" << endl;
				checkoverdue(p, n, b, L, time);
				checkinbook(p, n, b, L, time);
				while (flag != 0)
				{
					cout << "查询个人信息输入1，图书查询输入2，借阅率查询输入3，借书输入4，还书输入5，退出图书馆输入0" << endl;
					cout << "请输入你要选择的功能：";
					cin >> func;
					while (func > 5 || func < 0)
					{
						cout << "输入数字有误！请重新输入：";
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
		cout << "图书馆中不存在该工作号，请重新输入！" << endl;
		login(p, L, b, time);
	}
	if (flag == 0)
	{
		system("cls");
		main_menu(p, L, b, time);//重新回到主菜单
	}
}

void student(struct role *p, booklist L, borrowlist &b, date time)//学生登录界面
{
	int func;
	string id, key;
	int n = 0;
	int flag = 1;//控制返回上一级菜单
	cout << "欢迎进入学生图书馆界面！" << endl;
	cout << "请输入你的工作号:";
	cin >> id;
	while (n < peoplenum)
	{
		if (id.compare(p[n].ID) == 0 && p[n].type == 'B')
		{
			cout << "请输入密码：";
			cin >> key;
			while (key.compare(p[n].password) != 0)
			{
				cout << "你输入的密码错误，请重新输入！" << endl;
				cin >> key;
			}
			if (key.compare(p[n].password) == 0)
			{
				cout << p[n].name << ",你已登陆成功，欢迎进入图书馆！" << endl;
				checkoverdue(p, n, b, L, time);
				checkinbook(p, n, b, L, time);
				while (flag != 0)
				{
					cout << "查询个人信息输入1，图书查询输入2，借阅率查询输入3，借书输入4，还书输入5，退出图书馆输入0" << endl;
					cout << "请输入你要选择的功能：";
					cin >> func;
					while (func > 5 || func < 0)
					{
						cout << "输入数字有误！请重新输入：";
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
		cout << "图书馆中不存在该工作号，请重新输入！" << endl;
		login(p, L, b, time);
	}
	if (flag == 0)
	{
		system("cls");
		main_menu(p, L, b, time);
	}
}

void manager(struct role *p, booklist L, borrowlist &b, date time)//管理员登录界面
{
	string id, key;
	int n, choice;
	int flag = 1;//控制回到上一级目录
	int state = 1;
	cout << "欢迎进入管理员图书馆界面！" << endl;
	cout << "请输入你的工作号:";
	cin >> id;
	for (n = 0; n < peoplenum && state; n++)
	{
		if (id.compare(p[n].ID) == 0 && p[n].type == 'C')
		{
			cout << "请输入密码：";
			cin >> key;
			while (key.compare(p[n].password) != 0)
			{
				cout << "你输入的密码错误，请重新输入！" << endl;
				cin >> key;
			}
			if (key.compare(p[n].password) == 0)
			{
				cout << p[n].name << ",你已登陆成功，欢迎进入图书馆！" << endl;
				checkoverdue(p, n, b, L, time);
				checkinbook(p, n, b, L, time);
				while (flag != 0)
				{
					cout << "1、增加图书\n2、查询书籍信息\n3、查询人员信息\n0、退出图书馆\n请输入你要选择的功能：";
					cin >> choice;
					while (choice < 0 || choice>3)
					{
						cout << "输入不合规范，请重新输入！";
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
		cout << "图书馆中不存在该工作号，请重新输入！" << endl;
		login(p, L, b, time);
	}

	if (flag == 0)
	{
		system("cls");
		main_menu(p, L, b, time);
	}
}

void showpeopleinfo(struct role *p, int n, borrowlist &b)//打印人物信息
{
	cout << "姓名：" << p[n].name << "	" << "学院：" << p[n].college << endl;
	cout << "当前已经借书数量：" << p[n].borrow_num << "	" << "当前尚可借书数量：" << p[n].borrow_max - p[n].borrow_num << endl;
	cout << "当前诚信度：" << p[n].honesty << "  " << "当前所欠罚金：" << p[n].fine << endl;
	cout << "当前已经借阅的书及借书日期：" << endl;
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
//对录入图书进行编码，编码规则为 XX（类型）XX（出版社）XXXX（流水号）
{
	std::strstream ss; // #include <strstream>  
	long long value = ((m->sort - 'A' + 1)*power(10, 6) + (m->publisher - 'A' + 1)*power(10, 4) + liushuinum);
	ss << value;
	ss >> m->number;
	liushuinum++;
}

int power(int x, int y)//求x的y次方，编码部分
{
	int num = 1, n = 0;
	while (n < y)
	{
		num = num*x;
		n++;
	}
	return num;
}

void writerfind(booklist L)//学生按照作者查询图书
{
	int flag = 0;//是否查找到当前书籍的标志
	char answer;
	string writer;
	cout << "请输入要查找书籍的作者：";
	cin >> writer;
	booklist p = L;
	int m = 1;
	while (p)
	{
		if (writer == p->writer)
		{
			if (m == 1)
			{
				cout << "书名\t作者\t出版商\t总数量\t借阅率" << endl;
				cout << p->name << " " << p->writer << " " << p->publisher << " " << p->num << " " << (double)(p->times / borrowtimes) << endl;
				m++;
			}
			else
				cout << p->name << " " << p->writer << " " << p->publisher << " " << p->num << " " << (double)(p->times / borrowtimes) << endl;
			flag++;
		}
		p = p->next;
	}
	cout << "是否有您要找的书籍？Y/N" << endl;
	cin >> answer;
	if (answer == 'Y')
	{
		namefind(L);
	}
	else if (answer == 'N')
		cout << "对不起，没有找到您需要的书籍！" << endl;
	if (flag == 0)
		cout << "对不起，没有找到您需要的书籍！" << endl;
}

void publisherfind(booklist L)//按照出版社查找图书
{
	int flag = 0;//是否查找到当前书籍的标志
	char answer;
	char publisher;
	cout << "请输入要查找的出版社：";
	cin >> publisher;
	booklist p = L;
	int m = 1;
	while (p)
	{
		if (publisher == p->publisher)
		{
			if (m == 1)
			{
				cout << "书名\t作者\t出版商\t总数量\t借阅率" << endl;
				cout << p->name << " " << p->writer << " " << p->publisher << " " << p->num << " " << (double)(p->times / borrowtimes) << endl;
				m++;
			}
			else
				cout << p->name << "　" << p->writer << " " << p->publisher << " " << p->num << " " << (double)(p->times / borrowtimes) << endl;
			flag++;
		}
		p = p->next;
	}
	cout << "是否有您要找的书籍？Y/N" << endl;
	cin >> answer;
	if (answer == 'Y')
	{
		namefind(L);
	}
	else if (answer == 'N')
		cout << "对不起，没有找到您需要的书籍！" << endl;

	if (flag == 0)
		cout << "对不起，没有找到您需要的书籍！" << endl;
}

string upper(string &str)//转换大小写
{
	transform(str.begin(), str.end(), str.begin(), ::toupper);
	return str;
}

int find_str(string str, string substr)//字符串匹配
{
	return upper(str).find(upper(substr));
}

int  findbook(string a, string S)//匹配字符串
{
	if (find_str(a, S) != -1)
		return 1;
	else
		return 0;
}

void namefind(booklist L)//按照书名查找图书
{
	int flag = 0;//是否查找到当前书籍的标志
	char answer;//是否选择借书时的选择变量
	string name;
	cout << "请输入要查找的书名：";
	cin >> name;
	book *head = L;
	while (head)
	{
		if (judge(head->name, name) != 0)
		{
			cout << "书名\t作者\t出版商\t总数量\t借阅率" << endl;
			double  m = (double)head->times / (double)borrowtimes;//借阅率
			cout << head->name << "  " << head->writer << "  " << head->publisher << "  " << head->num << "  " << m * 100 << "%" << endl;
			flag++;
			break;
		}
		else
			head = head->next;
	}
	if (flag == 0)
		cout << "对不起，没有找到您需要的书籍！" << endl;
}

void numfind(booklist L)//按照书的编号查询图书
{
	int flag = 0;//是否查找到当前书籍的标志
	string num;
	cout << "请输入要查找书籍的编号：";
	cin >> num;
	booklist p = L;
	int m = 1;
	while (p)
	{
		if (num == p->number)
		{
			if (m == 1)
			{
				cout << "书名\t作者\t出版商\t总数量\t借阅率" << endl;
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
		cout << "对不起，没有找到您需要的书籍！" << endl;
}

void ISBNfind(booklist L)//按照书的编号查询
{
	int flag = 0;//是否查找到当前书籍的标志
	string isbn;
	cout << "请输入要查找书籍的ISBN号码：";
	cin >> isbn;
	booklist p = L;
	int m = 1;
	while (p)
	{
		if (isbn == p->ISBN)
		{
			if (m == 1)
			{
				cout << "书名\t作者\t出版商\t总数量\t借阅率" << endl;
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
		cout << "对不起，没有找到您需要的书籍！" << endl;
}

void sortfind(booklist L)//按照分类查询图书
{
	int flag = 0;//是否查找到当前书籍的标志
	char ch;
	char answer;
	booklist p = L;
	cout << "请输入要查找书籍的分类：";
	cin >> ch;
	int m = 1;
	while (p)
	{
		if (p->sort == ch)
		{
			if (m == 1)
			{
				cout << "书名\t作者\t出版商\t总数量\t借阅率" << endl;
				cout << p->name << "  " << p->writer << "  " << p->publisher << "  " << p->num << "  " << (double)((double)p->times / (double)borrowtimes) << endl;
				m++;
			}
			else
				cout << p->name << "  " << p->writer << "  " << p->publisher << "  " << p->num << "  " << (double)((double)p->times / (double)borrowtimes) << endl;
			flag++;
		}
		p = p->next;
	}
	cout << "是否有您要找的书籍？Y/N";
	cin >> answer;
	if (answer == 'Y')
	{
		namefind(L);
	}
	else if (answer == 'N')
		cout << "对不起，没有找到您需要的书籍！" << endl;

	if (flag == 0)
		cout << "对不起，没有找到您需要的书籍！" << endl;
}

void  Initiate_borrow(borrowlist  &a)//初始化借书链表
{
	a = NULL;
}

int  Append_borrow(borrowlist &a, borrow b) //借书链表，插入创建结点
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

booklist find_name(booklist &a, string m)//按照书名查找对应书籍
{
	booklist p = a;
	for (; p; p = p->next)
		if (m == p->name)
			return p;
}

void borrowbook(struct role *a, int n, borrowlist &b, booklist L, date time) //借书，并输入到借书日志
{
	fstream file1("book.txt");
	borrow m;
	booklist q = NULL;
	int c, d, e;//检查预约借书数组
	m.personname = a[n].name;
	if (a[n].honesty > 0)
		//诚信度大于0才可以借书
	{
		int num;
		cout << "请输入借书的数目：";
		cin >> num;
		getchar();
		while (num > (a[n].borrow_max - a[n].borrow_num))
		{
			cout << "你当前最多只能借" << (a[n].borrow_max - a[n].borrow_num) << "本书！" << endl;
			cout << "请输入借书的数目：";
			cin >> num;
			getchar();
		}
		cout << "请输入你要借的书的名字,用换行区分：" << endl;
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
					cout << "书名  作者  出版商  剩余数量" << endl;
					cout << p->name << "  " << p->writer << "  " << p->publisher << "  " << p->num << endl;
					flag++;
				}
				p = p->next;
			}
			if (flag == 0)
				cout << "对不起，没有找到您需要的书籍！" << endl;
			if (flag != 0)
			{
				q = find_name(L, m.bookname);
				if (q->num == 0)
					appointbook(a, n, b, L, m.bookname); //图书已被借完，调用预约函数
				else
				{
					m.time.year = time.year;
					m.time.month = time.month;
					m.time.day = time.day;//将借书时间存入借书结构体中
					Append_borrow(b, m);
					q->num--;
					q->times++;
					borrowtimes++;//整个图书馆的总借书次数增加
					Savedate(time);
					a[n].borrow_num++;
					fstream file3("borrowlog.txt", ios::app);
					file3 << m.personname << " " << m.bookname << " " << time.year << " " << time.month << " " << time.day << endl;
					file3.close();
					fstream file4("log.txt", ios::app);
					file4 << m.personname << " " << m.bookname << " " << time.year << "/" << time.month << "/" << time.day << "借书" << endl;
					file4.close();
					cout << "于" << time.year << "/" << time.month << "/" << time.day << "借书成功！请于20日内归还" << endl;
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
					Savebook(L);//修改书籍信息文档内容
				}
			}
		}
	}
	else
		cout << "你的诚信度已经降为0，你已无法借书！" << endl;
}

void searchbook(booklist L)//查询图书信息
{
	int choice;
	cout << "您可以根据以下方式查询：1.书名;2.作者;3.出版商;4.编号;5.ISBN;6.分类" << endl;
	cout << "请输入您的选择：";
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
		cout << "输入不符合要求" << endl;
}

void appointbook(struct role *p, int n, borrowlist &b, booklist L, string bookname)//预约借书
{
	cout << "您想借的该本书已经全部被借出，请问您是否需要预约？" << endl;
	cout << "当您预约后，一旦该本书有人归还，我们将在您登录图书馆时立即通知您" << endl;
	cout << "需要预约输入Y，不需要输入N : ";
	char answer;
	cin >> answer;
	if (answer == 'Y')
	{
		if (p[n].appoint_num <= 2)
		{
			p[n].appoint_num++;
			p[n].appointlist[p[n].appoint_num - 1].Wantedbook = bookname;
			cout << "您已预约成功！" << endl;
			Saverole(p);
		}
		else
			cout << "您预约图书过多，不可再预约！" << endl;
	}
}

int findbook(string bookname, booklist L)//查找书是否数量不为0
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
	int num = p[n].appoint_num;//n为当前预约借书的数量
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
					cout << p[n].appointlist[i - 1].Wantedbook << "可以借阅了" << endl;
					cout << "您要现在借阅该本书吗（Y / N）：";
					cin >> A;
					if (A == 'Y')
					{
						if (p[n].honesty > 0)
							//诚信度大于0才可以借书
						{
							if (p[n].borrow_num < p[n].borrow_max)//借书数目没有超过借书权限
							{
								k->num--;
								k->times++;
								borrowtimes++;
								p[n].borrow_num++;
								fstream file3("borrowlog.txt", ios::app);//修改borrowlog.txt
								file3 << p[n].name << " " << k->name << " " << time.year << " " << time.month << " " << time.day << endl;
								file3.close();
								fstream file4("log.txt", ios::app);//修改log.txt
								file4 << p[n].name << " " << k->name << " " << time.year << "/" << time.month << "/" << time.day << "借书" << endl;
								file4.close();
								cout << "于" << time.year << "/" << time.month << "/" << time.day << "借书成功！请于20日内归还" << endl;
								for (j = m + 1; j <= p[n].appoint_num; j++, m++)
									p[n].appointlist[m - 1].Wantedbook = p[n].appointlist[j - 1].Wantedbook;
								p[n].appoint_num--;
								Saverole(p);
								Savebook(L);//修改书籍信息文档内容
							}
							else
								cout << "你的借书数目已达到你的可借书权限，请先归还图书！" << endl;
						}
						else
							cout << "你的诚信度已经降为0，你已无法借书！" << endl;
					}
				}
				break;
			}
			k = k->next;
		}
	}
}

void loading(borrowlist &b)//每次开始运行时都重新将借书记录文档中的内容读到内存中
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

void Savebook(booklist L)//刷新修改后的文档并更新内容
{
	fstream file2("book.txt", ios::out);
	if (file2)
		for (booklist p = L->next; p; p = p->next)
			file2 << p->name << " " << p->writer << " " << p->publisher << " " << p->sort << " " << p->price << " " << p->buytime << " " << p->ISBN << " " << p->number << " " << p->times << " " << p->num << endl;
	file1.close();
}

void Saveborrow(borrowlist b, date time)//更新借书记录内容
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
{//还书
	borrow m;
	m.personname = p[n].name;
	cout << "请输入你要归还图书的数量：";
	int num, i;
	cin >> num;
	for (i = 0; i < num; i++)
	{
		cout << "请输入要归还的图书书名：";
		cin >> m.bookname;
		borrowlist k = b->next, temp = k;
		while ((k->bookname != m.bookname || k->personname != m.personname) && k->next)
		{
			temp = k;
			k = k->next;
		}//找到要删除结点
		if (k->bookname == m.bookname && k->personname == m.personname)
		{
			if (caldays(k->time, time) > 15)
			{//判断有没有书籍逾期
				p[n].honesty = p[n].honesty - 10;//一旦逾期了一本书，诚信度减10
				p[n].fine = (caldays(k->time, time) - 15) * 0.5;//罚金等于逾期天数乘以0.5
				cout << "你所归还的" << k->bookname << "已经逾期" << (caldays(k->time, time) - 15) << "天，你将被扣以" << p[n].fine << "元违约金，同时诚信度减10！" << endl;
			}
			else
				p[n].honesty++;//若按时归还了图书，诚信度加1
			if (k == b->next)
				b->next = k->next;//若删除的是头结点
			else
			{
				temp->next = k->next;
				delete k;
			}
			p[n].borrow_num--;
			cout << "还书成功！" << endl;
			fstream file4("log.txt", ios::app);
			file4 << m.personname << " " << m.bookname << " " << time.year << "/" << time.month << "/" << time.day << "还书" << endl;
			file4.close();
		}
		for (booklist w = L; w; w = w->next)//更新书籍链表信息
		{
			if (w->name == m.bookname)
				w->num++;
		}
		Saverole(p);//更新人物信息
		Savebook(L);//更新书籍信息中书籍数量加一
		Saveborrow(b, time);//同时删掉借书记录中的相关借书记录
	}
}

void addbook(booklist &L)//管理员增加书籍
{
	book *p = NULL, *q = NULL;
	p = L;
	while (p->next != NULL)
	{
		p = p->next;
	}//通过遍历，指向book链表的表尾，在表尾处添加节点
	q = new book;
	cout << "请依次输入书籍的书名、作者名、出版社、类型、价格、购买时间、ISBN、数量，中间以空格间隔：" << endl;
	cin >> q->name >> q->writer >> q->publisher >> q->sort >> q->price >> q->buytime >> q->ISBN >> q->num;
	q->times = 0;
	getbooknumber(q);
	//读取完书籍信息后，将书籍信息读入到文件
	file2.open("book.txt", ios::app);
	file2 << q->name << " " << q->writer << " " << q->publisher << " " << q->sort << " " << q->price << " " << q->buytime << " " << q->ISBN << " " << q->number << " " << q->times << " " << q->num << endl;
	file2.close();
	booknum = booknum + q->num;
	p->next = q;
	p = q;
	p->next = NULL;
}

void Search_book(borrowlist &b, booklist L)
{//管理员查询书籍信息
	int flag = 1;//控制回到上一级目录
	int choice;
	while (flag)
	{
		cout << "1、查询单本书籍借阅信息\n2、查询一类书籍借阅率\n3、书籍借阅率排名查询\n0、返回上一级菜单\n请输入你要选择的功能：";
		cin >> choice;
		while (choice < 0 || choice>3)
		{
			cout << "输入不合规范，请重新输入！";
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
{//查询单本书籍借阅信息
	string name;
	cout << "请输入要查找的书名：";
	cin >> name;
	borrowlist q = b;
	booklist p = L->next;
	while (q != NULL)
	{
		if (q->bookname == name)
		{
			cout << "借阅人为：" << q->personname << "  ";
			cout << "借阅时间为：" << q->time.year << "/" << q->time.month << "/" << q->time.day << endl;
		}
		q = q->next;
	}
	while (p != NULL)
	{
		if (p->name == name)
		{
			double  m = (double)p->times / (double)borrowtimes;
			cout << "该类书籍借阅率为：" << m * 100 << "%" << endl;
			break;
		}
		p = p->next;
	}
}

void Search_typebook(booklist L)
{//管理员查询一类书籍借阅率
	char type;
	double x = 0;
	cout << "请输入要查找的书的类型：";
	cin >> type;
	booklist p = L->next;
	while (p != NULL)
	{
		if (p->sort == type)
			x = x + p->times;
		p = p->next;
	}
	double  m = (double)x / (double)borrowtimes;
	cout << "该类书籍借阅率为：" << (m * 100) << "%" << endl;
}

void raterank(booklist L)//采用归并排序的方式对借阅率从高到低进行排名
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
	cout << "请输入你想查询借阅率排名前多少名的书：";
	cin >> n;
	for (j = 1; j <= n; j++)
		cout << "书名为：" << name[j] << "  " << "借阅次数为：" << times[j] << "  借阅率为：" << (double)((double)times[j] / (double)borrowtimes) << endl;
	system("pause");
	system("cls");
}

void MSort(int *sr, int *tr1, string *mr, string *mr1, int s, int t)
{//将sr[s..t]归并排序为tr[s..t]
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
		m = (s + t) / 2;  //将sr[s...t]平分为sr[s..m]和sr[m+1..t]
		MSort(sr, tr2, mr, mr2, s, m);   //递归地将sr[s..m]归并为有序的tr2[s..m]
		MSort(sr, tr2, mr, mr2, m + 1, t);   //递归地将sr[m+1..t]归并为有序的tr2[m+1..t]
		Merge(tr2, tr1, mr2, mr1, s, m, t);  //将tr2[s..m]和tr2[m+1..t]归并到tr1[s..t]
	}
}//MSort

void Merge(int *sr, int *tr, string *mr, string *nr, int i, int m, int n)
{//将有序的sr[i..m]和sr[m+1..n]归并为tr[i..n]
	int j, k, temp;
	for (j = m + 1, k = i; i <= m &&j <= n; ++k)
	{//将sr中记录由小到大地并入tr中
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
	{//将剩余的sr[i..m]复制到tr
		for (temp = k; temp <= n && i <= m; temp++, i++)
		{
			tr[temp] = sr[i];
			nr[temp] = mr[i];
		}
	}
	if (j <= n)
	{//将剩余的sr[j..n]复制到tr
		for (temp = k; temp <= n && j <= n; temp++, j++)
		{
			tr[temp] = sr[j];
			nr[temp] = mr[j];
		}
	}
}//Merge

void Search_person(struct role *p, borrowlist &b)
{//管理员查询人员信息
	string name;
	int i;
	cout << "请输入要查找的人名：";
	cin >> name;
	borrowlist q = b;
	for (i = 0; i < peoplenum; i++)
	{
		if (p[i].name == name)
		{
			cout << "学院：" << p[i].college << endl;
			cout << "已经借书数量：" << p[i].borrow_num << "	" << "尚可借书数量：" << p[i].borrow_max - p[i].borrow_num << endl;
			cout << "当前诚信度：" << p[i].honesty << "  " << "当前所欠罚金：" << p[i].fine << endl;
			break;
		}
	}
	while (q != NULL)
	{

		if (q->personname == name&&p[i].borrow_num != 0)
		{
			cout << "借阅图书为：" << q->bookname << endl;
			cout << "借阅时间为：" << q->time.year << "/" << q->time.month << "/" << q->time.day << endl;
			q = q->next;
		}
		else
			q = q->next;
	}
}

int isLeap(int year) //判断是否是闰年的函数 
{
	if (year % 4 == 0 && year % 100 != 0 || year % 400 == 0)
		return 1;//是闰年则返回1 
	else
		return 0;//不是闰年返回0 
}

void Savedate(date T)//刷新修改后的时间文档并更新内容
{
	fstream file5("configuration.txt", ios::out);
	if (file5)
		file5 << T.year << " " << T.month << " " << T.day << " " << borrowtimes << endl;
	file5.close();
}

int GetMonthDays(int year, int month)//得到每月有多少天的函数 
{
	if (isLeap(year) == 1)//闰年的情况 
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

date Add_One_Day(date date)//根据具体情况进行日期的推移
{
	if (date.day + 1 <= GetMonthDays(date.year, date.month))
		date.day++;//调用万年历的函数计算每个月对应的天数
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
	Savedate(date);//对修改的日期进行保存
	return date;
}

void checkoverdue(struct role *a, int n, borrowlist &b, booklist L, date time)
{//检查借阅的书籍有没有逾期情况，超过15天为逾期
	borrow *p = b->next;
	int i = a[n].borrow_num, j;
	for (j = 0; j < i; j++)
	{
		while (p != NULL)
		{
			if (p->personname == a[n].name)
			{
				if (caldays(p->time, time) > 15)
					cout << "你借阅的" << p->bookname << "已经过期，请尽快归还！" << endl;
			}
			p = p->next;
		}
	}
}

int caldays(date t1, date t2)
{
	//计算t2比t1前进了多少天
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

void get_next(int *next, char *a, int la) //求next[]的值
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
	strcpy_s(a_, a.c_str());//转换类型，将string转换成字符数组
	char b_[256];
	strcpy_s(b_, b.c_str());
	int la = 0, lb = 0;
	la = strlen(a_);
	lb = strlen(b_);
	for (k = la - 1; k >= 0; k--)
		a[k + 1] = a[k];
	for (k = lb - 1; k >= 0; k--)
		b[k + 1] = b[k];
	get_next(next, b_, lb);//得到next数组
	k = str_kmp(next, a_, b_, la, lb);
	if (k == -1)
		return 0;
	else
		return 1;

}
