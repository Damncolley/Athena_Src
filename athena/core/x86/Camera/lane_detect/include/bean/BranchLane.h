#pragma once

class BranchLane
{
private:
    BOOL _bFlag;	// �C3???E��L???t?��?O
    LaneMarker *_LaneMarkerMain;	// �C{?�����衱��?��
    LaneMarker *_LaneMarkerBranch;	// ?a?�����衱��?��
public:
    BranchLane(void):_bFlag(FALSE), _LaneMarkerMain(NULL), _LaneMarkerBranch(NULL)	{}
    void flag(BOOL v)
    {
        _bFlag = v;
    }
    BOOL flag(void)
    {
        return _bFlag;
    }
    LaneMarker *getLaneMarkerMain(void)
    {
        return _LaneMarkerMain;
    }
    void getLaneMarkerMain(LaneMarker *p)
    {
        _LaneMarkerMain = p;
    }
    LaneMarker *getLaneMarkerBranch(void)
    {
        return _LaneMarkerBranch;
    }
    void getLaneMarkerBranch(LaneMarker *p)
    {
        _LaneMarkerBranch = p;
    }
};
