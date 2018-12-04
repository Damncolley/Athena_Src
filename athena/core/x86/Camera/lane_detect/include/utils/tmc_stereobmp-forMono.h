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
    int			num_of_cam;					//�J�����̑䐔�i�P��F�P �X�e���I�F�Q�j�� ���̃p�����[�^���Ȃ��ꍇ�͂P�Ƃ���j
    double		base_length;				//�J���������
    int			width;						//���͉摜��
    int			height;						//���͉摜����

    double		cam_pos_x;					//�J�������t���ʒuX[mm]
    double		cam_pos_z;					//�J�������t���ʒuZ[mm]
    double		cam_pos_y;					//�J�������t���ʒuY[mm]
    double		roll;						//�J�������t���p���[��[deg]
    double		pitch;						//�J�������t���p�s�b�`[deg]
    double		yaw;						//�J�������t���p���[[deg]

    // *************************************************************************** //
    double		pix_f_x;					//�œ_����(pixel) X����
    double		pix_f_y;					//�œ_����(pixel) Y����
    double		i_x0;						//���w�摜���S(X)
    double		i_y0;						//���w�摜���S(Y)
    double		lens_dist_rad_k1;			//���a�����̃����Y�c�݌W��K1
    double		lens_dist_rad_k2;			//���a�����̃����Y�c�݌W��K2
    double		lens_dist_tan_p1;			//�~�������̃����Y�c�݌W��P1
    double		lens_dist_tan_p2;			//�~�������̃����Y�c�݌W��P2
    double		undist_pix_f_x;				//�c�␳�p�œ_����(pixel)X����
    double		undist_pix_f_y;				//�c�␳�p�œ_����(pixel)Y����

    double		r_r[3][3];					//���s���s�� [�s][��]

    double		foe_x;						//Z�����_��X���W[pixel]
    double		foe_y;						//Z�����_��Y���W[pixel]
    double		ratio;						//�c����i�c/���j
    double		cam_deg_v;					//�J����������p[deg/pix]
    double		res_ang_v;					//������p�𑜓x

    double		l_pix_f_x;					//�œ_����(pixel) X����
    double		l_pix_f_y;					//�œ_����(pixel) Y����
    double		l_i_x0;						//���w�摜���S(X)
    double		l_i_y0;						//���w�摜���S(Y)
    double		l_lens_dist_rad_k1;			//���a�����̃����Y�c�݌W��K1
    double		l_lens_dist_rad_k2;			//���a�����̃����Y�c�݌W��K2
    double		l_lens_dist_tan_p1;			//�~�������̃����Y�c�݌W��P1
    double		l_lens_dist_tan_p2;			//�~�������̃����Y�c�݌W��P2
    double		l_undist_pix_f_x;			//�c�␳�p�œ_����(pixel)X����
    double		l_undist_pix_f_y;			//�c�␳�p�œ_����(pixel)Y����

    double		r_l[3][3];					//���s���s�� [�s][��]

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
