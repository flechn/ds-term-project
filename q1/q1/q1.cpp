#include<iostream>
#include<cmath>
using namespace std;

typedef struct PNode {
	float coef; // ϵ��
	int expn;   // ָ��
	struct PNode *next;
}PNode, *Polynomial;

Polynomial CreatePNode(Polynomial, int); // ����һԪ����ʽ
void Insert(Polynomial, Polynomial);     // ������ʽ�в�����
void Display(Polynomial);                // �������ʽ
int Compare(Polynomial, Polynomial);     // ����Calculate()�еĿ���
void Result(Polynomial, float);          // ����xֵ������
Polynomial Calculate(Polynomial, Polynomial, char); // �������ݽ�������

int main() {
	int whethercon = 1; // Whether continue
	do {
		Polynomial a = 0, b = 0, c = 0;
		int m, n;
		cout << "-------------------------------------------------------------------" << endl;
		cout << endl << "�������һ������ʽ��" << endl;
		cout << "������������ ";
		cin >> m;
		a = CreatePNode(a, m);
		Display(a);
		cout << endl << "������ڶ�������ʽ�� " << endl;
		cout << "������������ ";
		cin >> n;
		b = CreatePNode(b, n);
		Display(b);

		char op;
		cout << endl << "��������Ҫ����������ʽ���еĲ�����������롮+����������롮-���� ";
		cin >> op;
		c = Calculate(a, b, op);
		Display(c);
		Result(c, 3.2);

		cout << endl << "��������������1���˳�����������0: ";
		cin >> whethercon;
		cout << endl;
	} while (whethercon);

	cout << "-------------------------------------------------------------------" << endl;
	cout << endl << "���в��������ɡ�" << endl;
	return 0;
}

Polynomial CreatePNode(Polynomial head, int m) {
	// ����һ��ͷָ��Ϊhead������Ϊm��һԪ����ʽ
	int i;
	Polynomial p;
	p = head = new PNode;
	head->next = NULL;

	for (i = 0; i < m; i++) {
		p = new PNode;   // �����½���Խ�������
		cout << "�������" << i + 1 << "���ϵ����ָ���� ";
		cin >> p->coef >> p->expn;
		Insert(p, head); // ����Insert����������
	}
	return head;
}

void Insert(Polynomial p, Polynomial q) {
	if (p->coef == 0) delete p; // ϵ��Ϊ0���ͷŽ��
	else {
		Polynomial p1, p2; // ����ѭ�����Ҳ���λ�õ�����ָ��
		p1 = q;
		p2 = q->next;
		while (p2&&p->expn < p2->expn) {
			// ѭ����q->next == NULL����������ָ��С��q->next->expnʱΪ����λ��
			p1 = p2;
			p2 = p2->next; // ָ�����
		}
		if (p2&&p->expn == p2->expn) {
			// ��q->nextָ�����������ָ����ͬʱ�����кϲ�
			p2->coef += p->coef;
			delete p; // ������Ϻ��ͷŽ��
			if (!p2->coef) {
				// ϵ��Ϊ0���ͷŽ�㣬ɾ������
				p1->next = p2->next;
				delete p2;
			}
		}
		else {
			// ִ�д˲�ʱ��q->next == NULL && p->expn > p2->expn��Ϊ�µ����������
			p->next = p2;
			p1->next = p;
		}
	}
}

void Display(Polynomial p) {
	// �������ʽ
	Polynomial Counter, pp; // Counter���ڼ���������pp�����������ֵ

	int n = 0;
	Counter = p;
	while (Counter) {
		n++;
		Counter = Counter->next;
	};
	cout << --n << " "; // ��ȥͷ���

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
	else if (!a&&b) return -1; // a����ʽ�ѿգ���b����ʽ�ǿ�
	else return 1;             // b����ʽ�ѿգ���a����ʽ�ǿ�
}

Polynomial Calculate(Polynomial a, Polynomial b, char op) {
	Polynomial qa = a->next;
	Polynomial qb = b->next;
	Polynomial headc, hc, qc;

	hc = new PNode; // ����ͷ���
	hc->next = NULL;
	headc = hc;     // ���ͷ���
	while (qa || qb) {
		qc = new PNode;
		switch (Compare(qa, qb)) {
		case 1: { // qc���qa��Ԫ��
			qc->coef = qa->coef;
			qc->expn = qa->expn;
			qa = qa->next;
			break;
		}
		case 0: { // ����ϵ�������
			if (op == '+') qc->coef = qa->coef + qb->coef;
			else if (op == '-') qc->coef = qa->coef - qb->coef;
			qc->expn = qa->expn;
			qa = qa->next;
			qb = qb->next;
			break;
		}
		case -1: { // qc���qb��Ԫ��
			qc->coef = qb->coef;
			qc->expn = qb->expn;
			qb = qb->next;
			break;
		}
		}

		if (qc->coef != 0) { // ����Ԫ�أ�ָ�����
			qc->next = hc->next;
			hc->next = qc;
			hc = qc;
		}
		else delete qc; // ϵ��Ϊ0���ͷŽ��
	}
	return headc;
}

void Result(Polynomial p, float x) {
	float res = 0;     // ������
	Polynomial pp; // pp��Ϊָ��
	pp = p->next;

	if (pp != NULL) do {
		res += pp->coef * pow(x, pp->expn);
		pp = pp->next;
	} while (pp);
	cout << endl << "x = " << x << " ʱ����ʽ��ֵΪ" << res << endl;
}

/*
����Ϊ�������ݣ��ɸ��Ƶ������е������г���

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