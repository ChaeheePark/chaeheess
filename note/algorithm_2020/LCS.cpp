#include <cstring>
#include <iostream>
#pragma warning (disable : 4996)

using namespace std;

static int c[100][100]; // 길이가 저장된 2차원 배열
static char b[100][100]; // Up(위), Cross(대각선), Left(왼쪽) 표시를 위해 나타냄
int i, j;

void LCS_Length(char* X, char* Y, int m, int n){
	 
	// 0으로 설정된 곳, 빈곳 0으로 설정
	for (i = 1; i <= m; i++) {
		c[i][0] = 0;
	}
	for (j = 0; j <= n; j++) {
		c[0][j] = 0;
	}
	
	for (i = 1; i <= m; i++) {
		for (j = 1; j <= n; j++) {
			if (X[i-1] == Y[j-1]) { // 문자열이 같을때
				b[i][j] = 'C';
				c[i][j] = c[i - 1][j - 1] + 1; //  Cross에서 온 값에 문자열이 같으므로 1증가
			}
			else { // 문자열이 다를때 
				if (c[i - 1][j] >= c[i][j - 1]) { 
					b[i][j] = 'U'; // i-1이 더 클때(전의 행에서 온 값이 더 클때) Up에서 온 값 저장
					c[i][j] = c[i - 1][j];
				}
				else {
					b[i][j] = 'L'; // j-1이 더 클때(전의 열에서 온 값이 더 클때) Left에서 온 값 저장
					c[i][j] = c[i][j - 1];
				}
			}
		}
	}
}

void LCS_Print(char* X,int i, int j) { // LCS 프린트하는 과정
	
	if (i == 0 || j == 0) {
		return;
	}
	
	if (b[i][j] == 'C') { // Cross 일때 출력
		LCS_Print(X, i - 1, j - 1); // 재귀함수로 구현
		cout << X[i - 1];
	}
	else {
		if (b[i][j] == 'U') {
			LCS_Print(X, i - 1, j); // 행 줄여가면서 재귀함수
		}
		else {
			LCS_Print(X, i, j - 1); // 열 줄여가면서 재귀함수
		}
	}
}

int main() {

	char dna1[100]; // 첫번째 DNA
	char dna2[100]; // 두번째 DNA
	
	cout << "S1: ";
	cin >> dna1;
	cout << "S2: ";
	cin >> dna2;

	int dna1_len = strlen(dna1);
	int dna2_len = strlen(dna2);
	 
	LCS_Length(dna1, dna2, dna1_len, dna2_len);

	cout << "LCS : ";
	LCS_Print(dna1,dna1_len,dna2_len);


	/*
	// c,b 배열 프린트하는 과정
	cout << "\n Length 배열: \n";
	for (i = 0; i < dna1_len+1; i++) {
		for (j = 0; j < dna2_len+1; j++) {
			printf("%3d ", c[i][j]);
		}
		cout << "\n";
	}
	cout << "\n\n 방향 배열: \n";
	for (i = 0; i < dna1_len+1; i++) {
		for (j = 0; j < dna2_len+1; j++) {
			printf("%3C ", b[i][j]);
		}
		cout << "\n";
	}
	*/

	return 0;
	
}