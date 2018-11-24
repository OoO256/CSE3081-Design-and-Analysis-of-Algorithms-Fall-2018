#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <fstream>
#include <cmath>
#include <time.h>
using namespace std;

FILE* input_file_stream;
FILE* output_file_stream;
// 입력, 출력할 파일의 stream
// 각 알고리즘 함수에서 읽고 쓸 수 있어야하므로 전역변수이다.

double clock_start, clock_end;
// 정렬 시작 클럭과 끝 클럭을 저장할 변수

class MinHeap
{
	// min heap 을 구현할 클래스

public:
	int size;
	// heap의 원소 개수
	vector<int>tree;
	// min heap을 구현한 트리
	// 1부터 시작하고, i의 자식은 2i, 2i+1로 참조한다
	MinHeap () : size (0) {};
	//기본생성자
	MinHeap (int _size) : tree (_size + 1), size (0) {};
	//크기가 주어진 생성자

	void build ();
	void push (const int e);
	int pop ();

};

void MinHeap::build () {
	// 트리를 받아 생성되었을 경우에 이를 min heap으로 변경하는 함수
	// 이 프로젝트에서는 사용하지 않았다
	for (int i = size / 2 + 1; i <= size; i++)
	{
		int curr = i;
		while (curr != 1)
		{
			if (tree[curr] < tree[curr / 2])
				swap (tree[curr / 2], tree[curr]);
		}
	}
}

void MinHeap::push (const int new_element) {
	// 트리에 원소를 추가하는 함수
	size++;
	if (tree.size () < size + 1)
	{
		tree.resize (size + 1);
	}
	// 벡터의 크기가 필요미만이면 resize한다

	tree[size] = new_element;
	// 마지막 원소에 e를 저장

	int curr = size;
	// 현제 새 원소의 위치
	while (curr != 1)
	{
		if (tree[curr] < tree[curr / 2])
			swap (tree[curr / 2], tree[curr]);
		else
			break;

		curr /= 2;
	}
	// 더 큰 부모가 없을 때까지 부모와 자식을 swap한다
}

int MinHeap::pop () {
	// min heap에서 원소를 pop하는 함수

	if (size == 0)
		return NULL;
	// 빈 트리인경우, 0을 출력한다

	const int top = tree[1];
	tree[1] = tree[size];
	size--;
	// root의 원소를 저장하고, 이를 마지막원소로 대체한다


	int curr = 1;
	// 현재 마지막 원소였던 원소가 있는 위치
	int next = curr;
	// 마지막 원소였던 원소가 다음으로 향할 위치
	while (curr * 2 <= size)
	{
		if (curr * 2 + 1 <= size && tree[curr * 2] > tree[curr * 2 + 1])
		{
			next = curr * 2 + 1;
		}
		else
		{
			next = curr * 2;
		}

		// next는 curr의 두 자식 중 최소를 가리킨다.
		// next가 curr 보다 작으면 둘을 swap한다
		// 그렇지 않으면 반복문을 종료한다.
		if (tree[curr] > tree[next]) {
			swap (tree[curr], tree[next]);
			curr = next;
		}
		else
		{
			break;
		}

	}
	return top;
}

void HeapSort (vector<int>& arr, const int len) {
	MinHeap mh (len);
	// MinHeap 객체를 선언한다

	for (int& e : arr)
	{
		mh.push (e);
	}
	// arr의 모든 원소를 push한다.

	for (int& e : arr)
	{
		e = mh.pop ();
	}
	// arr의 각 원소에 pop의 반환값을 저장한다.
}

void InsertionSort (vector<int>& arr, const int len) {

	// 삽입정렬한다.
	for (int i = 1; i < len; i++)
	{
		int for_insert = arr[i];
		// for_insert는 지금 삽입할 원소를 저장한다.

		int j = i;
		for (; j > 0; j--)
		{
			// j-1에 있는 원소를 j로 당겨가며
			// for_insert보다 작거나같은 첫 원소를 찾는다
			if (arr[j - 1] > for_insert)
			{
				arr[j] = arr[j - 1];
			}
			else
			{
				break;
			}
		}
		arr[j] = for_insert;
		// arr[j-1] <= for_insert < arr[j+1]
		// 이 성립하므로 for_insert는 j에 대입한다. 
	}
}

int Partition (vector<int>& arr, int left, int right, const int pivot) {

	// 주어진 배열의 [left, right]구간을, pviot보다 작은 원소와 작지 않은 원소로 분할한다.
	int first_not_small = left;
	// 변수 first_not_small는  pviot보다 작지 않을 수 있는 첫 원소의 인덱스를 저장한다.

	for (int j = left; j <= right; j++)
	{
		if (arr[j] < pivot)
		{
			swap (arr[first_not_small], arr[j]);
			first_not_small++;
			// pivot보다 작은 원소를 발견하면, first_not_small에 넣고
			// first_not_small은 1증가시킨다.
		}
	}
	// 주어진 배열의 [left, right]구간에서, pviot보다 작지 않은 첫 원소의 인덱스를 반환한다.
	return first_not_small;
}

void QuickSort (vector<int>& arr, const int left, const int right) {
	// 최적화 없는 Quick Sort

	if (left >= right)
		return;
	// left > right 이거나 left==right이면 divid 할 필요가 없으므로 종료  


	const int mid = Partition (arr, left, right - 1, arr[right]);
	swap (arr[mid], arr[right]);
	// arr[right]을 기준으로 Partition 수행

	QuickSort (arr, left, mid - 1);
	// [left, mid-1]에는 arr[right] 보다 작은 원소만이
	QuickSort (arr, mid + 1, right);
	// [mid+1, right]에는 arr[right] 보다 크거나 같은 원소만이 있다.
	// 분할, 정복한다.
}

void IntroSort (vector<int>& arr, int left, int right, int depth) {
	// Introspective Sort 를 하는 함수이다.
	// 여기에는 4가지 최적화 기법이 사용되었다.
	// 1. Tail Call Optimization
	// 2. IntroSpective Sort
	// 3. 3 - median pivot
	// 4. threshold 까지만 sort 하고 추후에 Insertion Sort 로 sort 하기

	while (right - left >= 32) {
		// 구간의 길이가 32를 넘어야 intro sort 한다
		// 아닌 경우 나중에 insertion sort
		if (depth == 0) {
			MinHeap mh (right - left + 1);
			for (int i = left; i <= right; i++)
			{
				mh.push (arr[i]);
			}
			for (int i = left; i <= right; i++)
			{
				arr[i] = mh.pop ();
			}
			return;

			// 만약 이 제귀함수의 깊이가 log(len)*2를 넘은 경우,
			// 이 구간은 heap sort 한다
			// pivot이 worst하게 설정되어 시간복잡도가 O(n^2)이 되는 것을 막기 위함이다
		}

		if (arr[left] > arr[(right + left) / 2])
			swap (arr[left], arr[(right + left) / 2]);
		if (arr[(right + left) / 2] > arr[right])
			swap (arr[(right + left) / 2], arr[right]);
		if (arr[left] > arr[(right + left) / 2])
			swap (arr[left], arr[(right + left) / 2]);

		// arr[left], arr[(left+right)/2], arr[right]을 소팅한다.
		// 셋의 중간값인 arr[(left + right) / 2]을 pivot으로 두고 Partition한다
		const int mid = Partition (arr, left, right, arr[(left + right) / 2]);

		if (mid > (left + right) / 2) {
			IntroSort (arr, mid, right, depth - 1);
			right = mid - 1;
		}
		else {
			IntroSort (arr, left, mid - 1, depth - 1);
			left = mid;
		}
		// Tail Call Optimization 이 적용되었다.
		// 둘 중 큰 구간은 이 함수에서 계속 작동하고,
		// 작은 구간은 새 함수상에서 작동한다. 
	}
}

void CountingSort (vector<int>& arr, const int element_min, const int element_max, const int len) {
	// count 배열에 각 원소가 얼마나 등장하는 지 저장하고 이를 순회한다.
	// O(element_max - element_min)만에 정렬할 수 있으나 공간을 많이 사용한다.

	vector<int> count (element_max - element_min + 1, 0);

	for (int i = 0; i < len; i++)
	{
		count[arr[i] - element_min]++;
	}
	// 등장 횟수를 저장

	int arri = 0;
	for (int i = 0; i < element_max - element_min + 1; i++)
	{
		while (count[i]--) {
			arr[arri] = i + element_min;
			arri++;
		}
	}
	// 등장한 만큼 배열에 다시 저장
}


int main (int argc, char *argv[]) {
	if (argc != 3)
	{
		printf ("Wrong number of arguments\n");
	}
	// 인수의 갯수를 검사한다

	string filename_in (argv[1]);

	string filename_out ("result___");
	filename_out[filename_out.size()-2] = argv[2][0];
	filename_out += filename_in;
	// 입, 출력파일의 이름을 string 객체에 저장한다
	// stirng 객체는 + 연산을 지원한다

	input_file_stream = fopen(filename_in.c_str(), "r");
	output_file_stream = fopen(filename_out.c_str(), "w");
	// 입력, 출력할 파일을 연다.

	fprintf(output_file_stream, "%s\n", filename_in.c_str());
	fprintf(output_file_stream, "%c\n", argv[2][0]);
	// 입력파일의 이름과 알고리즘의 인덱스를 미리 출력한다.



	int len;
	fscanf(input_file_stream, "%d", &len);
	// 입력의 길이를 저장

	int input_min = 2147483647;
	int input_max = -2147483648;
	vector<int>arr (len);
	for (int& i : arr)
	{
		fscanf(input_file_stream, "%d", &i);
		if (i < input_min)
			input_min = i;
		if (i > input_max)
			input_max = i;
	}
	// 입력은 arr벡터에 저장
	// 최소값과 최대값도 저장


	clock_start = clock ();
	// 시작시간 저장

	switch (argv[2][0] - '0')
	{
	case(1):
		InsertionSort (arr, len);
		break;
	case(2):
		QuickSort (arr, 0, len - 1);
		break;
	case(3):
		HeapSort (arr, len);
		break;
	case(4):

		// Counting Sort( O(max - min) ) 과 IntroSort ( O(n log(n)) ) 중 효율적인 것을 선택한다.

		// 또한 Counting Sort는 저장공간을 많이 차지하므로
		// 512MB 이상을 사용할 것으로 예상되는 경우 작동시키지 않았다.
		// max - min 의 크기가 2^27 이면 count 배열이 512MB를 차지한다.

		if (input_max - input_min < (int)min((long long)1 << 27, (long long)ceil(len*log2(len))))
		{
		CountingSort(arr, input_min, input_max, len);
		}
		else
		{
		IntroSort(arr, 0, len - 1, ceil(log2(len)));
		InsertionSort(arr, len);
		}
		break;
	}

	clock_end = clock ();


	fprintf(output_file_stream, "%d\n", len);
	fprintf(output_file_stream, "%lf\n", (clock_end - clock_start) / CLOCKS_PER_SEC);

	for (int& e : arr)
	{
		fprintf(output_file_stream, "%d ", e);
	}

	fclose(input_file_stream);
	fclose(output_file_stream);

	return 0;
}
