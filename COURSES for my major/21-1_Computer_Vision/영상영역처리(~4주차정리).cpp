#pragma warning (disable:4996)
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>

void InverseImage(BYTE* Img, BYTE* Out, int W, int H){
	int ImgSize = W * H;
	for (int i = 0; i < ImgSize; i++){
		Out[i] = 255 - Img[i];
	}
}

void BrightnessAdj(BYTE* Img, BYTE* Out, int W, int H, int Val){
	int ImgSize = W * H;
	for (int i = 0; i < ImgSize; i++){
		if (Img[i] + Val > 255){
			Out[i] = 255;
		}
		else if (Img[i] + Val < 0){
			Out[i] = 0;
		}
		else Out[i] = Img[i] + Val;
	}
}

void ContrastAdj(BYTE* Img, BYTE* Out, int W, int H, double Val){
	int ImgSize = W * H;
	for (int i = 0; i < ImgSize; i++) {
		if (Img[i] * Val > 255.0) Out[i] = 255;
		else Out[i] = (BYTE)(Img[i] * Val);
	}
}


void HistogramStretching(BYTE* Img, BYTE* Out, int* Histo, int W, int H){
	int ImgSize = W * H;
	BYTE Low, High;
	for (int i = 0; i < 256; i++) {
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
		Out[i] = (BYTE)((Img[i] - Low) / (double)(High - Low) * 255.0);
	}
}

void HistogramEqualization(BYTE* Img, BYTE* Out, int* AHisto, int W, int H){
	int ImgSize = W * H;
	int Nt = W * H, Gmax = 255;
	double Ratio = Gmax / (double)Nt;
	BYTE NormSum[256];
	for (int i = 0; i < 256; i++) {
		NormSum[i] = (BYTE)(Ratio * AHisto[i]);
	}
	for (int i = 0; i < ImgSize; i++){
		Out[i] = NormSum[Img[i]];
	}
}


void ObtainHistogram(BYTE* Img, int* Histo, int W, int H) {
	int ImgSize = W * H;
	for (int i = 0; i < ImgSize; i++) {
		Histo[Img[i]]++;
	}
}

void ObtainAHistogram(int* Histo, int* AHisto){
	for (int i = 0; i < 256; i++) {
		for (int j = 0; j <= i; j++) {
			AHisto[i] += Histo[j];
		}
	}
}


int GozalezBinThresh(BYTE* Img, int* Histo, int W, int H) {
	int ImgSize = W * H;
	BYTE Low, High;
	BYTE* Temp = (BYTE*)malloc(ImgSize);

	for (int i = 0; i < 256; i++) {
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

	int T = (int)(High + Low) / 2;
	int pastT = T;
	int diff = 0;

	do {
		pastT = T;
		for (int i = 0; i < ImgSize; i++) {
			if (Img[i] < T) Temp[i] = 0;
			else Temp[i] = 1;
		}

		int sum0 = 0, count0 = 0;
		int sum1 = 0, count1 = 0;
		for (int i = 0; i < ImgSize; i++) {
			if (Temp[i] == 0) {
				sum0 += Img[i];
				count0++;
			}
			else {
				sum1 += Img[i];
				count1++;
			}
		}
		T = (int)((sum0 / count0) + (sum1 / count1)) / 2;

		diff = pastT - T;
		if (diff < 0) diff = (-1) * diff;

		//printf("%d\n", T);
		//printf("diff %d\n", diff);
	} while (diff > 3);
	return T;
}

void Binarization(BYTE* Img, BYTE* Out, int W, int H, BYTE Threshold)
{
	int ImgSize = W * H;
	for (int i = 0; i < ImgSize; i++) {
		if (Img[i] < Threshold) Out[i] = 0;
		else Out[i] = 255;
	}
}

void Prewitt_X_Conv(BYTE* Img, BYTE* Out, int W, int H) { // Prewitt 마스킅
	double Kernal[3][3] = { -1.0, 0.0,  1.0,
							-1.0, 0.0, 1.0,
							-1.0, 0.0,  1.0 };
	double SumProduct = 0.0;
	for (int i = 1; i < H - 1; i++) { // Y좌표
		for (int j = 1; j < W - 1; j++) { // X좌표마진생성
			for (int m = -1; m <= 1; m++) { //커널 행
				for (int n = -1; n <= 1; n++) {  //커널 열 //Y*W+X == 2차원을 1차원에서 찾기
					SumProduct += Img[(i + m) * W + (j + n)] * Kernal[m + 1][n + 1];
				}
			}
			//0~765 ==> 0~255

			Out[i * W + j] = abs((long)SumProduct) / 3;
			SumProduct = 0.0;
		}
	}
}

void Prewitt_Y_Conv(BYTE* Img, BYTE* Out, int W, int H) { // Prewitt 마스킅
	double Kernal[3][3] = { -1.0,-1.0,  -1.0,
							0.0, 0.0, 0.0,
							1.0, 1.0,  1.0 };
	double SumProduct = 0.0;
	for (int i = 1; i < H - 1; i++) { // Y좌표
		for (int j = 1; j < W - 1; j++) { // X좌표마진생성
			for (int m = -1; m <= 1; m++) { //커널 행
				for (int n = -1; n <= 1; n++) {  //커널 열 //Y*W+X == 2차원을 1차원에서 찾기
					SumProduct += Img[(i + m) * W + (j + n)] * Kernal[m + 1][n + 1];
				}
			}
			//0~765 ==> 0~255

			Out[i * W + j] = abs((long)SumProduct) / 3;
			SumProduct = 0.0;
		}
	}
}


void Sobel_X_Conv(BYTE* Img, BYTE* Out, int W, int H) { // Prewitt 마스킅
	double Kernal[3][3] = { -1.0, 0.0,  1.0,
							-2.0, 0.0, 2.0,
							-1.0, 0.0,  1.0 };
	double SumProduct = 0.0;
	for (int i = 1; i < H - 1; i++) { // Y좌표
		for (int j = 1; j < W - 1; j++) { // X좌표마진생성
			for (int m = -1; m <= 1; m++) { //커널 행
				for (int n = -1; n <= 1; n++) {  //커널 열 //Y*W+X == 2차원을 1차원에서 찾기
					SumProduct += Img[(i + m) * W + (j + n)] * Kernal[m + 1][n + 1];
				}
			}
			//0~1020 ==> 0~255

			Out[i * W + j] = abs((long)SumProduct) / 4;
			SumProduct = 0.0;
		}
	}
}

void Sobel_Y_Conv(BYTE* Img, BYTE* Out, int W, int H) { // Prewitt 마스킅
	double Kernal[3][3] = { -1.0,-2.0,  -1.0,
							0.0, 0.0, 0.0,
							1.0, 2.0,  1.0 };
	double SumProduct = 0.0;
	for (int i = 1; i < H - 1; i++) { // Y좌표
		for (int j = 1; j < W - 1; j++) { // X좌표마진생성
			for (int m = -1; m <= 1; m++) { //커널 행
				for (int n = -1; n <= 1; n++) {  //커널 열 //Y*W+X == 2차원을 1차원에서 찾기
					SumProduct += Img[(i + m) * W + (j + n)] * Kernal[m + 1][n + 1];
				}
			}
			//0~765 ==> 0~255

			Out[i * W + j] = abs((long)SumProduct) / 3;
			SumProduct = 0.0;
		}
	}
}

void Laplace_Conv_DC(BYTE* Img, BYTE* Out, int W, int H) { // Prewitt 마스킅
	double Kernal[3][3] = { -1.0,-1.0,  -1.0,
							-1.0, 9.0, -1.0,
							-1.0, -1.0,  -1.0 }; //원래 영상의 밝기를 유지 + 경계 high freq 증폭
	double SumProduct = 0.0;
	for (int i = 1; i < H - 1; i++) { // Y좌표
		for (int j = 1; j < W - 1; j++) { // X좌표마진생성
			for (int m = -1; m <= 1; m++) { //커널 행
				for (int n = -1; n <= 1; n++) {  //커널 열 //Y*W+X == 2차원을 1차원에서 찾기
					SumProduct += Img[(i + m) * W + (j + n)] * Kernal[m + 1][n + 1];
				}
			}
			//0~2295 ==> 0~255
			if (SumProduct > 255) Out[i * W + j] = 255;
			else if (SumProduct < 0.0) Out[i * W + j] = 0;
			else Out[i * W + j] = (BYTE)SumProduct;
			//Out[i * W + j] = abs((long)SumProduct) / 8;
			SumProduct = 0.0;
		}
	}
}

void GaussAvgConv(BYTE* Img, BYTE* Out, int W, int H) { // 박스평활
	double Kernal[3][3] = { 0.0625,0.125,  0.0625,
						0.125, 0.25, 0.125,
						 0.0625, 0.125,  0.0625 };
	double SumProduct = 0.0;
	for (int i = 1; i < H - 1; i++) { // Y좌표
		for (int j = 1; j < W - 1; j++) { // X좌표마진생성
			for (int m = -1; m <= 1; m++) { //커널 행
				for (int n = -1; n <= 1; n++) {  //커널 열 //Y*W+X == 2차원을 1차원에서 찾기
					SumProduct += Img[(i + m) * W + (j + n)] * Kernal[m + 1][n + 1];
				}
			}
			Out[i * W + j] = (BYTE)SumProduct;
			SumProduct = 0.0;
		}
	}
}

void AverageConv(BYTE* Img, BYTE* Out, int W, int H) { // 박스평활
	double Kernal[3][3] = { 0.11111,0.11111, 0.11111,
						0.11111, 0.11111, 0.11111,
						0.11111, 0.11111, 0.11111 };
	double SumProduct = 0.0;
	for (int i = 1; i < H - 1; i++) { // Y좌표
		for (int j = 1; j < W - 1; j++) { // X좌표마진생성
			for (int m = -1; m <= 1; m++) { //커널 행
				for (int n = -1; n <= 1; n++) {  //커널 열 //Y*W+X == 2차원을 1차원에서 찾기
					SumProduct += Img[(i + m) * W + (j + n)] * Kernal[m + 1][n + 1];
				}
			}
			Out[i * W + j] = (BYTE)SumProduct;
			SumProduct = 0.0;
		}
	}
}

int main() {
	BITMAPFILEHEADER hf; //14바이트
	BITMAPINFOHEADER hinfo; //40바이트
	RGBQUAD hRGB[256]; //4*256=1024바이트
	FILE* fp;
	fp = fopen("LENNA.bmp", "rb");
	if (fp == NULL) {
		printf("File not found!\n");
		return -1;
	}
	fread(&hf, sizeof(BITMAPFILEHEADER), 1, fp); // 첫번째 인자, 어디에다가 파일 정보를 저장할 것인지, 변수의 주소를 받기 때문에 & 붙임
	fread(&hinfo, sizeof(BITMAPINFOHEADER), 1, fp);
	fread(hRGB, sizeof(RGBQUAD), 256, fp); // 세번째 인자, 256번 읽어옴
	int ImgSize = hinfo.biWidth * hinfo.biHeight;
	BYTE* Image = (BYTE*)malloc(ImgSize); //==BYTE Image[ImgSize]
	BYTE* Output = (BYTE*)malloc(ImgSize);
	BYTE* Temp = (BYTE*)malloc(ImgSize);

	fread(Image, sizeof(BYTE), ImgSize, fp);
	fclose(fp);

	int Histo[256] = { 0 };
	int AHisto[256] = { 0 };

	/*ObtainHistogram(Image, Histo, hInfo.biWidth, hInfo.biHeight);
	ObtainAHistogram(Histo, AHisto);
	HistogramEqualization(Image, Output, AHisto, hInfo.biWidth, hInfo.biHeight);*/
	/*int Thres = GozalezBinThresh();
	Binarization(Image, Output, hInfo.biWidth, hInfo.biHeight, Thres);*/

	//GaussAvrConv(Image, Output, hInfo.biWidth, hInfo.biHeight);

	/*Sobel_X_Conv(Image, Temp, hInfo.biWidth, hInfo.biHeight);
	Sobel_Y_Conv(Image, Output, hInfo.biWidth, hInfo.biHeight);
	for (int i = 0; i < ImgSize; i++) {
		if (Temp[i] > Output[i]) 	Output[i] = Temp[i];
	}
	Binarization(Output, Output, hInfo.biWidth, hInfo.biHeight, 40);*/
	
	//HistogramStretching(Image, Output, Histo, hInfo.biWidth, hInfo.biHeight);
	//InverseImage(Image, Output, hInfo.biWidth, hInfo.biHeight);
	//BrightnessAdj(Image, Output, hInfo.biWidth, hInfo.biHeight, 70);
	//ContrastAdj(Image, Output, hInfo.biWidth, hInfo.biHeight, 0.5);

	/*Prewitt_X_Conv(Image, Temp, hinfo.biWidth, hinfo.biHeight);
	Prewitt_Y_Conv(Image, Output, hinfo.biWidth, hinfo.biHeight);*/
	/*Sobel_X_Conv(Image, Temp, hinfo.biWidth, hinfo.biHeight);
	Sobel_Y_Conv(Image, Output, hinfo.biWidth, hinfo.biHeight);
	for (int i = 0; i < ImgSize; i++) {
		if (Temp[i] > Output[i]) Output[i] = Temp[i];
	}*/
	GaussAvgConv(Image, Temp, hinfo.biWidth, hinfo.biHeight);
	Laplace_Conv_DC(Temp, Output, hinfo.biWidth, hinfo.biHeight); //근데 노이즈가 너무많이생겨 어떡하지?
	//->가우시안평활화를 한번 처리해준뒤 라플레이스 돌림
	//==lowpass 한 뒤 highpass
	//Binarization(Output, Output, hinfo.biWidth, hinfo.biHeight, 40);

	/*
	ObtainHistogram(Image, Histo, hinfo.biWidth, hinfo.biHeight);


	int Thres = GozalezBinThresh(Image, Histo, hinfo.biWidth, hinfo.biHeight);
	Binarization(Image, Output, hinfo.biWidth, hinfo.biHeight, Thres);*/

	fp = fopen("outputLENNA.bmp", "wb");
	fwrite(&hf, sizeof(BYTE), sizeof(BITMAPFILEHEADER), fp); //byte단위로 써야함(읽어올때랑다름!)
	fwrite(&hinfo, sizeof(BYTE), sizeof(BITMAPINFOHEADER), fp);
	fwrite(hRGB, sizeof(RGBQUAD), 256, fp);
	fwrite(Output, sizeof(BYTE), ImgSize, fp);
	fclose(fp);

	free(Temp);
	free(Image);
	free(Output); // 하지 않으면 memory leak
	return 0;
}
