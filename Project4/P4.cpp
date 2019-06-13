//VS无法编译，仅在Ubuntu中编译运行！！！

//#include<iostream>
//#include<unistd.h>
//#include<sys/types.h>
//#include<sys/wait.h>
//using namespace std;
//int main() {
//	int count = 1;
//	int pid1, pid2;
//	cout << "father A pid:" << getpid() << endl;
//	pid1 = fork();
//	if (pid1 < 0)
//	{
//		cout << "error in fork!!" << endl;
//	}
//	else if (pid1 == 0)
//	{
//		cout << "father A pid:" << getppid() << endl;
//		cout << "son B pid:" << getpid() << endl;
//		count++;
//	}
//	else
//	{
//		int pid2 = fork();
//		if (pid2 < 0)
//		{
//			cout << "error in fork!!" << endl;
//		}
//		else if (pid2 == 0)
//		{
//			cout << "father A pid:" << getppid() << endl;
//			cout << "son C pid:" << getpid() << endl;
//			count++;
//		}
//		else
//		{
//			waitpid(pid1, NULL, 0);
//			waitpid(pid2, NULL, 0);
//			count++;
//			cout << "father A pid:" << getpid() << endl;
//		}
//	}
//	cout << count << endl;
//	return 0;
//}