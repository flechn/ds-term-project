#include<iostream>
#include<cstdlib>
#include<string>
using namespace std;

#define STACK_INIT_SIZE 100 // 线性表存储空间的初始分配量
#define STACKINCREMENT 10   // 线性表存储空间的分配增量
#define MAXSIZE 1000

#define OK       1
#define ERROR    0
#define OVERFLOW -2

typedef struct {
	int x;
	int y; // 记录位置
}Pos;

typedef struct {
	Pos pos; // 当前位置
	int dir; // 当前方向
}Elem;

typedef struct SNode {
	Elem data;
	SNode *next; // 指向下一个节点
}SNode, *LinkStack;

int flag = 0; // flag用来标记是否路径全部走完 
int add[4][2] = { { 0,1 },{ 1,0 },{ 0,-1 },{ -1,0 } };   // 存储行进方向
int Maze[11][10] = { { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
{ 1, 0, 0, 1, 0, 0, 0, 1, 0, 1 },
{ 1, 0, 0, 1, 0, 0, 0, 1, 0, 1 },
{ 1, 0, 0, 0, 0, 1, 1, 0, 1, 1 },
{ 1, 0, 1, 1, 1, 0, 0, 1, 0, 1 },
{ 1, 0, 0, 0, 1, 0, 0, 0, 0, 1 },
{ 1, 0, 1, 0, 0, 0, 1, 0, 1, 1 },
{ 1, 0, 1, 1, 1, 1, 0, 0, 1, 1 },
{ 1, 1, 1, 0, 0, 0, 1, 0, 1, 1 },
{ 1, 1, 1, 0, 0, 0, 0, 0, 0, 1 },   // 迷宫的初始数据，可在此修改，
{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 } }; // 周围的1代表墙

									// 1. 实现一个链表作存储结构的栈类型，用非递归的方式求解迷宫
int InitStack(LinkStack &);     // 构造一个空栈S
int StackEmpty(LinkStack);      // 判断栈是否为空
int StackLength(LinkStack);     // 返回栈S的长度
int Push(LinkStack &, Elem);    // 元素入栈
int Pop(LinkStack &, Elem &);   // 元素出栈
void GetTop(LinkStack, Elem &); // 取栈S的栈顶元素
string ShowDir(int);            // 将int型的方向转换成汉字
void MazePath(Pos, Pos, int[11][10], int[4][2]); // 寻找通路核心算法

												 // 2. 编写递归形式的算法，求得迷宫中所有可能的通路
int Trace(int, int); // 递归查找出口
void Pace(int, int); // 以三元组(i, j, d)的形式输出通路

int main() {
	int i, j, k, l;  // i, j, k, l为二维数组下标变量

	for (i = 0; i < 11; i++) {
		for (j = 0; j < 10; j++) cout << Maze[i][j] << " ";
		cout << endl;
	} // 输出初始迷宫
	cout << "以上为初始迷宫" << endl;
	cout << "0表示通道，1表示墙，2表示路径。" << endl << endl;

	int Mazebk[11][10];
	for (i = 0; i < 11; i++) for (j = 0; j < 10; j++) Mazebk[i][j] = Maze[i][j]; // 储存原始迷宫

	cout << "1. 实现一个链表作存储结构的栈类型，用非递归的方式求解迷宫" << endl;
	LinkStack pace;
	InitStack(pace); // 记录路径

	Pos start, end;  // start,end入口和出口的坐标
	start.x = 1; start.y = 1;
	end.x = 9; end.y = 8; // 标记出入口的位置
	MazePath(start, end, Maze, add); // 寻找通路
	cout << endl;

	cout << "2. 编写递归形式的算法，求得迷宫中所有可能的通路" << endl;
	for (i = 0; i < 11; i++) for (j = 0; j < 10; j++) Maze[i][j] = Mazebk[i][j]; // 恢复原始迷宫

	if (Trace(1, 1) == 0) cout << "没有通路。" << endl; // 设置入口为(1, 1)
	else {
		for (k = 0; k < 11; k++) {
			for (l = 0; l < 10; l++) cout << Maze[k][l] << " ";
			cout << endl;
		} // 以矩阵形式输出迷宫及其通路
		cout << endl;
	}

	cout << "以下输出路径的方式不借用栈：" << endl;
	Pace(1, 1);
	cout << endl << "所有步骤均已完成。" << endl;
	return 0;
}

int InitStack(LinkStack &S) {
	// 构造一个空栈S
	S = (SNode *)malloc(sizeof(SNode));
	if (!S) {
		cerr << "InitStack()存储分配失败" << endl;
		exit(OVERFLOW);
	}
	S->next = NULL; // 指向下一个节点的指针为空
	return OK;
}

int StackEmpty(LinkStack S) {
	// 若栈为空栈，则返回1，否则返回0。
	if (!S->next) return 1;
	else return 0;
}

int StackLength(LinkStack S) {
	// 返回栈S的长度
	int i = 0;
	LinkStack p = S->next;
	while (p) {
		++i;
		p = p->next;
	}// 若p=NULL，则循环结束，i为栈长
	return i;
}

int Push(LinkStack &S, Elem e) {
	// 插入元素e为新的栈顶元素
	SNode *p;
	p = (SNode *)malloc(sizeof(SNode));
	if (!p) {
		cerr << "Push()存储分配失败" << endl;
		exit(OVERFLOW);
	}
	p->data = e; // 元素e进栈
	p->next = S->next;
	S->next = p; // 将p作为S当前的下一个节点
	return OK;
}

int Pop(LinkStack &S, Elem &e) {
	// 删除栈S的栈项元素，并用e返回其值
	SNode * p;
	p = S->next;
	if (StackEmpty(S)) {
		cout << "栈为空，无法进行出栈操作。" << endl;
		return ERROR;
	}
	S->next = p->next;
	e = p->data; // 用e返回栈顶元素值
	free(p); // 释放p的存储空间
	return OK;
}

void GetTop(LinkStack S, Elem &e) {
	// 取栈S的栈顶元素
	LinkStack p = S->next;
	if (p) e = p->data; // 返回栈顶元素
	else cerr << "栈为空，无法进行GetTop操作。" << endl;
}

string ShowDir(int a) {
	// 将int型的方向转换成汉字
	if (a == 0) return "东";
	else if (a == 1) return "南";
	else if (a == 2) return "西";
	else if (a == 3) return "北";
	else if (a == 4) return "终";
}

void MazePath(Pos start, Pos end, int maze[11][10], int diradd[4][2]) {
	int i, j, d; // 临时储存元素的x, y与方向
	int a, b;    // 临时储存元素的x, y
	Elem fin;    // 主要用于存放倒数第二个元素
	Elem elem, e;
	LinkStack S1, S2;
	InitStack(S1);
	InitStack(S2); // S2用于存放倒置的栈

	maze[start.x][start.y] = 2; // 标示入口已经走过
	elem.pos.x = start.x;
	elem.pos.y = start.y;
	elem.dir = -1; // 初始化
	Push(S1, elem);

	while (!StackEmpty(S1)) {
		// 有路可走
		Pop(S1, elem);
		i = elem.pos.x;
		j = elem.pos.y;
		d = elem.dir + 1; // 下一个方向
		while (d < 4) {
			// 以顺时针的顺序试探东南西北
			a = i + diradd[d][0];
			b = j + diradd[d][1];
			fin.dir = d;
			if (a == end.x && b == end.y && maze[a][b] == 0) {
				// 到达出口
				Push(S1, fin); // 将倒数第二个元素入栈
				elem.pos.x = a;
				elem.pos.y = b;
				elem.dir = 4; // 将4标记为出口
				Push(S1, elem);
				while (!StackEmpty(S1)) {
					// 逆置序列
					Pop(S1, e);
					Push(S2, e);
				}
				while (!StackEmpty(S2)) {
					// 输出迷宫路径
					Pop(S2, e);
					string Showdir;
					Showdir = ShowDir(e.dir);
					cout << "(" << e.pos.x << ", " << e.pos.y << ", " << Showdir << ")" << endl;
				}
				return; // 跳出循环
			}

			if (maze[a][b] == 0) {
				// 找到可以前进的非出口点
				maze[a][b] = 2; // 标记走过此点
				elem.pos.x = i;
				elem.pos.y = j;
				elem.dir = d;
				Push(S1, elem); // 当前位置入栈
				fin.pos.x = i = a;
				fin.pos.y = j = b; // 此处可用于存放出口前一个结点
				d = -1; // 下一点转化为当前点
			}
			d++;
		}
	}
	cout << "没有通路。" << endl;
}

int Trace(int x, int y) {
	Maze[x][y] = 2;
	if ((x == 9 && y == 8)) flag = 1; // 设置出口为(9,8)

	if (flag != 1 && Maze[x - 1][y] == 0) Trace(x - 1, y); // 判断向北是否有路
	if (flag != 1 && Maze[x][y + 1] == 0) Trace(x, y + 1); // 判断向东是否有路
	if (flag != 1 && Maze[x + 1][y] == 0) Trace(x + 1, y); // 判断向南是否有路
	if (flag != 1 && Maze[x][y - 1] == 0) Trace(x, y - 1); // 判断向西是否有路
	if (flag != 1) Maze[x][y] = 0; // 非通路时恢复为0

	return flag;
}

void Pace(int x, int y) {
	Maze[x][y] = 0;
	if (Maze[x - 1][y] == 2) {
		cout << "(" << x << ", " << y << ", 北)" << endl;
		Pace(x - 1, y);
	}
	else if (Maze[x][y + 1] == 2) {
		cout << "(" << x << ", " << y << ", 东)" << endl;
		Pace(x, y + 1);
	}
	else if (Maze[x + 1][y] == 2) {
		cout << "(" << x << ", " << y << ", 南)" << endl;
		Pace(x + 1, y);
	}
	else if (Maze[x][y - 1] == 2) {
		cout << "(" << x << ", " << y << ", 西)" << endl;
		Pace(x, y - 1);
	}
}