#ifndef _LANE_REAGION_H_
#define _LANE_REAGION_H_

#include "../utils/type.h"
#include "../utils/config.h"
#include "../utils/flexarray.h"
#include "LaneMarkerPairs.h"
#include "LaneParameter.h"
#include "LaneParameterOneSide.h"
#include "LaneSide.h"



extern double g_dbOffsetSearchMarginSequence ;              // ??¨CT???¨¹???E??¡ë¡°?????¨¹???E?¨¬¡ª¨¬?????E???¨¬???¨º?¨¬?¨C¡ªe¡¯l // 20110703
extern double g_dbYawSearchMarginSequence ;    // 20110702
extern double g_dbOffsetSearchMarginTrack ;    // 20111014
extern double g_dbYawSearchMarginTrack ;

extern double g_dbOffsetSearchMarginForComplexLaneBoundary ;       // 20111202

class LaneRegion
{
private:
    double _dPos[2][2];
    int *_piProcLine;
    int _iProcLineNumber;
    int _iProcLineWidth;
    unsigned char *_pcProcLineImage;
    LaneSide _Side[LR_NUM];
    //LaneMarkerPair	_aLaneMarkerPairCandidate[CS4_LANES];
    int _iLaneMarkerPairNumber;
    BOOL _bAvailable;
    int _iRefLaneWidth;
    int _iRefLaneWidth_a;
    int _iRefLaneWidth_p;
    BOOL _bCrossLineClearFlag;

    //////////////////////////////////////////////////////////////////////////
    LaneMarkerPairs *_pLaneMarkerPairs,*_pLaneMarkerMul;

    int _iLineNumber;

#define	MAX_EDGE_POINT_NUM	1000
    int _aUpEdgeOnInputlines[MAX_EDGE_POINT_NUM][2];
    int _aDownEdgeOnInputlines[MAX_EDGE_POINT_NUM][2];
    int _aiUpEdgeOnRoad[MAX_EDGE_POINT_NUM][2];
    int _aiDownEdgeOnRoad[MAX_EDGE_POINT_NUM][2];
    int _iUpEdgePointNumber;
    int _iDownEdgePointNumber;

    int *_piUpEdgeOnOneline;
    int *_piDownEdgeOnOneline;

    int _iFoundCounter;
    int _iLostCounter;

    BOOL _bFoundNow;
    BOOL _bFound;

public:
    LaneRegion(void);
    inline ~LaneRegion(void)
    {
        SAFE_DELETE(_pLaneMarkerPairs);
    }
    void initialize(void);
    inline double Top(void)
    {
        return _dPos[0][0];
    }
    inline double Left(void)
    {
        return _dPos[0][1];
    }
    inline double Bottom(void)
    {
        return _dPos[1][0];
    }
    inline double Right(void)
    {
        return _dPos[1][1];
    }
    inline void Top(double dV)
    {
        _dPos[0][0] = dV;
    }
    inline void Left(double dV)
    {
        _dPos[0][1] = dV;
    }
    inline void Bottom(double dV)
    {
        _dPos[1][0] = dV;
    }
    inline void Right(double dV)
    {
        _dPos[1][1] = dV;
    }
    inline int ProcLineNumber(void)
    {
        return _iProcLineNumber;
    }
    inline void ProcLineNumber(int iV)
    {
        _iProcLineNumber = iV;
    }
    inline int &ProcLineWidth(void)
    {
        return _iProcLineWidth;
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
    inline unsigned char *ProcLineImage(void)
    {
        return _pcProcLineImage;
    }
    BOOL calcProcLine(PARAM_CAM *pCamParam, int iMaxLine);
    int calcProcLine(PARAM_CAM *pCamParam, int iSrcMin, int iSrcMax, int iMaxNumber);
    inline void clearUpEdgePointNumber(void)
    {
        _iUpEdgePointNumber = 0;
    }
    inline void clearDownEdgePointNumber(void)
    {
        _iDownEdgePointNumber = 0;
    }
    inline int &UpEdgePointNumber(void)
    {
        return _iUpEdgePointNumber;
    }
    inline int &DownEdgePointNumber(void)
    {
        return _iDownEdgePointNumber;
    }
    inline int &UpEdgeOnInputlinesI(int iIdx)
    {
        return _aUpEdgeOnInputlines[iIdx][0];
    }
    inline int &UpEdgeOnInputlinesJ(int iIdx)
    {
        return _aUpEdgeOnInputlines[iIdx][1];
    }
    inline int &DownEdgeOnInputlinesI(int iIdx)
    {
        return _aDownEdgeOnInputlines[iIdx][0];
    }

    inline LaneMarkerPairs *getLaneMarkerPairs(void)
    {
        return _pLaneMarkerPairs;
    }
    inline void setLaneMarkerPairs(LaneMarkerPairs *p)
    {
        SAFE_DELETE(_pLaneMarkerPairs);
        _pLaneMarkerPairs = p;
    }

    inline int &DownEdgeOnInputlinesJ(int iIdx)
    {
        return _aDownEdgeOnInputlines[iIdx][1];
    }

    BOOL transformInputToRoadImage(PARAM_CAM *pCamParam, int iIsrc, int iJsrc, int *piIdst,int *piJdst);

    inline int checkUpEdgeOnOneline(int j)
    {
        return _piUpEdgeOnOneline[j];
    }
    inline void setUpEdgeOnOneline(int j)
    {
        _piUpEdgeOnOneline[j] = 1;
    }
    inline void clearUpEdgeOnOneline(int j)
    {
        _piUpEdgeOnOneline[j] = 0;
    }
    inline int	checkDownEdgeOnOneline(int j)
    {
        return _piDownEdgeOnOneline[j];
    }
    inline void setDownEdgeOnOneline(int j)
    {
        _piDownEdgeOnOneline[j] = 1;
    }
    inline void clearDownEdgeOnOneline(int j)
    {
        _piDownEdgeOnOneline[j] = 0;
    }

    BOOL setDownEdgeOnInputlines(int iIsrc, int iJsrc);
    BOOL setUpEdgeOnInputlines(int iIsrc, int iJsrc);
    inline BOOL setDownEdgeOnRoad(int iIsrc, int iJsrc)
    {
        if(_iDownEdgePointNumber >= MAX_EDGE_POINT_NUM)	return FALSE;
        _aiDownEdgeOnRoad[_iUpEdgePointNumber][0] = iIsrc;
        _aiDownEdgeOnRoad[_iUpEdgePointNumber][1] = iJsrc;
        _iDownEdgePointNumber++;
        return TRUE;
    }
    inline BOOL setUpEdgeOnRoad(int iIsrc, int iJsrc)
    {
        if(_iUpEdgePointNumber >= MAX_EDGE_POINT_NUM)	return FALSE;
        _aiUpEdgeOnRoad[_iUpEdgePointNumber][0] = iIsrc;
        _aiUpEdgeOnRoad[_iUpEdgePointNumber][1] = iJsrc;
        _iUpEdgePointNumber++;
        return TRUE;
    }
    int  calcEdgeStrength(PARAM_CAM *pCamParam, unsigned char *pucBuf, int iIsrc, int iJsrc);
    void linefilter_DualEdge_sub(PARAM_CAM *pCamParam, int iIdx, int iJsrcMin, int iJsrcMax, unsigned char *pucBuf);
    void linefilter_DualEdge_SelectEdgePoint(PARAM_CAM *pCamParam, int iIsrc);
    void linefilter_DualEdge(PARAM_CAM *pCamParam, int iIdx, unsigned char *pucBuf);
    void linefilterForAllProclines(void);
    inline LaneSide *Side(int iLR)
    {
        return &(_Side[iLR]);
    }
//	inline LaneMarkerPair	&LaneMarkerPairCandidate(int iIdx)	{	return _aLaneMarkerPairCandidate[iIdx];	}
    inline int LaneMarkerPairNumber(void)
    {
        return _iLaneMarkerPairNumber;
    }
    inline BOOL &available(void)
    {
        return _bAvailable;
    }
    inline int &RefLaneWidth(void)
    {
        return _iRefLaneWidth;
    }
    inline int &RefLaneWidth_a(void)
    {
        return _iRefLaneWidth_a;
    }
    inline int &RefLaneWidth_p(void)
    {
        return _iRefLaneWidth_p;
    }
    inline BOOL &CrossLineClearFlag(void)
    {
        return _bCrossLineClearFlag;
    }

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

    inline void clearBallotBox(void)
    {
        for(int iLR = 0; iLR < LR_NUM; iLR++)
        {
            for(int iUD = 0; iUD < UD_NUM; iUD++)
            {
                Side(iLR)->BB(iUD)->clear();
            }
        }
    }
    void linefilterForAllProclines(PARAM_CAM *pCamParam);
    void voteForAllProclines(PARAM_CAM *pCamParam);
    void vote(PARAM_CAM *pCamParam);
    void vote(PARAM_CAM *pCamParam, LaneMarkerPoints *pUpEdgePoints, LaneMarkerPoints *pDownEdgePoints);
    inline void BB_diff(void)
    {
        for(int iLR = 0; iLR < LR_NUM; iLR++)
        {
            LaneSide *pSide = Side(iLR);
            if(pSide == NULL)	continue;
            pSide->BB_diff();
        }
    }
    void searchLaneMarkerLines(void);
    void pairLaneMarkerLines(PARAM_CAM *pCamParam, Uchar *pucInputImage, int aiTopIntensity[]);
    void pairLaneMarkers(void);
    BOOL pickupLaneMarkerPointsOfLaneMarkerLines(PARAM_CAM *pCamParam, LaneMarkerPoints *pUpEdgePoints, LaneMarkerPoints *pDownEdgePoints, int iIsrcMin, int iIsrcMax);
    BOOL calc3DLineOfLaneMarkerLines(PARAM_CAM *pCamParam, LaneMarkerPoints *pUpEdgePoints, LaneMarkerPoints *pDownEdgePoints);
    BOOL calcAverageEdgeStrengthOfLaneMarkerLines(void);
    BOOL eliminateShortLaneMarkerLines(PARAM_CAM *pCamParam);
    BOOL eliminateFewPointLaneMarkerLines(PARAM_CAM *pCamParam);

    BOOL searchLaneBoundary(LaneParameter *p);
    BOOL pickupLaneBoundaryPoints(PARAM_CAM *pCamParam, LaneMarkerPoints *pUpEdgePoints, LaneMarkerPoints *pDownEdgePoints, int iIsrcMin, int iIsrcMax);

    inline int LineNumber(void)
    {
        return _iLineNumber;
    }
    inline void LineNumber(int iV)
    {
        _iLineNumber = iV;
    }
    inline void calcLineNumber(PARAM_CAM *pCamParam)
    {
        int iIsrcMin =  calcHorizontalLineOfDepth(pCamParam, Top());
        int iIsrcMax =  calcHorizontalLineOfDepth(pCamParam, Bottom());

        int iLineNumber = iIsrcMax - iIsrcMin + 1;
        LineNumber(iLineNumber);
    }
    BOOL checkComplexLaneBoundary(LaneParameterOneSide *pLPLeft, LaneParameterOneSide *pLPRight);
};
#endif // _LANEAREA_
