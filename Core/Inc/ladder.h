#ifndef ladder_H
#define ladder_H

#ifdef __cplusplus
extern "C" {
#endif
  
#include "PLC_timer_api.h"

#define len(arr) ((int) (sizeof (arr) / sizeof (arr)[0]))
#define TotalRungs 3
#define T0 0
#define T1 1
#define TimCount 2
#define TotalElements 10


// Rung properties
struct ST_PLC_RungStructure {
	int elems;
	int nodes;
	int branches;
	int output;
	int rungIndex;
	int leftBranches;
};


// Ladder element type
enum ST_PLC_ElementType {digitalInput,
                   digitalOutput,
                   timerStatus,
                   outputStatus,
                   timerOnDelay,
                   timerOffDelay,
                   timerRTO,
                   timerEnable,
                   timerDone,
                   XIO,
                   XIC};


void ST_PLC_InitializeLadder(void);
void ST_PLC_StartExecution(void);
void ST_PLC_ExecuteLadder(void);

#ifdef __cplusplus
}
#endif

#endif