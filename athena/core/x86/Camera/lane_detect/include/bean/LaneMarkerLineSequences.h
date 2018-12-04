#pragma once

#include "../utils/type.h"
#include "../utils/flexarray.h"
#include "LaneMarkerLineSequence.h"

class LaneMarkerLineSequences
{
private:
    FlexArray<ptrLaneMarkerLineSequence>	*_faLaneMarkerLineSequences;

public:
    LaneMarkerLineSequences(void);
    ~LaneMarkerLineSequences(void);
    LaneMarkerLineSequence *getLaneMarkerLineSequence(int idx);
    void deleteLaneMarkerLineSequence(void);
    void deleteLaneMarkerLineSequence(int iIdx);
    int getLaneMarkerLineSequenceNumber(void);
    void addLaneMarkerLineSequence(LaneMarkerLineSequence *pLaneMarkerLineSequence);
    inline void remove_delete(int iIdx)
    {
        _faLaneMarkerLineSequences->remove_delete(iIdx);
    }
    inline void clear_reset(void)
    {
        _faLaneMarkerLineSequences->clear();
        _faLaneMarkerLineSequences->reset();
    }
    inline void reset(void)
    {
        _faLaneMarkerLineSequences->reset();
    }
    void set(int iIdx, LaneMarkerLineSequence *pLaneMarkerLineSequence);
};
