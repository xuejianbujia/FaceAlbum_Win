// Copyright Soyeez Technology Inc. 2013 All rights reserved.

#ifndef ALLEN_CACHEDDESCRIPTOR_H_
#define ALLEN_CACHEDDESCRIPTOR_H_

#include "opencv/cv.h"

// Declaration of HOGCache struct hidden in the opencv objdetect library.
namespace cv {
struct HOGCache
{
    struct BlockData
    {
        BlockData() : histOfs(0), imgOffset() {}
        int histOfs;
        Point imgOffset;
    };

    struct PixData
    {
        size_t gradOfs, qangleOfs;
        int histOfs[4];
        float histWeights[4];
        float gradWeight;
    };

    HOGCache();
    HOGCache(const HOGDescriptor* descriptor,
        const Mat& img, Size paddingTL, Size paddingBR,
        bool useCache, Size cacheStride);
     virtual ~HOGCache() {};
     virtual void init(const HOGDescriptor* descriptor,
        const Mat& img, Size paddingTL, Size paddingBR,
        bool useCache, Size cacheStride);

    Size windowsInImage(Size imageSize, Size winStride) const;
    Rect getWindow(Size imageSize, Size winStride, int idx) const;

    const float* getBlock(Point pt, float* buf);
    virtual void normalizeBlockHistogram(float* histogram) const;

    vector<PixData> pixData;
    vector<BlockData> blockData;

    bool useCache;
    vector<int> ymaxCached;
    Size winSize, cacheStride;
    Size nblocks, ncells;
    int blockHistogramSize;
    int count1, count2, count4;
    Point imgoffset;
    Mat_<float> blockCache;
    Mat_<uchar> blockCacheFlags;

    Mat grad, qangle;
    const HOGDescriptor* descriptor;
};

}  // namespace cv

namespace allen_facemarks {
// A cached implementation of HOG descriptor so that multiple calls of compute
// will reuse the HOGCache computed at initialization time.
class CachedDescriptor {
 public:
  CachedDescriptor();
  virtual ~CachedDescriptor();
  // Initialize the cached descriptor with the input image, pre-compute all the
  // necessary part. This should be call once per image.
  void Init(const cv::Mat& img,
            const cv::HOGDescriptor* hog_descriptor,
            cv::Size winStride = cv::Size(),
            cv::Size padding = cv::Size());

  // Multiple calls to ComputeHOG will reuse the same HOGCache computed in
  // Init().
  void ComputeHOG(
      std::vector<float>& descriptors,
      const std::vector<cv::Point>& locations = std::vector<cv::Point>());

  cv::Size GetWinSize() {
    return hog_descriptor_->winSize;
  }

  int GetDescriptorSize() {
    return hog_descriptor_->getDescriptorSize();
  }

 private:
  cv::HOGCache hog_cache_;
  cv::Size padded_img_size_;
  cv::Size win_stride_;
  cv::Size padding_;
  cv::Size cache_stride_;
  int img_cols_;
  int img_rows_;
  const cv::HOGDescriptor* hog_descriptor_;
};

}  // namespace allen_facemarks

#endif /* CACHEDDESCRIPTOR_H_ */
