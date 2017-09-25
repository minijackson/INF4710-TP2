
#include "tp2/common.hpp"

using namespace tp2;

// dct_inv: computes the inverse discrete cosinus tranform of a matrix
cv::Mat_<uchar> tp2::dct_inv(const cv::Mat_<float>& oBlock) {
    CV_Assert(!oBlock.empty() && oBlock.rows==oBlock.cols && (oBlock.rows%2)==0 && oBlock.isContinuous());
    cv::Mat_<uchar> oOutput(oBlock.size());

    // @@@@ TODO (do not use built-in functions here!)

    return oOutput;
}
