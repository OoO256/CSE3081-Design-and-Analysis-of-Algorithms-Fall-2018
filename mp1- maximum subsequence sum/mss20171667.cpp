#include <cstdio>
#include <ctime>
#include <string>
#include <algorithm>
using namespace std;
const int INF = 987654321;
// 초기화를 위한 변수, 1000보다 커야한다
int arr[(1<<20)+5];
// 입력을 받을 배열

void mss_bf(FILE* in, FILE* out);
// mss를 brute force 알고리즘으로 구한다
void mss_dc(FILE* in, FILE* out);
// mss를 divide and conquer 알고리즘으로 구한다 
void mss_dp(FILE* in, FILE* out);
// mss를 dynamic programing 으로 구한다
void divide_conquer(int s, int e, int& mss, int& sub_start, int &sub_end);
// 구간[s, e]의 mss와 양 끝 인덱스를 구한다

int main(int argc, char *argv[]) {
	
	if(argc != 3)
	{
		printf("Wrong number of arguments\n");
	}
	// 인수의 갯수를 검사한다

	string in_filename(argv[1]);
	string out_filename("result_"+in_filename);
	// 입, 출력파일의 이름을 string 객체에 저장한다
	// stirng 객체는 + 연산을 지원한다

	FILE* in = fopen(in_filename.c_str(), "r");
	FILE* out = fopen(out_filename.c_str(), "w");
	// 입, 출력파일을 연다
	
	fprintf(out, "%s\n", in_filename.c_str());
	fprintf(out, "%c\n", argv[2][0]);
	// 입력파일의 이름과 알고리즘의 인덱스를 미리 출력한다.


	// 요구한 알고리즘의 인덱스는 문자열이므로 [0]번째 문자에서 '0'을 뺀다
	switch(argv[2][0]-'0')
	{
	case(1):
		mss_bf(in, out);
		break;
	case(2):
		mss_dc(in, out);
		break;
	case(3):
		mss_dp(in, out);
		break;
	default:
		printf("Wrong 3rd argument\n");
		break;
	}

}

void mss_bf(FILE* in, FILE* out) {
	// mss를 brute force 알고리즘으로 구한다
	int n;
	fscanf(in, "%d", &n);

	for(int i = 0; i < n; i++)
		fscanf(in, "%d", &arr[i]);
	// 입력을 저장한다.

	double before = clock();
	// 알고리즘 시작시간을 기록한다.

	int mss = -INF;
	// mss 를 저장할 변수
	int sub_start, sub_end;
	// mss의 인덱스 시작과 끝을 저장할 변수


	for(int i = 0; i < n; i++)
	{
		int sum = 0;
		// i에서 j 까지 더한 누적합을 구한다.
		for(int j = i; j < n; j++)
		{
			sum += arr[j];

			if(mss < sum)
			{
				// 이때 sum 이 mss이고 i, j 가 mss의 양 끝이다
				// < 비교를 해주어야 앞의 0은 포함되고 뒤의 0은 제외된다
				mss = sum;
				sub_start = i;
				sub_end = j;
			}
		}
	}

	double after = clock();
	// 끝난시각을 젠다

	fprintf(out, "%d\n", n);
	fprintf(out, "%d\n", sub_start);
	fprintf(out, "%d\n", sub_end);
	fprintf(out, "%d\n", mss);
	fprintf(out, "%lf\n", ((after-before)*1000)/CLOCKS_PER_SEC);
	// 결과를 출력한다
	// 시작과 끝의 클럭을 빼고 초당 클록으로 나눠주면 걸린 시간을 구할 수 있다
}


void mss_dc(FILE* in, FILE* out) {
	// mss를 divide and conquer 알고리즘으로 구한다 
	int n;
	fscanf(in, "%d", &n);

	for(int i = 0; i < n; i++)
		fscanf(in, "%d", &arr[i]);
	// 입력을 저장한다.


	double before = clock();
	// 알고리즘 시작시간을 기록한다.


	int mss = -INF;
	// mss 를 저장할 변수
	int sub_start, sub_end;
	// mss의 인덱스 시작과 끝을 저장할 변수


	divide_conquer(0, n-1, mss, sub_start, sub_end);
	// 0 부터 n-1까지의 인덱스에 대해 분할정복 함수를 호출한다.

	double after = clock();	
	// 끝난시각을 젠다

	fprintf(out, "%d\n", n);
	fprintf(out, "%d\n", sub_start);
	fprintf(out, "%d\n", sub_end);
	fprintf(out, "%d\n", mss);
	fprintf(out, "%lf\n", ((after-before)*1000)/CLOCKS_PER_SEC);
	// 결과를 출력한다
	// 시작과 끝의 클럭을 빼고 초당 클록으로 나눠주면 걸린 시간을 구할 수 있다
}


void divide_conquer(int s, int e, int& mss, int& sub_start, int &sub_end) {
	if(e-s == 0)
	{
		// e와 s가 같은경우 배열의 한 원소만을 보는 것 이므로 다음과 같이 처리한다.
		mss = arr[s];
		sub_start = s;
		sub_end = e;
		return;
	}

	int mss_left, mss_right;
	int start_left, start_right;
	int end_left, end_right;
	int mid = (s+e)/2;
	// [s, e]를 [s, mid], [mid+1, e]로 나누어 정복한다
	// 이때의 결과 mss, start, end를 저장하기 위한 변수들이다

	divide_conquer(s, mid, mss_left, start_left, end_left);
	divide_conquer(mid+1, e, mss_right, start_right, end_right);


	int mss_merge = arr[mid] + arr[mid+1];
	int start_merge = mid, end_merge = mid + 1;
	// 두 영역을 모두 포함하는 부분배열이 더 큰 mss를 가질 수 있다
	// 따라서 mid, mid+1을 포함하는 mss를 구한다

	// mid+1에서 시작하여, 오른쪽으로  배열를 누적해서 더하며 mss를 갱신한다.
	// brute force 알고리즘과 유사하다
	int sum = mss_merge;
	for(int i = mid+2; i <= e; i++)
	{
		sum += arr[i];
		if(mss_merge < sum)
		{
			// mss 뒤의 0은 포함되지 않아야 하므로 < 으로 비교하였다
			mss_merge = sum;
			end_merge = i;
		}
	}

	// mid에서 시작하여, 왼쪽으로  배열를 누적해서 더하며 mss를 갱신한다.
	// brute force 알고리즘과 유사하다
	sum = mss_merge;
	for(int i = mid-1; i >= s; i--)
	{
		sum += arr[i];
		if(mss_merge <= sum)
		{
			// mss 앞의 0은 포함되어야 하므로 <= 으로 비교하였다
			mss_merge = sum;
			start_merge = i;
		}
	}

	// left, right, merge중 큰 것을 구해 저장한다.
	// 우선 left로 초기화한다
	mss = mss_left;
	sub_start = start_left;
	sub_end = end_left;

	if(mss < mss_right)
	{
		// right가 더 큰 경우에만 갱신한다
		// 같은 경우에는 left가 인덱스가 더 작으므로 바꾸지 않는다
		mss = mss_right;
		sub_start = start_right;
		sub_end = end_right;
	}

	if(mss < mss_merge
		|| (mss == mss_merge && start_merge < sub_start))
	{
		// merge 한 것이 mss가 더 크거나,
		// mss는 같으나 시작점이 더 인덱스가 작으면 바꿔준다
		mss = mss_merge;
		sub_start = start_merge;
		sub_end = end_merge;
	}
}

void mss_dp(FILE* in, FILE* out) {
	// mss를 dynamic programing 으로 구한다
	int n;
	fscanf(in, "%d", &n);


	for(int i = 0; i < n; i++)
		fscanf(in, "%d", &arr[i]);
	// 입력을 저장한다.


	double before = clock();
	// 알고리즘 시작시간을 기록한다.


	int mss = -INF;
	// mss 를 저장할 변수
	int sub_start=0, sub_end, tmp_start = 0;
	// mss의 인덱스 시작과 끝을 저장할 변수
	// tmp_start는 임시적으로 지금 누적합을 구하고 있는 부분수열의 시작을 저장한다

	int localsum = 0;
	// 누적합이 0 이하인 이후부터의 부분누적합
	for(int i = 0; i < n; i++)
	{
		localsum += arr[i];
		if(mss < localsum)
		{
			// 부분누적합이 mss보다 크면 갱신한다
			mss = localsum;
			sub_end = i;
			sub_start = tmp_start;
		}
		if(localsum < 0)
		{
			// 부분누적합이 0 보다 작아지면 0으로 갱신하며
			// 이때 시작한 인덱스를 tmp_start에 저장해둔다
			localsum = 0;
			tmp_start = i+1;
		}

	}


	double after = clock();
	// 끝난시각을 젠다

	fprintf(out, "%d\n", n);
	fprintf(out, "%d\n", sub_start);
	fprintf(out, "%d\n", sub_end);
	fprintf(out, "%d\n", mss);
	fprintf(out, "%lf\n", ((after-before)*1000)/CLOCKS_PER_SEC);
	// 결과를 출력한다
	// 시작과 끝의 클럭을 빼고 초당 클록으로 나눠주면 걸린 시간을 구할 수 있다

}
