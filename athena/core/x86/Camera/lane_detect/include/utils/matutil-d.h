///////////////////////////////////////////////////////////////////////////////
//	file:	matutil-d.h
//  author: krutch_zhou
//  email:zhtwhut163@163.com
//	$modified: 2015/06/23
//	$Log: matutil-d.h,v $
///////////////////////////////////////////////////////////////////////////////
/******************************************/
/***	　行列計算用ヘッダファイル		***/
/******************************************/
#include "config.h"

/*              sslib.h         include file for sslib          */
#ifndef _MATUTIL_D_H_
#define _MATUTIL_D_H_

#include  <math.h>
#ifndef	FOR_WTP
#include "type.h"
#endif	FOR_WTP


/******************************************/
/***		使用関数ﾌﾟﾛﾄﾀｲﾌﾟ宣言		***/
/******************************************/
/*      matrix.c        */
VD dmadd(DB a[], DB b[], DB c[], S4 m, S4 n);
VD dmsub(DB a[], DB b[], DB c[], S4 m, S4 n);
VD dmmul1(DB a[], DB b[], S4 m);
VD dmmul2(DB a[], DB b[], DB c[], S4 m, S4 n, S4 k);
VD dmtra1(DB a[], S4 m, S4 n);
VD dmtra2(DB a[], DB b[], S4 m, S4 n);
DB dminver(DB a[], DB b[], S4 l, S4 m, DB eps);
VD dmmove(DB a[], DB b[], S4 m, S4 n);
VD dmswap(DB a[], DB b[], S4 m, S4 n);

// V added by watanabe at 20090914
extern BOOL dminver2(DB a[], DB b[], S4 l, S4 m);
// A added by watanabe at 20090914
// V added by watanabe at 20090918
extern VD dmUDdecomposition(DB *dbS, DB *dbU, DB *dbD, S4 s4_Dim);
// A added by watanabe at 20090918
// V added by watanabe at 20090923
extern VD dmColumnVector(DB *adbSrc, S4 s4_height, S4 s4_width, DB *adbDst, S4 s4_column);
extern VD dmRawVector(DB *adbSrc, S4 s4_height, S4 s4_width, DB *adbDst, S4 s4_raw);
extern VD dmPutSubMatrix(DB *adbDst, S4 s4_DstHeight, S4 s4_DstWidth, DB *adbSub, S4 s4_SubHeight, S4 s4_SubWidth, S4 s4_raw, S4 s4_column);
extern VD dmSetZero(DB *adbDst, S4 s4_height, S4 s4_width);
extern VD dmAddSubMatrix(DB *adbDst, S4 s4_DstHeight, S4 s4_DstWidth, DB *adbSub, S4 s4_SubHeight, S4 s4_SubWidth, S4 s4_raw, S4 s4_column);
extern VD dmSubSubMatrix(DB *adbDst, S4 s4_DstHeight, S4 s4_DstWidth, DB *adbSub, S4 s4_SubHeight, S4 s4_SubWidth, S4 s4_raw, S4 s4_column);
// A added by watanabe at 20090923
#endif

