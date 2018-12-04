#ifndef	_LANEPARAMETERESTIMATOR_H_
#define	_LANEPARAMETERESTIMATOR_H_
#include <stdio.h>

#include "../utils/config.h"
#include "../utils/config2.h"
#include "../utils/matutil-d.h"

#include "../utils/type.h"

class PARAM_CAM;
class LaneMarkerPoints;
class LaneMarkerPoint;

#define	CS4_STATUS_NUMBER	8
//#define	CS4_OBS_NUMBER	10
#define	CS4_OBS_NUMBER	1
#define	CS4_CONTROL_INPUT_NUMBER	2
#define	CS4_STOCHASTIC_VARIAVLE_NUMBER	8//3
#define	CDB_PSIDASH_SCALE		1
#define	CDB_PSI_SCALE			1
#define	CDB_EDASH_SCALE	1
#define	CDB_E_SCALE		1
#define	CDB_RHODASH_SCALE		1
#define	CDB_RHO_SCALE			1
#define	CDB_DELTAPHI_SCALE		1
#define	CDB_WIDTH_SCALE			1

const int LPID_YAWDASH			=	0;
const int LPID_YAW				=	1;
const int LPID_OFFSETDASH		=	2;
const int LPID_OFFSET			=	3;
const int LPID_CURVATUREDASH	=	4;
const int LPID_CURVATURE		=	5;
const int LPID_PITCH			=	6;
const int LPID_LANEWIDTH		=	7;

#define	CX0		    (pCamParam->ParamCam()->i_x0)
#define	CY0		    (pCamParam->ParamCam()->i_y0)
#define	WIDTH	    (pCamParam->ParamCam()->width)
#define HEIGHT	    (pCamParam->ParamCam()->height)
#define	FOCUS	    (pCamParam->ParamCam()->pix_f_y)
#define	FOCUSX	    (pCamParam->ParamCam()->pix_f_x)
#define	FOCUSY	    (pCamParam->ParamCam()->pix_f_y)
#define	POSX	    (pCamParam->ParamCam()->cam_pos_x)
#define	POSY	    (pCamParam->ParamCam()->cam_pos_y)
#define	POSZ	    (pCamParam->ParamCam()->cam_pos_z)
#define	PITCH	    (pCamParam->ParamCam()->pitch / 180. * M_PI)
#define	YAW		    (pCamParam->ParamCam()->yaw / 180. * M_PI)
#define	CV		    (pCamParam->getCurvatureVertical())
//#define	MINIMUM_CV	(0.0001)

#define	TCX0		(lCamParamTmp.ParamCam()->i_x0)
#define	TCY0		(lCamParamTmp.ParamCam()->i_y0)
#define	TWIDTH	(lCamParamTmp.ParamCam()->width)
#define	THEIGHT	(lCamParamTmp.ParamCam()->height)
#define	TFOCUS	(lCamParamTmp.ParamCam()->pix_f_y)
#define	TFOCUSX	(lCamParamTmp.ParamCam()->pix_f_x)
#define	TFOCUSY	(lCamParamTmp.ParamCam()->pix_f_y)
#define	TPOSX	(lCamParamTmp.ParamCam()->cam_pos_x)
#define	TPOSY	(lCamParamTmp.ParamCam()->cam_pos_y)
#define	TPOSZ	(lCamParamTmp.ParamCam()->cam_pos_z)
#define	TPITCH	(lCamParamTmp.ParamCam()->pitch / 180. * M_PI)
#define	TYAW		(lCamParamTmp.ParamCam()->yaw / 180. * M_PI)
#define	TCV		(lCamParamTmp.getCurvatureVertical())

//yu_2014.9.20
extern void transformInputToRoad(PARAM_CAM *pCamParam, int iIsrc, int iJsrc, double *pdZg, double *pdXg);
extern double calcVimageFromXvehicleAndZvehicleOnRoad(PARAM_CAM *pCamParam, double a_dXVehicle, double a_dZVehicle);
extern double calcVimageFromXvehicleAndZvehicleOnRoadByNearAreaParameter(PARAM_CAM *pCamParam, double a_dXVehicle, double a_dZVehicle);
extern double calcUimageFromIsrc(PARAM_CAM *pCamParam, int iIsrc);

#define	BUFSIZE	1024
// psidash, psi, edash, e rhodash, rho, phi, w
// psidash, psi, edash, e rhodash, rho, phi, w
static double g_adb_X_0[CS4_STATUS_NUMBER * 1] = {
	0.1 / 180 * M_PI,	// psidash
	1.0 / 180 * M_PI,	// psi
	100.0,	// edash,
	1000.0,	// e
	1.0e-12,	// rhodash
	1.0e-9,	// rho,
	5.0 / 180 * M_PI,	// phi
	DB_DP_AVE * 1.1//4000	// w
};
static double g_adb_X_m[CS4_STATUS_NUMBER * 1] = {
	(0.0) / CDB_PSIDASH_SCALE,	// psidash
	(0.0) / CDB_PSI_SCALE,	// psi
	(0.0) / CDB_EDASH_SCALE,	// edash,
	(0.0) / CDB_E_SCALE,	// e
	(0.0) / CDB_RHODASH_SCALE,	// rhodash
	(0.0) / CDB_RHO_SCALE,	// rho,
	(0.0) / CDB_DELTAPHI_SCALE,	// phi
	(DB_DP_AVE) / CDB_WIDTH_SCALE	// w[mm]
};

static double g_adb_W_m[CS4_STOCHASTIC_VARIAVLE_NUMBER * 1] = {
	1.0,
	1.0,
	1.0,
	1.0,
	1.0,
	1.0,
	1.0,
	1.0
};
static double g_adb_W_m0[CS4_STOCHASTIC_VARIAVLE_NUMBER * 1] = {
	1.0,
	1.0,
	1.0,
	1.0,
	1.0,
	1.0,
	1.0,
	1.0
};
static double g_adb_V_m[CS4_OBS_NUMBER * 1] = {
	V_V_M * OBSERVER_ERROR_FACTOR
};



#endif	_LANEPARAMETERESTIMATOR_H_
