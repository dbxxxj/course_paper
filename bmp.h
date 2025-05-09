#pragma once
#include <stdint.h>
#include<stdio.h>
#pragma pack(push, 1)

typedef struct {
	uint16_t bfType; // ��������� �����, �.�. ������ ���� ���:0x4D42
	uint32_t bfSize; // ������ ����� �����;
	uint16_t bfReserved1;
	uint16_t bfReserved2;
	uint32_t bfOffbits;// �������� �� ������ ����� �� ������ ��������
}BMPFileHeader;
typedef struct {
    uint32_t biSize;             // ������ ���� ��������� (������ 40 ����)
    int32_t  biWidth;            // ������ ����������� � ��������
    int32_t  biHeight;           // ������ ����������� � �������� (����� ���� ������������� ��� "������ ����")
    uint16_t biPlanes;           // ���������� �������� ���������� (������ ������ ���� 1)
    uint16_t biBitCount;         // ���������� ��� �� ������� (��������, 24 ��� RGB)
    uint32_t biCompression;      // ��� ������ (0 = BI_RGB, �� ���� ��� ������)
    uint32_t biSizeImage;        // ������ ����������� � ������ (����� ���� 0 ��� ���������)
    int32_t  biXPelsPerMeter;    // �������������� ���������� (�������� �� ����)
    int32_t  biYPelsPerMeter;    // ������������ ���������� (�������� �� ����)
    uint32_t biClrUsed;          // ���������� ������������ ������ (0 ��������, ��� ������������ ������������ ����������)
    uint32_t biClrImportant;     // ���������� "������" ������ (0 ��������, ��� ��� �����)
} BMPInfoHeader;
#pragma pack(pop)
typedef struct {
	uint8_t r, g, b;
} RGB;


int read_bmp_headers(FILE* fp, BMPFileHeader* fileHeader, BMPInfoHeader* infoHeader);
RGB** read_bmp_pixels(FILE* fp, const BMPFileHeader* fileHeader, const BMPInfoHeader* infoHeader);
int write_bmp(const char* filename, const BMPFileHeader* fileHeader, const BMPInfoHeader* infoHeader, RGB** pixels);
void free_pixels(RGB** pixels, int height);