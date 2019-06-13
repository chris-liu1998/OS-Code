/*
模拟P、V操作实现同步机构，且用P、V操作解决生产者—消费者问题。
*/

#include<iostream>
#include<string>
#include<windows.h>
#include<time.h>

using namespace std;

struct PCB {
	string name;
	string state;
	string wait_rsn;
	int break_point;
};

PCB producer, consumer;
PCB* process_1, * process_2;
int PA[6], SA[6];
int PC;
int in, out;
int s1, s2;
int i, j, m;
char p_array[10];
char c, x;

int p(int s)//P操作原语
{ 
	s--;
	if (s < 0) {
		process_1->state = "阻塞";
		process_1->wait_rsn = "s";
	}
	else
	{
		process_1->state = "就绪";
	}
	return s;
}

int v(int s)   //V操作原语
{
	s++;
	if (s <= 0) {
		process_2->state = "就绪";
	}
	process_1->state = "就绪";
	return s;
}

char RandomChar()
{
	char arr[10] = { 'a','b','c','d','e','f','g','h','i','j' };    //随机产品
	srand((unsigned int)(time(NULL)));
	return arr[abs(rand() % 10)];
}


void initPCBs()/*初始化程序*/
{
	s1 = 10;                //s1表示空缓冲区的数量
	s2 = 0;                 //s2表示满缓冲区的数量
	producer.name = "生产者";           //对生产者进程初始化
	producer.state = "就绪";
	producer.wait_rsn = "";
	producer.break_point = 0;
	consumer.name = "消费者";          //对消费者进程初始化
	consumer.state = "就绪";
	consumer.wait_rsn = "";
	consumer.break_point = 0;
	for (int k = 0; k < 10; k++)
	{
		p_array[k] = ' ';
	}
}

void gotoL()
{
	PC = 0;
}

void nop()
{
	;  //空操作
}

void put()
{

	cout << "按下回车以继续";
	system("pause");
	cout << endl;
	Sleep(1000);
	p_array[in] = RandomChar();
	in = (in + 1) % 10;
	cout << "------------------------" << endl;
	cout << "生产者 生产一个产品：" << p_array[in - 1] << endl;
	int count = 0;
	for (m = 0; m < 10; m++)
	{
		if (p_array[m] != ' ') {
			cout << p_array[m] << endl;
			count++;
		}

	}
	cout << "缓冲池中有" << count << "个产品" << endl;
	cout << "------------------------" << endl;
}

void get()
{
	Sleep(1000);
	x = p_array[out];
	cout << "------------------------" << endl;
	cout << "消费者 取出一个产品：" << x << endl;
	p_array[out] = ' ';
	out = (out + 1) % 10;
	int count = 0;
	for (m = 0; m < 10; m++)
	{
		if (p_array[m] != ' ') {
			cout << p_array[m] << endl;
			count++;
		}

	}
	cout << "缓冲池中有" << count << "个产品" << endl;
	cout << "------------------------" << endl;
}

void showPCBs() //显示当前进程
{
	cout << "进程名" << "\t" << "状态" << "\t" << "原因" << "\t" << "断点" << endl;
	cout << process_1->name << "\t";
	cout << process_1->state << "\t";
	cout << process_1->wait_rsn << "\t";
	cout << process_1->break_point << "\t" << endl;
	cout << endl;
}

void putInPA()           //存放生产者程序中的一条模拟指令执行的入口地址
{
	for (i = 0; i <= 3; i++)
	{
		PA[i] = i;
	}
}

void putInSA()               //存放消费者程序中的一条模拟指令执行的入口地址
{
	for (i = 0; i <= 3; i++)
	{
		SA[i] = i;
	}
}

void runPCBs() {
	while ((producer.state == "就绪") || (consumer.state == "就绪"))
	{
		x = rand();    //x随机获得一个数
		x = x % 2;   //对x取余
		if (x == 0)      //若x等于零，则执行生产者进程，反之执行消费者进程
		{
			process_1 = &producer;   //process_1表示现行进程，将现行进程置为生产者进程
			process_2 = &consumer;
		}
		else
		{
			process_1 = &consumer;
			process_2 = &producer;
		}
		PC = process_1->break_point;
		i = PC;                     //此时把PC的值赋给i
		if ((process_1->name == "生产者") && (process_1->state == "就绪"))
		{

			j = PA[i];
			PC = i + 1;
			switch (j)
			{

			case 0:
				s1 = p(s1); 
				process_1->break_point = PC;
				break;
			case 1:
				put();
				process_1->state = "就绪";
				process_1->wait_rsn = "";
				process_1->break_point = PC;
				break;
			case 2:
				s2 = v(s2);
				process_1->break_point = PC;
				break;
			case 3:
				gotoL(); 
				process_1->state = "就绪";
				process_1->wait_rsn = "";
				process_1->break_point = PC;
				break;
			}
			showPCBs();
		}
		else if ((process_1->name == "消费者") && (process_1->state == "就绪"))//执行消费者进程且该进程处于就绪状态
		{
			process_1->state = "就绪";
			process_1->wait_rsn = "";
			j = SA[i];
			PC = i + 1;
			switch (j)
			{
			case 0:
				s2 = p(s2);
				process_1->break_point = PC;
				break;               //申请资源，若没有申请到则跳转
			case 1:
				get();
				process_1->break_point = PC;
				break;
			case 2:
				s1 = v(s1); 
				process_1->break_point = PC;
				break;
			case 3:
				gotoL(); 
				process_1->state = "就绪";
				process_1->break_point = PC;
				break;
			}
			showPCBs();
		}
	}
	cout << "The program is over!" << endl;

}

int main() {     //主程序
	initPCBs();
	putInPA();
	putInSA();
	runPCBs();
	return 0;
}

