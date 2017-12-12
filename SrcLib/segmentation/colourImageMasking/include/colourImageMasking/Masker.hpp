/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2017.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __COLOURIMAGEMASKING_MASKER_HPP__
#define __COLOURIMAGEMASKING_MASKER_HPP__

#include "colourImageMasking/config.hpp"

#include <opencv2/ml.hpp>
#include <opencv2/opencv.hpp>

namespace colourImageMasking
{

/**
 * @brief Define supported color spaces
 */
enum ColSpace
{
    BGR,
    HSv,
    lAB,
    yCrCb
};

/**
 * @brief Define supported image masking mode
 */
enum DetectionMode
{
    fgLL, /// Using only a foreground likelihood color model
    bgLL, /// Using only a background likelihood color model
    LLRatio /// Using a ratio between foreground and background color model
};

/**
 * @brief Class performing OpenCV Expectation Maximization segmentation after a learning step of two color models. One
 * for the foreground objects that we need to segment and a second one for the background.
 *
 * @see Documentation from OpenCV https://docs.opencv.org/master/dc/dd6/ml_intro.html#ml_intro_em
 */
class COLOURIMAGEMASKING_CLASS_API Masker
{

public:

    /// Constructor
    COLOURIMAGEMASKING_API Masker(const ColSpace& c, const DetectionMode& d);

    /// Destructor
    COLOURIMAGEMASKING_API ~Masker();

    /// Train the foreground color model defined with a number of clusters inside a mask on a given image
    COLOURIMAGEMASKING_API void trainForegroundModel(const ::cv::Mat& rgbImg, const ::cv::Mat& selectionMask,
                                                     const unsigned int numClusters, const double noise = 0.0);

    /// Train the background color model defined with a number of clusters inside a mask on a given image
    COLOURIMAGEMASKING_API void trainBackgroundModel(const ::cv::Mat& rgbImg, const ::cv::Mat& selectionMask,
                                                     const unsigned int numClusters);

    /// Perform an image masking based on the learned model on a downscaled image inside a given mask
    COLOURIMAGEMASKING_API ::cv::Mat makeMask(const ::cv::Mat& testImg, const ::cv::Size& downSize,
                                              ::cv::InputArray filterMask) const;

    /// Set threshold value to get final binary image
    COLOURIMAGEMASKING_API void setThreshold(double t);

    /// Return if a model is learned
    COLOURIMAGEMASKING_API bool isModelLearned(void);

private:

    /// Make a response mask from a model on a given image inside a mask
    static ::cv::Mat makeResponseImage(const ::cv::Mat& inImg, const ::cv::Ptr< ::cv::ml::EM > model,
                                       ::cv::Mat& filterMask);

    /// Convert the colorspace of an image
    static ::cv::Mat convertColourSpace(const ::cv::Mat& src, const ColSpace& c);

    /// Train the model from samples
    static ::cv::Ptr< ::cv::ml::EM > trainModelFromSamples(const ::cv::Mat& samples, const unsigned int numClusters);

    /// Get samples of an image inside a mask to train the model
    static ::cv::Mat makeTrainingSamples(const ::cv::Mat& trainImg, const ::cv::Mat& mask, const ColSpace& c);

    /// OpenCV operations to remove holes in the mask
    static ::cv::Mat removeMaskHoles(const ::cv::Mat& m, size_t n, ::cv::InputArray insideMask);

    /// Foreground and background models
    ::cv::Ptr< ::cv::ml::EM > m_foregroundModel;
    ::cv::Ptr< ::cv::ml::EM > m_backgroundModel;

    /// Colorspace and detection mode of the image masking model
    const ColSpace m_COLORSPACE;
    const DetectionMode m_DETECTIONMODE;

    /// Threshold of image masking model to get binary mask
    double m_threshold;

    /// Store if the threshold is set
    bool m_hasSetThreshold;

    /// Morphological element type
    static const int s_MORPHTYPE = ::cv::MORPH_ELLIPSE;

    /// Morphological element size
    static const int s_MORPHSIZE = 1;

    /// Morphological element
    static const ::cv::Mat s_MORPHELEMENT;

};

}

#endif // __COLOURIMAGEMASKING_MASKER_HPP__