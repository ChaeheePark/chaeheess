#include <iostream>
#include <string.h>
#include <chrono>
#pragma warning (disable:4996)
#define SIZE 1024
using namespace std;

int i, j, k; // 반복문과 함수안에서 자주 쓰는 변수들을 전역변수로 선언
static int insertion_count = 0, heap_count = 0, merge_count = 0, quick_count_1 = 0, quick_count_2 = 0, quick_count_3 = 0;

// swap 함수 구현
void swap(int* a, int* b) {
	int temp = *a;
	*a = *b;
	*b = temp;
}


// 배열 프린트하는 함수 구현
void arrPrint(int* arr) {
	for (int i = 0; i < SIZE; i++) {
		cout << arr[i] << " ";
	}
	cout << "\n\n";
}


//random으로 값 생성해주는 함수
int randomNum(int adr) {
	static int count = 0;
	int random = 8253729 * adr + 2396403 + count;
	count++;
	if (random < 0) random  = -random;
	return random % SIZE;
}
	

int randomPivot(int adr,int start, int end) {
	static int count = 0;
	int random = 8253729 * adr + 2396403 + count;
	count++;
	if (random < 0) random = -random;
	random = random % (end - start) + start;
	//cout << random << "\n";
	return random;
}

// Insertion sort
void insertionSort(int* arr) {
	for (i=1; i < SIZE ; i++) { // 1부터 마지막 인덱스까지
		int key=arr[i]; // key는 현재 i의 키값
		j = i - 1;// i-1까지는 정렬된 배열 -> arr[i]의 값을 앞의 정렬된 배열에 삽입
		
		while (j >= 0 && arr[j] > key) { // j는 0보다 작으면 배열이탈하므로 && j번째 원소가 arr[i]보다 크면 자리바꿈
			arr[j + 1] = arr[j];
			j--;
			insertion_count++;
		}
		if (j != -1) insertion_count++;
		//비교 후 count증가
		arr[j + 1] = key;
	}
}

// Heapsort
void heapSort(int* arr) {
	for (i = 1; i < SIZE; i++) {
		int child = i;
		do {
			int root = (child - 1) / 2; // 자식을 토대로 부모를 생성
			if (arr[root] < arr[child]) { // 자식의 값이 더 크면 값을 서로 바꿔줌
				swap(&arr[root], &arr[child]);
			}
			heap_count++; // 비교후 count 증가
			child = root; // 자식의 값이 부모가 되어 root에 있는 값이 최대가 되게 반복적으로 수행
		} while (child != 0); // 최대 heap구조만듬
	}
	for (i = SIZE - 1; i >= 0; i--) {
		swap(&arr[0], &arr[i]); // 마지막 원소와 첫번째 원소의 자리를 바꿈 -> 마지막 자리에 계속 들어가게 되어 오름차순 배열
		int root = 0;
		int child = 1;
		do {
			child = root * 2 + 1;
			if (child < i - 1 && arr[child] < arr[child + 1]) { // 자식중에 더 큰값을 찾음
				child++;
			}
			heap_count++; // 자식 원소간의 비교 후 count 증가
			if (child < i && arr[child] > arr[root]) { // 자식의 값이 더 크다면 부모와 자식을 바꿈
				swap(&arr[child], &arr[root]);
			}
			heap_count++; // 자식원소와 부모원소 비교 후 count 증가
			root = child; // 자식의 값을 부모로 넣어줌
		} while (child < i); // root의 값은 자식보다 작을 수 있음 -> 다시 heap구조를 만들어줌
	}
}

// Mergesort
int sorted[SIZE];// 공간의 낭비를 줄이기 위해 전역변수로 선언
void merge(int* arr, int m, int middle, int n) {
	i = m; // 첫번째 부분배열의 시작
	j = middle + 1; /// 두번째 부분배열의 시작
	k = m; // sorted 배열의 index 
	while (i <= middle && j <= n) {
		// 원소들 비교해서 작은 원소 찾아서 sorted배열에 넣기
		if (arr[i] <= arr[j]) {
			sorted[k] = arr[i];
			i++;
		}
		else {
			sorted[k] = arr[j];
			j++;
		}
		k++;
		merge_count++; // 원소들 비교 후 count 증가
	}
	if (i > middle) { //i에 있는 배열의 원소를 다 넣어 먼저 끝났을때
		for (int x = j; x <= n; x++) {
			sorted[k] = arr[x];
			k++;
		}
	}
	else { //j에 있는 배열의 원소를 다 넣어 먼저 끝났을때 남은 데이터를 삽입
		for (int x = i; x <= middle; x++) {
			sorted[k] = arr[x];
			k++;
		}
	}
	for (int x = m; x <= n; x++) { //실제배열로 옮겨주기
		arr[x] = sorted[x];
	}
}

void mergeSort(int* arr, int m, int n) {
	if (m < n) { // 배열의 크기가 1보다 큰 경우 계속 부르기
		int middle = (m + n) / 2;
		mergeSort(arr, m, middle);
		mergeSort(arr, middle + 1, n);
		merge(arr, m, middle, n); //merge해주는 과정
	}
}


//quick sort_version1
int partition_1(int* arr, int start, int end) {
	int pivot = end; // pivot이 end값에 가도록
	i = start - 1;

	for (int j = start; j <= pivot - 1; j++) {
		if (arr[j] <= arr[pivot]) {
			i++;
			swap(&arr[i], &arr[j]);
		}
		quick_count_1++; //pivot이랑 비교 후 count 증가
	}
	swap(&arr[i + 1], &arr[pivot]);

	return i + 1;
}

void quickSort_1(int* arr, int start, int end) {
	if (start < end) {
		int m = partition_1(arr, start, end);
		quickSort_1(arr, start, m - 1);
		quickSort_1(arr, m + 1, end);
	}
}


//quick sort_version2
int partition_2(int* arr, int start, int end) {
	int num;
	i = start - 1;
	int pivot = randomPivot((int)&num,start,end);
	if (pivot != end) {
		swap(&arr[pivot], &arr[end]);
		pivot = end;
	}

	for (int j = start; j <= pivot - 1; j++) {
		if (arr[j] <= arr[pivot]) {
			i++;
			swap(&arr[i], &arr[j]);
		}
		quick_count_2++; //pivot이랑 비교 후 count 증가
	}
	swap(&arr[i + 1], &arr[pivot]);

	return i + 1;

}
void quickSort_2(int* arr, int start, int end) {
	if (start < end) {
		int m=partition_2(arr, start, end);
		quickSort_2(arr, start, m-1);
		quickSort_2(arr, m+1, end); 
	}
}

//quick sort_version3
int partition_3(int* arr, int start, int end) {
	int mid = (start + end) / 2;
	int pivot = 0;
	if (arr[mid] > arr[start]) {
		if (arr[mid] < arr[end]) {
			pivot = mid;
		}
		else {
			if (arr[start] > arr[end]) {
				pivot = start;
			}
			else {
				pivot = end;
			}
		}
	}
	else {
		if (arr[start] < arr[end]) {
			pivot = start;
		}
		else {
			if (arr[mid] > arr[end]) {
				pivot = mid;
			}
			else {
				pivot = end;
			}
		}
	}

	swap(&arr[pivot], &arr[end]);
	pivot = end;// pivot이 end값에 가도록
	i = start - 1;

	for (int j = start; j <= pivot -1; j++) {
		if (arr[j] <= arr[pivot]) {
			i++;
			swap(&arr[i], &arr[j]);
		}
		quick_count_3++;
	}

	swap(&arr[i + 1], &arr[pivot]);

	return i + 1;

}
void quickSort_3(int* arr, int start, int end) {
	if (start < end) {
		int m = partition_3(arr, start, end);
		quickSort_3(arr, start, m - 1);
		quickSort_3(arr, m + 1, end);
	}
}


int main() {
	int start = 0; int end =SIZE-1;
	
	int arr[SIZE]; //애초에 배열에 주소값이 랜덤으로 배정
	for (int i = 0; i < SIZE; i++) {
		arr[i] = randomNum((int)&arr[i]);
	}
	
	int arr1[SIZE], arr2[SIZE], arr3[SIZE], arr4[SIZE], arr5[SIZE], arr6[SIZE];

	// insertionSort
	// arr의 값 arr1에 복사
	memcpy(arr1, arr, (sizeof(int)) * SIZE);
	
	//시간 측정 및 sort실행
	auto start_time1 = chrono::high_resolution_clock::now();
	insertionSort(arr1);
	auto end_time1 = chrono::high_resolution_clock::now();

	//print
	cout << "insertion sort: ";
	arrPrint(arr1);
	cout << "time : " << (end_time1 - start_time1).count() << endl;
	cout << "insertion count: " << insertion_count;
	cout << "\n\n\n";


	// heapSort
	// arr의 값 arr2에 복사
	memcpy(arr2, arr, (sizeof(int)) * SIZE);

	//시간 측정 및 sort실행
	auto start_time2 = chrono::high_resolution_clock::now();
	heapSort(arr2);
	auto end_time2 = chrono::high_resolution_clock::now();

	//print
	cout << "heap sort: ";
	arrPrint(arr2);
	cout << "time : " << (end_time2 - start_time2).count() << endl;
	cout << "heap count: " << heap_count;
	cout << "\n\n\n";


	//mergeSort
	//arr의 값 arr3에 복사
	memcpy(arr3, arr, (sizeof(int)) * SIZE);

	//시간 측정 및 sort실행
	auto start_time3 = chrono::high_resolution_clock::now();
	mergeSort(arr3, start, end);
	auto end_time3 = chrono::high_resolution_clock::now();

	//print
	cout << "merge sort: ";
	arrPrint(arr3);
	cout << "time : " << (end_time3 - start_time3).count() << endl;
	cout << "merge count: " << merge_count;
	cout << "\n\n\n";


	//quickSort_1
	//arr의 값 arr4에 복사
	memcpy(arr4, arr, (sizeof(int)) * SIZE);

	//시간 측정 및 sort실행
	auto start_time4 = chrono::high_resolution_clock::now();
	quickSort_1(arr4,start,end);
	auto end_time4 = chrono::high_resolution_clock::now();

	//print
	cout << "quick sort_1: ";
	arrPrint(arr4);
	cout << "time : " << (end_time4 - start_time4).count() << endl;
	cout << "quick count 1: " << quick_count_1;
	cout << "\n\n\n";


	//quickSort_2
	//arr의 값 arr5에 복사
	memcpy(arr5,arr, (sizeof(int)) * SIZE);

	//시간 측정 및 sort 실행
	auto start_time5 = chrono::high_resolution_clock::now();
	quickSort_2(arr5, start, end);
	auto end_time5 = chrono::high_resolution_clock::now();

	//print
	cout << "quick sort_2: ";
	arrPrint(arr5);
	cout << "time : " << (end_time5 - start_time5).count() << endl;
	cout << "quick count 2: " << quick_count_2;
	cout << "\n\n\n";


	//quickSort_3
	//arr의 값 arr6에 복사
	memcpy(arr6, arr, (sizeof(int)) * SIZE);

	//시간 측정 및 sort 실행
	auto start_time6 = chrono::high_resolution_clock::now();
	quickSort_3(arr6, start, end);
	auto end_time6 = chrono::high_resolution_clock::now();

	//print
	cout << "quick sort_3: ";
	arrPrint(arr6);
	cout << "time : " << (end_time6 - start_time6).count() << endl;
	cout << "quick count 3: " << quick_count_3;
	cout << "\n\n\n";

	return 0;
}

