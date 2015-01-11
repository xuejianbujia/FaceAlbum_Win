// Copyright Soyeez Technology Inc. 2013 All rights reserved.

#ifndef ALLEN_SDMMODEL_H_
#define ALLEN_SDMMODEL_H_

#include <vector>
#include <string>

#include "opencv/cv.h"

//#define USE_OPENCV_NON_FREE
#ifdef USE_OPENCV_NON_FREE
#include "opencv2/nonfree/nonfree.hpp"
#endif

using std::string;
using std::vector;




namespace allen_facemarks 
{

	class CachedDescriptor;

	// SDM related parameters.
	class SDMParams {
	public:
		SDMParams();
		~SDMParams();
		// The full path to the model.
		string model_path;
		// Maximum number of iterations.
		int max_iter;
		// Whether to project the estimated shape back to PCA eigenvectors.
		bool project_result;
		// Whether to reduce the ground truth annotation to a smaller set.
		bool reduce_points;
		// Whether to use opencv detector
		string face_detector_model_path;
		bool show_debug_window;
		string debug_dir;
		// The kernel used to blur image
		int blur_kernel;
		// Training parameters.
		float perturbed_bbox_ratio;
		float perturbed_angle;
		int perturb_count;
		bool add_zero_constraints;
		float pca_energy;

		// Parsing the commmand line flags into parameters.
		int SetParameters(string modelname,int MaxIter);
		
	};

	class SDMModel {
	public:
		SDMModel();
		

		SDMModel(const SDMParams& p);

		void SetParameters(const SDMParams& p);


		~SDMModel();
		

		// Load an SDM model from file, file path specified in SDMParams passed in the
		// constructor.
		bool LoadModel();


		//cv::Rect ComputeBoundingBox_ByEye(const cv::Point Eye_left,const cv::Point Eye_right,const int Width,const int Height,int RandomFlag);
		//float RangedRandDemo(float fMin,float fMax);
		//cv::Rect PredictDetectorBox(const cv::Rect& face_bbox);
		// Crop the face image out with a margin and make sure the resolution is
		// correct for raw feature extraction and landmark localization. The cropped
		// out image can be used in FindFaceMarks.
		// Output:
		//   normalized_image -- Cropped and rescaled face image that matches the SDM
		//       model.
		//   init_points -- The initial landmark locations needed in FindFaceMarks().
		//   restore_shift -- The shift needed to restore the landmarks back to the
		//      coordinates of the original image.
		//   restore_scale -- The scaling needed to restore the landmarks back to the
		//      coordinates of the original image. To transform the point coordinates
		//      back to the original image, use
		//        X_o = X_c * restore_scale + restore_shift
		//      X_o are the coordinates of the original image.
		//      X_c are the coordinates of the cropped image.
		//   s0_shift -- The shift needed to transform the points to the coordinates
		//       of the shape model.
		void GetNormalizedImage(const cv::Mat& image,
			const cv::Rect& face_rect,
			cv::Mat* normalized_image,
			vector<cv::KeyPoint>* init_points,
			cv::Point2f* restore_shift,
			float* restore_scale,
			cv::Point2f* s0_shift) const;

		// Iterate a number of times based on the model starting from the initial
		// points.
		// Output:
		//   final_key_points -- the final result of the SDM iterations.
		void FindFaceMarks(const cv::Mat& cropped_image,
			vector<cv::KeyPoint>& init_key_points,
			const cv::Point2f& s0_shift,
			const int iterations,
			vector<cv::KeyPoint>* final_key_points) const;


		void GetNormalizedImage_frontal(const cv::Mat& image,
			const cv::Point Eye_left,const cv::Point Eye_right,
			cv::Mat* normalized_image,
			vector<cv::KeyPoint>* init_points,
			cv::Point2f* restore_shift,
			float* restore_scale,
			cv::Point2f* s0_shift) const;

		// Iterate a number of times based on the model starting from the initial
		// points.
		// Output:
		//   final_key_points -- the final result of the SDM iterations.
		void FindFaceMarks_frontal(const cv::Mat& cropped_image,
			vector<cv::KeyPoint>& init_key_points,
			const cv::Point2f& s0_shift,
			const int iterations,
			vector<cv::KeyPoint>* final_key_points) const;

		// The following methods are mainly used to support the training of SDM model.
		// Save the trained model to model_path in params_.
		bool SaveModel();

		// Extracts the feature descriptors from image.
		// Output:
		//   descriptors -- each row is a descriptor.
		void ExtractFeature(const cv::Mat& image,
			vector<cv::KeyPoint>& key_points,  // Opencv requires this.
			cv::Mat* descriptors) const ;

		// Extracts the feature descriptors using cached descriptor.
		void ExtractFeatureCached(
			const cv::Mat& image,
			vector<cv::KeyPoint>& key_points,  // Opencv requires this to be nonconst.
			CachedDescriptor* cached_desc,
			cv::Mat* descriptors) const ;

		// Clear the PCA matrix in the model.
		void ClearPCA();

		// Different descriptor types supported by the SDMModel.
		enum {
			kDescriptorORB = 1,
			kDescriptorSIFT = 2,
			kDescriptorSURF = 3,
			kDescriptorFREAK = 4,
			kDescriptorSURF64 = 5,
			kDescriptorHOG = 6,
			kDescriptorHOG4x4 = 7,
			kDescriptorHOG7 = 8,
			kDescriptorHOG5 = 9,
			kDescriptorHOG5GridSamples = 10,
			kDescriptorHOG5GridSamples3x3 = 11,
			kDescriptorHOG5GridSamples1x1 = 12,
			kDescriptorHOG80GridSamples3x3 = 13,
		} FeatureType;

		// Accessors.
		int GetFeatureType() const {
			return feature_type_;
		}

		const string& GetModelName() const {
			return params_.model_path;
		}

		int GetMaxIter() const {
			return params_.max_iter;
		}

		const SDMParams& GetParams() const {
			return params_;
		}

		int GetFaceSize() const {
			return face_size_;
		}

		int GetVertNum() const {
			return vert_num_;
		}

		void AddNewModelMatrix(const cv::Mat& x_mat) {
			models_.push_back(x_mat);
		}

		int GetDescriptorSize() const {
			return descriptor_size_;
		}

		public:
		
		
		// Extract HOG descriptors at the location specified by key_points.
		void ExtractHOG(const cv::HOGDescriptor& hog_extractor,
			const cv::Mat& image,
			const vector<cv::KeyPoint>& key_points,
			cv::Mat* descriptors) const;

		// Extract both HOG descriptors and raw samples.
		void ExtractHOGAndGridSamples(const cv::Mat& image,
			const vector<cv::KeyPoint>& key_points,
			int sample_count,
			int sample_spacing,
			CachedDescriptor* cached_desc,
			cv::Mat* descriptors) const;

		void ProjectShapeBackToBasis(const cv::Point2f& s0_shift,
			vector<cv::KeyPoint>* key_points) const;


		SDMParams params_;

		

		// Models for each iteration.
		vector<cv::Mat> models_;

		// Different descriptor extractors:
	//	mutable cv::ORB orb_extractor_;
	//	cv::FREAK freak_extractor_;
		cv::HOGDescriptor hog_extractor_;
		cv::HOGDescriptor hog4x4_extractor_;
		cv::HOGDescriptor hog7_extractor_;
		cv::HOGDescriptor hog5_extractor_;
		cv::HOGDescriptor hog80_extractor_;
#ifdef USE_OPENCV_NON_FREE
		cv::SIFT sift_extractor_;
		cv::SURF surf_extractor_;
		cv::SURF surf64_extractor_;
#endif

		// The mean shape in the normalized detection window.
		vector<float> s0_;
		cv::Rect s0_bbox_;
		// The shape basis.
		cv::Mat s_star_;
		int shape_mode_;

		// The descriptor pca;
		cv::PCA descriptor_pca_;

		int vert_num_;
		int feature_type_;
		int face_size_;
		int descriptor_size_;
	};

} /* namespace allen_facemarks */
#endif /* SDMMODEL_H_ */
