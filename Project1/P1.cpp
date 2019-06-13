#include <iostream>
#include<time.h>
#include<string>
#include<vector>
using namespace std;

static int countNo = 0; //记录并限定进程数
static vector<string> existedPCBs(5);

struct PCB{
	string pId = ""; //进程名
	int level; //进程优先数
	int time; //需要运行的时间
	string state = ""; //状态R表示就绪， E表示结束
	PCB* next;
};
void initPCBs(PCB *head) {
	PCB* p, *q;
	while (countNo < 5) {
		p = head;
		q = new PCB();  //创建新进程
		cout << "请依次输入进程名, 进程优先级，进程需要运行的时间（大于0）" << endl;
		cin >> q->pId >> q->level >> q->time;    //接收用户输入
		int nRet = count(existedPCBs.begin(), existedPCBs.end(), q->pId);
		if (nRet > 0) {//判定用户输入的进程名是否重复
			cout << "进程重复！请重新创建进程！" << endl;
			continue;
		}
		else
			existedPCBs.push_back(q->pId); //将进程名放入vector中
		if (q->time > 0) {
			q->state = "R";
			while (p->next) {
				if (q->level > p->next->level)
					break;
				p = p->next;
			}
			q->next = p->next;
			p->next = q;
			countNo++;  //计数（当前已创建的进程）
		}
		else {
			q->state = "E";
			cout << "错误！运行时间必须大于0，请重新创建进程！" << endl;
		}
	}
	
}
void runPCBs(PCB *head) {  //优先数越大越先执行
	PCB* p, *cp;
	string state = "";   //描述当前运行的进程运行后的状态
	p = head;
	cp = p->next;
	string pname = cp->pId;  //获取当前运行的进程名
	p->next = cp->next;
	cout << pname + "运行" << endl;
	cp->level--;             //优先数减1
	if (cp->level < 0) {    //优先数小于0时置为0
		cp->level = 0;
	}
	cp->time--;              //要求运行时间减1
	if (cp->time > 0) {
		state = "···就绪···";
		while (p->next) {
			if (cp->level >= p->next->level)
				break;                              //如果优先数相等，则按照队列顺序运行
			p = p->next;
		}
		cp->next = p->next;
		p->next = cp;                      //优先数变化后重新排序
	}
	else {
		state = "···结束···";
		delete cp;
	}
	cout << "****************************" << endl;
	cout << pname + "进程状态：" + state << endl;  //当前进程状态
	cout << "----------------------------" << endl;
	p = head;
	if (p->next != NULL) {
		cout << "剩余进程状况：" << endl;
		cout << "进程名" << "\t" << "优先数" << "\t" << "时间" << "\t" << "状态" << endl;
	}
	while (p->next)
	{   //输出剩余进程状况
		cout << p->next->pId << "\t" << p->next->level << "\t" << p->next->time << "\t" << p->next->state << endl;
		p = p->next;
	}
	cout << endl;
}
int main() {  
	PCB* head = new PCB();
	head->next = NULL;
	initPCBs(head);    //初始化进程
	cout << endl;
	while (head->next) {
		runPCBs(head);        //依次运行进程
	}
	cout << "***********" << endl;
	cout << "*模拟结束!*" << endl;
	cout << "***********" << endl;
	return 0;
}

/*(1)假定系统有5个进程，每个进程用一个PCB来代表。PCB的结构为：
·进程名——如P1~P5。
·指针——按优先数的大小把5个进程连成队列，用指针指出下一个进程PCB的首地址。
·要求运行时间——假设进程需要运行的单位时间数。
·优先数——赋予进程的优先数，调度时总是选取优先数大的进程先执行。
·状态——假设两种状态：就绪和结束，用R表示就绪，用E表示结束。初始状态都为就绪状态。
(2) 开始运行之前，为每个进程确定它的“优先数”和“要求运行时间”。通过键盘输入这些参数。
(3) 处理器总是选择队首进程运行。采用动态改变优先数的办法，进程每运行1次，优先数减1，要求运行时间减1。
(4) 进程运行一次后，若要求运行时间不等于0，则将它加入就绪队列，否则，将状态改为“结束”，退出就绪队列。
(5) 若就绪队列为空，结束，否则转到(3)重复。
 要求能接受键盘输入的进程优先数及要求运行时间，能显示每次进程调度的情况，如哪个进程在运行，哪些进程就绪，就绪进程的排列情况。
 */