#pragma once
//#include "functions.h"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <opencv2\opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <windows.h>
//#include <>
//#include <opencv2\core\core.hpp>

//using namespace cv;

namespace Open_CV___test__02 {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Runtime::InteropServices;
	//using namespace System::String;
	using namespace std;
	using namespace cv;



	

	Mat src;
	Mat srcOriginal;



	/// <summary>
	/// Summary for MyForm
	/// </summary>
	public ref class MyForm : public System::Windows::Forms::Form
	{
	public:
		MyForm(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//

		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~MyForm()
		{
			if (components)
			{
				delete components;
			}
		}




		uchar get_max(int a, int b, int c) {
			return  max(a, max(b, c));
		}

		uchar get_min(int a, int b, int c) {
			return min(a, min(b, c));
		}





		Mat toGrayScale(Mat image)
		{
			uchar lightness;

			for (int i = 0; i < image.rows; ++i)
				for (int j = 0; j < image.cols; ++j) {
					Vec3b intensity = image.at<Vec3b>(i, j);
					uchar blue = intensity.val[0];
					uchar green = intensity.val[1];
					uchar red = intensity.val[2];

					lightness = (get_max(blue, green, red) + get_min(blue, green, red)) / 2;

					image.at<Vec3b>(i, j) = Vec3b(lightness, lightness, lightness);
				}


			return image;
		}



		double uniform(double sigma) {
			//sigma = 10;
			return sigma * sqrt(3) * ((2 * (rand() % 100) / 100) - 1);
		}


		Mat addNoize(Mat src, double sigma, int percent)
		{
			for (int i = 0; i < src.rows; ++i)
				for (int j = 0; j < src.cols; ++j) {
					if (rand() % 100 <= percent)
						src.at<Vec3b>(i, j) += Vec3b(uniform(sigma), uniform(sigma), uniform(sigma));
				}
			return src;
		}



		float Similarity(Mat img1, Mat img2)
		{

			float val1 = 0.0;

			for (int i = 0; i < img1.rows; ++i)
				for (int j = 0; j < img1.cols; ++j)
				{
					val1 += pow(img1.at<uchar>(i, j) - img2.at<uchar>(i, j), 2);
				}

			return 1.0 - sqrt(val1) / (img1.rows *img1.cols);
		}

		float Single_Lightness(Mat img, int i, int j) {
			Vec3b intensity = img.at<Vec3b>(i, j);
			uchar blue = intensity.val[0];
			uchar green = intensity.val[1];
			uchar red = intensity.val[2];
			double lightness = 0.2126 * red + 0.7152 * green + 0.0722 * blue;
			//float intensity = (float)img.at<uchar>(i, j); 
		return lightness;
		}

		float Lightness(Mat img) {
			float lightness = 0;
			for (int i = 0; i < img.cols; ++i)
				for (int j = 0; j < img.rows; ++j) {
					lightness += Single_Lightness(img, i, j);
				}
			return lightness / (img.rows * img.cols);
		}

		float Contrast(Mat img) {
			float contrast = 0, lightness = 0;
			float L = Lightness(img);
			for (int i = 0; i < img.cols; ++i)
				for (int j = 0; j < img.rows; ++j) {
					contrast += ((Single_Lightness(img, i, j) - L)*(Single_Lightness(img, i, j) - L));
				}
			return contrast / (img.rows * img.cols);
		}
	
		float Struct(Mat img1, Mat img2) {
			float Struct = 0;
			float L1 = Lightness(img1), L2 = Lightness(img2);
			for (int i = 0; i < img1.cols; ++i)
				for (int j = 0; j < img1.rows; ++j) {
					Struct += ((Single_Lightness(img1, i, j) - L1) * (Single_Lightness(img2, i, j) - L2));
				}
			return Struct / (img1.rows * img1.cols);
		}


		float SSIM_Similarity(Mat img1, Mat img2) {
			float K = 0.01;
			float L1 = Lightness(img1), L2 = Lightness(img2);
			float lightness = (2 * L1 * L2 + K) / (L1 * L1 + L2 * L2 + K);

			float C1 = Contrast(img1), C2 = Contrast(img2);
			float contrast = (2 * sqrt(C1) * sqrt(C2) + K) / (C1 + C2 + K);

			float S = Struct(img1, img2);
			float strct = (S + K) / (sqrt(C1) * sqrt(C2) + K);

			float SSIM = lightness * contrast * strct;

			return SSIM;

		}


		Mat mid_filtration(Mat src, int sigma)
		{
		

			int WinSize = sigma;
			Vec3b res_color;
			int aveR, aveG, aveB;
			double sumR, sumG, sumB;
			int col, row, n, m;

			Mat n_mat = src;

			for (col = 0; col < src.cols; col++)
				for (row = 0; row < src.rows; row++)
				{
					sumB = 0;
					sumG = 0;
					sumR = 0;
					int count = 0;
					for (m = col - ((WinSize - 1) / 2); m <= (col + (WinSize - 1) / 2); m++) {
						if (m < 0) {
							//m = 0;
							continue;
						}
						if (m > src.cols - 1) {
							//m = image.cols - 1;
							continue;
						}
						for (n = row - ((WinSize - 1) / 2); n <= row + ((WinSize - 1) / 2); n++)
						{
							if (n < 0) {
								//n = 0;
								continue;
							}
							if (n > src.rows - 1) {
								continue;
								//n = image.rows - 1;
							}
							count++;
							res_color = src.at<Vec3b>(m, n);
							Vec3b color = src.at<Vec3b>(m, n);
							uchar blue = color.val[0];
							uchar green = color.val[1];
							uchar red = color.val[2];
							sumR += red;
							sumG += green;
							sumB += blue;
						}
					}
					aveR = (int)(sumR / count);
					aveG = (int)(sumG / count);
					aveB = (int)(sumB / count);
					n_mat.at<Vec3b>(col, row) = Vec3b(aveB, aveG, aveR);
				}

			return n_mat;
		
		}




		Bitmap^ toBitMapPoint(Mat src)
		{
			// ------- Mat TO BitMap^ func
			Bitmap^ bit = gcnew Bitmap(src.rows, src.cols);

			for (int i = 0; i < src.cols; i++)
				for (int j = 0; j < src.rows; j++)
				{
					Vec3b color = src.at<Vec3b>(i, j);
					bit->SetPixel(j, i, Color().FromArgb(color.val[2], color.val[1], color.val[0]));
				}
			return bit;
		}







	private: System::Windows::Forms::PictureBox^  pictureBox1;
	private: System::Windows::Forms::Button^  button1;
	private: System::Windows::Forms::PictureBox^  pictureBox2;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::Button^  button2;
	private: System::Windows::Forms::Button^  button3;
	private: System::Windows::Forms::Button^  button4;
	private: System::Windows::Forms::Button^  button5;
private: System::Windows::Forms::Button^  button6;
private: System::Windows::Forms::Label^  label2;
private: System::Windows::Forms::Button^  button7;


	protected:

		





	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->pictureBox1 = (gcnew System::Windows::Forms::PictureBox());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->pictureBox2 = (gcnew System::Windows::Forms::PictureBox());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->button2 = (gcnew System::Windows::Forms::Button());
			this->button3 = (gcnew System::Windows::Forms::Button());
			this->button4 = (gcnew System::Windows::Forms::Button());
			this->button5 = (gcnew System::Windows::Forms::Button());
			this->button6 = (gcnew System::Windows::Forms::Button());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->button7 = (gcnew System::Windows::Forms::Button());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox2))->BeginInit();
			this->SuspendLayout();
			// 
			// pictureBox1
			// 
			this->pictureBox1->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->pictureBox1->Location = System::Drawing::Point(18, 18);
			this->pictureBox1->Margin = System::Windows::Forms::Padding(4, 5, 4, 5);
			this->pictureBox1->Name = L"pictureBox1";
			this->pictureBox1->Size = System::Drawing::Size(720, 753);
			this->pictureBox1->SizeMode = System::Windows::Forms::PictureBoxSizeMode::Zoom;
			this->pictureBox1->TabIndex = 0;
			this->pictureBox1->TabStop = false;
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(957, 737);
			this->button1->Margin = System::Windows::Forms::Padding(4, 5, 4, 5);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(112, 35);
			this->button1->TabIndex = 1;
			this->button1->Text = L"Browse...";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &MyForm::button1_Click);
			// 
			// pictureBox2
			// 
			this->pictureBox2->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->pictureBox2->Location = System::Drawing::Point(748, 18);
			this->pictureBox2->Margin = System::Windows::Forms::Padding(4, 5, 4, 5);
			this->pictureBox2->Name = L"pictureBox2";
			this->pictureBox2->Size = System::Drawing::Size(320, 319);
			this->pictureBox2->SizeMode = System::Windows::Forms::PictureBoxSizeMode::Zoom;
			this->pictureBox2->TabIndex = 2;
			this->pictureBox2->TabStop = false;
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(854, 343);
			this->label1->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(111, 20);
			this->label1->TabIndex = 3;
			this->label1->Text = L"Original Image";
			// 
			// button2
			// 
			this->button2->Location = System::Drawing::Point(748, 540);
			this->button2->Margin = System::Windows::Forms::Padding(4, 5, 4, 5);
			this->button2->Name = L"button2";
			this->button2->Size = System::Drawing::Size(188, 35);
			this->button2->TabIndex = 4;
			this->button2->Text = L"To Gray Scale";
			this->button2->UseVisualStyleBackColor = true;
			this->button2->Click += gcnew System::EventHandler(this, &MyForm::button2_Click);
			// 
			// button3
			// 
			this->button3->Location = System::Drawing::Point(748, 629);
			this->button3->Margin = System::Windows::Forms::Padding(4, 5, 4, 5);
			this->button3->Name = L"button3";
			this->button3->Size = System::Drawing::Size(188, 35);
			this->button3->TabIndex = 5;
			this->button3->Text = L"Filtration";
			this->button3->UseVisualStyleBackColor = true;
			this->button3->Click += gcnew System::EventHandler(this, &MyForm::button3_Click);
			// 
			// button4
			// 
			this->button4->Location = System::Drawing::Point(748, 443);
			this->button4->Margin = System::Windows::Forms::Padding(4, 5, 4, 5);
			this->button4->Name = L"button4";
			this->button4->Size = System::Drawing::Size(188, 35);
			this->button4->TabIndex = 6;
			this->button4->Text = L"Cmpr with original";
			this->button4->UseVisualStyleBackColor = true;
			this->button4->Click += gcnew System::EventHandler(this, &MyForm::button4_Click);
			// 
			// button5
			// 
			this->button5->Location = System::Drawing::Point(748, 398);
			this->button5->Margin = System::Windows::Forms::Padding(4, 5, 4, 5);
			this->button5->Name = L"button5";
			this->button5->Size = System::Drawing::Size(188, 35);
			this->button5->TabIndex = 7;
			this->button5->Text = L"Make Original";
			this->button5->UseVisualStyleBackColor = true;
			this->button5->Click += gcnew System::EventHandler(this, &MyForm::button5_Click);
			// 
			// button6
			// 
			this->button6->Location = System::Drawing::Point(748, 585);
			this->button6->Margin = System::Windows::Forms::Padding(4, 5, 4, 5);
			this->button6->Name = L"button6";
			this->button6->Size = System::Drawing::Size(188, 35);
			this->button6->TabIndex = 8;
			this->button6->Text = L"Add Noize";
			this->button6->UseVisualStyleBackColor = true;
			this->button6->Click += gcnew System::EventHandler(this, &MyForm::button6_Click);
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(748, 483);
			this->label2->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(94, 20);
			this->label2->TabIndex = 9;
			this->label2->Text = L"Similarity is: ";
			// 
			// button7
			// 
			this->button7->Location = System::Drawing::Point(753, 737);
			this->button7->Margin = System::Windows::Forms::Padding(4, 5, 4, 5);
			this->button7->Name = L"button7";
			this->button7->Size = System::Drawing::Size(188, 35);
			this->button7->TabIndex = 10;
			this->button7->Text = L"Open in CV";
			this->button7->UseVisualStyleBackColor = true;
			this->button7->Click += gcnew System::EventHandler(this, &MyForm::button7_Click);
			// 
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(9, 20);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(1245, 796);
			this->Controls->Add(this->button7);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->button6);
			this->Controls->Add(this->button5);
			this->Controls->Add(this->button4);
			this->Controls->Add(this->button3);
			this->Controls->Add(this->button2);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->pictureBox2);
			this->Controls->Add(this->button1);
			this->Controls->Add(this->pictureBox1);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
			this->Margin = System::Windows::Forms::Padding(4, 5, 4, 5);
			this->Name = L"MyForm";
			this->Text = L"MyForm";
			this->Load += gcnew System::EventHandler(this, &MyForm::MyForm_Load);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox2))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	


	private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e) {

		//Start to load image into the picture box and set as src Mat image
		OpenFileDialog^ opDialog = gcnew OpenFileDialog();
		opDialog->Filter = "Image(*.bmp; *.jpg)|*.bmp;*.jpg|All files (*.*)|*.*||";
		if (opDialog->ShowDialog() == System::Windows::Forms::DialogResult::Cancel)
		{
			return;
		}
		Bitmap^ bmpSrc = gcnew Bitmap(opDialog->FileName);
		pictureBox1->Image = bmpSrc;
		pictureBox1->Refresh();
		pictureBox2->Image = bmpSrc;
		pictureBox2->Refresh();
		src = imread(ConvertString2Char(opDialog->FileName));
		srcOriginal = imread(ConvertString2Char(opDialog->FileName));
	}

private: System::Void button2_Click(System::Object^  sender, System::EventArgs^  e) {


	if (!pictureBox1->Image) return;


	src = toGrayScale(src);
	
	pictureBox1->Image = toBitMapPoint(src);
	pictureBox1->Refresh();
	imshow("Display window", src);

}



	private: char* ConvertString2Char(System::String^ str)
	{
		char* str2 = (char*)(void*)Marshal::StringToHGlobalAnsi(str);
		return str2;	
	}


private: System::Void button3_Click(System::Object^  sender, System::EventArgs^  e) {

	if (!pictureBox1->Image) return;



	src = mid_filtration(src, 9);
	
	pictureBox1->Image = toBitMapPoint(src);
	pictureBox1->Refresh();
	imshow("Display window", src);

}
private: System::Void button6_Click(System::Object^  sender, System::EventArgs^  e) {


	if (!pictureBox1->Image) return;

	src = addNoize(src, 10, 20);
	
	pictureBox1->Image = toBitMapPoint(src);
	pictureBox1->Refresh();
	imshow("Display window", src);

}
private: System::Void button4_Click(System::Object^  sender, System::EventArgs^  e) {

	if (!pictureBox1->Image) return;


	//float sim = Similarity(src, srcOriginal);
	float sim = SSIM_Similarity(src, srcOriginal);

	label2->Text = "Similarity is: " + Convert::ToString(sim * 100) + "%";

}
private: System::Void button5_Click(System::Object^  sender, System::EventArgs^  e) {


	if (!pictureBox1->Image) return;

	srcOriginal = src.clone();
	pictureBox2->Image = toBitMapPoint(src);
	pictureBox2->Refresh();


}
private: System::Void button7_Click(System::Object^  sender, System::EventArgs^  e) {

	if (!pictureBox1->Image) return;
	imshow("Display window", src);

}
private: System::Void MyForm_Load(System::Object^  sender, System::EventArgs^  e) {
}
};
}
