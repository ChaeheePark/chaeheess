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
	fwrite(hRGB, sizeof(RGBQUAD), 256, fp);
	fwrite(Out, sizeof(BYTE), W * H, fp);
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

void DrawRectOutline(BYTE* Img, int W, int H, int LU_X, int LU_Y, int RD_X, int RD_Y) {
	// Img: �簢���� �׸� �̹����迭, W: ���� ���λ�����, H: ���� ���λ�����,
	// LU_X: �簢���� ������� X��ǥ, LU_Y: �簢���� ������� Y��ǥ,
	// RD_X: �簢���� �����ϴ� X��ǥ, LU_Y: �簢���� �����ϴ� Y��ǥ.
	for (int j = LU_X; j <= RD_X; j++) {
		Img[LU_Y * W + j] = 255;
	}
	for (int j = LU_X; j <= RD_X; j++) {
		Img[RD_Y * W + j] = 255;
	}
	for (int i = LU_Y; i <= RD_Y; i++) {
		Img[i * W + LU_X] = 255;
	}
	for (int i = LU_Y; i <= RD_Y; i++) {
		Img[i * W + RD_X] = 255;
	}
}

void DrawCrossLine(BYTE* Img, int W, int H, int Cx, int Cy) {
	// Img: ����/���� ������ �׸� �̹����迭, W: ���� ���λ�����, H: ���� ���λ�����,
	// Cx: ����/���� ������ �����Ǵ� ������ X��ǥ
	// Cy: ����/���� ������ �����Ǵ� ������ Y��ǥ
	for (int i = 0; i <= W; i++) {
		Img[Cy * W + i] = 255;
	}
	for (int j = 0; j <= H; j++) {
		Img[j * W + Cx] = 255;
	}
}

//���� �� �Ʒ��� �ٲ�
void VerticalFlip(BYTE* Img, int W, int H) {
	for (int i = 0; i < H / 2; i++) {
		for (int j = 0; j < W; j++) {
			swap(&Img[i * W + j], &Img[(H - 1-i) * W + j]);
		}
	}
}

//���� ���� �������� �ٲ�
void HorizontalFlip(BYTE* Img, int W, int H) {
	for (int i = 0; i < W / 2; i++) {
		for (int j = 0; j < H; j++) {
			swap(&Img[j * W + i], &Img[ j * W + (W-1-i)]);
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
			if ((tmpY < H && tmpY>=0) && (tmpX < W && tmpX>=0)) { //���������ϸ� hole �̻���, �׷��� ���������� ����
				Out[i * W + j] = Img[tmpY * W + tmpX];
			}
		}
	}
}


int main() {

	//�����Է�
	BITMAPFILEHEADER hf;
	BITMAPINFOHEADER hInfo;
	RGBQUAD hRGB[256];
	FILE* fp;

	fp = fopen("lenna.bmp", "rb");
	if (fp == NULL) {
		printf("file not found!\n");
		return -1;
	}

	fread(&hf, sizeof(BITMAPFILEHEADER), 1, fp); //14byte�� 1�� �о�ͼ� hf�� �����ض�
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
	//����ó��

	/*InverseImage(Image, Output, W, H);
	BrightnessAdj(Image, Output, W, H, 70);
	ContrastAdj(Image, Output, W, H, 1.5);*/

	// ������׷�
	ObtainHistogram(Image, Histo, W, H);
	// ���� ������׷�
	ObtainAHistogram(Histo, AHisto);
	
	//VerticalFlip(Image, W, H);
	//HorizontalFlip(Image, W, H);
	//Translation(Image, Output, W, H, 100, 40);
	//Scaling(Image, Output, W, H, 0.7, 0.7); //Uniform scaling 
	Rotation(Image, Output, W, H, 30);


	//��Ȱȭ
	//AverageConv(Image, Output, W, H);
	//GaussAvgConv(Image, Output, W, H);

	////������ �� ����ȭ
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

	//��Ȱȭ �� ������
	//Laplace_Conv(Image, Output, W, H);
	//GaussAvgConv(Image, Output, W, H);
	//Laplace_Conv_DC(Image, Output, W, H);



	//��Ʈ��Ī
	//HistogramStretching(Image, Output, Histo, W, H);

	//��Ȱȭ
	//HistogramEqualization(Image, Output, AHisto, W, H);

	/*����ȭ
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

	///* median filtering*/
	//int length = 3;  // ����ũ�� �� ���� ����
	//int margin = length / 2;
	//int wsize = length * length;
	//byte* temp = (byte*)malloc(sizeof(byte) * wsize);

	//int i, j, m, n;
	//for (i = margin; i < h - margin; i++) {
	//	for (j = margin; j < w - margin; j++) {
	//		for (m = -margin; m <= margin; m++) {
	//			for (n = -margin; n <= margin; n++) {
	//				temp[(m + margin) * length + (n + margin)] = image[(i + m) * w + j + n];
	//			}
	//		}
	//		temp[i * w + j] = median(temp, wsize);
	//	}
	//}
	//free(temp);
	//

	//����ū ��������
	//Binarization(Image, Temp, W, H, 50);
	//InverseImage(Temp, Temp, W, H);
	//m_BlobColoring(Temp, H, W);
	//for (int i = 0; i < ImgSize; i++) Output[i] = Image[i];
	//BinaryImageEdgeDetetion(Temp, Output, W, H);

	//int y1 = 0, y2 = 0, x1 = 0, x2 = 0;
	////x1, x2, y1, y2 ���ϱ�
	//for (int i = 0; i < ImgSize; i++) {
	//	if (Temp[i] == 0) {
	//		y1 = (int)i / W;
	//		break;
	//	}
	//}
	//for (int i = ImgSize; i >= 0; i--) {
	//	if (Temp[i] == 0) {
	//		y2 = (int)i / W;
	//		break;
	//	}
	//}
	//for (int i = 0; i < ImgSize; i++) {
	//	if (Temp[(i * W) % (ImgSize - 1)] == 0) {
	//		x1 = (i * W) % (ImgSize - 1) % W;
	//		break;
	//	}
	//}
	//for (int i = ImgSize; i >= 0; i--) {
	//	if (Temp[(i * W) % (ImgSize - 1)] == 0) {
	//		x2 = (i * W) % (ImgSize - 1) % W;
	//		break;
	//	}
	//}
	////printf("%d, %d, %d,  %d\n",x1,x2,y1,y2);
	//DrawRectOutline(Output, W, H, x1, y1, x2, y2);

	////cx, cy���ϱ�
	//int sumx = 0, sumy = 0, count = 0;
	//for (int i = x1; i <= x2; i++) {
	//	for (int j = y1; j <= y2; j++) {
	//		if (Temp[j * W + i] == 0) {
	//			sumx += i;
	//			sumy += j;
	//			count++;
	//		}
	//	}
	//}

	////printf("%d,%d\n", sumx, sumy);
	//int cx = (int)sumx / count;
	//int cy = (int)sumy / count;
	////printf("%d, %d", cx, cy);
	//DrawCrossLine(Output, W, H, cx, cy);


	//������
	SaveBMPFile(hf, hInfo, hRGB, Output, W, H, "outputt.bmp");
	free(Image);
	free(Output);
	free(Temp);
	return 0;
}