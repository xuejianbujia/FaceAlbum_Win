#ifndef _CGT_FACE_TYPE_H_
#define _CGT_FACE_TYPE_H_

// #if defined(__cplusplus)
// extern "C"
// {
// #endif

	typedef struct _cgt_face_ 
	{
		int ulx;
		int uly;
		int urx;
		int ury;
		int brx;
		int bry;
		int blx;
		int bly;
		int pose_angle[3];  /*YPR*/
		int size;
		int confidence;
		unsigned char quality;  /*Value 0 for manually labeled; 1~16 for automatic detection with 1 is the highest; 255 for unknown quality;*/
		
	} CGT_FACE;
	
	typedef struct _cgt_eye_ 
	{
		int xleft;
		int yleft;
		int xright;
		int yright;
		int confidence;  /*[50.0, 100.0]*/
		unsigned char l_quality;  /*Value 0 for manually labeled; 1~16 for automatic detection with 1 is the highest; 255 for unknown quality;*/
		unsigned char r_quality;  /*Value 0 for manually labeled; 1~16 for automatic detection with 1 is the highest; 255 for unknown quality;*/
		
	} CGT_EYE;
	
	typedef	struct _cgt_face_quality_ 
	{
		int		b_frontal; /*	0: nofrontal; 1: frontal */
		float	deviation_frontal; /* for cgn */
		float	pose_angle_roll; /*	degree of roll angle */
		float	pose_yaw_val;	 //	left | frontal | right
		float	pose_yaw_sym;	 //	the	bigger -> frontal
		float	pose_pitch_val;	 //	the	bigger -> non-frontal

		int		b_eye_open;	/*0x00:	close; 0x01: open*/
		float	re_open;
		float	le_open;
		int		b_mouth_close; /*0x00: open; 0x01: close*/
		float	mouth_close;

		int		b_only_one_face; /*0x00: no; 0x01: yes*/
		int		num_face;

		int		b_light; /*	0: bad;	1: good	*/
		float	deviation_light;
		int		b_no_eye_shadow;
		float	no_eye_shadow;
		int		b_no_hot_spot; /* 0: bad; 1: good */
		float	no_hot_spot;

		int		b_glasses; /* 0x00:	without; 0x01: with*/
		float	glasses;
		int		b_no_tinted_glasses; /* 0: bad; 1: good */
		float	re_tinted;
		float	le_tinted;

		int		b_exposure; /* 0:	bad; 1:	good */
		float	exposure;

		int		b_sharp; /* 0: bad; 1: good */
		float	sharp;

		int		b_h_centred;
		int		b_v_pos;

		int		b_width_head;
		int		b_length_head;

		int		b_gray_scale_prof; /*	0: bad;	1: good	*/
		int		gray_scale_density;

		int		b_natural_skin_color;
		float	natural_skin_color;
		int		b_not_red_eye; /*0x00: no; 0x01: yes*/
		float	re_red_eye;
		float	le_red_eye;

		int		b_background;
		float	background;

		int		b_resolution;
		int		b_w_h_ratio; /* 0: bad; 1: good */
		
		int		reserve[21];	// padding to 64 * 4
		
	} CGT_FACE_QUALITY;
	
	typedef struct _cgt_face_quality_cfg_
	{
		float eye_open_th;              /* [-0.3, 0.3], default is 0 */
		float mouth_close_th;           /* [], default is 0 */
		float low_exposure_th;          /* [60, 90], default is 76 */
		float high_exposure_th;         /* [170, 210], default is 190 */
		int   pose_roll_th;             /* [3, 20], default is 5 */
		int   pose_dev_level;           /* [0, 100], default is 50 */
		int   pose_left_level;          /* [80, 120], default is 100 */
		int   pose_right_level;         /* [80, 120], default is 100 */
		int   pose_up_level;            /* [0, 100], default is 23 */
		int   pose_down_level;          /* [0, 100], default is 30 */
		int   pose_sym_level;           /* [0, 1400], default is 1050 */
		int   min_grayscale_density;    /* [100, 156], default is 128 */
		float uniform_lighting_th;      /* [0.1, 0.2], default is 0.15 */
		float uniform_lighting_th_hl;   /* [0.0, 10.0], default is 2.0 */
		float uniform_lighting_th_ar;   /* [0.0, 0.5], default is 0.015 */
		float min_width_of_head_ratio;  /* [1.0, 1.7], default is 1.333 */
		float max_width_of_head_ratio;  /* [1.7, 3.0], default is 2.0 */
		float min_length_of_head_ratio; /* [1.0, 1.34], default is 1.111 */
		float max_length_of_head_ratio; /* [1.34, 3.0], default is 1.667 */
		float min_width_height_ratio;   /* [1.0, 1.3], default is 1.25 */
		float max_width_height_ratio;   /* [1.3, 2.0], default is 1.34 */
		float min_eye_distance;         /* [60, 120], default is 90 */
		float min_vertical_position;    /* [0.3, 0.6], default is 0.5 */
		float max_vertical_position;    /* [0.6, 0.9], default is 0.7 */
		float min_horizontal_position;  /* [0.3, 0.5], default is 0.45 */
		float max_horizontal_position;  /* [0.5, 0.7], default is 0.55 */
		float with_glasses_th;          /* [], default is 0.0 */
		float sharpness_th;             /* [], default is 0.5 */
		float skin_tone_th;             /* [], default is 0.5 */
		float red_eye_th;               /* [], default is 0.5 */
		float background_th;            /* [], default is 0.5 */
		
		int	  reserve[17];		        /* padding to 48 * 4 */
		
	} CGT_FACE_QUALITYCFG;

	typedef struct _cgt_face_trackstate_
	{
		int		nMoveFast;             //  0 stable, 1 unstable -1 unknown
		int		nFaceSize;             //  0 good, 1 too far, 2 too close
		int		nIntentLog;		       //  1 logon 0 unknown
		int		nTooDark;              //  0 normal, 1 too dark
		int		nROIL, nROIT, nROIR, nROIB;
		int		nROIMean, nROIVar;
		int		pnReserved[94];			   /* padding to 16 * 4 */
			
	}  CGT_FACE_TRACKSTATE;

	typedef struct CGT_FACE_LANDMARK_POINT 
	{
		float x;
		float y;
		float confidence;
		int   reserve;
	}CGT_FACE_LANDMARK_POINT;
	
	typedef struct CGT_FACE_LANDMARK_ORGAN
	{
		int   index;
		int   count;
		float confidence;
		int   reserve;	
	}CGT_FACE_LANDMARK_ORGAN;
	
	#define CGTFACE_LANDMARK_POINT_MAXNUM 128
	#define CGTFACE_LANDMARK_ORGAN_MAXNUM 16
	
	enum CGT_FACE_LANDMARK_ORGAN_INDEX
	{
		CGT_FACE_LANDMARK_ORGAN_LEYE,		//0
		CGT_FACE_LANDMARK_ORGAN_REYE,		//1
		CGT_FACE_LANDMARK_ORGAN_LEYEBROW,	//2
		CGT_FACE_LANDMARK_ORGAN_REYEBROW,	//3
		CGT_FACE_LANDMARK_ORGAN_NOSE,		//4
		CGT_FACE_LANDMARK_ORGAN_MOUTH,		//5
		CGT_FACE_LANDMARK_ORGAN_CONTOUR		//6
	};
	
	typedef struct _cgt_face_landmark_ 
	{
		CGT_FACE_LANDMARK_POINT pPoint[CGTFACE_LANDMARK_POINT_MAXNUM];
		CGT_FACE_LANDMARK_ORGAN pOrgan[CGTFACE_LANDMARK_ORGAN_MAXNUM];
		int iPointNum;
		int iOrganNum;
		float confidence;
		int reserve[9];
	}CGT_FACE_LANDMARK;
// #if defined(__cplusplus)
// }
// #endif

#endif	// _CGT_FACE_TYPE_H_
