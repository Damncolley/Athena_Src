#pragma once
#include "LaneMarkerPoints.h"
#include "../utils/tmc_stereobmp-forMono.h"
#include "LaneParameterEstimator.h"

extern double getNEAR_BASEY(void);
extern double getNEARHEIGHT(void);
extern double getNEAR_TOPY(void);
extern double getFAR_BASEY(void);
extern double getFARHEIGHT(void);
extern double getFAR_TOPY(void);
extern double getFARAREA_TOPY(void);

extern void setNEAR_BASEY(double v);
extern void setNEARHEIGHT(double v);
extern void setNEAR_TOPY(double v);
extern void setFAR_BASEY(double v);
extern void setFARHEIGHT(double v);
extern void setFAR_TOPY(double v);
extern void setFARAREA_TOPY(double v);

extern int getPositiveThresholdDefault(void);
extern int getNegativeThresholdDefault(void);
extern void setPositiveThresholdDefault(int iV);
extern void setNegativeThresholdDefault(int iV);
extern int getLaneBoundaryPointsNumberForParamameterEstimation(void);
extern void setLaneBoundaryPointsNumberForParamameterEstimation(int iV);

extern double calcZvehicleFromIsrc(PARAM_CAM *pCamParam, int iIsrc);
extern double calcZvehicleFromIsrcByNearAreaParameter(PARAM_CAM *pCamParam, int iIsrc);
extern int calcHorizontalLineOfDepth(PARAM_CAM *pCamParam, double dZroad);
extern int calcHorizontalLine(PARAM_CAM *pCamParam);
extern double calcDepthOfHorizontalLine(PARAM_CAM *pCamParam, int iIsrc);
extern double getWidthOfPixel(PARAM_CAM *pCamParam, int iIsrc);
extern double getWidthOfPixelByNearAreaParameter(PARAM_CAM *pCamParam, int iIsrc);
extern BOOL detectLaneMarkerPoints(PARAM_CAM *pCamParam, Uchar *pImage, LaneMarkerPoints *pUpEdgePoints, LaneMarkerPoints *pDownEdgePoints);

extern BOOL calc3DPlane(LaneMarkerPoints *pLMPs, double *pa, double *pc, double *pd);
extern BOOL calc3DPlaneThroughOrigin(LaneMarkerPoints *pLMPs, double *pa, double *pc, double *pd);
extern BOOL calc3DSurface(LaneMarkerPoints *pLMPs, double *pdalpha, double *pdcv, double *pdgamma, double *pdd);
extern BOOL calc3DSurfaceWithoutGamma(LaneMarkerPoints *pLMPs, double *pdalpha, double *pdcv, double *pdd);
extern BOOL judgeLaneMarkerPointAboveGround(PARAM_CAM *pCamParam, double *pdPlane3D, double dTh, LaneMarkerPoints *pLaneMarkerPoints);

extern BOOL calcPosImageFromPosRoad(PARAM_CAM *pCamParam, int a_iIroad, int a_iJroad, double *a_pdIimage, double *a_pdJimage);
extern BOOL calcPosImageFromPosRoad(PARAM_CAM *pCamParam, double *pdPlane3D, int a_iIroad, int a_iJroad, double *a_pdIimage, double *a_pdJimage);
extern BOOL calcPosRoadFromPosImage(PARAM_CAM *pCamParam, int a_iIimage, int a_iJimage, double *a_pdIRoad, double *a_pdJRoad);
extern double calcIimageFromUimage(PARAM_CAM *pCamParam, DB dUimage);
extern double calcJimageFromVimage(PARAM_CAM *pCamParam, DB dVimage);

extern BOOL calcProjectionTo3DPlane(PARAM_CAM *pCamParam, double *pdPlane3D, double *pdSrc, double *pdDst);
extern BOOL calcPosImageFromPosVehicle(PARAM_CAM *pCamParam, double dXVehicle, double dYVehicle, double dZVehicle, int *piIsrc, int *piJsrc);


void transformInputToRoad(PARAM_CAM *pCamParam, int iIsrc, int iJsrc, double *pdZVehicle, double *pdXVehicle);

//////////////////////////////////////////////////////////////////////////
const int LR_NUM	=	2;
const int LR_LEFT	=	0;
const int LR_RIGHT	=	1;

const int NF_NUM	=	2;
const int NF_NEAR	=	0;
const int NF_FAR	=	1;

const int UD_NUM	=	2;
const int UD_UP		=	0;
const int UD_DOWN	=	1;

const int LBT_NONE					= 0;
const int LBT_LANEMARKERLINE		= 1;
const int LBT_LANEMARKER			= 2;
const int LBT_LANEMARKERLINE_TRACK	= 3;
const int LBT_LANEMARKER_TRACK		= 4;

const int LBT_SOLID	=	1;
const int LBT_DASH	=	2;
const int LBT_DOT	=	3;

const int LBT_UP	=	1;
const int LBT_DOWN	=	2;

const int LBT_FLAT	=	1;
const int LBT_STEP	=	2;

const int LPT_NONE	=	0;
const int LPT_BOTHSIDE	=	1;

int calcIsrcFromUimage(PARAM_CAM *pCamParam, double dUimage);
double calcUimageFromIsrc(PARAM_CAM *pCamParam, int iIsrc);
int calcJsrcFromVimage(PARAM_CAM *pCamParam, double dVimage);
double calcVimageFromJsrc(PARAM_CAM *pCamParam, int iJsrc);
double calcZvehicleFromIsrc(PARAM_CAM *pCamParam, int iIsrc);
double calcZvehicleFromIsrcByNearAreaParameter(PARAM_CAM *pCamParam, int iIsrc);
double calcXcameraFromXvehicle(PARAM_CAM *pCamParam, double dXvehicle);
double calcXcameraFromXvehicleByNearAreaParameter(PARAM_CAM *pCamParam, double dXvehicle);
double calcXvehicleFromXcamera(PARAM_CAM *pCamParam, double dXcamera);
double calcXvehicleFromXcameraByNearAreaParameter(PARAM_CAM *pCamParam, double dXcamera);
double calcYcameraFromYvehicle(PARAM_CAM *pCamParam, double dYvehicle);
double calcYvehicleFromYcamera(PARAM_CAM *pCamParam, double dYcamera);
double calcZcameraFromZvehicle(PARAM_CAM *pCamParam, double dZVehicle);
double calcZcameraFromZvehicleByNearAreaParameter(PARAM_CAM *pCamParam, double dZVehicle);
double calcZvehicleFromZcamera(PARAM_CAM *pCamParam, double dZcamera);
double calcUimageFromZcameraOnRoad(PARAM_CAM *pCamParam, double dZcamera);
double calcUimageOfHorizontalLine(PARAM_CAM *pCamParam);
double calcUimageFromYcamera(PARAM_CAM *pCamParam, double dYcamera);
double calcYcameraFromUimage(PARAM_CAM *pCamParam, double dUimage);
double calcYcameraFromUimageByNearAreaParameter(PARAM_CAM *pCamParam, double dUimage);
double calcZcameraFromYcameraOnRoad(PARAM_CAM *pCamParam, double dYcamera);
double calcZcameraFromYcameraOnRoadByNearAreaParameter(PARAM_CAM *pCamParam, double dYcamera);
double calcXcameraFromVimage(PARAM_CAM *pCamParam, double dVimage);
double calcVimageFromXcamera(PARAM_CAM *pCamParam, double dXcamera);
double calcVimageFromXcameraAndZcameraOnRoad(PARAM_CAM *pCamParam, double dXcameraRoad, double dZcameraRoad);
double calcVimageFromXcameraAndZcameraOnRoadByNearAreaParameter(PARAM_CAM *pCamParam, double dXcameraRoad, double dZcameraRoad);
int calcHorizontalLineOfDepth(PARAM_CAM *pCamParam, double dZvehicle);
int calcHorizontalLine(PARAM_CAM *pCamParam);
double calcDepthOfHorizontalLine(PARAM_CAM *pCamParam, int iIsrc);
double getWidthOfPixel(PARAM_CAM *pCamParam, int iIsrc);
double getWidthOfPixelByNearAreaParameter(PARAM_CAM *pCamParam, int iIsrc);

//////////////////////////// for topview image //////////////////////////
#define	ROAD_TOP_Y	50000	//(100000)	// ?H每那㏑?＆?㏑※??＊[[mm]
#define	ROAD_LEFT_X	(-10000)			// ?H每那㏑?＆???＊[[mm]
#define	ROAD_REAL_WIDTH	(20000)			// ?H每那㏑?＆???[mm]
#define	ROAD_REAL_HEIGHT	ROAD_TOP_Y	// ?H每那㏑?＆???[mm]

#define	ROAD_IMAGE_WIDTH	480//(192)	// ?H每那㏑?＆?㏑?㏑?＆f[pix]
#define	ROAD_IMAGE_HEIGHT	(960)		// ?H每那㏑?＆??c㏑?＆f[pix]

static double g_dRoadTopY	= 	ROAD_TOP_Y;
static double g_dRoadLeftX	=	ROAD_LEFT_X;
static double g_dRoadRealWidth	=	ROAD_REAL_WIDTH;
static double g_dRoadRealHeight	=	ROAD_REAL_HEIGHT;

static int g_iRoadImageWidth	=	ROAD_IMAGE_WIDTH;
static int g_iRoadImageHeight	=	ROAD_IMAGE_HEIGHT;

double getRoadTopY(void);
double getRoadLeftX(void);
double getRoadRealWidth(void);
double getRoadRealHeight(void);
int getRoadImageWidth(void);
int getRoadImageHeight(void);
double calcZvehicleFromIroad(int iIroad);
double calcXvehicleFromJroad(int a_iJroad);
int calcIroadFromZvehicle(double dZVehicle);
int calcJroadFromXvehicle(double dXVehicle);
double calcUimageFromZvehicleOnRoad(PARAM_CAM *pCamParam, DB dZVehicle);	// Zvehicle->Uimage
double calcZvehicleFromUimage(PARAM_CAM *pCamParam, double dUimage);	// Uimage->Zvehicle
double calcVimageFromXvehicleAndZvehicleOnRoad(PARAM_CAM *pCamParam, DB dXvehicleOnRoad, DB dZvehicleOnRoad);
double calcVimageFromXvehicleAndZvehicleOnRoadByNearAreaParameter(PARAM_CAM *pCamParam, DB dXvehicleOnRoad, DB dZvehicleOnRoad);
BOOL calcPosImageFromPosVehicle(PARAM_CAM *pCamParam, double dXvehicle, double dYvehicle, double dZvehicle, int *piIsrc, int *piJsrc);
double calcXcameraOnRoadFromXcameraAndZcamera(PARAM_CAM *pCamParam, double dXcamera, double dZcamera);
double calcXvehicleFromVimageAndZvehicle(PARAM_CAM *pCamParam, DB dVimage, DB dZvehicle);
double calcIimageFromUimage(PARAM_CAM *pCamParam, double dUimage);
double calcJimageFromVimage(PARAM_CAM *pCamParam, DB dVimage);
double calcUimageFromIimage(PARAM_CAM *pCamParam, DB dIimage);
double calcVimageFromJimage(PARAM_CAM *pCamParam, DB dJimage);
BOOL calcPosImageFromPosRoad(PARAM_CAM *pCamParam, int iIroad, int iJroad, double *pdIimage, double *pdJimage);
BOOL calcPosRoadFromPosImage(PARAM_CAM *pCamParam, int iIimage, int iJimage, double *pdIRoad, double *pdJRoad);


