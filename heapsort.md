힙 정렬 : 힙 트리구조를 이용하는 정렬 (이진트리), 최솟값이나 최댓값을 빠르게 찾아내기 위해 완전 이진트리를 기반으로 한다. **merge sort와 시간복잡도가 같지만 heap sort는 in-place특성을 갖고 있다.

완전 이진트리 :  데이터가 루트노드부터 시작해서 자식노드가 왼쪽 자식노드, 오른쪽 자식노드로 차근차근 들어가는 이진트리

최대힙 : 부모 노드의 값이 자식 노드의 값보다 큰 트리로 이루어진 구조

힙생성 알고리즘 (Heapify Algorithm): 특정한 노드의 두 자식중에서 더 큰 자식과 자신의 위치를 바꾸는 알고리즘

위치를 바꾸는건 트리의 최대 레벨까지만 할수 있으므로 시간복잡도는 O(logn)이다.

=>모든 노드에 대하여 힙 생성 알고리즘을 수행하면 시간복잡도는 모든 노드 개수가 n이므로 O(nlogn)이다. 

최대힙 정렬된 후에 오름차순으로 정렬하는 것도 heapify algorithm 이므로 O(nlogn)이다. 따라서 힙 정렬은 O(nlogn)의 시간복잡도를 갖는다.



#include <stdio.h>

int main() {
	int arr[9] = { 5,3,4,2,8,1,9,7,6 };
	int i;
	int temp;
	for (i = 1; i < 9; i++) {
		int c = i;
		do {
			int root = (c - 1) / 2;
			if (arr[root] < arr[c]) {
				temp = arr[root];
				arr[root] = arr[c];
				arr[c] = temp;
			}
			c = root; //부모가 더 클때까지 level올라가며 반복적으로 수행
		} while (c != 0); //최대 heap구조만듬
	}
	for (i = 0; i < 9; i++) {
		printf("%d ", arr[i]);
	} /
	for (i = 8; i >= 0; i--) {
		temp = arr[0];
		arr[0] = arr[i];
		arr[i] = temp;
		int root = 0;
		int c = 1;
		do {
			c = root * 2 + 1;
			if (c<i-1&&arr[c] < arr[c + 1]) {
				c++;
			}
			if (c<i&&arr[c] > arr[root]) {
				temp = arr[c];
				arr[c] = arr[root];
				arr[root] = temp;
			}
			root = c;
		} while (c < i); //최대힙의 반대로 생각? c를 주체로!
	}
	for (i = 0; i < 9; i++) {
		printf("%d ", arr[i]);
	}
	return 0;
}



-max heapify O(logn)

-build max heap (tight bound heap) O(n)

-heap sort O(nlogn)

-heap maximum O(1)

-heap extract max O(logn) 

-heap increase key O(logn)

-heap insert O(logn)