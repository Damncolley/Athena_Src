#ifndef _IMRGB_H_
#define _IMRGB_H_

#include <stdio.h>
#include <string.h>

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "type.h"

#define VALMAX	255
#define	RGB_R	0
#define	RGB_G	1
#define	RGB_B	2

#define LOBYTE(w)           ((Uchar)(((Ulong)(w)) & 0xff))
#define GetRValue(rgb)      (LOBYTE(rgb))
#define GetGValue(rgb)      (LOBYTE(((Ushort)(rgb)) >> 8))
#define GetBValue(rgb)      (LOBYTE((rgb)>>16))

enum ColorPlane	{	CP_RED = 0, CP_GREEN, CP_BLUE, CP_AVE, CP_ALL	};

class IMRGB
{
private:
    int _width;// = 0;
    int _height;// = 0;
    //Uchar *_data;// = NULL;		// bgrbgr...
    char *loadHeaderOfPPM(FILE *fp);
    BOOL loadFromBinaryPPM(FILE *fp);
    BOOL loadFromAsciiPPM(FILE *fp);
    char *loadHeaderOfPGM(FILE *fp);
    BOOL loadFromBinaryPGM(FILE *fp);
    BOOL loadFromAsciiPGM(FILE *fp);
public:
    //////////////////////////////////////////////////////////////////////////
    Uchar *_data;// = NULL;		// bgrbgr...
    //////////////////////////////////////////////////////////////////////////
    IMRGB(void);
    IMRGB(int width,int height);
    IMRGB(int width,int height, Uchar *pucR, Uchar *pucG, Uchar *pucB);
    IMRGB(char *fname);		//inline: load from PPM file
    IMRGB(FILE *fp);		//inline: load from PPM file
    IMRGB(IMRGB &);			// duplicator
    //IMRGB(LPBITMAPINFOHEADER pbmih);
    ~IMRGB();
    inline int width(void);		//inline
    inline int height(void);		//inline
    inline Uchar &r(int i,int j);//inline
    inline Uchar &g(int i,int j);//inline
    inline Uchar &b(int i,int j);//inline
    inline Uchar v(ColorPlane cp, int i, int j);
    inline Uchar *data(void);
    inline Uchar *dataOfLine(int h);
    BOOL loadFromPPM(char *fname);
    BOOL loadFromPPM(FILE *fp = stdin);
    BOOL loadFromPGM(char *fname);
    BOOL loadFromPGM(FILE *fp = stdin);
    BOOL saveAsPPM(FILE *fp = stdout);
    BOOL saveAsPPM(char *fname);
    BOOL saveAsPGM(int plane, FILE *fp = stdout);
    BOOL saveAsPGM(int plane, char *fname);
    BOOL saveAsXLS(FILE *fp = stdout);
    BOOL saveAsXLS(char *fname);
    BOOL loadFromBMP(char *fname);
    BOOL loadFromBMP(FILE *fp = stdin);
    BOOL loadFromBMR(char *, char *, int);//20031017 yamaga
    //	IMRGB *loadFromBMR(char *fname, int);//20031015 sugaya
    BOOL loadFromBMR(FILE *fp);
    BOOL loadFromPNR(char *, char *, int);
    BOOL loadFromPNR(FILE *fp, Uchar *pFrameData);
    BOOL writeToPNR(FILE *fp);
    BOOL saveAsBMP24(FILE *fp = stdout);
    BOOL saveAsBMP24(char *fname);
    BOOL loadFromPNG(char *fname);
    BOOL loadFromJPG(char *fname);
    BOOL saveAsJPG(char *fname, int iQuality);
    IMRGB *flipHorizontal(void);
    IMRGB *flipVertical(void);
    IMRGB *rotatePi(void);
    IMRGB *rotateQuarter(void);
    IMRGB *rotateQuarter(IMRGB *dst);
    IMRGB *rotateThreeQuarters(void);
    IMRGB *rotateThreeQuarters(IMRGB *dst);

    void verticalSlide(int di);
    void horizontalSlide(int dj);

    IMRGB *shrink(int scale);
    IMRGB *shrink(IMRGB *dst, int scale);
    IMRGB *shrink(int vscale, int hscale);
    IMRGB *shrink(IMRGB *dst, int vscale, int hscale);

    IMRGB *enlarge(int scale);
    IMRGB *enlarge(IMRGB *dst, int scale);
    IMRGB *enlarge(int vscale, int hscale);
    IMRGB *enlarge(IMRGB *dst, int vscale, int hscale);

    IMRGB *resize(int iSrcTop, int iSrcLeft, int iSrcHeight, int iSrcWidth, int iDstHeight, int iDstWidth);
    IMRGB *partial(int uli, int ulj, int height, int width);
    IMRGB *partial(IMRGB *dst, int uli, int ulj, int height, int width);

//#ifndef	FOR_TMC
//		IMRGB *partial(AwBox *pBox);
//		IMRGB *partial(IMRGB *dst, AwBox *pBox);
//#endif	FOR_TMC

    IMRGB *upperHalf(void);
    IMRGB *upperHalf(IMRGB *dst);
    IMRGB *lowerHalf(void);
    IMRGB *lowerHalf(IMRGB *dst);

    IMRGB *upperHalfWithOriginalSize(void);
    IMRGB *upperHalfWithOriginalSize(IMRGB *dst);
    IMRGB *lowerHalfWithOriginalSize(void);
    IMRGB *lowerHalfWithOriginalSize(IMRGB *dst);

    Ulong *packToLong(void);
    Ulong *packToLong(Ulong *);
    Ulong *packToLongWithVerticalFlip(void);
    Ulong *packToLongWithVerticalFlip(Ulong *);
    Uchar *packToBGR(void);
    Uchar *packToBGR(Uchar *);
    Uchar *packTo8(void);
    Uchar *packTo8(Uchar *);
    Uchar *packTo8(int plane);
    Uchar *packTo8(Uchar *, int plane);
    inline void clear(void);
    inline void fillWhite(void);
    void putIMRGB(IMRGB *src, int i, int j);
    void putIMRGBWithBackGround(IMRGB *src, int uli, int ulj, int iBGred, int iBGgreen, int iBGblue);
    void putIMRGBAlpha(IMRGB *src, int uli, int ulj, double dAlpha);
    void putIMRGBAlpha(IMRGB *src, int uli, int ulj);
    void putBinaryAlpha(IMRGB *src, int uli, int ulj, double dAlpha, int iRed, int iGreen, int iBlue);
    void drawPoint(int i, int j, int r, int g, int b);
    void drawPoint(int i, int j, int r, int g, int b, double alpha);
    void drawPoint(int i, int j, int r, int g, int b, int scale);
    void drawPoint3(int i, int j, int r, int g, int b);
    void drawCross(int i, int j, int r, int g, int b, int scale);
    void drawCross(int i, int j, int r, int g, int b, int scale, int tick);
    void drawCross3(int i, int j, int r, int g, int b, int scale, int tick);
    void drawCrossX(int i, int j, int r, int g, int b, int scale);
    void drawPoint2(int i, int j, int r, int g, int b);
    void drawLine(int si, int sj, int ei, int ej, int r, int g, int b);
    void drawLine(int si, int sj, int ei, int ej, int r, int g, int b, double transparency);
    void drawThickLine(int si, int sj, int ei, int ej, int r, int g, int b, int iThickness);
    void drawLineWithClipping(int si, int sj, int ei, int ej, int rv, int gv, int bv, int imin, int imax, int jmin, int jmax);
    void drawLine2(int si, int sj, int ei, int ej, int r, int g, int b);
    void drawLine3(int si, int sj, int ei, int ej, int r, int g, int b);
    void drawDashLine(int si, int sj, int ei, int ej, int rv, int gv, int bv, int dash_step);
    void drawThickDashLine(int si, int sj, int ei, int ej, int rv, int gv, int bv, int iThickness, int dash_step);
    void drawLineWithFilledArrow(int si, int sj, int ei, int ej, int arrowLength, double arrowAngle, int r, int g, int b);
    void drawThickLineWithFilledArrow(int si, int sj, int ei, int ej, int arrowLength, double arrowAngle, int r, int g, int b, int iThickness);
    void drawRect(int uli, int ulj, int h, int w, int r, int g, int b);
    void drawThickRect(int uli, int ulj, int h, int w, int r, int g, int b, int iThickness);
    void drawRect(int uli, int ulj, int h, int w, COLORREF cr_v);
    void drawRectFill(int uli, int ulj, int h, int w, int r, int g, int b);
    void drawRectFill(int uli, int ulj, int h, int w, int r, int g, int b, double transparency);
    void drawRectFill(int uli, int ulj, int h, int w, COLORREF cr_v);
    void drawRectFill(int uli, int ulj, int h, int w, COLORREF cr_v, double transparency);
    void drawRectWithRotation(int uli, int ulj, int h, int w, int r, int g, int b, double rad);
    void drawRectFillWithRotation(int uli, int ulj, int h, int w, int r, int g, int b, double rad);
//#ifndef	FOR_TMC
//		void drawRect(AwBox *pBox, int r, int g, int b);
//		void drawRect(AwBox *pBox, COLORREF cr_v);
//		void drawRectFill(AwBox *pBox, int r, int g, int b);
//		void drawRectFill(AwBox *pBox, int r, int g, int b, double transparency);
//		void drawRectFill(AwBox *pBox, COLORREF cr_v);
//		void drawRectFill(AwBox *pBox, COLORREF cr_v, double transparency);
//		void drawRectWithRotation(AwBox *pBox, int r, int g, int b, double rad);
//		void drawRectFillWithRotation(AwBox *pBox, int r, int g, int b, double rad);
//#endif	FOR_TMC

    void drawPointWithPseudColor(int i, int j, double v, double min_v = 0.0, double max_v = 1.0);
//		void drawTriangularFill(int i0, int j0, double i1, double j1, double i2, double j2, int rv, int gv, int bv);
    void drawTriangularFill(int i0, int j0, int i1, int j1, int i2, int j2, int rv, int gv, int bv);
    void drawLinePolar(double rho, double theta, int r, int g, int b);
    void drawLinePolar2(double rho, double theta, int r, int g, int b);
    void drawLinePolar(int i0, int j0, double rho, double theta, int r, int g, int b);
    void drawLinePolar(int i0, int j0, double rho, double theta, int r, int g, int b, double transparency);
    void calcBoundaryPointOnLinePolar(int i0, int j0, double rho, double theta, int *piIb0, int *piJb0, int *piIb1, int *piJb1);
    void drawPoint(int i, int j, COLORREF cr_v);
    void drawPoint(int i, int j, COLORREF cr_v, int scale);
    void drawCross(int i, int j, COLORREF cr_v, int scale);
    void drawCross3(int i, int j, COLORREF cr_v, int scale);
    void drawCrossX(int i, int j, COLORREF cr_v, int scale);
    void drawPoint2(int i, int j, COLORREF cr_v);
    void drawLine(int si, int sj, int ei, int ej, COLORREF cr_v);
    void drawLineWithClipping(int si, int sj, int ei, int ej, COLORREF cr_v, int imin, int imax, int jmin, int jmax);
    void drawLine2(int si, int sj, int ei, int ej, COLORREF cr_v);
    void drawDashLine(int si, int sj, int ei, int ej, COLORREF cr_v, int dash_step);

    void drawEllipse(int i0, int j0, int h, int w, double phi, double ts, double te, int r, int g, int b);
    void drawEllipse(int i0, int j0, int h, int w, double phi, int r, int g, int b);
    void drawEllipseFill(int i0, int j0, int h, int w, double phi, double ts, double te, int r, int g, int b);
    void drawEllipseFill(int i0, int j0, int h, int w, double phi, int r, int g, int b);
    void drawPie(int i0, int j0, int h, int w, double phi, double ts, double te, int r, int g, int b);

//#ifndef	FOR_TMC
//		void drawPolyLine(AwPoints *pPoints, int r, int g, int b);
//		void drawPolygone(AwPoints *pPoints, int r, int g, int b);
//		void drawPolygone(AwPoints *pPoints, int r, int g, int b, int iThickness);
//		void drawPolygoneFill(AwPoints *pPoints, int r, int g, int b);
//#endif	FOR_TMC

    IMRGB *W720toW640(void);
    IMRGB *W720toW640(IMRGB *dst);
    void putIMRGBEachPlane(IMRGB *src, int uli, int ulj, BOOL fR, BOOL fG, BOOL fB);
    void convertToGray(ColorPlane cp);

    void convertToGrayScaleImage(int plane);
    void convertToGrayScaleImage(int plane, IMRGB *dst);

    IMRGB *sobel(void);
    IMRGB *sobel(IMRGB *pDst);
    Uchar medianValue(ColorPlane cp, int i, int j, int size = 3);
    void median(ColorPlane cp);
    void median(ColorPlane cp, IMRGB *dst);
    void median(void);
    void median(IMRGB *dst);
    Uchar snnfValue(ColorPlane cp, int i, int j);
    void snnf(ColorPlane cp);
    void snnf(ColorPlane cp, IMRGB *dst);
    void snnf(void);
    void snnf(IMRGB *dst);
    void snnfSelf(ColorPlane cp);
    void snnfSelf(void);
    void toEven(void);
    void toOdd(void);

    IplImage *toIplImage8(ColorPlane cp);
    IplImage *toIplImage8(ColorPlane cp, IplImage *);
    IplImage *toIplImage(void);
    IplImage *toIplImage(IplImage *);

    void inverse(void);

    BOOL histogram(int iBinNumber, int *r_hist, int *g_hist, int *b_hist);

    Ulong sad(int iUpper, int iLeft, int iHeight, int iWidth, IMRGB *pImTarget, int iTargetUpper, int iTargetLeft, ColorPlane cp);
    long searchMinimumSAD(int iUpper, int iLeft, int iHeight, int iWidth, IMRGB *pImTarget, int iTargetUpper, int iTargetLeft, int iTargetHeight, int iTargetWidth, int *piIpos, int *piJpos, ColorPlane cp);

    BOOL difference(IMRGB *pImSubtractor);
    BOOL binarize(Uchar th);
    BOOL binarize(Uchar th, IMRGB *dst);
    void convertToPseudColor2(void);
    void convertToPseudColor2(IMRGB *dst);
    void expandDynamicRange(int iMinV, int iMaxV);
    void expandDynamicRange(IMRGB *dst, int iMinV, int iMaxV);

    void edgeHistogram(ColorPlane cp, int iTop, int iLeft, int iBottom, int iRight, double *pdHistV, double *pdHistH);
    void drawHistogram(double *pdHistV, double *pdHistH, int iSize, int r, int g, int b);

    void calcStatistic(ColorPlane cp, int iTop, int iLeft, int iBottom, int iRight, int *piAverage, int *piStdDev);
    void calcStatisticHorizontalDifferential(ColorPlane cp, int iTop, int iLeft, int iBottom, int iRight, int *piAverageDif, int *piStdDevDif);
    double calcIntensityAverage(ColorPlane cp, int iIcenter, int iJcenter, int iHeight, int iWidth);
    BOOL calcIntensityAverageWithFOE(ColorPlane cp, int iIfoe, int iJfoe, int iIcenter, int iJcenter, int iHeight, int iWidth, double *pdLeftAverage, double *pdRightAverage);
    BOOL calcIntensityVarianceWithFOE(ColorPlane cp, int iIfoe, int iJfoe, int iIcenter, int iJcenter, int iHeight, int iWidth, double *pdLeftVariance, double *pdRightVariance);
    double calcIntensityAverageOfHorizontalLine(ColorPlane cp, int iIcenter, int iHeight, int iWidth, double *pdAverage);
    double calcIntensityAverageRatioOfHorizontalLineWithFOE(ColorPlane cp, int iIfoe, int iJfoe, int iIcenter, int iHeight, int iWidth, double *pdRatio);
    double calcIntensityVarianceRatioOfHorizontalLineWithFOE(ColorPlane cp, int iIfoe, int iJfoe, int iIcenter, int iHeight, int iWidth, double *pdRatio);
};
//#ifndef	FOR_TMC
//extern IMRGB *makeIMRGBfromIplImage8(IplImage *pIplImage, IMRGB *pDst);
//extern IMRGB *makeIMRGBfromIplImage8(IplImage *pIplImage);
//extern IMRGB *makeIMRGBfromIplImage(IplImage *pIplImage, IMRGB *pDst);
//extern IMRGB *makeIMRGBfromIplImage(IplImage *pIplImage);
//#endif

extern double calcNormalizedCorrelation( IMRGB *pImrgbTemp, IMRGB *pImrgbDst, int iTempTop, int iTempLeft, int iTempHeight, int TempiWidth,	 int iDstTop, int iDstLeft);
extern BOOL searchByNormalizedCorrelation( IMRGB *pImrgbTemp, IMRGB *pImrgbSearch, int iTempTop, int iTempLeft, int iTempHeight, int iTempWidth, int iSearchTop, int iSearchLeft, int iSearchHeight, int iSearchWidth, int *piMaxIdxV, int *piMaxIdxH, double *pdMaxValue);
extern double calcSAD( IMRGB *pImrgbTemp, IMRGB *pImrgbDst, int iTempTop, int iTempLeft, int iTempHeight, int TempiWidth,	 int iDstTop, int iDstLeft);
extern BOOL searchBySAD( IMRGB *pImrgbTemp, IMRGB *pImrgbSearch, int iTempTop, int iTempLeft, int iTempHeight, int iTempWidth, int iSearchTop, int iSearchLeft, int iSearchHeight, int iSearchWidth, int *piMaxIdxV, int *piMaxIdxH, double *pdMaxValue);
extern double calcSSD( IMRGB *pImrgbTemp, IMRGB *pImrgbDst, int iTempTop, int iTempLeft, int iTempHeight, int TempiWidth,	 int iDstTop, int iDstLeft);
extern BOOL searchBySSD( IMRGB *pImrgbTemp, IMRGB *pImrgbSearch, int iTempTop, int iTempLeft, int iTempHeight, int iTempWidth, int iSearchTop, int iSearchLeft, int iSearchHeight, int iSearchWidth, int *piMaxIdxV, int *piMaxIdxH, double *pdMaxValue);

extern IMRGB *calcSubAbs(IMRGB *pIMRGBminuend, IMRGB *pIMRGBsubtracter, IMRGB *pIMRGBresult);
extern IMRGB *calcSubAbs(IMRGB *pIMRGBminuend, IMRGB *pIMRGBsubtracter);

inline
IMRGB::IMRGB(void):
    _width(0),
    _height(0),
    _data(NULL)
{
}

inline
IMRGB::IMRGB(char *fname):
    _width(0),
    _height(0),
    _data(NULL)
{
    loadFromPPM(fname);
}

inline
IMRGB::IMRGB(FILE *fp):
    _width(0),
    _height(0),
    _data(NULL)
{
    loadFromPPM(fp);
}

inline int
IMRGB::width(void)
{
    return(_width);
}

inline int
IMRGB::height(void)
{
    return(_height);
}

inline Uchar &
IMRGB::r(int i, int j)
{
    return(_data[(i * width() + j) * 3 + 2]);
}
inline Uchar &
IMRGB::g(int i, int j)
{
    return(_data[(i * width() + j) * 3 + 1]);
}
inline Uchar &
IMRGB::b(int i, int j)
{
    return(_data[(i * width() + j) * 3 + 0]);
}
inline Uchar
IMRGB::v(ColorPlane cp, int i, int j)
{
    switch(cp)
    {
    case	CP_ALL:
        return (Uchar)(((Ulong)r(i,j) + (Ulong)g(i,j) + (Ulong)b(i,j)) / 3);
    case	CP_RED:
        return r(i,j);
    case	CP_GREEN:
        return g(i,j);
    case	CP_BLUE:
        return b(i,j);
    case    CP_AVE:
        return r(i,j);
    }
    return 0;
}

inline Uchar *
IMRGB::data(void)
{
    return _data;
}

inline Uchar *
IMRGB::dataOfLine(int h)
{
    return(data() + width() * h * 3);
}

inline void
IMRGB::clear(void)
{
    memset(_data, 0, height() * width() * 3);
}

inline void
IMRGB::fillWhite(void)
{
    memset(_data, 255, height() * width() * 3);
}

inline void IMRGB::drawPoint(int i, int j, COLORREF cr_v)
{
    drawPoint(i, j, GetRValue(cr_v), GetGValue(cr_v), GetBValue(cr_v));
}
inline void IMRGB::drawPoint(int i, int j, COLORREF cr_v, int scale)
{
    drawPoint(i, j, GetRValue(cr_v), GetGValue(cr_v), GetBValue(cr_v), scale);
}
inline void IMRGB::drawCross(int i, int j, COLORREF cr_v, int scale)
{
    drawCross(i, j, GetRValue(cr_v), GetGValue(cr_v), GetBValue(cr_v), scale);
}
inline void IMRGB::drawCrossX(int i, int j, COLORREF cr_v, int scale)
{
    drawCrossX(i, j, GetRValue(cr_v), GetGValue(cr_v), GetBValue(cr_v), scale);
}
inline void IMRGB::drawPoint2(int i, int j, COLORREF cr_v)
{
    drawPoint2(i, j, GetRValue(cr_v), GetGValue(cr_v), GetBValue(cr_v));
}
inline void IMRGB::drawLine(int si, int sj, int ei, int ej, COLORREF cr_v)
{
    drawLine(si, sj, ei, ej, GetRValue(cr_v), GetGValue(cr_v), GetBValue(cr_v));
}
inline void IMRGB::drawLineWithClipping(int si, int sj, int ei, int ej, COLORREF cr_v, int imin, int imax, int jmin, int jmax)
{
    drawLineWithClipping(si, sj, ei, ej, GetRValue(cr_v), GetGValue(cr_v), GetBValue(cr_v), imin, imax, jmin, jmax);
}
inline void IMRGB::drawLine2(int si, int sj, int ei, int ej, COLORREF cr_v)
{
    drawLine2(si, sj, ei, ej, GetRValue(cr_v), GetGValue(cr_v), GetBValue(cr_v));
}
inline void IMRGB::drawDashLine(int si, int sj, int ei, int ej, COLORREF cr_v, int dash_step)
{
    drawDashLine(si, sj, ei, ej, GetRValue(cr_v), GetGValue(cr_v), GetBValue(cr_v), dash_step);
}
inline void IMRGB::drawRect(int uli, int ulj, int h, int w, COLORREF cr_v)
{
    drawRect(uli, ulj, h, w, GetRValue(cr_v), GetGValue(cr_v), GetBValue(cr_v));
}
inline void IMRGB::drawRectFill(int uli, int ulj, int h, int w, COLORREF cr_v)
{
    drawRectFill(uli, ulj, h, w, GetRValue(cr_v), GetGValue(cr_v), GetBValue(cr_v));
}
inline void IMRGB::drawRectFill(int uli, int ulj, int h, int w, COLORREF cr_v, double transparency)
{
    drawRectFill(uli, ulj, h, w, GetRValue(cr_v), GetGValue(cr_v), GetBValue(cr_v), transparency);
}
//#ifndef	FOR_TMC
//inline void IMRGB::drawRect(AwBox *pBox, int r, int g, int b)
//{
//	if(pBox == NULL)	return;
//	drawRect(pBox->getTop(), pBox->getLeft(), pBox->getHeight(), pBox->getWidth(), r, g, b);
//}
//inline void IMRGB::drawRect(AwBox *pBox, COLORREF cr_v)
//{
//	if(pBox == NULL)	return;
//	drawRect(pBox->getTop(), pBox->getLeft(), pBox->getHeight(), pBox->getWidth(), cr_v);
//}
//inline void IMRGB::drawRectFill(AwBox *pBox, int r, int g, int b)
//{
//	if(pBox == NULL)	return;
//	drawRectFill(pBox->getTop(), pBox->getLeft(), pBox->getHeight(), pBox->getWidth(), r, g, b);
//}
//inline void IMRGB::drawRectFill(AwBox *pBox, int r, int g, int b, double transparency)
//{
//	if(pBox == NULL)	return;
//	drawRectFill(pBox->getTop(), pBox->getLeft(), pBox->getHeight(), pBox->getWidth(), r, g, b, transparency);
//}
//inline void IMRGB::drawRectFill(AwBox *pBox, COLORREF cr_v)
//{
//	if(pBox == NULL)	return;
//	drawRectFill(pBox->getTop(), pBox->getLeft(), pBox->getHeight(), pBox->getWidth(), cr_v);
//}
//inline void IMRGB::drawRectFill(AwBox *pBox, COLORREF cr_v, double transparency)
//{
//	if(pBox == NULL)	return;
//	drawRectFill(pBox->getTop(), pBox->getLeft(), pBox->getHeight(), pBox->getWidth(), cr_v, transparency);
//}
//inline void IMRGB::drawRectWithRotation(AwBox *pBox, int r, int g, int b, double rad)
//{
//	if(pBox == NULL)	return;
//	drawRectWithRotation(pBox->getTop(), pBox->getLeft(), pBox->getHeight(), pBox->getWidth(), r, g, b, rad);
//}
//inline void IMRGB::drawRectFillWithRotation(AwBox *pBox, int r, int g, int b, double rad)
//{
//	if(pBox == NULL)	return;
//	drawRectFillWithRotation(pBox->getTop(), pBox->getLeft(), pBox->getHeight(), pBox->getWidth(), r, g, b, rad);
//}
//
//inline IMRGB *IMRGB::partial(AwBox *pBox)
//{
//	if(pBox == NULL)	return NULL;
//	return partial(pBox->getTop(), pBox->getLeft(), pBox->getHeight(), pBox->getWidth());
//}
//inline IMRGB *IMRGB::partial(IMRGB *dst, AwBox *pBox)
//{
//	if(pBox == NULL)	return NULL;
//	return partial(dst, pBox->getTop(), pBox->getLeft(), pBox->getHeight(), pBox->getWidth());
//}
//#endif	FOR_TMC

extern void sortUchar(Uchar *a_ucBuf, int iNumber);
#endif
