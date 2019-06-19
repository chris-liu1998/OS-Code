#include<iostream>
#include<string>
#include<vector>
#define MEM_SIZE 128
#define BUSY 1
#define FREE 0
using namespace std;

static vector<int> existIds;  //存储内存中存在的作业名

struct MNode {
	int taskId;  //作业名（分区号）
	int length;   //空闲区长度
	int begin_address;  //起始地址
	int state;  //0表示空闲， 1表示已分配
};
struct ZoneNode {
	ZoneNode* prior;
	ZoneNode* next;
	MNode dsp;
};

ZoneNode* headZ;
ZoneNode* lastZ;

bool initMemZ() {        //内存初始化
	headZ = new ZoneNode();
	lastZ = new ZoneNode();
	headZ->prior = NULL; //初始化头结点
	headZ->next = lastZ;
	lastZ->prior = headZ;   //初始化尾节点
	lastZ->next = NULL;  
	lastZ->dsp.begin_address = 0;
	lastZ->dsp.length = MEM_SIZE;
	lastZ->dsp.state = FREE;
	lastZ->dsp.taskId = 0;
	return true;
}

bool firstFitAlloc(int taskid, int nmem);   //声明首次适应算法函数，方便被分配函数调用

bool allocMem() {      //内存分配函数
	int tId, neededMem;
	cout << "请输入作业名（分区号）：";
	cin >> tId;
	if (tId <= 0) {
		cout << "无效！" << endl;
		return false;
	}
	cout << "请输入需要分配的主存大小（单位:KB）：";
	cin >> neededMem;
	if (neededMem <= 0) {
		cout << "错误！分配的内存空间必须大于0，请重试！" << endl;
		return false;
	}
	if (firstFitAlloc(tId, neededMem)) {
		cout << "分配成功！" << endl;
		existIds.push_back(tId);
		return true;
	}
	else{
		cout << "分配失败！" << endl;
		return false;
	}
}
bool firstFitAlloc(int taskid, int nmem) {   //首次适应算法函数
	ZoneNode* newp, * p;
	newp = new ZoneNode();
	p = headZ->next;
	newp->dsp.taskId = taskid;
	newp->dsp.state = BUSY;
	newp->dsp.length = nmem;
	while (p)
	{
		if (p->dsp.taskId != taskid) {        //作业名不能重复
			if (p->dsp.length == nmem && p->dsp.state == FREE) {//刚好有足够空间可以分配
				p->dsp.state = BUSY;
				p->dsp.taskId = taskid;
				return true;
			}
			else if (p->dsp.length > nmem && p->dsp.state == FREE) {//会有剩余空间
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
			cout << "作业" << taskid << "已在内存中！" << endl;
			return false;
		}
	}
	cout << "内存空间不足！" << endl;
	return false;
}

bool freeMem() {    //回收内存
	ZoneNode* p = headZ;
	int taskid;
	cout << "请输入您要释放的分区号：";
	cin >> taskid;
	while (p)
	{
		int nRet = count(existIds.begin(), existIds.end(), taskid);
		if (nRet == 0) {                          //不能回收内存中不存在的作业
			cout << "作业不存在！";
			return false;
		}
		if (p->dsp.taskId == taskid)
		{	
			p->dsp.state = FREE;
			p->dsp.taskId = FREE;
			if (p->prior->dsp.state == FREE && p->prior->prior != NULL)//与前面的空闲块合并
			{
				p->prior->dsp.length += p->dsp.length;
				p->prior->next = p->next;
				p->next->prior = p->prior;
				p = p->prior;
			}
			if (p->next->dsp.state == FREE)//与后面的空闲块合并
			{
				p->dsp.length += p->next->dsp.length;
				if (p->next->next == NULL)         //防止回收最后一个时出错
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
	cout << "分区号：" << taskid << " 回收成功！" << endl;
	return true;
}

bool showMem() {   //显示内存分配状况
	ZoneNode* p = headZ->next;
	cout << "**************" << endl;
	cout << "*主存分配情况*" << endl;
	cout << "**************" << endl;
	cout << "\t" << "id：" << "\t" << "begin_addr：" << "\t" << "size：" << "\t" << "state：" << endl;
	while (p)
	{
		if (p->dsp.taskId == FREE)
			cout << "\t" << "Free" << "\t";
		else
			cout << "\t" << p->dsp.taskId << "\t";
		
		cout  << p->dsp.begin_address << "\t" << "\t";
		cout  << p->dsp.length << " KB" << "\t";
		
		if (p->dsp.state == FREE)
			cout << "空 闲" << endl;
		else
			cout << "已分配" << endl;
		p = p->next;
	}
	return true;
}

int main() {
	int choice;
	if (initMemZ()) 
		cout << "**初始化成功**" << endl;
	cout << endl;
	cout << "首次适应算法模拟：" << endl;
	while (true)
	{
		cout << "-------------------------------------------------------" << endl;
		cout << "1. 分配内存" <<  "\t" <<"2. 回收内存" << "\t" << "3. 查看分配" << "\t" << "0. 退出" << endl;
		cout << endl;
		cout << "请输入您的操作：";
		cin >> choice;
		cout << "-------------------------------------------------------" << endl;
		switch (choice) {
		case 1:
			cout << "-------------------------------------------------------" << endl;
			if (allocMem()) {
				cout << endl;
				cout << "操作成功！" << endl;
			}
			else
			{
				cout << "操作失败！" << endl;
			}
			cout << "-------------------------------------------------------" << endl;
			break;
		case 2:
			cout << "-------------------------------------------------------" << endl;
			if (freeMem()) {
				cout << endl;
				cout << "操作成功！" << endl;
			}
			else
			{
				cout << "操作失败！" << endl;
			}
			cout << "-------------------------------------------------------" << endl;
			break;
		case 3:
			cout << "-------------------------------------------------------" << endl;
			if (showMem()) {
				cout << endl;
				cout << "操作成功！" << endl;
			}
			else
			{
				cout << "操作失败！" << endl;
			}
			cout << "-------------------------------------------------------" << endl;
			break;
		case 0:
			exit(0);
			break;
		default:
			cout << "-------------------------------------------------------" << endl;
			cout << "无效！！";
			cout << "-------------------------------------------------------" << endl;
			break;
		}
	}
	return 0;
}
/*
可变分区管理方式下采用首次适应算法实现主存分配和回收
[提示]：
(1)	可变分区方式是按作业需要的主存空间大小来分割分区的。当要装入一个作业时，根据作业需要的主存容量查看是否有足够的空闲空间，若有，则按需分配，否则，作业无法装入。假定内存大小为128K，空闲区说明表格式为：
·分区号——表示是第几个空闲分区；
·起始地址——指出空闲区的起始地址；
·长度——一个连续空闲区的长度；
(2)	采用首次适应算法分配回收内存空间。运行时，输入一系列分配请求和回收请求。
要求能接受来自键盘的空间申请及释放请求，能显示分区分配及回收后的内存布局情况。

*/