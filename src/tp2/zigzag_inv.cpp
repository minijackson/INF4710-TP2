
#include "tp2/common.hpp"

using namespace tp2;

// zigzag_inv: returns an NxN block created by de-zigzagging through a (NxN)-element 1D array
cv::Mat_<short> tp2::zigzag_inv(const std::array<short,s_nVectorSize>& aZigzag) {
    constexpr size_t nBlockSize = tp2::s_nBlockSize;
    int nIdx = 0;
    cv::Mat_<short> oOutput(nBlockSize*nBlockSize,1);
    for(int i=0; i<nBlockSize*2; ++i)
        for(int j=(i<nBlockSize)?0:i-nBlockSize+1; j<=i && j<nBlockSize; ++j)
            oOutput((i&1)?j*(nBlockSize-1)+i:(i-j)*nBlockSize+j) = aZigzag[nIdx++];
    return oOutput.reshape(0,nBlockSize);
}
