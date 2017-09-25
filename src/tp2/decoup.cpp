
#include "tp2/common.hpp"

using namespace tp2;

// decoup: reformats a 2D image (i.e. always single channel) to a block array
std::vector<cv::Mat_<uchar>> tp2::decoup(const cv::Mat_<uchar>& oImage) {
	constexpr size_t nBlockSize = tp2::s_nBlockSize;
	CV_Assert(!oImage.empty() && (oImage.rows % nBlockSize) == 0 &&
	          (oImage.cols % nBlockSize) == 0 && oImage.isContinuous());
	std::vector<cv::Mat_<uchar>> vOutput;

	// @@@@ TODO (fill vOutput)

	return vOutput;
}
