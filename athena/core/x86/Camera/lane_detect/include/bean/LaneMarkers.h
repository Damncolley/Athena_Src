#pragma once

#include "../utils/type.h"
#include "../utils/flexarray.h"
#include "LaneMarker.h"

class LaneMarkers
{
private:
    FlexArray<ptrLaneMarker>	*_faLaneMarkers;

public:
    LaneMarkers(void);
    ~LaneMarkers(void);
    LaneMarker *getLaneMarker(int idx);
    void deleteLaneMarker(void);
    void deleteLaneMarker(int iIdx);
    int getLaneMarkerNumber(void);
    void addLaneMarker(LaneMarker *pLaneMarker);

    inline void remove_delete(int iIdx)
    {
        _faLaneMarkers->remove_delete(iIdx);
    }
    inline void clear_reset(void)
    {
        _faLaneMarkers->clear();
        _faLaneMarkers->reset();
    }
    inline void reset(void)
    {
        _faLaneMarkers->reset();
    }
    void set(int iIdx, LaneMarker *pLaneMarker);
};
