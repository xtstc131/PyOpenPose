//
// Created by padeler on 21/6/2017.
//

#ifndef PYOPENPOSE_OPENPOSEWRAPPER_H
#define PYOPENPOSE_OPENPOSEWRAPPER_H

#include <string>
#include <vector>
#include <opencv/cv.h>
#include <memory>
#include <opencv2/opencv.hpp>



class OpenPoseWrapper {


public:
    typedef std::shared_ptr<OpenPoseWrapper> Ptr;
    typedef std::vector<cv::Mat> KeypointGroups;

    enum class ScaleMode : unsigned char
    {
        InputResolution,
        NetOutputResolution,
        OutputResolution,
        ZeroToOne, // [0, 1]
        PlusMinusOne, // [-1, 1]
        UnsignedChar, // [0, 255]
    };

    enum KeypointType { POSE, FACE, HAND };

    /**
     * Openpose wrapper constructor.
     * @param netPoseSize Body pose network input size
     * @param netFaceHandsSize Face and Hands networks input size
     * @param outputSize The output joint positions is rescaled to this size.
     * @param model The model to be used to the body pose estimation
     * @param modelFolder Folder containing the models for body pose, hands and faces (open pose models folder)
     * @param logLevel Openpose loglevel parameter
     * @param downloadHeatmaps If False the heatmaps wont be downloaded from GPU mem.
     * @param heatMapScaleMode The scale mode of the output heatmaps @see ScaleMode
     * @param withFace Initialize the network for face pose detection
     * @param withHands Initialize the network for hand pose detection
     */
    OpenPoseWrapper(const cv::Size &netPoseSize = cv::Size(320, 240), const cv::Size &netFaceHandsSize = cv::Size(128,128),
                    const cv::Size &outputSize = cv::Size(640,480),
                    const std::string &model="COCO", const std::string &modelFolder="models/", const int logLevel=255,
                    bool downloadHeatmaps=false,
                    OpenPoseWrapper::ScaleMode heatMapScaleMode = OpenPoseWrapper::ScaleMode::ZeroToOne,
                    bool withFace=true, bool withHands=true);

    /**
     * Detect body poses in given image
     * @param rgb input image
     */
    void detectPose(const cv::Mat &rgb);

    /**
     * Detect faces in given image.
     * You need to call the @refitem detectPose with the same image first.
     *
     * @param rgb input image
     */
    void detectFace(const cv::Mat &rgb);

    /**
     * Detect hands in given image.
     * You need to call the @refitem detectPose with the same image first.
     *
     * @param rgb input image
     */
    void detectHands(const cv::Mat &rgb);

    /**
     * Uses the visualization tools of Openpose to render detected keypoints on the input rgb image
     * @param rgb input image
     * @return the input image with the keypoints and skeletons rendered on it.
     */
    cv::Mat render(const cv::Mat &rgb);

    /**
     * Returns the keypoints of a given type.
     * @param t the keypointtype @see KeypointType
     * @return A vector of cv::Mat. The vector has a single entry for t=POSE and t=FACE and two entries for t==HAND.
     * For hands the first entry is a Mat with all left hands, and the second is a Mat with all the right hands.
     * The dimensions of each mat is NxKx3. N is the number of item detected (i.e number of FACEs if t=FACE).
     * K is the number of keypoitns (i.e 21 if t=HAND). The last dimension is the X,Y coordinates of the keypoint and
     * its score.
     */
    KeypointGroups getKeypoints(KeypointType t=POSE);

    /**
     * Returns the heatmaps and PAFs generated by the POSE network.
     * @return a multichannel cv::Mat, each channel corresponds to a different headmap. Each PAF is represented by two
     * consecutive channels one for each dimension of the PAF.
     */
    cv::Mat getHeatmaps();


private:
    struct PrivateData;
    std::shared_ptr<PrivateData> membersPtr;
    bool withFace, withHands;

};


#endif //PYOPENPOSE_OPENPOSEWRAPPER_H
