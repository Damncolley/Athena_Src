
// ???€?I?t?Z?b?g
class RefOffset {
private:
	int _iInitCnt;	// ?¡ë?¨²?J?E?¡°?^
	int _iCnt;		// ?J?E?¡°?^
	int _aiOffset[CS4_NOISE];	// ???n¡ª??f?[?^?o?b?t?@
public:
	inline RefOffset(void)	:	_iInitCnt(0), _iCnt(0)	{
		{for(int iIdx = 0; iIdx < CS4_NOISE; iIdx++)	{	_aiOffset[iIdx] = 0;	}}
	}
	inline int InitCnt(void)	{	return _iInitCnt;	}
	inline void InitCnt(int v)	{	_iInitCnt = v;		}
	inline int Cnt(void)		{	return _iCnt;		}
	inline void Cnt(int v)		{	_iCnt = v;			}
	inline int Offset(int iIdx)	{	return _aiOffset[iIdx];	}
	inline void Offset(int iIdx, int v)	{	_aiOffset[iIdx] = v;	}
};

