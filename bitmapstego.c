#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <memory.h>

#pragma warning(disable:4996)
#pragma pack(push, 1)                // ����ü�� 1����Ʈ ũ��� ����

typedef struct _BITMAPFILEHEADER     // BMP ��Ʈ�� ���� ��� ����ü
{
    unsigned short bfType;           // BMP ���� ���� �ѹ�
    unsigned int   bfSize;           // ���� ũ��
    unsigned short bfReserved1;      // ����
    unsigned short bfReserved2;      // ����
    unsigned int   bfOffBits;        // ��Ʈ�� �������� ���� ��ġ
} BITMAPFILEHEADER;

typedef struct _BITMAPINFOHEADER     // BMP ��Ʈ�� ���� ��� ����ü(DIB ���)
{
    unsigned int   biSize;           // ���� ����ü�� ũ��
    int            biWidth;          // ��Ʈ�� �̹����� ���� ũ��
    int            biHeight;         // ��Ʈ�� �̹����� ���� ũ��
    unsigned short biPlanes;         // ����ϴ� �������� ��
    unsigned short biBitCount;       // �ȼ� �ϳ��� ǥ���ϴ� ��Ʈ ��
    unsigned int   biCompression;    // ���� ���
    unsigned int   biSizeImage;      // ��Ʈ�� �̹����� �ȼ� ������ ũ��
    int            biXPelsPerMeter;  // �׸��� ���� �ػ�(���ʹ� �ȼ�)
    int            biYPelsPerMeter;  // �׸��� ���� �ػ�(���ʹ� �ȼ�)
    unsigned int   biClrUsed;        // ���� ���̺��� ���� ���Ǵ� ���� ��
    unsigned int   biClrImportant;   // ��Ʈ���� ǥ���ϱ� ���� �ʿ��� ���� �ε��� ��
} BITMAPINFOHEADER;

typedef struct _RGBTRIPLE            // 24��Ʈ ��Ʈ�� �̹����� �ȼ� ����ü
{
    unsigned char rgbtBlue;          // �Ķ�
    unsigned char rgbtGreen;         // �ʷ�
    unsigned char rgbtRed;           // ����
} RGBTRIPLE;

#pragma pack(pop)

int Data_Hiding(char filename[]) {
    FILE* fp = fopen(filename, "wb");
    BITMAPFILEHEADER bf; // ��Ʈ�� ��������� �����մϴ�.
    BITMAPINFOHEADER bi; // ��Ʈ�� ��������� �����մϴ�.
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

    // ��Ʈ���� �������� 4�� ����̸� �׳� �� �濡 �о��.
    
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
    	printf("�����͸� ã�� ���� �̸��� �Է��ϼ���. (*.bmp) : ");
    	scanf("%s", filename);
        Data_Finding(filename);
        break;
    case 3:
        return 0;
    default:
        return 0;
    }
}
