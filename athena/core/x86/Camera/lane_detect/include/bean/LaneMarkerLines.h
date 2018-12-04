#pragma once

#include "../utils/type.h"
#include "../utils/flexarray.h"
#include "LaneMarkerLine.h"

class LaneMarkerLines
{
private:
    FlexArray<ptrLaneMarkerLine> *_faLaneMarkerLines;

public:
    LaneMarkerLines(void);
    LaneMarkerLines(LaneMarkerLines *pSrc);
    ~LaneMarkerLines(void);
    LaneMarkerLine *getLaneMarkerLine(int idx);
    void deleteLaneMarkerLine(void);
    void deleteLaneMarkerLine(int iIdx);
    int getLaneMarkerLineNumber(void);
    void addLaneMarkerLine(int iOffset, int iYaw, int iVotes);
    void addLaneMarkerLine(LaneMarkerLine *p);

    inline void remove_delete(int iIdx)
    {
        _faLaneMarkerLines->remove_delete(iIdx);
    }
    inline void clear_reset(void)
    {
        _faLaneMarkerLines->clear();
        _faLaneMarkerLines->reset();
    }
    inline void reset(void)
    {
        _faLaneMarkerLines->reset();
    }
    void set(int iIdx, LaneMarkerLine *pLaneMarkerLine);
};
