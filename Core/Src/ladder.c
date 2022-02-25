#include "main.h"
#include "STEVALPLC.h"
#include "ladder.h"

// To check if already initialized
uint8_t alreadyInit = 0;
ST_PLC_TimerHandle_t timers[TimCount];
struct ST_PLC_RungStructure rungDatabase[TotalRungs];

/*
int ST_PLC_Ladder[9][7] = {{0,	0,	1,	digitalInput,		DI_Channel_I0_0, 	        0,	        XIC},
                           {0,	1,	2,	digitalInput,		DI_Channel_I0_1, 	        0,      	XIC},
                           {0,	0,	2,	digitalInput,		DI_Channel_I0_2, 	        0,	        XIC},
                           {0,	2,	3,	digitalInput,		DI_Channel_I0_3, 	        0,	        XIC},
		   	   {0,	3,	4,	digitalOutput,	        DO_Channel_Q0_0, 	        0,              0},
                           {1,	0,	1,	outputStatus,		DO_Channel_Q0_0,                0,	        XIC},
                           {1,	1,	2,	timerOnDelay,	        T0, 	                        2000,           0},
                           {1,	0,	1,	outputStatus,	        DO_Channel_Q0_0, 	                        0,	XIC},
                           {1,	1,	2,	digitalOutput,	        DO_Channel_Q0_7, 	        0,		0}};

*/
// LED blinking -

int ST_PLC_Ladder[6][7] = {{0,	0,	1,	timerStatus,		T1, 	timerDone,	XIO},
		   	   	   	   	   {0,	1,	2,	timerOnDelay,	        T0, 	                1000,		0},
						   {1,	0,	1,	timerStatus,		T0, 	                timerDone,	XIC},
						   {1,	1,	2,	timerOnDelay,	        T1, 	                1000,		0},
						   {2,	0,	1,	timerStatus,	        T0, 	                timerDone,	XIC},
						   {2,	1,	2,	digitalOutput,	        DO_Channel_Q0_7, 	0,	        0}};




/* @brief: Initializes number of rung nodes, elements and branches
 * */
void ST_PLC_InitializeLadder(){
	// Iteration variables
	int nRung;
        int nElem;
	int currentRung = 0;
	int oldRung = ST_PLC_Ladder[0][0];
	int rungIndex = 0;

	/* Initialize rung properties */
	for (nRung = 0; nRung<TotalRungs; nRung++){
		rungDatabase[nRung].elems = 0;
		rungDatabase[nRung].nodes = 0;
		rungDatabase[nRung].branches = 0;
		rungDatabase[nRung].rungIndex = 0;
		rungDatabase[nRung].leftBranches = 0;
	}

	/* Count number of elements in each rung */
	for (nElem = 0; nElem<len(ST_PLC_Ladder); nElem++){
		if(ST_PLC_Ladder[nElem][0] != oldRung){
			oldRung = ST_PLC_Ladder[nElem][0];
			currentRung++;
			rungDatabase[currentRung].rungIndex = nElem;
		}

		rungDatabase[currentRung].elems++;
	}

	/* Count nodes in each rung */
	for (nRung =0; nRung<TotalRungs; nRung++){

		// Find number of nodes excluding node of origin
		for(nElem=0; nElem<rungDatabase[nRung].elems; nElem++){
			rungIndex = rungDatabase[nRung].rungIndex;

			// check the number of node
			if(ST_PLC_Ladder[rungIndex+nElem][1] > rungDatabase[nRung].nodes){
				rungDatabase[nRung].nodes = ST_PLC_Ladder[rungIndex+nElem][1];
			} else if(ST_PLC_Ladder[rungIndex+nElem][2] > rungDatabase[nRung].nodes){
				rungDatabase[nRung].nodes = ST_PLC_Ladder[rungIndex+nElem][2];
			}

			// Count number of left elements
			if(ST_PLC_Ladder[rungIndex+nElem][3] == digitalInput ||
			   ST_PLC_Ladder[rungIndex+nElem][3] == timerStatus ||
			   ST_PLC_Ladder[rungIndex+nElem][3] == outputStatus){
					rungDatabase[nRung].leftBranches++;
			}

			// Count number of left elements
			switch(ST_PLC_Ladder[rungIndex+nElem][3]){
				case timerOnDelay:
					timers[ST_PLC_Ladder[rungIndex+nElem][4]] = ST_PLC_CreateTimer(ST_PLC_Ladder[rungIndex+nElem][5],
														ST_PLC_OnDelayTimer, NULL);
					break;
				case timerOffDelay:
					timers[ST_PLC_Ladder[rungIndex+nElem][4]] = ST_PLC_CreateTimer(ST_PLC_Ladder[rungIndex+nElem][5],
														ST_PLC_OffDelayTimer, NULL);
					break;
				case timerRTO:
					timers[ST_PLC_Ladder[rungIndex+nElem][4]] = ST_PLC_CreateTimer(ST_PLC_Ladder[rungIndex+nElem][5],
														ST_PLC_RTOTimer, NULL);
					break;

			}
		}

		// Excluding origin, branches are equal to nodes
		rungDatabase[nRung].branches = rungDatabase[nRung].nodes;
	}
}




/* @brief: Executes rungs
 * */
void ST_PLC_StartExecution(){
	// iteration variables
	int nRung;
	int nElem;
	int nBranch;
	int rungIndex;
	int outRungIndex;
	int rightBranches;
	bool IOStatusBit;
        ST_PLC_Blink_One_DebugLed(DebugLed2,100,2); /*D36 -Blue*/
	// iterate all rungs
	for(nRung=0; nRung<TotalRungs; nRung++){
		// get start rungIndex for current rung in ladder netlist
		rungIndex = rungDatabase[nRung].rungIndex;

		// Empty array to store the path closure denoting rung is open
		int closedPath[10];
		for(nBranch=0; nBranch<rungDatabase[nRung].leftBranches; nBranch++){
			closedPath[nBranch] = 0;
		}

		// Check if there is a closed path in rung
		for(nElem=0; nElem<rungDatabase[nRung].elems; nElem++){

			// iterate for all branches
			for(nBranch=0; nBranch<rungDatabase[nRung].leftBranches; nBranch++){

				// check nodes
				if(nBranch >= ST_PLC_Ladder[rungIndex+nElem][1] && nBranch < ST_PLC_Ladder[rungIndex+nElem][2]){
					// check contactor status
					switch (ST_PLC_Ladder[rungIndex+nElem][3]){
						case digitalInput:
                                                  ST_PLC_GetOne_DI(ST_PLC_Ladder[rungIndex+nElem][4],&IOStatusBit);
                                                  IOStatusBit = ST_PLC_Ladder[rungIndex+nElem][6] == XIC ?
                                                  IOStatusBit: !IOStatusBit;
                                                  closedPath[nBranch] = closedPath[nBranch] || IOStatusBit;
                                                  break;

						case outputStatus:
                                                  ST_PLC_GetOne_DO_Status(ST_PLC_Ladder[rungIndex+nElem][4],&IOStatusBit);
                                                  IOStatusBit = ST_PLC_Ladder[rungIndex+nElem][6] == XIC ?
                                                  IOStatusBit: !IOStatusBit;
                                                  closedPath[nBranch] = closedPath[nBranch] || IOStatusBit;
                                                  break;

						case timerStatus:
							if(ST_PLC_Ladder[rungIndex+nElem][5] == timerDone){
								IOStatusBit = ST_PLC_Ladder[rungIndex+nElem][6] == XIC ?
										timers[ST_PLC_Ladder[rungIndex+nElem][4]]->done:
										!timers[ST_PLC_Ladder[rungIndex+nElem][4]]->done;

								closedPath[nBranch] = closedPath[nBranch] || IOStatusBit;

							} else if(ST_PLC_Ladder[rungIndex+nElem][5] == timerEnable){
								IOStatusBit = ST_PLC_Ladder[rungIndex+nElem][6] == XIC ?
										timers[ST_PLC_Ladder[rungIndex+nElem][4]]->enable:
										!timers[ST_PLC_Ladder[rungIndex+nElem][4]]->enable;
								closedPath[nBranch] = closedPath[nBranch] || IOStatusBit;
							}
							break;

						default:
							closedPath[nBranch] = 1;
							break;
					}
				}
			}

			// iterate for all input elements
			rungDatabase[nRung].output = 1;
			for(nBranch=0; nBranch<rungDatabase[nRung].leftBranches; nBranch++){
				rungDatabase[nRung].output &= closedPath[nBranch];
			}

			// iterate for all output elements
			rightBranches = rungDatabase[nRung].branches - rungDatabase[nRung].leftBranches;
			for(nBranch=0; nBranch<rightBranches; nBranch++){
				outRungIndex = rungIndex + rungDatabase[nRung].leftBranches;

				// set the output status based on rung output
				switch (ST_PLC_Ladder[outRungIndex+nBranch][3]){
					case digitalOutput:
						if (rungDatabase[nRung].output){
							ST_PLC_SetOne_DO( ST_PLC_Ladder[outRungIndex+nBranch][4]);
						} else {
							ST_PLC_ClrOne_DO(ST_PLC_Ladder[outRungIndex+nBranch][4]);
						}
						break;

					case timerOnDelay:
						if (rungDatabase[nRung].output)
							ST_PLC_EnableTimer(timers[ST_PLC_Ladder[outRungIndex+nBranch][4]]);
						else
							ST_PLC_DisableTimer(timers[ST_PLC_Ladder[outRungIndex+nBranch][4]]);
						break;

					case timerOffDelay:
						if (rungDatabase[nRung].output)
							ST_PLC_EnableTimer(timers[ST_PLC_Ladder[outRungIndex+nBranch][4]]);
						else
							ST_PLC_DisableTimer(timers[ST_PLC_Ladder[outRungIndex+nBranch][4]]);
						break;

					case timerRTO:
						if (rungDatabase[nRung].output)
							ST_PLC_EnableTimer(timers[ST_PLC_Ladder[outRungIndex+nBranch][4]]);
						else
							ST_PLC_DisableTimer(timers[ST_PLC_Ladder[outRungIndex+nBranch][4]]);
						break;
				}
			}
		}
	}

}

void ST_PLC_ExecuteLadder(){
	// Initialize rungs if not already initialized
	if(!alreadyInit){
		ST_PLC_InitializeLadder();
		alreadyInit = 1;
	}
	ST_PLC_StartExecution();
}