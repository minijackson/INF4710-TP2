
#pragma once

#include "tp2/defines.hpp"

// note: this file should never be modified!

#include <cmath>
#include <vector>
#include <iomanip>
#include <fstream>
#include <memory>
#include <map>
#include <queue>
#include <map>
#include <string>
#include <array>
#include <climits>
#include <cstdlib>
#include <iterator>
#include <algorithm>
#include <type_traits>
#include <numeric>
#include <iostream>
#include <exception>
#include <opencv2/opencv.hpp>

#ifdef _MSC_VER
#define PAUSE_EXIT(code) do {system("pause"); std::exit(code);} while(0)
#else //ndef(_MSC_VER)
#define PAUSE_EXIT(code) do {std::exit(code);} while(0)
#endif //ndef(_MSC_VER)

namespace tp2 {

    // reminder: uchar==uint8_t, short==int16_t

    constexpr size_t s_nBlockSize = 8u;
    constexpr size_t s_nVectorSize = s_nBlockSize*s_nBlockSize;

    using HuffCode = std::vector<bool>;
    template<typename T>
    using HuffMap_ = std::map<T,HuffCode>;
    template<typename T>
    using HuffReverseMap_ = std::map<HuffCode,T>;

    using ImagePathFlag = std::pair<std::string,int>; // first = path, second = imread flag

    template<typename T>
    struct HuffOutput_ {
        HuffCode string; // contains the final coded bitstring for the input signal
        HuffMap_<T> map; // contains the symbol-to-bitstring mapping to use for encoding
        inline size_t size() const { // return the encoded (byte) size of this Huffman-coded chain
            return string.size()/8+map.size()*sizeof(T)*2; // rough map size approximation, but close enough!
        }
    };

    using HuffOutput = HuffOutput_<short>;

    namespace huff {

        class INode {
        public:
            const int f;
            virtual ~INode() = default;
        protected:
            INode(int f_init) :
                f(f_init) {}
        };

        using INodePtr = std::shared_ptr<INode>;

        class InternalNode : public INode {
        public:
            INodePtr left;
            INodePtr right;
            InternalNode(INodePtr c0, INodePtr c1) :
                INode(c0->f + c1->f), left(c0), right(c1) {}
        };

        template<typename T>
        class LeafNode : public INode {
        public:
            const T c;
            LeafNode(int f_init, T c_init) :
                INode(f_init), c(c_init) {}
        };

        struct NodeCmp {
            bool operator()(const INodePtr& lhs, const INodePtr& rhs) const {
                return lhs->f > rhs->f;
            }
        };

        template<typename T,size_t NF>
        inline INodePtr buildTree(const std::array<int,NF>& frequencies) {
            std::priority_queue<INodePtr,std::vector<INodePtr>,NodeCmp> trees;
            for(size_t i=0; i<frequencies.size(); ++i) {
                if(frequencies[i]!=0)
                    trees.push(std::make_shared<LeafNode<T>>(frequencies[i],(T)i));
            }
            while(trees.size()>1) {
                INodePtr childR = trees.top();
                trees.pop();
                INodePtr childL = trees.top();
                trees.pop();
                trees.push(std::make_shared<InternalNode>(childR, childL));
            }
            return trees.top();
        }

        template<typename T>
        inline void generateCodes(const INodePtr& node, const HuffCode& prefix, std::map<T,HuffCode>& outCodes) {
            if(const LeafNode<T>* lf = dynamic_cast<const LeafNode<T>*>(node.get()))
                outCodes[lf->c] = prefix;
            else if(const InternalNode* in = dynamic_cast<const InternalNode*>(node.get())) {
                HuffCode leftPrefix = prefix;
                leftPrefix.push_back(false);
                generateCodes(in->left,leftPrefix,outCodes);
                HuffCode rightPrefix = prefix;
                rightPrefix.push_back(true);
                generateCodes(in->right,rightPrefix,outCodes);
            }
        }

        template<size_t N=s_nVectorSize, typename T=short>
        inline HuffOutput_<T> encode(const std::vector<std::array<T,N>>& vv) {
            static_assert(sizeof(T)<4,"Do you really have 16GB to spend?"); // replace by unordered map for larger symbol sets
            std::array<int,((size_t)1)<<sizeof(T)*8> anSymbolFrequencies = {};
            for(size_t i=0; i<vv.size(); ++i)
                for(size_t j=0; j<vv[i].size(); ++j)
                    ++anSymbolFrequencies[(typename std::make_unsigned<T>::type)(vv[i][j])];
            INodePtr pRoot = buildTree<T>(anSymbolFrequencies);
            HuffOutput_<T> oOutput;
            generateCodes<T>(pRoot,HuffCode(),oOutput.map);
            for(size_t i=0; i<vv.size(); ++i) {
                for(size_t j=0; j<vv[i].size(); ++j) {
                    const HuffCode& vbCurrString = oOutput.map.at(vv[i][j]);
                    oOutput.string.insert(oOutput.string.end(),vbCurrString.begin(),vbCurrString.end());
                }
            }
            return oOutput;
        }

        template<size_t N=s_nVectorSize, typename T=short>
        inline std::vector<std::array<T,N>> decode(const HuffOutput_<T>& oInput) {
            CV_Assert(!oInput.map.empty() && !oInput.string.empty());
            HuffReverseMap_<T> mReverseMap;
            for(const auto& pMapElement : oInput.map)
                mReverseMap[pMapElement.second] = pMapElement.first;
            CV_Assert(mReverseMap.count(std::vector<bool>())==0);
            std::vector<std::array<T,N>> vvOutput;
            std::vector<bool> vCurrString;
            vCurrString.reserve(1024);
            size_t nStringIdx = 0;
            size_t nOutputBlockIdx = 0;
            while(nStringIdx<=oInput.string.size()) {
                auto pMapCodeIter = mReverseMap.find(vCurrString);
                if(pMapCodeIter!=mReverseMap.end()) {
                    if((nOutputBlockIdx%N)==0)
                        vvOutput.resize(vvOutput.size()+1);
                    ((T*)vvOutput.data())[nOutputBlockIdx++]=pMapCodeIter->second;
                    vCurrString.clear();
                }
                if(nStringIdx<oInput.string.size())
                    vCurrString.push_back(oInput.string[nStringIdx]);
                ++nStringIdx;
            }
            CV_Assert(vCurrString.empty() && (nOutputBlockIdx%N)==0);
            return vvOutput;
        }

    } // namespace Huff

    void conv_rgb2ycbcr(const cv::Mat& RGB, bool bSubsample, cv::Mat_<uchar>& Y, cv::Mat_<uchar>& Cb, cv::Mat_<uchar>& Cr);
    void conv_ycbcr2rgb(const cv::Mat_<uchar>& Y, const cv::Mat_<uchar>& Cb, const cv::Mat_<uchar>& Cr, bool bSubsample, cv::Mat& RGB);
    cv::Mat_<float> dct(const cv::Mat_<uchar>& oBlock);
    cv::Mat_<uchar> dct_inv(const cv::Mat_<float>& oBlock);
    std::vector<cv::Mat_<uchar>> decoup(const cv::Mat_<uchar>& oImage);
    cv::Mat_<uchar> decoup_inv(const std::vector<cv::Mat_<uchar>>& vBlocks, const cv::Size& oImageSize);
    cv::Mat_<short> quantif(const cv::Mat_<float>& oBlock, int nQualityFactor=10);
    cv::Mat_<float> quantif_inv(const cv::Mat_<short>& oBlock, int nQualityFactor=10);
    std::array<short,s_nVectorSize> zigzag(const cv::Mat_<short>& oBlock);
    cv::Mat_<short> zigzag_inv(const std::array<short,s_nVectorSize>& aZigzag);
    cv::Point2i match_block(const cv::Mat_<uchar>& oBlock, cv::Point2i oBlockPos, cv::Rect oSearchRegion, const cv::Mat_<uchar>& oSearchImage, double& dEQM_min);

    template<typename T, int nChannels=1>
    inline bool isEqual(const cv::Mat& a, const cv::Mat& b) {
        if(a.empty() && b.empty())
            return true;
        if(a.dims!=b.dims || a.size!=b.size || a.type()!=b.type())
            return false;
        CV_Assert(a.total()*a.elemSize()==b.total()*b.elemSize() && a.dims==2);
        for(int i=0; i<a.rows; ++i)
            for(int j=0; j<a.cols; ++j)
                for(int c=0; c<nChannels; ++c)
                    if(a.ptr<T>(i,j)[c]!=b.ptr<T>(i,j)[c])
                        return false;
        return true;
    }

    template<typename T, size_t nChannels=1>
    inline bool isNearlyEqual(const cv::Mat& a, const cv::Mat& b, double dMaxDiff) {
        if(a.empty() && b.empty())
            return true;
        if(a.dims!=b.dims || a.size!=b.size || a.type()!=b.type())
            return false;
        CV_Assert(a.total()*a.elemSize()==b.total()*b.elemSize() && a.dims==2);
        for(int i=0; i<a.rows; ++i)
            for(int j=0; j<a.cols; ++j)
                for(int c=0; c<nChannels; ++c)
                    if(std::abs(double(a.ptr<T>(i,j)[c])-double(b.ptr<T>(i,j)[c]))>dMaxDiff)
                        return false;
        return true;
    }

} // namespace tp2