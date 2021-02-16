Dynamic Programming : 하나의 문제를 단 한번만 풀도록 하는 알고리즘

일반적으로 상당수 분할 정복 기법은 동일한 문제를 다시 푼다는 단점 - > 해결하기 위해 이미 계산한 결과는 배열에 저장

ex) 피보나치수열 

```c
#include <stdio.h>

int pibo(int n) {
	if (n == 1) return 1;
	if (n == 2) return 1;
	return pibo(n - 1) + pibo(n - 2);
}

int main() {
	printf("%d", pibo(10));
	return 0;
}
```

이런경우에는 pibo(50)을 했을때 매우 느리게 나오는 것을 알 수 있다.



DP로 구성한 피보나치수열

```c
#include <stdio.h>

int dp[100];

int pibo(int n) {
	if (n == 1) return 1;
	if (n == 2) return 1;
	if (dp[n] != 0) return dp[n];
	return dp[n] = pibo(n - 1) + pibo(n - 2);
}

int main() {
	printf("%d", pibo(30));
	return 0;
}
```

