#pragma once
#include "../utils/type.h"
#include "../utils/config.h"
#include "LaneMarkerPoints.h"

class LaneMarkerLine
{
private:
    int _iOffset;
    int _iYaw;
    int _iVotes;

    double _ad3DLine[2][3];
    int _iDirection;
    LaneMarkerPoints *_pLaneMarkerPoints;
    BOOL	_bOn3DPlaneBoundary;
    double _adAverageHeight[2];

public:
    inline LaneMarkerLine(void)	:
        _iOffset(-1),
        _iYaw(-1),
        _iVotes(-1),
        _iDirection(-1)
    {
        for(int iIdx0 = 0; iIdx0 < 2; iIdx0++)
        {
            for(int iIdx1 = 0; iIdx1 < 3; iIdx1++)
            {
                _ad3DLine[iIdx0][iIdx1] = 0.0;
            }
        }
        _pLaneMarkerPoints = NULL;
        _bOn3DPlaneBoundary = FALSE;
        _adAverageHeight[0] = 0;
        _adAverageHeight[1] = 0;
    }

    inline LaneMarkerLine(int iOffset, int iYaw, int iVotes)
    {
        _iOffset = iOffset;
        _iYaw = iYaw;
        _iVotes = iVotes;
        _iDirection = -1;
        for(int iIdx0 = 0; iIdx0 < 2; iIdx0++)
        {
            for(int iIdx1 = 0; iIdx1 < 3; iIdx1++)
            {
                _ad3DLine[iIdx0][iIdx1] = 0.0;
            }
        }
        _pLaneMarkerPoints = NULL;
        _bOn3DPlaneBoundary = FALSE;
        _adAverageHeight[0] = 0;
        _adAverageHeight[1] = 0;
    }
    inline LaneMarkerLine(LaneMarkerLine *pSrc)
    {
        if(pSrc == NULL)	return;
        _iOffset = pSrc->offset();
        _iYaw = pSrc->yaw();
        _iVotes = pSrc->votes();
        _iDirection = pSrc->direction();
        for(int iIdx0 = 0; iIdx0 < 2; iIdx0++)
        {
            for(int iIdx1 = 0; iIdx1 < 3; iIdx1++)
            {
                _ad3DLine[iIdx0][iIdx1] = (pSrc->get3DLine(iIdx0))[iIdx1];
            }
        }
        if(pSrc->getLaneMarkerPoints() != NULL)
        {
            _pLaneMarkerPoints = new LaneMarkerPoints(pSrc->getLaneMarkerPoints());
        }
        _bOn3DPlaneBoundary = pSrc->getFlagOn3DPlaneBoundary();
        _adAverageHeight[0] = pSrc->getAverageHeight()[0];
        _adAverageHeight[1] = pSrc->getAverageHeight()[1];
    }

    inline ~LaneMarkerLine(void)
    {
        SAFE_DELETE(_pLaneMarkerPoints);
    }

    inline void offset(int v)
    {
        _iOffset = v;
    }
    inline void yaw(int v)
    {
        _iYaw = v;
    }
    inline void votes(int v)
    {
        _iVotes = v;
    }
    inline int offset(void)
    {
        return _iOffset;
    }
    inline int yaw(void)
    {
        return _iYaw;
    }
    inline int votes(void)
    {
        return _iVotes;
    }
    inline void direction(int v)
    {
        _iDirection = v;
    }
    inline int direction(void)
    {
        return _iDirection;
    }
    inline double *get3DLine(int iIdx)
    {
        return _ad3DLine[iIdx];
    }
    inline LaneMarkerPoints *getLaneMarkerPoints(void)
    {
        return _pLaneMarkerPoints;
    }
    inline void setLaneMarkerPoints(LaneMarkerPoints *pLMP)
    {
        _pLaneMarkerPoints = pLMP;
    }
    BOOL calcPosOn3DLine(double *pdX, double *pdY, double dZ);
    BOOL calcAngleOn3DLine(double *pdPitch, double *pdYaw);
    inline BOOL getFlagOn3DPlaneBoundary(void)
    {
        return	_bOn3DPlaneBoundary;
    }
    inline void setFlagOn3DPlaneBoundary(BOOL bV)
    {
        _bOn3DPlaneBoundary = bV;
    }
    inline double *getAverageHeight(void)
    {
        return _adAverageHeight;
    }
};

typedef LaneMarkerLine * ptrLaneMarkerLine;
