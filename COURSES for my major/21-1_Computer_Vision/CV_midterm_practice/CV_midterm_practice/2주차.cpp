#pragma warning (disable:4996)
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>

int main() {
	
	//�����Է�
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

	//����ó��
	for (int i = 0; i < ImgSize; i++) {
		printf("%d\n",Image[i]);
	}

	//������
	//fp = fopen("output_LENNA.bmp", "wb");
	//fwrite(&hf,sizeof(BYTE),sizeof(BITMAPFILEHEADER), fp); //binary file�� byte ������ �ǹ̰� ����, �׷��� �о�ö��� �ٸ��� �� ���� byte ������ ��� �� �ǹ�����
	//fwrite(&hInfo, sizeof(BYTE), sizeof(BITMAPINFOHEADER), fp);
	//fwrite(hRGB, sizeof(RGBQUAD), 256, fp);
	//fwrite(Output, sizeof(BYTE), ImgSize, fp);
	//fclose(fp);

	free(Image);
	free(Output);
	return 0;
}