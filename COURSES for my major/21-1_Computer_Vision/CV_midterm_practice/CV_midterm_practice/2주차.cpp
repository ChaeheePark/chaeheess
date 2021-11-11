#pragma warning (disable:4996)
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>

int main() {
	
	//영상입력
	BITMAPFILEHEADER hf;
	BITMAPINFOHEADER hInfo;
	RGBQUAD hRGB[256];
	FILE* fp;

	fp = fopen("open.bmp", "rb");
	if (fp == NULL) {
		printf("file not found!\n");
		return 0;
	}

	fread(&hf, sizeof(BITMAPFILEHEADER), 1, fp);
	fread(&hInfo, sizeof(BITMAPINFOHEADER), 1, fp);
	fread(hRGB, sizeof(RGBQUAD), 256, fp);
	int ImgSize = hInfo.biWidth * hInfo.biHeight;

	BYTE* Image = (BYTE*)malloc(ImgSize);
	BYTE* Output = (BYTE*)malloc(ImgSize);
	fread(Image, sizeof(BYTE), ImgSize, fp);
	fclose(fp);

	//영상처리
	for (int i = 0; i < ImgSize; i++) {
		printf("%d\n",Image[i]);
	}

	//결과출력
	//fp = fopen("output_LENNA.bmp", "wb");
	//fwrite(&hf,sizeof(BYTE),sizeof(BITMAPFILEHEADER), fp); //binary file은 byte 단위로 의미가 있음, 그래서 읽어올때와 다르게 쓸 때는 byte 단위로 써야 더 의미있음
	//fwrite(&hInfo, sizeof(BYTE), sizeof(BITMAPINFOHEADER), fp);
	//fwrite(hRGB, sizeof(RGBQUAD), 256, fp);
	//fwrite(Output, sizeof(BYTE), ImgSize, fp);
	//fclose(fp);

	free(Image);
	free(Output);
	return 0;
}