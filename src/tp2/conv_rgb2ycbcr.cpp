
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
}
