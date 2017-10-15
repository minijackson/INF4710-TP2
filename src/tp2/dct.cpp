#include "tp2/common.hpp"

using namespace tp2;

// Quake!
// http://h14s.p5r.org/2012/09/0x5f3759df.html
float fast_inverse_sqrt(float x) {
	float xhalf = 0.5f * x;
	int i       = *(int*)&x;             // evil floating point bit level hacking
	i           = 0x5f3759df - (i >> 1); // what the fuck?
	x           = *(float*)&i;
	return x * (1.5f - (xhalf * x * x));
}

//double fast_inverse_sqrt(double x) {
	//return 1 / std::sqrt(x);
//}

double c(int w, int n) {
	if(w == 0) {
		return fast_inverse_sqrt(n);
	} else {
		return fast_inverse_sqrt(n) * 1.4142135623730950488f;
	}
}

// dct: computes the discrete cosinus tranform of a matrix
cv::Mat_<float> tp2::dct(const cv::Mat_<uchar>& oBlock) {
	CV_Assert(!oBlock.empty() && oBlock.rows == oBlock.cols && (oBlock.rows % 2) == 0 &&
	          oBlock.isContinuous());
	cv::Mat_<float> oOutput(oBlock.size());

	for(int u = 0; u < oBlock.rows; ++u) {
		for(int v = 0; v < oBlock.cols; ++v) {

			float value = 0.f;

			value += c(u, oBlock.rows) * c(v, oBlock.rows);

			float sum = 0;

			for(int x = 0; x < oBlock.rows; ++x) {
				for(int y = 0; y < oBlock.cols; ++y) {

					double color     = oBlock.at<uchar>(x, y);
					double first_cos = std::cos(u * M_PI * (2.0 * x + 1.0) / (2.0 * oBlock.rows));
					double second_cos =
					        std::cos(v * M_PI * (2.0 * y + 1.0) / (2.0 * oBlock.rows));

					sum += color * first_cos * second_cos;
				}
			}

			value *= sum;

			oOutput.at<float>(u, v) = value;
		}
	}

	return oOutput;
}
