#ifndef	_ROADIMAGE_WINDOW_H_
#define	_ROADIMAGE_WINDOW_H_

////////////////////////////////////////////////////////////////////////////////
// personal header files and macros
#include "config.h"
#include "imrgb.h"
#include "my_resource.h"

///////////////////////////////////////////////////////////////////////////////////
// function definition
//////////////////////////////////////////////////////////////////////////
extern IMRGB *getRoadImage(void);
extern void drawRoadImageOnRoadImageWindow(IMRGB *pImrgb);

extern void makeRoadImage(IMRGB *a_imrgb_input);
extern void makeRoadImage(IMRGB *a_imrgb_input, DB dPitch);
extern void drawVerticalEdgePointOnRoadImage(void);
extern void drawHorizontalEdgePointOnRoadImage(void);
extern void drawVerticalEdgePeakOnRoadImage(void);
extern void drawHorizontalEdgePeakOnRoadImage(void);
extern void drawWhiteLinesOnRoadImage(void);
extern void drawRoadSideObjectsOnRoadImage(void);
extern void drawLaneMarksOnRoadImage(void);
extern void drawLaneBoundariesOnRoadImage(void);
extern void drawEdgeBoundariesOnRoadImage(void);
extern void drawLaneBoundaryPairsOnRoadImage(void);
extern void drawTopLaneBoundaryPairsOnRoadImage(void);
static void drawCrossWalkLineNumber(void);


static void drawDepthLine(void);
static void drawVerticalLine(void);
static void drawHorizontalLineDepth(void);
static void drawVerticalLineOffset(void);
#endif	_ROADIMAGE_WINDOW_H_
