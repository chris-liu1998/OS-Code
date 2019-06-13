#include <iostream>
#include<time.h>
#include<string>
#include<vector>
using namespace std;

static int countNo = 0; //��¼���޶�������
static vector<string> existedPCBs(5);

struct PCB{
	string pId = ""; //������
	int level; //����������
	int time; //��Ҫ���е�ʱ��
	string state = ""; //״̬R��ʾ������ E��ʾ����
	PCB* next;
};
void initPCBs(PCB *head) {
	PCB* p, *q;
	while (countNo < 5) {
		p = head;
		q = new PCB();  //�����½���
		cout << "���������������, �������ȼ���������Ҫ���е�ʱ�䣨����0��" << endl;
		cin >> q->pId >> q->level >> q->time;    //�����û�����
		int nRet = count(existedPCBs.begin(), existedPCBs.end(), q->pId);
		if (nRet > 0) {//�ж��û�����Ľ������Ƿ��ظ�
			cout << "�����ظ��������´������̣�" << endl;
			continue;
		}
		else
			existedPCBs.push_back(q->pId); //������������vector��
		if (q->time > 0) {
			q->state = "R";
			while (p->next) {
				if (q->level > p->next->level)
					break;
				p = p->next;
			}
			q->next = p->next;
			p->next = q;
			countNo++;  //��������ǰ�Ѵ����Ľ��̣�
		}
		else {
			q->state = "E";
			cout << "��������ʱ��������0�������´������̣�" << endl;
		}
	}
	
}
void runPCBs(PCB *head) {  //������Խ��Խ��ִ��
	PCB* p, *cp;
	string state = "";   //������ǰ���еĽ������к��״̬
	p = head;
	cp = p->next;
	string pname = cp->pId;  //��ȡ��ǰ���еĽ�����
	p->next = cp->next;
	cout << pname + "����" << endl;
	cp->level--;             //��������1
	if (cp->level < 0) {    //������С��0ʱ��Ϊ0
		cp->level = 0;
	}
	cp->time--;              //Ҫ������ʱ���1
	if (cp->time > 0) {
		state = "����������������";
		while (p->next) {
			if (cp->level >= p->next->level)
				break;                              //�����������ȣ����ն���˳������
			p = p->next;
		}
		cp->next = p->next;
		p->next = cp;                      //�������仯����������
	}
	else {
		state = "����������������";
		delete cp;
	}
	cout << "****************************" << endl;
	cout << pname + "����״̬��" + state << endl;  //��ǰ����״̬
	cout << "----------------------------" << endl;
	p = head;
	if (p->next != NULL) {
		cout << "ʣ�����״����" << endl;
		cout << "������" << "\t" << "������" << "\t" << "ʱ��" << "\t" << "״̬" << endl;
	}
	while (p->next)
	{   //���ʣ�����״��
		cout << p->next->pId << "\t" << p->next->level << "\t" << p->next->time << "\t" << p->next->state << endl;
		p = p->next;
	}
	cout << endl;
}
int main() {  
	PCB* head = new PCB();
	head->next = NULL;
	initPCBs(head);    //��ʼ������
	cout << endl;
	while (head->next) {
		runPCBs(head);        //�������н���
	}
	cout << "***********" << endl;
	cout << "*ģ�����!*" << endl;
	cout << "***********" << endl;
	return 0;
}

/*(1)�ٶ�ϵͳ��5�����̣�ÿ��������һ��PCB������PCB�ĽṹΪ��
��������������P1~P5��
��ָ�롪�����������Ĵ�С��5���������ɶ��У���ָ��ָ����һ������PCB���׵�ַ��
��Ҫ������ʱ�䡪�����������Ҫ���еĵ�λʱ������
������������������̵�������������ʱ����ѡȡ��������Ľ�����ִ�С�
��״̬������������״̬�������ͽ�������R��ʾ��������E��ʾ��������ʼ״̬��Ϊ����״̬��
(2) ��ʼ����֮ǰ��Ϊÿ������ȷ�����ġ����������͡�Ҫ������ʱ�䡱��ͨ������������Щ������
(3) ����������ѡ����׽������С����ö�̬�ı��������İ취������ÿ����1�Σ���������1��Ҫ������ʱ���1��
(4) ��������һ�κ���Ҫ������ʱ�䲻����0����������������У����򣬽�״̬��Ϊ�����������˳��������С�
(5) ����������Ϊ�գ�����������ת��(3)�ظ���
 Ҫ���ܽ��ܼ�������Ľ�����������Ҫ������ʱ�䣬����ʾÿ�ν��̵��ȵ���������ĸ����������У���Щ���̾������������̵����������
 */