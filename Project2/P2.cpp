#include<iostream>
#include<string>
#include<vector>
#define MEM_SIZE 128
#define BUSY 1
#define FREE 0
using namespace std;

static vector<int> existIds;  //�洢�ڴ��д��ڵ���ҵ��

struct MNode {
	int taskId;  //��ҵ���������ţ�
	int length;   //����������
	int begin_address;  //��ʼ��ַ
	int state;  //0��ʾ���У� 1��ʾ�ѷ���
};
struct ZoneNode {
	ZoneNode* prior;
	ZoneNode* next;
	MNode dsp;
};

ZoneNode* headZ;
ZoneNode* lastZ;

bool initMemZ() {        //�ڴ��ʼ��
	headZ = new ZoneNode();
	lastZ = new ZoneNode();
	headZ->prior = NULL; //��ʼ��ͷ���
	headZ->next = lastZ;
	lastZ->prior = headZ;   //��ʼ��β�ڵ�
	lastZ->next = NULL;  
	lastZ->dsp.begin_address = 0;
	lastZ->dsp.length = MEM_SIZE;
	lastZ->dsp.state = FREE;
	lastZ->dsp.taskId = 0;
	return true;
}

bool firstFitAlloc(int taskid, int nmem);   //�����״���Ӧ�㷨���������㱻���亯������

bool allocMem() {      //�ڴ���亯��
	int tId, neededMem;
	cout << "��������ҵ���������ţ���";
	cin >> tId;
	if (tId <= 0) {
		cout << "��Ч��" << endl;
		return false;
	}
	cout << "��������Ҫ����������С����λ:KB����";
	cin >> neededMem;
	if (neededMem <= 0) {
		cout << "���󣡷�����ڴ�ռ�������0�������ԣ�" << endl;
		return false;
	}
	if (firstFitAlloc(tId, neededMem)) {
		cout << "����ɹ���" << endl;
		existIds.push_back(tId);
		return true;
	}
	else{
		cout << "����ʧ�ܣ�" << endl;
		return false;
	}
}
bool firstFitAlloc(int taskid, int nmem) {   //�״���Ӧ�㷨����
	ZoneNode* newp, * p;
	newp = new ZoneNode();
	p = headZ->next;
	newp->dsp.taskId = taskid;
	newp->dsp.state = BUSY;
	newp->dsp.length = nmem;
	while (p)
	{
		if (p->dsp.taskId != taskid) {        //��ҵ�������ظ�
			if (p->dsp.length == nmem && p->dsp.state == FREE) {//�պ����㹻�ռ���Է���
				p->dsp.state = BUSY;
				p->dsp.taskId = taskid;
				return true;
			}
			else if (p->dsp.length > nmem && p->dsp.state == FREE) {//����ʣ��ռ�
				newp->prior = p->prior;
				newp->next = p;
				p->prior->next = newp;
				p->prior = newp;
				newp->dsp.begin_address = p->dsp.begin_address;
				p->dsp.begin_address = newp->dsp.begin_address + newp->dsp.length;
				p->dsp.length -= nmem;
				return true;
			}
			p = p->next;
		}
		else {
			cout << "��ҵ" << taskid << "�����ڴ��У�" << endl;
			return false;
		}
	}
	cout << "�ڴ�ռ䲻�㣡" << endl;
	return false;
}

bool freeMem() {    //�����ڴ�
	ZoneNode* p = headZ;
	int taskid;
	cout << "��������Ҫ�ͷŵķ����ţ�";
	cin >> taskid;
	while (p)
	{
		int nRet = count(existIds.begin(), existIds.end(), taskid);
		if (nRet == 0) {                          //���ܻ����ڴ��в����ڵ���ҵ
			cout << "��ҵ�����ڣ�";
			return false;
		}
		if (p->dsp.taskId == taskid)
		{	
			p->dsp.state = FREE;
			p->dsp.taskId = FREE;
			if (p->prior->dsp.state == FREE && p->prior->prior != NULL)//��ǰ��Ŀ��п�ϲ�
			{
				p->prior->dsp.length += p->dsp.length;
				p->prior->next = p->next;
				p->next->prior = p->prior;
				p = p->prior;
			}
			if (p->next->dsp.state == FREE)//�����Ŀ��п�ϲ�
			{
				p->dsp.length += p->next->dsp.length;
				if (p->next->next == NULL)         //��ֹ�������һ��ʱ����
				{
					p->next = NULL;
				}
				else {
					p->next->next->prior = p;
					p->next = p->next->next;
				}
			}
			break;
		}

		p = p->next;
	}
	cout << "�����ţ�" << taskid << " ���ճɹ���" << endl;
	return true;
}

bool showMem() {   //��ʾ�ڴ����״��
	ZoneNode* p = headZ->next;
	cout << "**************" << endl;
	cout << "*����������*" << endl;
	cout << "**************" << endl;
	cout << "\t" << "id��" << "\t" << "begin_addr��" << "\t" << "size��" << "\t" << "state��" << endl;
	while (p)
	{
		if (p->dsp.taskId == FREE)
			cout << "\t" << "Free" << "\t";
		else
			cout << "\t" << p->dsp.taskId << "\t";
		
		cout  << p->dsp.begin_address << "\t" << "\t";
		cout  << p->dsp.length << " KB" << "\t";
		
		if (p->dsp.state == FREE)
			cout << "�� ��" << endl;
		else
			cout << "�ѷ���" << endl;
		p = p->next;
	}
	return true;
}

int main() {
	int choice;
	if (initMemZ()) 
		cout << "**��ʼ���ɹ�**" << endl;
	cout << endl;
	cout << "�״���Ӧ�㷨ģ�⣺" << endl;
	while (true)
	{
		cout << "-------------------------------------------------------" << endl;
		cout << "1. �����ڴ�" <<  "\t" <<"2. �����ڴ�" << "\t" << "3. �鿴����" << "\t" << "0. �˳�" << endl;
		cout << endl;
		cout << "���������Ĳ�����";
		cin >> choice;
		cout << "-------------------------------------------------------" << endl;
		switch (choice) {
		case 1:
			cout << "-------------------------------------------------------" << endl;
			if (allocMem()) {
				cout << endl;
				cout << "�����ɹ���" << endl;
			}
			else
			{
				cout << "����ʧ�ܣ�" << endl;
			}
			cout << "-------------------------------------------------------" << endl;
			break;
		case 2:
			cout << "-------------------------------------------------------" << endl;
			if (freeMem()) {
				cout << endl;
				cout << "�����ɹ���" << endl;
			}
			else
			{
				cout << "����ʧ�ܣ�" << endl;
			}
			cout << "-------------------------------------------------------" << endl;
			break;
		case 3:
			cout << "-------------------------------------------------------" << endl;
			if (showMem()) {
				cout << endl;
				cout << "�����ɹ���" << endl;
			}
			else
			{
				cout << "����ʧ�ܣ�" << endl;
			}
			cout << "-------------------------------------------------------" << endl;
			break;
		case 0:
			exit(0);
			break;
		default:
			cout << "-------------------------------------------------------" << endl;
			cout << "��Ч����";
			cout << "-------------------------------------------------------" << endl;
			break;
		}
	}
	return 0;
}
/*
�ɱ��������ʽ�²����״���Ӧ�㷨ʵ���������ͻ���
[��ʾ]��
(1)	�ɱ������ʽ�ǰ���ҵ��Ҫ������ռ��С���ָ�����ġ���Ҫװ��һ����ҵʱ��������ҵ��Ҫ�����������鿴�Ƿ����㹻�Ŀ��пռ䣬���У�������䣬������ҵ�޷�װ�롣�ٶ��ڴ��СΪ128K��������˵�����ʽΪ��
�������š�����ʾ�ǵڼ������з�����
����ʼ��ַ����ָ������������ʼ��ַ��
�����ȡ���һ�������������ĳ��ȣ�
(2)	�����״���Ӧ�㷨��������ڴ�ռ䡣����ʱ������һϵ�з�������ͻ�������
Ҫ���ܽ������Լ��̵Ŀռ����뼰�ͷ���������ʾ�������估���պ���ڴ沼�������

*/