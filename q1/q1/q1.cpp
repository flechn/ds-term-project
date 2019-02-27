#include<iostream>
#include<cmath>
using namespace std;

typedef struct PNode {
	float coef; // 系数
	int expn;   // 指数
	struct PNode *next;
}PNode, *Polynomial;

Polynomial CreatePNode(Polynomial, int); // 建立一元多项式
void Insert(Polynomial, Polynomial);     // 往多项式中插入结点
void Display(Polynomial);                // 输出多项式
int Compare(Polynomial, Polynomial);     // 控制Calculate()中的开关
void Result(Polynomial, float);          // 代入x值计算结果
Polynomial Calculate(Polynomial, Polynomial, char); // 代入数据进行运算

int main() {
	int whethercon = 1; // Whether continue
	do {
		Polynomial a = 0, b = 0, c = 0;
		int m, n;
		cout << "-------------------------------------------------------------------" << endl;
		cout << endl << "请输入第一个多项式：" << endl;
		cout << "请输入项数： ";
		cin >> m;
		a = CreatePNode(a, m);
		Display(a);
		cout << endl << "请输入第二个多项式： " << endl;
		cout << "请输入项数： ";
		cin >> n;
		b = CreatePNode(b, n);
		Display(b);

		char op;
		cout << endl << "请输入需要对两个多项式进行的操作，相加输入‘+’，相减输入‘-’： ";
		cin >> op;
		c = Calculate(a, b, op);
		Display(c);
		Result(c, 3.2);

		cout << endl << "继续运算请输入1，退出运算请输入0: ";
		cin >> whethercon;
		cout << endl;
	} while (whethercon);

	cout << "-------------------------------------------------------------------" << endl;
	cout << endl << "所有步骤均已完成。" << endl;
	return 0;
}

Polynomial CreatePNode(Polynomial head, int m) {
	// 建立一个头指针为head，项数为m的一元多项式
	int i;
	Polynomial p;
	p = head = new PNode;
	head->next = NULL;

	for (i = 0; i < m; i++) {
		p = new PNode;   // 建立新结点以接收数据
		cout << "请输入第" << i + 1 << "项的系数与指数： ";
		cin >> p->coef >> p->expn;
		Insert(p, head); // 调用Insert函数插入结点
	}
	return head;
}

void Insert(Polynomial p, Polynomial q) {
	if (p->coef == 0) delete p; // 系数为0则释放结点
	else {
		Polynomial p1, p2; // 用于循环查找插入位置的两个指针
		p1 = q;
		p2 = q->next;
		while (p2&&p->expn < p2->expn) {
			// 循环，q->next == NULL或待插入项的指数小于q->next->expn时为插入位置
			p1 = p2;
			p2 = p2->next; // 指针后移
		}
		if (p2&&p->expn == p2->expn) {
			// 当q->next指数与待插入项指数相同时，进行合并
			p2->coef += p->coef;
			delete p; // 插入完毕后释放结点
			if (!p2->coef) {
				// 系数为0则释放结点，删除该项
				p1->next = p2->next;
				delete p2;
			}
		}
		else {
			// 执行此步时，q->next == NULL && p->expn > p2->expn，为新的项，插入链表
			p->next = p2;
			p1->next = p;
		}
	}
}

void Display(Polynomial p) {
	// 输出多项式
	Polynomial Counter, pp; // Counter用于计算项数，pp用来输出各项值

	int n = 0;
	Counter = p;
	while (Counter) {
		n++;
		Counter = Counter->next;
	};
	cout << --n << " "; // 除去头结点

	pp = p->next;
	if (pp != NULL) do {
		cout << pp->coef << " " << pp->expn << " ";
		pp = pp->next;
	} while (pp);
	cout << endl;
}

int Compare(Polynomial a, Polynomial b) {
	if (a&&b) {
		if (!a || a->expn > b->expn) return 1;
		else if (!b || a->expn < b->expn) return -1;
		else return 0;
	}
	else if (!a&&b) return -1; // a多项式已空，但b多项式非空
	else return 1;             // b多项式已空，但a多项式非空
}

Polynomial Calculate(Polynomial a, Polynomial b, char op) {
	Polynomial qa = a->next;
	Polynomial qb = b->next;
	Polynomial headc, hc, qc;

	hc = new PNode; // 建立头结点
	hc->next = NULL;
	headc = hc;     // 存放头结点
	while (qa || qb) {
		qc = new PNode;
		switch (Compare(qa, qb)) {
		case 1: { // qc存放qa的元素
			qc->coef = qa->coef;
			qc->expn = qa->expn;
			qa = qa->next;
			break;
		}
		case 0: { // 进行系数的相加
			if (op == '+') qc->coef = qa->coef + qb->coef;
			else if (op == '-') qc->coef = qa->coef - qb->coef;
			qc->expn = qa->expn;
			qa = qa->next;
			qb = qb->next;
			break;
		}
		case -1: { // qc存放qb的元素
			qc->coef = qb->coef;
			qc->expn = qb->expn;
			qb = qb->next;
			break;
		}
		}

		if (qc->coef != 0) { // 插入元素，指针后移
			qc->next = hc->next;
			hc->next = qc;
			hc = qc;
		}
		else delete qc; // 系数为0则释放结点
	}
	return headc;
}

void Result(Polynomial p, float x) {
	float res = 0;     // 计算结果
	Polynomial pp; // pp作为指针
	pp = p->next;

	if (pp != NULL) do {
		res += pp->coef * pow(x, pp->expn);
		pp = pp->next;
	} while (pp);
	cout << endl << "x = " << x << " 时多项式的值为" << res << endl;
}

/*
以下为测试数据，可复制到运行中的命令行程序

3
2 1
5 8
-3.1 11
3
7 0
-5 8
11 9
+
1
4
6 -3
-1 1
4.4 2
-1.2 9
4
-6 -3
5.4 2
-1 2
7.8 15
-
1
6
1 0
1 1
1 2
1 3
1 4
1 5
2
-1 3
-1 4
+
1
2
1 1
1 3
2
-1 1
-1 3
+
1
3
1 1
1 2
1 3
0
+
0

*/