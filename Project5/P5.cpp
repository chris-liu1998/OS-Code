/*
ģ��P��V����ʵ��ͬ������������P��V������������ߡ����������⡣
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

int p(int s)//P����ԭ��
{ 
	s--;
	if (s < 0) {
		process_1->state = "����";
		process_1->wait_rsn = "s";
	}
	else
	{
		process_1->state = "����";
	}
	return s;
}

int v(int s)   //V����ԭ��
{
	s++;
	if (s <= 0) {
		process_2->state = "����";
	}
	process_1->state = "����";
	return s;
}

char RandomChar()
{
	char arr[10] = { 'a','b','c','d','e','f','g','h','i','j' };    //�����Ʒ
	srand((unsigned int)(time(NULL)));
	return arr[abs(rand() % 10)];
}


void initPCBs()/*��ʼ������*/
{
	s1 = 10;                //s1��ʾ�ջ�����������
	s2 = 0;                 //s2��ʾ��������������
	producer.name = "������";           //�������߽��̳�ʼ��
	producer.state = "����";
	producer.wait_rsn = "";
	producer.break_point = 0;
	consumer.name = "������";          //�������߽��̳�ʼ��
	consumer.state = "����";
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
	;  //�ղ���
}

void put()
{

	cout << "���»س��Լ���";
	system("pause");
	cout << endl;
	Sleep(1000);
	p_array[in] = RandomChar();
	in = (in + 1) % 10;
	cout << "------------------------" << endl;
	cout << "������ ����һ����Ʒ��" << p_array[in - 1] << endl;
	int count = 0;
	for (m = 0; m < 10; m++)
	{
		if (p_array[m] != ' ') {
			cout << p_array[m] << endl;
			count++;
		}

	}
	cout << "���������" << count << "����Ʒ" << endl;
	cout << "------------------------" << endl;
}

void get()
{
	Sleep(1000);
	x = p_array[out];
	cout << "------------------------" << endl;
	cout << "������ ȡ��һ����Ʒ��" << x << endl;
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
	cout << "���������" << count << "����Ʒ" << endl;
	cout << "------------------------" << endl;
}

void showPCBs() //��ʾ��ǰ����
{
	cout << "������" << "\t" << "״̬" << "\t" << "ԭ��" << "\t" << "�ϵ�" << endl;
	cout << process_1->name << "\t";
	cout << process_1->state << "\t";
	cout << process_1->wait_rsn << "\t";
	cout << process_1->break_point << "\t" << endl;
	cout << endl;
}

void putInPA()           //��������߳����е�һ��ģ��ָ��ִ�е���ڵ�ַ
{
	for (i = 0; i <= 3; i++)
	{
		PA[i] = i;
	}
}

void putInSA()               //��������߳����е�һ��ģ��ָ��ִ�е���ڵ�ַ
{
	for (i = 0; i <= 3; i++)
	{
		SA[i] = i;
	}
}

void runPCBs() {
	while ((producer.state == "����") || (consumer.state == "����"))
	{
		x = rand();    //x������һ����
		x = x % 2;   //��xȡ��
		if (x == 0)      //��x�����㣬��ִ�������߽��̣���ִ֮�������߽���
		{
			process_1 = &producer;   //process_1��ʾ���н��̣������н�����Ϊ�����߽���
			process_2 = &consumer;
		}
		else
		{
			process_1 = &consumer;
			process_2 = &producer;
		}
		PC = process_1->break_point;
		i = PC;                     //��ʱ��PC��ֵ����i
		if ((process_1->name == "������") && (process_1->state == "����"))
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
				process_1->state = "����";
				process_1->wait_rsn = "";
				process_1->break_point = PC;
				break;
			case 2:
				s2 = v(s2);
				process_1->break_point = PC;
				break;
			case 3:
				gotoL(); 
				process_1->state = "����";
				process_1->wait_rsn = "";
				process_1->break_point = PC;
				break;
			}
			showPCBs();
		}
		else if ((process_1->name == "������") && (process_1->state == "����"))//ִ�������߽����Ҹý��̴��ھ���״̬
		{
			process_1->state = "����";
			process_1->wait_rsn = "";
			j = SA[i];
			PC = i + 1;
			switch (j)
			{
			case 0:
				s2 = p(s2);
				process_1->break_point = PC;
				break;               //������Դ����û�����뵽����ת
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
				process_1->state = "����";
				process_1->break_point = PC;
				break;
			}
			showPCBs();
		}
	}
	cout << "The program is over!" << endl;

}

int main() {     //������
	initPCBs();
	putInPA();
	putInSA();
	runPCBs();
	return 0;
}

