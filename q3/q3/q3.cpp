#include<iostream>
//#include<ctime>
using namespace std;

#define OK 1
#define ERROR 0
#define OVERFLOW -2

#define MAXSIZE 1000

// ����˳��洢�ṹ�ľ�̬���ұ�
typedef struct {
	int elem[MAXSIZE + 1];  // ����Ԫ�ش洢�ռ��ַ������ʱ��ʵ�ʳ��ȷ��䣬0�ŵ�Ԫ����
	int length; // ����
}SSTable;

class SST {
public:
	SST();
	~SST();
	int Create();
	int Search_Seq(int);
	int TraverseSST(int(*Visit)(int e));

	int RandomCreate();
	int reset();

	SSTable ST;
	SSTable temp;
};

SST::SST() {

}

SST::~SST() {
	// ����ɾ��ʱ�Զ������ٵ����á�
}

int SST::Create() {
	// ����һ����n������Ԫ�صľ�̬���ұ�ST��
	int n;
	cout << "����������Ԫ�صĸ���n��";
	cin >> n;

	for (int i = 1; i <= n; i++) cin >> ST.elem[i];
	ST.length = n;

	for (int i = 1; i <= n; i++) temp.elem[i] = ST.elem[i]; // ����
	temp.length = n;
	return OK;
}

int SST::Search_Seq(int key) {
	// ��˳���ST��˳�������ؼ��ֵ���key������Ԫ�ء����ҵ�������ֵΪ
	// ��Ԫ���ڱ��е�λ�ã�����Ϊ0��
	int i;
	ST.elem[0] = key;                            // ���ڱ���
	for (i = ST.length; ST.elem[i] != key; --i); // �Ӻ���ǰ��
	return i;                                    // �Ҳ���ʱ��iΪ0
}

int PrintElement(int e) {
	cout << e << " ";
	return OK;
}

int SST::TraverseSST(int(*Visit)(int e)) {
	for (int i = 1; i <= ST.length; i++) cout << ST.elem[i] << " ";
	cout << endl;
	return OK;
}

int SST::RandomCreate() {
	// �������һ����n������Ԫ�صľ�̬���ұ�ST��
	int n;
	cout << "���������������������Ԫ�صĸ���n��n < 1000������1000����";
	cin >> n;

	for (int i = 1; i <= n; i++) ST.elem[i] = temp.elem[i] = rand();
	ST.length = n;
	temp.length = n;
	return OK;
}

int SST::reset() {
	// ����ST
	for (int i = 1; i <= ST.length; i++) ST.elem[i] = temp.elem[i];
	return OK;
}

// ��������
void BubbleSort(SSTable &SST, int &comt, int &movt) {
	int i, j;
	int temp;
	int tag;

	for (i = SST.length; i >= 2; i--) {
		tag = false;        // tag��Ǳ��������Ƿ����Ľ���
		for (j = 1; j <= i - 1; j++) {
			if (SST.elem[j + 1] < SST.elem[j]) {
				temp = SST.elem[j + 1];
				SST.elem[j + 1] = SST.elem[j];
				SST.elem[j] = temp;

				movt += 3;
				tag = true; // ������������������˵�������Ѿ�����
			}
			comt++;
		}
		if (!tag) break;
	}
}

// ��������
void InsertSort(SSTable &SST, int &comt, int &movt) {
	// ��˳���SST��ֱ�Ӳ�������
	int i, j;
	for (i = 2; i <= SST.length; ++i) {
		if (SST.elem[i] < SST.elem[i - 1]) {
			// "<"���轫L.r[i]���������ӱ�
			SST.elem[0] = SST.elem[i];     // ����Ϊ�ڱ�
			SST.elem[i] = SST.elem[i - 1];
			for (j = i - 2; SST.elem[0] < SST.elem[j]; --j) {
				SST.elem[j + 1] = SST.elem[j]; // ��¼����
				movt++;
			}
			SST.elem[j + 1] = SST.elem[0]; // ���뵽��ȷλ��
		}
		comt++;
	}
}

// ��ѡ������
int SelectMinKey(SSTable SST, int i, int &comt, int &movt) {
	int min, k;
	min = i;
	SST.elem[0] = SST.elem[i];
	for (k = i; k <= SST.length; k++) {
		if (SST.elem[k] < SST.elem[0]) {
			min = k;
			SST.elem[0] = SST.elem[k];
		}
		comt++;
	}
	return min;
}

void SelectSort(SSTable &SST, int &comt, int &movt) {
	// ��˳���L����ѡ������
	int i, j;
	int temp;
	for (i = 1; i<SST.length; ++i) {
		// ѡ���i��С�ļ�¼����������λ
		j = SelectMinKey(SST, i, comt, movt); // ��L.r[i..L.length]��ѡ��key��С�ļ�¼
		if (i != j) {
			temp = SST.elem[i];
			SST.elem[i] = SST.elem[j];
			SST.elem[j] = temp;

			movt += 3;
		} // ���i����¼����
		comt++;
	}
}

// ��������
int Partition(SSTable &SST, int low, int high, int &comt, int &movt) {
	// ����˳���SST���ӱ�r[low..high]�ļ�¼�������¼��λ��������������λ�ã���ʱ
	// ����֮ǰ���󣩵ļ�¼������С��������
	SST.elem[0] = SST.elem[low];  // ���ӱ�ĵ�һ����¼�������¼
	int pivotkey = SST.elem[low]; // �����¼�ؼ���
	while (low < high) {
		while (low < high && SST.elem[high] >= pivotkey) {
			--high;
			comt++;
		}
		SST.elem[low] = SST.elem[high]; // ���������¼С�ļ�¼�Ƶ��Ͷ�

		while (low < high && SST.elem[low] <= pivotkey) {
			++low;
			comt++;
		}
		SST.elem[high] = SST.elem[low]; // ���������¼��ļ�¼�Ƶ��߶�

		movt += 2;
		comt++;
	}
	SST.elem[low] = SST.elem[0];        // �����¼��λ
	return low;                         // ��������λ��
}

void QSort(SSTable &SST, int low, int high, int &comt, int &movt) {
	// ��˳���L�е�������SST.elem[low..high]����������
	if (low < high) { // ���ȴ���1
		int pivotloc = Partition(SST, low, high, comt, movt); // ��L.r[low..high]һ��Ϊ��
		QSort(SST, low, pivotloc - 1, comt, movt);  // �Ե��ӱ�ݹ�����pivotloc������λ��
		QSort(SST, pivotloc + 1, high, comt, movt); // �Ը��ӱ�ݹ�����
	}
}

void QuickSort(SSTable &SST, int &comt, int &movt) {
	// ��˳���L����������
	QSort(SST, 1, SST.length, comt, movt);
}

// ϣ������
void ShellInsert(SSTable &SST, int dk, int &comt, int &movt) {
	// ��˳���L��һ��ϣ���������򡣱��㷨�Ǻ�һ��ֱ�Ӳ���������ȣ����������޸ģ�
	//     1.ǰ���¼λ�õ�������dk��������1��
	//     2.elem[0]ֻ���ݴ浥Ԫ�������ڱ�����j <= 0ʱ������λ�����ҵ���
	int i, j;
	for (i = dk + 1; i <= SST.length; ++i) {
		if (SST.elem[i] < SST.elem[i - dk]) {   // �轫SST.elem[i]�������������ӱ�
			SST.elem[0] = SST.elem[i];          // �ݴ���SST.elem[0]
			for (j = i - dk; j > 0 && SST.elem[0] < SST.elem[j]; j -= dk) {
				SST.elem[j + dk] = SST.elem[j]; // ��¼���ƣ����Ҳ���λ��
				movt++;
			}
			SST.elem[j + dk] = SST.elem[0];     // ����
		}
		comt++;
	}
}

void ShellSort(SSTable &SST, int dlta[], int t, int &comt, int &movt) {
	// ����������dlta[0..t - 1]��˳���L��ϣ������
	for (int k = 0; k < t; ++k)
		if (dlta[k]) ShellInsert(SST, dlta[k], comt, movt); // һ������Ϊdlta[k]�Ĳ�������
}

// ������
typedef SSTable HeapType; // �Ѳ���˳���洢��ʾ

void HeapAdjust(HeapType &H, int s, int m, int &comt, int &movt) {
	// ��֪H.r[s..m]�м�¼�Ĺؼ��ֳ�H.r[s].key֮�������ѵĶ��壬����������H.r[s]
	// �Ĺؼ��֣�ʹH.r[s..m]��Ϊһ���󶥶ѣ������м�¼�Ĺؼ��ֶ��ԣ�
	int rc = H.elem[s];
	for (int j = 2 * s; j <= m; j *= 2) {            // ��key�ϴ�ĺ��ӽ������ɸѡ
		if (j < m && H.elem[j] < H.elem[j + 1]) ++j; // jΪkey�ϴ�ļ�¼���±�
		if (!(rc < H.elem[j])) break;                // rcӦ������λ��s��
		H.elem[s] = H.elem[j];
		s = j;
		comt += 3;
	}
	H.elem[s] = rc; // ����
}

void swap(int &a, int &b, int &comt, int &movt) {
	int temp;
	temp = a;
	a = b;
	b = temp;
	movt += 3;
}

void HeapSort(HeapType &H, int &comt, int &movt) {
	// ��˳���H���ж�����
	for (int i = H.length / 2; i > 0; --i) // ��H.r[1..H.length]���ɴ󶥶�
		HeapAdjust(H, i, H.length, comt, movt);
	for (int i = H.length; i > 1; --i) {
		// ���Ѷ���¼�͵�ǰδ������������Hr[1..i]�����һ����¼�໥����
		// �ٽ�H.r[1..i - 1]���µ���Ϊ�󶥶�
		swap(H.elem[1], H.elem[i], comt, movt);
		HeapAdjust(H, 1, i - 1, comt, movt);
	}
}

// �鲢����
void Merge(int SR[], int TR[], int i, int m, int n, int &comt, int &movt) {
	// �������SR[i..m]��SR[m + 1..n]�鲢Ϊ�����TR[i..n]
	int j, k;
	for (j = m + 1, k = i; i <= m && j <= n; ++k) { // ��SR�м�¼��С����ز���TR
		if (SR[i] <= SR[j]) TR[k] = SR[i++];
		else TR[k] = SR[j++];
		comt++;
		movt++;
	}
	while (i <= m) TR[k++] = SR[i++]; // ��ʣ���SR[i..m]���Ƶ�TR
	while (j <= n) TR[k++] = SR[j++]; // ��ʣ���SR[j..n]���Ƶ�TR
}

void MSort(int SR[], int TR1[], int s, int t, int &comt, int &movt) {
	int TR2[MAXSIZE + 1];

	if (s == t) {
		TR1[s] = SR[s];
		comt++;
	}
	else {
		int m = (s + t) / 2;      // ��SR[s..t]ƽ��ΪSR[s..m]��SR[m + 1..t]
		MSort(SR, TR2, s, m, comt, movt);     // �ݹ�ؽ�SR[s..m]�鲢Ϊ�����TR2[s..m]
		MSort(SR, TR2, m + 1, t, comt, movt); // �ݹ�ؽ�SR[m + 1..t]�鲢Ϊ�����TR2[m + 1..t]
		Merge(TR2, TR1, s, m, t, comt, movt); // ��TR2[s..m]��TR2[m + 1..t]�鲢��TR1[s..t]
	}
}

void MergeSort(SSTable &SST, int &comt, int &movt) {
	// ��˳���SST���鲢����
	MSort(SST.elem, SST.elem, 1, SST.length, comt, movt);
}

// CompareSortTime
void CompareSortTime(SST ST2) {
	int dlta[100];
	int t = 3;
	cout << "������ϣ��������������������";
	cin >> t;
	cout << "������ϣ�������������У�";
	for (int i = 0; i < t; i++)cin >> dlta[i];
	dlta[t] = '\0';

	int comt = 0, movt = 0;
	BubbleSort(ST2.ST, comt, movt);
	cout << endl << "��������ؼ��ֲμӱȽϴ���Ϊ��" << comt << "���ƶ�����Ϊ��" << movt;

	ST2.reset();
	comt = 0, movt = 0;
	InsertSort(ST2.ST, comt, movt);
	cout << endl << "��������ؼ��ֲμӱȽϴ���Ϊ��" << comt << "���ƶ�����Ϊ��" << movt;

	ST2.reset();
	comt = 0, movt = 0;
	SelectSort(ST2.ST, comt, movt);
	cout << endl << "��ѡ������ؼ��ֲμӱȽϴ���Ϊ��" << comt << "���ƶ�����Ϊ��" << movt;

	ST2.reset();
	comt = 0, movt = 0;
	QuickSort(ST2.ST, comt, movt);
	cout << endl << "��������ؼ��ֲμӱȽϴ���Ϊ��" << comt << "���ƶ�����Ϊ��" << movt;

	ST2.reset();
	comt = 0, movt = 0;
	ShellSort(ST2.ST, dlta, t, comt, movt);
	cout << endl << "ϣ������ؼ��ֲμӱȽϴ���Ϊ��" << comt << "���ƶ�����Ϊ��" << movt;

	ST2.reset();
	comt = 0, movt = 0;
	HeapSort(ST2.ST, comt, movt);
	cout << endl << "������ؼ��ֲμӱȽϴ���Ϊ��" << comt << "���ƶ�����Ϊ��" << movt;

	ST2.reset();
	comt = 0, movt = 0;
	MergeSort(ST2.ST, comt, movt);
	cout << endl << "�鲢����ؼ��ֲμӱȽϴ���Ϊ��" << comt << "���ƶ�����Ϊ��" << movt << endl;;
}

// main
int main() {
	cout << "------------------------------����------------------------------" << endl;
	SST ST;
	ST.Create();

	int comt = 0, movt = 0; // ��¼�Ƚϴ������ƶ�����

	cout << endl << "����������������ʾ��" << endl;
	BubbleSort(ST.ST, comt, movt);
	ST.TraverseSST(PrintElement);

	ST.reset();
	cout << endl << "����������������ʾ��" << endl;
	InsertSort(ST.ST, comt, movt);
	ST.TraverseSST(PrintElement);

	ST.reset();
	cout << endl << "��ѡ��������������ʾ��" << endl;
	SelectSort(ST.ST, comt, movt);
	ST.TraverseSST(PrintElement);

	ST.reset();
	cout << endl << "����������������ʾ��" << endl;
	QuickSort(ST.ST, comt, movt);
	ST.TraverseSST(PrintElement);

	ST.reset();
	int dlta[100];
	int t = 3;
	cout << endl << "��������������������";
	cin >> t;
	cout << "�������������У�";
	for (int i = 0; i < t; i++)cin >> dlta[i];
	dlta[t] = '\0';

	cout << "ϣ��������������ʾ��" << endl;
	ShellSort(ST.ST, dlta, t, comt, movt);
	ST.TraverseSST(PrintElement);

	ST.reset();
	cout << endl << "��������������ʾ��" << endl;
	HeapSort(ST.ST, comt, movt);
	ST.TraverseSST(PrintElement);

	ST.reset();
	cout << endl << "�鲢������������ʾ��" << endl;
	MergeSort(ST.ST, comt, movt);
	ST.TraverseSST(PrintElement);

	cout << "----------------------------�������----------------------------" << endl << endl;

	for (int i = 0; i < 5; i++) {
		cout << "��" << i + 1 << "�αȽ�" << endl;
		cout << "----------------------������ɲ��ұ�ļ�ʱ----------------------" << endl;
		SST ST2;
		ST2.RandomCreate();   // �������һ����̬���ұ�
		CompareSortTime(ST2); // ��������ɵľ�̬���ұ�
		cout << "--------------------------�����ʱ����--------------------------" << endl << endl;
	}

	cout << endl << "���в��������ɡ�" << endl;
	return 0;
}

/*
����Ϊ�������ݣ��ɸ��Ƶ������е������г���

15
79 25 4 34 2 7 3 43 12 36 55 74 25 87 80
4
7 5 3 1
1000
10
500 200 100 50 25 15 10 5 3 1
1000
10
500 200 100 50 25 15 10 5 3 1
1000
10
500 200 100 50 25 15 10 5 3 1
1000
10
500 200 100 50 25 15 10 5 3 1
1000
10
500 200 100 50 25 15 10 5 3 1

*/