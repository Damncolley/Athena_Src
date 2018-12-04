
#include "../utils/config.h"
#include "../utils/config2.h"
#include "../utils/matutil-d.h"
#include "../utils/tmc_stereobmp-forMono.h"
#include "LaneMarkerPoint.h"
#include "LaneMarkerPoints.h"

#pragma once

/////////////////////
#define	CS4_ONESIDE_STATUS_NUMBER	6
#define	CS4_ONESIDE_OBS_NUMBER	10
#define	CS4_ONESIDE_CONTROL_INPUT_NUMBER	2
#define	CS4_ONESIDE_STOCHASTIC_VARIAVLE_NUMBER	6 //3

class LaneParameterOneSide
{
private:
    static const int _iDim = CS4_ONESIDE_STATUS_NUMBER;
    BOOL _bAvailable;
    int _iK;
    int _iCounterAfterInitialization;
    double _dParameterInit[_iDim];
    double _dParameterMin[_iDim];
    double _dParameterMax[_iDim];
//	double _dParameter[_iDim];

    BOOL _bNotAddSystemNoise;

    int _s4_mobs;

    double _adb_X_t_t[	CS4_ONESIDE_STATUS_NUMBER				* 1];
    double _adb_X_t_tp[	CS4_ONESIDE_STATUS_NUMBER				* 1];
    double _adb_X_tn_t[	CS4_ONESIDE_STATUS_NUMBER				* 1];
    double _adb_P_t_t[	CS4_ONESIDE_STATUS_NUMBER				* CS4_ONESIDE_STATUS_NUMBER];
    double _adb_P_t_tp[	CS4_ONESIDE_STATUS_NUMBER				* CS4_ONESIDE_STATUS_NUMBER];
    double _adb_P_tn_t[	CS4_ONESIDE_STATUS_NUMBER				* CS4_ONESIDE_STATUS_NUMBER];
    double _adb_R_t[	CS4_ONESIDE_OBS_NUMBER					* CS4_ONESIDE_OBS_NUMBER];
    double _adb_F_t[	CS4_ONESIDE_STATUS_NUMBER				* CS4_ONESIDE_STATUS_NUMBER];
    double _adb_F_t_T[	CS4_ONESIDE_STATUS_NUMBER				* CS4_ONESIDE_STATUS_NUMBER];
    double _adb_D_t[	CS4_ONESIDE_STATUS_NUMBER				* CS4_ONESIDE_CONTROL_INPUT_NUMBER];
    double _adb_G_t[	CS4_ONESIDE_STATUS_NUMBER				* CS4_ONESIDE_STOCHASTIC_VARIAVLE_NUMBER];
    double _adb_G_t_T[	CS4_ONESIDE_STOCHASTIC_VARIAVLE_NUMBER	* CS4_ONESIDE_STATUS_NUMBER];
    double _adb_K_t[	CS4_ONESIDE_STATUS_NUMBER				* CS4_ONESIDE_OBS_NUMBER];
    double _adb_U_t[	CS4_ONESIDE_CONTROL_INPUT_NUMBER		* 1];
    double _adb_H_t[	CS4_ONESIDE_OBS_NUMBER					* CS4_ONESIDE_STATUS_NUMBER];
    double _adb_H_t_T[	CS4_ONESIDE_STATUS_NUMBER				* CS4_ONESIDE_OBS_NUMBER];
    double _adb_Q_t[	CS4_ONESIDE_STATUS_NUMBER				* CS4_ONESIDE_STOCHASTIC_VARIAVLE_NUMBER];
    double _adb_h_t[	CS4_ONESIDE_OBS_NUMBER					* 1];
    double _adb_y_t[	CS4_ONESIDE_OBS_NUMBER					* 1];
    int _adb_side_t[	CS4_ONESIDE_OBS_NUMBER					* 1];
    double _adb_i_t[	CS4_ONESIDE_OBS_NUMBER					* 1];
    double _adb_P_0[	CS4_ONESIDE_STATUS_NUMBER				* CS4_ONESIDE_STATUS_NUMBER];
    double _adb_Q_0[	CS4_ONESIDE_STOCHASTIC_VARIAVLE_NUMBER	* CS4_ONESIDE_STOCHASTIC_VARIAVLE_NUMBER];
    double _adb_DM_a[2 * 3];
    double _adb_DM_b[2 * 2];
    double _db_delta_t;

    double _adb_X_t_t_NearArea[	CS4_ONESIDE_STATUS_NUMBER				* 1];
    // psidash, psi, edash, e rhodash, rho, phi, w
//	double _adb_X_0[CS4_STATUS_NUMBER * 1];
//	double _adb_X_m[CS4_STATUS_NUMBER * 1];

//	double _adb_W_m[CS4_STOCHASTIC_VARIAVLE_NUMBER * 1];
//	double _adb_W_m0[CS4_STOCHASTIC_VARIAVLE_NUMBER * 1];
//	double _adb_V_m[CS4_OBS_NUMBER * 1];
    //(1)
    double _adb_H_tP_t_tp[				CS4_ONESIDE_OBS_NUMBER		* CS4_ONESIDE_STATUS_NUMBER];
    double _adb_H_tP_t_tpH_t_T[			CS4_ONESIDE_OBS_NUMBER		* CS4_ONESIDE_OBS_NUMBER];
    double _adb_H_tP_t_tpH_t_T_R_t[		CS4_ONESIDE_OBS_NUMBER		* CS4_ONESIDE_OBS_NUMBER];
    double _adb_H_tP_t_tpH_t_T_R_t_1[	CS4_ONESIDE_OBS_NUMBER		* CS4_ONESIDE_OBS_NUMBER];
    double _adb_P_t_tpHt_T[				CS4_ONESIDE_STATUS_NUMBER	* CS4_ONESIDE_OBS_NUMBER];
    //(2)
    double _adb_K_tHt[					CS4_ONESIDE_STATUS_NUMBER	* CS4_ONESIDE_STATUS_NUMBER];
    double _adb_K_tHtP_t_tp[			CS4_ONESIDE_STATUS_NUMBER	* CS4_ONESIDE_STATUS_NUMBER];
    //(3)
    double _adb_y_t_h_t[				CS4_ONESIDE_OBS_NUMBER		* 1];
    double _adb_K_ty_t_h_t[				CS4_ONESIDE_STATUS_NUMBER	* 1];
    //(4)
    double _adb_G_tQ_t[					CS4_ONESIDE_STATUS_NUMBER	* CS4_ONESIDE_STOCHASTIC_VARIAVLE_NUMBER];
    double _adb_G_tQ_tG_t_T[			CS4_ONESIDE_STATUS_NUMBER	* CS4_ONESIDE_STATUS_NUMBER];
    double _adb_F_tP_t_t[				CS4_ONESIDE_STATUS_NUMBER	* CS4_ONESIDE_STATUS_NUMBER];
    double _adb_F_tP_t_tF_t_T[			CS4_ONESIDE_STATUS_NUMBER	* CS4_ONESIDE_STATUS_NUMBER];
    //(5)
    double _adb_F_tX_t_t[				CS4_ONESIDE_STATUS_NUMBER	* 1];
    double _adb_D_tU_t[					CS4_ONESIDE_STATUS_NUMBER	* 1];

#ifdef	USE_DYNAMIC_SYSTEM_NOISE
    int _aiDynamicNoiseFlag[CS4_ONESIDE_STATUS_NUMBER];
#endif	USE_DYNAMIC_SYSTEM_NOISE

    double _dDetectedDistance;
    double _dDeltaOffsetCurb;

public:
    inline LaneParameterOneSide(PARAM_CAM *p, int iK)
    {
        _bAvailable = FALSE;
        _iCounterAfterInitialization = 0;
        _iK = iK;

        setupParameterInit(p);
        setupParameterMinMax(p);

        _db_delta_t = 0.1;
        initialize();
    }

    inline ~LaneParameterOneSide()
    {

    }
    inline int K(void)
    {
        return _iK;
    }

    void setupParameterInit(PARAM_CAM *p);
    void initialize(void);
//	inline void initialize(void) {
//		for(int iIdx = 0; iIdx < Dim(); iIdx++) {
//			_adb_X_t_t[iIdx] = _dParameterInit[iIdx];
//		}
//	}
    void setupParameterMinMax(PARAM_CAM *p);
    inline int Dim(void)
    {
        return _iDim;
    }
    inline BOOL Available(void)
    {
        return _bAvailable;
    }
    inline void Available(BOOL bV)
    {
        _bAvailable = bV;
    }
    inline int getCounterAfterInitialization(void)
    {
        return _iCounterAfterInitialization;
    }
    inline void clearCounterAfterInitialization(void)
    {
        _iCounterAfterInitialization = 0;
    }
    inline void incCounterAfterInitialization(void)
    {
        _iCounterAfterInitialization++;
    }
    inline double Param(int iIdx)
    {
        return _adb_X_t_t[iIdx];
    }
    inline void Param(int iIdx, double dV)
    {
        _adb_X_t_t[iIdx] = dV;
    }
    inline double ParamInit(int iIdx)
    {
        return _dParameterInit[iIdx];
    }
    inline void ParamInit(int iIdx, double dV)
    {
        _dParameterInit[iIdx] = dV;
    }
    inline double ParamMin(int iIdx)
    {
        return _dParameterMin[iIdx];
    }
    inline void ParamMin(int iIdx, double dV)
    {
        _dParameterMin[iIdx] = dV;
    }
    inline double ParamMax(int iIdx)
    {
        return _dParameterMax[iIdx];
    }
    inline void ParamMax(int iIdx, double dV)
    {
        _dParameterMax[iIdx] = dV;
    }

    inline void Param_tn_t(int iIdx, double dV)
    {
        _adb_X_tn_t[iIdx] = dV;
    }
    double LaneBoundaryPositionOnRoad(int iK, double dZ);
    double LaneBoundaryPositionOnRoad(int iK, double dZ, double *pdLaneParameter);
    double CurbPositionOnRoad(int iK, double dZ);
    double CurbPositionOnRoad(int iK, double dZ, double *pdLaneParameter);
    double LaneBoundaryPositionOnImagePixel(PARAM_CAM *p, int iSrc);
    double LaneBoundaryPositionOnImagePixelByNearAreaParameter(PARAM_CAM *pParamCam, int iISrc);
    double LaneBoundaryPositionOnImagePixel(PARAM_CAM *p, int iSrc, double dXVehicle);
    double CurbPositionOnImagePixel(PARAM_CAM *p, int iSrc);
    inline BOOL getNotAddSystemNoise(void)
    {
        return _bNotAddSystemNoise;
    }
    inline void setNotAddSystemNoise(void)
    {
        _bNotAddSystemNoise = TRUE;
    }
    inline void clearNotAddSystemNoise(void)
    {
        _bNotAddSystemNoise = FALSE;
    }

//	LaneParameterEstimator();
//	~LaneParameterEstimator();

    double *getX_t_t(void)
    {
        return _adb_X_t_t;
    }
    double *getX_t_t_NearArea(void)
    {
        return _adb_X_t_t_NearArea;
    }
    inline void saveX_t_t_NearArea(void)
    {
        dmmove(_adb_X_t_t, _adb_X_t_t_NearArea,  CS4_ONESIDE_STATUS_NUMBER, 1);
    }

    void calc_DM_a_b(void);
    void get_U_t(void);
    void update_t(void);
//	void set_y_t(void);
//	void set_y_t(int s4_side, int s4_iIdx);
    void set_y_t(int s4_side, LaneMarkerPoint *pLMP);
    void calc_Ht(PARAM_CAM *pParamCam);
    void calc_Ft(void);
    void calc_D_t(void);
    void calc_G_t(void);
    void calc_Kt(void);
    void calc_Kt2(void);
    void calc_P_t_t(void);
    void calc_h_t(PARAM_CAM *pParamCam);
    double get_HX_t_DM(PARAM_CAM *pParamCam, S4 s4_a_isrc, S4 s4_a_side);
    void calc_X_t_t(void);
    void calc_P_tn_t(void);
    void calc_X_tn_t(void);
    void calc_P_0(void);
    void calc_Q_t(void);
    void calc_Q_0(void);
    void calc_R_t(void);


    int update(PARAM_CAM *pParamCam, LaneMarkerPoints *pBoundaryPoints);
    unsigned long isInvalidWithMinAndMax(void);

#ifdef	USE_DYNAMIC_SYSTEM_NOISE
    inline int *geDynamicNoiseFlag(void)
    {
        return _aiDynamicNoiseFlag;
    }
    void DynamicNoise(void);
    inline void resetDynamicNoiseFlag(void)
    {
        for(int iIdx = 0; iIdx < CS4_ONESIDE_STATUS_NUMBER;	iIdx++)
        {
            _aiDynamicNoiseFlag[iIdx] = TRUE;
        }
    }
    inline void clearDynamicNoiseFlag(int iIdx)
    {
        _aiDynamicNoiseFlag[iIdx] = FALSE;
    }
    inline void setDynamicNoiseFlag(int iIdx)
    {
        _aiDynamicNoiseFlag[iIdx] = TRUE;
    }
#endif	USE_DYNAMIC_SYSTEM_NOISE


    inline double getDetectedDistance(void)
    {
        return _dDetectedDistance;
    }
    inline void setDetectedDistance(double dV)
    {
        _dDetectedDistance = dV;
    }
    inline double getDeltaOffsetCurb(void)
    {
        return _dDeltaOffsetCurb;
    }
    inline void setDeltaOffsetCurb(double dV)
    {
        _dDeltaOffsetCurb = dV;
    }

};
