#pragma once
#include "../utils/config.h"
#include "../utils/tmc_stereobmp-forMono.h"
#include "LaneDetectorTools.h"
#include "LaneMarkerLine.h"

class LaneMarkerLineSequence
{
private:
    int _iUD;	// UD_UP or UD_DOWN
    BOOL	_bOn3DPlaneBoundary;
    double _adAverageHeight[2];
    LaneMarkerLine *_pLML[NF_NUM];

public:
    inline LaneMarkerLineSequence(void)
    {
        _iUD = -1;
        _bOn3DPlaneBoundary = FALSE;
        _adAverageHeight[0] = 0;
        _adAverageHeight[1] = 0;
        _pLML[NF_NEAR] = NULL;
        _pLML[NF_FAR] = NULL;
    }
    inline LaneMarkerLineSequence(LaneMarkerLineSequence *p)
    {
        UD(p->UD());
        setFlagOn3DPlaneBoundary(p->getFlagOn3DPlaneBoundary());
        for(int iNF = 0; iNF < NF_NUM; iNF++)
        {
            LaneMarkerLine *pLML = p->getLaneMarkerLine(iNF);
            _pLML[iNF] = new LaneMarkerLine(pLML);
        }
    }
    inline ~LaneMarkerLineSequence(void)
    {
        for(int iNF = 0; iNF < NF_NUM; iNF++)
        {
            SAFE_DELETE(_pLML[iNF]);
        }
    }
    inline int UD(void)
    {
        return _iUD;
    }
    inline void UD(int iUD)
    {
        _iUD = iUD;
    }
    inline LaneMarkerLine	*Near(void)
    {
        return _pLML[NF_NEAR];
    }
    inline LaneMarkerLine	*Far(void)
    {
        return _pLML[NF_FAR];
    }
    inline void	Near(LaneMarkerLine *p)
    {
        _pLML[NF_NEAR] = p;
    }
    inline void	Far(LaneMarkerLine *p)
    {
        _pLML[NF_FAR] = p;
    }
    inline LaneMarkerLine *getLaneMarkerLine(int iNF)
    {
        return _pLML[iNF] ;
    }
    inline void setLaneMarkerLine(int iNF, LaneMarkerLine *pLML)
    {
        _pLML[iNF] = pLML;
    }
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

typedef LaneMarkerLineSequence * ptrLaneMarkerLineSequence;
