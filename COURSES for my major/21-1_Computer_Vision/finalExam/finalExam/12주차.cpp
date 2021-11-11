#pragma warning (disable:4996)
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <math.h>

// ���� ȭ�� �� ����
void InverseImage(BYTE* Img, BYTE* Out, int W, int H) {
	int ImgSize = W * H;
	for (int i = 0; i < ImgSize; i++) {
		Out[i] = 255 - Img[i];
	}
}

void SaveBMPFile(BITMAPFILEHEADER hf, BITMAPINFOHEADER hInfo, RGBQUAD* hRGB,
	BYTE* Out, int W, int H, const char* FileName) {
	FILE* fp = fopen(FileName, "wb");
	fwrite(&hf, sizeof(BYTE), sizeof(BITMAPFILEHEADER), fp); //binary file�� byte ������ �ǹ̰� ����, �׷��� �о�ö��� �ٸ��� �� ���� byte ������ ��� �� �ǹ�����
	fwrite(&hInfo, sizeof(BYTE), sizeof(BITMAPINFOHEADER), fp);
	if (hInfo.biBitCount == 24) {
		fwrite(Out, sizeof(BYTE), W * H * 3, fp);
	}
	else {
		fwrite(hRGB, sizeof(RGBQUAD), 256, fp);
		fwrite(Out, sizeof(BYTE), W * H, fp);
	}
	fclose(fp);

}
// ���� ��� ����
void BrightnessAdj(BYTE* Img, BYTE* Out, int W, int H, int Val) {
	int ImgSize = W * H;
	for (int i = 0; i < ImgSize; i++) {
		if (Img[i] + Val > 255) Out[i] = 255;
		else if (Img[i] + Val < 0) Out[i] = 0;
		else Out[i] = Img[i] + Val;
	}
}

// ���� ��� ����
void ContrastAdj(BYTE* Img, BYTE* Out, int W, int H, double Val) {
	int ImgSize = W * H;
	for (int i = 0; i < ImgSize; i++) {
		if (Img[i] * Val > 255.0) Out[i] = 255;
		else Out[i] = (BYTE)(Img[i] * Val);
	}
}

// ȭ�� Histogram
void ObtainHistogram(BYTE* Img, int* Histo, int W, int H) {
	int ImgSize = W * H;
	for (int i = 0; i < ImgSize; i++)  Histo[Img[i]]++;
}

// Histo�� ���� Histogram
void ObtainAHistogram(int* Histo, int* AHisto) {
	for (int i = 0; i < 256; i++) {
		for (int j = 0; j <= i; j++) {
			AHisto[i] += Histo[j];
		}
	}
}

// ��Ʈ��Ī
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

// ��Ȱȭ
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

// ���߷��� �Ӱ谪 ���ϱ�
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
	Th = (Low + High) / 2; //�ʱ� �Ӱ谪
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

// ���� ����ȭ (�Ӱ谪�� ��������)
void Binarization(BYTE* Img, BYTE* Out, int W, int H, BYTE Threshold) {
	int ImgSize = W * H;
	for (int i = 0; i < ImgSize; i++) {
		if (Img[i] < Threshold) Out[i] = 0;
		else Out[i] = 255;
	}
}

// average lowpass(���������)
void AverageConv(BYTE* Img, BYTE* Out, int W, int H) {
	double Kernel[3][3] = { 0.11111,0.11111,0.11111,
							0.11111,0.11111,0.11111,
							0.11111,0.11111,0.11111 };
	double SumProduct = 0.0;
	for (int i = 1; i < H - 1; i++) { //margin�� �ֱ����� 1���� ���� ~ H-1 ����
		for (int j = 1; j < W - 1; j++) {
			for (int m = -1; m <= 1; m++) { // 3*3 kernel ���� Height 
				for (int n = -1; n <= 1; n++) { //  Width ��Ÿ��������
					SumProduct += Img[(i + m) * W + (j + n)] * Kernel[m + 1][n + 1]; //Y*W+X
				}
			}
			Out[i * W + j] = (BYTE)SumProduct;
			SumProduct = 0;
		}
	}
}


// ����þ���Ȱȭ
void GaussAvgConv(BYTE* Img, BYTE* Out, int W, int H) {
	double Kernel[3][3] = { 0.0625,0.125,0.0625,
							0.125,0.25,0.125,
							0.0625,0.125,0.0625 };
	double SumProduct = 0.0;
	for (int i = 1; i < H - 1; i++) { //margin�� �ֱ����� 1���� ���� ~ H-1 ����
		for (int j = 1; j < W - 1; j++) {
			for (int m = -1; m <= 1; m++) { // 3*3 kernel ���� Height 
				for (int n = -1; n <= 1; n++) { //  Width ��Ÿ��������
					SumProduct += Img[(i + m) * W + (j + n)] * Kernel[m + 1][n + 1]; //Y*W+X
				}
			}
			Out[i * W + j] = (BYTE)SumProduct;
			SumProduct = 0;
		}
	}
}


// Prewitt ����ũ
void Prewitt_X_Conv(BYTE* Img, BYTE* Out, int W, int H) {
	double Kernel[3][3] = { -1.0,0.0,1.0,
							-1.0,0.0,1.0,
							-1.0,0.0,1.0 };
	double SumProduct = 0.0;
	for (int i = 1; i < H - 1; i++) { //margin�� �ֱ����� 1���� ���� ~ H-1 ����
		for (int j = 1; j < W - 1; j++) {
			for (int m = -1; m <= 1; m++) { // 3*3 kernel ���� Height 
				for (int n = -1; n <= 1; n++) { //  Width ��Ÿ��������
					SumProduct += Img[(i + m) * W + (j + n)] * Kernel[m + 1][n + 1]; //Y*W+X
				}
			}
			// ���밪 ����� 0~765 ������ ���� ���ð���
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
	for (int i = 1; i < H - 1; i++) { //margin�� �ֱ����� 1���� ���� ~ H-1 ����
		for (int j = 1; j < W - 1; j++) {
			for (int m = -1; m <= 1; m++) { // 3*3 kernel ���� Height 
				for (int n = -1; n <= 1; n++) { //  Width ��Ÿ��������
					SumProduct += Img[(i + m) * W + (j + n)] * Kernel[m + 1][n + 1]; //Y*W+X
				}
			}
			// ���밪 ����� 0~765 ������ ���� ���ð���
			Out[i * W + j] = abs((long)SumProduct) / 3;
			SumProduct = 0;
		}
	}
}

// Sobel ����ũ
void Sobel_X_Conv(BYTE* Img, BYTE* Out, int W, int H) {
	double Kernel[3][3] = { -1.0,0.0,1.0,
							-2.0,0.0,2.0,
							-1.0,0.0,1.0 };
	double SumProduct = 0.0;
	for (int i = 1; i < H - 1; i++) { //margin�� �ֱ����� 1���� ���� ~ H-1 ����
		for (int j = 1; j < W - 1; j++) {
			for (int m = -1; m <= 1; m++) { // 3*3 kernel ���� Height 
				for (int n = -1; n <= 1; n++) { //  Width ��Ÿ��������
					SumProduct += Img[(i + m) * W + (j + n)] * Kernel[m + 1][n + 1]; //Y*W+X
				}
			}
			// ���밪 ����� 0~765 ������ ���� ���ð���
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
	for (int i = 1; i < H - 1; i++) { //margin�� �ֱ����� 1���� ���� ~ H-1 ����
		for (int j = 1; j < W - 1; j++) {
			for (int m = -1; m <= 1; m++) { // 3*3 kernel ���� Height 
				for (int n = -1; n <= 1; n++) { //  Width ��Ÿ��������
					SumProduct += Img[(i + m) * W + (j + n)] * Kernel[m + 1][n + 1]; //Y*W+X
				}
			}
			// ���밪 ����� 0~765 ������ ���� ���ð���
			Out[i * W + j] = abs((long)SumProduct) / 4;
			SumProduct = 0;
		}
	}
}

//Laplace ����ũ
void Laplace_Conv(BYTE* Img, BYTE* Out, int W, int H) {
	double Kernel[3][3] = { -1.0,-1.0,-1.0,
							-1.0,8.0,-1.0,
							-1.0,-1.0,-1.0 };
	double SumProduct = 0.0;
	for (int i = 1; i < H - 1; i++) { //margin�� �ֱ����� 1���� ���� ~ H-1 ����
		for (int j = 1; j < W - 1; j++) {
			for (int m = -1; m <= 1; m++) { // 3*3 kernel ���� Height 
				for (int n = -1; n <= 1; n++) { //  Width ��Ÿ��������
					SumProduct += Img[(i + m) * W + (j + n)] * Kernel[m + 1][n + 1]; //Y*W+X
				}
			}
			// ���밪 ����� 0~765 ������ ���� ���ð���
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
	for (int i = 1; i < H - 1; i++) { //margin�� �ֱ����� 1���� ���� ~ H-1 ����
		for (int j = 1; j < W - 1; j++) {
			for (int m = -1; m <= 1; m++) { // 3*3 kernel ���� Height 
				for (int n = -1; n <= 1; n++) { //  Width ��Ÿ��������
					SumProduct += Img[(i + m) * W + (j + n)] * Kernel[m + 1][n + 1]; //Y*W+X
				}
			}
			//���� ������ ��� ����
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

// 9�� ȭ�Ұ��� ��� ������ ġȯ (��Ʈ���� ������ ����)
BYTE Median(BYTE* arr, int size) {
	const int S = size;
	for (int i = 0; i < size - 1; i++) {//pivot index
		for (int j = 0; j < size; j++) {// �񱳴�� index
			if (arr[i] > arr[j]) swap(&arr[i], &arr[j]);
		}
	}
	return arr[size / 2];
}

//9�� ȭ�Ұ��� ���� ū ������ ġȯ
BYTE MaxPooling(BYTE* arr, int size) {
	const int S = size;
	for (int i = 0; i < size - 1; i++) {//pivot index
		for (int j = 0; j < size; j++) {// �񱳴�� index
			if (arr[i] > arr[j]) swap(&arr[i], &arr[j]);
		}
	}
	return arr[S - 1];
}

//9�� ȭ�Ұ��� ���� ���� ������ ġȯ ��Ʈ�� ���۳������� �ϳ��� ���󿡼� ���� ������ �� ���
BYTE MinPooling(BYTE* arr, int size) {
	const int S = size;
	for (int i = 0; i < size - 1; i++) {//pivot index
		for (int j = 0; j < size; j++) {// �񱳴�� index
			if (arr[i] > arr[j]) swap(&arr[i], &arr[j]);
		}
	}
	return arr[0];
}


//�󺧸�
int push(short* stackx, short* stacky, int arr_size, short vx, short vy, int* top)
{
	if (*top >= arr_size) return(-1);
	(*top)++;
	stackx[*top] = vx;
	stacky[*top] = vy;
	return(1);
}

int pop(short* stackx, short* stacky, short* vx, short* vy, int* top)
{
	if (*top == 0) return(-1);
	*vx = stackx[*top];
	*vy = stacky[*top];
	(*top)--;
	return(1);
}


// GlassFire �˰����� �̿��� �󺧸� �Լ�
void m_BlobColoring(BYTE* CutImage, int height, int width)
{
	int i, j, m, n, top, area, Out_Area, index, BlobArea[1000];
	long k;
	short curColor = 0, r, c;
	//	BYTE** CutImage2;
	Out_Area = 1;


	// �������� ����� �޸� �Ҵ�
	short* stackx = new short[height * width];
	short* stacky = new short[height * width];
	short* coloring = new short[height * width];

	int arr_size = height * width;

	// �󺧸��� �ȼ��� �����ϱ� ���� �޸� �Ҵ�

	for (k = 0; k < height * width; k++) coloring[k] = 0;  // �޸� �ʱ�ȭ

	for (i = 0; i < height; i++)
	{
		index = i * width;
		for (j = 0; j < width; j++)
		{
			// �̹� �湮�� ���̰ų� �ȼ����� 255�� �ƴ϶�� ó�� ����
			if (coloring[index + j] != 0 || CutImage[index + j] != 255) continue;
			r = i; c = j; top = 0; area = 1;
			curColor++;

			while (1)
			{
			GRASSFIRE:
				for (m = r - 1; m <= r + 1; m++)
				{
					index = m * width;
					for (n = c - 1; n <= c + 1; n++)
					{
						//���� �ȼ��� �����踦 ����� ó�� ����
						if (m < 0 || m >= height || n < 0 || n >= width) continue;

						if ((int)CutImage[index + n] == 255 && coloring[index + n] == 0)
						{
							coloring[index + n] = curColor; // ���� �󺧷� ��ũ
							if (push(stackx, stacky, arr_size, (short)m, (short)n, &top) == -1) continue;
							r = m; c = n; area++;
							goto GRASSFIRE;
						}
					}
				}
				if (pop(stackx, stacky, &r, &c, &top) == -1) break;
			}
			if (curColor < 1000) BlobArea[curColor] = area;
		}
	}

	float grayGap = 255.0f / (float)curColor;
	//curColor 25�� (threshold �� ������ ������.)
	//grayGap �󸶳� ��� �����Ұ��ΰ� (10.2 ����������) 10.2 -> 20.4 ���
	//blobArea�� 1���� Ư�� ������ �� pixel�� ������ ���������� ���� �迭 component�� ũ��

	// ���� ������ ���� ������ ã�Ƴ��� ���� 
	for (i = 1; i <= curColor; i++)
	{
		if (BlobArea[i] >= BlobArea[Out_Area]) Out_Area = i;
	}
	// CutImage �迭 Ŭ����~
	for (k = 0; k < width * height; k++) CutImage[k] = 255;

	//CutImage�� Output�迭, 255�� �ʱ�ȭ��Ŵ �Ͼ�� ä��

	// coloring�� ����� �󺧸� ����� (Out_Area�� �����) ������ ���� ū �͸� CutImage�� ����
	for (k = 0; k < width * height; k++)
	{
		if (coloring[k] == Out_Area) CutImage[k] = 0;  // ���� ū �͸� ����(size filtering)
		//CutImage[k] = (unsigned char)(coloring[k] * grayGap);
		//coloring �迭�� ��������� (1~25�� ��ȯ)
		//if (BlobArea[coloring[k]] > 500) CutImage[k] = 0; //500 �̻� ������ filtering
	}

	delete[] coloring;
	delete[] stackx;
	delete[] stacky;
}
// �󺧸� �� ���� ���� ������ ���ؼ��� �̾Ƴ��� �ڵ� ����

void BinaryImageEdgeDetetion(BYTE* Bin, BYTE* Out, int W, int H) {
	//���� ū ������ �������ڵ�
	for (int i = 0; i < H; i++) {
		for (int j = 0; j < W; j++) {
			if (Bin[i * W + j] == 0) {//!(����ȭ�Ҷ��), 4���� ȭ���� �ϳ��� ������ �ƴ϶��
				if (!(Bin[(i - 1) * W + j] == 0 &&
					Bin[(i + 1) * W + j] == 0 &&
					Bin[i * W + j - 1] == 0 && Bin[i * W + j + 1] == 0)) {
					Out[i * W + j] = 255;
				}

			}
		}
	}
}


//���� �� �Ʒ��� �ٲ�
void VerticalFlip(BYTE* Img, int W, int H) {
	for (int i = 0; i < H / 2; i++) {
		for (int j = 0; j < W; j++) {
			swap(&Img[i * W + j], &Img[(H - 1 - i) * W + j]);
		}
	}
}

//���� ���� �������� �ٲ�
void HorizontalFlip(BYTE* Img, int W, int H) {
	for (int i = 0; i < W / 2; i++) {
		for (int j = 0; j < H; j++) {
			swap(&Img[j * W + i], &Img[j * W + (W - 1 - i)]);
		}
	}
}

// ���� �̵� �ٵ� y�� -1�����ִ� ������ bmp������ ���ʿ� �� �Ʒ� �ٲ�����־ ���� ���ϴ� ������ �������� -1 �������
void Translation(BYTE* Img, BYTE* Out, int W, int H, int Tx, int Ty) {
	Ty *= -1;
	for (int i = 0; i < H; i++) {
		for (int j = 0; j < W; j++) {
			if ((i + Ty < H && i + Ty >= 0) && (j + Tx < W && j + Tx >= 0)) Out[(i + Ty) * W + (j + Tx)] = Img[i * W + j];
		}
	}
}

//���� ���, Ȯ��
void Scaling(BYTE* Img, BYTE* Out, int W, int H, double SF_X, double SF_Y) {
	//SF x,y 1���� ũ�� Ȯ�� 1���� ������ ���
	int tmpX, tmpY;
	for (int i = 0; i < H; i++) {
		for (int j = 0; j < W; j++) {
			tmpX = (int)(j / SF_X), tmpY = (int)(i / SF_Y); //������ ��������ϴµ� ���������̶� ������
			if (tmpY < H && tmpX < W) { //���������ϸ� hole �̻���, �׷��� ���������� ����
				Out[i * W + j] = Img[tmpY * W + tmpX];
			}
		}
	}
}

// ���� ȸ��
void Rotation(BYTE* Img, BYTE* Out, int W, int H, int Angle) {
	int tmpX, tmpY;
	double Radian = Angle * 3.141592 / 180.0;
	for (int i = 0; i < H; i++) {
		for (int j = 0; j < W; j++) {
			tmpX = (int)(cos(Radian) * j + sin(Radian) * i);
			tmpY = (int)(-sin(Radian) * j + cos(Radian) * i); //����ķ� ����
			if ((tmpY < H && tmpY >= 0) && (tmpX < W && tmpX >= 0)) { //���������ϸ� hole �̻���, �׷��� ���������� ����
				Out[i * W + j] = Img[tmpY * W + tmpX];
			}
		}
	}
}

void MedianFiltering(BYTE* Image, BYTE* Output, int W, int H, int Size) {
	int Length = Size;  // ����ũ�� �� ���� ����
	int Margin = Length / 2;
	int Wsize = Length * Length;
	BYTE* temp = (BYTE*)malloc(sizeof(BYTE) * Wsize);

	int i, j, m, n;
	for (i = Margin; i < H - Margin; i++) {
		for (j = Margin; j < W - Margin; j++) {
			for (m = -Margin; m <= Margin; m++) {
				for (n = -Margin; n <= Margin; n++) {
					temp[(m + Margin) * Length + (n + Margin)] = Image[(i + m) * W + j + n];
				}
			}
			Output[i * W + j] = Median(temp, Wsize);
		}
	}
	free(temp);

}

void DrawCrossOutline(BYTE* Image, int W, int H, int Cx, int Cy) {
	for (int i = 0; i < W; i++) { //����
		Image[Cy * W + i] = 255;
	}
	for (int i = 0; i < H; i++) { //����
		Image[i * W + Cx] = 255;
	}
}

void DrawRectOutline(BYTE* Image, int W, int H, int LU_X, int LU_Y, int RD_X, int RD_Y) {
	for (int i = LU_X; i <= RD_X; i++) Image[LU_Y * W + i] = 255;
	for (int i = LU_X; i <= RD_X; i++) Image[RD_Y * W + i] = 255;
	for (int i = LU_Y; i <= RD_Y; i++) Image[i * W + LU_X] = 255;
	for (int i = LU_Y; i <= RD_Y; i++) Image[i * W + RD_X] = 255;
}

void DrawRectColorOutline(BYTE* Image, int W, int H, int LU_X, int LU_Y, int RD_X, int RD_Y, BYTE R, BYTE G, BYTE B) {
	for (int i = LU_X; i <= RD_X; i++) {
		Image[LU_Y * W * 3 + i * 3] = B;
		Image[LU_Y * W * 3 + i * 3 + 1] = G;
		Image[LU_Y * W * 3 + i * 3 + 2] = R;
	}
	for (int i = LU_X; i <= RD_X; i++) {
		Image[RD_Y * W * 3 + i * 3] = B;
		Image[RD_Y * W * 3 + i * 3 + 1] = G;
		Image[RD_Y * W * 3 + i * 3 + 2] = R;
	}
	for (int i = LU_Y; i <= RD_Y; i++) {
		Image[i * W * 3 + LU_X * 3] = B;
		Image[i * W * 3 + LU_X * 3 + 1] = G;
		Image[i * W * 3 + LU_X * 3 + 2] = R;
	}
	for (int i = LU_Y; i <= RD_Y; i++) {
		Image[i * W * 3 + RD_X * 3] = B;
		Image[i * W * 3 + RD_X * 3 + 1] = G;
		Image[i * W * 3 + RD_X * 3 + 2] = R;
	}
}


void Obtain2DCenter(BYTE* Temp, int W, int H, int* Cx, int* Cy) {
	int ImgSize = W * H;

	int sumX = 0, sumY = 0;
	int cnt = 0;

	for (int i = 0; i < H; i++) {
		for (int j = 0; j < W; j++) {
			if (Temp[i * W + j] == 0) {//���� �����̸�
				sumX += j;
				sumY += i;
				cnt++;
			}
		}
	}
	if (cnt == 0) cnt = 1;
	*Cx = sumX / cnt;
	*Cy = sumY / cnt;
}

void Obtain2DBoundingBox(BYTE* Temp, int W, int H, int* LUX, int* LUY, int* RDX, int* RDY) {
	int flag = 0;
	for (int i = 0; i < H; i++) {
		for (int j = 0; j < W; j++) {
			if (Temp[i * W + j] == 0) {
				*LUY = i;
				flag = 1;
				break;
			}
		}
		if (flag == 1) break;
	}

	flag = 0;
	for (int i = H - 1; i >= 0; i--) {
		for (int j = 0; j < W; j++) {
			if (Temp[i * W + j] == 0) {
				*RDY = i;
				flag = 1;
				break;
			}
		}
		if (flag == 1) break;
	}

	flag = 0;
	for (int i = 0; i < W; i++) {
		for (int j = 0; j < H; j++) {
			if (Temp[j * W + i] == 0) {
				*LUX = i;
				flag = 1;
				break;
			}
		}
		if (flag == 1) break;
	}

	flag = 0;
	for (int i = W - 1; i >= 0; i--) {
		for (int j = 0; j < H; j++) {
			if (Temp[j * W + i] == 0) {
				*RDX = i;
				flag = 1;
				break;
			}
		}
		if (flag == 1) break;
	}
}

void FillColor(BYTE* Image, int X, int Y, int W, int H, BYTE R, BYTE G, BYTE B) {
	Image[Y * W * 3 + X * 3] = B; //x,y �� 3���� ���ؾߵ� Blue����
	Image[Y * W * 3 + X * 3 + 1] = G; //Green����
	Image[Y * W * 3 + X * 3 + 2] = R; // Red����
}

void RGB2YCbCr(BYTE* Image, BYTE* Y, BYTE* Cb, BYTE* Cr, int W, int H) {
	for (int i = 0; i < H; i++) {
		for (int j = 0; j < W; j++) {
			Y[i * W + j] = (BYTE)(0.299 * Image[i * W * 3 + j * 3 + 2]) + (0.587 * Image[i * W * 3 + j * 3 + 1]) + (0.114 * Image[i * W * 3 + j * 3]);
			Cb[i * W + j] = (BYTE)(-0.1684 * Image[i * W * 3 + j * 3 + 2]) + (-0.3313 * Image[i * W * 3 + j * 3 + 1]) + (0.5 * Image[i * W * 3 + j * 3]) + 128;
			Cr[i * W + j] = (BYTE)(0.5 * Image[i * W * 3 + j * 3 + 2]) + (-0.4187 * Image[i * W * 3 + j * 3 + 1]) + (-0.0813 * Image[i * W * 3 + j * 3]) + 128;
		}
	}
}

void Erosion(BYTE* Image, BYTE* Output, int W, int H) {
	//���Ʒ����ʿ����� 4���� �˻�
	for (int i = 1; i < H - 1; i++) {
		for (int j = 1; j < W - 1; j++) {
			if (Image[i * W + j] == 255) { //����ȭ�Ҷ��
				if (!(Image[(i - 1) * W + j] == 255 && //����ȭ���ε� ��� 255���ƴ϶��, �� �׵θ����
					Image[(i + 1) * W + j] == 255 &&
					Image[i * W + j - 1] == 255 &&
					Image[i * W + j + 1] == 255)) {
					Output[i * W + j] = 0;
				}
				else Output[i * W + j] = 255;
			}
			else Output[i * W + j] = 0; //���ȭ�Ҷ��
		}
	}
}

void Dilation(BYTE* Image, BYTE* Output, int W, int H) {
	//���Ʒ����ʿ����� 4���� �˻�
	for (int i = 1; i < H - 1; i++) {
		for (int j = 1; j < W - 1; j++) {
			if (Image[i * W + j] == 0) { //���ȭ�Ҷ��
				if (!(Image[(i - 1) * W + j] == 0 && //���ȭ���ε� ��� 0���ƴ϶��, �� ����ȭ���� �׵θ����
					Image[(i + 1) * W + j] == 0 &&
					Image[i * W + j - 1] == 0 &&
					Image[i * W + j + 1] == 0)) {
					Output[i * W + j] = 255;
				}
				else Output[i * W + j] = 0;
			}
			else Output[i * W + j] = 255; //����ȭ�Ҷ��
		}
	}
}

// 2���� �迭 �����Ҵ� ����
unsigned char** imageMatrix;
// �������󿡼� 
unsigned char blankPixel = 255, imagePixel = 0;

typedef struct {
	int row, col;
}pixel;

int getBlackNeighbours(int row, int col) {

	int i, j, sum = 0;

	for (i = -1; i <= 1; i++) {
		for (j = -1; j <= 1; j++) {
			if (i != 0 || j != 0)
				sum += (imageMatrix[row + i][col + j] == imagePixel);
		}
	}

	return sum;
}

int getBWTransitions(int row, int col) {
	return 	((imageMatrix[row - 1][col] == blankPixel && imageMatrix[row - 1][col + 1] == imagePixel)
		+ (imageMatrix[row - 1][col + 1] == blankPixel && imageMatrix[row][col + 1] == imagePixel)
		+ (imageMatrix[row][col + 1] == blankPixel && imageMatrix[row + 1][col + 1] == imagePixel)
		+ (imageMatrix[row + 1][col + 1] == blankPixel && imageMatrix[row + 1][col] == imagePixel)
		+ (imageMatrix[row + 1][col] == blankPixel && imageMatrix[row + 1][col - 1] == imagePixel)
		+ (imageMatrix[row + 1][col - 1] == blankPixel && imageMatrix[row][col - 1] == imagePixel)
		+ (imageMatrix[row][col - 1] == blankPixel && imageMatrix[row - 1][col - 1] == imagePixel)
		+ (imageMatrix[row - 1][col - 1] == blankPixel && imageMatrix[row - 1][col] == imagePixel));
}

int zhangSuenTest1(int row, int col) {
	int neighbours = getBlackNeighbours(row, col);

	return ((neighbours >= 2 && neighbours <= 6)
		&& (getBWTransitions(row, col) == 1)
		&& (imageMatrix[row - 1][col] == blankPixel || imageMatrix[row][col + 1] == blankPixel || imageMatrix[row + 1][col] == blankPixel)
		&& (imageMatrix[row][col + 1] == blankPixel || imageMatrix[row + 1][col] == blankPixel || imageMatrix[row][col - 1] == blankPixel));
}

int zhangSuenTest2(int row, int col) {
	int neighbours = getBlackNeighbours(row, col);

	return ((neighbours >= 2 && neighbours <= 6)
		&& (getBWTransitions(row, col) == 1)
		&& (imageMatrix[row - 1][col] == blankPixel || imageMatrix[row][col + 1] == blankPixel || imageMatrix[row][col - 1] == blankPixel)
		&& (imageMatrix[row - 1][col] == blankPixel || imageMatrix[row + 1][col] == blankPixel || imageMatrix[row][col + 1] == blankPixel));
}

void zhangSuen(unsigned char* image, unsigned char* output, int rows, int cols) {

	int startRow = 1, startCol = 1, endRow, endCol, i, j, count, processed;

	pixel* markers;

	imageMatrix = (unsigned char**)malloc(rows * sizeof(unsigned char*));

	for (i = 0; i < rows; i++) {
		imageMatrix[i] = (unsigned char*)malloc((cols + 1) * sizeof(unsigned char));
		for (int k = 0; k < cols; k++) imageMatrix[i][k] = image[i * cols + k];
	}

	endRow = rows - 2;
	endCol = cols - 2;
	do {
		markers = (pixel*)malloc((endRow - startRow + 1) * (endCol - startCol + 1) * sizeof(pixel));
		count = 0;

		for (i = startRow; i <= endRow; i++) {
			for (j = startCol; j <= endCol; j++) {
				if (imageMatrix[i][j] == imagePixel && zhangSuenTest1(i, j) == 1) {
					markers[count].row = i;
					markers[count].col = j;
					count++;
				}
			}
		}

		processed = (count > 0);

		for (i = 0; i < count; i++) {
			imageMatrix[markers[i].row][markers[i].col] = blankPixel;
		}

		free(markers);
		markers = (pixel*)malloc((endRow - startRow + 1) * (endCol - startCol + 1) * sizeof(pixel));
		count = 0;

		for (i = startRow; i <= endRow; i++) {
			for (j = startCol; j <= endCol; j++) {
				if (imageMatrix[i][j] == imagePixel && zhangSuenTest2(i, j) == 1) {
					markers[count].row = i;
					markers[count].col = j;
					count++;
				}
			}
		}

		if (processed == 0)
			processed = (count > 0);

		for (i = 0; i < count; i++) {
			imageMatrix[markers[i].row][markers[i].col] = blankPixel;
		}

		free(markers);
	} while (processed == 1);


	for (i = 0; i < rows; i++) {
		for (j = 0; j < cols; j++) {
			output[i * cols + j] = imageMatrix[i][j];
		}
	}
}

void FeatureExtractThinImage(BYTE* Image, BYTE* Output, int W, int H) {
	for (int i = 0; i < W * H; i++) Output[i] = Image[i];
	int cnt = 0;
	for (int i = 1; i < H - 1; i++) {
		for (int j = 1; j < W - 1; j++) {
			if (Image[i*W+j]==0){ //����ȭ�Ҷ��
				if (Image[(i+1) * W + j+1] == 0) cnt++;
				if (Image[(i + 1) * W + j] == 0) cnt++;
				if (Image[(i + 1) * W + j-1] == 0) cnt++;
				if (Image[i * W + j+1] == 0) cnt++;
				if (Image[i * W + j-1] == 0) cnt++;
				if (Image[(i - 1) * W + j+1] == 0) cnt++;
				if (Image[(i - 1) * W + j] == 0) cnt++;
				if (Image[(i - 1) * W + j-1] == 0) cnt++;
			}
			if (cnt == 1) Output[i * W + j] = 128; //����
			else if (cnt >= 3) Output[i * W + j] = 128; //�б���
 			cnt = 0;
		}
	}
}

void FeatureExtractThinImage2(BYTE* Image, BYTE* Output, int W, int H) {
	for (int i = 0; i < W * H; i++) Output[i] = Image[i];
	int cnt = 0;
	for (int i = 1; i < H - 1; i++) {
		for (int j = 1; j < W - 1; j++) {
			if (Image[i * W + j] == 0) { //����ȭ�Ҷ��, 0->255 �ٲ�� �κп� ���ؼ� check! ��¥�б��� ���� �� ����
				if (Image[(i - 1) * W + j - 1] == 0 && Image[(i - 1) * W + j] == 255) cnt++;
				if (Image[(i - 1) * W + j] == 0 && Image[(i - 1) * W + j + 1] == 255) cnt++;
				if (Image[(i - 1) * W + j + 1] == 0 && Image[i * W + j + 1] == 255) cnt++;
				if (Image[i * W + j + 1] == 0 && Image[(i + 1) * W + j + 1] == 255) cnt++;
				if (Image[(i + 1) * W + j + 1] == 0 && Image[(i + 1) * W + j] == 255) cnt++;
				if (Image[(i + 1) * W + j] == 0 && Image[(i + 1) * W + j - 1] == 255) cnt++;
				if (Image[(i + 1) * W + j - 1] == 0 && Image[i * W + j - 1] == 255) cnt++;
				if (Image[i * W + j - 1] == 0 && Image[(i - 1) * W + j - 1] == 255) cnt++;
			}
			if (cnt == 1) Output[i * W + j] = 128; //����
			else if (cnt >= 3) Output[i * W + j] = 128; //�б���
			cnt = 0;
		}
	}
}


int main() {

	//�����Է�
	BITMAPFILEHEADER hf;
	BITMAPINFOHEADER hInfo;
	RGBQUAD hRGB[256];
	FILE* fp;

	fp = fopen("dilation.bmp", "rb");
	if (fp == NULL) {
		printf("file not found!\n");
		return -1;
	}
	BYTE* Image;
	BYTE* Output;
	BYTE* Temp;


	fread(&hf, sizeof(BITMAPFILEHEADER), 1, fp); //14byte�� 1�� �о�ͼ� hf�� �����ض�
	fread(&hInfo, sizeof(BITMAPINFOHEADER), 1, fp);
	int W = hInfo.biWidth, H = hInfo.biHeight;
	int ImgSize = hInfo.biWidth * hInfo.biHeight;
	if (hInfo.biBitCount == 24) { //Ʈ���÷�
		Image = (BYTE*)malloc(ImgSize * 3);
		Output = (BYTE*)malloc(ImgSize * 3);
		Temp = (BYTE*)malloc(ImgSize * 3);
		fread(Image, sizeof(BYTE), ImgSize * 3, fp);
	}
	else { //�ε���(�׷���)
		fread(hRGB, sizeof(RGBQUAD), 256, fp);
		Image = (BYTE*)malloc(ImgSize);
		Output = (BYTE*)malloc(ImgSize);
		Temp = (BYTE*)malloc(ImgSize);
		fread(Image, sizeof(BYTE), ImgSize, fp);
	}
	fclose(fp);

	int Histo[256] = { 0 };
	int AHisto[256] = { 0 };
	//����ó��
	/*InverseImage(Image, Output, W, H);
	BrightnessAdj(Image, Output, W, H, 70);
	ContrastAdj(Image, Output, W, H, 1.5);*/

	// ������׷�
	ObtainHistogram(Image, Histo, W, H);
	// ���� ������׷�
	ObtainAHistogram(Histo, AHisto);

	// 9����
	//median filtering
	//MedianFiltering(Image, Output, W, H, 5);

	//�������� ����ū ��������
	//Binarization(Image, Temp, W, H, 30);
	//InverseImage(Temp, Temp, W, H);
	//m_BlobColoring(Temp, H, W);

	//for (int i = 0; i < ImgSize; i++) Output[i] = Image[i];

	//int Cx, Cy;
	//Obtain2DCenter(Temp, W, H, &Cx, &Cy); //���� ������ �����߽�
	//int LUX, LUY, RDX, RDY;
	//Obtain2DBoundingBox(Temp, W, H, &LUX, &LUY, &RDX, &RDY); //���� ������ �����簢�� ��ǥ ����
	//DrawRectOutline(Output, W, H, LUX, LUY, RDX, RDY);
	//DrawCrossOutline(Output, W, H, Cx, Cy);


	//10����
	//Ư����ġ�� ���� ä���
	/*for (int i = 0; i < W; i++) {
		FillColor(Image, i, 200, W, H, 0, 255, 255);
	}*/

	//50,100 ~300,400 ä���
	/*for (int i = 100; i <= 400; i++) {
		for (int j = 50; j <= 300; j++) {
			FillColor(Image, j, i, W, H, 255, 0, 255);
		}
	}*/

	//���� �� �����
	//�ʱ�ȭ
	//for (int i = 0; i < H; i++) {
	//	for (int j = 0; j < W; j++) {
	//		Image[i * W * 3 + j * 3 ] = 0;
	//		Image[i * W * 3 + j * 3 + 1] = 0;
	//		Image[i * W * 3 + j * 3 + 2] = 0;
	//	}
	//}
	////y��ǥ ���� 0~240 Red
	//for (int i = 0; i < 240; i++) {
	//	for (int j = 0; j < W; j++) {
	//		Image[i * W * 3 + j * 3 + 2] = 255;
	//	}
	//}
	////y��ǥ ���� 120~360 Green
	//for (int i = 120; i < 360; i++) {
	//	for (int j = 0; j < W; j++) {
	//		Image[i * W * 3 + j * 3 + 1] = 255;
	//	}
	//}
	////y��ǥ ���� 240~480 Blue
	//for (int i = 240; i < 480; i++) {
	//	for (int j = 0; j < W; j++) {
	//		Image[i * W * 3 + j * 3] = 255;
	//	}
	//}

	// �׶��̼� �� ����� blue->Yellow
	//double wt;
	//for (int a = 320; a <= 480; a++) {
	//	for (int i = 0; i < W; i++) {
	//		wt = i / (double)(W - 1);
	//		Image[a * W * 3 + i * 3] = (BYTE)255 * (1 - wt);
	//		Image[a * W * 3 + i * 3 + 1] = (BYTE)255 * wt;
	//		Image[a * W * 3 + i * 3 + 2] = (BYTE)255 * wt;
	//	}
	//}
	//// �׶��̼� �� ����� green->Magenta
	//for (int a = 160; a <= 320; a++) {
	//	for (int i = 0; i < W; i++) {
	//		wt = i / (double)(W - 1);
	//		Image[a * W * 3 + i * 3] = (BYTE)255 * wt;
	//		Image[a * W * 3 + i * 3 + 1] = (BYTE)255 * (1 - wt);
	//		Image[a * W * 3 + i * 3 + 2] = (BYTE)255 * wt;
	//	}
	//}
	//// �׶��̼� �� ����� red->Cyan
	//for (int a = 0; a <= 160; a++) {
	//	for (int i = 0; i < W; i++) {
	//		wt = i / (double)(W - 1);
	//		Image[a * W * 3 + i * 3] = (BYTE)255 * wt;
	//		Image[a * W * 3 + i * 3 + 1] = (BYTE)255 * wt;
	//		Image[a * W * 3 + i * 3 + 2] = (BYTE)255 * (1 - wt);
	//	}
	//}

	//VerticalFlip(Image, W * 3, H);


	//11����
	////Red����ū ȭ�Ҹ� masking
	//for (int i = 0; i < H; i++) {
	//	for (int j = 0; j < W; j++) {
	//		if (Image[i * W * 3 + j * 3 + 2] > 140 && 
	//			Image[i * W * 3 + j * 3 + 1] < 50 && 
	//			Image[i * W * 3 + j * 3 ] < 100) {
	//			Output[i * W * 3 + j * 3 ] = Image[i * W * 3 + j * 3];
	//			Output[i * W * 3 + j * 3 + 1] = Image[i * W * 3 + j * 3 + 1];
	//			Output[i * W * 3 + j * 3 + 2] = Image[i * W * 3 + j * 3 + 2];
	//		}
	//		else {
	//			Output[i * W * 3 + j * 3] = Output[i * W * 3 + j * 3 + 1] = Output[i * W * 3 + j * 3 + 2] = 0;
	//		}
	//	}
	//}

	/*BYTE* Y = (BYTE*)malloc(ImgSize);
	BYTE* Cb = (BYTE*)malloc(ImgSize);
	BYTE* Cr = (BYTE*)malloc(ImgSize);
	RGB2YCbCr(Image, Y, Cb, Cr, W, H);*/

	//������ ���� ������ masking
	///*for (int i = 0; i < H; i++) {
	//	for (int j = 0; j < W; j++) {
	//		if (Cb[i * W + j] < 140 && Cr[i * W + j]>200) {
	//			Output[i * W * 3 + j * 3] = Image[i * W * 3 + j * 3];
	//			Output[i * W * 3 + j * 3 + 1] = Image[i * W * 3 + j * 3 + 1];
	//			Output[i * W * 3 + j * 3 + 2] = Image[i * W * 3 + j * 3 + 2];
	//		}
	//		else {
	//			Output[i * W * 3 + j * 3] = Output[i * W * 3 + j * 3 + 1] = Output[i * W * 3 + j * 3 + 2] = 0;
	//		}
	//	}
	//}*/


	// �Ǻλ� ������ masking
	//for (int i = 0; i < H; i++) {
	//	for (int j = 0; j < W; j++) {
	//		if (Cb[i * W + j] > 95 && Cb[i * W + j] < 125 &&
	//			Cr[i * W + j]>135 && Cr[i * W + j] < 205) {
	//			Output[i * W * 3 + j * 3] = Image[i * W * 3 + j * 3];
	//			Output[i * W * 3 + j * 3 + 1] = Image[i * W * 3 + j * 3 + 1];
	//			Output[i * W * 3 + j * 3 + 2] = Image[i * W * 3 + j * 3 + 2];
	//		}
	//		else {
	//			Output[i * W * 3 + j * 3] = Output[i * W * 3 + j * 3 + 1] = Output[i * W * 3 + j * 3 + 2] = 0;
	//		}
	//	}
	//}

	//for (int i = 0; i < H; i++) {
	//	for (int j = 0; j < W; j++) {
	//		if (Output[i * W * 3 + j * 3] == 0) {
	//			Temp[i * W + j] = 0;
	//		}
	//		else {
	//			Temp[i * W + j] = 255;
	//		}
	//	}
	//}
	//m_BlobColoring(Temp, H, W); //�󺧸� ��� ���� ū ������ ����
	//int LUX, LUY, RDX, RDY;
	//Obtain2DBoundingBox(Temp, W, H, &LUX, &LUY, &RDX, &RDY);
	//DrawRectColorOutline(Image, W, H, LUX, LUY, RDX, RDY, 255, 0, 0);


	//fp = fopen("Y.raw", "wb");
	//fwrite(Y, sizeof(BYTE), W*H, fp); //binary file�� byte ������ �ǹ̰� ����, �׷��� �о�ö��� �ٸ��� �� ���� byte ������ ��� �� �ǹ�����
	//fclose(fp);
	//fp = fopen("Cb.raw", "wb");
	//fwrite(Cb, sizeof(BYTE), W* H, fp); //binary file�� byte ������ �ǹ̰� ����, �׷��� �о�ö��� �ٸ��� �� ���� byte ������ ��� �� �ǹ�����
	//fclose(fp);
	//fp = fopen("Cr.raw", "wb");
	//fwrite(Cr, sizeof(BYTE), W* H, fp); //binary file�� byte ������ �ǹ̰� ����, �׷��� �о�ö��� �ٸ��� �� ���� byte ������ ��� �� �ǹ�����
	//fclose(fp);
	//free(Y);
	//free(Cb);
	//free(Cr);



	//12����
	/*Erosion(Image, Output, W, H);
	Erosion(Output, Image, W, H);
	Erosion(Image, Output, W, H);
	Erosion(Output, Image, W, H);
	Erosion(Image, Output, W, H);*/

	Dilation(Image, Output, W, H);
	Dilation(Output, Image, W, H);
	Dilation(Image, Output, W, H);
	Erosion(Output, Image, W, H);
	Erosion(Image, Output, W, H);
	Erosion(Output, Image, W, H);
	//��â �� ħ�� ->Closing

	//���ȭ(���������� ����), ����ȭ(������ �뷫���� ���) -> �β��� 1pixel�� line���°� ��
	//����ȭ - >�����ν�, ������ �б����� ������ ã���� ���
	InverseImage(Image, Image, W, H);
	zhangSuen(Image, Output, H, W);
	//�б���, ���� gray�� ó��
	FeatureExtractThinImage2(Output, Image, W, H);




	//������
	SaveBMPFile(hf, hInfo, hRGB, Image, W, H, "output_erosion_thining2.bmp");

	
	free(Image);
	free(Output);
	free(Temp);
	return 0;
}