
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

	int blockSize = vBlocks[0].rows;
	for(size_t i = 0; i < vBlocks.size(); ++i)
	{
		int nbBlocksPerLine = oImageSize.width /8;
		int xOffset = (i/nbBlocksPerLine) * 8;
		int yOffset = (i%nbBlocksPerLine)*8;

		for(int iBlock = 0; iBlock < blockSize; ++iBlock)
		{
			for(int jBlock = 0; jBlock < blockSize; ++jBlock)
			{
				oOutput.at<uchar>(xOffset+iBlock, yOffset+jBlock) = vBlocks[i].at<uchar>(iBlock, jBlock);
			}
		}
	}

	return oOutput;
}
