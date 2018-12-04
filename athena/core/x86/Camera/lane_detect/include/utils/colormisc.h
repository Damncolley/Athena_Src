
#ifndef	_COLORMISC_H_
#define	_COLORMISC_H_

#include "type.h"

extern BOOL convertToPseudColor(double v, Uchar *r, Uchar *g, Uchar *b, double min_v = 0.0, double max_v = 1.0);
extern BOOL convertToPseudColor2(double v, Uchar *r, Uchar *g, Uchar *b, double min_v, double max_v);
extern BOOL convertToPseudColor3(double v, Uchar *r, Uchar *g, Uchar *b, double min_v, double max_v);
extern double rgbtoh(Uchar R, Uchar G, Uchar B);
extern double rgbtos(Uchar R, Uchar G, Uchar B);
extern double rgbtoi(Uchar R, Uchar G, Uchar B);
extern Uchar rgbtog(Uchar R, Uchar G, Uchar B);

#endif
