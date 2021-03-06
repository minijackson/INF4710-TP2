#include "tp2/common.hpp"

using namespace tp2;

// quantif: divides the values contained in a matrix based on a quantification quality factor
cv::Mat_<short> tp2::quantif(const cv::Mat_<float>& oBlock, int nQualityFactor) {
	CV_Assert(!oBlock.empty() && oBlock.rows == 8 && oBlock.cols == 8);
	static const std::map<int, cv::Mat_<float>> s_mQuantMats = {
	        {10,
	         (cv::Mat_<float>(8, 8) <<
	          80, 60, 50, 80, 120, 200, 255, 255,
	          55, 60, 70, 95, 130, 255, 255, 255,
	          70, 65, 80, 120, 200, 255, 255, 255,
	          70, 85, 110, 145, 255, 255, 255, 255,
	          90, 110, 185, 255, 255, 255, 255, 255,
	          120, 175, 255, 255, 255, 255, 255, 255,
	          245, 255, 255, 255, 255, 255, 255, 255,
	          255, 255, 255, 255, 255, 255, 255, 255)},
	        {50,
	         (cv::Mat_<float>(8, 8) <<
	          16, 11, 10, 16, 24, 40, 51, 61,
	          12, 12, 14, 19, 26, 58, 60, 55,
	          14, 13, 16, 24, 40, 57, 69, 56,
	          14, 17, 22, 29, 51, 87, 80, 62,
	          18, 22, 37, 56, 68, 109, 103, 77,
	          24, 35, 55, 64, 81, 104, 113, 92,
	          49, 64, 78, 87, 103, 121, 120, 101,
	          72, 92, 95, 98, 112, 100, 103, 99)},
	        {90,
	         (cv::Mat_<float>(8, 8) <<
	          3, 2, 2, 3, 5, 8, 10, 12,
	          2, 2, 3, 4, 5, 12, 12, 11,
	          3, 3, 3, 5, 8, 11, 14, 11,
	          3, 3, 4, 6, 10, 17, 16, 12,
	          4, 4, 7, 11, 14, 22, 21, 15,
	          5, 7, 11, 13, 16, 12, 23, 18,
	          10, 13, 16, 17, 21, 24, 24, 21,
	          14, 18, 19, 20, 22, 20, 20, 20)},
	        {100, cv::Mat_<float>::ones(8, 8)},
	};
	auto pMapIter             = s_mQuantMats.begin();
	int nClosestQualityFactor = pMapIter->first;
	std::advance(pMapIter, 1);
	for(; pMapIter != s_mQuantMats.end(); ++pMapIter)
		nClosestQualityFactor = (std::abs(nQualityFactor - pMapIter->first) <
		                         std::abs(nQualityFactor - nClosestQualityFactor))
		                                ? pMapIter->first
		                                : nClosestQualityFactor;

	const cv::Mat_<float>& oQuantifMat = s_mQuantMats.at(nClosestQualityFactor);
	cv::Mat_<short> oOutput(oBlock.size());

	{
		auto block_it = oBlock.begin();
		auto quantif_matrix_it = oQuantifMat.begin();
		auto output_it = oOutput.begin();

		while(block_it != oBlock.end()) {

			*output_it = std::round(*block_it / *quantif_matrix_it);

			++block_it;
			++quantif_matrix_it;
			++output_it;
		}
	}

	return oOutput;
}
