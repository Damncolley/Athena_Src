#ifndef	_LaneDetector_H_
#define	_LaneDetector_H_

#include "utils/config.h"
#include "utils/config2.h"	// configuration file
#include "utils/tmc_stereobmp-forMono.h"
#include "bean/LaneParameterEstimator.h"
#include "bean/LaneArea.h"
#include "utils/imrgb.h"
#include "utils/my_resource.h"
#include "utils/roadimage_window.h"

double getSelfVelocity(void);
void setSelfVelocity(double dV);

class LaneDetector
{
private:
//	int *_piProcLine;
//	Uchar *_pucProcLineImage;
    PARAM_CAM *_pCamParam;
    int _aiProcLineNumber[NF_NUM];
    LaneArea _pLaneArea;

    LaneMarkerPoints *_pUpEdgePoints;
    LaneMarkerPoints *_pDownEdgePoints;
    Disparity *_pDisparity;

public:
    inline LaneDetector(void)
    {
        _aiProcLineNumber[NF_NEAR] = 30;
        _aiProcLineNumber[NF_FAR] = 20;
        _pUpEdgePoints = new LaneMarkerPoints;
        _pDownEdgePoints = new LaneMarkerPoints;
        _pDisparity = NULL;
        _pCamParam = NULL;
    }
    inline ~LaneDetector(void)
    {
        SAFE_DELETE(_pUpEdgePoints);
        SAFE_DELETE(_pDownEdgePoints)
        _pCamParam = NULL;
        SAFE_DELETE(_pDisparity);
        SAFE_DELETE(_pCamParam);
    }
    inline LaneArea *Area(void)
    {
        return &_pLaneArea;
    }
    BOOL initialize(void);
    BOOL initialize(int iRegionHeight);
    BOOL initialize(PARAM_CAM *pCamParam);
    BOOL initializeWithAVIFileName(char *pcAVIFileName);
    BOOL detect(Uchar *pInputImage);
//	BOOL detect(IplImage *pInputImage);
//	BOOL detect(IplImage *pInputImage, double dVelocity);
    BOOL detect(IplImage *pInputImage, double dVelocity, Disparity *pDisparity);

    BOOL sortFlexArrayDouble(FlexArray<double> *pfa);
    BOOL sortLaneMarkerLinesByOffset(LaneMarkerLines *pLMLs);
    BOOL checkMismatchOfLaneMarkerPointsFromLaneParameter(PARAM_CAM *pCamParam, LaneParameterOneSide *pLPOneSide, LaneMarkerPoints *pLMPs, LaneMarkerPoints *pNewLMPs);
    BOOL getMinMax(LaneMarkerPoints *pLMPs, double *pdXmin, double *pdXmax, double *pdYmin, double *pdYmax, double *pdZmin, double *pdZmax);
    BOOL getYMin(LaneMarkerPoints *pLMPs, double *pdYmin);
    LaneMarkerPoints *mergeLaneMarkerPointsHead(LaneMarkerPoints *pBaseLMPs,LaneMarkerPoints *pLMPs);
    LaneMarkerPoints *mergeLaneMarkerPointsTail(LaneMarkerPoints *pBaseLMPs,LaneMarkerPoints *pLMPs);

    inline PARAM_CAM *CameraParam(void)
    {
        return _pCamParam;
    }
    inline int &ProcLineNumber(int iIdx)
    {
        return _aiProcLineNumber[iIdx];
    }
//	void searchTopsInBallotBox(void);
    inline LaneMarkerPoints *getUpEdgePoints(void)
    {
        return _pUpEdgePoints;
    }
    inline LaneMarkerPoints *getDownEdgePoints(void)
    {
        return _pDownEdgePoints;
    }
    inline void setCamParam(PARAM_CAM *p)
    {
        if(p == NULL)	return;
        SAFE_DELETE(_pCamParam);
        _pCamParam = new PARAM_CAM;
        if(_pCamParam == NULL)	return;
        (*_pCamParam) = *p;
    }
    inline PARAM_CAM *getCamParam(void)
    {
        return _pCamParam;
    }
    inline Disparity *getDisparity(void)
    {
        return _pDisparity;
    }
    inline void setDisparity(Disparity *p)
    {
        SAFE_DELETE(_pDisparity);
        _pDisparity = p;
    }
};
extern BOOL getMinMax(LaneMarkerPoints *pLMPs, double *pdXmin, double *pdXmax, double *pdYmin, double *pdYmax, double *pdZmin, double *pdZmax);
extern BOOL getYMin(LaneMarkerPoints *pLMPs, double *pdYmin);

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
#endif	_LaneDetector_H_
