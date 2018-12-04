#ifndef _FLEX_ARRAY_H_
#define _FLEX_ARRAY_H_

#include <stdlib.h>

template<class Type>
class FlexArray
{
private:
    int _iMaxNumber;
    int _iNumber;
    Type *_pData;

public:
    FlexArray(void);
    FlexArray(int s);
    ~FlexArray(void);
    void reset(void);
    void clear(void);
    int getNumber(void);
    void add(Type Data);
    Type get(int idx);
    Type set(int idx, Type Data);
    inline Type *data(void)
    {
        return _pData;
    }
    void remove(int idx);
    void remove_delete(int idx);
    void insert(int idx, Type Data);
    void swap(int idx0, int idx1);
};

template<class Type>
void FlexArray<Type>::add(Type NewData)
{
    if(_iNumber >= _iMaxNumber)
    {
        _iMaxNumber *= 2;
        Type *new_p = new Type[_iMaxNumber];
        if(new_p == NULL)	return;
        for(int iIdx = 0; iIdx < _iNumber; iIdx++)
        {
            new_p[iIdx] = _pData[iIdx];
        }
        delete [] _pData;
        _pData = new_p;
    }
    _pData[_iNumber] = NewData;
    _iNumber++;
}

template<class Type>
FlexArray<Type>::FlexArray(void)
{
    _iNumber = 0;
    _iMaxNumber = 1;
    _pData = new Type[_iMaxNumber];
}

template<class Type>
FlexArray<Type>::~FlexArray(void)
{
    if(_pData) delete [] _pData;
    _pData = NULL;
}

template<class Type>
void FlexArray<Type>::reset(void)
{
    _iNumber = 0;
}

template<class Type>
void FlexArray<Type>::clear(void)
{
    for(int iIdx = 0; iIdx < _iNumber; iIdx++)
    {
        if(_pData[iIdx] != NULL)
        {
            delete _pData[iIdx];
        }
    }
}

template<class Type>
Type FlexArray<Type>::get(int iIdx)
{
    if(iIdx < 0)	return NULL;
    if(iIdx >= getNumber())	return NULL;
    return _pData[iIdx];
}

template<class Type>
Type FlexArray<Type>::set(int iIdx, Type Data)
{
    return _pData[iIdx] = Data;
}

template<class Type> int FlexArray<Type>::getNumber(void)
{
    return _iNumber;
}

template<class Type>
void FlexArray<Type>::remove(int iDeleteIdx)
{
    if(iDeleteIdx < 0)	return;
    if(iDeleteIdx >= _iNumber)	return;
    for(int iIdx = iDeleteIdx; iIdx < (_iNumber - 1); iIdx++)
    {
        _pData[iIdx] = _pData[iIdx + 1];
    }
    _iNumber--;
}

template<class Type>
void FlexArray<Type>::remove_delete(int iDeleteIdx)
{
    if(iDeleteIdx < 0)	return;
    if(iDeleteIdx >= _iNumber)	return;
    delete _pData[iDeleteIdx];
    for(int iIdx = iDeleteIdx; iIdx < (_iNumber - 1); iIdx++)
    {
        _pData[iIdx] = _pData[iIdx + 1];
    }
    _iNumber--;
}

template<class Type>
void FlexArray<Type>::insert(int iInsertIdx, Type NewData)
{
    if(iInsertIdx < 0)	return;
    if(iInsertIdx >= _iNumber)
    {
        add(NewData);
        return;
    }
    if(_iNumber >= _iMaxNumber)
    {
        _iMaxNumber *= 2;
        Type *new_p = new Type[_iMaxNumber];
        if(new_p == NULL)	return;
        for(int iIdx = 0; iIdx < _iNumber; iIdx++)
        {
            new_p[iIdx] = _pData[iIdx];
        }
        delete [] _pData;
        _pData = new_p;
    }
    for(int iIdx = (_iNumber - 1); iIdx >= iInsertIdx; iIdx--)
    {
        _pData[iIdx + 1] = _pData[iIdx];
    }
    _pData[iInsertIdx] = NewData;
    _iNumber++;
}

template<class Type>
void FlexArray<Type>::swap(int iIdx0, int iIdx1)
{
    Type tmp = _pData[iIdx0];
    _pData[iIdx0] = _pData[iIdx1];
    _pData[iIdx1] = tmp;
}


#endif
