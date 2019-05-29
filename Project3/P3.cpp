#include<iostream>
#include<string>
#define MAX 8

static int freeBit = MAX * MAX;
static int ocupBit = 0;

using namespace std;

struct BitMap {
	int bitmap[MAX][MAX];  //位视图大小
};
void initBitMap(BitMap* bmap) {    //初始化位视图
	int cylNo, trackNo, secNo;
	char choice;
	cout << "···初始化位视图···" << endl;
	cout << "是否进行初始化？（Y/N）";
	cin >> choice;
	while (true)
	{
		if (choice == 'y' || choice == 'Y') {
			cout << "柱面号（0~7）：";
			cin >> cylNo;
			if (cylNo > 7 || cylNo < 0) {
				cout << "柱面号不存在！" << endl;
			}
			else {
				cout << "磁道号（0~1）：";
				cin >> trackNo;
				if (trackNo > 1 || trackNo < 0) {
					cout << "磁道号不存在！" << endl;
				}
				else {
					cout << "物理记录号（0~3）：";
					cin >> secNo;
					if (secNo > 3 || secNo < 0) {
						cout << "物理记录号不存在！" << endl;
					}
					else {
						if (bmap->bitmap[cylNo][trackNo * 4 + secNo] == 0) {  //判断盘块是否已经在初始化时被分配
							bmap->bitmap[cylNo][trackNo * 4 + secNo] = 1;
							cout << "初始化成功！" << endl;
						}
						else {
							cout << "该盘块已被分配！" << endl;
						}
					}

				}

			}
			cout << "是否继续初始化？（Y/N）";
			cin >> choice;
		}
		else if (choice == 'n' || choice == 'N')
		{
			cout << "将默认全部初始化为0" << endl;
			break;
		}
		else{
			cout << "无效！" << endl;
			cout << "是否继续初始化？（Y/N）";
			cin >> choice;
		}
	}
	for (int i = 0; i < MAX; i++) {
		for (int j = 0; j < MAX; j++)
		{
			if (bmap->bitmap[i][j] == 1) {
				freeBit--;
				ocupBit++;
			}
		}
	}
	cout << "空闲盘块：" << freeBit << "个" << "\t" << "占用盘块：" << ocupBit << "个" << endl;
}

void allocSpace(BitMap* bmap) {     //分配磁盘空间
	int cylNo, trackNo, secNo;
	int bitCount = 0;
	int i, j;
	char choice;
	int byteNo, bitNo;
	while (true) {
		cout << "需要分配指定数量的盘块吗？（Y/N）";
		cin >> choice;
		if (choice == 'y' || choice == 'Y') {
			cout << "请输入所需要盘块数：";
			cin >> bitCount;
		}
		else if (choice == 'n' || choice == 'N')
		{
			cout << "要申请盘块的柱面号（0~7）：";
			cin >> cylNo;
			if (cylNo > 7 || cylNo < 0) {
				cout << "柱面号不存在！" << endl;
			}
			else {
				cout << "要申请盘块磁道号（0~1）：";
				cin >> trackNo;
				if (trackNo > 1 || trackNo < 0) {
					cout << "磁道号不存在！" << endl;
				}
				else {
					cout << "要申请盘块物理记录号（0~3）：";
					cin >> secNo;
					if (secNo > 3 || secNo < 0) {
						cout << "物理记录号不存在！" << endl;
					}
					else {
						if (bmap->bitmap[cylNo][trackNo * 4 + secNo] == 0) {  //判断盘块是否已经在初始化时被分配
							bmap->bitmap[cylNo][trackNo * 4 + secNo] = 1;
							cout << "分配成功！";
							byteNo = cylNo;
							bitNo = trackNo * 4 + secNo;
							cout << "字节号：" << byteNo << "\t" << "位号：" << bitNo << endl;
						}
						else {
							cout << "分配失败！该盘块已被分配！" << endl;
						}
					}
					break;
				}
				
			}
			
		}
		else
		{
			cout << "无效！" << endl;
			continue;
		} 
		if (bitCount > freeBit) {
			cout << "分配失败！磁盘空间不足！" << endl;
			break;
		}
		for (i = 0; i < MAX; i++) {
			for (j = 0; j < MAX; j++) {
				if (bitCount <= 0)
					break;
				if (bmap->bitmap[i][j] == 0) {
					bmap->bitmap[i][j] = 1;
					if (bitCount <= 0)
						break;
					bitCount--;
					freeBit--;
					ocupBit++;
					cylNo = i;
					trackNo = j / 4;
					secNo = j % 4;
					byteNo = cylNo;
					bitNo = trackNo * 4 + secNo;
					cout << "分配到的柱面号：" << cylNo << " " << "分配到的磁道号：" << trackNo << " " << "分配到的磁盘记录号：" << secNo << endl;
					cout << "字节号：" << byteNo << "\t" << "位号：" << bitNo << endl;
				}
				else {
					continue;
				}
			}

		}
		break;
		
	}
}

void recycleSpace(BitMap* bmap) {  //回收块
	int cylNo, trackNo, secNo;
	cout << "输入回收块的柱面号（0~7）：";
	cin >> cylNo;
	if (cylNo > 7 || cylNo < 0) {
		cout << "柱面号不存在！" << endl;
	}
	else {
		cout << "输入回收块的磁道号（0~1）：";
		cin >> trackNo;
		if (trackNo > 1 || trackNo < 0) {
			cout << "磁道号不存在！" << endl;
		}
		else {
			cout << "输入回收块的物理记录号（0~3）：";
			cin >> secNo;
			if (secNo > 3 || secNo < 0) {
				cout << "物理记录号不存在！" << endl;
			}
			else {
				if (bmap->bitmap[cylNo][trackNo * 4 + secNo] == 0) {
					cout << "此块尚未被分配，回收失败！" << endl;
				}
				else {
					bmap->bitmap[cylNo][trackNo * 4 + secNo] = 0;
					freeBit++;
					ocupBit--;
					int byteNo = cylNo;
					int bitNo = trackNo * 4 + secNo;
					cout << "回收成功！回收块的位置为：";
					cout << "字节号：" << byteNo << "\t" << "位号：" << bitNo << endl;
				}
			}
		}
	}
}

int main() {
	BitMap* bm = new BitMap();
	int choice;
	for (int i = 0; i < MAX; i++) {
		for (int j = 0; j < MAX; j++) {
			bm->bitmap[i][j] = 0;	
		}
	}
	initBitMap(bm);	
	while (true)
	{
		cout << "-----------------------------------" << endl; 
		cout << "请选择要进行的操作" << endl;
		cout << "1.分配磁盘空间" << "\t" << "2.回收磁盘空间" << "\t" << "3.查看位视图" << "\t" << "0.退出" << endl;
		cin >> choice;
		cout << "-----------------------------------" << endl;
		switch (choice)
		{
		case 1:
			cout << "-----------------------------------" << endl; //分配盘块
			allocSpace(bm);
			cout << "空闲盘块：" << freeBit << "个" << "\t" << "占用盘块：" << ocupBit << "个" << endl;
			cout << "-----------------------------------" << endl;
			break;
		case 2:
			cout << "-----------------------------------" << endl;  //回收指定盘块
			recycleSpace(bm);
			cout << "空闲盘块：" << freeBit << "个" << "\t" << "占用盘块：" << ocupBit << "个" << endl;
			cout << "-----------------------------------" << endl;
			break;
		case 3:
			cout << "-----------------------------------" << endl;    //遍历输出位视图
			cout << "位视图：" << endl;
			for (int i = 0; i < MAX; i++) {
				for (int j = 0; j < MAX; j++)
				{
					cout << bm->bitmap[i][j] << "\t";
				}
				cout << endl;
			}
			cout << "-----------------------------------" << endl;
			break;
		case 0:
			exit(0);
			break;
		default:
			cout << "-----------------------------------" << endl;
			cout << "无效！";
			cout << "-----------------------------------" << endl;
			break;
		}
	}
	return 0;
}

/*
用位示图管理磁盘存储空间
[提示]：
(1) 为了提高磁盘存储空间的利用率，可在磁盘上组织成链接文件、索引文件，这类文件可以把逻辑记录存放在不连续的存储空间。为了表示哪些磁盘空间已被占用，哪些磁盘空间是空闲的，可用位示图来指出。位示图由若干字节构成，每一位与磁盘上的一块对应，“1”状态表示相应块已占用，“0”状态表示该块为空闲。位示图的形式与实习二中的位示图一样，但要注意，对于主存储空间和磁盘存储空间应该用不同的位示图来管理，绝不可混用。
(2) 申请一块磁盘空间时，由分配程序查位示图，找出一个为“0”的位，计算出这一位对应块的磁盘物理地址，且把该位置成占用状态“1”。假设现在有一个盘组共8个柱面，每个柱面有2个磁道（盘面），每个磁道分成4个物理记录。那么，当在位示图中找到某一字节的某一位为“0”时，这个空闲块对应的磁盘物理地址为：
柱面号=字节号
磁道号= 位数 / 4
物理记录号= 位数%4
(3) 归还一块磁盘空间时，由回收程序根据归还的磁盘物理地址计算出归还块在位示图中的对应位，把该位置成“0”。按照（2）中假设的盘组，归还块在位示图中的位置计算如下：
字节号=柱面号
位数=磁道号*4+物理记录号
(4) 设计申请磁盘空间和归还磁盘空间的程序。
要求能接受来自键盘的空间申请及释放请求，要求能显示或打印程序运行前和运行后的位示图；分配时把分配到的磁盘空间的物理地址显示或打印出来，归还时把归还块对应于位示图的字节号和位数显示或打印出来。
*/
