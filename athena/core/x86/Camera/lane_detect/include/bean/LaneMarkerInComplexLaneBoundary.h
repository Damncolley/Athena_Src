#pragma once

const int MaximumLaneMarkerNumberInComplexLaneBoundary = 3;
const int MaximumFrameNumberForComplexLaneBoundary = 10;
const int CLBT_NONE		=	0;
const int CLBT_SINGLE	=	1;
const int CLBT_DOUBLE	=	2;
const int CLBT_TRIPLE	=	3;

class LaneMarkerInComplexLaneBoundary
{
private:
    int _iLineType;
    LaneMarker *_pLM;
    FlexArray<int> *_faiWidth;
    FlexArray<int> *_faiPointNumber;
public:
    inline LaneMarkerInComplexLaneBoundary(void)
    {
        _iLineType = LBT_NONE;
        _pLM = NULL;
        _faiWidth = new FlexArray<int>;
        _faiPointNumber = new FlexArray<int>;
    }
    inline LaneMarkerInComplexLaneBoundary(LaneMarker *pLM)
    {
        _pLM = NULL;
        if(pLM != NULL)
        {
            _pLM = new LaneMarker(pLM);
        }
        _iLineType = LBT_NONE;
        _faiWidth = new FlexArray<int>;
        _faiPointNumber = new FlexArray<int>;
    }
    inline ~LaneMarkerInComplexLaneBoundary(void)
    {
//		SAFE_DELETE(_faiWidth);
//		SAFE_DELETE(_faiPointNumber);
        SAFE_DELETE(_pLM);
        delete _faiWidth;
        delete _faiPointNumber;
    }
    inline LaneMarker *getLaneMarker(void)
    {
        return _pLM;
    }
    inline void setLaneMarker(LaneMarker *pLM)
    {
        SAFE_DELETE(_pLM);
        _pLM = pLM;
    }
    inline int LineType(void)
    {
        return _iLineType;
    }
    inline void LineType(int iV)
    {
        _iLineType = iV;
    }
    inline FlexArray<int> *Widths(void)
    {
        return _faiWidth;
    }
    inline FlexArray<int> *PointNumbers(void)
    {
        return _faiPointNumber;
    }
    inline void addWidth(int iV)
    {
        if(Widths() == NULL)	return;
        int iNumber = Widths()->getNumber();
        if(iNumber >= MaximumFrameNumberForComplexLaneBoundary)
        {
            Widths()->remove(0);
        }
        Widths()->add(iV);
    }
    inline void addPointNumber(int iV)
    {
        if(PointNumbers() == NULL)	return;
        int iNumber = PointNumbers()->getNumber();
        if(iNumber >= MaximumFrameNumberForComplexLaneBoundary)
        {
            PointNumbers()->remove(0);
        }
        PointNumbers()->add(iV);
    }
    inline int AverageWidth(void)
    {
        if(Widths() == NULL)	return -1;
        int iNumber = Widths()->getNumber();
        int iSum = 0;
        for(int iIdx = 0; iIdx < iNumber; iIdx++)
        {
            iSum += Widths()->get(iIdx);
        }
        return iSum / iNumber;
    }
    inline int AveragePointNumber(void)
    {
        if(PointNumbers() == NULL)	return -1;
        int iNumber = PointNumbers()->getNumber();
        int iSum = 0;
        for(int iIdx = 0; iIdx < iNumber; iIdx++)
        {
            iSum += PointNumbers()->get(iIdx);
        }
        return iSum / iNumber;
    }

};

