
#include "tp2/common.hpp"

using namespace tp2;

// decoup: reformats a 2D image (i.e. always single channel) to a block array
std::vector<cv::Mat_<uchar>> tp2::decoup(const cv::Mat_<uchar>& oImage) {
	constexpr size_t nBlockSize = tp2::s_nBlockSize;
	CV_Assert(!oImage.empty() && (oImage.rows % nBlockSize) == 0 &&
	          (oImage.cols % nBlockSize) == 0 && oImage.isContinuous());
	std::vector<cv::Mat_<uchar>> vOutput;

	for(int i = 0; i < oImage.cols; i += 8)
	{
		for(int j = 0; j < oImage.rows; j += 8)
		{
			cv::Mat_<uchar> block8x8(8,8);

			for(size_t iBlock = 0; iBlock < nBlockSize; iBlock++)
			{
				for(size_t jBlock = 0; jBlock < nBlockSize; jBlock++)
				{
					if( (i+iBlock < oImage.cols) && (j+jBlock < oImage.rows) )
					{
						block8x8.at<uchar>(iBlock, jBlock) = oImage.at<uchar>(i+iBlock, j+jBlock);
					}
				}
			}

			vOutput.push_back(block8x8);
		}
	}

	return vOutput;
}
