#pragma once
#include "../utils/type.h"
#include "../utils/flexarray.h"
#include "LaneMarkerLines.h"

class BallotBox{
private:
	int _iHeight;	              //投票框高
	int _iVotingNumber;	          //投票数
	int *_piVotingTableOffset;    //[CS4_NEARIMAGEHEIGHT][CS4_NUNBER_OF_NEAR_BALLOT];
	int *_piVotingTableYaw;       //[CS4_NEARIMAGEHEIGHT][CS4_NUNBER_OF_NEAR_BALLOT];
	int *_piVotTablCnt;           //[CS4_NEARIMAGEHEIGHT];
	int *_piVotingTableWeight;    //[CS4_NEARIMAGEHEIGHT][CS4_NUNBER_OF_NEAR_BALLOT];

	int _iNumberOfOffset;	      // offset
	int _iNumberOfYaw;		      // yaw
	unsigned int *_piBallotBox;	  //
	int _iVotingPointNumber;	  //
	int _iWeightMax;			  //
	int _iMaxYaw;				  // 最大倾角
	int _iOffsetMin;	          //
	int _iOffsetMax;	          //
	int _iYawMin;		          //
	int _iYawMax;		          //
	int _iVoteingThreshold;	      // 投票阈值
	int _SearchOffsetMin;	      //
	int _SearchOffsetMax;	      //
	int _SearchYawMin;		      //
	int _SearchYawMax;		      //

public:
	inline BallotBox(void)	{
		_iHeight = CS4_NEARIMAGEHEIGHT;
		_iVotingNumber = CS4_NUNBER_OF_NEAR_BALLOT;
		_piVotingTableOffset = NULL;
		_piVotingTableYaw = NULL;
		_piVotTablCnt = NULL;
		_piVotingTableWeight = NULL;

		_iNumberOfOffset = CS4_NUMBER_OF_NEAR_OFFSET;
		_iNumberOfYaw = CS4_NUMBER_OF_NEAR_YAW;
		_piBallotBox = NULL;
		_iVotingPointNumber = 0;
		_iWeightMax = CS4_WEIGHT_MAX;
		_iMaxYaw = CS4_NEAR_MAX_YAW;
		_iOffsetMin = 0;
		_iOffsetMax = CS4_NUMBER_OF_NEAR_OFFSET - 1;
		_iYawMin = -CS4_NEAR_MAX_YAW;
		_iYawMax = CS4_NEAR_MAX_YAW;
		_iVoteingThreshold = CS4_MINIMUM_VOTES;
		_SearchOffsetMin	=	_iOffsetMin;
		_SearchOffsetMax	=	_iOffsetMax;
		_SearchYawMin		=	_iYawMin;
		_SearchYawMax		=	_iYawMax;
	}
	inline ~BallotBox(void)	{
		SAFE_DELETE_ARRAY(_piBallotBox);
		SAFE_DELETE_ARRAY(_piVotingTableOffset);
		SAFE_DELETE_ARRAY(_piVotingTableYaw);
		SAFE_DELETE_ARRAY(_piVotTablCnt);
		SAFE_DELETE_ARRAY(_piVotingTableWeight);
	}
//	BallotBox(int iHeight, int iVotingNumber);
	inline unsigned int **data(void)	{	return &_piBallotBox;	}

	void makeVotingTable(int iHeight, int iWidth, int iImageHeight, int iImageWidth);
	void clear(void);
	void initialize();
	inline int Height(void)	{	return _iHeight;	}
	inline int VotingNumber(void)	{	return _iVotingNumber;	}
	inline int VotingPointNumber(void)	{	return _iVotingPointNumber;	}
	inline void VotingPointNumber(int iV)	{	_iVotingPointNumber = iV;	}
	inline void incVotingPointNumber(void)	{	_iVotingPointNumber++;	}
	inline int VotingTableCount(int iIdst)	{	return _piVotTablCnt[iIdst];	}
	inline int VotingTableOffset(int iIdst, int iIdx)	{	return _piVotingTableOffset[VotingNumber() * iIdst + iIdx];	}
	inline int VotingTableYaw(int iIdst, int iIdx)	{	return _piVotingTableYaw[VotingNumber() * iIdst + iIdx];	}
	inline int VotingTableWeight(int iIdst, int iIdx)	{	return _piVotingTableWeight[VotingNumber() * iIdst + iIdx];	}
	inline int NumberOfOffset(void)	{	return _iNumberOfOffset;	}
	inline int NumberOfYaw(void)		{	return _iNumberOfYaw;		}
	inline int Box(int iOffset, int iYaw)	{	return _piBallotBox[NumberOfYaw() * iOffset + iYaw];	}
	inline int WeightMax(void)			{	return _iWeightMax;			}
	inline int MaxYaw(void)			{	return _iMaxYaw;			}
	inline int OffsetMin(void)	{	return _iOffsetMin;	}
	inline int OffsetMax(void)	{	return _iOffsetMax;	}
	inline int YawMin(void)		{	return _iYawMin;		}
	inline int YawMax(void)		{	return _iYawMax;		}
	inline int SearchOffsetMin(void)	{	return	_SearchOffsetMin;	}
	inline int SearchOffsetMax(void)	{	return	_SearchOffsetMax;	}
	inline int SearchYawMin(void)	{	return	_SearchYawMin;	}
	inline int SearchYawMax(void)	{	return	_SearchYawMax;	}
	inline void SearchOffsetMin(int iV)	{	_SearchOffsetMin	=	iV;	}
	inline void SearchOffsetMax(int iV)	{	_SearchOffsetMax	=	iV;	}
	inline void SearchYawMin(int iV)	{	_SearchYawMin	=	iV;	}
	inline void SearchYawMax(int iV)	{	_SearchYawMax	=	iV;	}

	inline void Height(int iV)	{	_iHeight = iV;	}
	inline void VotingNumber(int iV)	{	_iVotingNumber = iV;	}
	inline void VotingTableCount(int iIdst, int iV)	{	_piVotTablCnt[iIdst] = iV;	}
	inline void VotingTableOffset(int iIdst, int iIdx, int iV)	{	_piVotingTableOffset[VotingNumber() * iIdst + iIdx] = iV;	}
	inline void VotingTableYaw(int iIdst, int iIdx, int iV)	{	_piVotingTableYaw[VotingNumber() * iIdst + iIdx] = iV;	}
	inline void VotingTableWeight(int iIdst, int iIdx, int iV)	{	_piVotingTableWeight[VotingNumber() * iIdst + iIdx] = iV;	}
	inline void NumberOfOffset(int iV)	{	_iNumberOfOffset = iV;	}
	inline void NumberOfYaw(int iV)		{	_iNumberOfYaw = iV;		}
	inline void Box(int iOffset, int iYaw, int iV)	{	_piBallotBox[NumberOfYaw() * iOffset + iYaw] = iV;	}
	inline void WeightMax(int iV)			{	_iWeightMax = iV;			}
	inline void MaxYaw(int iV)			{	_iMaxYaw = iV;			}
	inline void OffsetMin(int iV)	{	_iOffsetMin = iV;	}
	inline void OffsetMax(int iV)	{	_iOffsetMax = iV;	}
	inline void YawMin(int iV)		{	_iYawMin = iV;		}
	inline void YawMax(int iV)		{	_iYawMax = iV;		}
	inline int VotingThreshold(void)	{	return _iVoteingThreshold;	}
	inline void VotingThreshold(int iV)	{	_iVoteingThreshold = iV;	}

	LaneMarkerLines *searchPeak(int iMinOffset, int iMaxOffset, int iMinYaw, int iMaxYaw, int iVotesThreshold);
	inline LaneMarkerLines *searchPeak(void)	{
		return searchPeak(SearchOffsetMin(), SearchOffsetMax(), SearchYawMin(), SearchYawMax(), VotingThreshold());
	}

};
