#pragma warning (disable:4996)
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <math.h>

// 영상 화소 값 반전
void InverseImage(BYTE* Img, BYTE* Out, int W, int H) {
	int ImgSize = W * H;
	for (int i = 0; i < ImgSize; i++) {
		Out[i] = 255 - Img[i];
	}
}

void SaveBMPFile(BITMAPFILEHEADER hf, BITMAPINFOHEADER hInfo, RGBQUAD* hRGB,
	BYTE* Out, int W, int H, const char* FileName) {
	FILE* fp = fopen(FileName, "wb");
	fwrite(&hf, sizeof(BYTE), sizeof(BITMAPFILEHEADER), fp); //binary file은 byte 단위로 의미가 있음, 그래서 읽어올때와 다르게 쓸 때는 byte 단위로 써야 더 의미있음
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
void Prewitt_X_Conv(BYTE* Img, BYTE* Out, int W, int H) {
	double Kernel[3][3] = { -1.0,0.0,1.0,
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
	return arr[size / 2];
}

//9개 화소값중 가장 큰 값으로 치환
BYTE MaxPooling(BYTE* arr, int size) {
	const int S = size;
	for (int i = 0; i < size - 1; i++) {//pivot index
		for (int j = 0; j < size; j++) {// 비교대상 index
			if (arr[i] > arr[j]) swap(&arr[i], &arr[j]);
		}
	}
	return arr[S - 1];
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


//라벨링
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


// GlassFire 알고리즘을 이용한 라벨링 함수
void m_BlobColoring(BYTE* CutImage, int height, int width)
{
	int i, j, m, n, top, area, Out_Area, index, BlobArea[1000];
	long k;
	short curColor = 0, r, c;
	//	BYTE** CutImage2;
	Out_Area = 1;


	// 스택으로 사용할 메모리 할당
	short* stackx = new short[height * width];
	short* stacky = new short[height * width];
	short* coloring = new short[height * width];

	int arr_size = height * width;

	// 라벨링된 픽셀을 저장하기 위해 메모리 할당

	for (k = 0; k < height * width; k++) coloring[k] = 0;  // 메모리 초기화

	for (i = 0; i < height; i++)
	{
		index = i * width;
		for (j = 0; j < width; j++)
		{
			// 이미 방문한 점이거나 픽셀값이 255가 아니라면 처리 안함
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
						//관심 픽셀이 영상경계를 벗어나면 처리 안함
						if (m < 0 || m >= height || n < 0 || n >= width) continue;

						if ((int)CutImage[index + n] == 255 && coloring[index + n] == 0)
						{
							coloring[index + n] = curColor; // 현재 라벨로 마크
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
	//curColor 25개 (threshold 로 영역을 나눈것.)
	//grayGap 얼마나 밝기 단위할것인가 (10.2 간격정도로) 10.2 -> 20.4 등등
	//blobArea는 1부터 특정 영역이 몇 pixel의 면적을 가지는지에 대한 배열 component의 크기

	// 가장 면적이 넓은 영역을 찾아내기 위함 
	for (i = 1; i <= curColor; i++)
	{
		if (BlobArea[i] >= BlobArea[Out_Area]) Out_Area = i;
	}
	// CutImage 배열 클리어~
	for (k = 0; k < width * height; k++) CutImage[k] = 255;

	//CutImage는 Output배열, 255로 초기화시킴 하얗게 채움

	// coloring에 저장된 라벨링 결과중 (Out_Area에 저장된) 영역이 가장 큰 것만 CutImage에 저장
	for (k = 0; k < width * height; k++)
	{
		if (coloring[k] == Out_Area) CutImage[k] = 0;  // 가장 큰 것만 저장(size filtering)
		//CutImage[k] = (unsigned char)(coloring[k] * grayGap);
		//coloring 배열은 몇변영역인지 (1~25를 반환)
		//if (BlobArea[coloring[k]] > 500) CutImage[k] = 0; //500 이상 영역만 filtering
	}

	delete[] coloring;
	delete[] stackx;
	delete[] stacky;
}
// 라벨링 후 가장 넓은 영역에 대해서만 뽑아내는 코드 포함

void BinaryImageEdgeDetetion(BYTE* Bin, BYTE* Out, int W, int H) {
	//가장 큰 면적의 경계출력코드
	for (int i = 0; i < H; i++) {
		for (int j = 0; j < W; j++) {
			if (Bin[i * W + j] == 0) {//!(전경화소라면), 4방향 화소중 하나라도 전경이 아니라면
				if (!(Bin[(i - 1) * W + j] == 0 &&
					Bin[(i + 1) * W + j] == 0 &&
					Bin[i * W + j - 1] == 0 && Bin[i * W + j + 1] == 0)) {
					Out[i * W + j] = 255;
				}

			}
		}
	}
}


//영상 위 아래를 바꿈
void VerticalFlip(BYTE* Img, int W, int H) {
	for (int i = 0; i < H / 2; i++) {
		for (int j = 0; j < W; j++) {
			swap(&Img[i * W + j], &Img[(H - 1 - i) * W + j]);
		}
	}
}

//영상 왼쪽 오른쪽을 바꿈
void HorizontalFlip(BYTE* Img, int W, int H) {
	for (int i = 0; i < W / 2; i++) {
		for (int j = 0; j < H; j++) {
			swap(&Img[j * W + i], &Img[j * W + (W - 1 - i)]);
		}
	}
}

// 영상 이동 근데 y에 -1곱해주는 이유는 bmp파일이 애초에 위 아래 바뀌어져있어서 내가 원하는 방향을 얻으려면 -1 해줘야함
void Translation(BYTE* Img, BYTE* Out, int W, int H, int Tx, int Ty) {
	Ty *= -1;
	for (int i = 0; i < H; i++) {
		for (int j = 0; j < W; j++) {
			if ((i + Ty < H && i + Ty >= 0) && (j + Tx < W && j + Tx >= 0)) Out[(i + Ty) * W + (j + Tx)] = Img[i * W + j];
		}
	}
}

//영상 축소, 확대
void Scaling(BYTE* Img, BYTE* Out, int W, int H, double SF_X, double SF_Y) {
	//SF x,y 1보다 크면 확대 1보다 작으면 축소
	int tmpX, tmpY;
	for (int i = 0; i < H; i++) {
		for (int j = 0; j < W; j++) {
			tmpX = (int)(j / SF_X), tmpY = (int)(i / SF_Y); //원래는 곱해줘야하는데 역방향사상이라서 나눠줌
			if (tmpY < H && tmpX < W) { //순방향사상하면 hole 이생김, 그래서 역방향사상을 진행
				Out[i * W + j] = Img[tmpY * W + tmpX];
			}
		}
	}
}

// 영상 회전
void Rotation(BYTE* Img, BYTE* Out, int W, int H, int Angle) {
	int tmpX, tmpY;
	double Radian = Angle * 3.141592 / 180.0;
	for (int i = 0; i < H; i++) {
		for (int j = 0; j < W; j++) {
			tmpX = (int)(cos(Radian) * j + sin(Radian) * i);
			tmpY = (int)(-sin(Radian) * j + cos(Radian) * i); //역행렬로 구성
			if ((tmpY < H && tmpY >= 0) && (tmpX < W && tmpX >= 0)) { //순방향사상하면 hole 이생김, 그래서 역방향사상을 진행
				Out[i * W + j] = Img[tmpY * W + tmpX];
			}
		}
	}
}

void MedianFiltering(BYTE* Image, BYTE* Output, int W, int H, int Size) {
	int Length = Size;  // 마스크의 한 변의 길이
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
	for (int i = 0; i < W; i++) { //가로
		Image[Cy * W + i] = 255;
	}
	for (int i = 0; i < H; i++) { //세로
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
			if (Temp[i * W + j] == 0) {//동공 영역이면
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
	Image[Y * W * 3 + X * 3] = B; //x,y 를 3개씩 곱해야됨 Blue성분
	Image[Y * W * 3 + X * 3 + 1] = G; //Green성분
	Image[Y * W * 3 + X * 3 + 2] = R; // Red성분
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
	//위아래왼쪽오른쪽 4방향 검사
	for (int i = 1; i < H - 1; i++) {
		for (int j = 1; j < W - 1; j++) {
			if (Image[i * W + j] == 255) { //전경화소라면
				if (!(Image[(i - 1) * W + j] == 255 && //전경화소인데 모두 255가아니라면, 즉 테두리라면
					Image[(i + 1) * W + j] == 255 &&
					Image[i * W + j - 1] == 255 &&
					Image[i * W + j + 1] == 255)) {
					Output[i * W + j] = 0;
				}
				else Output[i * W + j] = 255;
			}
			else Output[i * W + j] = 0; //배경화소라면
		}
	}
}

void Dilation(BYTE* Image, BYTE* Output, int W, int H) {
	//위아래왼쪽오른쪽 4방향 검사
	for (int i = 1; i < H - 1; i++) {
		for (int j = 1; j < W - 1; j++) {
			if (Image[i * W + j] == 0) { //배경화소라면
				if (!(Image[(i - 1) * W + j] == 0 && //배경화소인데 모두 0가아니라면, 즉 전경화소의 테두리라면
					Image[(i + 1) * W + j] == 0 &&
					Image[i * W + j - 1] == 0 &&
					Image[i * W + j + 1] == 0)) {
					Output[i * W + j] = 255;
				}
				else Output[i * W + j] = 0;
			}
			else Output[i * W + j] = 255; //전경화소라면
		}
	}
}

// 2차원 배열 동적할당 위함
unsigned char** imageMatrix;
// 이진영상에서 
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
			if (Image[i*W+j]==0){ //전경화소라면
				if (Image[(i+1) * W + j+1] == 0) cnt++;
				if (Image[(i + 1) * W + j] == 0) cnt++;
				if (Image[(i + 1) * W + j-1] == 0) cnt++;
				if (Image[i * W + j+1] == 0) cnt++;
				if (Image[i * W + j-1] == 0) cnt++;
				if (Image[(i - 1) * W + j+1] == 0) cnt++;
				if (Image[(i - 1) * W + j] == 0) cnt++;
				if (Image[(i - 1) * W + j-1] == 0) cnt++;
			}
			if (cnt == 1) Output[i * W + j] = 128; //끝점
			else if (cnt >= 3) Output[i * W + j] = 128; //분기점
 			cnt = 0;
		}
	}
}

void FeatureExtractThinImage2(BYTE* Image, BYTE* Output, int W, int H) {
	for (int i = 0; i < W * H; i++) Output[i] = Image[i];
	int cnt = 0;
	for (int i = 1; i < H - 1; i++) {
		for (int j = 1; j < W - 1; j++) {
			if (Image[i * W + j] == 0) { //전경화소라면, 0->255 바뀌는 부분에 대해서 check! 진짜분기점 얻을 수 있음
				if (Image[(i - 1) * W + j - 1] == 0 && Image[(i - 1) * W + j] == 255) cnt++;
				if (Image[(i - 1) * W + j] == 0 && Image[(i - 1) * W + j + 1] == 255) cnt++;
				if (Image[(i - 1) * W + j + 1] == 0 && Image[i * W + j + 1] == 255) cnt++;
				if (Image[i * W + j + 1] == 0 && Image[(i + 1) * W + j + 1] == 255) cnt++;
				if (Image[(i + 1) * W + j + 1] == 0 && Image[(i + 1) * W + j] == 255) cnt++;
				if (Image[(i + 1) * W + j] == 0 && Image[(i + 1) * W + j - 1] == 255) cnt++;
				if (Image[(i + 1) * W + j - 1] == 0 && Image[i * W + j - 1] == 255) cnt++;
				if (Image[i * W + j - 1] == 0 && Image[(i - 1) * W + j - 1] == 255) cnt++;
			}
			if (cnt == 1) Output[i * W + j] = 128; //끝점
			else if (cnt >= 3) Output[i * W + j] = 128; //분기점
			cnt = 0;
		}
	}
}


int main() {

	//영상입력
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


	fread(&hf, sizeof(BITMAPFILEHEADER), 1, fp); //14byte를 1번 읽어와서 hf에 저장해라
	fread(&hInfo, sizeof(BITMAPINFOHEADER), 1, fp);
	int W = hInfo.biWidth, H = hInfo.biHeight;
	int ImgSize = hInfo.biWidth * hInfo.biHeight;
	if (hInfo.biBitCount == 24) { //트루컬러
		Image = (BYTE*)malloc(ImgSize * 3);
		Output = (BYTE*)malloc(ImgSize * 3);
		Temp = (BYTE*)malloc(ImgSize * 3);
		fread(Image, sizeof(BYTE), ImgSize * 3, fp);
	}
	else { //인덱스(그레이)
		fread(hRGB, sizeof(RGBQUAD), 256, fp);
		Image = (BYTE*)malloc(ImgSize);
		Output = (BYTE*)malloc(ImgSize);
		Temp = (BYTE*)malloc(ImgSize);
		fread(Image, sizeof(BYTE), ImgSize, fp);
	}
	fclose(fp);

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

	// 9주차
	//median filtering
	//MedianFiltering(Image, Output, W, H, 5);

	//동공영역 가장큰 영역검출
	//Binarization(Image, Temp, W, H, 30);
	//InverseImage(Temp, Temp, W, H);
	//m_BlobColoring(Temp, H, W);

	//for (int i = 0; i < ImgSize; i++) Output[i] = Image[i];

	//int Cx, Cy;
	//Obtain2DCenter(Temp, W, H, &Cx, &Cy); //이진 영상의 무게중심
	//int LUX, LUY, RDX, RDY;
	//Obtain2DBoundingBox(Temp, W, H, &LUX, &LUY, &RDX, &RDY); //이진 영상의 외접사각형 좌표 추출
	//DrawRectOutline(Output, W, H, LUX, LUY, RDX, RDY);
	//DrawCrossOutline(Output, W, H, Cx, Cy);


	//10주차
	//특정위치에 색깔 채우기
	/*for (int i = 0; i < W; i++) {
		FillColor(Image, i, 200, W, H, 0, 255, 255);
	}*/

	//50,100 ~300,400 채우기
	/*for (int i = 100; i <= 400; i++) {
		for (int j = 50; j <= 300; j++) {
			FillColor(Image, j, i, W, H, 255, 0, 255);
		}
	}*/

	//가로 띠 만들기
	//초기화
	//for (int i = 0; i < H; i++) {
	//	for (int j = 0; j < W; j++) {
	//		Image[i * W * 3 + j * 3 ] = 0;
	//		Image[i * W * 3 + j * 3 + 1] = 0;
	//		Image[i * W * 3 + j * 3 + 2] = 0;
	//	}
	//}
	////y좌표 기준 0~240 Red
	//for (int i = 0; i < 240; i++) {
	//	for (int j = 0; j < W; j++) {
	//		Image[i * W * 3 + j * 3 + 2] = 255;
	//	}
	//}
	////y좌표 기준 120~360 Green
	//for (int i = 120; i < 360; i++) {
	//	for (int j = 0; j < W; j++) {
	//		Image[i * W * 3 + j * 3 + 1] = 255;
	//	}
	//}
	////y좌표 기준 240~480 Blue
	//for (int i = 240; i < 480; i++) {
	//	for (int j = 0; j < W; j++) {
	//		Image[i * W * 3 + j * 3] = 255;
	//	}
	//}

	// 그라데이션 띠 만들기 blue->Yellow
	//double wt;
	//for (int a = 320; a <= 480; a++) {
	//	for (int i = 0; i < W; i++) {
	//		wt = i / (double)(W - 1);
	//		Image[a * W * 3 + i * 3] = (BYTE)255 * (1 - wt);
	//		Image[a * W * 3 + i * 3 + 1] = (BYTE)255 * wt;
	//		Image[a * W * 3 + i * 3 + 2] = (BYTE)255 * wt;
	//	}
	//}
	//// 그라데이션 띠 만들기 green->Magenta
	//for (int a = 160; a <= 320; a++) {
	//	for (int i = 0; i < W; i++) {
	//		wt = i / (double)(W - 1);
	//		Image[a * W * 3 + i * 3] = (BYTE)255 * wt;
	//		Image[a * W * 3 + i * 3 + 1] = (BYTE)255 * (1 - wt);
	//		Image[a * W * 3 + i * 3 + 2] = (BYTE)255 * wt;
	//	}
	//}
	//// 그라데이션 띠 만들기 red->Cyan
	//for (int a = 0; a <= 160; a++) {
	//	for (int i = 0; i < W; i++) {
	//		wt = i / (double)(W - 1);
	//		Image[a * W * 3 + i * 3] = (BYTE)255 * wt;
	//		Image[a * W * 3 + i * 3 + 1] = (BYTE)255 * wt;
	//		Image[a * W * 3 + i * 3 + 2] = (BYTE)255 * (1 - wt);
	//	}
	//}

	//VerticalFlip(Image, W * 3, H);


	//11주차
	////Red값이큰 화소만 masking
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

	//빨간색 딸기 영역만 masking
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


	// 피부색 영역만 masking
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
	//m_BlobColoring(Temp, H, W); //라벨링 결과 가장 큰 영역만 남김
	//int LUX, LUY, RDX, RDY;
	//Obtain2DBoundingBox(Temp, W, H, &LUX, &LUY, &RDX, &RDY);
	//DrawRectColorOutline(Image, W, H, LUX, LUY, RDX, RDY, 255, 0, 0);


	//fp = fopen("Y.raw", "wb");
	//fwrite(Y, sizeof(BYTE), W*H, fp); //binary file은 byte 단위로 의미가 있음, 그래서 읽어올때와 다르게 쓸 때는 byte 단위로 써야 더 의미있음
	//fclose(fp);
	//fp = fopen("Cb.raw", "wb");
	//fwrite(Cb, sizeof(BYTE), W* H, fp); //binary file은 byte 단위로 의미가 있음, 그래서 읽어올때와 다르게 쓸 때는 byte 단위로 써야 더 의미있음
	//fclose(fp);
	//fp = fopen("Cr.raw", "wb");
	//fwrite(Cr, sizeof(BYTE), W* H, fp); //binary file은 byte 단위로 의미가 있음, 그래서 읽어올때와 다르게 쓸 때는 byte 단위로 써야 더 의미있음
	//fclose(fp);
	//free(Y);
	//free(Cb);
	//free(Cr);



	//12주차
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
	//팽창 후 침식 ->Closing

	//골격화(원래도형의 뼈대), 세선화(도형의 대략적인 경로) -> 두께가 1pixel인 line형태가 됨
	//세선화 - >지문인식, 지문의 분기점과 끝점을 찾을때 사용
	InverseImage(Image, Image, W, H);
	zhangSuen(Image, Output, H, W);
	//분기점, 끝점 gray로 처리
	FeatureExtractThinImage2(Output, Image, W, H);




	//결과출력
	SaveBMPFile(hf, hInfo, hRGB, Image, W, H, "output_erosion_thining2.bmp");

	
	free(Image);
	free(Output);
	free(Temp);
	return 0;
}