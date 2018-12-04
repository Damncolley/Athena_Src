#pragma once
#include "../utils/type.h"
#include "../utils/config.h"
#include "../utils/flexarray.h"
#include "LaneMarkerInComplexLaneBoundary.h"

class ComplexLaneBoundary
{

private:
    int _iMode;
    int _iLaneMarkerNumberCounter[MaximumLaneMarkerNumberInComplexLaneBoundary + 1];
    LaneMarkerInComplexLaneBoundary *_apLaneMarkerInComplexLaneBoundary[MaximumLaneMarkerNumberInComplexLaneBoundary];


public:
    inline ComplexLaneBoundary(void)
    {
        _iMode = CLBT_NONE;
        for(int iIdx = 0; iIdx <= MaximumLaneMarkerNumberInComplexLaneBoundary; iIdx++)
        {
            _apLaneMarkerInComplexLaneBoundary[iIdx] = NULL;
            _iLaneMarkerNumberCounter[iIdx] = 0;
        }
    }
    inline ~ComplexLaneBoundary(void)
    {
        for(int iIdx = 0; iIdx < MaximumLaneMarkerNumberInComplexLaneBoundary; iIdx++)
        {
            SAFE_DELETE(_apLaneMarkerInComplexLaneBoundary[iIdx]);
        }
    }
    inline int Mode(void)
    {
        return _iMode;
    }
    inline void Mode(int iV)
    {
        _iMode = iV;
    }
    inline int getLaneMarkerCounter(int iIdx)
    {
        return _iLaneMarkerNumberCounter[iIdx];
    }
    inline void clearLaneMarkerCounter(int iIdx)
    {
        _iLaneMarkerNumberCounter[iIdx] = 0;
    }
    inline void incLaneMarkerCounter(int iIdx)
    {
        _iLaneMarkerNumberCounter[iIdx]++;
    }

    inline void clearLaneMarkerCounter(void)
    {
        for(int iIdx = 0; iIdx <= MaximumLaneMarkerNumberInComplexLaneBoundary; iIdx++)
        {
            _iLaneMarkerNumberCounter[iIdx] = 0;
        }
    }

    inline LaneMarkerInComplexLaneBoundary *getLaneMarker(int iIdx)
    {
        if(iIdx < 0)
           return NULL;
        if(iIdx >= MaximumLaneMarkerNumberInComplexLaneBoundary)
           return NULL;
        return _apLaneMarkerInComplexLaneBoundary[iIdx];
    }

    inline void setLaneMarker(int iIdx, LaneMarkerInComplexLaneBoundary *p)
    {
        if(iIdx < 0)
           return ;
        if(iIdx >= MaximumLaneMarkerNumberInComplexLaneBoundary)
           return;
        SAFE_DELETE(_apLaneMarkerInComplexLaneBoundary[iIdx]);
        _apLaneMarkerInComplexLaneBoundary[iIdx] = p;
    }
};
