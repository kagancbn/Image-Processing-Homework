#pragma once
#include <Windows.h>
void GriImgeKaydet(BYTE* Intensity, int Uzunluk, int yukseklik, LPCTSTR Output);
void cemberCiz(BYTE* Buffer, int x, int y, int r, int width, int height);
BYTE* Mask(BYTE* Buffer, int uzunluk, int yukseklik, float mask[], int maskElemanSayisi, long* yeniUzunluk, long* yeniYukseklik);
unsigned long *Histogram(BYTE* Buffer, int uzunluk, int yukseklik);
BYTE* BinaryImageIntensityMinumumDistance(BYTE* Buffer, int uzunluk, int yukseklik);
BYTE* KmeanImageIntensityMinumumDistance(BYTE* Buffer, int uzunluk, int yukseklik, int K);
double* MatrisCarp(double* A, double* B, int Asatir, int Asutun, int Bsatýr, int Bsutun);
double* MatrisCarp(double c, double* A, int Asatir, int Asutun);
double* Tranpose(double* Matris, int satir, int sutun);
BYTE* Dilation(BYTE* Buffer, int uzunluk, int yukseklik, int* DMatris, int DMuzunluk, int DMyukseklik);
BYTE* Erosion(BYTE*Buffer, int uzunluk, int yukseklik, int* EMatris, int EMuzunluk, int EMyukseklik);
BYTE* Opening(BYTE*Buffer, int uzunluk, int yukseklik, int* Matris, int Muzunluk, int Myukseklik);
BYTE* Closing(BYTE*Buffer, int uzunluk, int yukseklik, int* Matris, int Muzunluk, int Myukseklik);
BYTE* Sinirlar(BYTE*Buffer, int uzunluk, int yukseklik, int* Matris, int Muzunluk, int Myukseklik);
BYTE* Tumleme(BYTE*Buffer, int uzunluk, int yukseklik);
BYTE* BinaryImageIntensityMahalonobisDistance(BYTE* Buffer, int uzunluk, int yukseklik);
BYTE* KmeanImageIntensityMahalonobisDistance(BYTE* Buffer, int uzunluk, int yukseklik, int K);
BYTE* NesneleriAyýr(BYTE* Buffer, int uzunluk, int yukseklik);


BYTE* DogruBul(BYTE* Buffer, int Uzunluk, int Yukseklik, int DogruSayisi);
int* GradiantDegeriniBnaryYap(int *GradiantDegeri, int Yuzde, int Uzunluk, int Yukseklik);
int GradiantHistogramDegeri(int* GradiantDegeri, int Yuzde, int Uzunluk, int Yukseklik);
int* GrandiantDegeriBul(int* Gx, int *Gy, int Uzunluk, int Yukseklik);
int* TurevAl(BYTE* Buffer, int Uzunluk, int Yukseklik, int* Tmatrisi);
int* AciDegeriniBul(int* Gx, int * Gy, int Uzunluk, int Yukseklik);
int tanInverse(double Sayi);
int* GBinary(BYTE*Buffer, int Uzunluk, int Yukseklik);

BYTE* RGBMahalonobisDistance(BYTE* Buffer, int uzunluk, int yukseklik);
BYTE* RGBMahalonobisDistanceKMean(BYTE* Buffer, int uzunluk, int yukseklik, int K);
int *asd(BYTE* Buffer, int uzunluk, int yukseklik, int K);

BYTE* RGBVektor(BYTE* Buffer, int uzunluk, int yukseklik, int K);