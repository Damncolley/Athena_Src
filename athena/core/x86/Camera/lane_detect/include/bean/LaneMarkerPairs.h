#pragma once
#include "../utils/type.h"
#include "../utils/flexarray.h"
#include "LaneMarkerPair.h"

class LaneMarkerPairs
{
private:
    FlexArray<ptrLaneMarkerPair>	*_faLaneMarkerPairs;

public:
    LaneMarkerPairs(void);
    ~LaneMarkerPairs(void);
    LaneMarkerPair *getLaneMarkerPair(int idx);
    void deleteLaneMarkerPair(void);
    void deleteLaneMarkerPair(int iIdx);
    int getLaneMarkerPairNumber(void);
    void addLaneMarkerPair(LaneMarkerPair *pLaneMarkerPair);

    inline void remove_delete(int iIdx)
    {
        _faLaneMarkerPairs->remove_delete(iIdx);
    }
    inline void clear_reset(void)
    {
        _faLaneMarkerPairs->clear();
        _faLaneMarkerPairs->reset();
    }
    inline void reset(void)
    {
        _faLaneMarkerPairs->reset();
    }
    void set(int iIdx, LaneMarkerPair *pLaneMarkerPair);
};
