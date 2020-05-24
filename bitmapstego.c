#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <memory.h>

#pragma warning(disable:4996)
#pragma pack(push, 1)                // 구조체를 1바이트 크기로 정렬

typedef struct _BITMAPFILEHEADER     // BMP 비트맵 파일 헤더 구조체
{
    unsigned short bfType;           // BMP 파일 매직 넘버
    unsigned int   bfSize;           // 파일 크기
    unsigned short bfReserved1;      // 예약
    unsigned short bfReserved2;      // 예약
    unsigned int   bfOffBits;        // 비트맵 데이터의 시작 위치
} BITMAPFILEHEADER;

typedef struct _BITMAPINFOHEADER     // BMP 비트맵 정보 헤더 구조체(DIB 헤더)
{
    unsigned int   biSize;           // 현재 구조체의 크기
    int            biWidth;          // 비트맵 이미지의 가로 크기
    int            biHeight;         // 비트맵 이미지의 세로 크기
    unsigned short biPlanes;         // 사용하는 색상판의 수
    unsigned short biBitCount;       // 픽셀 하나를 표현하는 비트 수
    unsigned int   biCompression;    // 압축 방식
    unsigned int   biSizeImage;      // 비트맵 이미지의 픽셀 데이터 크기
    int            biXPelsPerMeter;  // 그림의 가로 해상도(미터당 픽셀)
    int            biYPelsPerMeter;  // 그림의 세로 해상도(미터당 픽셀)
    unsigned int   biClrUsed;        // 색상 테이블에서 실제 사용되는 색상 수
    unsigned int   biClrImportant;   // 비트맵을 표현하기 위해 필요한 색상 인덱스 수
} BITMAPINFOHEADER;

typedef struct _RGBTRIPLE            // 24비트 비트맵 이미지의 픽셀 구조체
{
    unsigned char rgbtBlue;          // 파랑
    unsigned char rgbtGreen;         // 초록
    unsigned char rgbtRed;           // 빨강
} RGBTRIPLE;

#pragma pack(pop)

int Data_Hiding(char filename[]) {
    FILE* fp = fopen(filename, "wb");
    BITMAPFILEHEADER bf; // 비트맵 파일헤더를 선언합니다.
    BITMAPINFOHEADER bi; // 비트맵 인포헤더를 선언합니다.
    unsigned int user_width = 0;
    unsigned int user_height = 0;
    unsigned char user_data[4096 * 3] = { "\0", };

    memset(&bf, 0, sizeof(BITMAPFILEHEADER));
    memset(&bi, 0, sizeof(BITMAPFILEHEADER));
   
    printf("Bitmap width(1 ~ INT_MAX) : ");
    scanf("%d", &user_width);

    printf("Bitmap height(1 ~ INT_MAX) : ");
    scanf("%d", &user_height);
    rewind(stdin);

    printf("Enter Your Data : ");
    scanf("%[^\n]", user_data);

    bi.biSize = 40;
    bi.biWidth = user_width;
    bi.biHeight = user_height; 
    bi.biPlanes = 1;
    bi.biBitCount = 24;
    bi.biCompression = 0;
    bi.biSizeImage = 3 * bi.biWidth * bi.biHeight;
    bi.biXPelsPerMeter = 0x130B; 
    bi.biYPelsPerMeter = 0x130B;
    bi.biClrUsed = 0x0; 
    bi.biClrImportant = 0x0; 

    bf.bfType = 0x4D42;
    bf.bfSize = 54 + bi.biSizeImage;
    bf.bfReserved1 = 0x0;
    bf.bfReserved2 = 0x0;
    bf.bfOffBits = 54;

    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, fp);
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, fp);

    // bitmap header end



    printf("\n\nYour DATA : %s\n\n", user_data);

    uint8_t* pixelbuffer = (uint8_t*)malloc(bi.biSizeImage);
    memset(pixelbuffer, 0xFF, bi.biSizeImage);
    fwrite(user_data, strlen(user_data), 1, fp);
    fwrite(pixelbuffer, 1, bi.biSizeImage - strlen(user_data), fp);

    
    fclose(fp);

    printf("Your Data Size : %d\n\n", strlen(user_data));
    printf("Data Hiding Complete. Filename is %s.\n", filename);
}

int Data_Finding(char filename[]) {
    
    unsigned char* user_data;
    BITMAPFILEHEADER bf; 
    BITMAPINFOHEADER bi; 
    FILE* fp; 
	int i = 0;
    memset(&bf, 0, sizeof(BITMAPFILEHEADER)); 
    memset(&bi, 0, sizeof(BITMAPFILEHEADER));
    
    

    fp = fopen(filename, "rb");
    if (fp == NULL)
        return 0;

    fread(&bf, sizeof(BITMAPFILEHEADER), 1, fp);

    if (bf.bfType != 0x4D42)
    {
        fclose(fp);
        return 0;
    }
    
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, fp);
    
    if (bi.biBitCount != 24)
    {
        fclose(fp);
        return 0;
    }

    user_data = (unsigned char*)malloc(bi.biWidth * bi.biHeight * 3);

    // 비트맵의 가로폭이 4의 배수이면 그냥 한 방에 읽어옴.
    
    fread(user_data, bi.biWidth * 3, bi.biHeight, fp);
    fclose(fp);

    for(i = 0; i < strlen(user_data); i++) {
    	if(user_data[i] != 0xFF) {
    		printf("%c", user_data[i]);
		}
	}
    return 1;
    
    system("pause");
    
}

int main(void) {
    int op = 0;
	char filename[256];
    system("title BITMAPSTEGO VER 0.1");

   
    printf("BITMAPSTEGO VER 0.1\n");
    printf("[1] Data Hiding [2] Data Finding [] Exit\n");
    printf("Select Menu : ");
    scanf("%d", &op);
    
    switch (op) {
    case 1:
    	printf("Enter File Name : ");
    	scanf("%s", filename);
        Data_Hiding(filename);
        break;
    case 2:
    	printf("데이터를 찾을 파일 이름을 입력하세요. (*.bmp) : ");
    	scanf("%s", filename);
        Data_Finding(filename);
        break;
    case 3:
        return 0;
    default:
        return 0;
    }
}
