
#include "tp2/common.hpp"

using namespace tp2;

// match_block: finds the best match for a block in a given image region by minimizing mean squared error (EQM)
cv::Point2i tp2::match_block(const cv::Mat_<uchar>& oBlock, cv::Point2i oBlockPos, cv::Rect oSearchRegion, const cv::Mat_<uchar>& oSearchImage, double& dEQM_min) {
    // return value : 2d displacement vector between oBlockPos and the newly found block's top-left corner
    // oBlock : square image block taken from the latest image to encode
    // oBlockPos : 2d position vector of the top-left corner of 'oBlock' in the image to encode
    // oSearchRegion : rectangle containing the image space to search for correspondences
    // oSearchImage : previously encoded image in which to search for correspondences
    // dEQM_min : minimal mean squared error found associated with the returned vector (output only)
    CV_Assert(oBlock.rows==oBlock.cols && oBlock.rows>0);
    const int nBlockSize = oBlock.rows;
    CV_Assert(oBlockPos.x>=0 && oBlockPos.y>=0);
    CV_Assert(oBlockPos.x<oSearchImage.cols-nBlockSize+1 && oBlockPos.y<oSearchImage.rows-nBlockSize+1);
    CV_Assert(oSearchRegion.height>nBlockSize && oSearchRegion.width>nBlockSize);
    CV_Assert(oSearchImage.rows>nBlockSize && oSearchImage.cols>nBlockSize);
    cv::Point2i vOutputDir; // this is a direction vector; it should sometimes also contain negative values!

    // @@@@ TODO (hint: make sure operations around borders wont crash your implementation!)
    // note1: top-left corner of image is x,y=<0,0> origin; therefore...
    //     - if the best matched block is to the left of the input block location (given by oBlockPos), the output vector value in x MUST be negative
    //     - if the best matched block is above of the input block location (given by oBlockPos), the output vector value in y MUST be negative
    // note2: DO NOT USE OPENCV BUILT-IN FUNCTIONS HERE (you can look at the implementation of 'cv::matchTemplate', but don't use it!)

    return vOutputDir;
}
