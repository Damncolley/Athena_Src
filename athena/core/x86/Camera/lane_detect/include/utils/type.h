#ifndef _TYPE_H_
#define _TYPE_H_

#include <stdlib.h>

typedef signed char		Schar;
typedef signed short	Sshort;
typedef signed int		Sint;
typedef signed long		Slong;
typedef unsigned char	Uchar;
typedef unsigned short	Ushort;
typedef unsigned int	Uint;
typedef unsigned long	Ulong;
typedef Ulong COLORREF;

typedef	int	BOOL;
//enum {FALSE = 0, TRUE};

#define FALSE 0
#define TRUE 1

typedef union {
	Uchar b[4];
	Ushort w[2];
	Ulong dw;
	float fl;
} LogData;

typedef char			S1;
typedef unsigned char	U1;
typedef short			S2;
typedef unsigned short 	U2;
typedef long			S4;
typedef unsigned long	U4;
typedef void			VD;
typedef char			I1;
typedef short			I2;
typedef long			I4;

typedef	float			FL;
typedef	double			DB;

#endif

