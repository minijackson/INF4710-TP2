
#include "tp2/common.hpp"

using namespace tp2;

// conv_ycbcr2rgb: converts an 8-bit-depth YCbCr image to RGB format considering optional 4:2:0
// subsampling
void tp2::conv_ycbcr2rgb(const cv::Mat_<uchar>& Y,
                         const cv::Mat_<uchar>& Cb,
                         const cv::Mat_<uchar>& Cr,
                         bool bSubsample,
                         cv::Mat& RGB) {
	CV_Assert(!Y.empty() && Y.isContinuous() && !Cb.empty() && Cb.isContinuous() && !Cr.empty() &&
	          Cr.isContinuous());
	CV_Assert(!bSubsample || (Y.rows / 2 == Cb.rows && Y.rows / 2 == Cr.rows &&
	                          Y.cols / 2 == Cb.cols && Y.cols / 2 == Cr.cols));
	CV_Assert(bSubsample ||
	          (Y.rows == Cb.rows && Y.rows == Cr.rows && Y.cols == Cb.cols && Y.cols == Cr.cols));
	RGB.create(Y.size(), CV_8UC3);

	// @@@@ TODO (do not use built-in functions here!)

	for(int row = 0; row < RGB.rows; ++row)
	{
		for(int col = 0; col < RGB.cols; ++col)
		{

			uchar y = Y.at<uchar>(row, col);
			uchar cb = Cb.at<uchar>(row, col);
			uchar cr = Cr.at<uchar>(row, col);

			// Blue
			RGB.at<cv::Vec3b>(row, col)[0] = y + 1.773*(cb-128);

			// Green
			RGB.at<cv::Vec3b>(row, col)[1] = y - 0.714*(cr-128) - 0.344*(cb-128);

			// Red
			RGB.at<cv::Vec3b>(row, col)[2] = y + 1.403*(cr-128);
		}
	}
}
