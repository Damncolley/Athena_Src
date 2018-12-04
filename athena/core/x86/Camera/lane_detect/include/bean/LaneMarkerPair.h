#pragma once
#include "../utils/type.h"
#include "../utils/config.h"
#include "../utils/flexarray.h"
#include "LaneMarker.h"

class LaneMarkerPair
{
private:
    LaneMarker	*_pLaneMarker[LR_NUM];

public:
    inline LaneMarkerPair(void)
    {
        for(int iIdx = 0; iIdx < LR_NUM; iIdx++)
        {
            _pLaneMarker[iIdx]	=	NULL;
        }
    }
    inline ~LaneMarkerPair(void)
    {
        for(int iIdx = 0; iIdx < LR_NUM; iIdx++)
        {
            SAFE_DELETE(_pLaneMarker[iIdx]);
        }
    }
    inline LaneMarker	*Left(void)
    {
        return _pLaneMarker[LR_LEFT];
    }
    inline LaneMarker	*Right(void)
    {
        return _pLaneMarker[LR_RIGHT];
    }
    inline void	Left(LaneMarker *p)
    {
        SAFE_DELETE(_pLaneMarker[LR_LEFT]);
        _pLaneMarker[LR_LEFT] = p;
    }
    inline void	Right(LaneMarker *p)
    {
        SAFE_DELETE(_pLaneMarker[LR_RIGHT]);
        _pLaneMarker[LR_RIGHT] = p;
    }
    inline LaneMarker	*getLaneMarker(int iLR)
    {
        return _pLaneMarker[iLR];
    }
    inline void  getLaneMarker(int iLR, LaneMarker *p)
    {
        SAFE_DELETE(_pLaneMarker[iLR]);
        _pLaneMarker[iLR] = p;
    }
};

typedef LaneMarkerPair * ptrLaneMarkerPair;
