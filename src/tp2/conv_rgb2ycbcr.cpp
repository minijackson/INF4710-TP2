#include "tp2/common.hpp"

using namespace tp2;

// conv_rgb2ycbcr: converts an 8-bit-depth RGB image to Y'CbCr format using optional 4:2:0
// subsampling
void tp2::conv_rgb2ycbcr(const cv::Mat& RGB,
                         bool bSubsample,
                         cv::Mat_<uchar>& Y,
                         cv::Mat_<uchar>& Cb,
                         cv::Mat_<uchar>& Cr) {
	CV_Assert(!RGB.empty() && RGB.type() == CV_8UC3 && RGB.isContinuous());
	Y.create(RGB.rows, RGB.cols);

	// @@@@ TODO (do not use built-in functions here!)
	// Cb.create(/*...*/);
	// Cr.create(/*...*/);
	// ...

	for(int row = 0; row < RGB.rows; ++row)
	{
		for(int col = 0; col < RGB.cols; ++col)
		{
			uint8_t b = RGB.at<cv::Vec3b>(row, col)[0];
			uint8_t g = RGB.at<cv::Vec3b>(row, col)[1];
			uint8_t r = RGB.at<cv::Vec3b>(row, col)[2];

			uchar y =0.299*r + 0.587*g + 0.114*b;
			Y.at<uchar>(row, col) = y;
			Cb.at<uchar>(row, col) = 128 + 0.564*(b-y);
			Cr.at<uchar>(row, col) = 128 + 0.713*(r-y);
		}
	}
}
