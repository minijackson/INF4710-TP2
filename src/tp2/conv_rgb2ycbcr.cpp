#include "tp2/common.hpp"

using namespace tp2;

double rgb2y(uchar r, uchar g, uchar b) {
	return std::min((0.299 * r) + (0.587 * g) + (0.114 * b), 255.0);
}

double rgb2cb(uchar r, uchar g, uchar b) {
	return std::min(128.0 - (0.16878 * r) - (0.3313 * g) + (0.5 * b), 255.0);
}

double rgb2cr(uchar r, uchar g, uchar b) {
	return std::min(128.0 + (0.5 * r) - (0.4187 * g) - (0.08131 * b), 255.0);
}

// conv_rgb2ycbcr: converts an 8-bit-depth RGB image to Y'CbCr format using optional 4:2:0
// subsampling
void tp2::conv_rgb2ycbcr(const cv::Mat& RGB,
                         bool bSubsample,
                         cv::Mat_<uchar>& Y,
                         cv::Mat_<uchar>& Cb,
                         cv::Mat_<uchar>& Cr) {
	CV_Assert(!RGB.empty() && RGB.type() == CV_8UC3 && RGB.isContinuous());

	Y.create(RGB.rows, RGB.cols);

	if(bSubsample) {
		Cb.create(RGB.rows / 2, RGB.cols / 2);
		Cr.create(RGB.rows / 2, RGB.cols / 2);
	} else {
		Cb.create(RGB.rows, RGB.cols);
		Cr.create(RGB.rows, RGB.cols);
	}

	double cb = 0, cr = 0;

	for(int row = 0; row < RGB.rows; ++row) {

		for(int col = 0; col < RGB.cols; ++col) {
			uint8_t b = RGB.at<cv::Vec3b>(row, col)[0];
			uint8_t g = RGB.at<cv::Vec3b>(row, col)[1];
			uint8_t r = RGB.at<cv::Vec3b>(row, col)[2];

			uchar y = rgb2y(r, g, b);

			Y.at<uchar>(row, col) = y;

			if(bSubsample) {

				if(row % 2 == 0 && col % 2 == 0) {
					cb = 0;
					cr = 0;
					std::cout << "Initialization" << std::endl;
				}

				cb += rgb2cb(r, g, b);
				cr += rgb2cr(r, g, b);
				std::cout << "cb = " << cb << ", cr = " << cr << std::endl;

				if(row % 2 == 1 && col % 2 == 1) {
					cb /= 4.0;
					cr /= 4.0;

					std::cout << "Mean: cb = " << cb << ", cr = " << cr << std::endl;

					Cb.at<uchar>(row / 2, col / 2) = std::round(cb);
					Cr.at<uchar>(row / 2, col / 2) = std::round(cr);
				}

			} else {
				Cb.at<uchar>(row, col) = std::round(rgb2cb(r, g, b));
				Cr.at<uchar>(row, col) = std::round(rgb2cr(r, g, b));
			}
		}
	}
}
