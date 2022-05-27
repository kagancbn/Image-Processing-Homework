#include "image.h"
#include "humoments.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb/include/stb_image.h"
#include <iostream>
#include <vector>
#include "../../read_image/read_image/humoments.h"
/*
Ýmage segmentation -- Object Detection
-> Edge-Based Algorithm
-> Histogram Ýnstensity- Based Algorithm, Threshold 
-> Region Growing- Based Algorithm
Amacý objelerin Sýnýrlarýný Belirlemek ve sonra image classification yapmak

K-Means Thresholding

choose random 2 T which is T1,T2 between 0-255
iki tane rastgele T deðeri seç (histogram i (index) deðeri)
int T1,T2;

check all histogram indexes (0-255) which T is closest to T1,or T2,
and tag indexes T1, T2
while i=255
if(|T1-i|>|T2-i|) T1[j]=i;
else T2[k]=i;
i++

 ÝLK ADIM
Bütün histogramdaki i deðerlerinin için hangi T deðerine daha yakýn olduðunu belirle ve iþaretle

Tüm T1'in i (hist index) deðerlerinin ortalama aðýrlýk merkezini hesapla aðýrlýk merkezini yeni T1 ata
Tüm T2s'in i (hist index) deðerlerinin ortalama aðýrlýk merkezini hesapla aðýrlýk merkezini yeni T2 ata

new T1' = ortalama T1;
new t2' = ortalama t2;

t1* epsilon == new t1' ise dur deðilse devam et
t2* epsilon == new t2' ise dur deðilse devam et

ÝKÝNCÝ ADIM ( ÝLK ÝTERASYONU AYNI ÞEKÝL DEVAM ETTÝR)
Bütün histogramdaki i deðerlerinin için hangi T deðerine daha yakýn olduðunu belirle ve tag'le
if( |Hist[i]-T1| < |Hist[i]-T2| ) hist[i]=T1 tag;
else hist[i] = T2 tag; 
.
.
EN SON ADIM
if (hist[i] = T1) hist[i] = 0;
if (hist[i] = T2) hist[i] = 255;

--------------------------------------------------------------------------------------------------
SEGMANTASYONUN ÝKÝNCÝ ADIMI 

---DETECTÝON-------

OBJELERÝ (TANELERÝ) ARKA PLANDAN AYIRDIKTAN SONRA HER BÝR TANENÝN NE OLDUÐNU ANLAMAK ÝÇÝN
HER BÝR OBJEYÝ (TANEYÝ) NE OLDUÐUNU ANLAMAK ÝÇÝN ALGORÝTMAYA GÖNDERMEDEN ÖNCE BOUNDÝNG BOX A ALLMAK GEREKÝYOR

Connectivity and labeling

arka plan 0 ön plan 255 deðerde ise;

255 olan deðerlerin hepsini tek tek kontrol et.
pixelin 8 komþusuna da bak


*/

image load_image(const char* filename)
{
	int w, h, c;
	int channel = 3;
	//w = width, h = height, c = # 8 - bit components per pixel ...
	unsigned char* data = stbi_load(filename, &w, &h, &c, channel);// without OpenCV
	
	if (!data) {
		exit(EXIT_FAILURE);
	}
	
	image out;
	out.data = data;
	out.h = h;
	out.w = w;
	out.c = c;
	return out;
}//load_image

image make_empty_image(int w, int h, int c) 
{
	int size = w * h * c;
	image out;
	out.data = new unsigned char[size];
	out.h = h;
	out.w = w;
	out.c = c;
	return out;
}
image make_image(int w, int h,int c, unsigned char* data)
{
	image out;
	out.data = data;
	out.h = h;
	out.w = w;
	out.c = c;
	return out;
}
image rgb_to_gray(int w, int h, unsigned char* data) 
{
	image out = make_empty_image(w,h,1);

	int psw,psw1, bufpos=0,bufpos1=0;
	psw = w * 3;
	psw1 = w;

	for (int row = 0; row < h; row++)
		for (int col = 0; col < w; col++) {
			bufpos = row * psw + col * 3;
			bufpos1 = row*psw1  + col ;
			out.data[bufpos1] = (data[bufpos] * 0.299 + data[bufpos + 1] * 0.587 +data[bufpos + 2] * 0.114);
		}
	return out;
}
int** Histogram(image im) {
	int** hist = new int*[im.c];
	for (int i = 0; i < im.c; i++)
	{
		hist[i] = new int[256];
	}	

	for (int i = 0; i <im.c; i++)
	{
		for (int j = 0; j < 256; j++)
		{
			hist[i][j] = 0;
		}
		
	}
	int psw = im.w*im.c;
	int bufpos;
	for (int row = 0; row < im.h; row++)
	{
		for (int col = 0; col < im.w; col++)
		{
			bufpos = row * psw + col * im.c;

			for (int i = 0; i < im.c; i++)
			{
				int k = (int)im.data[bufpos + i];
				hist[i][k]++;
			}
		}
	}
	return hist;
}
image Convolition(image im, int* mask, const int mask_w, const int mask_h, int pad) {
	int sum = 0;
	int psw = im.w;
	int bufpos;
	int bufpos_mask;
	for (int row = pad ; row < im.h - pad; row++)
	{
		for (int col = pad; col < im.w - pad; col++)
		{
			sum = 0;
			bufpos = row * psw + col;

			//Loop for Masking
			for (int mask_row = 0; mask_row < mask_h; mask_row++)
			{
				bufpos += psw * mask_row;// pointeri bir aþaðý satýra indir
				for (int mask_col = 0; mask_col < mask_w; mask_col++)
				{
					//Buffer for mask
					bufpos_mask = mask_row * mask_w + mask_col;//mask pointer'i
					sum += mask[bufpos_mask] * im.data[bufpos + mask_col];
				}	
			}
			bufpos = row * psw + col;//pointeri tekrar hesapla
			im.data[bufpos] = sum / 16;//(mask_h*mask_w); // average of neighbor pixels
		}
	}
	
	/*for (int i = 0; i < im.h*pad; i++)
	{
		im.data[i] = 0;
	}
	for (int i = (im.h)*(im.w-pad); i < im.w*im.h; i++)
	{
		im.data[i] = 0;
	} 
	bufpos = 0;
	for (int row = 0; row < im.w*im.h; row+=im.w)
	{
		int i = -pad+1;
		while (i<=pad)
		{
			im.data[row-i] = 0;
			i++;
		}	
	}*/
	// Image size after Kernel Mask
	

	return im;
}


image gray_to_binary_image(image im, int* T_values) {

	for (int i = 0; i < im.w * im.h; i++)
	{
		int distance = labs(im.data[i] - T_values[0]);
		if (distance < labs(im.data[i]- T_values[1])) im.data[i] = 0;
		else
		{
			im.data[i] = 255;
		}
	}
	return im;
}
image rgb_to_binary_image(image im, int* T_values,int T_num) {
	int psw = im.w*im.c;
	int bufpos;
	for (int row = 0; row < im.h; row++)
	{
		for (int col = 0; col < im.w; col++)
		{
			bufpos = row * psw + col * im.c;
			int* pixel = new int[im.c];
			for (int i = 0; i < im.c; i++)
			{
				pixel[i] = (int)im.data[bufpos + i];
			}

			int temp_dist = Euclidean_distance(im.c, pixel, 0, T_values, 0); // pixelin ilk T deðerine olan uzaklýðýný ilk deðer olarak ata
			int temp_index = 0;;
			int distance;
			for (int j = im.c; j <T_num*im.c; j+=im.c)
			{
				distance = Euclidean_distance(im.c, pixel, 0, T_values, j); // pixelin bütün T deðerlerine olan uzaklýðýna abak
				if (distance < temp_dist)
				{
					temp_dist = distance;
					temp_index = j; // Bu satýrda 2 den fazla T deðeri varsa o T deðerinin indisini bulmak amaçlandý
				}
			    for (int i = 0; i < im.c; i++)
				{
					im.data[bufpos + i] = T_values[temp_index+i]; // 0 yada 255 deðerini deðilde T intensity deðerini buffer a atamak
				}
			}
		}
	}
	return im;
}
int* K_means(int** hist, int T_num, int c) {
	int* T_vals = new int[T_num*c];
	int* T_vals_temp = new int[T_num*c];
	int totalpix_val = 1;
	int means = 0;

	for (int i = 0; i < T_num*c; i++)
	{
		T_vals[i] = rand() % 256;
		T_vals_temp[i] = 0;
	}
	int* T_tagged = new int[256];

	for (int i = 0; i < 256; i++)
	{
		T_tagged[i] = -1;
	}

	/*for (int i = 0; i < T_num*c; i++)
	{
		printf("T[%d]=%d\n", i, T_vals[i]);
	}*/
	while (true)
	{
		for (int i = 0; i < 256; i++)
		{
			int distance = 0;
			int temp_dist= Euclidean_distance(c, T_vals, 0, i);
			int temp_indx = 0;
			for (int j = c; j < T_num*c; j+=c)
			{
				distance = Euclidean_distance(c,T_vals,j,i); // indis deðerine olan uzaklýðý ölçtük
				if (distance < temp_dist)
				{
					temp_dist = distance;
					temp_indx = j;
				}
				T_tagged[i]=temp_indx;
			}
		}
		/*for (int i = 0; i < 256; i++)
		{
			printf("intensity:%d  Tagged T val :%d\n", i, T_tagged[i]);
		}*/
		for (int i = 0; i < T_num * c; i++)
		{
			printf("T[%d]=%d\n", i, T_vals[i]);
		}
		int counter = 0;
		int newT_dist;
		
		for (int j = 0; j < T_num * c; j += c)
		{	
			newT_dist=0;
			for (int k = 0; k < c; k++)
			{
				for (int i = 0; i < 256; i++)
				{
					if (T_tagged[i] == j) {
						totalpix_val += hist[k][i];
						means += i * hist[k][i];
					}
				}
				means = means / totalpix_val;
				T_vals_temp[j+k] = T_vals[j+k];
				T_vals[j+k] = means;
				means = 0;
				totalpix_val = 1;
			}
			newT_dist = Euclidean_distance(c,T_vals,j,T_vals_temp,j);
			if (labs(newT_dist)==0) {
				counter++;
			}
		}
		if (counter == T_num) {
			return T_vals;
		}
	}
}
int Euclidean_distance(int space_size, int*x ,int buffer_x,int *y,int buffer_y) {
	// ilk parametre uzay boyutu,
	// ikinci parametre ilk Noktanýn tutulduðu dizi, üçüncü parametre ilk noktanýn kaçýncý indiste tutulduðu deðer
	// dördüncü parametre ikinci Noktanýn tutulduðu dizi, beþinci parametre ikinci noktanýn kaçýncý indiste tutulduðu deðer
	// Tek boyutlu dizilerde çalýþýlýldýðý için 5 parametreli fonksiyon oldu
	int distance=0;
	int sum = 0;
	for (int i = 0; i < space_size; i++) // uzay boyutu kadar dizi de ilerle
	{
		sum += pow(labs(x[buffer_x+i] - y[buffer_y+i]),2);
	}
	distance = sqrt(sum);
	return distance;
}
int Euclidean_distance(int space_size, int* x, int buffer_x, int y) {
	//Noktanýn herhangi bir intensity deðerine olan uzaklýðý
	int distance = 0;
	int sum = 0;
	for (int i = 0; i < space_size; i++)
	{
		sum += pow(labs(x[buffer_x + i] - y), 2);
	}
	distance = sqrt(sum);
	return distance;
}
float Euclidean_Distance(float* arr1, float* arr2, int size)
{
	float temp = 0;
	for (int i = 0;i < size;i++)
	{
		temp += pow((arr1[i] - arr2[i]), 2);
	}
	return sqrt(temp);
}
int* Connectivity(image im) {//8 komþu deðere göre connectivity

	int* tagged_data = new int[im.w * im.h];
	int* collision = new int[im.w * im.h];
	int psw = im.w;
	for (int i = 0; i < im.w * im.h; i++)
	{
		tagged_data[i] = 0;
		collision[i] = 0;
	}
	int counter = 2;
	for (int row = 1; row < im.h; row++)
	{
		for (int col = 0; col < im.w; col++)
		{
			int bufpos = row * psw + col;
			int bufpos_rowup = (row - 1) * psw + col;
			if (im.data[bufpos] == 255) {
				if (col >= 1 && col + 1 <= im.w) // sýnýr deðerleri
				{
					int temp[4] = { 0,0,0,0 };
					temp[0] = tagged_data[bufpos_rowup - 1];
					temp[1] = tagged_data[bufpos_rowup];
					temp[2] = tagged_data[bufpos_rowup + 1];
					temp[3] = tagged_data[bufpos - 1];

					if (!(temp[0] || temp[1] || temp[2] || temp[3]))
						//komþularýndan etiket alan yoksa yeni deðer
					{
						tagged_data[bufpos] = counter; // yeni deðer
						counter++;
					}
					//else if((temp[0]==temp[1]==temp[2]==temp[3]) && temp [0] != 0)// komþularýnýn hepsi ayný deðerse 
					//{
					//	tagged_data[bufpos] = temp[0]; // tekdeðer
					//}
					else
					{
						int count = 0;
						int val = 0;
						for (int i = 0; i < 4; i++)
						{
							if (temp[i] != 0) val = temp[i];
						}
						for (int i = 0; i < 4; i++)
						{
							if ((temp[i] != 0) && (temp[i] != val) && (val != 0)) count++;//collision varsa counter i arttýr
						}
						if (count == 0) { // tek deðer alan komþu varsa o deðeri ata
							tagged_data[bufpos] = val;// tek deðer
						}
						else {
							int min_val = val;
							for (int i = 0; i < 4; i++)
							{
								if (temp[i] != 0 && min_val >= temp[i])  min_val = temp[i];
							}
							tagged_data[bufpos] = min_val; // küçük deðeri ata
							collision[bufpos] = 1;;// ikinci iterasyonda collision olan deðerler den büyük olanýn tüm deðerlerini küçük olana çevir
						}
					}
				}
			}
		}
	}
	int num = 0;
	for (int i = 0; i < im.w * im.h; i++)
	{
		if (collision[i] == 1) {
			num++;
		}
	}
	printf("counter: %d \n collision ::%d\n", counter, num);//
	num = 0;
	for (int row = im.h; row > 1; --row)
	{
		for (int col = im.w; col > 0; --col)
		{
			int bufpos = row * psw + col;
			int bufpos_rowup = (row - 1) * psw + col;
			if (collision[bufpos] == 1) {
				num++;
				if (col >= 1 && col + 1 <= im.w) // sýnýr deðerleri
				{
					int temp[4] = { 0,0,0,0 };
					temp[0] = tagged_data[bufpos_rowup - 1];
					temp[1] = tagged_data[bufpos_rowup];
					temp[2] = tagged_data[bufpos_rowup + 1];
					temp[3] = tagged_data[bufpos - 1];

					int count = 0;
					int val = 0;
					for (int i = 0; i < 4; i++)
					{
						if (temp[i] != 0 && temp[i] > val) val = temp[i];
					}
					for (int i = 0; i < 4; i++)
					{
						if ((temp[i] != 0) && (temp[i] != val) && (val != 0)) {
							count++;
						} //collision varsa counter i arttýr
					}
					if (count != 0) { // tek deðer alan komþu varsa o deðeri ata
						int min_val = val;
						for (int i = 0; i < 4; i++)
						{
							if (temp[i] != 0 && min_val >= temp[i])  min_val = temp[i];
						}
						int z = 0;
						while (z < im.w * im.h) {
							if (tagged_data[z] == val)//Eðer bütün pikseller içerisinden collisiondaki büyük deðer varsa
							{
								tagged_data[z] = min_val;// küçük deðeri ata
							}
							z++;
						}
					}
				}
			}
		}
	}
	return tagged_data;
}
Boxes* bounding_box (int*tagged_data,image im) {
	int counter=5000;//
	int psw = im.w;
	int a = 0;
	int* area_obj = new int[counter];
	for (int i = 0; i < counter; i++)
	{
		area_obj[i] = 0;
	}
	while (a < im.w * im.h)
	{
		if (tagged_data[a] != 0)
		{
			area_obj[tagged_data[a]]++;
		}
		a++;
	}
	int countobjts = 0;
	int* objects = new int[counter];
	for (int i = 0; i < counter; i++)
	{
		if (area_obj[i] > 5) {
			objects[countobjts] = i;
			countobjts++;
		}
	}
	for (int i = 0; i < counter; i++)
	{
		if (area_obj[i] > 5) {
			printf("\n Piksel Tag : %d----- Area : %d \n",i, area_obj[i]);
			if (area_obj[i] > 1000) {
				printf("bu bir makarna");
			}
		}
	}
	int flag = 0;
	int top = 0, bottom=0, left=4000, right=0;
	Boxes* Bounding = new Boxes[countobjts];
	Bounding->obj_num = countobjts;
	for (int i = 0; i < countobjts; i++)
	{
		int bufpos;
		for (int row = 0; row < im.h; row++)
		{
			for (int col = 0; col < im.w; col++)
			{
				bufpos = psw * row + col;
				if (tagged_data[bufpos] == objects[i])
				{
					if (flag == 0){//first row is top
						top = row;
						flag++;
					}
					if (col < left)left = col;
					if (col > right) right = col;
					if (row > bottom) bottom = row;
				}
			}
		}
		Bounding[i].top = top;
		Bounding[i].bottom = bottom;
		Bounding[i].right = right;
		Bounding[i].left = left;
		printf("obj no : %d, piksel tag no : %d, Top point : %d bot point : %d left point : %d right point : %d\n", i, objects[i], top, bottom, left, right);
		top = 0;
		bottom = 0;
		right = 0;
		left = 4000;
		flag = 0;
	}

	/*for (int i = 0; i < countobjts; i++)
	{
		printf("obj no : %d quantitiy : %d,, Top point : %d bot point : %d left point : %d right point : %d\n", i, objects[i], obj[i].top, obj[i].bottom, obj[i].left, obj[i].right);
		
	}*/

	/*for (int i = 0; i <countobjts; i++)
	{
		
	}*/
	return Bounding;
}
image ConnectivityColor(image im) {//8 komþu deðere göre connectivity

	int* tagged_data = new int[im.w * im.h];
	int* collision = new int[im.w * im.h];
	int psw = im.w;
	for (int i = 0; i < im.w * im.h; i++)
	{
		tagged_data[i] = 0;
		collision[i] = 0;
	}
	int counter = 2;
	for (int row = 1; row < im.h; row++)
	{
		for (int col = 0; col < im.w; col++)
		{
			int bufpos = row * psw + col;
			int bufpos_rowup = (row - 1) * psw + col;
			if (im.data[bufpos] == 255) {
				if (col >= 1 && col + 1 <= im.w) // sýnýr deðerleri
				{
					int temp[4] = { 0,0,0,0 };
					temp[0] = tagged_data[bufpos_rowup - 1];
					temp[1] = tagged_data[bufpos_rowup];
					temp[2] = tagged_data[bufpos_rowup + 1];
					temp[3] = tagged_data[bufpos - 1];

					if (!(temp[0] || temp[1] || temp[2] || temp[3]))
						//komþularýndan etiket alan yoksa yeni deðer
					{
						tagged_data[bufpos] = counter; // yeni deðer
						counter++;
					}
					//else if((temp[0]==temp[1]==temp[2]==temp[3]) && temp [0] != 0)// komþularýnýn hepsi ayný deðerse 
					//{
					//	tagged_data[bufpos] = temp[0]; // tekdeðer
					//}
					else
					{
						int count = 0;
						int val = 0;
						for (int i = 0; i < 4; i++)
						{
							if (temp[i] != 0) val = temp[i];
						}
						for (int i = 0; i < 4; i++)
						{
							if ((temp[i] != 0) && (temp[i] != val) && (val != 0)) count++;//collision varsa counter i arttýr
						}
						if (count == 0) { // tek deðer alan komþu varsa o deðeri ata
							tagged_data[bufpos] = val;// tek deðer
						}
						else {
							int min_val = val;
							for (int i = 0; i < 4; i++)
							{
								if (temp[i] != 0 && min_val >= temp[i])  min_val = temp[i];
							}
							tagged_data[bufpos] = min_val; // küçük deðeri ata
							collision[bufpos] = 1;;// ikinci iterasyonda collision olan deðerler den büyük olanýn tüm deðerlerini küçük olana çevir
						}
					}
				}
			}
		}
	}
	int num = 0;
	for (int i = 0; i < im.w * im.h; i++)
	{
		if (collision[i] == 1) {
			num++;
		}
	}
	printf("counter: %d \n collision ::%d\n", counter, num);//
	num = 0;
	for (int row = im.h; row > 1; --row)
	{
		for (int col = im.w; col > 0; --col)
		{
			int bufpos = row * psw + col;
			int bufpos_rowup = (row - 1) * psw + col;
			if (collision[bufpos] == 1) {
				num++;
				if (col >= 1 && col + 1 <= im.w) // sýnýr deðerleri
				{
					int temp[4] = { 0,0,0,0 };
					temp[0] = tagged_data[bufpos_rowup - 1];
					temp[1] = tagged_data[bufpos_rowup];
					temp[2] = tagged_data[bufpos_rowup + 1];
					temp[3] = tagged_data[bufpos - 1];

					int count = 0;
					int val = 0;
					for (int i = 0; i < 4; i++)
					{
						if (temp[i] != 0 && temp[i] > val) val = temp[i];
					}
					for (int i = 0; i < 4; i++)
					{
						if ((temp[i] != 0) && (temp[i] != val) && (val != 0)) {
							count++;
						} //collision varsa counter i arttýr
					}
					if (count != 0) { // tek deðer alan komþu varsa o deðeri ata
						int min_val = val;
						for (int i = 0; i < 4; i++)
						{
							if (temp[i] != 0 && min_val >= temp[i])  min_val = temp[i];
						}
						int z = 0;
						while (z < im.w * im.h) {
							if (tagged_data[z] == val)//Eðer bütün pikseller içerisinden collisiondaki büyük deðer varsa
							{
								tagged_data[z] = min_val;// küçük deðeri ata
							}
							z++;
						}
					}
				}
			}
		}
	}
	int a = 0;
	int* hist_obj = new int[counter];
	for (int i = 0; i < counter; i++)
	{
		hist_obj[i] = 0;
	}
	while (a < im.w * im.h)
	{
		if (tagged_data[a] != 0)
		{
			hist_obj[tagged_data[a]]++;
		}
		a++;
	}
	int countobjts = 0;
	int* objects = new int[counter];
	for (int i = 0; i < counter; i++)
	{
		if (hist_obj[i] > 5) {
			objects[countobjts] = i;
			countobjts++;
		}
	}
	/*struct objcts
	{
		int top;
		int bottom;
		int left;
		int right;
	}obj[20];*/
	int flag = 0;
	int top = 0, bottom = 0, left = 1500, right = 0;
	for (int i = 0; i < countobjts; i++)
	{
		int bufpos;
		for (int row = 0; row < im.h; row++)
		{
			for (int col = 0; col < im.w; col++)
			{
				bufpos = psw * row + col;
				if (tagged_data[bufpos] == objects[i])
				{
					if (flag == 0) {
						top = row;
						flag++;
					}
					if (col < left)left = col;
					if (col > right) right = col;
					if (row > bottom) bottom = row;
				}
			}
		}
		/*obj[i].top = top;
		obj[i].bottom = bottom;
		obj[i].right = right;
		obj[i].left = left;*/
		printf("obj no : %d quantitiy : %d,, Top point : %d bot point : %d left point : %d right point : %d\n", i+1 , objects[i], top, bottom, left, right);
		top = 0;
		bottom = 0;
		right = 0;
		left = 0;
		flag = 0;
	}

	/*for (int i = 0; i < countobjts; i++)
	{
		printf("obj no : %d quantitiy : %d,, Top point : %d bot point : %d left point : %d right point : %d\n", i, objects[i], obj[i].top, obj[i].bottom, obj[i].left, obj[i].right);

	}*/

	image im_out = make_empty_image(im.w, im.h, 3);

	int bufpos1;
	int psw_rgb = im.w * 3;
	int bufpos_gray;
	srand((unsigned)time(NULL));
	for (int i = 0; i < counter; i++)
	{
		if(hist_obj[i]>5)// Küçük nesneler için eleme
		{
			int rand_val = rand () % 256;
			for (int row = 0; row < im.h; row++)
			{
				for (int col = 0; col < im.w; col++)
				{
					bufpos1 = row * psw_rgb + col * im_out.c;
					bufpos_gray = row * psw + col ;

					if (tagged_data[bufpos_gray] == i)
					{
						im_out.data[bufpos1] = rand_val;
						im_out.data[bufpos1 + 1] = rand_val;
						im_out.data[bufpos1 + 2] = rand_val;;
					}
				}
			}
		}
	}
	return im_out;
}


image FeatureExtraction(Boxes* Bounding,int* tagged_data, int box_num, image im) {


	float momentvalue[7];
	float mercimek[7] = { 0.165087,0.00170739,0.00708642,0.000142945,2.59475e-07,-0.00802825,1.58015e-07 };
	float cekirdek[7] = { 0.234149,0.0277601,1.0764,0.237477,0.105781,-0.196802,-0.0101849 };
	float eriste[7] = { 1.00171,0.997429,22.9338,19.5061,761.183,19.5376,-11.6029 };
	float fasulye[7] = { 0.190439,0.0107762,0.0305301,0.00227828,3.78958e-05,0.00298993,2.54525e-05 };
	float mýsýr[7] = { 0.164747,0.000834593,0.0931484,0.00139435,2.86284e-06,-0.0279052,5.64801e-06 };
	float nohut[7] = { 0.166182,0.00138244,0.156274,0.0031489,0.000338772,-0.0143759,-0.000127367 };
	float para[7] = { 0.16244,0.00104876,0.0017776,1.98324e-05,-3.45616e-09,-0.00703107,9.6867e-10 };


	int psw = im.w;
	image im_out = make_empty_image(im.w, im.h, 3);
	for (int i = 0; i < im.w * im.h * 3; i++)
	{
		im_out.data[i] = 255;
	}
	for (int i = 0; i < box_num; i++)
	{
		int* obj_data = new int[(Bounding[i].bottom - Bounding[i].top + 1) * (Bounding[i].right - Bounding[i].left + 1)];
		int h = 0;
		for (int row = Bounding[i].top; row < Bounding[i].bottom; row++)
		{
			for (int col = Bounding[i].left; col < Bounding[i].right; col++)
			{
				obj_data[h] = tagged_data[psw * row + col];
				h++;
			}
		}
		HuMoments obje(obj_data, (Bounding[i].bottom - Bounding[i].top + 1), (Bounding[i].right - Bounding[i].left + 1));
		obje.calcOrgins();
		obje.calcInvariantMoments();

		float* moments = obje.getInvariantMoments();

		//nesnenin moment deðeri ile diðer momenti belli olan nesnelerin momentleri oklid'e göre tek tek kýyaslanýr bir diziye atýlýr 
		momentvalue[0] = Euclidean_Distance(moments, mercimek, 7);
		momentvalue[1] = Euclidean_Distance(moments, eriste, 7);
		momentvalue[2] = Euclidean_Distance(moments, nohut, 7);
		momentvalue[3] = Euclidean_Distance(moments, fasulye, 7);
		momentvalue[4] = Euclidean_Distance(moments, cekirdek, 7);
		momentvalue[5] = Euclidean_Distance(moments, mýsýr, 7);
		momentvalue[6] = Euclidean_Distance(moments, para, 7);
		
		//daha önceden bilinen hangi nesneye daha yakýnsa o nesneyi yakýn olan olarak tanimlariz
		float byComparison = 10000000;
		int object = 0;
		for (int i = 0; i < 7; i++)
		{
			if (momentvalue[i] < byComparison)
			{
				byComparison = momentvalue[i];
				object = i;		//7 nesneden hangisine daha yakinsa ona setlenir
			}
		}
		int r, g, b;
		if (object == 0) {
			r = 0;g = 0;b = 255;
		}
		else if (object == 1)
		{
			r = 255;g = 150;b = 255;
		}
		else if (object == 2)
		{
			r = 0;g = 255;b = 0;
		}
		else if (object == 3)
		{
			r = 255;g = 0;b = 0;
		}
		else if (object == 4)
		{
			r = 255;g = 150;b = 0;
		}
		else if (object == 5)
		{
			r = 0;g = 150;b = 255;
		}
		else if (object == 6)
		{
			r = 0;g = 255;b = 100;
		}
		
		int bufpos = 0;
		for (int row = Bounding[i].top; row < Bounding[i].bottom; row++)
		{
			for (int col = Bounding[i].left; col < Bounding[i].right; col++)
			{
				bufpos = row * psw * 3 + col * 3;
				if (tagged_data[psw * row + col] != 0) {
					im_out.data[bufpos] = r;
					im_out.data[bufpos + 1] = g;
					im_out.data[bufpos + 2] = b;
				}
			}
		}
	}
	return im_out;
}


//int* K_means(int* hist, int T_num) { //istenilen kadar T deðeri bulan k-meansfonksiyonu
//
//	int* T_vals = new int[T_num];
//	int* T_vals_temp = new int[T_num];
//	int totalpix_val = 1;
//	int means = 0;
//
//	for (int i = 0; i < T_num; i++)
//	{
//		T_vals[i] = rand() % 256;
//		T_vals_temp[i] = 0;
//	}
//	int* T_tagged = new int [256];
//
//	for (int i = 0; i <256; i++)
//	{
//		T_tagged[i] = -1;
//	}
//
//	while (true)
//	{	
//		for (int i = 0; i < 256; i++)
//		{	
//			int temp_dist= labs(i - T_vals[0]);
//			int temp_indx=0;
//			for (int j = 1; j < T_num; j++)
//			{
//				if (labs(i - T_vals[j]) < temp_dist)
//				{
//					temp_dist = labs(i - T_vals[j]);
//					temp_indx = j;
//				}	
//				T_tagged[i] = temp_indx;
//			}
//		}
//		int k = 0;
//		for (int j = 0; j < T_num; j++)
//		{
//			for (int i = 0; i < 256; i++)
//			{
//				if (T_tagged[i] == j) {
//					totalpix_val += hist[i];
//					means += i * hist[i];
//				}
//			}
//			means = means / totalpix_val;
//			T_vals_temp[j] = T_vals[j];
//			T_vals[j] = means;
//			means = 0;
//			totalpix_val = 1;
//			if (labs(T_vals[j] - T_vals_temp[j]) == 0) {
//				k++;
//			}
//		}
//		if (k == T_num ) {
//			return T_vals;
//		}
//	}
//}
//int K_means(int* hist) { // ilk olarak yaptýðým iki T deðeri bulan k means
//	// Initial values
//	int* T_vals = new int[2];
//	int T1, T2;
//	int Temp_T1 =0, Temp_T2=0;
//	int totalpix_val = 1;
//	int means = 0;
//
//	T1 = rand() % 256;
//	T2 = rand() % 256;
//
//	int* T1_tagged = new int[256];
//	int* T2_tagged = new int[256];
//
//	for (int i = 0; i < 256; i++)
//	{
//		T1_tagged[i] = 0;
//		T2_tagged[i] = 0;
//	}
//
//	while (true)
//	{
//		for (int i = 0; i < 256; i++)
//		{
//			if (labs(i - T1) < labs(i - T2))
//			{
//				T1_tagged[i] = 1;
//				T2_tagged[i] = 0;
//			}
//			else
//			{
//				T2_tagged[i] = 1;
//				T1_tagged[i] = 0;
//			}
//		}
//
//		for (int i = 0; i < 256; i++)
//		{
//			if (T1_tagged[i] == 1) {
//				totalpix_val += hist[i];
//				means += i * hist[i];
//			}
//		}
//		means = means / totalpix_val;
//		Temp_T1 = T1;
//		T1 = means;
//		means = 0;
//		totalpix_val = 1;
//
//		for (int i = 0; i < 256; i++)
//		{
//			if (T2_tagged[i] == 1) {
//				totalpix_val += hist[i];
//				means += i * hist[i];
//			}
//		}
//		means = means / totalpix_val;
//		Temp_T2 = T2;
//		T2 = means;
//		means = 0;
//		totalpix_val = 1;
//
//		if (labs(T1 - Temp_T1) == 0 && (labs(T2 - Temp_T2)) == 0) {
//			
//			int T=0;
//			T = labs(T1 - T2) / 2;
//			if (T1 < T2) T += T1;
//			else T += T2;
//
//			return T;
//		}
//	}
//}
//int* Histogram(image im) { // ilk yaptýðým gri görüntünün histogramýný çýkaran fonksiyon. tek boyutlu histogram çýkarýr
//	int* hist;
//	hist = new int[256];
//
//	for (int i = 0; i < 256; i++)
//	{
//		hist[i] = 0;
//	}
//
//	int length = im.w * im.h;
//	int i = 0;
//
//	while (i < length) {
//		int h = (int)im.data[i];
//		hist[h] ++;
//		i++;
//	}
//	return hist;
//}