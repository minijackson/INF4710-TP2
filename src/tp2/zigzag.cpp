
#include "tp2/common.hpp"

using namespace tp2;

// zigzag: returns a (NxN)-element 1D array created by zig-zagging through a NxN block
std::array<short,tp2::s_nVectorSize> tp2::zigzag(const cv::Mat_<short>& oBlock) {
    constexpr size_t nBlockSize = tp2::s_nBlockSize;
	CV_Assert(!oBlock.empty() && oBlock.rows==oBlock.cols && oBlock.rows==nBlockSize);
    std::array<short,nBlockSize*nBlockSize> aZigzag={};

    // @@@@ TODO
	
	return aZigzag;
}
