#pragma once
#include <string>
#include <vector>
#include "bean/BaseDefine.h"
#include "utils/colormisc.h"
#include "utils/config.h"
#include "utils/config2.h"
#include "utils/imrgb.h"

#include "bean/Lane.h"
#include "utils/roadimage_window.h"
#include "utils/lm_type.h"
#include "utils/matutil-d.h"
#include "main_proc.h"
#include "utils/my_resource.h"
#include "utils/tmc_stereobmp-forMono.h"
#include "bean/LaneMarkerPoints.h"
#include "LaneDetector.h"
#include "utils/type.h"
#include "utils/globalVal.h"
using namespace std;
class LaneDraw
{
public:
    int judge_lane_flag = 0;
    vector<point> leftPoints;
    vector<point> rightPoints;
    vector<point> leftImagePoints, rightImagePoints;
	BOOL drawBoundaryPointOnRoadImage(IMRGB *pImrgb, LaneDetector *pLaneDetector);
	BOOL drawDistanceLinesOnInputImage(IMRGB *pImrgb, LaneDetector *pLaneDetector);
	BOOL drawEdgePointsOnRoadOnInputImage(IMRGB *pImrgb, LaneDetector *pLaneDetector);
	BOOL drawGridMap1DFunc(IMRGB *pImrgb, LaneDetector *pLaneDetector);
	BOOL drawLaneBoundaryAvailableFlagOnInputImage(IMRGB *pImrgb, LaneDetector *pLaneDetector);
	BOOL drawLaneBoundaryDetectionStatusOnInputImage(IMRGB *pImrgb, LaneDetector *pLaneDetector);
	BOOL drawLaneBoundaryOnInputImage(IMRGB *pImrgb, LaneDetector *pLaneDetector);
	BOOL drawLaneBoundaryOnRoadImage(IMRGB *pImrgb, LaneDetector *pLaneDetector);
	BOOL drawLaneBoundaryPointsForLaneParamterEstimationOnInputImage(IMRGB *pImrgb, LaneDetector *pLaneDetector);
	BOOL drawLaneBoundaryPointsForLaneParamterEstimationOnRoadImage(IMRGB *pImrgb, LaneDetector *pLaneDetector);
	BOOL drawLaneBoundaryPointsInFarAreaOnInputImage(IMRGB *pImrgb, LaneDetector *pLaneDetector);
	BOOL drawLaneBoundaryPointsInNearAreaOnInputImage(IMRGB *pImrgb, LaneDetector *pLaneDetector);
	BOOL drawLaneBoundaryPointsOnInputImage(IMRGB *pImrgb, LaneDetector *pLaneDetector);
	BOOL drawLaneBoundaryWithLaneParamterOneSideAndLaneMarkerLineSequencesOnInputImage(IMRGB *pImrgb, LaneDetector *pLaneDetector);	BOOL drawLaneBoundaryWithLaneParamterCenterLineOnInputImage(IMRGB *pImrgb, LaneDetector *pLaneDetector);
    void drawLaneBoundaryByPreviousFrame(IMRGB *pImrgb);
    IplImage* drawDetectObjectOnInputImage(IplImage* Im_output_,LaneDetector *pLaneDetector);

	BOOL drawLaneBoundaryWithLaneParamterOneSideOfNearAreaOnInputImage(IMRGB *pImrgb, LaneDetector *pLaneDetector);
	BOOL drawLaneBoundaryWithLaneParamterOneSideOnInputImage(IMRGB *pImrgb, LaneDetector *pLaneDetector);
	BOOL drawLaneBoundaryWithLaneParamterOneSideOnInputImage(IMRGB *pImrgb, LaneDetector *pLaneDetector, int iLR);
	BOOL drawLaneBoundaryWithLaneParamterOnInputImage(IMRGB *pImrgb, LaneDetector *pLaneDetector);
	BOOL drawLaneCenterOnInputImage(IMRGB *pImrgb, LaneDetector *pLaneDetector);
	BOOL drawLaneMarkerLineSequencesOnInputImage(IMRGB *pImrgb, LaneDetector *pLaneDetector);
	BOOL drawLaneMarkerLineSequencesOnRoadImage(IMRGB *pImrgb, LaneDetector *pLaneDetector);
	BOOL drawLaneMarkerLinesOnInputImage(IMRGB *pImrgb, LaneDetector *pLaneDetector);
	BOOL drawLaneMarkerLinesOnRoadImage(IMRGB *pImrgb, LaneDetector *pLaneDetector);
	BOOL drawLaneMarkerPointOnRoadImage(IMRGB *pImrgb, LaneDetector *pLaneDetector);
	BOOL drawLaneMarkerPoints(IMRGB *pImrgb, LaneMarkerPoints *pUpEdgePoints,LaneMarkerPoints *pDownEdgePoints);
	BOOL drawLaneMarkersOnInputImage(IMRGB *pImrgb, LaneDetector *pLaneDetector);
	BOOL drawLaneMarkersOnRoadImage(IMRGB *pImrgb, LaneDetector *pLaneDetector);
	BOOL drawObserverPointOnRoadImage(IMRGB *pImrgb, LaneDetector *pLaneDetector);
	BOOL drawProcLineLinesOnInputImage(IMRGB *pImrgb, LaneDetector *pLaneDetector);
	void DrawResultFunc(IMRGB *pImrgb);
	BOOL drawSelectedLinesOnInputImage(IMRGB *pImrgb, LaneDetector *pLaneDetector);
	BOOL overlay3DImageOnInputImage(IMRGB *pImrgbInput);
	BOOL overlayRoadImageOnInputImage(IMRGB *pImrgbInput);
};

class lane_lateral_LR_point
{
public:

   double Left_point;
   double Right_point;
   lane_lateral_LR_point()
   {
        Left_point=0;
        Right_point=0;
   }

};

class lane_lateral_LR_point_list
{
 public:
 int point_num;
 std::vector< lane_lateral_LR_point > LR_point_list;
};

