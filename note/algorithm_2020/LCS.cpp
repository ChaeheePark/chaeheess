#include <cstring>
#include <iostream>
#pragma warning (disable : 4996)

using namespace std;

static int c[100][100]; // ���̰� ����� 2���� �迭
static char b[100][100]; // Up(��), Cross(�밢��), Left(����) ǥ�ø� ���� ��Ÿ��
int i, j;

void LCS_Length(char* X, char* Y, int m, int n){
	 
	// 0���� ������ ��, ��� 0���� ����
	for (i = 1; i <= m; i++) {
		c[i][0] = 0;
	}
	for (j = 0; j <= n; j++) {
		c[0][j] = 0;
	}
	
	for (i = 1; i <= m; i++) {
		for (j = 1; j <= n; j++) {
			if (X[i-1] == Y[j-1]) { // ���ڿ��� ������
				b[i][j] = 'C';
				c[i][j] = c[i - 1][j - 1] + 1; //  Cross���� �� ���� ���ڿ��� �����Ƿ� 1����
			}
			else { // ���ڿ��� �ٸ��� 
				if (c[i - 1][j] >= c[i][j - 1]) { 
					b[i][j] = 'U'; // i-1�� �� Ŭ��(���� �࿡�� �� ���� �� Ŭ��) Up���� �� �� ����
					c[i][j] = c[i - 1][j];
				}
				else {
					b[i][j] = 'L'; // j-1�� �� Ŭ��(���� ������ �� ���� �� Ŭ��) Left���� �� �� ����
					c[i][j] = c[i][j - 1];
				}
			}
		}
	}
}

void LCS_Print(char* X,int i, int j) { // LCS ����Ʈ�ϴ� ����
	
	if (i == 0 || j == 0) {
		return;
	}
	
	if (b[i][j] == 'C') { // Cross �϶� ���
		LCS_Print(X, i - 1, j - 1); // ����Լ��� ����
		cout << X[i - 1];
	}
	else {
		if (b[i][j] == 'U') {
			LCS_Print(X, i - 1, j); // �� �ٿ����鼭 ����Լ�
		}
		else {
			LCS_Print(X, i, j - 1); // �� �ٿ����鼭 ����Լ�
		}
	}
}

int main() {

	char dna1[100]; // ù��° DNA
	char dna2[100]; // �ι�° DNA
	
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
	// c,b �迭 ����Ʈ�ϴ� ����
	cout << "\n Length �迭: \n";
	for (i = 0; i < dna1_len+1; i++) {
		for (j = 0; j < dna2_len+1; j++) {
			printf("%3d ", c[i][j]);
		}
		cout << "\n";
	}
	cout << "\n\n ���� �迭: \n";
	for (i = 0; i < dna1_len+1; i++) {
		for (j = 0; j < dna2_len+1; j++) {
			printf("%3C ", b[i][j]);
		}
		cout << "\n";
	}
	*/

	return 0;
	
}