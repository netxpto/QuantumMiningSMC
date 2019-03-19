#ifndef SINGLE_PHOTON_RECEIVER_H
#define SINGLE_PHOTON_RECEIVER_H

#include "netxpto_20180418.h"
#include "pulse_shaper_20180111.h"
#include "sampler_20171119.h"
#include "coincidence_detector_20180206.h"
#include "super_block_interface_20180118.h"
#include "clock_20171219.h"
#include "bit_decider_20170818.h"

class SinglePhotonReceiver : public SuperBlock{ 

	// #####################################################################################################
	// ################## Internal Signals Declaration and Inicialization ##################################
	// #####################################################################################################

	TimeContinuousAmplitudeContinuousReal SPR01{ "SPR01.sgn" }; // single photon detector output
	TimeContinuousAmplitudeContinuousReal SPR02{ "SPR02.sgn" }; //shapper
	TimeContinuousAmplitudeContinuousReal SPR03{ "SPR03.sgn" }; // single photon detector output
	TimeContinuousAmplitudeContinuousReal SPR04{ "SPR04.sgn" }; //clock
	TimeContinuousAmplitudeContinuousReal SPR05{ "SPR05.sgn" }; //sampler
	TimeContinuousAmplitudeContinuousReal SPR06{ "SPR06.sgn" }; //sampler
	Binary SPR07{ "SPR07.sgn" };
	Binary SPR08{ "SPR08.sgn" };
	TimeDiscreteAmplitudeDiscreteReal SPR09{ "SPR09.sgn" };
	TimeDiscreteAmplitudeDiscreteReal SPR09_out{ "SPR09_out.sgn" };

	// #####################################################################################################
	// ########################### Blocks Declaration and Inicialization ###################################
	// #####################################################################################################
	PulseShaper B1;
	PulseShaper B2;
	Clock B3;
	Clock B4;
	Sampler B5;
	Sampler B6;
	BitDecider B7;
	BitDecider B8;
	CoincidenceDetector B9;
	SuperBlockInterface B10;

	

public:

	/* Input Parameters */
	int samplesToSkip{ 0 };
	PulseShaperFilter filterType{ Square };

	/*Methods*/
	SinglePhotonReceiver(vector <Signal*> &inputSignals, vector <Signal*> &outputSignals);

	/*Set Methods*/
	void setPulseShaperFilter(PulseShaperFilter fType) { B1.setFilterType(fType); B2.setFilterType(fType); };
	void setPulseShaperWidth(double pulseW) { B1.setPulseWidth(pulseW); B2.setPulseWidth(pulseW); };
	void setClockBitPeriod(double period) { B3.setClockPeriod(period); B4.setClockPeriod(period); };
	void setClockPhase(double phase) { B3.setClockPhase(phase); B4.setClockPhase(phase); };
	void setClockSamplingPeriod(double sPeriod) { B3.setSamplingPeriod(sPeriod); B4.setSamplingPeriod(sPeriod); };
	void setThreshold(double threshold) { B7.setDecisionLevel(threshold); B8.setDecisionLevel(threshold); };
};

#endif
