#ifndef _LANE_AREA_H_
#define _LANE_AREA_H_

#include "../utils/type.h"
#include "../utils/flexarray.h"
#include "LaneMarkerPoint.h"
#include "LaneMarkerPoints.h"
#include "LaneMarkerLineSequence.h"
#include "LaneMarkerLineSequences.h"

#include "LaneParameter.h"
#include "LaneParameterOneSide.h"
#include "LaneRegion.h"
#include "../utils/GridMap1D.h"
#include "../utils/OutputInfo.h"
#include <iostream>
using namespace std;

class LaneArea
{
private:
    LaneRegion _Regions[NF_NUM];

    LaneMarkerLineSequences *_pLaneMerkerLineSequences[LR_NUM];
    LaneMarkerLineSequence *_pLaneMerkerLineSequence3D[LR_NUM];
    LaneMarkerLine *_pLaneMerkerLine3D[LR_NUM];

    LaneMarkerPoints *_pLaneBoundaryPointsToEstimateLaneParameter[LR_NUM];	// ¡®¨C?H?p?¡ë???[?^??¡¯¨¨¡ªp?¨¬?G?b?W¡°_???X?g(???E¡ëE)
    LaneMarkerPoints *_pLaneBoundaryPointsInFarArea[LR_NUM];					// ¡ë¡°??¡ª¨¬??(¡¯??¨¹???¡ª¡ª¨¬???¨¨¡ë¡°)?¨¬?G?b?W¡°_???X?g(???E¡ëE)
    LaneMarkerPoints *_pLaneBoundaryPointsInNearArea[LR_NUM];					// ??¨CT¡ª¨¬???¨¬?G?b?W¡°_???X?g(???E¡ëE)(?p?¡ë???[?^?¨¬???¨¦¡¯T??)
    LaneMarkerPoints *_pLaneBoundaryPoints[LR_NUM];					// ???¨¹???E¡°_???X?g(???E¡ëE)(?p?¡ë???[?^?¨¬???¨¦¡¯T??)

    LaneParameter *_pLaneParameter;								// ¡®¨C?H?p?¡ë???[?^(¡ª?¡®¡è???¨¹???E¡ªL)
    LaneParameterOneSide *_pLaneParameterOneSide[LR_NUM];		// ?D¡®¡è???¨¹???E?¨¦???¨¦¡®¨C?H?p?¡ë???[?^(???E¡ëE)

    double _adPlane3D[3];	// a,c,d					// ?H¨C¨º??¨C¨º?¨¬?p?¡ë???[?^
    double _adSurface3D[4];	// alpha,cv,gamma,d			// ?H¨C¨º???E?¨¬?p?¡ë???[?^

    int _iLaneBoundaryFoundCounter[LR_NUM];
    int _iLaneBoundaryLostCounter[LR_NUM];
    int _iLaneBoundaryType[LR_NUM][2];

    int _iLaneBoundarySolidCounter[LR_NUM];
    int _iLaneBoundaryDashCounter[LR_NUM];


    LaneMarkerPoints *_pEdgePointsOnRoad;
    int _iIsrcMin;
    int _iIsrcMax;
    int _iJsrcLeft_At_IsrcMin;
    int _iJsrcRight_At_IsrcMin;
    int _iJsrcLeft_At_IsrcMax;
    int _iJsrcRight_At_IsrcMax;

    double _dSearchMarginInNearArea;
    double _dSearchMarginInFarArea;

    double _dMaxDiffFromMedianDeviationInNearArea;
    double _dMaxDiffFromMedianDeviationInFarArea;

    int _iEdgeStrengthOfLaneBoundary[LR_NUM];
    BOOL _bSearchInDefaultLanePosition[LR_NUM];

    int _iBothSideLostCounter;
    int _iLaneParameterType;

    double _dPitchEstimated;
    double _dPitchPredicted;

    int *_piProcLine;
    int _iProcLineNumber;

    int _iLineNumberFarArea;
    int	_iLineNumberNearArea;

    int _iIsrcAvailable[LR_NUM];

    FlexArray<int>	*_faTopIntensity[LR_NUM];

    GridMap1D *_pGridMap1D;
    OutputInfo _OutputInfo;

#ifdef	USE_EDGEPOINT2D
    LaneMarkerPoint **_ppEdgePoint2D;
#endif	USE_EDGEPOINT2D

public:
    inline LaneArea(void)
    {
        {
            for(int iLR = 0; iLR < LR_NUM; iLR++)
            {
                _pLaneBoundaryPointsToEstimateLaneParameter[iLR] = NULL;
            }
        }
        {
            for(int iLR = 0; iLR < LR_NUM; iLR++)
            {
                _pLaneBoundaryPointsInFarArea[iLR] = NULL;
            }
        }
        {
            for(int iLR = 0; iLR < LR_NUM; iLR++)
            {
                _pLaneBoundaryPointsInNearArea[iLR] = NULL;
            }
        }
        {
            for(int iLR = 0; iLR < LR_NUM; iLR++)
            {
                _pLaneBoundaryPoints[iLR] = NULL;
            }
        }
        {
            for(int iLR = 0; iLR < LR_NUM; iLR++)
            {
                _pLaneMerkerLineSequences[iLR] = NULL;
            }
        }
        {
            for(int iLR = 0; iLR < LR_NUM; iLR++)
            {
                _pLaneMerkerLineSequence3D[iLR] = NULL;
            }
        }
        {
            for(int iLR = 0; iLR < LR_NUM; iLR++)
            {
                _pLaneMerkerLine3D[iLR] = NULL;
            }
        }

        _pLaneParameter = NULL;
        {
            for(int iLR = 0; iLR < LR_NUM; iLR++)
            {
                _pLaneParameterOneSide[iLR] = NULL;
            }
        }

        _adPlane3D[0] = 0.0;
        _adPlane3D[1] = 0.0;
        _adPlane3D[2] = 0.0;
        {
            for(int iLR = 0; iLR < LR_NUM; iLR++)
            {
                _iLaneBoundaryFoundCounter[iLR] = 0;
                _iLaneBoundaryLostCounter[iLR] = 0;
                _iLaneBoundarySolidCounter[iLR] = 0;
                _iLaneBoundaryDashCounter[iLR] = 0;

            }
        }
        _iLaneBoundaryType[LR_LEFT][0] = LBT_NONE;
        _iLaneBoundaryType[LR_LEFT][1] = LBT_NONE;
        _iLaneBoundaryType[LR_RIGHT][0] = LBT_NONE;
        _iLaneBoundaryType[LR_RIGHT][1] = LBT_NONE;

        _pEdgePointsOnRoad = NULL;

        _iIsrcMin = 0;
        _iIsrcMax = 0;
        _iJsrcLeft_At_IsrcMin = 0;
        _iJsrcRight_At_IsrcMin = 0;
        _iJsrcLeft_At_IsrcMax = 0;
        _iJsrcRight_At_IsrcMax = 0;

        _dSearchMarginInNearArea = DEFAULT_SEARCH_MARGIN_IN_NEAR_AREA;
        _dSearchMarginInFarArea = DEFAULT_SEARCH_MARGIN_IN_FAR_AREA;

        _dMaxDiffFromMedianDeviationInNearArea = DEFAULT_MAX_DIFF_FROM_MEDIAN_DEVIATION_IN_NEAR_AREA;
        _dMaxDiffFromMedianDeviationInFarArea = DEFAULT_MAX_DIFF_FROM_MEDIAN_DEVIATION_IN_FAR_AREA;

        for(int iLR = 0; iLR < LR_NUM; iLR++)
        {
            _iEdgeStrengthOfLaneBoundary[iLR] = 0;
            _bSearchInDefaultLanePosition[iLR] = FALSE;
            _iIsrcAvailable[iLR] = 0;
        }
        _iBothSideLostCounter = 0;
        _iLaneParameterType = LPT_NONE;
        _dPitchEstimated = 0.;
        _dPitchPredicted = 0.;

        _piProcLine = NULL;
        _iProcLineNumber = 0;

        _iLineNumberFarArea = 0;
        _iLineNumberNearArea = 0;
        for(int iLR = 0; iLR < LR_NUM; iLR++)
        {
            _faTopIntensity[iLR] = new FlexArray<int>;
        }
        _pGridMap1D = new GridMap1D();
#ifdef	USE_EDGEPOINT2D
        _ppEdgePoint2D = NULL;
#endif	USE_EDGEPOINT2D
    }
    inline ~LaneArea(void)
    {
        {
            for(int iLR = 0; iLR < LR_NUM; iLR++)
            {
                SAFE_DELETE(_pLaneBoundaryPointsToEstimateLaneParameter[iLR]);
            }
        }
        {
            for(int iLR = 0; iLR < LR_NUM; iLR++)
            {
                SAFE_DELETE(_pLaneBoundaryPointsInFarArea[iLR]);
            }
        }
        {
            for(int iLR = 0; iLR < LR_NUM; iLR++)
            {
                SAFE_DELETE(_pLaneBoundaryPointsInNearArea[iLR]);
            }
        }
        {
            for(int iLR = 0; iLR < LR_NUM; iLR++)
            {
                SAFE_DELETE(_pLaneBoundaryPoints[iLR]);
            }
        }
        SAFE_DELETE(_pLaneParameter);
        {
            for(int iLR = 0; iLR < LR_NUM; iLR++)
            {
                SAFE_DELETE(_pLaneParameterOneSide[iLR]);
            }
        }
        {
            for(int iLR = 0; iLR < LR_NUM; iLR++)
            {
                SAFE_DELETE(_pLaneMerkerLineSequences[iLR]);
            }
        }
        {
            for(int iLR = 0; iLR < LR_NUM; iLR++)
            {
                SAFE_DELETE(_pLaneMerkerLineSequence3D[iLR]);
            }
        }
        {
            for(int iLR = 0; iLR < LR_NUM; iLR++)
            {
                SAFE_DELETE(_pLaneMerkerLine3D[iLR]);
            }
        }

        SAFE_DELETE(_pEdgePointsOnRoad);
        SAFE_DELETE(_piProcLine);
        for(int iLR = 0; iLR < LR_NUM; iLR++)
        {
            SAFE_DELETE(_faTopIntensity[iLR]);
        }
        SAFE_DELETE(_pGridMap1D);
#ifdef	USE_EDGEPOINT2D
        SAFE_DELETE_ARRAY(_ppEdgePoint2D);
#endif	USE_EDGEPOINT2D
    }
    inline void initialize(void)
    {
        for(int iNF = 0; iNF < NF_NUM; iNF++)
        {
            Region(iNF)->initialize();
        }
        getLaneParameter()->initialize();
    }
    inline void initialize(PARAM_CAM *pCamParam)
    {
        for(int iNF = 0; iNF < NF_NUM; iNF++)
        {
            Region(iNF)->initialize();
        }
        setupVotingThreshold();
        SAFE_DELETE(_pLaneParameter);
        _pLaneParameter = new LaneParameter(pCamParam);
        {
            for(int iLR = 0; iLR < LR_NUM; iLR++)
            {
                SAFE_DELETE(_pLaneParameterOneSide[iLR]);
                int iK = iLR == LR_LEFT ? -1 : 1;
                _pLaneParameterOneSide[iLR] = new LaneParameterOneSide(pCamParam, iK);
            }
        }
    }
    inline void setupVotingThreshold(void)
    {
        for(int iNF = 0; iNF < NF_NUM; iNF++)
        {
            for(int iLR = 0; iLR < LR_NUM; iLR++)
            {
                for(int iUD = 0; iUD < UD_NUM; iUD++)
                {
#ifndef	FOR_TME
                    int iVT = Region(iNF)->LineNumber() / VOTING_THRESHOLD_RATIO;
//					int iVT = Region(iNF)->LineNumber() / 10;	// 20111020
#else	FOR_TME
                    int iVT = Region(iNF)->LineNumber() / 10;
#endif	FOR_TME
                    Region(iNF)->Side(iLR)->BB(iUD)->VotingThreshold(iVT);
                }
            }
        }
    }
    inline LaneRegion *Region(int iNF)
    {
        return &(_Regions[iNF]);
    }

    inline LaneParameter *getLaneParameter(void)
    {
        return _pLaneParameter;
    }
    inline LaneParameterOneSide *getLaneParameterOneSide(int iLR)
    {
        return _pLaneParameterOneSide[iLR];
    }

    inline void setLaneBoundaryPointsToEstimateLaneParameter(int iLR, LaneMarkerPoints *p)
    {
        SAFE_DELETE(_pLaneBoundaryPointsToEstimateLaneParameter[iLR]);
        _pLaneBoundaryPointsToEstimateLaneParameter[iLR] = p;
    }
    inline LaneMarkerPoints *getLaneBoundaryPointsToEstimateLaneParameter(int iLR)
    {
        return _pLaneBoundaryPointsToEstimateLaneParameter[iLR];
    }
    inline void setLaneBoundaryPointsInFarArea(int iLR, LaneMarkerPoints *p)
    {
        SAFE_DELETE(_pLaneBoundaryPointsInFarArea[iLR]);
        _pLaneBoundaryPointsInFarArea[iLR] = p;
    }
    inline void setLaneBoundaryPointsInNearArea(int iLR, LaneMarkerPoints *p)
    {
        SAFE_DELETE(_pLaneBoundaryPointsInNearArea[iLR]);
        _pLaneBoundaryPointsInNearArea[iLR] = p;
    }
    inline void setLaneBoundaryPoints(int iLR, LaneMarkerPoints *p)
    {
        SAFE_DELETE(_pLaneBoundaryPoints[iLR]);
        _pLaneBoundaryPoints[iLR] = p;
    }

    inline LaneMarkerPoints *getLaneBoundaryPointsInNearArea(int iLR)
    {
        return _pLaneBoundaryPointsInNearArea[iLR];
    }
    inline LaneMarkerPoints *getLaneBoundaryPointsInFarArea(int iLR)
    {
        return _pLaneBoundaryPointsInFarArea[iLR];
    }
    inline LaneMarkerPoints *getLaneBoundaryPoints(int iLR)
    {
        return _pLaneBoundaryPoints[iLR];
    }
    inline void setLaneMarkerLineSequences(int iLR, LaneMarkerLineSequences *p)
    {
        SAFE_DELETE(_pLaneMerkerLineSequences[iLR]);
        _pLaneMerkerLineSequences[iLR] = p;
    }
    inline LaneMarkerLineSequences *getLaneMarkerLineSequences(int iLR)
    {
        return _pLaneMerkerLineSequences[iLR];
    }
    inline void setLaneMarkerLineSequence3D(int iLR, LaneMarkerLineSequence *p)
    {
        SAFE_DELETE(_pLaneMerkerLineSequence3D[iLR]);
        _pLaneMerkerLineSequence3D[iLR] = p;
    }
    inline LaneMarkerLineSequence *getLaneMarkerLineSequence3D(int iLR)
    {
        return	_pLaneMerkerLineSequence3D[iLR];
    }
    inline void setLaneMarkerLine3D(int iLR, LaneMarkerLine *p)
    {
        SAFE_DELETE(_pLaneMerkerLine3D[iLR]);
        _pLaneMerkerLine3D[iLR] = p;
    }
    inline LaneMarkerLine *getLaneMarkerLine3D(int iLR)
    {
        return	_pLaneMerkerLine3D[iLR];
    }
    inline LaneMarkerPoints *getEdgePointsOnRoad(void)
    {
        return _pEdgePointsOnRoad;
    }
    inline void setEdgePointsOnRoad(LaneMarkerPoints *p)
    {
        SAFE_DELETE(_pEdgePointsOnRoad);
        _pEdgePointsOnRoad = p;
    }

    void detectLaneMarkerPointsOneLine(PARAM_CAM *pCamParam, int iIsrc, int iProcLineIndex, Uchar *pImageAll, LaneMarkerPoints *pUpEdgePoints, LaneMarkerPoints *pDownEdgePoints, Disparity *pDisparity);
    BOOL detectLaneMarkerPoints(PARAM_CAM *pCamParam, Uchar *pImage, LaneMarkerPoints *pUpEdgePoints, LaneMarkerPoints *pDownEdgePoints, Disparity *pDisparity = NULL);
    BOOL detectLaneMarkers(PARAM_CAM *pCamParam, LaneMarkerPoints *pUpEdgePoints, LaneMarkerPoints *pDownEdgePoints, Uchar *pucInputImage);
    BOOL selectLaneMarkers(void);
    BOOL selectLaneMarkers(LaneParameter *pLaneParameter, LaneParameterOneSide *pLeftLaneParameter, LaneParameterOneSide *pRightLaneParameter);
    void detectLaneMarkerPointsOneLine(PARAM_CAM *pCamParam,int iIsrc,int iProcLineIndex,Uchar *pImageAll,LaneMarkerPoints *pUpEdgePoints,LaneMarkerPoints *pDownEdgePoints,int iPositiveThresholdLeft,int iNegativeThresholdLeft,int iPositiveThresholdRight,int iNegativeThresholdRight,Disparity *pDisparity,LaneMarkerPoint **ppEdgePoint2D);
    LaneMarker *searchLaneMarkerForLaneBoundaryByVotes(LaneMarkers *pLMs, int iUD, int iOffsetCenter, int iOffsetMin, int iOffsetMax, int iYawCenter, int iYawMin, int iYawMax);
    LaneMarkerLineSequence *searchLaneMarkerLineSequenceForLaneBoundaryByVotes(LaneMarkerLineSequences *pLMLSs, int iUD, int iBottomXCenter, int iBottomXMin, int iBottomXMax, int iYawCenter, int iYawMin, int iYawMax);
    LaneMarkerLineSequence *searchLaneMarkerLineSequenceForLaneBoundary2(LaneMarkerLineSequences *pLMLSs, int iUD,int iNearBottomXCenter, int iNearBottomXMin, int iNearBottomXMax,int iNearTopXCenter, int iNearTopXMin, int iNearTopXMax);
    LaneMarkerLineSequence *searchLaneMarkerLineSequenceForLaneBoundaryByVotes2(LaneMarkerLineSequences *pLMLSs, int iUD,int iNearBottomXCenter, int iNearBottomXMin, int iNearBottomXMax,int iNearTopXCenter, int iNearTopXMin, int iNearTopXMax);
    LaneMarkerLine *searchLaneMarkerLineForLaneBoundary2(LaneMarkerLines *pLMLs,int iBottomXCenter, int iBottomXMin, int iBottomXMax,int iTopXCenter, int iTopXMin, int iTopXMax);
    double searchMedian(FlexArray<double> *pfadDeviations);
    LaneMarker *searchLaneMarkerForLaneBoundary(LaneMarkers *pLMs,int iUD,int iOffsetCenter,int iOffsetMin,int iOffsetMax,int iYawCenter,int iYawMin,int iYawMax);
    LaneMarkerLine *searchLaneMarkerLineForLaneBoundary(LaneMarkerLines *pLMLs, int iOffsetCenter, int iOffsetMin, int iOffsetMax, int iYawCenter, int iYawMin, int iYawMax);
    LaneMarker *searchLaneMarkerForLaneBoundaryByVotes(LaneMarkers *pLMs,int iUD,int iBottomXCenter,int iBottomXMin,int iBottomXMax,int iTopZ,int iTopXCenter,int iTopXMin,int iTopXMax);
    int calcAverageOfHighIntensityArea(PARAM_CAM *pCamParam, int iLR, Uchar *pInputImage, int iIsrc);


    double calcYawDiffNearAndFar(int iNF0, int iNF1);
    double calcYawDiffNearAndFar(int iLR, int iNF0, int iNF1);
    int calcNewThOfEdgeStrength(LaneMarkerPoints *pLMPs);
    int calcEdgeStrength(PARAM_CAM *pCamParam, int iIsrc, Uchar *pImage, int iJsrc);
    LaneMarkerLineSequence *searchLaneMarkerLineSequenceForLaneBoundary(LaneMarkerLineSequences *pLMLSs,int iUD,int iBottomXCenter,int iBottomXMin,int iBottomXMax, int iYawCenter, int iYawMin, int iYawMax);
    LaneMarker *searchLaneMarkerForLaneBoundaryByInitialParameter(int iNF, int iLR);							// 01
    LaneMarkerLineSequence *searchLaneMarkerLineSequenceForLaneBoundaryByInitialParameter(int iLR);				// 02
    LaneMarker *searchLaneMarkerForLaneBoundaryByNearerRegion(int iNF, int iLR);								// 03
    LaneMarker *searchLaneMarkerForLaneBoundaryByPreviousFrame(int iNF, int iLR);								// 04
    LaneMarker *searchLaneMarkerForLaneBoundaryByPreviousFurtherFrame(int iNF, int iLR);
    LaneMarkerLineSequence *searchLaneMarkerLineSequenceForLaneBoundaryByPreviousFrame(int iLR);				// 05
    LaneMarker *searchLaneMarkerForLaneBoundaryByParameter(int iNF, int iLR);									// 06
    LaneMarker *searchLaneMarkerForLaneBoundaryByNearerRegionAndParameter(int iNF, int iLR);					// 07
    LaneMarkerLineSequence *searchLaneMarkerLineSequenceForLaneBoundaryByParameter(int iLR);					// 08
    LaneMarkerLine *searchLaneMarkerLineForLaneBoundaryByParameter(int iNF, int iLR, int iUD);					// 09
    LaneMarkerLine *searchLaneMarkerLineForLaneBoundaryByNearerRegionAndParameter(int iNF, int iLR, int iUD);	// 10

    BOOL pickupLaneBoundaryPoints(PARAM_CAM *pCamParam, LaneMarkerPoints *pUpEdgePoints, LaneMarkerPoints *pDownEdgePoints);
//	LaneMarkerPoints *searchLaneBoundaryPointsInFarArea(PARAM_CAM *pCamParam, LaneMarkerPoints *pUpEdgePoints, LaneMarkerPoints *pDownEdgePoints, Disparity *pDisparity = NULL);
    BOOL eliminateIsoletedBoundaryPointsInFarArea(PARAM_CAM *pCamParam, LaneMarkerPoints *pLMPs);
    BOOL eliminateIsoletedBoundaryPointsInFarArea(PARAM_CAM *pCamParam);
    BOOL eliminateIsoletedBoundaryPointsInNearArea(PARAM_CAM *pCamParam);
    LaneMarkerPoints *searchLaneBoundaryPointsInFarArea(PARAM_CAM *pCamParam, int iLR, LaneMarkerPoints *pUpEdgePoints, LaneMarkerPoints *pDownEdgePoints, Disparity *pDisparity, double dZMax = -1.);
    LaneMarkerPoints *searchLaneBoundaryPointsInFarAreaForSolidLine(PARAM_CAM *pCamParam, LaneMarkerPoints *pUpEdgePoints, LaneMarkerPoints *pDownEdgePoints, Disparity *pDisparity = NULL);
    LaneMarkerPoints *searchLaneBoundaryPointsInFarAreaForSolidLine(PARAM_CAM *pCamParam, int iLR, LaneMarkerPoints *pUpEdgePoints, LaneMarkerPoints *pDownEdgePoints, Disparity *pDisparity = NULL);
    LaneMarkerPoints *searchLaneBoundaryPointsInFarAreaForSolidLine(PARAM_CAM *pCamParam, int iLR, LaneMarkerPoints *pUpEdgePoints, LaneMarkerPoints *pDownEdgePoints, Disparity *pDisparity = NULL, double dZMax = -1.);
    BOOL searchLaneBoundaryPointsInNearArea(PARAM_CAM *pCamParam, LaneMarkerPoints *pUpEdgePoints, LaneMarkerPoints *pDownEdgePoints, Disparity *pDisparity = NULL);
    BOOL searchLaneBoundaryPointsInNearArea(PARAM_CAM *pCamParam, int iLR, LaneMarkerPoints *pUpEdgePoints, LaneMarkerPoints *pDownEdgePoints, Disparity *pDisparity = NULL);
    BOOL selectLaneBoundaryPointsToEstimateLaneParameter(PARAM_CAM *pCamParam);
    BOOL selectLaneBoundaryPointsToEstimateLaneParameterWithFarArea(PARAM_CAM *pCamParam);
    BOOL estimateLaneParameter(PARAM_CAM *pCamParam);
    BOOL decideDetectionStatus(PARAM_CAM *pCamParam);
    BOOL checkAndAdjustLaneParameters(void);
    BOOL decideLaneMarkerType(PARAM_CAM *pCamParam);
    BOOL checkLaneBoundaryDetectionStatus(PARAM_CAM *pCamParam);

    BOOL calc3DLineOfLaneBoundaries(PARAM_CAM *pCamParam, LaneMarkerPoints *pUpEdgePoints, LaneMarkerPoints *pDownEdgePoints);
    BOOL calc3DLineOfLaneMarkers(PARAM_CAM *pCamParam, LaneMarkerPoints *pUpEdgePoints, LaneMarkerPoints *pDownEdgePoints);
    BOOL calc3DLineOfLaneMarkerLines(PARAM_CAM *pCamParam, LaneMarkerPoints *pUpEdgePoints, LaneMarkerPoints *pDownEdgePoints);
    BOOL calcAverageEdgeStrengthOfLaneMarkerLines(void);
    BOOL eliminateShortLaneMarkerLines(PARAM_CAM *pCamParam);
    BOOL eliminateFewPointLaneMarkerLines(PARAM_CAM *pCamParam);
    BOOL estimate3Dplane(void);
    BOOL estimate3DSurface(LaneMarkerPoints *pLMPs);
    BOOL estimate3DSurface(void);
    BOOL pickupLaneBoundaryPointsOnRoadRegion(PARAM_CAM *pCamParam,  LaneMarkerPoints *pEdgePointsOnRoad);
    BOOL pickupLaneMarkerPointsOnRoadRegion(PARAM_CAM *pCamParam, LaneMarkerPoints *pUpEdgePoints, LaneMarkerPoints *pDownEdgePoints, LaneMarkerPoints *pEdgePointsOnRoad);
    inline int getIsrcMin(void)
    {
        return _iIsrcMin;
    }
    inline int getIsrcMax(void)
    {
        return _iIsrcMax;
    }
    inline int getJsrcLeft_At_IsrcMin(void)
    {
        return _iJsrcLeft_At_IsrcMin;
    }
    inline int getJsrcRight_At_IsrcMin(void)
    {
        return _iJsrcRight_At_IsrcMin;
    }
    inline int getJsrcLeft_At_IsrcMax(void)
    {
        return _iJsrcLeft_At_IsrcMax;
    }
    inline int getJsrcRight_At_IsrcMax(void)
    {
        return _iJsrcRight_At_IsrcMax;
    }

    BOOL adjustEdgeThreshold(void);
    inline double *getPlane3D(void)
    {
        return _adPlane3D;
    }
    inline double *getSurface3D(void)
    {
        return _adSurface3D;
    }

    inline int LaneBoundaryFoundCounter(int iLR)
    {
        return	_iLaneBoundaryFoundCounter[iLR];
    }
    inline int LaneBoundaryLostCounter(int iLR)
    {
        return	_iLaneBoundaryLostCounter[iLR];
    }
    inline void clearLaneBoundaryFoundCounter(int iLR)
    {
        _iLaneBoundaryFoundCounter[iLR] = 0;
    }
    inline void clearLaneBoundaryLostCounter(int iLR)
    {
        _iLaneBoundaryLostCounter[iLR] = 0;
    }
    inline void incLaneBoundaryFoundCounter(int iLR)
    {
        _iLaneBoundaryFoundCounter[iLR]++;
    }
    inline void incLaneBoundaryLostCounter(int iLR)
    {
        _iLaneBoundaryLostCounter[iLR]++;
    }
    inline int LaneBoundarySolidCounter(int iLR)
    {
        return	_iLaneBoundarySolidCounter[iLR];
    }
    inline int LaneBoundaryDashCounter(int iLR)
    {
        return	_iLaneBoundaryDashCounter[iLR];
    }
    inline void clearLaneBoundarySolidCounter(int iLR)
    {
        _iLaneBoundarySolidCounter[iLR] = 0;
    }
    inline void clearLaneBoundaryDashCounter(int iLR)
    {
        _iLaneBoundaryDashCounter[iLR] = 0;
    }
    inline void incLaneBoundarySolidCounter(int iLR)
    {
        _iLaneBoundarySolidCounter[iLR]++;
    }
    inline void incLaneBoundaryDashCounter(int iLR)
    {
        _iLaneBoundaryDashCounter[iLR]++;
    }

    inline int LaneBoundaryType(int iLR, int iIdx)
    {
        return _iLaneBoundaryType[iLR][iIdx];
    }
    inline void LaneBoundaryType(int iLR, int iIdx, int iV)
    {
        _iLaneBoundaryType[iLR][iIdx] = iV;
    }


    inline double SearchMarginInNearArea(void)
    {
        return _dSearchMarginInNearArea;
    }
    inline void SearchMarginInNearArea(double v)
    {
        _dSearchMarginInNearArea = v;
    }
    inline double SearchMarginInFarArea(void)
    {
        return _dSearchMarginInFarArea;
    }
    inline void SearchMarginInFarArea(double v)
    {
        _dSearchMarginInFarArea = v;
    }
    inline double MaxDiffFromMedianDeviationInNearArea(void)
    {
        return 	_dMaxDiffFromMedianDeviationInNearArea;
    }
    inline void MaxDiffFromMedianDeviationInNearArea(double v)
    {
        _dMaxDiffFromMedianDeviationInNearArea = v;
    }
    inline double MaxDiffFromMedianDeviationInFarArea(void)
    {
        return 	_dMaxDiffFromMedianDeviationInFarArea;
    }
    inline void MaxDiffFromMedianDeviationInFarArea(double v)
    {
        _dMaxDiffFromMedianDeviationInFarArea = v;
    }


    BOOL detectLaneMarkerLineSequences(PARAM_CAM *pCamParam);
    BOOL checkLaneMarkerLineSequenceOn3DPlaneBoundaryByPointNumber(PARAM_CAM *pCamParam, Disparity *pDisparity, double *pdPlane3D, int iLR, LaneMarkerLineSequence *pLMLS);
    BOOL checkLaneMarkerLineSequenceOn3DPlaneBoundaryByAverageHeight(PARAM_CAM *pCamParam, Disparity *pDisparity, double *pdPlane3D, int iLR, LaneMarkerLineSequence *pLMLS);
    BOOL checkLaneMarkerLineSequenceOn3DPlaneBoundary(PARAM_CAM *pCamParam, Disparity *pDisparity, double *pdPlane3D, LaneMarkerLineSequence *pLMLS);
    BOOL checkLaneMarkerLineSequencesOn3DPlaneBoundary(PARAM_CAM *pCamParam, Disparity *pDisparity);
    BOOL checkLaneMarkerLinesOn3DPlaneBoundary(PARAM_CAM *pCamParam, Disparity *pDisparity);
    BOOL checkLaneMarkerLineSequenceOn3DPlaneBoundaryByPointsOnLine(PARAM_CAM *pCamParam, Disparity *pDisparity, double *pdPlane3D, int iLR, LaneMarkerLineSequence *pLMLS);
    BOOL checkLaneMarkerLineOn3DPlaneBoundaryByPointsOnLine(PARAM_CAM *pCamParam, Disparity *pDisparity, double *pdPlane3D, int iLR, LaneMarkerLine *pLML);

    BOOL checkLaneMarkerLineOn3DPlaneBoundaryByPointsOnLine(PARAM_CAM *pCamParam, Disparity *pDisparity);
//	BOOL checkLaneMarkerLineOn3DPlaneBoundaryByPointsOnLine(PARAM_CAM *pCamParam, Disparity *pDisparity, double *pdPlane3D, int iLR, LaneMarkerLine *pLML);
    LaneMarkerLineSequence *searchLaneMarkerLineSequenceOn3DPlaneBoundary(PARAM_CAM *pCamParam, int iLR, LaneMarkerLine *pLML, LaneMarkerLineSequences *pLMLSs);
    LaneMarkerLine *searchLaneMarkerLineOn3DPlaneBoundary(PARAM_CAM *pCamParam, int iLR, LaneMarkerLine *pLML, LaneMarkerLines *pLMLUps, LaneMarkerLines *pLMLDowns);
    LaneMarkerLine *searchLaneMarkerLineOn3DPlaneBoundary(PARAM_CAM *pCamParam, int iLR, LaneMarkerLine *pLML, LaneMarkerLine *pLMLOutSide, LaneMarkerLines *pLMLUps, LaneMarkerLines *pLMLDowns);
    BOOL searchLaneMarkerLineSequenceOn3DPlaneBoundary(PARAM_CAM *pCamParam);
    BOOL searchLaneMarkerLineOn3DPlaneBoundary(PARAM_CAM *pCamParam);

    inline int EdgeStrengthOfLaneBoundary(int iLR)
    {
        return _iEdgeStrengthOfLaneBoundary[iLR];
    }
    inline void EdgeStrengthOfLaneBoundary(int iLR, int iV)
    {
        _iEdgeStrengthOfLaneBoundary[iLR] = iV;
    }
    inline BOOL SearchInDefaultLanePositionFlag(int iLR)
    {
        return _bSearchInDefaultLanePosition[iLR];
    }
    inline void SearchInDefaultLanePositionFlag(int iLR, BOOL bV)
    {
        _bSearchInDefaultLanePosition[iLR] = bV;
    }

    inline int IsrcAvailable(int iLR)
    {
        return _iIsrcAvailable[iLR];
    }
    inline void IsrcAvailable(int iLR, int iV)
    {
        _iIsrcAvailable[iLR] = iV;
    }

    inline int getBothSideLostCounter(void)
    {
        return _iBothSideLostCounter;
    }
    inline void setBothSideLostCounter(int iV)
    {
        _iBothSideLostCounter = iV;
    }
    inline void incBothSideLostCounter(void)
    {
        _iBothSideLostCounter++;
    }
    inline void clearBothSideLostCounter(void)
    {
        _iBothSideLostCounter = 0;
    }


    inline int getLaneParameterType(void)
    {
        return _iLaneParameterType;
    }
    inline void setLaneParameterType(int iV)
    {
        _iLaneParameterType = iV;
    }

    inline double getPitchEstimated(void)
    {
        return _dPitchEstimated;
    }
    inline void setPitchEstimated(double dV)
    {
        _dPitchEstimated = dV;
    }
    inline double getPitchPredicted(void)
    {
        return _dPitchPredicted;
    }
    inline void setPitchPredicted(double dV)
    {
        _dPitchPredicted = dV;
    }


    inline int ProcLineNumber(void)
    {
        return _iProcLineNumber;
    }
    inline void ProcLineNumber(int iV)
    {
        _iProcLineNumber = iV;
    }
    inline void ProcLine(int *p)
    {
        SAFE_DELETE(_piProcLine);
        _piProcLine = p;
    }
    inline int *ProcLine(void)
    {
        return _piProcLine;
    }
//	BOOL calcProcLine(PARAM_CAM *pCamParam, int iMaxLineNumberOfRegion0, int iMaxLineNumberOfRegion1);
    BOOL calcProcLine(PARAM_CAM *pCamParam, int iIstepNearArea, int iMaxLineNumberRegion0, int iMaxLineNumberRegion1, int iIstepFarArea);
//	BOOL calcLineNumber(PARAM_CAM *pCamParam);

    inline int LineNumberFarArea(void)
    {
        return 	_iLineNumberFarArea;
    }
    inline void LineNumberFarArea(int iV)
    {
        _iLineNumberFarArea = iV;
    }
    inline int LineNumberNearArea(void)
    {
        return 	_iLineNumberNearArea;
    }
    inline void LineNumberNearArea(int iV)
    {
        _iLineNumberNearArea = iV;
    }

    LaneMarkerPoints *searchLaneMarkerPointsByLaneParameter(PARAM_CAM *pCamParam, LaneParameterOneSide *pLaneParameter, Disparity *pDisparity, double dXVehicle, double dZVehicleMin, double dZVehicleMax);
    BOOL calc3DplaneByLaneParameter(PARAM_CAM *pCamParam, LaneParameterOneSide *pLaneParameter, Disparity *pDisparity, double dXVehicleDiff, double dZVehicleMin, double dZVehicleMax, double *dPlane3D);
    BOOL calc3DplaneByLaneParameter(PARAM_CAM *pCamParam, int iK, LaneParameterOneSide *pLaneParameter, Disparity *pDisparity);

    BOOL checkMismatchOfLaneMarkerPointsFromLaneParameter(PARAM_CAM *pCamParam, LaneParameterOneSide *pLaneParameterOneSide, LaneMarkerPoints *pLaneMarkerPoints, LaneMarkerPoints *pNewLaneMarkerPoints);

    BOOL removeMismatchedDistanceLaneBoundaryPoints(PARAM_CAM *pCamParam, int iLR);
    BOOL removeIsolatedLaneBoundaryPoints(PARAM_CAM *pCamParam, int iLR);
    BOOL removeIsolatedLaneBoundaryPoints_2(PARAM_CAM *pCamParam, int iLR);

    inline void addTopIntensity(int iLR, int iV)
    {
        if(_faTopIntensity[iLR]->getNumber() >= 10)
        {
            _faTopIntensity[iLR]->remove(0);
        }
        _faTopIntensity[iLR]->add(iV);
    }
    inline int getTopIntensity(int iLR)
    {
        int iIdxMax = -1;
        int iVMax = -1;
        for(int iIdx = 0; iIdx < _faTopIntensity[iLR]->getNumber(); iIdx++)
        {
            int iV = _faTopIntensity[iLR]->get(iIdx);
            if(iVMax < iV)
            {
                iVMax = iV;
                iIdxMax = iIdx;
            }
        }
        return iVMax;
    }
    inline GridMap1D *getGridMap1D(void)
    {
        return _pGridMap1D;
    }
    BOOL voteToGridMap(PARAM_CAM *pCamParam, Disparity *pDisparity, int iIsrcMin, int iIsrcMax, double *pdOffset);
    inline OutputInfo *getOutputInfo(void)
    {
        return &_OutputInfo;
    }
    inline void SetOutputInfo(void)
    {
        LaneParameterOneSide *pLPLeft = getLaneParameterOneSide(LR_LEFT);
        LaneParameterOneSide *pLPRight = getLaneParameterOneSide(LR_RIGHT);
        getOutputInfo()->Pitch(atan(getPlane3D()[1]) / M_PI * 180.);
        getOutputInfo()->Cv(getSurface3D()[1]);
        getOutputInfo()->LeftOffset(pLPLeft->Param(LPID_OFFSET) / 1000.);
        getOutputInfo()->LeftYaw(pLPLeft->Param(LPID_YAW) / M_PI * 180.);
        getOutputInfo()->LeftC0(pLPLeft->Param(LPID_CURVATURE) * 1000.);
        getOutputInfo()->LeftC1(pLPLeft->Param(LPID_CURVATUREDASH) * 1000. * 1000.);
        getOutputInfo()->LeftAvailableDistance(pLPLeft->getDetectedDistance() / 1000.);
        getOutputInfo()->LeftCurbOffset(pLPLeft->getDeltaOffsetCurb() / 1000 + pLPLeft->Param(LPID_OFFSET) / 1000.);
        getOutputInfo()->RightOffset(pLPRight->Param(LPID_OFFSET) / 1000.);
        getOutputInfo()->RightYaw(pLPRight->Param(LPID_YAW) / M_PI * 180.);
        getOutputInfo()->RightC0(pLPRight->Param(LPID_CURVATURE) * 1000.);
        getOutputInfo()->RightC1(pLPRight->Param(LPID_CURVATUREDASH) * 1000. * 1000.);
        getOutputInfo()->RightAvailableDistance(pLPRight->getDetectedDistance() / 1000.);
        getOutputInfo()->RightCurbOffset(pLPRight->getDeltaOffsetCurb() / 1000. + pLPRight->Param(LPID_OFFSET) / 1000.);
    }

};
#endif // _LANE_AREA_H_
