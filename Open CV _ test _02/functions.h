#pragma once



using namespace cv;
using namespace std;


uchar get_max(int a, int b, int c) {
	return  max(a, max(b, c));
}

uchar get_min(int a, int b, int c) {
	return min(a, min(b, c));
}

double uniform() {
	double sigma = 10;
	return sigma * sqrt(3) * ((2 * (rand() % 100) / 100) - 1);
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



/*

int main_2()
{
	Mat image;
	image = imread("example.jpg", IMREAD_COLOR);
	//image = imread("example.jpg", CV_LOAD_IMAGE_GRAYSCALE); 
	if (!image.data)
	{
		cout << "Could not open or find the image" << std::endl;
		return -1;
	}

	/*Mat saltpepper_noise = Mat::zeros(image.rows, image.cols, CV_8U);
	randu(saltpepper_noise, 0, 255);

	Mat black = saltpepper_noise < 30;
	Mat white = saltpepper_noise > 225;

	Mat saltpepper_img = image.clone();
	saltpepper_img.setTo(255, white);
	saltpepper_img.setTo(0, black);



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

	for (int i = 0; i < image.rows; ++i)
		for (int j = 0; j < image.cols; ++j) {
			if (rand() % 100 <= 5)
				image.at<Vec3b>(i, j) += Vec3b(uniform(), uniform(), uniform());
		}

	int WinSize = 6;
	Vec3b res_color;
	int aveR, aveG, aveB;
	double sumR, sumG, sumB;
	int col, row, n, m;
	for (col = 0; col < image.cols; col++)
		for (row = 0; row < image.rows; row++)
		{
			sumB = 0;
			sumG = 0;
			sumR = 0;
			for (m = col - ((WinSize - 1) / 2); m <= (col + (WinSize - 1) / 2); m++) {
				if (m < 0) {
					//m = 0;
					continue;
				}
				if (m > image.cols - 1) {
					//m = image.cols - 1;
					continue;
				}
				for (n = row - ((WinSize - 1) / 2); n <= row + ((WinSize - 1) / 2); n++)
				{
					if (n < 0) {
						//n = 0;
						continue;
					}
					if (n > image.rows - 1) {
						continue;
						//n = image.rows - 1;
					}

					res_color = image.at<Vec3b>(m, n);
					Vec3b color = image.at<Vec3b>(m, n);
					uchar blue = color.val[0];
					uchar green = color.val[1];
					uchar red = color.val[2];
					sumR += red;
					sumG += green;
					sumB += blue;
				}
			}
			aveR = (int)(sumR / (WinSize * WinSize));
			aveG = (int)(sumG / (WinSize * WinSize));
			aveB = (int)(sumB / (WinSize * WinSize));
			image.at<Vec3b>(col, row) = Vec3b(aveR, aveG, aveB);
		}


	//imshow("Display window", saltpepper_img);
	imshow("Display window", image);
	waitKey(0);
	return 0;

}


*/