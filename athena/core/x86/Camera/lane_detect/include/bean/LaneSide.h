#pragma once
#include "../utils/type.h"
#include "../utils/flexarray.h"
#include "LaneMarkerPoints.h"
#include "LaneMarkerLines.h"
#include "LaneMarkers.h"
#include "BallotBox.h"
#include "ComplexLaneBoundary.h"

class LaneSide
{
private:
    double _dPos[2][2];	// ?H¨C¨º¡ª¨¬???¨¦?`¡¯[¡°_
    int _iRoadImageWidth;	// ?H¨C¨º¡ë?¡®??¡­???T?C?Y[pix]
    int _iRoadImageHeight;	// ?H¨C¨º¡ë?¡®???¡¯??T?C?Y[pix]
    int _iEffectiveArea[4][2];
    double _dEffectiveAreaLine[2][2];

    LaneMarkerLines	*_pLaneMarkerLines[UD_NUM];	// ???¨¹???E?G?b?W?¨¹?a(???a?¨¨-¡ëo?a?¨¨)
    LaneMarkers *_pLaneMarkers;	// ¡±¡¯?¨¹?¨®?a

    LaneMarker *_pLaneBoundary;		// ???¨¹???E¡±¡¯?¨¹
    LaneMarkerPoints *_pLaneBoundaryPoints;	// ???¨¹???E¡°_???X?g

    BallotBox *_pBB[UD_NUM];	// ¡°??[¡±?
    int _iThreshold[UD_NUM];	// ?G?b?W?-¡°x¨¨?¡¯l

    int _iFoundCounter;		// ¡±-?????¨¬?A¡®¡À???o?J?E?¡°?^
    int _iLostCounter;		// ¡¯??????¨¬?A¡®¡À¨C¡é???o?J?E?¡°?^

    BOOL _bFoundNow;		// ???t???[?€?¨¬???¨¹???E?¨¬???o?E¨C¡é???o?¨®¡®?
    BOOL _bFound;			// ???¨¹???E?¨¬???o?E¨C¡é???o?¨®¡®?

    int _iLaneBoundaryType[2];	// ???¨¹???E?¨¬?¨ª?¨º	¡±¡¯?¨¹ or ?G?b?W?¨¹, ???a?¨¨?G?b?W or ¡ëo?a?¨¨?G?b?W

    ComplexLaneBoundary _lComplexLaneBoundary;
    LaneMarkers *_pParallelLMs;
public:
    inline LaneSide(void)
    {
        {
            for(int iIdx0 = 0; iIdx0 < 2; iIdx0++)
            {
                for(int iIdx1 = 0; iIdx1 < 2; iIdx1++)
                {
                    _dPos[iIdx0][iIdx1] = 0.0;
                }
            }
        }
        _iRoadImageWidth = 0;
        _iRoadImageHeight = 0;
        {
            for(int iIdx0 = 0; iIdx0 < 4; iIdx0++)
            {
                for(int iIdx1 = 0; iIdx1 < 2; iIdx1++)
                {
                    _iEffectiveArea[iIdx0][iIdx1] = 0;
                }
            }
        }

        {
            for(int iIdx0 = 0; iIdx0 < UD_NUM; iIdx0++)
            {
                _pLaneMarkerLines[iIdx0] = NULL;
                _pBB[iIdx0] = NULL;
            }
        }

        _iThreshold[UD_UP]		= getPositiveThresholdDefault();
        _iThreshold[UD_DOWN]	= getNegativeThresholdDefault();
        _pLaneMarkers = NULL;
        _pLaneBoundary = NULL;
        _pLaneBoundaryPoints = NULL;

        _iFoundCounter = 0;
        _iLostCounter = 0;
        _bFoundNow = FALSE;
        _bFound = FALSE;
        _iLaneBoundaryType[0] = LBT_NONE;
        _iLaneBoundaryType[1] = LBT_NONE;
        _pParallelLMs = NULL;
    }
    inline ~LaneSide(void)
    {
        {
            for(int iUD = 0; iUD < UD_NUM; iUD++)
            {
                SAFE_DELETE(_pLaneMarkerLines[iUD]);
            }
        }
        SAFE_DELETE(_pLaneMarkers);

        SAFE_DELETE(_pLaneBoundary);
        SAFE_DELETE(_pLaneBoundaryPoints);

        {
            for(int iUD = 0; iUD < UD_NUM; iUD++)
            {
                SAFE_DELETE(_pBB[iUD]);
            }
        }
        SAFE_DELETE(_pParallelLMs);
    }
    void initialize(void);

    inline int LaneBoundaryType(int iIdx)
    {
        return _iLaneBoundaryType[iIdx];
    }
    inline void LaneBoundaryType(int iIdx, int iV)
    {
        _iLaneBoundaryType[iIdx] = iV;
    }

    inline BallotBox *BB(int iUD)
    {
        return _pBB[iUD];
    }
    inline void BB(int iUD, BallotBox *pV)
    {
        SAFE_DELETE(_pBB[iUD]);
        _pBB[iUD] = pV;
    }
    void BB_diff(void);
    inline int Threshold(int iUD)
    {
        return _iThreshold[iUD];
    }
    inline void Threshold(int iUD, int iV)
    {
        _iThreshold[iUD] = iV;
    }
    inline void resetThreshold(int iUD)
    {
        if(iUD == UD_UP)
        {
            _iThreshold[iUD] = getPositiveThresholdDefault();
        }
        else
        {
            _iThreshold[iUD] = getNegativeThresholdDefault();
        }
    }
    inline void resetThreshold(void)
    {
        for(int iUD = 0; iUD < UD_NUM; iUD++)
        {
            resetThreshold(iUD);
        }
    }
    BOOL adjustEdgeThreshold(void);
    inline int getFoundCounter(void)
    {
        return _iFoundCounter;
    }
    inline void clearFoundCounter(void)
    {
        _iFoundCounter = 0;
    }
    inline void incFoundCounter(void)
    {
        _iFoundCounter++;
    }
    inline int getLostCounter(void)
    {
        return _iLostCounter;
    }
    inline void clearLostCounter(void)
    {
        _iLostCounter = 0;
    }
    inline void incLostCounter(void)
    {
        _iLostCounter++;
    }

    inline BOOL getFoundNowFlag(void)
    {
        return _bFoundNow;
    }
    inline void clearFoundNowFlag(void)
    {
        _bFoundNow = FALSE;
    }
    inline void setFoundNowFlag(void)
    {
        _bFoundNow = TRUE;
    }

    inline BOOL getFoundFlag(void)
    {
        return _bFound;
    }
    inline void clearFoundFlag(void)
    {
        _bFound = FALSE;
    }
    inline void setFoundFlag(void)
    {
        _bFound = TRUE;
    }

    inline double &Top(void)
    {
        return _dPos[0][0];
    }
    inline double &Left(void)
    {
        return _dPos[0][1];
    }
    inline double &Bottom(void)
    {
        return _dPos[1][0];
    }
    inline double &Right(void)
    {
        return _dPos[1][1];
    }
    inline double Width(void)
    {
        return (Right() - Left());
    }
    inline double Height(void)
    {
        return (Top() - Bottom());
    }

    inline int RoadImageWidth(void)
    {
        return _iRoadImageWidth;
    }
    inline int RoadImageHeight(void)
    {
        return _iRoadImageHeight;
    }
    inline void RoadImageWidth(int iV)
    {
        _iRoadImageWidth = iV;
    }
    inline void RoadImageHeight(int iV)
    {
        _iRoadImageHeight = iV;
    }
    void CalcEffectiveEdgeArea(PARAM_CAM *pCamParam);
    inline int &EffectiveAreaTopLeftI(void)
    {
        return _iEffectiveArea[0][0];
    }
    inline int &EffectiveAreaTopLeftJ(void)
    {
        return _iEffectiveArea[0][1];
    }
    inline int &EffectiveAreaBottomLeftI(void)
    {
        return _iEffectiveArea[1][0];
    }
    inline int &EffectiveAreaBottomLeftJ(void)
    {
        return _iEffectiveArea[1][1];
    }
    inline int &EffectiveAreaTopRightI(void)
    {
        return _iEffectiveArea[2][0];
    }
    inline int &EffectiveAreaTopRightJ(void)
    {
        return _iEffectiveArea[2][1];
    }
    inline int &EffectiveAreaBottomRightI(void)
    {
        return _iEffectiveArea[3][0];
    }
    inline int &EffectiveAreaBottomRightJ(void)
    {
        return _iEffectiveArea[3][1];
    }

    inline int *getEffectiveAreaTopLeftI(void)
    {
        return &_iEffectiveArea[0][0];
    }
    inline int *getEffectiveAreaTopLeftJ(void)
    {
        return &_iEffectiveArea[0][1];
    }
    inline int *getEffectiveAreaBottomLeftI(void)
    {
        return &_iEffectiveArea[1][0];
    }
    inline int *getEffectiveAreaBottomLeftJ(void)
    {
        return &_iEffectiveArea[1][1];
    }
    inline int *getEffectiveAreaTopRightI(void)
    {
        return &_iEffectiveArea[2][0];
    }
    inline int *getEffectiveAreaTopRightJ(void)
    {
        return &_iEffectiveArea[2][1];
    }
    inline int *getEffectiveAreaBottomRightI(void)
    {
        return &_iEffectiveArea[3][0];
    }
    inline int *getEffectiveAreaBottomRightJ(void)
    {
        return &_iEffectiveArea[3][1];
    }

    inline double &EffectiveAreaLineLeftGradient(void)
    {
        return _dEffectiveAreaLine[0][0];
    }
    inline double &EffectiveAreaLineLeftIntercept(void)
    {
        return _dEffectiveAreaLine[0][1];
    }
    inline double &EffectiveAreaLineRightGradient(void)
    {
        return _dEffectiveAreaLine[1][0];
    }
    inline double &EffectiveAreaLineRightIntercept(void)
    {
        return _dEffectiveAreaLine[1][1];
    }
    inline LaneMarkerLines *getLaneMarkerLines(int iUD)
    {
        return _pLaneMarkerLines[iUD];
    }
    inline void setLaneMarkerLines(int iUD, LaneMarkerLines *p)
    {
        SAFE_DELETE(_pLaneMarkerLines[iUD]);
        _pLaneMarkerLines[iUD] = p;
    }
    inline LaneMarkers *getLaneMarkers(void)
    {
        return _pLaneMarkers;
    }
    inline void setLaneMarkers(LaneMarkers *p)
    {
        SAFE_DELETE(_pLaneMarkers);
        _pLaneMarkers = p;
    }
    LaneMarker *getLaneBoundary(void)
    {
        return _pLaneBoundary;
    }
    inline void setLaneBoundary(LaneMarker *p)
    {
        SAFE_DELETE(_pLaneBoundary);
        _pLaneBoundary = p;
    }
    LaneMarkerPoints *getLaneBoundaryPoints(void)
    {
        return _pLaneBoundaryPoints;
    }
    inline void setLaneBoundaryPoints(LaneMarkerPoints *p)
    {
        SAFE_DELETE(_pLaneBoundaryPoints);
        _pLaneBoundaryPoints = p;
    }

    BOOL IsEffectiveEdgeArea(int iIsrc, int iJsrc);

    BOOL transformInputToRoadImage(PARAM_CAM *pCamParam, int iIsrc, int iJsrc, int *piIdst, int *piJdst);
    inline int transformVehecleZToRoadI(double dZVehicle)
    {
        return (int)((Bottom() - dZVehicle) * RoadImageHeight() / (Top() - Bottom()) + RoadImageHeight());
    }
    inline int transformVehicleXToRoadJ(double dXVehicle)
    {
        return (int)(dXVehicle * RoadImageWidth() / (Right() - Left()) + RoadImageWidth() / 2.0);
    }
    void voteToRoadModel(int iUD,	int iIdst, int iJdst);
    void voteForAllProclines(void);
    void pairLaneMarkerLines(PARAM_CAM *pCamParam, Uchar *pucInputImage, int iTopIntensity);
    BOOL pickupLaneMarkerPointsOfLaneMarkerLines(PARAM_CAM *pCamParam, LaneMarkerPoints *pUpEdgePoints, LaneMarkerPoints *pDownEdgePoints, int iIsrcMin, int iIsrcMax);
    BOOL calc3DLineOfLaneMarkerLines(PARAM_CAM *pCamParam, LaneMarkerPoints *pUpEdgePoints, LaneMarkerPoints *pDownEdgePoints);
//	BOOL pickupLaneBoundaryPoints(PARAM_CAM *pCamParam, int iUD, LaneMarkerPoints *pEdgePoints);
    BOOL pickupEdgeLinePoints(PARAM_CAM *pCamParam, int iUD, int iIDx, LaneMarkerPoints *pEdgePoints, LaneMarkerPoints *pEdgeLinePoints, int iIsrcMin, int iIsrcMax);
    BOOL pickupEdgeLinePoints(PARAM_CAM *pCamParam, int iUD, LaneMarkerLine *pLML, LaneMarkerPoints *pEdgePoints, LaneMarkerPoints *pEdgeLinePoints, int iIsrcMin, int iIsrcMax);
    BOOL isVotedToRoadModel(int iUD, int iVotedOffset, int iVotedYaw, int iIdst, int iJdst);
    BOOL calc3DLine(PARAM_CAM *pCamParam, int iUD, int iIdx, LaneMarkerPoints *pEdgePoints, double *pd3DLine);
    BOOL calc3DLine(PARAM_CAM *pCamParam, int iUD, LaneMarkerLine *pLML, LaneMarkerPoints *pEdgePoints, double *pd3DLine);
    BOOL calc3DLine(PARAM_CAM *pCamParam, int iUD, int iIdx, LaneMarkerPoints *pEdgePoints);
    BOOL calc3DLine(PARAM_CAM *pCamParam, int iUD, LaneMarkerLine *pLML, LaneMarkerPoints *pEdgePoints);
    BOOL pickupLaneMarkerPointsOfLaneMarkerLines(PARAM_CAM *pCamParam, int iUD, LaneMarkerLine *pLML, LaneMarkerPoints *pEdgePoints, int iIsrcMin, int iIsrcMax);

    void transformInputToRoad(PARAM_CAM *pCamParam, int iIsrc, int iJsrc, double *pdZVehicle, double *pdXVehicle);
    int calcAverageIntensityBetweenLaneMarkerLines(PARAM_CAM *pCamParam,Uchar *pucInputImage,LaneMarkerLine *pLML0,LaneMarkerLine *pLML1);
    BOOL sortLaneMarkersByOffset(LaneMarkers *pLMs);
    BOOL isSameLineType(int iType0, int iType1);

    inline ComplexLaneBoundary *getComplexLaneBoundary(void)
    {
        return &_lComplexLaneBoundary;
    }
    BOOL setupComplexLaneBoundary(ComplexLaneBoundary *pCLB, LaneMarkers *pParallelLMs, int iLineNumber);
    BOOL updateComplexLaneBoundaryForCLBT_SINGLE(ComplexLaneBoundary *pCLB, LaneMarkers *pParallelLMs, int iLineNumber);
    BOOL updateComplexLaneBoundaryForCLBT_DOUBLE(ComplexLaneBoundary *pCLB, LaneMarkers *pParallelLMs, int iLineNumber);
    BOOL updateComplexLaneBoundaryForCLBT_TRIPLE(ComplexLaneBoundary *pCLB, LaneMarkers *pParallelLMs, int iLineNumber);
    BOOL checkComplexLaneBoundary(int iOffsetMin, int iOffsetMax, int iLineNumber);
    LaneMarker *selectLaneMarkerAtComplexLaneBoundary(int iUD);
    inline LaneMarkers *getParallelLMs(void)
    {
        return _pParallelLMs;
    }
    inline void setParallelLMs(LaneMarkers *p)
    {
        SAFE_DELETE(_pParallelLMs);
        _pParallelLMs = p;
    }

    LaneMarkers *checkParallelLaneMarkers(LaneMarkers *pLMs, int iOffsetMin, int iOffsetMax);
    int checkLaneMarkerType(int iLineNumber, LaneMarkerLine *pLML);
    int checkLaneMarkerType(int iLineNumber, LaneMarker *pLM);
};
