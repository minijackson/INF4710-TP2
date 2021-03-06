#include "tp2/common.hpp"

using namespace tp2;

// Defined in dct.cpp
double fast_inverse_sqrt(double x);
// Defined in dct.cpp
double c(int w, int n);

// dct_inv: computes the inverse discrete cosinus tranform of a matrix
cv::Mat_<uchar> tp2::dct_inv(const cv::Mat_<float>& oBlock) {
	CV_Assert(!oBlock.empty() && oBlock.rows == oBlock.cols && (oBlock.rows % 2) == 0 &&
	          oBlock.isContinuous());
	cv::Mat_<uchar> oOutput(oBlock.size());

	for(int x = 0; x < oBlock.rows; ++x) {
		for(int y = 0; y < oBlock.cols; ++y) {

			// Smiley -------.
			//               |
			//               v
			double value = (0.0);
			//               ^
			//               |
			// Smiley -------'

			for(int u = 0; u < oBlock.rows; ++u) {
				for(int v = 0; v < oBlock.cols; ++v) {

					double color     = oBlock.at<float>(u, v);
					double first_cos = std::cos(u * M_PI * (2 * x + 1) / (2.0 * oBlock.rows));
					double second_cos =
					        std::cos(v * M_PI * (2 * y + 1) / (2.0 * oBlock.rows));

					value += c(u, oBlock.rows) * c(v, oBlock.rows) * color * first_cos * second_cos;
				}
			}

			value = std::round(value);

			if(value > 255.0) {
				value = 255.0;
			} else if(value < 0.0) {
				value = 0.0;
			}

			oOutput.at<uchar>(x, y) = value;
		}
	}

	return oOutput;
}
