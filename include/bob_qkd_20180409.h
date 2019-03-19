#ifndef BOB_QKD_H_
#define BOB_QKD_H

#include "netxpto_20180118.h"
#include "binary_mapper_20180205.h"
#include "discrete_to_continuous_time_20180118.h"
#include "pulse_shaper_20180111.h"
#include "super_block_interface_20180118.h"
#include "fork_20180112.h"
#include "sink.h"
#include "clock_20171219.h"
#include "bobBB84_20180221.h"
#include "message_processor_bob_20180221.h"
#include "demux_1_2_20180205.h"


class BobQKD : public SuperBlock {

	// #####################################################################################################
	// ################## Internal Signals Declaration and Inicialization ##################################
	// #####################################################################################################
	TimeDiscreteAmplitudeDiscreteReal Bob_1{ "Bob_1.sgn" };
	TimeContinuousAmplitudeDiscreteReal Bob_2{ "Bob_2.sgn" };
	TimeContinuousAmplitudeDiscreteReal Bob_3{ "Bob_3.sgn" };
	TimeContinuousAmplitudeContinuousReal Bob_12{ "Bob_12.sgn" };
	TimeContinuousAmplitudeContinuousReal Bob_13{"Bob_13.sgn"};

	TimeDiscreteAmplitudeDiscreteReal Bob_7{ "Bob_7.sgn" };
	TimeContinuousAmplitudeDiscreteReal Bob_4{ "Bob_4.sgn" };
	Binary Bob_5{ "Bob_5.sgn" };
	Binary Bob_6{ "Bob_6.sgn" };
	Binary Bob_9{ "Bob_9.sgn" };
	TimeDiscreteAmplitudeDiscreteReal Bob_10{ "Bob_10.sgn" };
	TimeDiscreteAmplitudeDiscreteReal Bob_11{ "Bob_11.sgn" };

	TimeContinuousAmplitudeDiscreteReal Bob_8{ "Bob_8.sgn" };

	TimeContinuousAmplitudeDiscreteReal CLKB_out1{ "CLKB_out1.sgn" };
	TimeContinuousAmplitudeDiscreteReal CLKB_out2{ "CLKB_out2.sgn" };

	Messages C_C_2{ "C_C_2.sgn" };
	Messages C_C_1{ "C_C_1.sgn" };

	Binary Bob_6_out{ "Bob_6_out.sgn" };



	// #####################################################################################################
	// ########################### Blocks Declaration and Inicialization ###################################
	// #####################################################################################################

	BinaryMapper BB0;
	DiscreteToContinuousTime BB1;
	PulseShaper BB2;

	PulseShaper BB3;
	Fork BB4;

	SuperBlockInterface BB9;

	bobBB84 BB5;

	MessageProcessorBob BB7;

	Demux_1_2 BB6;

	Fork BB10;
	

	Sink BBSink;
	Sink BBSink0;
	Sink BBSink1;


public:

	BobQKD(vector <Signal*> &inputSignals, vector <Signal*> &outputSignals);

	void setNumberOfSamplesPerSymbol(int n) { BB1.setNumberOfSamplesPerSymbol(n); };
	int const getNumberOfSamplesPerSymbol(void) { return BB1.getNumberOfSamplesPerSymbol(); };

	void setSeeBeginningOfImpulseResponse(bool sBeginningOfImpulseResponse) { BB2.setSeeBeginningOfImpulseResponse(sBeginningOfImpulseResponse); };
	double const getSeeBeginningOfImpulseResponse(void) { return BB2.getSeeBeginningOfImpulseResponse(); };

	void setPulseShaperFilter(PulseShaperFilter fType) { BB2.setFilterType(fType); BB3.setFilterType(fType); };

	void setPulseWidth(double tWidth) { BB3.setPulseWidth(tWidth); };
	double getPulseWidth() { return BB3.getPulseWidth(); };

	void setPulseDelay(double pDelay) { BB3.setPulseDelay(pDelay); };
	double getPulseDelay() { return BB3.getPulseDelay(); };


};

#endif
