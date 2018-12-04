#pragma once

#include "../utils/type.h"
#include "../utils/config.h"
#include "../utils/flexarray.h"
#include "LaneMarkerLineSequence.h"
#include "LaneParameterEstimator.h"


class LaneParameter
{
private:
    static const int _iDim = CS4_STATUS_NUMBER;
    BOOL _bAvailable;
    int _iCounterAfterInitialization;
    double _dParameterInit[_iDim];
    double _dParameterMin[_iDim];
    double _dParameterMax[_iDim];
//	double _dParameter[_iDim];

    BOOL _bNotAddSystemNoise;

    int _s4_mobs;

    double _adb_X_t_t[		CS4_STATUS_NUMBER					* 1];
    double _adb_X_t_tp[	CS4_STATUS_NUMBER					* 1];
    double _adb_X_tn_t[	CS4_STATUS_NUMBER					* 1];
    double _adb_P_t_t[		CS4_STATUS_NUMBER					* CS4_STATUS_NUMBER];
    double _adb_P_t_tp[	CS4_STATUS_NUMBER					* CS4_STATUS_NUMBER];
    double _adb_P_tn_t[	CS4_STATUS_NUMBER					* CS4_STATUS_NUMBER];
    double _adb_R_t[		CS4_OBS_NUMBER						* CS4_OBS_NUMBER];
    double _adb_F_t[		CS4_STATUS_NUMBER					* CS4_STATUS_NUMBER];
    double _adb_F_t_T[		CS4_STATUS_NUMBER					* CS4_STATUS_NUMBER];
    double _adb_D_t[		CS4_STATUS_NUMBER					* CS4_CONTROL_INPUT_NUMBER];
    double _adb_G_t[		CS4_STATUS_NUMBER					* CS4_STOCHASTIC_VARIAVLE_NUMBER];
    double _adb_G_t_T[		CS4_STOCHASTIC_VARIAVLE_NUMBER		* CS4_STATUS_NUMBER];
    double _adb_K_t[		CS4_STATUS_NUMBER					* CS4_OBS_NUMBER];
    double _adb_U_t[		CS4_CONTROL_INPUT_NUMBER			* 1];
    double _adb_H_t[		CS4_OBS_NUMBER						* CS4_STATUS_NUMBER];
    double _adb_H_t_T[		CS4_STATUS_NUMBER					* CS4_OBS_NUMBER];
    double _adb_Q_t[		CS4_STATUS_NUMBER					* CS4_STOCHASTIC_VARIAVLE_NUMBER];
    double _adb_h_t[		CS4_OBS_NUMBER						* 1];
    double _adb_y_t[		CS4_OBS_NUMBER						* 1];
    int _adb_side_t[	CS4_OBS_NUMBER						* 1];
    double _adb_i_t[		CS4_OBS_NUMBER						* 1];
    double _adb_P_0[CS4_STATUS_NUMBER					* CS4_STATUS_NUMBER];
    double _adb_Q_0[CS4_STOCHASTIC_VARIAVLE_NUMBER	*	CS4_STOCHASTIC_VARIAVLE_NUMBER];
    double _adb_DM_a[2 * 3];
    double _adb_DM_b[2 * 2];
    double _db_delta_t;

    // psidash, psi, edash, e rhodash, rho, phi, w
//	double _adb_X_0[CS4_STATUS_NUMBER * 1];
//	double _adb_X_m[CS4_STATUS_NUMBER * 1];

//	double _adb_W_m[CS4_STOCHASTIC_VARIAVLE_NUMBER * 1];
//	double _adb_W_m0[CS4_STOCHASTIC_VARIAVLE_NUMBER * 1];
//	double _adb_V_m[CS4_OBS_NUMBER * 1];
    //(1)
    double _adb_H_tP_t_tp[				CS4_OBS_NUMBER		* CS4_STATUS_NUMBER];
    double _adb_H_tP_t_tpH_t_T[		CS4_OBS_NUMBER		* CS4_OBS_NUMBER];
    double _adb_H_tP_t_tpH_t_T_R_t[	CS4_OBS_NUMBER		* CS4_OBS_NUMBER];
    double _adb_H_tP_t_tpH_t_T_R_t_1[	CS4_OBS_NUMBER		* CS4_OBS_NUMBER];
    double _adb_P_t_tpHt_T[			CS4_STATUS_NUMBER	* CS4_OBS_NUMBER];
    //(2)
    double _adb_K_tHt[					CS4_STATUS_NUMBER	* CS4_STATUS_NUMBER];
    double _adb_K_tHtP_t_tp[			CS4_STATUS_NUMBER	* CS4_STATUS_NUMBER];
    //(3)
    double _adb_y_t_h_t[				CS4_OBS_NUMBER		* 1];
    double _adb_K_ty_t_h_t[			CS4_STATUS_NUMBER	* 1];
    //(4)
    double _adb_G_tQ_t[					CS4_STATUS_NUMBER	* CS4_STOCHASTIC_VARIAVLE_NUMBER];
    double _adb_G_tQ_tG_t_T[				CS4_STATUS_NUMBER	* CS4_STATUS_NUMBER];
    double _adb_F_tP_t_t[				CS4_STATUS_NUMBER	* CS4_STATUS_NUMBER];
    double _adb_F_tP_t_tF_t_T[			CS4_STATUS_NUMBER	* CS4_STATUS_NUMBER];
    //(5)
    double _adb_F_tX_t_t[				CS4_STATUS_NUMBER	* 1];
    double _adb_D_tU_t[					CS4_STATUS_NUMBER	* 1];

public:
    inline LaneParameter()
    {
        _bAvailable = FALSE;
        _iCounterAfterInitialization = 0;

        setupParameterInit(NULL);
        setupParameterMinMax(NULL);
        _db_delta_t = 0.1;
        initialize();
    }
    inline LaneParameter(PARAM_CAM *p)
    {
        _bAvailable = FALSE;
        _iCounterAfterInitialization = 0;

        setupParameterInit(p);
        setupParameterMinMax(p);

        _db_delta_t = 0.1;
        initialize();
    }
    inline ~LaneParameter()	{	}

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
    double LaneBoundaryPositionOnRoad(int iK, double dZ);
    double LaneBoundaryPositionOnRoad(int iK, double dZ, double *pdLaneParameter);
    double LaneBoundaryPositionOnImagePixel(PARAM_CAM *p, int iK, int iSrc);
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
    void calc_DM_a_b(void);
    void get_U_t(void);
    void update_t(void);
//	void set_y_t(void);
//	void set_y_t(int s4_side, int s4_iIdx);
    void set_y_t(int s4_side, LaneMarkerPoint *pLMP);
    BOOL calc_Ht(PARAM_CAM *pParamCam);
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


    int update(PARAM_CAM *pParamCam, LaneMarkerPoints *pLeftBoundaryPoints, LaneMarkerPoints *pRightBoundaryPoints);
    unsigned long isInvalidWithMinAndMax(void);

};
