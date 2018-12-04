#pragma once
#include "../utils/type.h"
#include "../utils/flexarray.h"
#include "LaneMarkerPoint.h"

class LaneMarkerPoints
{
private:
    FlexArray<ptrLaneMarkerPoint>	*_faLaneMarkerPoints;
    double _dAverageEdgeStrength;
public:
    LaneMarkerPoints(void);
    LaneMarkerPoints(LaneMarkerPoints *pSrc);
    ~LaneMarkerPoints(void);
    LaneMarkerPoint *getLaneMarkerPoint(int idx);
    void deleteLaneMarkerPoint(void);
    void deleteLaneMarkerPoint(int iIdx);
    int getLaneMarkerPointNumber(void);
    void addLaneMarkerPoint(int iIsrc, int iJsrc, double dEdgeStrength, double dGrandient, int iProcLineIndex, double dDisparity, double dX = 0.0, double dY = 0.0, double dZ = 0.0);
    void addLaneMarkerPoint(LaneMarkerPoint *pLMP);
    inline void remove_delete(int iIdx)
    {
        _faLaneMarkerPoints->remove_delete(iIdx);
    }
    inline void clear_reset(void)
    {
        _faLaneMarkerPoints->clear();
        _faLaneMarkerPoints->reset();
    }
    inline void reset(void)
    {
        _faLaneMarkerPoints->reset();
    }
    void set(int iIdx, LaneMarkerPoint *pLaneMarkerPoint);
    BOOL calcCentroid(double *pdCentroid);
    BOOL doPCA(double *pdAxis, double *pdVar);
    BOOL findMinMax(double *pdMinMax);
    inline double AverageEdgeStrength(void)
    {
        return _dAverageEdgeStrength;
    }
    void calcAverageEdgeStrength(BOOL bForce = FALSE);
};
