// Image_Process1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <windows.h>
#include <math.h>

//User defined includes
#include "imge_bmp.h"
#include "fonksiyonlar.h"


using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	LPCTSTR input, output;
	int Width, Height;
	long Size, new_size, newWidth, newHeight;
	int*B = new int[9];
	float dizi[9] = { 0.25,0.50,0.25,0.50,1,0.50,0.25,0.50,0.25 };
	for (int i = 0; i < 9; i++)
	{
		B[i] = 1;
	}
	input = L"C://Users//OguzhnOzbk//Desktop//Read_Image//Orjinal Resimler//lenna.bmp";
	
	BYTE* buffer = LoadBMP(&Width, &Height, &Size, input);
	/*BYTE* Buf = RGBMahalonobisDistanceKMean(buffer, Width, Height, 12);
	BYTE* Buff = RGBMahalonobisDistance(buffer, Width, Height);
*/
	BYTE* raw_intensity = ConvertBMPToIntensity(buffer, Width, Height);
	GBinary(raw_intensity, Width, Height);
	BYTE* binaryImage = BinaryImageIntensityMinumumDistance(raw_intensity, Width, Height);
	//BYTE* dogru = DogruBul(raw_intensity, Width, Height, 5);
	//cemberCiz(raw_intensity, 100, 100, 25, Width, Height);
	/*BYTE* maskMatris = Mask(raw_intensity, Width, Height, dizi, 9, &newWidth, &newHeight);
	
	BYTE* binaryMahalonobisImage = BinaryImageIntensityMahalonobisDistance(raw_intensity, Width, Height);
	BYTE* KMeanImage = KmeanImageIntensityMinumumDistance(raw_intensity, Width, Height, 6);
	BYTE* KMeanMahalonobis = KmeanImageIntensityMahalonobisDistance(raw_intensity, Width, Height, 6);
	BYTE* dilation = Dilation(binaryImage, Width, Height, B, 3, 3);
	BYTE* erosion = Erosion(binaryImage, Width, Height, B, 3, 3);
	BYTE* opening = Opening(binaryImage, Width, Height, B, 3, 3);
	BYTE* closing = Closing(binaryImage, Width, Height, B, 3, 3);
	BYTE* sinirlar = Sinirlar(binaryImage, Width, Height,B,3,3);
    BYTE* tumleme = Tumleme(binaryImage, Width, Height);
*/
	BYTE* NesnelerRenkli = NesneleriAyýr(binaryImage, Width, Height);

	char ch;

	cout << "Sonucu diske kaydetsin mi? E/H:"; cin >> ch;
	if ((ch == 'E') || (ch == 'e')) {

		GriImgeKaydet(raw_intensity, Width, Height, L"C://Users//OguzhnOzbk//Desktop//Read_Image//Sonuc//Intensity.bmp");
		GriImgeKaydet(raw_intensity, Width, Height, L"C://Users//OguzhnOzbk//Desktop//Read_Image//Intensity.bmp");
		//GriImgeKaydet(dogru, Width, Height, L"C://Users//OguzhnOzbk//Desktop//Read_Image//IntensityDogru.bmp");
		/*GriImgeKaydet(maskMatris, newWidth, newHeight, L"C://Users//OguzhnOzbk//Desktop//Read_Image//Sonuc//Mask.bmp");
		GriImgeKaydet(KMeanImage, Width, Height, L"C://Users//OguzhnOzbk//Desktop//Read_Image//Sonuc//KMeanImage.bmp");
		GriImgeKaydet(KMeanMahalonobis, Width, Height, L"C://Users//OguzhnOzbk//Desktop//Read_Image//Sonuc//KMeanMahalonobisImage.bmp");
		GriImgeKaydet(binaryImage, Width, Height, L"C://Users//OguzhnOzbk//Desktop//Read_Image//Sonuc//BinaryImage.bmp");
		GriImgeKaydet(binaryMahalonobisImage, Width, Height, L"C://Users//OguzhnOzbk//Desktop//Read_Image//Sonuc//BinaryMahalonobisImage.bmp");
		GriImgeKaydet(dilation, Width, Height, L"C://Users//OguzhnOzbk//Desktop//Read_Image//Sonuc//BinaryImageDilation.bmp");
		GriImgeKaydet(erosion, Width, Height, L"C://Users//OguzhnOzbk//Desktop//Read_Image//Sonuc//BinaryImageErosion.bmp");
		GriImgeKaydet(opening, Width, Height, L"C://Users//OguzhnOzbk//Desktop//Read_Image//Sonuc//BinaryImageOpening.bmp");
		GriImgeKaydet(closing, Width, Height, L"C://Users//OguzhnOzbk//Desktop//Read_Image//Sonuc//BinaryImageClosing.bmp");
		GriImgeKaydet(sinirlar, Width, Height, L"C://Users//OguzhnOzbk//Desktop//Read_Image//Sonuc//BinaryImageSinirlar.bmp");
		GriImgeKaydet(tumleme, Width, Height, L"C://Users//OguzhnOzbk//Desktop//Read_Image//Sonuc//BinaryImageTumleme.bmp");
*/
		SaveBMP(buffer, Width, Height, Size, L"C://Users//OguzhnOzbk//Desktop//Read_Image//Sonuc//Buffer.bmp");
		//SaveBMP(Buf, Width, Height, Size, L"C://Users//OguzhnOzbk//Desktop//Read_Image//Sonuc//RGBMahalonobisDistanceKMean.bmp");
		//SaveBMP(Buff, Width, Height, Size, L"C://Users//OguzhnOzbk//Desktop//Read_Image//Sonuc//RGBMahalonobisDistance.bmp");
		///*
		SaveBMP(NesnelerRenkli, Width, Height, Size, L"C://Users//OguzhnOzbk//Desktop//Read_Image//Sonuc//Nesneler.bmp");
		Histogram(raw_intensity, Width, Height);
	}


	delete[] buffer;
	delete[] B;
	delete[] raw_intensity;
	//delete[] dogru;
	/*delete[] maskMatris;
	delete[] binaryImage;
	delete[] binaryMahalonobisImage;
	delete[] KMeanImage;
	delete[] KMeanMahalonobis;
	delete[] dilation;
	delete[] erosion;
	delete[] opening;
	delete[] closing;
	delete[] sinirlar;
	delete[] tumleme;
	delete[] NesnelerRenkli;*/
	/*delete[] Buf;
	delete[] Buff;
*/
	return 0;
}







