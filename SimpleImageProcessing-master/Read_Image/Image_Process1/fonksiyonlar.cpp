#include "stdafx.h"
#include "fonksiyonlar.h"
#include <math.h>
#include <iostream>
#include "imge_bmp.h"

using namespace std;

void GriImgeKaydet(BYTE* Intensity, int Uzunluk, int yukseklik, LPCTSTR Output) {
	long newSize;
	BYTE* displayImge = ConvertIntensityToBMP(Intensity, Uzunluk, yukseklik, &newSize);
	if (SaveBMP(displayImge, Uzunluk, yukseklik, newSize, Output))
		cout << "Output Image was successfully saved" << endl;
	else cout << "Error on saving image" << endl;
	delete[] displayImge;
}

void cemberCiz(BYTE* Buffer, int x, int y, int r, int width, int height)
{
	int x0 = 0, y0 = 0, konum = 0;
	for (int i = 0; i < 360; i++)
	{

		x0 = (int)(x + cos(i) * r);
		y0 = (int)(y + sin(i) * r);

		konum = width*y0 + x0;

		if (x0 >= 0 && x0 < width && konum < width*height && konum >= 0)
		{
			Buffer[konum] = BYTE(255);
		}

	}
}

BYTE* Mask(BYTE* Buffer, int uzunluk, int yukseklik, float mask[], int maskElemanSayisi, long* yeniUzunluk, long* yeniYukseklik)
{
	int N = (int)(sqrt(maskElemanSayisi)); // N = matrisin kaça kaçlýk olduðu gösterir N x N
	if (N*N != maskElemanSayisi) return NULL; // Mask kare matris deðilse iþlem yapma
	if (N % 2 == 0) return NULL;			// Mask matrisi (2n+1) türünde yani tek matris deðilse (3x3,5x5,7x7 .... gibi deðilse ) iþlem yapma
	BYTE *buf = new BYTE[(uzunluk - N + 1)*(yukseklik - N + 1)]; // iþlem yaptýktan sonra ki görüntü içeriðimizi saklayacaðýmýz alan
	float maskToplam = 0.0; //Matris elemanlarýnýn toplamý
	int xKonum = 0, yKonum = 0; // mask gezinme esnasýndaki görüntüdeki konumlarý
	*yeniUzunluk = uzunluk - N + 1;
	*yeniYukseklik = yukseklik - N + 1;
	for (int i = 0; i < maskElemanSayisi; i++) {
		maskToplam = maskToplam + mask[i];// matris elemanlarýnýn toplanma iþlemi
	}
	for (int i = 0; i < (uzunluk - N + 1)*(yukseklik - N + 1); i++) // Maský görüntü üzerinde gezdirme iþlemi
	{
		float deger = 0.0;
		for (int j = 0; j < N; j++)
		{
			for (int k = 0; k < N; k++)
			{
				deger = deger + mask[k + j*N] * (int)(Buffer[(xKonum + k) + (yKonum + j)*uzunluk]);
			}
		}
		if (xKonum == uzunluk - N)
		{
			yKonum++;
			xKonum = 0;
		}
		else
		{
			xKonum++;
		}
		buf[i] = BYTE((int)(deger / maskToplam));
	}
	return buf;
}

unsigned long *Histogram(BYTE* Buffer, int uzunluk, int yukseklik) {
	unsigned long *histogramInt = new unsigned long[256];
	int x = 0;
	for (int i = 0; i < 256; i++)
	{
		histogramInt[i] = 0;
	}
	for (int i = 0; i < uzunluk*yukseklik; i++)
	{
		histogramInt[(int)(Buffer[i])]++;
	}
	for (int i = 0; i < 256; i++)
	{
		if (x < histogramInt[i])
		{
			x = histogramInt[i];
		}
	}

	BYTE* buf = new BYTE[x * 256];
	for (int i = 0; i < x; i++)
	{
		for (int j = 0; j < 256; j++)
		{
			if (histogramInt[j] >= x - i)
			{
				buf[i * 256 + j] = BYTE(0);
			}
			else
			{
				buf[i * 256 + j] = BYTE(255);
			}
		}

	}
	GriImgeKaydet(buf, 256, x, L"C://Users//OguzhnOzbk//Desktop//Read_Image//Sonuc//Histogram.bmp");

	return histogramInt;
}

BYTE* BinaryImageIntensityMinumumDistance(BYTE* Buffer, int uzunluk, int yukseklik) {
	unsigned long *histogram = new unsigned long[256];
	histogram = Histogram(Buffer, uzunluk, yukseklik);
	int t1 = 50, t2 = 150, t1u, t2u;
	int esikDegeri;
	unsigned long m1i = 0, m2i = 0, m1 = 0, m2 = 0;
	BYTE * buf = new BYTE[uzunluk*yukseklik];
	int hisEtiket[256];

	while (true)
	{
		esikDegeri = (t1 + t2) / 2;
		m1i = 0; m2i = 0; m1 = 0; m2 = 0;
		for (int i = 0; i < 256; i++)
		{
			if (i<esikDegeri)	hisEtiket[i] = 1;
			else                hisEtiket[i] = 2;
		}
		for (int i = 0; i < 256; i++)
		{
			if (hisEtiket[i] == 1)
			{
				m1i = m1i + i*histogram[i];
				m1 = m1 + histogram[i];
			}
			else if (hisEtiket[i]==2)
			{
				m2i = m2i + i*histogram[i];
				m2 = m2 + histogram[i];
			}
		} t1u = m1i / m1; t2u = m2i / m2; // T1' ve T2' degerleri
		if (t1 == t1u && t2 == t2u)
		{
			break;
		}
		else
		{
			t1 = t1u;
			t2 = t2u;
		}
	}
	esikDegeri = (t1 + t2) / 2;
	for (int i = 0; i < yukseklik*uzunluk; i++)
	{
		if (int(Buffer[i])<esikDegeri)
		{
			buf[i] = BYTE(0);
		}
		else
		{
			buf[i] = BYTE(255);
		}
	}
	return buf;
}

BYTE* BinaryImageIntensityMahalonobisDistance(BYTE* Buffer, int uzunluk, int yukseklik) {
	unsigned long *histogram = new unsigned long[256];
	histogram = Histogram(Buffer, uzunluk, yukseklik);
	int t1 = 50, t2 = 150, t1u, t2u;
	int esikDegeri;
	double m1i = 0, m2i = 0, m1 = 0, m2 = 0;
	BYTE * buf = new BYTE[uzunluk*yukseklik];
	int hisEtiket[256];

	while (true)
	{
		esikDegeri = (t1 + t2) / 2;
		m1i = 0; m2i = 0; m1 = 0; m2 = 0;
		for (int i = 0; i < 256; i++)
		{
			if (i<esikDegeri)	hisEtiket[i] = 1;
			else                hisEtiket[i] = 2;
		}
		for (int i = 0; i < 256; i++)
		{
			if (hisEtiket[i] == 1)
			{
				m1i = m1i + i*histogram[i];
				m1 = m1 + histogram[i];
			}
			else if (hisEtiket[i] == 2)
			{
				m2i = m2i + i*histogram[i];
				m2 = m2 + histogram[i];
			}
		} t1u = m1i / m1; t2u = m2i / m2; // T1' ve T2' degerleri
		m1i = 0; m1 = 0; m2i = 0; m2 = 0;
		for (int i = 0; i < 256; i++)
		{
			if (hisEtiket[i] == 1)
			{
				m1i = m1i + i*(t1u - histogram[i])*(t1u - histogram[i]);
				m1 = m1 + (t1u - histogram[i])*(t1u - histogram[i]);
			}
			else if (hisEtiket[i] == 2)
			{
				m2i = m2i + i*(t2u - histogram[i])*(t2u - histogram[i]);
				m2 = m2 + (t2u - histogram[i])*(t2u - histogram[i]);
			}
		}
		t1u = m1i / m1; t2u = m2i / m2;
		if (t1 == t1u && t2 == t2u)
		{
			break;
		}
		else
		{
			t1 = t1u;
			t2 = t2u;
		}
	}
	esikDegeri = (t1 + t2) / 2;
	for (int i = 0; i < yukseklik*uzunluk; i++)
	{
		if (int(Buffer[i])<esikDegeri)
		{
			buf[i] = BYTE(0);
		}
		else
		{
			buf[i] = BYTE(255);
		}
	}
	return buf;
}

BYTE* KmeanImageIntensityMinumumDistance(BYTE* Buffer, int uzunluk, int yukseklik, int K)
{
	
	unsigned long *histogram = new unsigned long[256];
	histogram = Histogram(Buffer, uzunluk, yukseklik);
	unsigned long *Mk = new unsigned long[K];
	unsigned long *Mki = new unsigned long[K];
	int *Tk = new int [K];
	int *Tku = new int[K];
	int * esik = new int[K - 1];
	int aralýk = 256 / K;
	int hisEtiket[256];
	for (int i = 0; i < K; i++)
	{
		Tk[i] = i*aralýk + aralýk/2 ;
	}

	while (true)
	{
		for (int i = 0; i < K-1; i++)
		{
			esik[i] = (Tk[i] + Tk[i + 1]) / 2;
		}
		for (int i = 0; i < K; i++)
		{
			Mk[i] = 0; Mki[i] = 0;
		}
		for (int i = 0; i < K-1; i++)
		{
			for (int j = 0; j < 256; j++)
			{
				if (i == 0 && j <= esik[i])
				{
					hisEtiket[j] = i;
				}
				else if (i != 0 && esik[i-1] < j && j <= esik[i])
				{
					hisEtiket[j] = i;
				}
				else if (i == K-2 && j > esik [i])
				{
					hisEtiket[j] = K-1;
				}
			}
		}
		for (int i = 0; i < K; i++)
		{
			for (int j = 0; j < 256; j++)
			{
				if (hisEtiket[j] == i)
				{
					Mki[i] = Mki[i] + j*histogram[j];
					Mk[i] = Mk[i] + histogram[j];
				}
			}
			
		}
		for (int i = 0; i < K; i++)
		{
			if (Mk[i] != 0)
			Tku[i] = Mki[i] / Mk[i];

		}
		int esitlik = 0;
		for (int i = 0; i < K; i++)
		{
			if (Tk[i] == Tku[i])
			{
				esitlik++;
			}
		}
		if (esitlik == K)
		{
			break;
		}
		else
		{
			for (int i = 0; i < K; i++)
			{
				Tk[i] = Tku[i];
			}
		}
	}
	BYTE *Buf = new BYTE[yukseklik*uzunluk];
	for (int i = 0; i < yukseklik*uzunluk; i++)
	{
		for (int k = 0; k < K-1; k++)
		{
			if (k == 0 && int(Buffer[i]) <= esik[k])
			{
				Buf[i] = BYTE(0);
			}
			else if (k != 0 && esik[k - 1] < int(Buffer[i]) && int(Buffer[i]) <= esik[k])
			{
				Buf[i] = BYTE(k*aralýk);
			}
			else if (k == K - 2 && int(Buffer[i]) > esik[k])
			{
				Buf[i] = BYTE(255);
			}
		}
	}
	return Buf;
}

BYTE* KmeanImageIntensityMahalonobisDistance(BYTE* Buffer, int uzunluk, int yukseklik, int K)
{

	unsigned long *histogram = new unsigned long[256];
	histogram = Histogram(Buffer, uzunluk, yukseklik);
	double *Mk = new double[K];
	double *Mki = new double[K];
	int *Tk = new int[K];
	int *Tku = new int[K];
	int * esik = new int[K - 1];
	int aralýk = 256 / K;
	int hisEtiket[256];
	for (int i = 0; i < K; i++)
	{
		Tk[i] = i*aralýk + aralýk / 2;
	}

	while (true)
	{
		for (int i = 0; i < K - 1; i++)
		{
			esik[i] = (Tk[i] + Tk[i + 1]) / 2;
		}
		for (int i = 0; i < K; i++)
		{
			Mk[i] = 0; Mki[i] = 0;
		}
		for (int i = 0; i < K - 1; i++)
		{
			for (int j = 0; j < 256; j++)
			{
				if (i == 0 && j <= esik[i])
				{
					hisEtiket[j] = i;
				}
				else if (i != 0 && esik[i - 1] < j && j <= esik[i])
				{
					hisEtiket[j] = i;
				}
				else if (i == K - 2 && j > esik[i])
				{
					hisEtiket[j] = K - 1;
				}
			}
		}
		for (int i = 0; i < K; i++)
		{
			for (int j = 0; j < 256; j++)
			{
				if (hisEtiket[j] == i)
				{
					Mki[i] = Mki[i] + j*histogram[j];
					Mk[i] = Mk[i] + histogram[j];
				}
			}

		}
		for (int i = 0; i < K; i++)
		{
			Tku[i] = Mki[i] / Mk[i];
			Mki[i] = 0; Mk[i] = 0;
		}

		for (int i = 0; i < K; i++)
		{
			for (int j = 0; j < 256; j++)
			{
				if (hisEtiket[j] == i)
				{
					Mki[i] = Mki[i] + j*(Tku[i] - histogram[j])*(Tku[i] - histogram[j]);
					Mk[i] = Mk[i] + (Tku[i] - histogram[j])*(Tku[i] - histogram[j]);
				}
			}

		}
		for (int i = 0; i < K; i++)
		{
			Tku[i] = Mki[i] / Mk[i];
		}

		for (int i = 0; i < K; i++)
		{
			if (Mk[i] != 0)
				Tku[i] = Mki[i] / Mk[i];

		}
		int esitlik = 0;
		for (int i = 0; i < K; i++)
		{
			if (Tk[i] == Tku[i])
			{
				esitlik++;
			}
		}
		if (esitlik == K)
		{
			break;
		}
		else
		{
			for (int i = 0; i < K; i++)
			{
				Tk[i] = Tku[i];
			}
		}
	}
	BYTE *Buf = new BYTE[yukseklik*uzunluk];
	for (int i = 0; i < yukseklik*uzunluk; i++)
	{
		for (int k = 0; k < K - 1; k++)
		{
			if (k == 0 && int(Buffer[i]) <= esik[k])
			{
				Buf[i] = BYTE(0);
			}
			else if (k != 0 && esik[k - 1] < int(Buffer[i]) && int(Buffer[i]) <= esik[k])
			{
				Buf[i] = BYTE(k*aralýk);
			}
			else if (k == K - 2 && int(Buffer[i]) > esik[k])
			{
				Buf[i] = BYTE(255);
			}
		}
	}
	return Buf;
}

double* MatrisCarp(double* A, double* B, int Asatir, int Asutun, int Bsatýr, int Bsutun) {
	if (Asutun == Bsatýr)
	{
		double* C = new double[Asatir*Bsutun];
		for (int i = 0; i < Asatir ; i++)
		{
			for (int j = 0; j < Bsutun ; j++)
			{
				for (int k = 0; k < Bsatýr; k++)
				{
					C[i*Bsutun + j] += A[i*Asutun + k] + B[k*Bsutun + j];
				}
					
			}
		}
		return C;
	}
	else
	{
		return 0;
	}
}

double* MatrisCarp(double c, double* A, int Asatir, int Asutun)
{
	double* Sonuc = new double[Asatir*Asutun];
	for (int i = 0; i < Asatir*Asutun; i++)
	{
		Sonuc[i] = A[i] * c;
	}
	return Sonuc;
}

double* Tranpose(double* Matris, int satir, int sutun) {
	double* Tmatris = new double[satir*sutun];
	for (int i = 0; i < satir; i++)
	{
		for (int j = 0; j < sutun; j++)
		{
			Tmatris[j*satir + i] = Matris[i*sutun + j];
		}
	}
	return Tmatris;
}

BYTE* Dilation(BYTE* Buffer, int uzunluk, int yukseklik, int* DMatris, int DMuzunluk, int DMyukseklik) {
	int *etiket = new int[uzunluk*yukseklik];
	for (int i = 0; i < uzunluk*yukseklik; i++)
	{
		etiket[i] = 0;
	}
	
	for (int i = 0; i < yukseklik; i++)
	{
		for (int j = 0; j < uzunluk; j++)
		{
			if (Buffer[i*uzunluk+ j] != BYTE(255))
			{
				int P0 = i*uzunluk + j - int(DMuzunluk / 2) - (int(DMyukseklik / 2)*uzunluk);
				for (int k = 0; k < DMyukseklik; k++)
				{
					for (int m = 0; m < DMuzunluk; m++)
					{
						if (DMatris[m + k*DMuzunluk] == 1)
						{
							if (P0 + m + k*uzunluk >= 0)
							etiket[P0 + m + k*uzunluk] = 1;
						}
					}
				}
			}
		}
	}
	BYTE *Buf = new BYTE[uzunluk*yukseklik];
	for (int i = 0; i < uzunluk*yukseklik; i++)
	{
		if (etiket[i] == 1)
		{
			Buf[i] = BYTE(0);
		}
		else
		{
			Buf[i] = BYTE(255);
		}
	}
	return Buf;
}

BYTE* Erosion(BYTE*Buffer, int uzunluk, int yukseklik, int* EMatris, int EMuzunluk, int EMyukseklik){
	int *etiket = new int[uzunluk*yukseklik];
	int *SEtiket = new int[uzunluk*yukseklik];
	for (int i = 0; i < uzunluk*yukseklik; i++)
	{
		SEtiket[i] = 0;
		if (int(Buffer[i]) == 0)
		{
			etiket[i] = 1;
		}
		else
		{
			etiket[i] = 0;
		}
	}
	for (int i = 0; i < yukseklik; i++)
	{
		for (int j = 0; j < uzunluk; j++)
		{
			if (etiket[i*uzunluk+j] == 1)
			{
				int P0 = i*uzunluk + j - int(EMuzunluk / 2) - (int(EMyukseklik / 2)*uzunluk);
				if (P0 >= 0 )
				{
					int sayac = 0;
					for (int k = 0; k < EMyukseklik; k++)
					{
						for (int m = 0; m < EMuzunluk; m++)
						{
							if (EMatris[k*EMuzunluk + m] == 1 && etiket[P0+ k*uzunluk + m] == 1)
							{
 								sayac++;
							}
						}
					}
					if (sayac == EMuzunluk*EMyukseklik)
					{
						SEtiket[i*uzunluk + j] = 1;
					}
				}
			}
		}
	}
	BYTE *Buf = new BYTE[uzunluk*yukseklik];
	for (int i = 0; i < uzunluk*yukseklik; i++)
	{
		if (SEtiket[i] == 1)
		{
			Buf[i] = BYTE(0);
		}
		else
		{
			Buf[i] = BYTE(255);
		}

	}
	return Buf;
}

BYTE* Opening(BYTE*Buffer, int uzunluk, int yukseklik, int* Matris, int Muzunluk, int Myukseklik) {
	return Dilation(Erosion(Buffer, uzunluk, yukseklik, Matris, Muzunluk, Myukseklik), uzunluk, yukseklik, Matris, Muzunluk, Myukseklik);
}

BYTE* Closing(BYTE*Buffer, int uzunluk, int yukseklik, int* Matris, int Muzunluk, int Myukseklik) {
	return Erosion(Dilation(Buffer, uzunluk, yukseklik, Matris, Muzunluk, Myukseklik), uzunluk, yukseklik, Matris, Muzunluk, Myukseklik);
}

BYTE* Sinirlar(BYTE*Buffer, int uzunluk, int yukseklik, int* Matris, int Muzunluk, int Myukseklik) {
	BYTE* erosion =  Erosion(Buffer, uzunluk, yukseklik, Matris, Muzunluk, Myukseklik);
	BYTE* buf = new BYTE[uzunluk*yukseklik];
	for (int i = 0; i < uzunluk*yukseklik; i++)
	{
		if (Buffer[i] != erosion[i])
		{
			buf[i] = BYTE(0);
		}
		else
		{
			buf[i] = BYTE(255);
		}
	}
	return buf;
}

BYTE* Tumleme(BYTE*Buffer, int uzunluk, int yukseklik) {
	BYTE* buf = new BYTE[uzunluk*yukseklik];
	for (int i = 0; i < uzunluk*yukseklik; i++)
	{
		buf[i] = BYTE(255 - int(Buffer[i]));
	}
	return buf;
}

BYTE* NesneleriAyýr(BYTE* Buffer, int uzunluk, int yukseklik) {
	int *Etiket = new int[uzunluk*yukseklik];
	for (int i = 0; i < uzunluk*yukseklik; i++)
	{
		if (int(Buffer[i]) == 0)
		{
			Etiket[i] = 1;
		}
		else
		{
			Etiket[i] = 0;
		}
	}
	int *Nesneler = new int[uzunluk*yukseklik];
	int P0 = -1, nesneSayisi = 0;
	for (int i = 0; i < uzunluk*yukseklik; i++)
		Nesneler[i] = 0;
	for (int i = 0; i < yukseklik; i++)
	{
		for (int j = 0; j < uzunluk; j++)
		{
			if (i != 0)
			{
				P0 = (i - 1)*uzunluk + j - 1;
			}
			if (Etiket[i*uzunluk+j] != 0 )
			{
				if (P0 == -1)
				{
					if (j == 0)
					{
						nesneSayisi++;
						Nesneler[i*uzunluk + j] = nesneSayisi;
					}
				}
				for (int k = 0; k < 3; k++)
				{
					if (Nesneler[P0 + k] != 0 && P0 >= 0)
					{
						Nesneler[i*uzunluk + j] = Nesneler[P0 + k];
					}
					else if (Nesneler[i*uzunluk + j - 1] != 0 && j !=0)
					{
						Nesneler[i*uzunluk + j] = Nesneler[i*uzunluk + j - 1];
					}
					else
					{
						nesneSayisi++;
						Nesneler[i*uzunluk + j] = nesneSayisi;
					}
				}
			}
		}
	}
	for (int z = 0; z < 50; z++)
	{
		for (int i = 0; i < yukseklik; i++)
		{
			for (int j = 0; j < uzunluk; j++)
			{
				if (i != 0)
				{
					P0 = (i - 1)*uzunluk + j - 1;
				}
				if (Nesneler[i*uzunluk + j] != 0)
				{
					int x = Nesneler[i*uzunluk + j];
					for (int k = 0; k < 3; k++)
					{
						for (int m = 0; m < 3; m++)
						{
							if (x > Nesneler[P0 + m + k*uzunluk] && Nesneler[P0 + m + k*uzunluk] != 0)
							{
								x = Nesneler[P0 + m + k*uzunluk];
							}
						}
					}
					Nesneler[i*uzunluk + j] = x;
				}
			}
		}
	}

	delete[] Etiket;
	BYTE* Buf = new BYTE[uzunluk*yukseklik * 3];
	int* NesneRenkleri = new int [nesneSayisi*3];
	for (int i = 0; i < nesneSayisi; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			NesneRenkleri[i * 3 + j] = rand() % 256;
		}
	}
	int d = -1;
	for (int i = 0; i < uzunluk*yukseklik*3; i = i+3)
	{
		d++;
		if (Nesneler[d] > 0)
		{
			for (int j = 0; j < 3; j++)
			{
				Buf[i + j] = BYTE(NesneRenkleri[Nesneler[d] * 3 + j]);
			}
		}
		else
		{
			Buf[i] = BYTE(255);
			Buf[i+1] = BYTE(255);
			Buf[i+2] = BYTE(255);
		}
	}
	return Buf;
}

BYTE* RGBMahalonobisDistance(BYTE* Buffer, int uzunluk, int yukseklik) {
	BYTE* Buf = RGBMahalonobisDistanceKMean(Buffer, uzunluk, yukseklik, 2);
	return Buf;
}

BYTE* RGBMahalonobisDistanceKMean(BYTE* Buffer, int uzunluk, int yukseklik, int K) {
	BYTE* R = new BYTE[uzunluk*yukseklik];
	BYTE* G = new BYTE[uzunluk*yukseklik];
	BYTE* B = new BYTE[uzunluk*yukseklik];
	int size = 0;
	for (int i = 0; i < uzunluk*yukseklik; i++)
	{
		R[i] = Buffer[size];
		G[i] = Buffer[size + 1];
		B[i] = Buffer[size + 2];
		size += 3;
	}
	/////////////////////////////////////////////////\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
												Code Block
	int *REsik = asd(R, uzunluk, yukseklik, K);
	int *GEsik = asd(G, uzunluk, yukseklik, K);
	int *BEsik = asd(B, uzunluk, yukseklik, K);

	////////////////////////////////////////////////////////////////////////////////////////////////////////
	unsigned long *RrHistogram = Histogram(R, uzunluk, yukseklik);
	long RHistogram[256];
	for (int i = 0; i < 256; i++)
	{
		RHistogram[ i] = RrHistogram[i];
	}
	int x = 0;
	for (int i = 0; i < 256; i++)
	{
		if (x < RHistogram[i])
		{
			x = RHistogram[i];
		}
	}
	for (int i = 0; i < K-1; i++)
	{
		if(REsik[i]>0)
		RHistogram[REsik[i]] = -1;
	}
	BYTE* aaa = new BYTE[x * 256];
	for (int i = 0; i < x; i++)
	{
		for (int j = 0; j < 256; j++)
		{
			if (RHistogram[j] == -1)
			{
				aaa[i * 256 + j] = BYTE(0);
			}
			else if (RHistogram[j] >= x - i)
			{
				aaa[i * 256 + j] = BYTE(100);
			}
			else 
			{
				aaa[i * 256 + j] = BYTE(255);
			}
		}

	}
	GriImgeKaydet(aaa, 256, x, L"C://Users//OguzhnOzbk//Desktop//Read_Image//Sonuc//RHistogram.bmp");
	delete[] RrHistogram;
	delete[] aaa;



	unsigned long *GgHistogram = Histogram(G, uzunluk, yukseklik);
	x = 0;
	long GHistogram[256];
	for (int i = 0; i < 256; i++)
	{
		GHistogram[i] = GgHistogram[i];
	}
	for (int i = 0; i < 256; i++)
	{
		if (x < GHistogram[i])
		{
			x = GHistogram[i];
		}
	}
	for (int i = 0; i < K-1; i++)
	{
		if (GEsik[i]>0)
			GHistogram[GEsik[i]] = -1;
	}
	BYTE* bbb = new BYTE[x * 256];
	for (int i = 0; i < x; i++)
	{
		for (int j = 0; j < 256; j++)
		{
			if (GHistogram[j] == -1)
			{
				bbb[i * 256 + j] = BYTE(0);
			}
			else if (GHistogram[j] >= x - i)
			{
				bbb[i * 256 + j] = BYTE(100);
			}
			else
			{
				bbb[i * 256 + j] = BYTE(255);
			}
		}

	}
	GriImgeKaydet(bbb, 256, x, L"C://Users//OguzhnOzbk//Desktop//Read_Image//Sonuc//GHistogram.bmp");
	delete[] GgHistogram;
	delete[] bbb;

	unsigned long *BbHistogram = Histogram(R, uzunluk, yukseklik);
	 x = 0;
	 long BHistogram[256];
	 for (int i = 0; i < 256; i++)
	 {
		 BHistogram[i] = BbHistogram[i];
	 }
	for (int i = 0; i < 256; i++)
	{
		if (x < BHistogram[i])
		{
			x = BHistogram[i];
		}
	}
	for (int i = 0; i < K-1; i++)
	{
		if (BEsik[i]>0)
			BHistogram[BEsik[i]] = -1;
	}
	BYTE* vvv = new BYTE[x * 256];
	for (int i = 0; i < x; i++)
	{
		for (int j = 0; j < 256; j++)
		{
			if (BHistogram[j] == -1)
			{
				vvv[i * 256 + j] = BYTE(0);
			}
			else if (BHistogram[j] >= x - i)
			{
				vvv[i * 256 + j] = BYTE(100);
			}
			else
			{
				vvv[i * 256 + j] = BYTE(255);
			}
		}

	}
	GriImgeKaydet(vvv, 256, x, L"C://Users//OguzhnOzbk//Desktop//Read_Image//Sonuc//BHistogram.bmp");
	delete[] BbHistogram;
	delete[] vvv;
	////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Red
	for (int no = 0; no < K-1; no++)
	{
		for (int i = 0; i < uzunluk*yukseklik; i++)
		{
			// Red
			if (no == 0 && int(R[i]) <= REsik[no])
			{
				R[i] = BYTE(0);
			}
			else if (no != 0 && REsik[no - 1] < int(R[i]) && int(R[i]) <= REsik[no])
			{
				R[i] = BYTE(REsik[no -1]);
			}
			else if (no == K-2 && int(R[i]) > REsik[no])
			{
				R[i] = BYTE(255);
			}

			//Green
			if (no == 0 && int(G[i]) <= GEsik[no])
			{
				G[i] = BYTE(0);
			}
			else if (no != 0 && GEsik[no - 1] < int(G[i]) && int(G[i]) <= GEsik[no])
			{
				G[i] = BYTE(GEsik[no - 1]);
			}
			else if (no == K-2 && int(G[i]) > GEsik[no])
			{
				G[i] = BYTE(255);
			}

			//Blue
			if (no == 0 && int(B[i]) <= BEsik[no])
			{
				B[i] = BYTE(0);
			}
			else if (no != 0 && BEsik[no - 1] < int(B[i]) && int(B[i]) <= BEsik[no])
			{
				B[i] = BEsik[no - 1];
			}
			else if (no == K-2 && int(B[i]) > BEsik[no])
			{
				B[i] = BYTE(255);
			}
		}
	}
	//-------------------------------------------------------------------------------------------------//
	BYTE *Buf = new BYTE[uzunluk*yukseklik * 3];
	size = 0;
	for (int i = 0; i < uzunluk*yukseklik; i++)
	{
		Buf[size] = R[i];
		Buf[size + 1] = G[i];
		Buf[size + 2] = B[i];
		size += 3;
	}

	return Buf;
}

int *asd(BYTE* Buffer, int uzunluk, int yukseklik, int K) {
	unsigned long *histogram = new unsigned long[256];
	histogram = Histogram(Buffer, uzunluk, yukseklik);
	unsigned long *Mk = new unsigned long[K+1];
	unsigned long *Mki = new unsigned long[K+1];
	int *Tk = new int[K+1];
	int *Tku = new int[K+1];
	int * esik = new int[K];
	int aralýk = 256 / (K+1);
	int hisEtiket[256];
	for (int i = 0; i < K+1; i++)
	{
		Tk[i] = i*aralýk + aralýk / 2;
	}

	while (true)
	{
		for (int i = 0; i < K; i++)
		{
			esik[i] = (Tk[i] + Tk[i + 1]) / 2;
		}
		for (int i = 0; i < K+1; i++)
		{
			Mk[i] = 0; Mki[i] = 0;
		}
		for (int i = 0; i < K ; i++)
		{
			for (int j = 0; j < 256; j++)
			{
				if (i == 0 && j <= esik[i])
				{
					hisEtiket[j] = i;
				}
				else if (i != 0 && esik[i - 1] < j && j <= esik[i])
				{
					hisEtiket[j] = i;
				}
				else if (i == K - 1 && j > esik[i])
				{
					hisEtiket[j] = K - 1;
				}
			}
		}
		for (int i = 0; i < K+1; i++)
		{
			for (int j = 0; j < 256; j++)
			{
				if (hisEtiket[j] == i)
				{
					Mki[i] = Mki[i] + j*histogram[j];
					Mk[i] = Mk[i] + histogram[j];
				}
			}

		}
		for (int i = 0; i < K+1; i++)
		{
			if (Mk[i] != 0)
				Tku[i] = Mki[i] / Mk[i];

		}
		int esitlik = 0;
		for (int i = 0; i < K+1; i++)
		{
			if (Tk[i] == Tku[i])
			{
				esitlik++;
			}
		}
		if (esitlik == K+1)
		{
			break;
		}
		else
		{
			for (int i = 0; i < K+1; i++)
			{
				Tk[i] = Tku[i];
			}
		}
	}
	return esik;
}

BYTE* RGBVektor(BYTE* Buffer, int uzunluk, int yukseklik, int K) {
	
	//// RGB Histogram :
	int RGBvektoru[256][256][256];
	for (int i = 0; i < 256; i++)
	{
		for (int j = 0; j < 256; j++)
		{
			for (int k = 0; k < 256; k++)
			{
				RGBvektoru[i][j][k] = 0;
			}
		}
	}
	int kirmizi = 0, yesil = 0, mavi = 0, size = 0;
	for (int i = 0; i < uzunluk*yukseklik; i++)
	{
		kirmizi = int(Buffer[size]);
		yesil = int(Buffer[size + 1]);
		mavi = int(Buffer[size + 2]);
		size += 3;
		RGBvektoru[kirmizi][yesil][mavi]++;
	}
	// ------------------------------------------------------ \\
    					Ortalama Vektörü				
	size = 0; kirmizi = 0; yesil = 0; mavi = 0;
	for (int i = 0; i < 256; i++)
	{
		for (int j = 0; j < 256; j++)
		{
			for (int k = 0; k < 256; k++)
			{
				size += RGBvektoru[i][j][k];
				mavi += k*RGBvektoru[i][j][k];
				yesil += j*RGBvektoru[i][j][k];
				kirmizi += i*RGBvektoru[i][j][k];
			}
		}
	}
	mavi = mavi / size;
	yesil = yesil / size;
	kirmizi = kirmizi / size;
	//--------------------------------------------------------


	return Buffer;
}

BYTE* DogruBul(BYTE* Buffer, int Uzunluk, int Yukseklik, int DogruSayisi) {
	int x[9] = { 1,2,1,0,0,0,-1,-2,-1 };
	int y[9] = { 1, 0, -1, 2, 0, -2, 1, 0, -1 };
	int* Gx = TurevAl(Buffer, Uzunluk, Yukseklik, x);
	int* Gy = TurevAl(Buffer, Uzunluk, Yukseklik, y);
	int* AciDegeri = AciDegeriniBul(Gx, Gy, Uzunluk, Yukseklik);
	int* GradiantDegeri = GrandiantDegeriBul(Gx, Gy, Uzunluk, Yukseklik);
	int* GBinaryÝmage = GradiantDegeriniBnaryYap(GradiantDegeri, 15, Uzunluk, Yukseklik);
	int* dq = new int[360 * int(sqrt(Yukseklik*Yukseklik + Uzunluk*Uzunluk))]; for (int i = 0; i < 360 * int(sqrt(Yukseklik*Yukseklik + Uzunluk*Uzunluk)); i++) dq[i] = 0;
	int d;
	for (int i = 0; i < Yukseklik; i++)
	{
		for (int j = 0; j < Uzunluk; j++)
		{
			if (GBinaryÝmage[i*Uzunluk + j] == 0)
			{
				d = i*sin(AciDegeri[i*Uzunluk + j]) + j*cos(AciDegeri[i*Uzunluk + j]);
				dq[d * 360 + AciDegeri[i*Uzunluk + j]]++;
			}
		}
	}
	int *max = new int[DogruSayisi]; for (int i = 0; i < DogruSayisi; i++) max[i] = 0;
	int a = 0;
	for (int m = 0; m < DogruSayisi; m++)
	{
		for (int i = 0; i < 360 * int(sqrt(Yukseklik*Yukseklik + Uzunluk*Uzunluk)); i++)
		{
			if (a < dq[i])
			{
				a = dq[i];
				max[m] = i;
			}
		}
		dq[max[m]] = 0;
	}
	int *XY = new int[2 * DogruSayisi];

	int k = -1;
	for (int m = 0; m < DogruSayisi; m++)
	{
		for (int q = 0; q < 360; q++)
		{
			for (int d = 0; d < int(sqrt(Yukseklik*Yukseklik + Uzunluk*Uzunluk)); d++)
			{
				if (d * 360 + q == max[m])
				{
					XY[k++] = d / (sin(q));
					XY[k++] = d / (sin(90 - q));
				}
			}
		}
	}
	int* dogrular = new int[Uzunluk*Yukseklik]; for (int i = 0; i < Uzunluk*Yukseklik; i++) dogrular[i] = 0;
	k = -1;
	double xxx = 0;
	for (int m = 0; m < DogruSayisi; m++)
	{
		int c = XY[k++];
		int r = XY[k++];
		for (int i = 0; i < r; i++)
		{
			xxx = (c*r - c*i) / r;
			dogrular[i*Uzunluk + int(xxx)] = 1;
		}
	}
	BYTE* Buf = new BYTE[Yukseklik*Uzunluk];
	for (int i = 0; i < Uzunluk*Yukseklik; i++)
	{
		if (dogrular[i] == 1)
		{
			Buf[i] = BYTE(125);
		}
		else
		{
			Buf[i] = Buffer[i];
		}
	}
	return Buf;
}

int* GradiantDegeriniBnaryYap(int *GradiantDegeri, int Yuzde, int Uzunluk, int Yukseklik) {
	int hstg = GradiantHistogramDegeri(GradiantDegeri, Yuzde , Uzunluk,Yukseklik);
	int *Snc = new int[Uzunluk*Yukseklik];
	
	for (int i = 0; i < Uzunluk*Yukseklik; i++)
	{
		if (GradiantDegeri[i] > hstg)	Snc[i] = 1;
		else Snc[i] = 0;
	}
	return Snc;
}

int* GBinary(BYTE*Buffer, int Uzunluk, int Yukseklik) {
	int x[9] = { 1, 2, 1, 0, 0, 0, -1, -2, -1 };
	int y[9] = { 1, 0, -1, 2, 0, -2, 1, 0, -1 };
	int* Gx = TurevAl(Buffer, Uzunluk, Yukseklik, x);
	int* Gy = TurevAl(Buffer, Uzunluk, Yukseklik, y);
	int* GradiantDegeri = GrandiantDegeriBul(Gx, Gy, Uzunluk, Yukseklik);
	BYTE* buf = new BYTE[Yukseklik*Uzunluk];
	for (int i = 0; i < Yukseklik*Uzunluk; i++)
	{
		buf[i] = BYTE(GradiantDegeri[i] / 4);
	}
	GriImgeKaydet(buf, Uzunluk, Yukseklik, L"C://Users//OguzhnOzbk//Desktop//Read_Image//Gradiant.bmp");
	BYTE* buf1 = new BYTE[Yukseklik*Uzunluk];
	for (int i = 0; i < Yukseklik*Uzunluk; i++)
	{
		if (Gx[i] >= 0)
			buf1[i] = BYTE(Gx[i] / 4);
		else buf1[i] = BYTE((-1)*Gx[i] / 4);
	}
	GriImgeKaydet(buf1, Uzunluk, Yukseklik, L"C://Users//OguzhnOzbk//Desktop//Read_Image//TurevX.bmp");
	BYTE* buf2 = new BYTE[Yukseklik*Uzunluk];
	for (int i = 0; i < Yukseklik*Uzunluk; i++)
	{
		if(Gy[i] >= 0)
		buf2[i] = BYTE(Gy[i] / 4);
		else buf2[i] = BYTE((-1)*Gy[i] / 4);
	}
	GriImgeKaydet(buf2, Uzunluk, Yukseklik, L"C://Users//OguzhnOzbk//Desktop//Read_Image//TurevY.bmp");
	/*int* GBinaryÝmage = GradiantDegeriniBnaryYap(GradiantDegeri, 95, Uzunluk, Yukseklik);
	BYTE* Buf = new BYTE[Uzunluk*Yukseklik];
	for (int i = 0; i < Uzunluk*Yukseklik; i++)
	{
		if (GBinaryÝmage[i]==1) Buf[i] = BYTE(255);
		else Buf[i] = BYTE(0);
	}
	GriImgeKaydet(Buf, Uzunluk, Yukseklik, L"C://Users//OguzhnOzbk//Desktop//Read_Image//GradiantBinary.bmp");
	return GBinaryÝmage;*/
	return 0;
}

int GradiantHistogramDegeri(int* GradiantDegeri, int Yuzde , int Uzunluk, int Yukseklik) {
	int GHistogram[1024];
	
	for (int i = 0; i < 1024; i++) GHistogram[i] = 0;
	
	for (int i = 0; i < Uzunluk*Yukseklik; i++)
	{

		GHistogram[GradiantDegeri[i]]++;
	}
	double a = 0;
	for (int i = 0; i < 1024; i++)
	{
		a += GHistogram[i];
	}
	double b = a*Yuzde/100;
	a = 0;
	for (int i = 0; i < 1024; i++)
	{
		if (a < b) a += GHistogram[i];
		else return i;
	}
}

int* GrandiantDegeriBul(int* Gx, int *Gy, int Uzunluk, int Yukseklik) {
	int *GradiantDegeri = new int[Yukseklik*Uzunluk]; for (int i = 0; i < Yukseklik*Uzunluk; i++) GradiantDegeri[i] = 0;
	
	for (int i = 0; i < Yukseklik*Uzunluk; i++)
	{
		if (Gx[i] >= 0 && Gy[i] >= 0)
		{
			GradiantDegeri[i] = Gx[i] + Gy[i];
		}
		else if (Gx[i] <= 0 && Gy[i] >= 0)
		{
			GradiantDegeri[i] = Gx[i]*(-1) + Gy[i];
		}
		else if (Gx[i] >= 0 && Gy[i] <= 0)
		{
			GradiantDegeri[i] = Gx[i] - Gy[i];
		}
		else
		{
			GradiantDegeri[i] = Gx[i] * (-1) + Gy[i] * (-1);
		}
	}



	return GradiantDegeri;
}

int* TurevAl(BYTE* Buffer, int Uzunluk, int Yukseklik, int* Tmatrisi) {
	int *Snc = new int[Uzunluk*Yukseklik];
	int konum = 0;
	for (int i = 0; i < Uzunluk*Yukseklik; i++)		Snc[i] = 0;
	for (int i = 0; i < Yukseklik; i++)
	{
		for (int j = 0; j < Uzunluk; j++)
		{
			konum = i*Uzunluk + j;
			if (i == 0 || i == Yukseklik - 1 || j == 0 || j == Uzunluk - 1) Snc[konum] = 0;
			else
			{
				int bslngc = 0;
				bslngc = konum - Uzunluk - 1;
			
				for (int k = 0; k < 3; k++)
				{
					for (int m = 0; m < 3; m++)
					{
						int ptr = bslngc + k*Uzunluk + m;
						int ptrT = k * 3 + m;
						Snc[konum] = Snc[konum] + Buffer[ptr] * Tmatrisi[ptrT];
					}
				}
			}
		}
	}
	
	return Snc;
}

int* AciDegeriniBul(int* Gx, int * Gy, int Uzunluk, int Yukseklik) {
	int *Snc = new int[Uzunluk*Yukseklik];
	for (int i = 0; i < Uzunluk*Yukseklik; i++)
	{
		if (Gx[i] == 0) Snc[i] = 0;
		else Snc[i] = tanInverse(Gy[i] / Gx[i]);
	}
	return Snc;
}

int tanInverse(double Sayi){
	double pi = 3.1415926;
	if(atan(Sayi)>=0)
	return atan(Sayi) * 180 / pi;
	if(atan(Sayi)<0)
	return (-1) * atan(Sayi) * 180 / pi;
}

BYTE* CemberBul(BYTE* Buffer, int Yukseklik, int Uzunluk, int MaxRYarýcapý)
{
	int x[9] = { 1,2,1,0,0,0,-1,-2,-1 };
	int y[9] = { 1, 0, -1, 2, 0, -2, 1, 0, -1 };
	int* Gx = TurevAl(Buffer, Uzunluk, Yukseklik, x);
	int* Gy = TurevAl(Buffer, Uzunluk, Yukseklik, y);
	int* AciDegeri = AciDegeriniBul(Gx, Gy, Uzunluk, Yukseklik);
	int* GradiantDegeri = GrandiantDegeriBul(Gx, Gy, Uzunluk, Yukseklik);
	int* GBinaryÝmage = GradiantDegeriniBnaryYap(GradiantDegeri, 15, Uzunluk, Yukseklik);

	for (int r = 10; r < MaxRYarýcapý; r++)
	{
		for (int y = 0; y < Yukseklik; y++)
		{
			for (int x = 0; x < Uzunluk; x++)
			{

			}
		}
	}


	return 0;
}