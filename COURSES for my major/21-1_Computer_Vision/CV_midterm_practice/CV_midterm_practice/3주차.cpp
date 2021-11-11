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
		if (Img[i] * Val> 255.0) Out[i] = 255;
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


int main() {

	//영상입력
	BITMAPFILEHEADER hf;
	BITMAPINFOHEADER hInfo;
	RGBQUAD hRGB[256];
	FILE* fp;

	fp = fopen("coin.bmp", "rb");
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

	//스트레칭
	//HistogramStretching(Image, Output, Histo, W, H);

	//평활화
	//HistogramEqualization(Image, Output, AHisto, W, H);

	//이진화
	BYTE Th;
	Th = DetermThGonzalez(Histo);
	printf("%d", Th);
	Binarization(Image, Output, W, H, Th);
	//결과출력
	fp = fopen("outputcoin.bmp", "wb");
	fwrite(&hf, sizeof(BYTE), sizeof(BITMAPFILEHEADER), fp); //binary file은 byte 단위로 의미가 있음, 그래서 읽어올때와 다르게 쓸 때는 byte 단위로 써야 더 의미있음
	fwrite(&hInfo, sizeof(BYTE), sizeof(BITMAPINFOHEADER), fp);
	fwrite(hRGB, sizeof(RGBQUAD), 256, fp);
	fwrite(Output, sizeof(BYTE), ImgSize, fp);
	fclose(fp);

	free(Image);
	free(Output);
	return 0;
}