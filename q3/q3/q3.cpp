#include<iostream>
//#include<ctime>
using namespace std;

#define OK 1
#define ERROR 0
#define OVERFLOW -2

#define MAXSIZE 1000

// 建立顺序存储结构的静态查找表
typedef struct {
	int elem[MAXSIZE + 1];  // 数据元素存储空间基址，建表时按实际长度分配，0号单元留空
	int length; // 表长度
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
	// 对象删除时自动起到销毁的作用。
}

int SST::Create() {
	// 构造一个含n个数据元素的静态查找表ST。
	int n;
	cout << "请输入数据元素的个数n：";
	cin >> n;

	for (int i = 1; i <= n; i++) cin >> ST.elem[i];
	ST.length = n;

	for (int i = 1; i <= n; i++) temp.elem[i] = ST.elem[i]; // 复制
	temp.length = n;
	return OK;
}

int SST::Search_Seq(int key) {
	// 在顺序表ST中顺序查找其关键字等于key的数据元素。若找到，则函数值为
	// 该元素在表中的位置，否则为0。
	int i;
	ST.elem[0] = key;                            // “哨兵”
	for (i = ST.length; ST.elem[i] != key; --i); // 从后往前找
	return i;                                    // 找不到时，i为0
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
	// 随机构造一个含n个数据元素的静态查找表ST。
	int n;
	cout << "请输入需随机创建的数据元素的个数n（n < 1000，建议1000）：";
	cin >> n;

	for (int i = 1; i <= n; i++) ST.elem[i] = temp.elem[i] = rand();
	ST.length = n;
	temp.length = n;
	return OK;
}

int SST::reset() {
	// 重置ST
	for (int i = 1; i <= ST.length; i++) ST.elem[i] = temp.elem[i];
	return OK;
}

// 起泡排序
void BubbleSort(SSTable &SST, int &comt, int &movt) {
	int i, j;
	int temp;
	int tag;

	for (i = SST.length; i >= 2; i--) {
		tag = false;        // tag标记遍历过程是否发生的交换
		for (j = 1; j <= i - 1; j++) {
			if (SST.elem[j + 1] < SST.elem[j]) {
				temp = SST.elem[j + 1];
				SST.elem[j + 1] = SST.elem[j];
				SST.elem[j] = temp;

				movt += 3;
				tag = true; // 若遍历不发生交换，说明序列已经有序
			}
			comt++;
		}
		if (!tag) break;
	}
}

// 插入排序
void InsertSort(SSTable &SST, int &comt, int &movt) {
	// 对顺序表SST作直接插入排序
	int i, j;
	for (i = 2; i <= SST.length; ++i) {
		if (SST.elem[i] < SST.elem[i - 1]) {
			// "<"，需将L.r[i]插入有序子表
			SST.elem[0] = SST.elem[i];     // 复制为哨兵
			SST.elem[i] = SST.elem[i - 1];
			for (j = i - 2; SST.elem[0] < SST.elem[j]; --j) {
				SST.elem[j + 1] = SST.elem[j]; // 记录后移
				movt++;
			}
			SST.elem[j + 1] = SST.elem[0]; // 插入到正确位置
		}
		comt++;
	}
}

// 简单选择排序
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
	// 对顺序表L作简单选择排序
	int i, j;
	int temp;
	for (i = 1; i<SST.length; ++i) {
		// 选择第i个小的记录，并交换到位
		j = SelectMinKey(SST, i, comt, movt); // 在L.r[i..L.length]中选择key最小的记录
		if (i != j) {
			temp = SST.elem[i];
			SST.elem[i] = SST.elem[j];
			SST.elem[j] = temp;

			movt += 3;
		} // 与第i个记录交换
		comt++;
	}
}

// 快速排序
int Partition(SSTable &SST, int low, int high, int &comt, int &movt) {
	// 交换顺序表SST中子表r[low..high]的记录，枢轴记录到位，并返回其所在位置，此时
	// 在它之前（后）的记录均不大（小）于它。
	SST.elem[0] = SST.elem[low];  // 用子表的第一个记录作枢轴记录
	int pivotkey = SST.elem[low]; // 枢轴记录关键字
	while (low < high) {
		while (low < high && SST.elem[high] >= pivotkey) {
			--high;
			comt++;
		}
		SST.elem[low] = SST.elem[high]; // 将比枢轴记录小的记录移到低端

		while (low < high && SST.elem[low] <= pivotkey) {
			++low;
			comt++;
		}
		SST.elem[high] = SST.elem[low]; // 将比枢轴记录大的记录移到高端

		movt += 2;
		comt++;
	}
	SST.elem[low] = SST.elem[0];        // 枢轴记录到位
	return low;                         // 返回枢轴位置
}

void QSort(SSTable &SST, int low, int high, int &comt, int &movt) {
	// 对顺序表L中的子序列SST.elem[low..high]作快速排序
	if (low < high) { // 长度大于1
		int pivotloc = Partition(SST, low, high, comt, movt); // 将L.r[low..high]一分为二
		QSort(SST, low, pivotloc - 1, comt, movt);  // 对低子表递归排序，pivotloc是枢轴位置
		QSort(SST, pivotloc + 1, high, comt, movt); // 对高子表递归排序
	}
}

void QuickSort(SSTable &SST, int &comt, int &movt) {
	// 对顺序表L作快速排序
	QSort(SST, 1, SST.length, comt, movt);
}

// 希尔排序
void ShellInsert(SSTable &SST, int dk, int &comt, int &movt) {
	// 对顺序表L作一趟希尔插入排序。本算法是和一趟直接插入排序相比，作了以下修改：
	//     1.前后记录位置的增量是dk，而不是1；
	//     2.elem[0]只是暂存单元，不是哨兵。当j <= 0时，插入位置已找到。
	int i, j;
	for (i = dk + 1; i <= SST.length; ++i) {
		if (SST.elem[i] < SST.elem[i - dk]) {   // 需将SST.elem[i]插入有序增量子表
			SST.elem[0] = SST.elem[i];          // 暂存在SST.elem[0]
			for (j = i - dk; j > 0 && SST.elem[0] < SST.elem[j]; j -= dk) {
				SST.elem[j + dk] = SST.elem[j]; // 记录后移，查找插入位置
				movt++;
			}
			SST.elem[j + dk] = SST.elem[0];     // 插入
		}
		comt++;
	}
}

void ShellSort(SSTable &SST, int dlta[], int t, int &comt, int &movt) {
	// 按增量序列dlta[0..t - 1]对顺序表L作希尔排序。
	for (int k = 0; k < t; ++k)
		if (dlta[k]) ShellInsert(SST, dlta[k], comt, movt); // 一趟增量为dlta[k]的插入排序
}

// 堆排序
typedef SSTable HeapType; // 堆采用顺序表存储表示

void HeapAdjust(HeapType &H, int s, int m, int &comt, int &movt) {
	// 已知H.r[s..m]中记录的关键字除H.r[s].key之外均满足堆的定义，本函数调整H.r[s]
	// 的关键字，使H.r[s..m]成为一个大顶堆（对其中记录的关键字而言）
	int rc = H.elem[s];
	for (int j = 2 * s; j <= m; j *= 2) {            // 沿key较大的孩子结点向下筛选
		if (j < m && H.elem[j] < H.elem[j + 1]) ++j; // j为key较大的记录的下标
		if (!(rc < H.elem[j])) break;                // rc应插入在位置s上
		H.elem[s] = H.elem[j];
		s = j;
		comt += 3;
	}
	H.elem[s] = rc; // 插入
}

void swap(int &a, int &b, int &comt, int &movt) {
	int temp;
	temp = a;
	a = b;
	b = temp;
	movt += 3;
}

void HeapSort(HeapType &H, int &comt, int &movt) {
	// 对顺序表H进行堆排序。
	for (int i = H.length / 2; i > 0; --i) // 把H.r[1..H.length]建成大顶堆
		HeapAdjust(H, i, H.length, comt, movt);
	for (int i = H.length; i > 1; --i) {
		// 将堆顶记录和当前未经排序子序列Hr[1..i]中最后一个记录相互交换
		// 再将H.r[1..i - 1]重新调整为大顶堆
		swap(H.elem[1], H.elem[i], comt, movt);
		HeapAdjust(H, 1, i - 1, comt, movt);
	}
}

// 归并排序
void Merge(int SR[], int TR[], int i, int m, int n, int &comt, int &movt) {
	// 将有序的SR[i..m]和SR[m + 1..n]归并为有序的TR[i..n]
	int j, k;
	for (j = m + 1, k = i; i <= m && j <= n; ++k) { // 将SR中记录由小到大地并入TR
		if (SR[i] <= SR[j]) TR[k] = SR[i++];
		else TR[k] = SR[j++];
		comt++;
		movt++;
	}
	while (i <= m) TR[k++] = SR[i++]; // 将剩余的SR[i..m]复制到TR
	while (j <= n) TR[k++] = SR[j++]; // 将剩余的SR[j..n]复制到TR
}

void MSort(int SR[], int TR1[], int s, int t, int &comt, int &movt) {
	int TR2[MAXSIZE + 1];

	if (s == t) {
		TR1[s] = SR[s];
		comt++;
	}
	else {
		int m = (s + t) / 2;      // 将SR[s..t]平分为SR[s..m]和SR[m + 1..t]
		MSort(SR, TR2, s, m, comt, movt);     // 递归地将SR[s..m]归并为有序的TR2[s..m]
		MSort(SR, TR2, m + 1, t, comt, movt); // 递归地将SR[m + 1..t]归并为有序的TR2[m + 1..t]
		Merge(TR2, TR1, s, m, t, comt, movt); // 将TR2[s..m]和TR2[m + 1..t]归并到TR1[s..t]
	}
}

void MergeSort(SSTable &SST, int &comt, int &movt) {
	// 对顺序表SST作归并排序
	MSort(SST.elem, SST.elem, 1, SST.length, comt, movt);
}

// CompareSortTime
void CompareSortTime(SST ST2) {
	int dlta[100];
	int t = 3;
	cout << "请输入希尔排序增量序列容量：";
	cin >> t;
	cout << "请输入希尔排序增量序列：";
	for (int i = 0; i < t; i++)cin >> dlta[i];
	dlta[t] = '\0';

	int comt = 0, movt = 0;
	BubbleSort(ST2.ST, comt, movt);
	cout << endl << "起泡排序关键字参加比较次数为：" << comt << "，移动次数为：" << movt;

	ST2.reset();
	comt = 0, movt = 0;
	InsertSort(ST2.ST, comt, movt);
	cout << endl << "插入排序关键字参加比较次数为：" << comt << "，移动次数为：" << movt;

	ST2.reset();
	comt = 0, movt = 0;
	SelectSort(ST2.ST, comt, movt);
	cout << endl << "简单选择排序关键字参加比较次数为：" << comt << "，移动次数为：" << movt;

	ST2.reset();
	comt = 0, movt = 0;
	QuickSort(ST2.ST, comt, movt);
	cout << endl << "快速排序关键字参加比较次数为：" << comt << "，移动次数为：" << movt;

	ST2.reset();
	comt = 0, movt = 0;
	ShellSort(ST2.ST, dlta, t, comt, movt);
	cout << endl << "希尔排序关键字参加比较次数为：" << comt << "，移动次数为：" << movt;

	ST2.reset();
	comt = 0, movt = 0;
	HeapSort(ST2.ST, comt, movt);
	cout << endl << "堆排序关键字参加比较次数为：" << comt << "，移动次数为：" << movt;

	ST2.reset();
	comt = 0, movt = 0;
	MergeSort(ST2.ST, comt, movt);
	cout << endl << "归并排序关键字参加比较次数为：" << comt << "，移动次数为：" << movt << endl;;
}

// main
int main() {
	cout << "------------------------------排序------------------------------" << endl;
	SST ST;
	ST.Create();

	int comt = 0, movt = 0; // 记录比较次数和移动次数

	cout << endl << "起泡排序结果如下所示：" << endl;
	BubbleSort(ST.ST, comt, movt);
	ST.TraverseSST(PrintElement);

	ST.reset();
	cout << endl << "插入排序结果如下所示：" << endl;
	InsertSort(ST.ST, comt, movt);
	ST.TraverseSST(PrintElement);

	ST.reset();
	cout << endl << "简单选择排序结果如下所示：" << endl;
	SelectSort(ST.ST, comt, movt);
	ST.TraverseSST(PrintElement);

	ST.reset();
	cout << endl << "快速排序结果如下所示：" << endl;
	QuickSort(ST.ST, comt, movt);
	ST.TraverseSST(PrintElement);

	ST.reset();
	int dlta[100];
	int t = 3;
	cout << endl << "请输入增量序列容量：";
	cin >> t;
	cout << "请输入增量序列：";
	for (int i = 0; i < t; i++)cin >> dlta[i];
	dlta[t] = '\0';

	cout << "希尔排序结果如下所示：" << endl;
	ShellSort(ST.ST, dlta, t, comt, movt);
	ST.TraverseSST(PrintElement);

	ST.reset();
	cout << endl << "堆排序结果如下所示：" << endl;
	HeapSort(ST.ST, comt, movt);
	ST.TraverseSST(PrintElement);

	ST.reset();
	cout << endl << "归并排序结果如下所示：" << endl;
	MergeSort(ST.ST, comt, movt);
	ST.TraverseSST(PrintElement);

	cout << "----------------------------排序结束----------------------------" << endl << endl;

	for (int i = 0; i < 5; i++) {
		cout << "第" << i + 1 << "次比较" << endl;
		cout << "----------------------随机生成查找表的计时----------------------" << endl;
		SST ST2;
		ST2.RandomCreate();   // 随机创建一个静态查找表
		CompareSortTime(ST2); // 对随机生成的静态查找表
		cout << "--------------------------随机计时结束--------------------------" << endl << endl;
	}

	cout << endl << "所有步骤均已完成。" << endl;
	return 0;
}

/*
以下为测试数据，可复制到运行中的命令行程序

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