
#include "tp2/common.hpp"

using namespace tp2;

// decoup_inv: reconstructs a 2D image (i.e. always single channel) from a block array
cv::Mat_<uchar> tp2::decoup_inv(const std::vector<cv::Mat_<uchar>>& vBlocks,
                                const cv::Size& oImageSize) {
	constexpr size_t nBlockSize = tp2::s_nBlockSize;
	CV_Assert(!vBlocks.empty() && !vBlocks[0].empty() && vBlocks[0].rows == nBlockSize &&
	          vBlocks[0].cols == nBlockSize && vBlocks[0].isContinuous());
	CV_Assert(oImageSize.area() > 0 && (oImageSize.height % nBlockSize) == 0 &&
	          (oImageSize.width % nBlockSize) == 0);
	cv::Mat_<uchar> oOutput(oImageSize);

	// @@@@ TODO (fill oOutput)

	return oOutput;
}
