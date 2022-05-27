#pragma once

#include <string.h>

// image.h
typedef struct image {
	int w;
	int h;
	int c;
	unsigned char* data;
} image;

typedef struct Boxes {
	int top;
	int bottom;
	int left;
	int right;
	int obj_num;
}Boxes;

image load_image(const char* filename);
image make_image(int w, int h, int c ,unsigned char*data);
image make_empty_image(int w, int h, int c);
image rgb_to_gray(int w, int h, unsigned char* data);
image gray_to_binary_image(image im, int* T_values);
image rgb_to_binary_image(image im, int* T_values, int T_num);
int* Connectivity(image im);
Boxes* bounding_box(int* tagged_data, image im);
image ConnectivityColor(image im);
image FeatureExtraction(Boxes* bounding,int* tagged_data ,int box_num,image im);

int** Histogram(image im );
int* K_means(int** hist,int T_num,int c);
image Convolition(image im, int* mask, const int mask_w, const int mask_h,int pad);
int Euclidean_distance(int space_size, int* x,int buffer_x, int* y,int buffer_y);
int Euclidean_distance(int space_size, int* x, int buffer_x, int y);
float Euclidean_Distance(float* arr1, float* arr2, int size);
//int* K_means(int* hist,int T_num);
//int K_means(int* hist);
//int* Histogram(image im, int c);



