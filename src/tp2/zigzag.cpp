#include "tp2/common.hpp"

using namespace tp2;

enum Direction {
	LEFT,
	RIGHT,
	DOWN,
	UP,
	DOWN_LEFT,
	UP_RIGHT,
};

enum DiagonalDirection {
	DOWN_LEFT2,
	UP_RIGHT2,
};

bool try_go(Direction const dir, size_t& x, size_t& y, size_t width, size_t height);

// zigzag: returns a (NxN)-element 1D array created by zig-zagging through a NxN block
std::array<short, tp2::s_nVectorSize> tp2::zigzag(const cv::Mat_<short>& oBlock) {
	constexpr size_t nBlockSize = tp2::s_nBlockSize;
	CV_Assert(!oBlock.empty() && oBlock.rows == oBlock.cols && oBlock.rows == nBlockSize);
	std::array<short, nBlockSize* nBlockSize> aZigzag = {};

	auto width  = (nBlockSize < oBlock.cols) ? nBlockSize : oBlock.cols,
	     height = (nBlockSize < oBlock.rows) ? nBlockSize : oBlock.rows;

	if(width == 0 || height == 0) {
		return aZigzag;
	}

	// 1st pixel
	//
	// Go right or down (can be last pixel)
	// Go down-left...
	// Go down or left (can be last pixel)
	// Go up-right...
	// Again

	auto current_direction = DiagonalDirection::DOWN_LEFT2;

	aZigzag[0] = oBlock.at<short>(0, 0);

	if(width == 1 && height == 1) {
		return aZigzag;
	}

	size_t i = 1, x = 0, y = 0;

	while(true) {
		if(current_direction == DiagonalDirection::DOWN_LEFT2) {

			try_go(Direction::RIGHT, x, y, width, height) ||
			        try_go(Direction::DOWN, x, y, width, height);
			aZigzag[i] = oBlock.at<short>(x, y);
			++i;

			if(x == width - 1 && y == height - 1) {
				break;
			}

			while(try_go(Direction::DOWN_LEFT, x, y, width, height)) {
				aZigzag[i] = oBlock.at<short>(x, y);
				++i;
			}

			current_direction = DiagonalDirection::UP_RIGHT2;

		} else {

			try_go(Direction::DOWN, x, y, width, height) ||
			        try_go(Direction::RIGHT, x, y, width, height);
			aZigzag[i] = oBlock.at<short>(x, y);
			++i;

			if(x == width - 1 && y == height - 1) {
				break;
			}

			while(try_go(Direction::UP_RIGHT, x, y, width, height)) {
				aZigzag[i] = oBlock.at<short>(x, y);
				++i;
			}

			current_direction = DiagonalDirection::DOWN_LEFT2;
		}
	}

	return aZigzag;
}

bool try_go(Direction const dir, size_t& x, size_t& y, size_t width, size_t height) {
	switch(dir) {
		case Direction::LEFT:
			if(x == 0) {
				return false;
			} else {
				--x;
			}
			break;

		case Direction::RIGHT:
			if(x >= width - 1) {
				return false;
			} else {
				++x;
			}
			break;

		case Direction::UP:
			if(y == 0) {
				return false;
			} else {
				--y;
			}
			break;

		case Direction::DOWN:
			if(y >= height - 1) {
				return false;
			} else {
				++y;
			}
			break;

		case Direction::DOWN_LEFT: {
			auto old_x = x, old_y = y;
			bool success = try_go(Direction::DOWN, x, y, width, height) &&
			               try_go(Direction::LEFT, x, y, width, height);
			if(!success) {
				x = old_x;
				y = old_y;
				return false;
			}
		} break;

		case Direction::UP_RIGHT: {
			auto old_x = x, old_y = y;
			bool success = try_go(Direction::UP, x, y, width, height) &&
			               try_go(Direction::RIGHT, x, y, width, height);
			if(!success) {
				x = old_x;
				y = old_y;
				return false;
			}
		} break;
	}

	return true;
}
