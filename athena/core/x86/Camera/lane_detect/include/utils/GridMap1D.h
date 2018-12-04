class GridMap1D {
private:
	double _dXmin;
	double _dXmax;
	double _dXstep;
	LaneMarkerPoints *_pLMPs;
	int _iIsrcMin;
	int _iIsrcMax;
	int _iGridNumber;
	double _adInitialOffset[LR_NUM];
	FlexArray<double> *_pYmins;
	double	*_pdMovingAverage;
	BOOL *_pbStep;
public:
	inline GridMap1D(void)	{
		_dXmin = 0;
		_dXmax = 0;
		_dXstep = 0;
		_pLMPs = NULL;
		_iIsrcMin = 0;
		_iIsrcMax = 0;
		_iGridNumber = 0;
		for(int iLR = LR_LEFT; iLR < LR_NUM; iLR++)		{	_adInitialOffset[iLR] = 0;	}
		_pYmins = NULL;
		_pdMovingAverage = NULL;
		_pbStep = NULL;
	}
	inline ~GridMap1D(void)	{
		SAFE_DELETE_ARRAY(_pLMPs);
		SAFE_DELETE_ARRAY(_pYmins);
		SAFE_DELETE_ARRAY(_pdMovingAverage);
		SAFE_DELETE_ARRAY(_pbStep);
	}
	inline double getXmin(void)	{	return _dXmin;	}
	inline double getXmax(void)	{	return _dXmax;	}
	inline double getXstep(void)	{	return _dXstep;	}
	inline void setXmin(double dV)	{	_dXmin = dV;	}
	inline void setXmax(double dV)	{	_dXmax = dV;	}
	inline void setXstep(double dV)	{	_dXstep = dV;	}
	inline int getIsrcMin(void)	{	return	_iIsrcMin;	}
	inline int getIsrcMax(void)	{	return	_iIsrcMax;	}
	inline void setIsrcMin(int iV)	{	_iIsrcMin = iV;	}
	inline void setIsrcMax(int iV)	{	_iIsrcMax = iV;	}
	inline int getGridNumber(void)	{	return _iGridNumber;	}
	inline void setGridNumber(int iV)	{	_iGridNumber = iV;	}
	inline double getInitialOffset(int iLR)	{	return _adInitialOffset[iLR];	}
	inline void setInitialOffset(int iLR, double dV)	{	_adInitialOffset[iLR] = dV;	}
	inline FlexArray<double> *getYmins(int iIdx)	{	return (_pYmins + iIdx);	}
	inline double getMovingAverage(int iIdx)	{	return *(_pdMovingAverage + iIdx);	}
	inline void setMovingAverage(int iIdx, double dV)	{	*(_pdMovingAverage + iIdx) = dV;	}
	inline BOOL getStepFlag(int iIdx)	{	return *(_pbStep + iIdx);	}
	inline void setStepFlag(int iIdx, BOOL bV)	{	*(_pbStep + iIdx) = bV;	}
	inline LaneMarkerPoints *getGrid(int iIdx) {
		if(iIdx < 0)	return NULL;
		if(iIdx >= getGridNumber())	return NULL;
		return (_pLMPs + iIdx);
	}

	inline void initlize(void) {
		SAFE_DELETE_ARRAY(_pLMPs);
		setXmin(0);
		setXmax(0);
		setXstep(0);
		setGridNumber(0);
		setIsrcMin(0);
		setIsrcMax(0);

		SAFE_DELETE_ARRAY(_pYmins);
		for(int iLR = LR_LEFT; iLR < LR_NUM; iLR++)		{	_adInitialOffset[iLR] = 0;	}
		_pLMPs = NULL;
		SAFE_DELETE_ARRAY(_pdMovingAverage);
		SAFE_DELETE_ARRAY(_pbStep);
	}
	inline void setup(double dXmin, double dXmax, double dXstep) {
		SAFE_DELETE_ARRAY(_pLMPs);
		if(dXmin >= dXmax)	return;
		if(dXstep <= 0)	return;
		setXmin(dXmin);
		setXmax(dXmax);
		setXstep(dXstep);
		int iNumber = (int)((getXmax() - getXmin()) / getXstep() + 0.5);
		setGridNumber(iNumber);
		_pLMPs = new LaneMarkerPoints[iNumber];

		SAFE_DELETE_ARRAY(_pYmins);
		_pYmins = new FlexArray<double>[iNumber];
		for(int iLR = LR_LEFT; iLR < LR_NUM; iLR++)		{	_adInitialOffset[iLR] = 0;	}
		SAFE_DELETE_ARRAY(_pdMovingAverage);
		_pdMovingAverage = new double[iNumber];
		_pbStep = new BOOL[iNumber];
		for(int iIdx = 0; iIdx < iNumber; iIdx++) {	_pbStep[iIdx] = FALSE;	}
	}
	inline int getGridIdx(double dX) {
		if(dX < getXmin())	return -1;
		if(dX > getXmax())	return -1;
		int iIdx = (int)((dX - getXmin()) / getXstep());
		return iIdx;
	}
	inline LaneMarkerPoints *getGrid(double dX) {
		if(dX < getXmin())	return NULL;
		if(dX > getXmax())	return NULL;
		int iIdx = (int)((dX - getXmin()) / getXstep());
		return (_pLMPs + iIdx);
	}
	inline void vote(LaneMarkerPoint *pLMP, double dOffset) {
		if(pLMP == NULL)	return;
		double dX = pLMP->X3D();
		dX += dOffset;
		LaneMarkerPoints *pLMPs =getGrid(dX);
		if(pLMPs == NULL)	return;
		pLMPs->addLaneMarkerPoint(pLMP);
	}
	inline BOOL calcMovingAverage(void)	{
		int iGridNumber = getGridNumber();
		for(int iIdx = 0; iIdx < iGridNumber; iIdx++) {
			FlexArray<double> *pfaYminsTmp = new FlexArray<double>;
			FlexArray<double> *pfaYmins = getYmins(iIdx);
			double dYmin = GM_MOVINGAVERAGE_INVALID_VALUE;
			for(int iIdx1 = 0; iIdx1 < pfaYmins->getNumber(); iIdx1++) {
				double dValue = pfaYmins->get(iIdx1);
				if(dValue >= GM_MOVINGAVERAGE_INVALID_VALUE) {
					continue;
				}
				pfaYminsTmp->add(dValue);
			}
			if(pfaYminsTmp->getNumber() < 3) {
				dYmin = GM_MOVINGAVERAGE_INVALID_VALUE;
			} else {
				// sorting
				for(int iIdxA = 0; iIdxA < pfaYminsTmp->getNumber() - 1; iIdxA++) {
					double dV_A = pfaYminsTmp->get(iIdxA);
					for(int iIdxB = iIdxA + 1; iIdxB < pfaYminsTmp->getNumber(); iIdxB++) {
						double dV_B = pfaYminsTmp->get(iIdxB);
						if(dV_A > dV_B) {
							pfaYminsTmp->set(iIdxA, dV_B);
							pfaYminsTmp->set(iIdxB, dV_A);
						}
					}
				}
				// median value
				dYmin = pfaYminsTmp->get(pfaYminsTmp->getNumber() / 2);
			}
			setMovingAverage(iIdx, dYmin);
			SAFE_DELETE(pfaYminsTmp);
		}
		return TRUE;
	}
	inline BOOL searchStep(double dTh) {
		int iCenter = getGridNumber() / 2;
		double dYPrev = getMovingAverage(iCenter);
		setStepFlag(iCenter, FALSE);
		for(int iIdx = iCenter - 1; iIdx >= 0; iIdx--) {
			setStepFlag(iIdx, FALSE);
			double dY = getMovingAverage(iIdx);
			if(	dYPrev	!= GM_MOVINGAVERAGE_INVALID_VALUE
			&&	dY		!= GM_MOVINGAVERAGE_INVALID_VALUE) {
				if(	(	(-(dY - dYPrev) >= dTh)
					&&	(-dY >= dTh)
					)
				||	(-dY >= dTh * 2)
				) {
					setStepFlag(iIdx, TRUE);
				}
			}
			dYPrev = dY;
		}
		dYPrev = getMovingAverage(iCenter);
		for(int iIdx = iCenter + 1; iIdx < getGridNumber(); iIdx++) {
			setStepFlag(iIdx, FALSE);
			double dY = getMovingAverage(iIdx);
			if(	dYPrev	!= GM_MOVINGAVERAGE_INVALID_VALUE
			&&	dY		!= GM_MOVINGAVERAGE_INVALID_VALUE) {
				if(	(	(-(dY - dYPrev) >= dTh)
					&&	(-dY >= dTh)
					)
				||	(-dY >= dTh * 2)
				) {
					setStepFlag(iIdx, TRUE);
				}
			}
			dYPrev = dY;
		}
		return TRUE;
	}
};
