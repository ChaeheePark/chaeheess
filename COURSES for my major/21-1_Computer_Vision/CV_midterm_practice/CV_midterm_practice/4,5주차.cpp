#pragma warning (disable:4996)
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>

// 영상 화소 값 반전
void InverseImage(BYTE* Img, BYTE* Out, int W, int H) {
	int ImgSize = W * H;
	for (int i = 0; i < ImgSize; i++) {
		Out[i] = 255 - Img[i];
	}
}

// 영상 밝기 조절
void BrightnessAdj(BYTE* Img, BYTE* Out, int W, int H, int Val) {
	int ImgSize = W * H;
	for (int i = 0; i < ImgSize; i++) {
		if (Img[i] + Val > 255) Out[i] = 255;
		else if (Img[i] + Val < 0) Out[i] = 0;
		else Out[i] = Img[i] + Val;
	}
}

// 영상 대비 조절
void ContrastAdj(BYTE* Img, BYTE* Out, int W, int H, double Val) {
	int ImgSize = W * H;
	for (int i = 0; i < ImgSize; i++) {
		if (Img[i] * Val > 255.0) Out[i] = 255;
		else Out[i] = (BYTE)(Img[i] * Val);
	}
}

// 화소 Histogram
void ObtainHistogram(BYTE* Img, int* Histo, int W, int H) {
	int ImgSize = W * H;
	for (int i = 0; i < ImgSize; i++)  Histo[Img[i]]++;
}

// Histo의 누적 Histogram
void ObtainAHistogram(int* Histo, int* AHisto) {
	for (int i = 0; i < 256; i++) {
		for (int j = 0; j <= i; j++) {
			AHisto[i] += Histo[j];
		}
	}
}

// 스트레칭
void HistogramStretching(BYTE* Img, BYTE* Out, int* Histo, int W, int H) {
	int ImgSize = W * H;
	BYTE Low, High;
	for (int i = 0; i < 255; i++) {
		if (Histo[i] != 0) {
			Low = i;
			break;
		}
	}
	for (int i = 255; i >= 0; i--) {
		if (Histo[i] != 0) {
			High = i;
			break;
		}
	}

	for (int i = 0; i < ImgSize; i++) {
		Out[i] = (BYTE)(Img[i] - Low) / (double)(High - Low) * 255.0;
	}

}

// 평활화
void HistogramEqualization(BYTE* Img, BYTE* Out, int* AHisto, int W, int H) {
	int ImgSize = W * H;
	int Nt = ImgSize;
	int Gmax = 255;
	double Ratio = Gmax / (double)Nt;
	BYTE NormSum[256];
	for (int i = 0; i < 256; i++) {
		NormSum[i] = (BYTE)(Ratio * AHisto[i]);
	}
	for (int i = 0; i < ImgSize; i++) {
		Out[i] = NormSum[Img[i]];
	}
}

// 곤잘레스 임계값 정하기
BYTE DetermThGonzalez(int* Histo) {
	int ep = 3;
	BYTE Low, High;
	BYTE Th, NewTh;
	int G1 = 0, G2 = 0, cnt1 = 0, cnt2 = 0, mu1, mu2;
	for (int i = 0; i < 255; i++) {
		if (Histo[i] != 0) {
			Low = i;
			break;
		}
	}
	for (int i = 255; i >= 0; i--) {
		if (Histo[i] != 0) {
			High = i;
			break;
		}
	}
	Th = (Low + High) / 2; //초기 임계값
	while (1) {
		for (int i = Th + 1; i <= High; i++) {
			G1 += Histo[i] * i;
			cnt1 += Histo[i];
		}
		for (int i = Low; i <= Th; i++) {
			G2 += Histo[i] * i;
			cnt2 += Histo[i];
		}

		if (cnt1 == 0) cnt1 = 1;
		if (cnt2 == 0) cnt2 = 1;
		mu1 = G1 / cnt1;
		mu2 = G2 / cnt2;
		NewTh = (mu1 + mu2) / 2;

		if (abs(NewTh - Th) < ep) {
			Th = NewTh;
			return Th;
		}
		else Th = NewTh;
		G1 = G2 = cnt1 = cnt2 = 0;
	}
}

// 영상 이진화 (임계값을 기준으로)
void Binarization(BYTE* Img, BYTE* Out, int W, int H, BYTE Threshold) {
	int ImgSize = W * H;
	for (int i = 0; i < ImgSize; i++) {
		if (Img[i] < Threshold) Out[i] = 0;
		else Out[i] = 255;
	}
}

// average lowpass(영상흐려짐)
void AverageConv(BYTE* Img, BYTE* Out, int W, int H) {
	double Kernel[3][3] = { 0.11111,0.11111,0.11111,
							0.11111,0.11111,0.11111,
							0.11111,0.11111,0.11111 };
	double SumProduct = 0.0;
	for (int i = 1; i < H-1; i++) { //margin을 주기위해 1부터 시작 ~ H-1 까지
		for (int j = 1; j < W-1; j++) {
			for (int m = -1; m <= 1; m++) { // 3*3 kernel 에서 Height 
				for (int n = -1; n <= 1; n++) { //  Width 나타내기위해
					SumProduct += Img[(i+m)*W+(j+n)] * Kernel[m+1][n+1]; //Y*W+X
				}
			}
			Out[i * W + j] = (BYTE)SumProduct;
			SumProduct = 0;
		}
	}
}


// 가우시안평활화
void GaussAvgConv(BYTE* Img, BYTE* Out, int W, int H) {
	double Kernel[3][3] = { 0.0625,0.125,0.0625,
							0.125,0.25,0.125,
							0.0625,0.125,0.0625 };
	double SumProduct = 0.0;
	for (int i = 1; i < H - 1; i++) { //margin을 주기위해 1부터 시작 ~ H-1 까지
		for (int j = 1; j < W - 1; j++) {
			for (int m = -1; m <= 1; m++) { // 3*3 kernel 에서 Height 
				for (int n = -1; n <= 1; n++) { //  Width 나타내기위해
					SumProduct += Img[(i + m) * W + (j + n)] * Kernel[m + 1][n + 1]; //Y*W+X
				}
			}
			Out[i * W + j] = (BYTE)SumProduct;
			SumProduct = 0;
		}
	}
}


// Prewitt 마스크
void Prewitt_X_Conv(BYTE * Img, BYTE * Out, int W, int H) {
	double Kernel[3][3] = {-1.0,0.0,1.0,
							-1.0,0.0,1.0,
							-1.0,0.0,1.0 };
	double SumProduct = 0.0;
	for (int i = 1; i < H - 1; i++) { //margin을 주기위해 1부터 시작 ~ H-1 까지
		for (int j = 1; j < W - 1; j++) {
			for (int m = -1; m <= 1; m++) { // 3*3 kernel 에서 Height 
				for (int n = -1; n <= 1; n++) { //  Width 나타내기위해
					SumProduct += Img[(i + m) * W + (j + n)] * Kernel[m + 1][n + 1]; //Y*W+X
				}
			}
			// 절대값 씌우면 0~765 사이의 값이 나올거임
			Out[i * W + j] = abs((long)SumProduct) / 3;
			SumProduct = 0;
		}
	}
}

void Prewitt_Y_Conv(BYTE* Img, BYTE* Out, int W, int H) {
	double Kernel[3][3] = { -1.0,-1.0,-1.0,
							0.0,0.0,0.0,
							1.0,1.0,1.0 };
	double SumProduct = 0.0;
	for (int i = 1; i < H - 1; i++) { //margin을 주기위해 1부터 시작 ~ H-1 까지
		for (int j = 1; j < W - 1; j++) {
			for (int m = -1; m <= 1; m++) { // 3*3 kernel 에서 Height 
				for (int n = -1; n <= 1; n++) { //  Width 나타내기위해
					SumProduct += Img[(i + m) * W + (j + n)] * Kernel[m + 1][n + 1]; //Y*W+X
				}
			}
			// 절대값 씌우면 0~765 사이의 값이 나올거임
			Out[i * W + j] = abs((long)SumProduct) / 3;
			SumProduct = 0;
		}
	}
}

// Sobel 마스크
void Sobel_X_Conv(BYTE* Img, BYTE* Out, int W, int H) {
	double Kernel[3][3] = { -1.0,0.0,1.0,
							-2.0,0.0,2.0,
							-1.0,0.0,1.0 };
	double SumProduct = 0.0;
	for (int i = 1; i < H - 1; i++) { //margin을 주기위해 1부터 시작 ~ H-1 까지
		for (int j = 1; j < W - 1; j++) {
			for (int m = -1; m <= 1; m++) { // 3*3 kernel 에서 Height 
				for (int n = -1; n <= 1; n++) { //  Width 나타내기위해
					SumProduct += Img[(i + m) * W + (j + n)] * Kernel[m + 1][n + 1]; //Y*W+X
				}
			}
			// 절대값 씌우면 0~765 사이의 값이 나올거임
			Out[i * W + j] = abs((long)SumProduct) / 4;
			SumProduct = 0;
		}
	}
}

void Sobel_Y_Conv(BYTE* Img, BYTE* Out, int W, int H) {
	double Kernel[3][3] = { -1.0,-2.0,-1.0,
							0.0,0.0,0.0,
							1.0,2.0,1.0 };
	double SumProduct = 0.0;
	for (int i = 1; i < H - 1; i++) { //margin을 주기위해 1부터 시작 ~ H-1 까지
		for (int j = 1; j < W - 1; j++) {
			for (int m = -1; m <= 1; m++) { // 3*3 kernel 에서 Height 
				for (int n = -1; n <= 1; n++) { //  Width 나타내기위해
					SumProduct += Img[(i + m) * W + (j + n)] * Kernel[m + 1][n + 1]; //Y*W+X
				}
			}
			// 절대값 씌우면 0~765 사이의 값이 나올거임
			Out[i * W + j] = abs((long)SumProduct) / 4;
			SumProduct = 0;
		}
	}
}

//Laplace 마스크
void Laplace_Conv(BYTE* Img, BYTE* Out, int W, int H) {
	double Kernel[3][3] = { -1.0,-1.0,-1.0,
							-1.0,8.0,-1.0,
							-1.0,-1.0,-1.0 };
	double SumProduct = 0.0;
	for (int i = 1; i < H - 1; i++) { //margin을 주기위해 1부터 시작 ~ H-1 까지
		for (int j = 1; j < W - 1; j++) {
			for (int m = -1; m <= 1; m++) { // 3*3 kernel 에서 Height 
				for (int n = -1; n <= 1; n++) { //  Width 나타내기위해
					SumProduct += Img[(i + m) * W + (j + n)] * Kernel[m + 1][n + 1]; //Y*W+X
				}
			}
			// 절대값 씌우면 0~765 사이의 값이 나올거임
			Out[i * W + j] = abs((long)SumProduct) / 8;
			SumProduct = 0;
		}
	}
}

void Laplace_Conv_DC(BYTE* Img, BYTE* Out, int W, int H) {
	double Kernel[3][3] = { -1.0,-1.0,-1.0,
							-1.0,9.0,-1.0,
							-1.0,-1.0,-1.0 };
	double SumProduct = 0.0;
	for (int i = 1; i < H - 1; i++) { //margin을 주기위해 1부터 시작 ~ H-1 까지
		for (int j = 1; j < W - 1; j++) {
			for (int m = -1; m <= 1; m++) { // 3*3 kernel 에서 Height 
				for (int n = -1; n <= 1; n++) { //  Width 나타내기위해
					SumProduct += Img[(i + m) * W + (j + n)] * Kernel[m + 1][n + 1]; //Y*W+X
				}
			}
			//원래 영상의 밝기 유지
			if (SumProduct > 255.0) Out[i * W + j] = 255;
			else if (SumProduct < 0.0) Out[i * W + j] = 0;
			else Out[i * W + j] = (BYTE)SumProduct;
			SumProduct = 0.0;
		}
	}
}

void swap(BYTE* a, BYTE* b) {
	BYTE tp = *a;
	*a = *b;
	*b = tp;
}

// 9개 화소값중 가운데 값으로 치환 (솔트페퍼 노이즈 감소)
BYTE Median(BYTE* arr, int size) {
	const int S = size;
	for (int i = 0; i < size - 1; i++) {//pivot index
		for (int j = 0; j < size; j++) {// 비교대상 index
			if (arr[i] > arr[j]) swap(&arr[i], &arr[j]);
		}
	}
	return arr[size/2];
}

//9개 화소값중 가장 큰 값으로 치환
BYTE MaxPooling(BYTE* arr, int size) {
	const int S = size;
	for (int i = 0; i < size - 1; i++) {//pivot index
		for (int j = 0; j < size; j++) {// 비교대상 index
			if (arr[i] > arr[j]) swap(&arr[i], &arr[j]);
		}
	}
	return arr[S-1];
}

//9개 화소값중 가장 작은 값으로 치환 솔트나 페퍼노이즈중 하나만 영상에서 많이 존재할 때 사용
BYTE MinPooling(BYTE* arr, int size) {
	const int S = size;
	for (int i = 0; i < size - 1; i++) {//pivot index
		for (int j = 0; j < size; j++) {// 비교대상 index
			if (arr[i] > arr[j]) swap(&arr[i], &arr[j]);
		}
	}
	return arr[0];
}


int main() {

	//영상입력
	BITMAPFILEHEADER hf;
	BITMAPINFOHEADER hInfo;
	RGBQUAD hRGB[256];
	FILE* fp;

	fp = fopen("LENNA_impulse.bmp", "rb");
	if (fp == NULL) {
		printf("file not found!\n");
		return 0;
	}

	fread(&hf, sizeof(BITMAPFILEHEADER), 1, fp); //14byte를 1번 읽어와서 hf에 저장해라
	fread(&hInfo, sizeof(BITMAPINFOHEADER), 1, fp);
	fread(hRGB, sizeof(RGBQUAD), 256, fp);
	int ImgSize = hInfo.biWidth * hInfo.biHeight;

	BYTE* Image = (BYTE*)malloc(ImgSize);
	BYTE* Output = (BYTE*)malloc(ImgSize);
	BYTE* Temp = (BYTE*)malloc(ImgSize);
	fread(Image, sizeof(BYTE), ImgSize, fp);
	fclose(fp);

	int W = hInfo.biWidth, H = hInfo.biHeight;
	int Histo[256] = { 0 };
	int AHisto[256] = { 0 };
	//영상처리

	/*InverseImage(Image, Output, W, H);
	BrightnessAdj(Image, Output, W, H, 70);
	ContrastAdj(Image, Output, W, H, 1.5);*/

	// 히스토그램
	ObtainHistogram(Image, Histo, W, H);
	// 누적 히스토그램
	ObtainAHistogram(Histo, AHisto);

	//평활화
	//AverageConv(Image, Output, W, H);
	//GaussAvgConv(Image, Output, W, H);

	////경계검출 후 이진화
	//Prewitt_X_Conv(Image, Temp, W, H);
	//Prewitt_Y_Conv(Image, Output, W, H);
	//for (int i = 0; i < ImgSize; i++) {
	//	if (Temp[i] > Output[i]) Output[i] = Temp[i];
	//}
	//Binarization(Output, Output, W, H, 50);

	//Sobel_X_Conv(Image, Temp, W, H);
	//Sobel_Y_Conv(Image, Output, W, H);
	//for (int i = 0; i < ImgSize; i++) {
	//	if (Temp[i] > Output[i]) Output[i] = Temp[i];
	//}
	//Binarization(Output, Output, W, H, 50);
	 
	
	//Laplace_Conv(Image, Output, W, H);

	//평활화 후 경계검출
	//GaussAvgConv(Image, Output, W, H);
	//Laplace_Conv_DC(Image, Output, W, H);



	//스트레칭
	//HistogramStretching(Image, Output, Histo, W, H);

	//평활화
	//HistogramEqualization(Image, Output, AHisto, W, H);

	/*이진화
	BYTE Th;
	Th = DetermThGonzalez(Histo);
	printf("%d", Th);
	Binarization(Image, Output, W, H, Th);*/

	/*median filtering
	BYTE temp[9];
	int i, j;
	for (i = 1; i < H - 1; i++) {
		for (j = 0; j < W - 1; j++) {
			temp[0] = Image[(i - 1) * W + j-1];
			temp[1] = Image[(i-1) * W + j];
			temp[2] = Image[(i - 1) * W + j+1];
			temp[3] = Image[i * W + j-1];
			temp[4] = Image[i * W + j];
			temp[5] = Image[i * W + j+1];
			temp[6] = Image[(i + 1)* W + j-1];
			temp[7] = Image[(i + 1 )* W + j];
			temp[8] = Image[(i + 1 )* W + j+1];
			Output[i * W + j] = Median(temp, 9);
			//Output[i * W + j] = MaxPooling(temp, 9);
			//Output[i * W + j] = MinPooling(temp, 9);
		}
	}*/

	/* Median filtering 
	int Length = 3;  // 마스크의 한 변의 길이
	int Margin = Length / 2;
	int WSize = Length * Length;
	BYTE* temp = (BYTE*)malloc(sizeof(BYTE) * WSize);

	int i, j, m, n;

	for (i = Margin; i < H - Margin; i++) {
		for (j = Margin; j < W - Margin; j++) {
			for (m = -Margin; m <= Margin; m++) {
				for (n = -Margin; n <= Margin; n++) {
					temp[(m + Margin) * Length + (n + Margin)] = Image[(i + m) * W + j + n];
				}
			}
			Output[i * W + j] = Median(temp, WSize);
		}
	}
	free(temp);
	*/


	//결과출력
	fp = fopen("medLENNA.bmp", "wb");
	fwrite(&hf, sizeof(BYTE), sizeof(BITMAPFILEHEADER), fp); //binary file은 byte 단위로 의미가 있음, 그래서 읽어올때와 다르게 쓸 때는 byte 단위로 써야 더 의미있음
	fwrite(&hInfo, sizeof(BYTE), sizeof(BITMAPINFOHEADER), fp);
	fwrite(hRGB, sizeof(RGBQUAD), 256, fp);
	fwrite(Output, sizeof(BYTE), ImgSize, fp);
	fclose(fp);

	free(Image);
	free(Output);
	free(Temp);
	return 0;
}