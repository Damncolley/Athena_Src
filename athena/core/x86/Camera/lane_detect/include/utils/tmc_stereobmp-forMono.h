#ifndef _TMC_STEREOBMP_H_
#define _TMC_STEREOBMP_H_
class Disparity
{

};

#include <stdlib.h>
#include <time.h>
#include <math.h>

typedef struct
{
    int			num_of_cam;					//カメラの台数（単眼：１ ステレオ：２）← このパラメータがない場合は１とする）
    double		base_length;				//カメラ基線長
    int			width;						//入力画像幅
    int			height;						//入力画像高さ

    double		cam_pos_x;					//カメラ取り付け位置X[mm]
    double		cam_pos_z;					//カメラ取り付け位置Z[mm]
    double		cam_pos_y;					//カメラ取り付け位置Y[mm]
    double		roll;						//カメラ取り付け角ロール[deg]
    double		pitch;						//カメラ取り付け角ピッチ[deg]
    double		yaw;						//カメラ取り付け角ヨー[deg]

    // *************************************************************************** //
    double		pix_f_x;					//焦点距離(pixel) X方向
    double		pix_f_y;					//焦点距離(pixel) Y方向
    double		i_x0;						//光学画像中心(X)
    double		i_y0;						//光学画像中心(Y)
    double		lens_dist_rad_k1;			//半径方向のレンズ歪み係数K1
    double		lens_dist_rad_k2;			//半径方向のレンズ歪み係数K2
    double		lens_dist_tan_p1;			//円周方向のレンズ歪み係数P1
    double		lens_dist_tan_p2;			//円周方向のレンズ歪み係数P2
    double		undist_pix_f_x;				//歪補正用焦点距離(pixel)X方向
    double		undist_pix_f_y;				//歪補正用焦点距離(pixel)Y方向

    double		r_r[3][3];					//平行化行列 [行][列]

    double		foe_x;						//Z消失点のX座標[pixel]
    double		foe_y;						//Z消失点のY座標[pixel]
    double		ratio;						//縦横比（縦/横）
    double		cam_deg_v;					//カメラ垂直画角[deg/pix]
    double		res_ang_v;					//垂直画角解像度

    double		l_pix_f_x;					//焦点距離(pixel) X方向
    double		l_pix_f_y;					//焦点距離(pixel) Y方向
    double		l_i_x0;						//光学画像中心(X)
    double		l_i_y0;						//光学画像中心(Y)
    double		l_lens_dist_rad_k1;			//半径方向のレンズ歪み係数K1
    double		l_lens_dist_rad_k2;			//半径方向のレンズ歪み係数K2
    double		l_lens_dist_tan_p1;			//円周方向のレンズ歪み係数P1
    double		l_lens_dist_tan_p2;			//円周方向のレンズ歪み係数P2
    double		l_undist_pix_f_x;			//歪補正用焦点距離(pixel)X方向
    double		l_undist_pix_f_y;			//歪補正用焦点距離(pixel)Y方向

    double		r_l[3][3];					//平行化行列 [行][列]

} IPDATA_PARAM_CAM,*LPIPDATA_PARAM_CAM;


#define	CAM_FILE	"camera.ini"

class PARAM_CAM
{
private:
    LPIPDATA_PARAM_CAM _pParamCam;
    LPIPDATA_PARAM_CAM _pParamCamDefault;
    LPIPDATA_PARAM_CAM _pParamCamNearArea;
    double _dCurvatureVertical;
    int _iProcArea[2][2];	// [I or J][Min or Max]


public:
    PARAM_CAM();
    ~PARAM_CAM();
    int load(char *fname);

    inline LPIPDATA_PARAM_CAM	ParamCam(void)
    {
        return _pParamCam;
    }
    inline LPIPDATA_PARAM_CAM	ParamCamDefault(void)
    {
        return _pParamCamDefault;
    }
    inline LPIPDATA_PARAM_CAM	ParamCamNearArea(void)
    {
        return _pParamCamNearArea;
    }
    inline double getCurvatureVertical(void)
    {
        return _dCurvatureVertical;
    }
    inline void setCurvatureVertical(double dV)
    {
        _dCurvatureVertical = dV;
    }
    inline int IsrcMin(void)
    {
        return _iProcArea[0][0];
    }
    inline int IsrcMax(void)
    {
        return _iProcArea[0][1];
    }
    inline int JsrcMin(void)
    {
        return _iProcArea[1][0];
    }
    inline int JsrcMax(void)
    {
        return _iProcArea[1][1];
    }
    inline void IsrcMin(int iV)
    {
        _iProcArea[0][0] = iV;
    }
    inline void IsrcMax(int iV)
    {
        _iProcArea[0][1] = iV;
    }
    inline void JsrcMin(int iV)
    {
        _iProcArea[1][0] = iV;
    }
    inline void JsrcMax(int iV)
    {
        _iProcArea[1][1] = iV;
    }
    inline void saveParamCamNearArea(void)
    {
        (*(ParamCamNearArea())) = *ParamCam();
    }

};

#endif _TMC_STEREOBMP_H_
