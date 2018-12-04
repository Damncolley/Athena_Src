
#include "cs55_torque_speed_throttle_map.h"

struct table {
	double x[10];                        // Variable: x
										 //  Referenced by: '<Root>/2-D Lookup Table'

	double y[9];                         // Variable: y
										 //  Referenced by: '<Root>/2-D Lookup Table'

	double z[90];                        // Variable: z
										 //  Referenced by: '<Root>/2-D Lookup Table'

	unsigned int DLookupTable_maxIndex[2];   // Computed Parameter: DLookupTable_maxIndex
};


static const table table_reference = {
		//  Variable: x
		//  Referenced by: '<Root>/2-D Lookup Table'

		{ 1.92, 3.48, 5.37, 7.1, 10.15, 13.15, 15.84, 18.35, 22.5, 23.93 },

		//  Variable: y
		//  Referenced by: '<Root>/2-D Lookup Table'

		{ 8.0, 9.0, 10.0, 12.0, 16.0, 17.0, 19.0, 23.0, 34.0 },

		//  Variable: z
		//  Referenced by: '<Root>/2-D Lookup Table'

		{ 1.5, 7.4, 8.0, 9.5, 10.2, 11.0, 11.5, 12.5, 13.9, 14.9, 1.5, 7.5, 8.5,
		10.0, 10.7, 11.5, 12.0, 13.0, 14.4, 15.5, 7.0, 7.8, 9.0, 10.5, 11.5, 12.0,
		12.5, 13.6, 15.0, 15.9, 7.3, 8.2, 9.3, 11.0, 12.0, 12.5, 12.8, 14.3, 15.5,
		16.5, 8.0, 9.0, 10.3, 11.5, 12.5, 13.1, 13.5, 14.8, 16.1, 17.3, 8.7, 9.8,
		10.5, 12.0, 13.0, 13.8, 14.2, 15.3, 16.5, 17.7, 9.5, 10.3, 10.9, 12.5,
		13.5, 14.5, 14.8, 15.8, 17.1, 19.5, 10.0, 10.7, 11.4, 14.0, 14.5, 15.5,
		16.5, 17.5, 18.5, 20.5, 15.5, 16.5, 17.5, 18.5, 19.5, 20.5, 21.5, 22.5,
		23.5, 24.5 },

		//  Computed Parameter: DLookupTable_maxIndex
		//  Referenced by: '<Root>/2-D Lookup Table'

		{ 9U, 8U }
	};                                   // Modifiable parameters



double look2_binlxpw(double u0, double u1, const double bp0[], const double bp1[],
	const double table[], const unsigned int maxIndex[], unsigned int
	stride)
{
	double frac;
	unsigned int bpIndices[2];
	double fractions[2];
	double yL_1d;
	unsigned int iRght;
	unsigned int bpIdx;
	unsigned int iLeft;

	// Lookup 2-D
	// Search method: 'binary'
	// Use previous index: 'off'
	// Interpolation method: 'Linear'
	// Extrapolation method: 'Linear'
	// Use last breakpoint for index at or above upper limit: 'off'
	// Remove protection against out-of-range input in generated code: 'off'

	// Prelookup - Index and Fraction
	// Index Search method: 'binary'
	// Extrapolation method: 'Linear'
	// Use previous index: 'off'
	// Use last breakpoint for index at or above upper limit: 'off'
	// Remove protection against out-of-range input in generated code: 'off'

	if (u0 <= bp0[0U]) {
		iLeft = 0U;
		frac = (u0 - bp0[0U]) / (bp0[1U] - bp0[0U]);
	}
	else if (u0 < bp0[maxIndex[0U]]) {
		// Binary Search
		bpIdx = maxIndex[0U] >> 1U;
		iLeft = 0U;
		iRght = maxIndex[0U];
		while (iRght - iLeft > 1U) {
			if (u0 < bp0[bpIdx]) {
				iRght = bpIdx;
			}
			else {
				iLeft = bpIdx;
			}

			bpIdx = (iRght + iLeft) >> 1U;
		}

		frac = (u0 - bp0[iLeft]) / (bp0[iLeft + 1U] - bp0[iLeft]);
	}
	else {
		iLeft = maxIndex[0U] - 1U;
		frac = (u0 - bp0[maxIndex[0U] - 1U]) / (bp0[maxIndex[0U]] - bp0[maxIndex[0U]
			- 1U]);
	}

	fractions[0U] = frac;
	bpIndices[0U] = iLeft;

	// Prelookup - Index and Fraction
	// Index Search method: 'binary'
	// Extrapolation method: 'Linear'
	// Use previous index: 'off'
	// Use last breakpoint for index at or above upper limit: 'off'
	// Remove protection against out-of-range input in generated code: 'off'

	if (u1 <= bp1[0U]) {
		iLeft = 0U;
		frac = (u1 - bp1[0U]) / (bp1[1U] - bp1[0U]);
	}
	else if (u1 < bp1[maxIndex[1U]]) {
		// Binary Search
		bpIdx = maxIndex[1U] >> 1U;
		iLeft = 0U;
		iRght = maxIndex[1U];
		while (iRght - iLeft > 1U) {
			if (u1 < bp1[bpIdx]) {
				iRght = bpIdx;
			}
			else {
				iLeft = bpIdx;
			}

			bpIdx = (iRght + iLeft) >> 1U;
		}

		frac = (u1 - bp1[iLeft]) / (bp1[iLeft + 1U] - bp1[iLeft]);
	}
	else {
		iLeft = maxIndex[1U] - 1U;
		frac = (u1 - bp1[maxIndex[1U] - 1U]) / (bp1[maxIndex[1U]] - bp1[maxIndex[1U]
			- 1U]);
	}


	bpIdx = iLeft * stride + bpIndices[0U];
	yL_1d = (table[bpIdx + 1U] - table[bpIdx]) * fractions[0U] + table[bpIdx];
	bpIdx += stride;
	return (((table[bpIdx + 1U] - table[bpIdx]) * fractions[0U] + table[bpIdx]) -
		yL_1d) * frac + yL_1d;
}


double CS55GetAccValue(double speed, double torque)
{
	return look2_binlxpw(speed,
		torque, *(double(*)[10])&
		table_reference.x[0], *(double(*)[9])&table_reference.y[0],
		*(double(*)[90])&table_reference.z[0], *(unsigned int(*)[2])&
		table_reference.DLookupTable_maxIndex[0], 10U);
}
