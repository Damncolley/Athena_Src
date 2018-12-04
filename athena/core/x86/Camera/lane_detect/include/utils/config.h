#ifndef _CONFIG_H_
#define _CONFIG_H_

//#define	FOR_FC2
//#define	FOR_TMC
//#define	KATAOKA_LOGIC

#define	USE_CALC_PROCLINE
#define	NOT_USE_DISPARITY
//#define	FORCE_UseDisparity
//#define	NOT_USE_ADJUST_EDGE_THRESHOLD
#define	NOT_USE_FARAREA
//#define	SIMPLE_CALC_EDGE_STRENGTH
//#define	NOT_USE_selectLaneMarkerAtComplexLaneBoundary
//#define	USE_checkMismatchOfLaneMarkerPointsFromLaneParameter
//#define	USE_BB_DIFF
#define	USE_NON_DISPARITY_EDGEPOINT
//#define	NOT_USE_CONSTRANT_OF_GAP_BETWEEN_EDGE_POINT
#define	NOT_USE_GRIDMAP1D

//#define	FOR_WTP
/////////////////////////////////////////////////////////////////////////////////
#define	NOT_USE_TMC_STEREO_BMP
//#define	USE_TYTLABS_STEREO_AVI
//#define	FOR_DEMONSTRATION1
//#define	FOR_DEMONSTRATION2
//#define	LONG_FOCAL_LENGTH

//#define	FOR_TME
//#define	WINDOWS_DEBUG_MODE
//#define	NOT_USE_ROAD_WINDOW
//#define	NOT_USE_CONTROL_WINDOW
#define	NOT_USE_CONSOLE
#define	NOT_USE_LISTWINDOW

//#define	OVERLAY_ROADIMAGE_ON_INPUTIMAGE
//#define	USE_DYNAMIC_SYSTEM_NOISE
#define	PROCESS_PRIORITY	5

#define	SCALE_TO_DRAW_POINT	1

//#define	LONG_FOCAL_LENGTH

#define	SAFE_DELETE(p)	{	if(p != NULL)	{	delete p;	p = NULL;	}	}
#define	SAFE_DELETE_ARRAY(p)	{	if(p != NULL)	{	delete [] p;	p = NULL;	}	}

#define	GM_MOVINGAVERAGE_NUMBER	10
#define	GM_MOVINGAVERAGE_INVALID_VALUE	(1000000)


#define	ACC_FAC		100
#define	NO_STEER_SIGNAL
#define FIND_WIDE_LANE

#ifndef	M_PI
#define	M_PI	(3.141592)
#endif


//yuhuan 10.19
//#define	DB_REAL_FILTER_LENGTH	(80)
#define	DB_REAL_FILTER_LENGTH	(80)    //���ؼ���˲�����

// ��Ե�ݶ�������ֵ
#if	defined(FOR_DEMONSTRATION1) || defined(FOR_DEMONSTRATION2)
#define	CS4_POSITIVETHRESHOLD	(S4)64
#define	CS4_NEGATIVETHRESHOLD	(S4)(-(CS4_POSITIVETHRESHOLD))	// upper boundary of down edge strength
#elif	defined(FOR_TME)
#define	CS4_POSITIVETHRESHOLD	(S4)64
#define	CS4_NEGATIVETHRESHOLD	(S4)(-(CS4_POSITIVETHRESHOLD))	// upper boundary of down edge strength
#else	FOR_DEMONSTRATION
#define	CS4_POSITIVETHRESHOLD	(S4)16
#define	CS4_NEGATIVETHRESHOLD	(S4)(-(CS4_POSITIVETHRESHOLD))
#endif	FOR_DEMONSTRATION

#define	CS4_NEAR_BASEY	7000
#define	CS4_NEARHEIGHT	10000	          //(CS4_NEAR_TOPY - CS4_NEAR_BASEY)
#define	CS4_NEAR_TOPY	(CS4_NEAR_BASEY + CS4_NEARHEIGHT)	//17000
#define	CS4_NEARIMAGEHEIGHT		100      //����ͼ��߶�
#define	CS4_NEARWIDTH			50000    //ʵ�ʵ�·������
#define	CS4_NEARIMAGEWIDTH		962      //����ͼ����
//#define	CS4_NEAR_MAX_YAW	22//50
//#define	CS4_NEAR_MAX_YAW	44	// 20100831

#if	!defined(FOR_DEMONSTRATION2)
#define	CS4_NEAR_MAX_YAW	88	// 20111107
#else	!defined(FOR_DEMONSTRATION2)
#define	CS4_NEAR_MAX_YAW	44	// 20111107
#endif	!defined(FOR_DEMONSTRATION2)

#define	CS4_NUMBER_OF_NEAR_YAW		(CS4_NEAR_MAX_YAW * 2 + 1)
#define	CS4_NUMBER_OF_NEAR_OFFSET	CS4_NEARIMAGEWIDTH
#define	CS4_NUNBER_OF_NEAR_BALLOT	256

#define	CS4_FAR_BASEY	CS4_NEAR_TOPY	//17000
#define	CS4_FARHEIGHT	CS4_NEARHEIGHT	//10000	//(CS4_FAR_TOPY - CS4_FAR_BASEY)

#define	CS4_FAR_TOPY	(CS4_FAR_BASEY + CS4_FARHEIGHT)	//27000
#define	CS4_FARIMAGEHEIGHT		CS4_NEARIMAGEHEIGHT	//100
#define	CS4_FARWIDTH			CS4_NEARWIDTH	//10000
#define	CS4_FARIMAGEWIDTH		CS4_NEARIMAGEWIDTH	//192

#define	CS4_FARAREA_TOPY	80000
#define	CS4_WEIGHT_MAX	1

#define	CS4_MINIMUM_VOTES		(CS4_WEIGHT_MAX * 30)	// 20110630
#define	VOTING_THRESHOLD_RATIO	5

/////////////////////////////////
//#define	CS4_MAXIMUM_LINEWIDTH	500
#define	CS4_MAXIMUM_LINEWIDTH	1000	// 2011124
//TODO 100
#define	CS4_NEAR_MINIMUM_LINEWIDTH	80	// 20110704

#define	V_V_M	1
#define	FALSE	0
#define	TRUE	1

#define CS4_NOISE	1

#define	DB_OFFSET_MAX			(2000)
#define	DB_OFFSET_MIN			(-2000)
#define	DB_YAW_MAX			(45.0 / 180 * M_PI)
#define	DB_YAW_MIN			(-DB_YAW_MAX)
#define	DB_CURVATURE_MAX		(5e-5)
#define	DB_CURVATURE_MIN		(-5e-5)

#ifdef FIND_WIDE_LANE
#define	DB_LW_MAX			(8000)
#else FIND_WIDE_LANE
#define	DB_LW_MAX			(4000)
#endif FIND_WIDE_LANE

#define	DB_LW_MIN			(2000)

#define	DB_PITCH_MAX	(5.0 / 180. * M_PI)
#define	DB_PITCH_MIN	(-DB_PITCH_MAX)

#define	CDB_OFFSET_SCALE	(1.)
#define	CDB_YAW_SCALE		(1.)
#define	CDB_C_SCALE			(1.)
#define	CDB_LW_SCALE		(1.)
#define	CDB_DP_SCALE		(1.)

#define	CS4_LW_LSB	(1.)
#define	CS4_DP_LSB	(10000.)
#define	CS4_CURVATURE_LSB	(100000. * 1000)


// 20110928
#define	DEFAULT_SEARCH_MARGIN_IN_NEAR_AREA	(300.)
#define	DEFAULT_SEARCH_MARGIN_IN_FAR_AREA	(300.)
//#define	DEFAULT_MAX_DIFF_FROM_MEDIAN_DEVIATION_IN_NEAR_AREA (100.)
//#define	DEFAULT_MAX_DIFF_FROM_MEDIAN_DEVIATION_IN_NEAR_AREA (50.)	// 20111007
//#define	DEFAULT_MAX_DIFF_FROM_MEDIAN_DEVIATION_IN_FAR_AREA	(100.)
#define	DEFAULT_MAX_DIFF_FROM_MEDIAN_DEVIATION_IN_NEAR_AREA (50.)	// 20111121
#define	DEFAULT_MAX_DIFF_FROM_MEDIAN_DEVIATION_IN_FAR_AREA	(50.)	// 20111121

#define	DB_OFFSET_AVE		(0)
#define	DB_YAW_AVE			(0)
#define	DB_CURVATURE_AVE	(0)
#define	DB_LW_AVE			(3500)
#define	DB_DP_AVE			(0)

#define	CDB_SIGMA_PSI	(0.0)	// 20111026
#define	CDB_SIGMA_PSIDASH	(0.5 / 180 * M_PI)

#define	CDB_SIGMA_E			(0.0)// [mm]	// 20111026
#define	CDB_SIGMA_EDASH	(1.0 * 1000)	// [mm/s]

#define	CDB_SIGMA_RHO	(0.0)	// [1/mm]	// 20111026
//#define	CDB_SIGMA_RHODASH	(1.0e-6 * 1.0e-6)	// [1/mm^2]// 20111025
#define	CDB_SIGMA_RHODASH	(1.0e-5 * 1.0e-6)	// [1/mm^2]// 20111026

#define	CDB_SIGMA_DELTAPHI	(1.0 / 180 * M_PI)
#define	CDB_SIGMA_WIDTH	(1.0 * 1000)	// [mm]

// configuration
#define	MINIMUM_CV	(0.0001)
#define	MINIMUM_AVERAGE_EDGE_STRENGTH_RATIO	(0.5)
//#define	MAXMIMUM_DIFF_OF_ENDPOINT	(5000.)
#define	MAXMIMUM_DIFF_OF_ENDPOINT	(1000.)
//#define	MINIMUM_RATIO_OF_POINTNUM	(0.5)
//#define	MINIMUM_RATIO_OF_POINTNUM	(0.8)	// 20111121
#define	MINIMUM_RATIO_OF_POINTNUM	(0.7)
#define	MINIMUM_RATIO_OF_VOTES	(0.5)
#define	MINIMUM_FILTER_LENGTH	(2)
#define	MAXIMUM_YAW_OF_LANEMARKERLINESEQUENCE	(20)
//TODO 3. / 8
#define	MINIMUM_OFFSET_FOR_LANEMARKERLINESEQUENCE_INHIBIT	(4. / 8)
//TODO 6. / 8
#define	MAXIMUM_OFFSET_FOR_LANEMARKERLINESEQUENCE_INHIBIT	(5. / 8)
//TODO 50
#define	MAXIMUM_HORIZONTAL_DIFF_AT_REGION_BOUNDARY	(30.)
//TODO 5
#define	MAXIMUM_YAW_DIFF_AT_REGION_BOUNDARY	(50)
#define	OFFSETMARGIN_FOR_LANEMARKERLINE_3D	5
#define	SEARCH_MARGIN_FOR_LANEBOUNDARY	(500.)
///////#define	SEARCH_MARGIN_FOR_YAW	(20)

//#define	INSIDE_OF_LANEBOUNDARY_FOR_LANE_BOUNDARY	(1000.)	//(500.)	//20111103(���g�p)
#define	OUTSIDE_OF_LANEBOUNDARY_FOR_LANE_BOUNDARY	(1500.)
//#define	MARGIN_OFFSET_FOR_LANE_BOUNDARY_TRACKING	(500.)
//TODO 250
#define	MARGIN_FOR_SEARCH_LANEMARKER_WITH_INITIAL_PARAMETER	(200.)

//#define	MAXIMIM_YAW_DIFF_AT_REGION_BOUNDARY	(10)

#define OFFSET_SEARCH_MARGIN_SEQUENCE	(200.)				// �ߖT�Ԑ����E�Ɖ����Ԑ����E�̗̈拫�E�ł̂���̋��e�l	// 20110703
#define YAW_SEARCH_MARGIN_SEQUENCE	(3.0 / 180. * M_PI)				// �ߖT�Ԑ����E�Ɖ����Ԑ����E�̗̈拫�E�ł̂���̋��e�l	// 20110703
//#define OFFSET_SEARCH_MARGIN_TRACK	(1000.)				// �ߖT�Ԑ����E�Ɖ����Ԑ����E�̗̈拫�E�ł̂���̋��e�l	// 20110703
#define OFFSET_SEARCH_MARGIN_TRACK	(200.)				// �ߖT�Ԑ����E�Ɖ����Ԑ����E�̗̈拫�E�ł̂���̋��e�l	// 20111129
#define YAW_SEARCH_MARGIN_TRACK	(1.0 / 180. * M_PI)				// �ߖT�Ԑ����E�Ɖ����Ԑ����E�̗̈拫�E�ł̂���̋��e�l	// 20110703

//#define	MAXIMUM_DIFF_FROM_PREVIOUS_FOUND_I	(10)	// �����o�_�Ƃ̐��������̍ő�M���b�v(�b��)
//#define	MAXIMUM_DIFF_FROM_PREVIOUS_FOUND_J	(10)	// �����o�_�Ƃ̐��������̍ő�M���b�v(�b��)
#define	MAXIMUM_DIFF_FROM_PREVIOUS_FOUND_I	(100)	// �����o�_�Ƃ̐��������̍ő�M���b�v(�b��)// 20120416
#define	MAXIMUM_DIFF_FROM_PREVIOUS_FOUND_J	(100)	// �����o�_�Ƃ̐��������̍ő�M���b�v(�b��)// 20120416

#define	MAXIMUM_DIFF_FROM_PREVIOUS_FOUND_Z	(15.0 * 1000)	// 20111121// �����o�_�Ƃ̂y�����̍ő�M���b�v(�b��)
#define	MAXIMUM_DIFF_FROM_PREVIOUS_FOUND_X	(0.5 * 1000)	// 20120118// �����o�_�Ƃ̂w�����̍ő�M���b�v(�b��)
//#define	MAXIMUM_DIFF_FROM_PREVIOUS_FOUND_X	(2.0 * 1000)	// 20120124

////#define	DB_OFFSET_MARGIN_OUTSIDE	OUTSIDE_OF_LANEBOUNDARY_FOR_LANE_BOUNDARY	// 20111103//���g�p
////#define	DB_OFFSET_MARGIN_INSIDE		INSIDE_OF_LANEBOUNDARY_FOR_LANE_BOUNDARY	// 20111103//���g�p


#define	MAX_DISTANCE_OF_FIRST_DETECTED_POINT	(50000.)	// ���������_�T�����Ɏn�߂Č��������_�̍ŏ����e����(��������_�ď��߂Č��������ꍇ�͒��~)
#define	MAXIMUM_RATIO_OF_REJECTED_POINTS	(0.5)	// �j�����ꂽ�Ԑ����E�_�̊����̍ő�l(�b��)


#define	MINIMUM_SEARCHMARGIN_IN_PIXEL	10	// ����ʒu�𒆐S�Ƃ���T���͈͂̍ŏ���[pix]	// 20111109


//#define	EDGETHRESHOLD_FACTOR_FOR_LANEMARKER	6
//#define	EDGETHRESHOLD_FACTOR_FOR_LANEMARKER	8	// 20111103
//#define	EDGETHRESHOLD_FACTOR_FOR_LANEMARKER	6	// 20111107
//#define	EDGETHRESHOLD_FACTOR_FOR_LANEMARKER	2	// 20111109
#define	EDGETHRESHOLD_FACTOR_FOR_LANEMARKER	1	// 20111124	// ������⌟�o���́C���σG�b�W���x��臒l���G�b�W�_���o����臒l��荂�߂ɐݒ肷��
#define	CURB_MARGIN	300	//[mm]	// ���̎Ԑ����E�T���͈͂̎Ԑ����E��������̋���[mm]
#define	EDGESTRENGTH_RATIO_TO_ELIMINATE	0.5	// 20110921// ���σG�b�W���x�Ɣ�r���ア�G�b�W�_���폜
#define	MAX_SECOND_VALUE	(400.)	// ���g�p

#define	LANE_BOUNDARY_POINTS_NUMBER_FOR_PARAMETER_ESTIMATION	10	// �p�����[�^����ɗp����Ԑ����E�_��
#define	MINIMUM_RATIO_OF_EDGESTRENGTH_FOR_FARAREA_LANEBOUNDARY	0.5	// ���������_�T�����ɋ��e����ߖT�����_�̕��σG�b�W���x�Ƃ̔�



#ifndef	FOR_TME
#define	COUNT_FOR_FOUND_STATUS_LANE_SIDE	5	// �������ǐՈڍs�t���[����(���E)
#define	COUNT_FOR_LOST_STATUS_LANE_SIDE		3	// �ǐՁ������ڍs�t���[����(���E)
//#define	COUNT_FOR_LOST_STATUS_LANE_SIDE		10	// �ǐՁ������ڍs�t���[����(���E)	20111003
#define	COUNT_FOR_FOUND_STATUS_LANE_REGION	5	// �������ǐՈڍs�t���[����(�߉�)
#define	COUNT_FOR_LOST_STATUS_LANE_REGION	3	// �ǐՁ������ڍs�t���[����(�߉�)
//#define	COUNT_FOR_LOST_STATUS_LANE_REGION	10	// �ǐՁ������ڍs�t���[����(�߉�)	20111003
#define	LBT_LANEMARKER_FOUND_COUNT	3	// �����������ǐՈڍs�t���[����
#define	LBT_LANEMARKERLINE_FOUND_COUNT	3// �G�b�W���������ǐՈڍs�t���[����
//#define	LBT_LANEMARKERLINE_FOUND_COUNT	5	// 20111003
#define	LBT_LANEMARKER_LOST_COUNT	3	// �����ǐՁ������o�ڍs�t���[����
#define	LBT_LANEMARKERLINE_LOST_COUNT	3	// �G�b�W���ǐՁ������o�ڍs�t���[����
//#define	LBT_LANEMARKERLINE_LOST_COUNT	5	// 20111003
#else	FOR_TME
#define	COUNT_FOR_FOUND_STATUS_LANE_SIDE	5	// �������ǐՈڍs�t���[����(���E)
#define	COUNT_FOR_LOST_STATUS_LANE_SIDE		10	// �ǐՁ������ڍs�t���[����(���E)	20111003
#define	COUNT_FOR_FOUND_STATUS_LANE_REGION	5	// �������ǐՈڍs�t���[����(�߉�)
#define	COUNT_FOR_LOST_STATUS_LANE_REGION	10	// �ǐՁ������ڍs�t���[����(�߉�)	20111003
#define	LBT_LANEMARKER_FOUND_COUNT	3
#define	LBT_LANEMARKERLINE_FOUND_COUNT	3
#define	LBT_LANEMARKER_LOST_COUNT	10
#define	LBT_LANEMARKERLINE_LOST_COUNT	10
#endif	FOR_TME




#define	COUNT_FOR_SOLID_LINE	2	// �������o����t���[����
#define	COUNT_FOR_DASH_LINE	2	// �j�����o����t���[����
//#define	COUNT_FOR_DASH_LINE	10	// �j�����o����t���[����	20111107
#define	MIN_RATIO_OF_SOLID_LINE_FOR_ONELINE_FILTER	(0.7)	// 20120127	// �����Ƃ݂Ȃ��ׂ̏������C�����ɑ΂���Ԑ����E�_���̔�̍ŏ��l
// TODO 0.25
#define	AVERAGE_EDGE_STRENGTH_AND_TH_RATIO	(0.8)	// ���σG�b�W���x�ɂ�铮�I臒l�ݒ莞�̃p�����[�^

//#define	MISMATCH_OF_LANEMARKERPOINTS_FROM_LANEPARAMATER_PIXEL	(10)	// �Ԑ����E�_�Ƒ��H�p�����[�^�ɂ��Ԑ����E�Ƃ̂���̋��e��[pix]
#define	MISMATCH_OF_LANEMARKERPOINTS_FROM_LANEPARAMATER_PIXEL	(20)	// �Ԑ����E�_�Ƒ��H�p�����[�^�ɂ��Ԑ����E�Ƃ̂���̋��e��[pix]	// 20111201
#define	MISMATCH_OF_LANEMARKERPOINTS_FROM_LANEPARAMETER_RATIO	(0.5)	// �Ԑ����E�_�Ƒ��H�p�����[�^�ɂ��Ԑ����E�Ƃ̂���ʂ̔�̋��e��

/// V moved at 20120313 ///////////////////////////////
#define	EXTENDED_HEIGHT_FOR_FAR_AREA	20	// �Œ�s�b�`�p�ŎZ�o����鉓���̈�̍ŉ����C�����摜����ւ��炵�C�����̈���g�傷��

#define	FACTOR_OF_LANEMARKER_WIDTH_FOR_FARSIDE	2	// �������̉������������̐���̊ɘa�W��

#define	FACTOR_FOR_AVERAGE_INTENSITY_OF_LANEMARKER	2	// ���ϋP�x�Ɋւ��锒�������̌W��

#define	LANEMARKERPAIR_MINIMUM_WIDTH	(2500.)	// �����y�A���̋��e�ŏ��Ԋu
#define	LANEMARKERPAIR_MAXIMUM_WIDTH	(5000.)	// �����y�A���̋��e�ő�Ԋu
//#define	LANEMARKERPAIR_MAXIMUM_YAWDIFF_DEG	(10.)	// �����y�A���̋��e�ő僈�[����(�Œ�s�b�`�p)//10
#define	LANEMARKERPAIR_MAXIMUM_YAWDIFF_DEG	(4.)//2014.9.25

//#define	MULTIDIRECTION_CALCEDGESTRENGTH		// �w�����(0,45,-45�x)�̃G�b�W���x���Z�o

#define	FACTOR_FOR_FAR_END_OF_PROCLINE	1 / 5	// �摜�㕔�̃G�b�W�_���o�������̈�

#define	INVALID_DISTANCE_VALUE	(100. * 1000)	// �����ȋ����̒l[mm]

//#define	MAXIMUM_YAW_DIFF_IN_COMPLEX_LANE_MARKER	(5)
#define	MAXIMUM_YAW_DIFF_IN_COMPLEX_LANE_MARKER	(10)	// 20111124	// �������̕��s���Q�Ƃ݂Ȃ��ׂ̃��[�p�̍��̍ő�l[pix]
#define	MAXIMUM_OFFSET_DIFF_IN_COMPLEX_LANE_MARKER	(10)// �������̕��s���Q�Ƃ݂Ȃ��ׂ̃I�t�Z�b�g�̍��̍ő�l[pix]
#define	MINIMUM_OFFSET_DIFF_IN_COMPLEX_LANE_MARKER	(-3)// �������̕��s���Q�Ƃ݂Ȃ��ׂ̃I�t�Z�b�g�̍��̍ŏ��l[pix]

#define	MINIMUM_POINT_NUMBER_FOR_LANEMARKER_TYPE	10

#define	MAXIMUM_LINE_SEGMENT_FOR_LANEMARKER_TYPE	10

#define	MAXIMUM_GAP_OF_LINE_SEGMENT_FOR_LANEMARKER_TYPE	20


#define	MAXIMUMLINE_SEGMENT_OF_DASH_LANEBOUNDARY_TYPE	2	// �j���Ƃ݂Ȃ����߂̍ő������

// ���������[�h�̏�ԑJ�ڗp�J�E���^��臒l
#define	FRAME_CLBT_SINGLE_TO_NONE	10//5	//���������[�h�̒P�����������o��ԑJ�ڂ̂��߂̃t���[����
#define	FRAME_CLBT_DOUBLE_TO_NONE	10//5	//���������[�h�̓�d���������o��ԑJ�ڂ̂��߂̃t���[����
#define	FRAME_CLBT_DOUBLE_TO_SINGLE	10//5	//���������[�h�̓�d�����P������ԑJ�ڂ̂��߂̃t���[����
#define	FRAME_CLBT_TRIPLE_TO_NONE	10//5	//���������[�h�̎O�d���������o��ԑJ�ڂ̂��߂̃t���[����
#define	FRAME_CLBT_TRIPLE_TO_SINGLE	10//5	//���������[�h�̎O�d�����P������ԑJ�ڂ̂��߂̃t���[����
#define	FRAME_CLBT_TRIPLE_TO_DOUBLE	10//5	//���������[�h�̎O�d������d����ԑJ�ڂ̂��߂̃t���[����

#define	GAP_IN_COMPLEX_LANE_BOUNDARY	2000.	// ���g�p

#define	MININUM_POINT_NUMBER_FOR_NEAR_AREA	10	// ���H�p�����[�^�ɂ��ߖT�̈�̎Ԑ����E�_�T�����̍ŏ��_��
#define	MAXIMUM_GAP_BETWEEN_CONNECTED_POINTS	3

#define	BOTHSIDE_LOST_COUNT	10

#define	FACTOR_FOR_SEARCHING_IN_DEFAULT_LANE_POSITION	2


#define	MAXIMUM_YAW_FOR_MAIN_LANE	(1.0 / 180 * M_PI)
#define	MINIMUM_YAW_FOR_BRANCH_LANE	(5.0 / 180 * M_PI)

#define	CALC3DPLANE_MAXIMUM_A	0.5

#define	MAXIMUM_AVERAGE_DISTANCE_FROM_3D_SURFACE	100.
#define	MAXINUM_GAP_FOR_ISOLATED_LANE_BOUNDARY	(15000.)

#define	MAXIMUM_ISRC_DIFF_FOR_ISOLATED_LANE_BOUNDARY_POINTS	(5)
#define	MAXIMUM_JSRC_DIFF_FOR_ISOLATED_LANE_BOUNDARY_POINTS	(5)

#define	SEARCH_MARGIN_IN_NEAR_AREA	(500.)
#define	SEARCH_MARGIN_IN_FAR_AREA	(500.)

//#define	DB_OFFSET_MIN_FACTOR	1
//#define	DB_OFFSET_MIN_FACTOR	10	//20111122
#define	DB_OFFSET_MIN_FACTOR	5
#define	OBSERVER_ERROR_FACTOR	1.0
#define	MINIMUM_VELOCITY (30. * 1000 / 3600. * 1000)

#define	MINIMUM_Z3D	(1.0 * 1000)
//#define	TH_ABOVE_GROUND	100.0
#define	TH_ABOVE_GROUND	150.0

#define	LENGTH_TO_ELIMINATE_SHORT_LANEMARKERLINES	(2000.)
#define	LENGTH_TO_ELIMINATE_FEWPOINT_LANEMARKERLINES	(20)


#endif _CONFIG_H_
