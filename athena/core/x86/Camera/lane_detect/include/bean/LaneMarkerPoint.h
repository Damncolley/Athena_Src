#pragma once
#include "../utils/type.h"

class LaneMarkerPoint {
private:
	int _iI;	// 2???3??¡¯??¨¤?W[pix]
	int _iJ;	// 2???3?¡­???¨¤?W[pix]
	double _dEdgeStrength;	// ?G?b?W?-¡°x
	double _dEdgeGradient;	// ?G?b?W???¨¹[rad]
	int _iProcLineIndex;
	double _dDisparity;		// ???¡¤[sub-pix]
	double _adPos3D[3];		// 3???3?¨¤?W{[mm],[mm],[mm]}
	BOOL _bAboveGround;		// ¡ª¡ì¡®¨¬?¡§???¨¬¡°_?t?¡ë?O

public:
	inline	LaneMarkerPoint()	: _iI(-1), _iJ(-1), _dEdgeStrength(-1), _dEdgeGradient(0), _iProcLineIndex(-1), _dDisparity(-1)	{	_adPos3D[0] = 0.0;	_adPos3D[1] = 0.0;	_adPos3D[2] = 0.0;	_bAboveGround = FALSE;	}
	inline  LaneMarkerPoint(int iIsrc, int iJsrc, double dEdgeStrength, double dGrandient, int iProcLineIndex, double dDisparity, double dX = 0.0,	double dY = 0.0, double dZ = 0.0)	{
		_iI = iIsrc;
		_iJ = iJsrc;
		_dEdgeStrength = dEdgeStrength;
		_dEdgeGradient = dGrandient;
		_iProcLineIndex = iProcLineIndex;
		_dDisparity = dDisparity;
		_adPos3D[0] = dX;
		_adPos3D[1] = dY;
		_adPos3D[2] = dZ;
		_bAboveGround = FALSE;
	}
	inline  LaneMarkerPoint(LaneMarkerPoint *pSrc)	{
		_iI = pSrc->getIsrc();
		_iJ = pSrc->getJsrc();
		_dEdgeStrength = pSrc->getEdgeStrength();
		_dEdgeGradient = pSrc->getEdgeGradient();
		_iProcLineIndex = pSrc->getProcLineIndex();
		_dDisparity = pSrc->getDisparity();
		_adPos3D[0] = pSrc->X3D();
		_adPos3D[1] = pSrc->Y3D();
		_adPos3D[2] = pSrc->Z3D();
		_bAboveGround = pSrc->getAboveGround();
	}

	inline int getIsrc(void)				{	return _iI;				}
	inline void setIsrc(int iV)				{	_iI = iV;				}
	inline int getJsrc(void)				{	return _iJ;				}
	inline void setJsrc(int iV)				{	_iJ = iV;				}
	inline double getEdgeStrength(void)		{	return _dEdgeStrength;	}
	inline void setEdgeStrength(double dV)	{	_dEdgeStrength = dV;	}
	inline double getEdgeGradient(void)		{	return _dEdgeGradient;	}
	inline void setEdgeGradient(double dV)	{	_dEdgeGradient = dV;	}
	inline double getDisparity(void)		{	return _dDisparity;		}
	inline void setDisparity(double dV)		{	_dDisparity = dV;		}
	inline int getProcLineIndex(void)		{	return _iProcLineIndex;	}
	inline void setProcLineIndex(int iV)	{	_iProcLineIndex = iV;	}

	inline double *getPos3D(void)	{	return _adPos3D;	}
	inline double X3D(void)	{	return _adPos3D[0];	}
	inline double Y3D(void)	{	return _adPos3D[1];	}
	inline double Z3D(void)	{	return _adPos3D[2];	}
	inline void X3D(double dV)	{	_adPos3D[0] = dV;	}
	inline void Y3D(double dV)	{	_adPos3D[1] = dV;	}
	inline void Z3D(double dV)	{	_adPos3D[2] = dV;	}
	inline BOOL getAboveGround(void)	{	return _bAboveGround;	}
	inline void setAboveGround(BOOL v)	{	_bAboveGround = v;	}
};

typedef LaneMarkerPoint * ptrLaneMarkerPoint;
