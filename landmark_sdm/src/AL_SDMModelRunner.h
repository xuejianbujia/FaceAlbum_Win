#ifndef ALLEN_SDMMODEL_RUNNER_H_
#define ALLEN_SDMMODEL_RUNNER_H_

#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <opencv2\core\core.hpp>

#include "AL_SDMModel.h"

#include "al_eyecenter_location.h"


namespace allen_facemarks
{
	typedef struct Eyestr
	{
		cv::Point eye_left;
		cv::Point eye_right;
	}Eyestr;

class SDMModelRunner
{
public:

	SDMModelRunner();
	
	SDMModelRunner(const SDMParams &front, const SDMParams &left, const SDMParams &right);
	
	static SDMModelRunner* GetRunner(const string &frontModel="", const string &leftModel="", const string &rightModel="");

	~SDMModelRunner();

	bool IsGood();

	/** Find the face's 95 key points.
	* image:		<input>  input face image.
	* rects:		<input>  the face locations.
	* pose:			<input>  the face pose, 0->front, 1->left, 2->right.
	* keyPoints:	<output> the return key points.
	*/
	int Run(const cv::Mat &image, const std::vector<cv::Rect> &rects, const std::vector<int> &pose, std::vector< std::vector<cv::Point2f> > &keyPoints);

	int GetEyePosition(const vector<cv::KeyPoint> &points,cv::Point &Eye_left,cv::Point &Eye_right);
	int GetEyePosition(const vector <cv::Point2f> &points,cv::Point &Eye_left,cv::Point &Eye_right);
	
	public:
	void  LocateEye(const cv::Mat &mat_img,const std::vector<cv::Rect> &face_rects,vector<Eyestr> &eyes ) ;
	bool LoadModel();



public:

	SDMModel front_sdm_;
	SDMModel left_sdm_;
	SDMModel right_sdm_;

	bool models_loaded_;
	ASEL_ENGINE   pEngine;
};

} // namespace allen_facemarks



#endif // SDMMODEL_RUNNER_H_