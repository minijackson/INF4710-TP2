#include "tp2/common.hpp"

#include <limits>

using namespace tp2;

double calcEQM(cv::Mat_<uchar> const& ref, cv::Mat_<uchar> const& target) {
	double sum = 0;

	auto ref_it = ref.begin(), ref_end = ref.end(), target_it = target.begin();
	while(ref_it != ref_end) {
		sum += std::pow(*ref_it - *target_it, 2);

		++ref_it;
		++target_it;
	}

	return sum / (ref.rows * ref.cols);
}

// match_block: finds the best match for a block in a given image region by minimizing mean squared
// error (EQM)
cv::Point2i tp2::match_block(const cv::Mat_<uchar>& oBlock,
                             cv::Point2i oBlockPos,
                             cv::Rect oSearchRegion,
                             const cv::Mat_<uchar>& oSearchImage,
                             double& dEQM_min) {
	// return value : 2d displacement vector between oBlockPos and the newly found block's top-left
	// corner oBlock : square image block taken from the latest image to encode oBlockPos : 2d
	// position vector of the top-left corner of 'oBlock' in the image to encode oSearchRegion :
	// rectangle containing the image space to search for correspondences oSearchImage : previously
	// encoded image in which to search for correspondences dEQM_min : minimal mean squared error
	// found associated with the returned vector (output only)
	CV_Assert(oBlock.rows == oBlock.cols && oBlock.rows > 0);
	const int nBlockSize = oBlock.rows;
	CV_Assert(oBlockPos.x >= 0 && oBlockPos.y >= 0);
	CV_Assert(oBlockPos.x < oSearchImage.cols - nBlockSize + 1 &&
	          oBlockPos.y < oSearchImage.rows - nBlockSize + 1);
	CV_Assert(oSearchRegion.height > nBlockSize && oSearchRegion.width > nBlockSize);
	CV_Assert(oSearchImage.rows > nBlockSize && oSearchImage.cols > nBlockSize);
	cv::Point2i vOutputDir; // this is a direction vector; it should sometimes also contain negative
	                        // values!

	dEQM_min = std::numeric_limits<double>::max();

	size_t endX = std::min(oSearchRegion.x + oSearchRegion.width - oBlock.cols, oSearchImage.cols),
	       endY = std::min(oSearchRegion.y + oSearchRegion.height - oBlock.rows, oSearchImage.cols);

	for(size_t x = oSearchRegion.x; x < endX; ++x) {
		for(size_t y = oSearchRegion.y; y < endY; ++y) {
			cv::Rect currentBlockArea(x, y, oBlock.cols, oBlock.rows);
			cv::Mat_<uchar> currentBlock = oSearchImage(currentBlockArea);

			double currentEQM = calcEQM(oBlock, currentBlock);

			if(currentEQM < dEQM_min) {
				dEQM_min = currentEQM;
				vOutputDir = cv::Point2i{x, y} - oBlockPos;
			}

		}
	}

	// @@@@ TODO (hint: make sure operations around borders wont crash your implementation!)
	// note1: top-left corner of image is x,y=<0,0> origin; therefore...
	//     - if the best matched block is to the left of the input block location (given by
	//     oBlockPos), the output vector value in x MUST be negative
	//     - if the best matched block is above of the input block location (given by oBlockPos),
	//     the output vector value in y MUST be negative
	// note2: DO NOT USE OPENCV BUILT-IN FUNCTIONS HERE (you can look at the implementation of
	// 'cv::matchTemplate', but don't use it!)

	return vOutputDir;
}
