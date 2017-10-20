// INF4710 A2017 TP2
//
//   REMEMBER: TO RUN THIS CODE, SET THIS PROJECT AS THE STARTUP PROJECT WHEN DEBUGGING! (MSVC)
//
//   ... also, to make it run >100x faster, run it in release!
//

#include "tp2/common.hpp"

// you have to test all configurations by modifying the parameters below!
#define USE_SUBSAMPLING 1
#define USE_QUANT_QUALITY 10

int main(int /*argc*/, char** /*argv*/) {

	//auto a = cv::Mat_<float>(16,16);
	//std::iota(a.begin(), a.end(), 0.0f);
	//std::cout << a << std::endl;
	//std::vector<cv::Mat_<uchar>> test = tp2::decoup(a);

	//for(auto i : test)
	//{
		//std::cout << i << std::endl;
	//}

	//std::cout << tp2::decoup_inv(test, {16, 16}) << std::endl;

	try {
		// note: by default, imread always returns 3-ch images unless the cv::IMREAD_GRAYSCALE flag
		// is set (here we hardcode it based on prior knowledge)
		const std::vector<tp2::ImagePathFlag> vsTestImages = {
		        {DATA_ROOT_PATH "/tp2/airplane.png", cv::IMREAD_COLOR},
		        {DATA_ROOT_PATH "/tp2/baboon.png", cv::IMREAD_COLOR},
		        {DATA_ROOT_PATH "/tp2/cameraman.tif", cv::IMREAD_COLOR},
		        {DATA_ROOT_PATH "/tp2/lena.png", cv::IMREAD_COLOR},
		        {DATA_ROOT_PATH "/tp2/logo.tif", cv::IMREAD_COLOR},
		        {DATA_ROOT_PATH "/tp2/logo_noise.tif", cv::IMREAD_COLOR},
		        {DATA_ROOT_PATH "/tp2/peppers.png", cv::IMREAD_COLOR},
		};
		for(const tp2::ImagePathFlag& oImagePathFlag : vsTestImages) { // for each test image
			const cv::Mat oInput =
			        cv::imread(oImagePathFlag.first, oImagePathFlag.second); // load the test image
			if(oInput.empty()) {
				std::cerr << "Could not load image at '" << oImagePathFlag.first << "'; exiting..."
				          << std::endl;
				std::exit(-1);
			}
			std::cout << "\nProcessing image at '" << oImagePathFlag.first << "'..." << std::endl;

			cv::Mat oInput_decompr;

			//////////////////////
			// COMPRESSION BELOW

			// note: matrices of the form cv::Mat_<X> can only have element type 'X' assigned at
			// each pixel; you can access this element directly via 'mat(row,col)' instead of
			// mat.at<X>(row,col)'

			cv::Mat_<uchar> Y, Cb, Cr;
			tp2::conv_rgb2ycbcr(oInput, USE_SUBSAMPLING, Y, Cb, Cr);

			// note: you can test individual sections by removing steps in the pipeline
			// e.g. to test if conv_rgb2ycbcr and conv_ycbcr2rgb work properly, try decompressing
			// here:
			//     conv_ycbcr2rgb(Y,Cb,Cr,USE_SUBSAMPLING,oInput_decompr);
			// ... the output should then be viewable via cv::imshow(...), as called at the bottom

			const std::vector<cv::Mat_<uchar>> vBlocks_Y  = tp2::decoup(Y);
			const std::vector<cv::Mat_<uchar>> vBlocks_Cb = tp2::decoup(Cb);
			const std::vector<cv::Mat_<uchar>> vBlocks_Cr = tp2::decoup(Cr);

			std::vector<cv::Mat_<uchar>> vBlocks;
			vBlocks.insert(vBlocks.end(), vBlocks_Y.begin(), vBlocks_Y.end());
			vBlocks.insert(vBlocks.end(), vBlocks_Cb.begin(), vBlocks_Cb.end());
			vBlocks.insert(vBlocks.end(), vBlocks_Cr.begin(), vBlocks_Cr.end());

			std::vector<cv::Mat_<float>> vDCTBlocks(vBlocks.size());
			for(size_t b = 0; b < vBlocks.size(); ++b)
				vDCTBlocks[b] = tp2::dct(vBlocks[b]);

			std::vector<cv::Mat_<short>> vQuantifDCTBlocks(vDCTBlocks.size());
			for(size_t b = 0; b < vDCTBlocks.size(); ++b)
				vQuantifDCTBlocks[b] = tp2::quantif(vDCTBlocks[b], USE_QUANT_QUALITY);

			std::vector<std::array<short, 8 * 8>> vInlinedBlocks(vQuantifDCTBlocks.size());
			for(size_t b = 0; b < vQuantifDCTBlocks.size(); ++b)
				vInlinedBlocks[b] = tp2::zigzag(vQuantifDCTBlocks[b]);

			const tp2::HuffOutput oCode = tp2::huff::encode(vInlinedBlocks);

			// @@@@ TODO: check compression rate here...
			size_t const origSize = oInput.total() * oInput.elemSize(), finalSize = oCode.size();

			std::cout << "Taille d'origine: " << origSize << std::endl;
			std::cout << "Taille finale: " << finalSize << std::endl;
			std::cout << "Taux de compression: "
			          << (1 - (static_cast<float>(finalSize) / static_cast<float>(origSize)))
			          << std::endl;

			////////////////////////
			// DECOMPRESSION BELOW

			const std::vector<std::array<short, 8 * 8>> vInlinedBlocks_decompr =
			        tp2::huff::decode(oCode);

			std::vector<cv::Mat_<short>> vQuantifDCTBlocks_decompr(vInlinedBlocks_decompr.size());
			for(size_t b = 0; b < vInlinedBlocks_decompr.size(); ++b)
				vQuantifDCTBlocks_decompr[b] = tp2::zigzag_inv(vInlinedBlocks_decompr[b]);

			std::vector<cv::Mat_<float>> vDCTBlocks_decompr(vQuantifDCTBlocks_decompr.size());
			for(size_t b = 0; b < vQuantifDCTBlocks_decompr.size(); ++b)
				vDCTBlocks_decompr[b] =
				        tp2::quantif_inv(vQuantifDCTBlocks_decompr[b], USE_QUANT_QUALITY);

			std::vector<cv::Mat_<uchar>> vBlocks_decompr(vDCTBlocks_decompr.size());
			for(size_t b = 0; b < vDCTBlocks_decompr.size(); ++b)
				vBlocks_decompr[b] = tp2::dct_inv(vDCTBlocks_decompr[b]);

			const std::vector<cv::Mat_<uchar>> vBlocks_Y_decompr(
			        vBlocks_decompr.begin(), vBlocks_decompr.begin() + vBlocks_Y.size());
			const std::vector<cv::Mat_<uchar>> vBlocks_Cb_decompr(
			        vBlocks_decompr.begin() + vBlocks_Y.size(),
			        vBlocks_decompr.begin() + vBlocks_Y.size() + vBlocks_Cb.size());
			const std::vector<cv::Mat_<uchar>> vBlocks_Cr_decompr(
			        vBlocks_decompr.begin() + vBlocks_Y.size() + vBlocks_Cb.size(),
			        vBlocks_decompr.end());
			const cv::Mat_<uchar> Y_decompr  = tp2::decoup_inv(vBlocks_Y_decompr, Y.size());
			const cv::Mat_<uchar> Cb_decompr = tp2::decoup_inv(vBlocks_Cb_decompr, Cb.size());
			const cv::Mat_<uchar> Cr_decompr = tp2::decoup_inv(vBlocks_Cr_decompr, Cr.size());

			tp2::conv_ycbcr2rgb(Y_decompr, Cb_decompr, Cr_decompr, USE_SUBSAMPLING, oInput_decompr);

			if(oInput_decompr.size() != oInput.size()) {
				std::cout << "ERROR! Reconstructed image had different size from original... "
				             "aborting."
				          << std::endl;
				std::exit(-1);
			}
			cv::Mat oDisplay;
			cv::hconcat(oInput, oInput_decompr, oDisplay);
			cv::Mat oDiff;
			cv::absdiff(oInput, oInput_decompr, oDiff);
			cv::hconcat(oDisplay, oDiff, oDisplay);
			const std::string sWinName =
			        oImagePathFlag.first.substr(oImagePathFlag.first.find_last_of("/\\") + 1) +
			        " orig/reconstr/diff";
			cv::imshow(sWinName, oDisplay);
			cv::waitKey(1);

			if(tp2::isEqual<uint8_t>(oInput, oInput_decompr))
				std::cout << "\t... done. Compression is LOSSLESS." << std::endl;
			else {
				const double dL2Dist = cv::norm(oInput, oInput_decompr);
				const double dMSE    = dL2Dist * dL2Dist / (oInput.total() * oInput.channels());
				const double dPSNR   = 10 * std::log10((255. * 255.) / dMSE);
				std::cout << "\t... done. Compression is LOSSY (PSNR=" << dPSNR << "dB, and "
				          << float(cv::countNonZero(oInput != oInput_decompr)) * 100 /
				                     oInput.total()
				          << "% of pixels have altered values)." << std::endl;
			}
		}
		std::cout << "all done; press any key on a window to quit..." << std::endl;
		cv::waitKey(0);
		return 0;
	} catch(const cv::Exception& e) {
		std::cerr << "Caught cv::Exceptions: " << e.what() << std::endl;
	} catch(const std::runtime_error& e) {
		std::cerr << "Caught std::runtime_error: " << e.what() << std::endl;
	} catch(...) {
		std::cerr << "Caught unhandled exception." << std::endl;
	}
	return 1;
}
