
// INF4710 A2017 TP2
//
//   REMEMBER: TO RUN THIS CODE, SET THIS PROJECT AS THE STARTUP PROJECT WHEN DEBUGGING! (MSVC)
//

#include "tp2/common.hpp"

int main(int /*argc*/, char** /*argv*/) {
	try {

		cv::VideoCapture oSequence(DATA_ROOT_PATH "/tp2/heart/heart.%03d.png");
		if(!oSequence.isOpened()) {
			std::cerr << "Could not load image sequence; exiting..." << std::endl;
			std::exit(-1);
		}

		cv::Mat oFrame;
		oSequence >> oFrame;
		CV_Assert(!oFrame.empty() && oFrame.type() == CV_8UC1 &&
		          oFrame.size() == cv::Size(256, 256));

		// extract block at hard-coded location to run some tracking tests... (you should try more
		// than one location!) note: don't expect good results after 5-6 frames, tracking will be
		// lost quickly if local intensities change too much!
		const cv::Point2i oDefaultBlockPos(172, 126); // always use top-left location for regions
		constexpr int nBlockSize =
		        16; // let's use big blocks, it will be easier to track large patterns!
		const cv::Rect oDefaultBlockRegion(oDefaultBlockPos, cv::Size(nBlockSize, nBlockSize));
		const cv::Mat_<uchar> oDefaultBlock = oFrame(oDefaultBlockRegion).clone();
		cv::Mat oFrame_display_base         = oFrame.clone();
		cv::rectangle(oFrame_display_base,
		              oDefaultBlockRegion,
		              cv::Scalar_<uchar>(255),
		              2); // highlight base block location in white
		bool bFirstFrame = true;
		double dCurrEQM;
		while(!oFrame.empty()) {

			const cv::Rect oDefaultSearchRegion(150, 100, 60, 60); // some hard-coded search
			                                                       // region... (you should try more
			                                                       // than one region! borders too!)
			const cv::Point2i vBlockOffsetVec = tp2::match_block(
			        oDefaultBlock, oDefaultBlockPos, oDefaultSearchRegion, oFrame, dCurrEQM);

			if(bFirstFrame) {
				// if we are still on the first frame, the search should find the same block at the
				// same location, since the search region overlaps with the original block location
				CV_Assert(vBlockOffsetVec.x == 0 &&
				          vBlockOffsetVec.y == 0); // this is only valid for the hard-coded values
				                                   // above; remove this if you intend on running
				                                   // more complicated tests later
				std::cout << "press any key to step through the video..." << std::endl;
				bFirstFrame = false;
			} else
				std::cout << "offset = " << vBlockOffsetVec << ",  eqm = " << dCurrEQM << std::endl;
			cv::imshow("base frame", oFrame_display_base);
			cv::Mat oFrame_display_next = oFrame.clone();
			const cv::Rect oNextBlockRegion(oDefaultBlockPos + vBlockOffsetVec,
			                                cv::Size(nBlockSize, nBlockSize));
			cv::rectangle(oFrame_display_next,
			              oNextBlockRegion,
			              cv::Scalar_<uchar>(255),
			              2); // highlight predicted block location in white
			cv::rectangle(oFrame_display_next,
			              oDefaultSearchRegion,
			              cv::Scalar_<uchar>(128),
			              1); // highlight search zone location in gray
			cv::imshow("next frame", oFrame_display_next);
			cv::waitKey(0);
			oSequence >> oFrame;
		}
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
