#pragma once
#include <atlstr.h>
#include <iostream>
#include "image.h"


namespace read_image {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for Form1
	/// </summary>
	/*public class objcts
	{
		int top;
		int bottom;
		int left;
		int right;
	}obj[20];*/
	public ref class Form1 : public System::Windows::Forms::Form
	{
	public:
		Form1(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}
		int StartPointx;
		int StartPointY;
		int EndPointX;
		int EndPointY;
	private: System::Windows::Forms::PictureBox^ pictureBox2;
	private: System::Windows::Forms::ToolStripMenuItem^ binary›mageToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^ objectDetectionToolStripMenuItem;
	public:

	private: System::Windows::Forms::ToolStripMenuItem^ grayScaleToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^ gaussianBlurToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^ objectDetection2ToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^ rgbToBinaryToolStripMenuItem;
	public:

	private: System::Windows::Forms::Button^ button1;
	public:

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~Form1()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::MenuStrip^ menuStrip1;
	protected:
	private: System::Windows::Forms::ToolStripMenuItem^ fileToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^ openToolStripMenuItem;
	private: System::Windows::Forms::PictureBox^ pictureBox1;
	private: System::Windows::Forms::OpenFileDialog^ openFileDialog1;

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container^ components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void ShowRGBImages(image im) {
			pictureBox1->Width = im.w;
			pictureBox1->Height = im.h;
			pictureBox1->Refresh();
			Bitmap^ surface = gcnew Bitmap(im.w, im.h);
			pictureBox1->Image = surface;
			Color c;
			int psw, bufpos;
			psw = im.w * 3;
			for (int row = 0; row < im.h; row++)
				for (int col = 0; col < im.w; col++) {
					bufpos = row * psw + col * im.c;
					c = Color::FromArgb(im.data[bufpos], im.data[bufpos + 1], im.data[bufpos + 2]);
					surface->SetPixel(col, row, c);
				}
		}//ShowImages
		void ShowGrayImages(image im) {
			pictureBox1->Width = im.w;
			pictureBox1->Height = im.h;
			pictureBox1->Refresh();
			Bitmap^ surface = gcnew Bitmap(im.w, im.h);
			pictureBox1->Image = surface;
			Color c;

			int bufpos, psw1;
			psw1 = im.w;
			for (int row = 0; row < im.h; row++)
				for (int col = 0; col < im.w; col++) {
					bufpos = row * psw1 + col;
					c = Color::FromArgb(im.data[bufpos], im.data[bufpos], im.data[bufpos]);
					surface->SetPixel(col, row, c);
				}
		}
		//void drawB_box(Boxes* tagged_data, image im) {
		//	pictureBox1->Refresh();
		//	Drawing::Graphics^ g;
		//	g = pictureBox1->CreateGraphics();
		//	Pen^ redPen = gcnew Pen(Color::Red);
		//	int counter = 1500;
		//	int psw = im.w;
		//	int a = 0;
		//	int* hist_obj = new int[counter];
		//	for (int i = 0; i < counter; i++)
		//	{
		//		hist_obj[i] = 0;
		//	}
		//	while (a < im.w * im.h)
		//	{
		//		if (tagged_data[a] != 0)
		//		{
		//			hist_obj[tagged_data[a]]++;
		//		}
		//		a++;
		//	}
		//	int countobjts = 0;
		//	int* objects = new int[counter];
		//	for (int i = 0; i < counter; i++) {
		//		if (hist_obj[i] > 5) {
		//			objects[countobjts] = i;
		//			countobjts++;
		//		}
		//	}
		//	int flag = 0;
		//	int top = 0, bottom = 0, left = 5000, right = 0;
		//	for (int i = 0; i < countobjts; i++) {
		//		int bufpos;
		//		for (int row = 0; row < im.h; row++) {
		//			for (int col = 0; col < im.w; col++)
		//			{
		//				bufpos = psw * row + col;
		//				if (tagged_data[bufpos] == objects[i])
		//				{
		//					if (flag == 0) {// first row is top
		//						top = row;
		//						flag++;
		//					}
		//					if (col < left)left = col;
		//					if (col > right) right = col;
		//					if (row > bottom) bottom = row;
		//				}
		//			}
		//		}
		//		Drawing::Rectangle rect(left, top, right - left, bottom - top);
		//		g->DrawRectangle(redPen, rect);
		//		top = 0;
		//		bottom = 0;
		//		right = 0;
		//		left = 1500;
		//		flag = 0;
		//	}
		//}
		void drawB_box(Boxes* Bounding,int box_num) {
			pictureBox1->Refresh();
			Drawing::Graphics^ g;
			g = pictureBox1->CreateGraphics();
			Pen^ redPen = gcnew Pen(Color::Red);
			for (int i = 0; i < box_num; i++)
			{
				Drawing::Rectangle rect(Bounding[i].left, Bounding[i].top, Bounding[i].right - Bounding[i].left, Bounding[i].bottom - Bounding[i].top);
				g->DrawRectangle(redPen, rect);
			} 		
		}
		void InitializeComponent(void)
		{
			this->menuStrip1 = (gcnew System::Windows::Forms::MenuStrip());
			this->fileToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->openToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->grayScaleToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->binary›mageToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->objectDetectionToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->gaussianBlurToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->objectDetection2ToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->pictureBox1 = (gcnew System::Windows::Forms::PictureBox());
			this->openFileDialog1 = (gcnew System::Windows::Forms::OpenFileDialog());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->pictureBox2 = (gcnew System::Windows::Forms::PictureBox());
			this->rgbToBinaryToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->menuStrip1->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox2))->BeginInit();
			this->SuspendLayout();
			// 
			// menuStrip1
			// 
			this->menuStrip1->ImageScalingSize = System::Drawing::Size(20, 20);
			this->menuStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) { this->fileToolStripMenuItem });
			this->menuStrip1->Location = System::Drawing::Point(0, 0);
			this->menuStrip1->Name = L"menuStrip1";
			this->menuStrip1->Size = System::Drawing::Size(1762, 28);
			this->menuStrip1->TabIndex = 0;
			this->menuStrip1->Text = L"menuStrip1";
			// 
			// fileToolStripMenuItem
			// 
			this->fileToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(7) {
				this->openToolStripMenuItem,
					this->grayScaleToolStripMenuItem, this->binary›mageToolStripMenuItem, this->objectDetectionToolStripMenuItem, this->gaussianBlurToolStripMenuItem,
					this->objectDetection2ToolStripMenuItem, this->rgbToBinaryToolStripMenuItem
			});
			this->fileToolStripMenuItem->Name = L"fileToolStripMenuItem";
			this->fileToolStripMenuItem->Size = System::Drawing::Size(46, 24);
			this->fileToolStripMenuItem->Text = L"File";
			// 
			// openToolStripMenuItem
			// 
			this->openToolStripMenuItem->Name = L"openToolStripMenuItem";
			this->openToolStripMenuItem->Size = System::Drawing::Size(224, 26);
			this->openToolStripMenuItem->Text = L"Open";
			this->openToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::openToolStripMenuItem_Click);
			// 
			// grayScaleToolStripMenuItem
			// 
			this->grayScaleToolStripMenuItem->Name = L"grayScaleToolStripMenuItem";
			this->grayScaleToolStripMenuItem->Size = System::Drawing::Size(224, 26);
			this->grayScaleToolStripMenuItem->Text = L"GrayScale";
			this->grayScaleToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::grayScaleToolStripMenuItem_Click);
			// 
			// binary›mageToolStripMenuItem
			// 
			this->binary›mageToolStripMenuItem->Name = L"binary›mageToolStripMenuItem";
			this->binary›mageToolStripMenuItem->Size = System::Drawing::Size(224, 26);
			this->binary›mageToolStripMenuItem->Text = L"Binary ›mage";
			this->binary›mageToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::binary›mageToolStripMenuItem_Click);
			// 
			// objectDetectionToolStripMenuItem
			// 
			this->objectDetectionToolStripMenuItem->Name = L"objectDetectionToolStripMenuItem";
			this->objectDetectionToolStripMenuItem->Size = System::Drawing::Size(224, 26);
			this->objectDetectionToolStripMenuItem->Text = L"Object Detection";
			this->objectDetectionToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::objectDetectionToolStripMenuItem_Click);
			// 
			// gaussianBlurToolStripMenuItem
			// 
			this->gaussianBlurToolStripMenuItem->Name = L"gaussianBlurToolStripMenuItem";
			this->gaussianBlurToolStripMenuItem->Size = System::Drawing::Size(224, 26);
			this->gaussianBlurToolStripMenuItem->Text = L"Gaussian Blur";
			this->gaussianBlurToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::gaussianBlurToolStripMenuItem_Click);
			// 
			// objectDetection2ToolStripMenuItem
			// 
			this->objectDetection2ToolStripMenuItem->Name = L"objectDetection2ToolStripMenuItem";
			this->objectDetection2ToolStripMenuItem->Size = System::Drawing::Size(224, 26);
			this->objectDetection2ToolStripMenuItem->Text = L"Object Detection2";
			this->objectDetection2ToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::objectDetection2ToolStripMenuItem_Click);
			// 
			// pictureBox1
			// 
			this->pictureBox1->Location = System::Drawing::Point(17, 36);
			this->pictureBox1->Margin = System::Windows::Forms::Padding(4);
			this->pictureBox1->Name = L"pictureBox1";
			this->pictureBox1->Size = System::Drawing::Size(811, 609);
			this->pictureBox1->TabIndex = 1;
			this->pictureBox1->TabStop = false;
			this->pictureBox1->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::pictureBox1_MouseDown);
			this->pictureBox1->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::pictureBox1_MouseUp);
			// 
			// openFileDialog1
			// 
			this->openFileDialog1->FileName = L"openFileDialog1";
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(835, 36);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(54, 34);
			this->button1->TabIndex = 2;
			this->button1->Text = L"Draw";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &Form1::button1_Click);
			// 
			// pictureBox2
			// 
			this->pictureBox2->Location = System::Drawing::Point(896, 36);
			this->pictureBox2->Margin = System::Windows::Forms::Padding(4);
			this->pictureBox2->Name = L"pictureBox2";
			this->pictureBox2->Size = System::Drawing::Size(811, 609);
			this->pictureBox2->TabIndex = 3;
			this->pictureBox2->TabStop = false;
			// 
			// rgbToBinaryToolStripMenuItem
			// 
			this->rgbToBinaryToolStripMenuItem->Name = L"rgbToBinaryToolStripMenuItem";
			this->rgbToBinaryToolStripMenuItem->Size = System::Drawing::Size(224, 26);
			this->rgbToBinaryToolStripMenuItem->Text = L"Rgb to binary";
			this->rgbToBinaryToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::rgbToBinaryToolStripMenuItem_Click);
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(1762, 735);
			this->Controls->Add(this->pictureBox2);
			this->Controls->Add(this->button1);
			this->Controls->Add(this->pictureBox1);
			this->Controls->Add(this->menuStrip1);
			this->MainMenuStrip = this->menuStrip1;
			this->Margin = System::Windows::Forms::Padding(4);
			this->Name = L"Form1";
			this->Text = L"Form1";
			this->menuStrip1->ResumeLayout(false);
			this->menuStrip1->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox2))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

	private: System::Void openToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e) {
		CString str; 
		
		if (openFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
			//pictureBox1->ImageLocation = openFileDialog1->FileName;
			str = openFileDialog1->FileName;
			CStringA s2(str);
			const char* input = s2;
			image im = load_image(input);

			int** im_hist = Histogram(im);
			int* T_val ;
			T_val = K_means(im_hist, 5, im.c);
			im = rgb_to_binary_image(im, T_val,5);
			ShowRGBImages(im);
			std::cout <<"w: " << im.w<<"\n";
			std::cout <<"c: " << im.c << "\n";
			std::cout << "data " << (int)im.data[10];
		}//		
	}//openTool
private: System::Void pictureBox1_MouseDown(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e) {
	StartPointx = e->X;
	StartPointY = e->Y;
	printf("dwn x: %d\n", StartPointx);
	printf("dwn y:%d\n", StartPointY);
}
private: System::Void pictureBox1_MouseUp(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e) {
	EndPointX = e->X;
	EndPointY = e->Y;
	printf("up x: %d\n", EndPointX);
	printf("up y: %d\n", EndPointY);
	
}
	   Drawing::Graphics^ g2;
private: System::Void button1_Click(System::Object^ sender, System::EventArgs^ e) {
	g2 = pictureBox1->CreateGraphics();
	Pen^ redPen = gcnew Pen(Color::Red);
	Drawing ::Rectangle rect(StartPointx, StartPointY,EndPointX-StartPointx, EndPointY-StartPointY);
	g2->DrawRectangle(redPen, rect);
}
public: System::Void grayScaleToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e) {
	CString str;

	if (openFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
		//pictureBox1->ImageLocation = openFileDialog1->FileName;
		str = openFileDialog1->FileName;
		CStringA s2(str);
		const char* input = s2;
		image im = load_image(input);
		im = rgb_to_gray(im.w, im.h, im.data);
		ShowGrayImages(im);
	}//Gray Scale 
}
private: System::Void binary›mageToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e) {
	CString str;

	if (openFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
		//pictureBox1->ImageLocation = openFileDialog1->FileName;
		str = openFileDialog1->FileName;
		CStringA s2(str);
		const char* input = s2;
		image im = load_image(input);
		im = rgb_to_gray(im.w, im.h, im.data);
		int** im_hist = Histogram(im);
		int* T_values;
		T_values = K_means(im_hist, 2,1);
		im = gray_to_binary_image(im, T_values);
		ShowGrayImages(im);
	}//Binary Image
}

private: System::Void objectDetectionToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e) {
	CString str;
	if (openFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
		//pictureBox1->ImageLocation = openFileDialog1->FileName;
		str = openFileDialog1->FileName;
		CStringA s2(str);
		const char* input = s2;
		image im = load_image(input);
		im = rgb_to_gray(im.w, im.h, im.data);
		int** im_hist = Histogram(im);
		int* T_values;
		int T_number = 2;
		T_values = K_means(im_hist, T_number, im.c);
		im = gray_to_binary_image(im, T_values);
		int* tagged_data = Connectivity(im);
		Boxes* bounding = bounding_box(tagged_data,im);
		//ShowGrayImages(im);
		
		image im3 = make_empty_image(im.w, im.c, 3);
		im3 = FeatureExtraction(bounding,tagged_data,bounding->obj_num, im);
		ShowRGBImages(im3);
		drawB_box(bounding,bounding->obj_num);

	}// Object Detection Bounding box
}
private: System::Void gaussianBlurToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e) {
	CString str;

	if (openFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
		//pictureBox1->ImageLocation = openFileDialog1->FileName;
		str = openFileDialog1->FileName;
		CStringA s2(str);
		const char* input = s2;
		image im = load_image(input);
		im = rgb_to_gray(im.w, im.h, im.data);
		const int kernel_w = 3;
		const int kernel_h = 3;
		int padding = 7;
		int* mask;
		//Mask Array 1D
		mask = new int[kernel_w * kernel_h];
		mask[0] = 1; mask[1] = 2; mask[2] = 1;
		mask[3] = 2; mask[4] = 4; mask[5] = 2;
		mask[6] = 1; mask[7] = 2; mask[8] = 1;
		im = Convolition(im, mask, kernel_w, kernel_h, padding);
		ShowGrayImages(im);
		/*mask[0] = 1; mask[1] = 4; mask[2] = 7; mask[3] = 4; mask[4] = 1;
		mask[5] = 4; mask[6] = 16; mask[7] = 26; mask[8] = 16; mask[9] = 4;
		mask[10] = 7; mask[11] = 26; mask[12] = 41; mask[13] = 26; mask[14] = 7;
		mask[15] = 4; mask[16] = 16; mask[17] = 26; mask[18] = 16; mask[19] = 4;
		mask[20] = 1; mask[21] = 4; mask[22] = 7; mask[23] = 4; mask[24] = 1;*/
	}

}//Gaussian Blur with convoliton
private: System::Void objectDetection2ToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e) {
	CString str;
	if (openFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
		//pictureBox1->ImageLocation = openFileDialog1->FileName;
		str = openFileDialog1->FileName;
		CStringA s2(str);
		const char* input = s2;
		image im = load_image(input);
		im = rgb_to_gray(im.w, im.h, im.data);
		int** im_hist = Histogram(im);
		int* T_values;
		int T_number = 2;
		T_values = K_means(im_hist, T_number, im.c);
		im = gray_to_binary_image(im, T_values);
		im = ConnectivityColor(im);
		ShowRGBImages(im);
	}
}//Random Colored Objects 
private: System::Void rgbToBinaryToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e) {
	CString str;

	if (openFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
		//pictureBox1->ImageLocation = openFileDialog1->FileName;
		str = openFileDialog1->FileName;
		CStringA s2(str);
		const char* input = s2;
		image im = load_image(input);

		int** im_hist = Histogram(im);
		int* T_values;
		int T_number = 4;
		T_values = K_means(im_hist, T_number, im.c);
		im = rgb_to_binary_image(im, T_values, T_number);
		ShowRGBImages(im);
	}
}//RGB Images to Binary images With Variable T values
};
}
