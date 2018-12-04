class OutputInfo {
//#define	LOGIC_LOG_HEADER0	"?s?b?`?p[deg],?c��f?����|[1/m]"
//#define	LOGIC_LOG_HEADER1	",??����?��?I?t?Z?b?g[m],??����?��???[?p[deg],??����?��?��?��?����|[1/m],??����?��?����|??��?��|[1/m^2],??����?����L???������[m],??��????I?t?Z?b?g[m]"
//#define	LOGIC_LOG_HEADER2	",��E����?��?I?t?Z?b?g[m],��E����?��???[?p[deg],��E����?��?��?��?����|[1/m],��E����?��?����|??��?��|[1/m^2],��E����?����L???������[m],��E��????I?t?Z?b?g[m]"

private:
	double _dPitch;	// ?s?b?`?p[deg]
	double _dCv;	// ?c��f?����|[1/m]
	double _dLeftOffset;				// ??����?��?I?t?Z?b?g[m]
	double _dLeftYaw;					// ??����?��???[?p[deg]
	double _dLeftC0;					// ??����?��?��?��?����|[1/m]
	double _dLeftC1;					//??����?��?����|??��?��|[1/m^2]
	double _dLeftAvailableDistance;		// ??����?����L???������[m]
	double _dLeftCurbOffset;			// ??��????I?t?Z?b?g[m]
	double _dRightOffset;				// ��E����?��?I?t?Z?b?g[m]
	double _dRightYaw;					// ��E����?��???[?p[deg]
	double _dRightC0;					// ��E����?��?��?��?����|[1/m]
	double _dRightC1;					// ��E����?��?����|??��?��|[1/m^2]
	double _dRightAvailableDistance;	// ��E����?����L???������[m]
	double _dRightCurbOffset;			// ��E��????I?t?Z?b?g[m]
public:
	inline OutputInfo(void)	{
		_dPitch = 0.0;					// ?s?b?`?p[deg]
		_dCv = 0.0;						// ?c��f?����|[1/m]
		_dLeftOffset = 0.0;				// ??����?��?I?t?Z?b?g[m]
		_dLeftYaw = 0.0;				// ??����?��???[?p[deg]
		_dLeftC0 = 0.0;					// ??����?��?��?��?����|[1/m]
		_dLeftC1 = 0.0;					//??����?��?����|??��?��|[1/m^2]
		_dLeftAvailableDistance = 0.0;	// ??����?����L???������[m]
		_dLeftCurbOffset = 0.0;			// ??��????I?t?Z?b?g[m]
		_dRightOffset = 0.0;			// ��E����?��?I?t?Z?b?g[m]
		_dRightYaw = 0.0;				// ��E����?��???[?p[deg]
		_dRightC0 = 0.0;				// ��E����?��?��?��?����|[1/m]
		_dRightC1 = 0.0;				// ��E����?��?����|??��?��|[1/m^2]
		_dRightAvailableDistance = 0.0;	// ��E����?����L???������[m]
		_dRightCurbOffset = 0.0;		// ��E��????I?t?Z?b?g[m]
	}
	inline double Pitch(void)					{	return _dPitch;	}
	inline double Cv(void)						{	return _dCv;	}
	inline double LeftOffset(void)				{	return _dLeftOffset;	}
	inline double LeftYaw(void)					{	return _dLeftYaw;	}
	inline double LeftC0(void)					{	return _dLeftC0;	}
	inline double LeftC1(void)					{	return _dLeftC1;	}
	inline double LeftAvailableDistance(void)	{	return _dLeftAvailableDistance;	}
	inline double LeftCurbOffset(void)			{	return	_dLeftCurbOffset;	}
	inline double RightOffset(void)				{	return _dRightOffset;	}
	inline double RightYaw(void)				{	return _dRightYaw;		}
	inline double RightC0(void)					{	return _dRightC0;		}
	inline double RightC1(void)					{	return _dRightC1;		}
	inline double RightAvailableDistance(void)	{	return _dRightAvailableDistance;	}
	inline double RightCurbOffset(void)			{	return _dRightCurbOffset;	}
	inline void Pitch(double dV)					{	_dPitch = dV;	}
	inline void Cv(double dV)						{	_dCv = dV;	}
	inline void LeftOffset(double dV)				{	_dLeftOffset = dV;	}
	inline void LeftYaw(double dV)					{	_dLeftYaw = dV;	}
	inline void LeftC0(double dV)					{	_dLeftC0 = dV;	}
	inline void LeftC1(double dV)					{	_dLeftC1 = dV;	}
	inline void LeftAvailableDistance(double dV)	{	_dLeftAvailableDistance = dV;	}
	inline void LeftCurbOffset(double dV)			{	_dLeftCurbOffset = dV;	}
	inline void RightOffset(double dV)				{	_dRightOffset = dV;	}
	inline void RightYaw(double dV)					{	_dRightYaw = dV;		}
	inline void RightC0(double dV)					{	_dRightC0 = dV;		}
	inline void RightC1(double dV)					{	_dRightC1 = dV;		}
	inline void RightAvailableDistance(double dV)	{	_dRightAvailableDistance = dV;	}
	inline void RightCurbOffset(double dV)			{	_dRightCurbOffset = dV;	}
};
