#pragma warning (disable:4996)
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>

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
	BYTE* Output1 = (BYTE*)malloc(ImgSize);
	BYTE* Output2 = (BYTE*)malloc(ImgSize);
	BYTE* Output3 = (BYTE*)malloc(ImgSize);

	fread(Image, sizeof(BYTE), ImgSize, fp);
	fclose(fp);

	for (int i = 0; i < ImgSize; i++) {
		Output1[i] =Image[i]; //반전
	}
	for (int i = 0; i < ImgSize; i++) {
		Output2[i] = Image[i]+50; //+50
	}
	for (int i = 0; i < ImgSize; i++) {
		Output3[i] = 255 - Image[i]; //반전
	}

	fp = fopen("output1.bmp", "wb");
	fwrite(&hf, sizeof(BYTE), sizeof(BITMAPFILEHEADER), fp); //byte단위로 써야함(읽어올때랑다름!)
	fwrite(&hinfo, sizeof(BYTE), sizeof(BITMAPINFOHEADER), fp);
	fwrite(hRGB, sizeof(RGBQUAD), 256, fp);
	fwrite(Output1, sizeof(BYTE), ImgSize, fp);
	fclose(fp);

	fp = fopen("output2.bmp", "wb");
	fwrite(&hf, sizeof(BYTE), sizeof(BITMAPFILEHEADER), fp); //byte단위로 써야함(읽어올때랑다름!)
	fwrite(&hinfo, sizeof(BYTE), sizeof(BITMAPINFOHEADER), fp);
	fwrite(hRGB, sizeof(RGBQUAD), 256, fp);
	fwrite(Output2, sizeof(BYTE), ImgSize, fp);
	fclose(fp);

	fp = fopen("output3.bmp", "wb");
	fwrite(&hf, sizeof(BYTE), sizeof(BITMAPFILEHEADER), fp); //byte단위로 써야함(읽어올때랑다름!)
	fwrite(&hinfo, sizeof(BYTE), sizeof(BITMAPINFOHEADER), fp);
	fwrite(hRGB, sizeof(RGBQUAD), 256, fp);
	fwrite(Output3, sizeof(BYTE), ImgSize, fp);
	fclose(fp);

	free(Image);
	free(Output1); // 하지 않으면 memory leak
	free(Output2);
	free(Output3);
	return 0;
}
