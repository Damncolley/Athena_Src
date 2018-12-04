#pragma once
#include "../utils/config.h"
#include "LaneMarkerLine.h"
#include "LaneDetectorTools.h"

class LaneMarker
{
protected:
    LaneMarkerLine	*_pLM[UD_NUM];
    int _iAverageIntensity;

public:
    inline LaneMarker(void)
    {
        _pLM[UD_UP] = NULL;
        _pLM[UD_DOWN] = NULL;
        _iAverageIntensity = -1;
    }

    inline LaneMarker(LaneMarker *p)
    {
        for(int iUD = 0; iUD < UD_NUM; iUD++)
        {
            _pLM[iUD] = new LaneMarkerLine(p->getLaneMarkerLine(iUD));
        }
        _iAverageIntensity = p->AverageIntensity();
    }

    inline LaneMarker(LaneMarkerLine *pLMLUp, LaneMarkerLine *pLMLDown)
    {
        _pLM[UD_UP] = new LaneMarkerLine(pLMLUp);
        _pLM[UD_DOWN] = new LaneMarkerLine(pLMLDown);
        _iAverageIntensity = -1;
    }

    inline ~LaneMarker(void)
    {
        for(int iUD = 0; iUD < UD_NUM; iUD++)
        {
            SAFE_DELETE(_pLM[iUD]);
        }
    }
    inline LaneMarkerLine	*Up(void)
    {
        return _pLM[UD_UP];
    }
    inline LaneMarkerLine	*Down(void)
    {
        return _pLM[UD_DOWN];
    }
    inline void	Up(LaneMarkerLine *p)
    {
        _pLM[UD_UP] = p;
    }
    inline void	Down(LaneMarkerLine *p)
    {
        _pLM[UD_DOWN] = p;
    }
    inline LaneMarkerLine *getLaneMarkerLine(int iUD)
    {
        return _pLM[iUD] ;
    }
    inline void setLaneMarkerLine(int iUD, LaneMarkerLine *pLML)
    {
        _pLM[iUD] = pLML;
    }
    inline int AverageIntensity(void)
    {
        return 	_iAverageIntensity;
    }
    inline void AverageIntensity(int iV)
    {
        _iAverageIntensity = iV;
    }
};

typedef LaneMarker * ptrLaneMarker;
