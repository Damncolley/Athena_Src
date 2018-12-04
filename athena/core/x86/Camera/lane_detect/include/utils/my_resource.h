//
//	file:	my_resource.h
//  author: krutch_zhou
//  email:zhtwhut163@163.com
//	$modified: 2015/06/27
//	$Log: my_resource.h,v $
//
#ifndef	_MY_RESOURCE_H_
#define	_MY_RESOURCE_H_
#define ICON_FM                         105

#define	RESOURCE_ID_BASE	WM_USER


#define	MENU_QUIT															(RESOURCE_ID_BASE + 0)
#define	MENU_PAUSE															(RESOURCE_ID_BASE + 1)


///////////////////////////////////////
#define	CW_IDC_BUTTON_REWIND												(RESOURCE_ID_BASE + 2)
#define	CW_IDC_EDIT_FRAMENUM												(RESOURCE_ID_BASE + 3)
#define	CW_IDC_BUTTON_FRAMESET												(RESOURCE_ID_BASE + 4)

#define	CW_IDC_BUTTON_PREV2													(RESOURCE_ID_BASE + 5)
#define	CW_IDC_BUTTON_PREV													(RESOURCE_ID_BASE + 6)
#define	CW_IDC_PAUSE														(RESOURCE_ID_BASE + 7)
#define	CW_IDC_BUTTON_NEXT													(RESOURCE_ID_BASE + 8)
#define	CW_IDC_BUTTON_NEXT2													(RESOURCE_ID_BASE + 9)

#define	CW_IDC_BUTTON_FREEZE												(RESOURCE_ID_BASE + 10)
#define	CW_IDC_BUTTON_RESET													(RESOURCE_ID_BASE + 11)
#define	CW_IDC_BUTTON_RESET_WINDOW											(RESOURCE_ID_BASE + 12)
#define	CW_IDC_QUIT															(RESOURCE_ID_BASE + 73)

#define	CW_IDC_EDIT_STOPATNUM												(RESOURCE_ID_BASE + 13)
#define	CW_IDC_BUTTON_STOPAT												(RESOURCE_ID_BASE + 14)
#define	CW_IDC_BUTTON_PAUSEONRESET											(RESOURCE_ID_BASE + 15)

///////////////////////////////////////
#define	CW_IDC_BUTTON_SaveIniFile											(RESOURCE_ID_BASE + 16)
#define	CW_IDC_BUTTON_LoadIniFile											(RESOURCE_ID_BASE + 17)
///////////////////////////////////////
#define	CW_IDC_BUTTON_saveAsAVI												(RESOURCE_ID_BASE + 18)
#define	CW_IDC_BUTTON_toggleBatchModeFlagOfOutputAVI						(RESOURCE_ID_BASE + 19)
#define	CW_IDC_BUTTON_saveDetectionLog										(RESOURCE_ID_BASE + 20)
#define	CW_IDC_BUTTON_saveLaneMarkerInfo									(RESOURCE_ID_BASE + 21)
#define	CW_IDC_BUTTON_saveLaneParameters									(RESOURCE_ID_BASE + 22)
#define	CW_IDC_BUTTON_saveEvaluationLog										(RESOURCE_ID_BASE + 23)



#define	CW_IDC_BUTTON_DrawNearMidFarLineOnInputImageFunc					(RESOURCE_ID_BASE + 24)
#define	CW_IDC_BUTTON_DrawFrameNumberOnInputImageFunc						(RESOURCE_ID_BASE + 25)
#define	CW_IDC_BUTTON_DrawDetectionStatus									(RESOURCE_ID_BASE + 26)
#define	CW_IDC_BUTTON_DrawLaneParametersOnInputImageFunc					(RESOURCE_ID_BASE + 27)

#define	CW_IDC_BUTTON_DrawDetectionTimeOnInputImageFunc						(RESOURCE_ID_BASE + 28)
#define	CW_IDC_BUTTON_DrawFileNameOnInputImageFunc							(RESOURCE_ID_BASE + 29)
#define	CW_IDC_BUTTON_DrawDetectionModeOnInputImageFunc						(RESOURCE_ID_BASE + 30)

///////////////////////////////////////

#define	CW_IDC_BUTTON_DrawInputImageFunc									(RESOURCE_ID_BASE + 31)
#define	CW_IDC_BUTTON_DrawDisparityOnInputImageFunc							(RESOURCE_ID_BASE + 32)

#define	CW_IDC_BUTTON_DrawLaneMarkerPointOnInputImageFunc					(RESOURCE_ID_BASE + 33)
#define	CW_IDC_BUTTON_DrawBoundaryPointOnInputImageFunc						(RESOURCE_ID_BASE + 34)
#define	CW_IDC_BUTTON_DrawObserverPointOnInputImageFunc						(RESOURCE_ID_BASE + 35)

#define	CW_IDC_BUTTON_DrawResultOnInputImageFunc							(RESOURCE_ID_BASE + 36)
#define	CW_IDC_BUTTON_DrawResultByNearAreaParameterOnInputImageFunc			(RESOURCE_ID_BASE + 77)
#define	CW_IDC_BUTTON_DrawCurbOnInputImageFunc								(RESOURCE_ID_BASE + 37)
#define	CW_IDC_BUTTON_DrawAnsTagOnInputImageFunc							(RESOURCE_ID_BASE + 38)

#define	CW_IDC_BUTTON_DrawLaneMarkerLineOnInputImageFunc					(RESOURCE_ID_BASE + 39)
#define	CW_IDC_BUTTON_DrawLaneMarkerLineSequencesOnInputImageFunc			(RESOURCE_ID_BASE + 40)
#define	CW_IDC_BUTTON_DrawLaneMarkerOnInputImageFunc						(RESOURCE_ID_BASE + 41)
#define	CW_IDC_BUTTON_DrawLaneBoundaryLineOnInputImageFunc					(RESOURCE_ID_BASE + 42)

///////////////////////////////////////

#define	CW_IDC_BUTTON_DrawRoadImageFunc										(RESOURCE_ID_BASE + 43)
#define	CW_IDC_BUTTON_DrawGridOnRoadImageFunc								(RESOURCE_ID_BASE + 44)
#define	CW_IDC_BUTTON_DisplayRoadWindow										(RESOURCE_ID_BASE + 45)
#define	CW_IDC_BUTTON_DrawRoadImageWithEstimatedPitch						(RESOURCE_ID_BASE + 46)

#define	CW_IDC_BUTTON_DrawLaneMarkerPointOnRoadImageFunc					(RESOURCE_ID_BASE + 47)
#define	CW_IDC_BUTTON_DrawBoundaryPointOnRoadImageFunc						(RESOURCE_ID_BASE + 48)
#define	CW_IDC_BUTTON_DrawObserverPointOnRoadImageFunc						(RESOURCE_ID_BASE + 49)

#define	CW_IDC_BUTTON_DrawResultOnRoadImageFunc								(RESOURCE_ID_BASE + 50)

#define	CW_IDC_BUTTON_DrawLaneMarkerLineOnRoadImageFunc						(RESOURCE_ID_BASE + 51)
#define	CW_IDC_BUTTON_DrawLaneMarkerLineSequencesOnRoadImageFunc			(RESOURCE_ID_BASE + 52)
#define	CW_IDC_BUTTON_DrawLaneMarkerOnRoadImageFunc							(RESOURCE_ID_BASE + 53)
#define	CW_IDC_BUTTON_DrawLaneBoundaryLineOnRoadImageFunc					(RESOURCE_ID_BASE + 54)


#define	CW_IDC_BUTTON_SearchWithParameterInNearArea							(RESOURCE_ID_BASE + 69)
#define	CW_IDC_BUTTON_UseFarArea											(RESOURCE_ID_BASE + 70)

#define	CW_IDC_BUTTON_UseComplexMode										(RESOURCE_ID_BASE + 71)
#define	CW_IDC_BUTTON_UseSelectLaneBoundaryByComplexMode					(RESOURCE_ID_BASE + 72)

#define	CW_IDC_BUTTON_DrawSelectedEdgePointOn3DImageFunc					(RESOURCE_ID_BASE + 74)
#define	CW_IDC_BUTTON_DrawGridMap1DFunc										(RESOURCE_ID_BASE + 75)

#define	CW_IDC_BUTTON_UseBB_DIFF											(RESOURCE_ID_BASE + 76)

///////////////////////////////////////



#endif	_MY_RESOURCE_H_

