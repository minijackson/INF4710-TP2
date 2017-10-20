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

	int increment = 1;
	if(bSubsample) {
		increment = 2;
	}

	for(int row = 0; row < RGB.rows; row += increment) {
		for(int col = 0; col < RGB.cols; col += increment) {
			uint8_t b = RGB.at<cv::Vec3b>(row, col)[0];
			uint8_t g = RGB.at<cv::Vec3b>(row, col)[1];
			uint8_t r = RGB.at<cv::Vec3b>(row, col)[2];

			Y.at<uchar>(row, col) = rgb2y(r, g, b);

			if(bSubsample) {
				uint8_t b2 = 0, g2 = 0, r2 = 0, b3 = 0, g3 = 0, r3 = 0, b4 = 0, g4 = 0, r4 = 0;
				int moyCount = 1;

				cb = rgb2cb(r, g, b);
				cr = rgb2cr(r, g, b);

				if(col+1 < RGB.cols)
				{
					b2 = RGB.at<cv::Vec3b>(row, col+1)[0];
					g2 = RGB.at<cv::Vec3b>(row, col+1)[1];
					r2 = RGB.at<cv::Vec3b>(row, col+1)[2];

					Y.at<uchar>(row, col+1) = rgb2y(r2, g2, b2);

					cb += rgb2cb(r2, g2, b2);
					cr += rgb2cr(r2, g2, b2);

					++moyCount;
				}
				if(row+1 < RGB.rows)
				{
					b3 = RGB.at<cv::Vec3b>(row+1, col)[0];
					g3 = RGB.at<cv::Vec3b>(row+1, col)[1];
					r3 = RGB.at<cv::Vec3b>(row+1, col)[2];

					Y.at<uchar>(row+1, col) = rgb2y(r3, g3, b3);

					cb += rgb2cb(r3, g3, b3);
					cr += rgb2cr(r3, g3, b3);

					++moyCount;
				}
				if(col+1 < RGB.cols && row+1 < RGB.rows)
				{
					b4 = RGB.at<cv::Vec3b>(row+1, col+1)[0];
					g4 = RGB.at<cv::Vec3b>(row+1, col+1)[1];
					r4 = RGB.at<cv::Vec3b>(row+1, col+1)[2];

					Y.at<uchar>(row+1, col+1) = rgb2y(r4, g4, b4);

					cb += rgb2cb(r4, g4, b4);
					cr += rgb2cr(r4, g4, b4);

					++moyCount;
				}

				cb /= moyCount;
				cr /= moyCount;

				Cb.at<uchar>(row / 2, col / 2) = std::round(cb);
				Cr.at<uchar>(row / 2, col / 2) = std::round(cr);

			} else {
				Cb.at<uchar>(row, col) = std::round(rgb2cb(r, g, b));
				Cr.at<uchar>(row, col) = std::round(rgb2cr(r, g, b));
			}
		}
	}
}
