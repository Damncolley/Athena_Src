#ifndef _CONFIG2_H_
#define _CONFIG2_H_

//#define	FOR_DLL	// 20081103
//#define	ONLY_FOR_VP_DETECTION	// 20081103
#include "type.h"
#pragma warning( disable : 4996 )
#pragma warning( disable : 4819 )
//#pragma warning( disable : 1786 )
#pragma warning( disable : 157 )
#pragma warning( disable : 4311 )
#pragma warning( disable : 4312 )

#define	AW_DEBUG
#define	INPUT_IMAGE_WIDTH	1280		// ���͉摜�̕�[pixel]
#define	INPUT_IMAGE_HEIGHT	720		// ���͉摜�̍���[pixel]
#define	ROAD_IMAGE_WIDTH	480		// ���͉摜�̕�[pixel]
#define	ROAD_IMAGE_HEIGHT	960		// ���͉摜�̍���[pixel]

#ifndef	M_PI
#define	M_PI	(3.141592)
#endif

#define BUFSIZE 1024

#define	FALSE	0
#define	TRUE	1

//#define CHECK_MEMORY_LEAK

//#define	CALC_PEAK_TH_BY_MAX_PEAK
#define	ERASE_CROSSING_PEAKS

// ���s���Ƃ݂Ȃ����߂̗����Ԑ����E�̋ȗ��̍�
//#define	CB_DIFF_OF_BOTH_SIDE_CURVATURE	10
//#define	CB_DIFF_OF_BOTH_SIDE_CURVATURE	10	// 20050214
#define	CB_DIFF_OF_BOTH_SIDE_CURVATURE	5	// 20050215


#define	NO_EGOMOTION	// 20050509

#define	SEARCH_LOCAL_MAXIMAM	// 20050509
//#define	USE_SOBEL	// 20050509

#define	REGION_FOR_CURB_DETECTION	1


//#define	VERTICAL_OFFSET_IN_IMAGE	100

#define INITIAL_FLAG_OF_OUTPUTAVI	FALSE
#define	APP_TITLE	"LaneDetector"	// �A�v���P�[�V�����^�C�g��


#define	DRAW_RESULT_ALWAYS	// 20090709
//#define	DRAW_TO_UNDETECTED_AREA
#define	DISTANCE_OF_UNDECTED_AREA_TO_DRAW	70000

#define	USE_PNR_DIR 1
//#define	WITH_FCW_TAG

#define	DRAW_DISPARITY
#endif _CONFIG2_H_?
