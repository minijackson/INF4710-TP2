
#include "tp2/common.hpp"

using namespace tp2;

// dct: computes the discrete cosinus tranform of a matrix
cv::Mat_<float> tp2::dct(const cv::Mat_<uchar>& oBlock) {
	CV_Assert(!oBlock.empty() && oBlock.rows == oBlock.cols && (oBlock.rows % 2) == 0 &&
	          oBlock.isContinuous());
	cv::Mat_<float> oOutput(oBlock.size());

	// @@@@ TODO (do not use built-in functions here!)

	return oOutput;
}
