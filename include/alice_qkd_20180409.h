#ifndef ALICE_QKD_H_
#define ALICE_QKD_H

#include "netxpto_20180118.h"
#include "binary_source_20180118.h"
#include "m_qam_mapper_20180118.h"
#include "discrete_to_continuous_time_20180118.h"
#include "pulse_shaper_20180111.h"
#include "clock_20171219.h"
#include "sink_20180118.h"
#include "super_block_interface_20180118.h"
#include "demux_1_2_20180205.h"
#include "message_processor_alice_20180205.h"

class AliceQKD : public SuperBlock {
	/* State Variables */

	// #####################################################################################################
	// ################## Internal Signals Declaration and Inicialization ##################################
	// #####################################################################################################

	//Binary S1{ "NUM_A.sgn" };

	TimeDiscreteAmplitudeDiscreteReal alice_1{ "alice_1.sgn" };


	TimeDiscreteAmplitudeDiscreteReal alice_2{ "alice_2.sgn" };

	TimeContinuousAmplitudeDiscreteReal alice_3{ "alice_3.sgn" };

	TimeContinuousAmplitudeDiscreteReal alice_4{ "alice_4.sgn" };
	TimeContinuousAmplitudeDiscreteReal alice_4_out{ "alice_4_out.sgn" };

	Binary alice_5{ "alice_5.sgn" };

	TimeContinuousAmplitudeContinuousReal clkA_out{ "clkA_out.sgn" };

	Binary alice_12_out{ "alice_12_out.sgn" };

	Binary alice_6{ "alice_6.sgn" };
	Binary alice_6_out{ "alice_6_out.sgn" };

	Binary alice_7{ "alice_7.sgn" };

	Binary alice_8{ "alice_8.sgn" };

	Binary alice_9{ "alice_9.sgn" };

	Binary alice_10{ "alice_10.sgn" };

	Binary alice_11{ "alice_11.sgn" };

	Binary alice_12{ "alice_12.sgn" };

	Binary alice_13{ "alice_13.sgn" };

	Messages C_C_1{ "C_C_1.sgn" };
	Messages C_C_1_out{ "C_C_1_out.sgn" };

	Binary MI_Alice{ "MI_alice.sgn" };


	// #####################################################################################################
	// ########################### Blocks Declaration and Inicialization ###################################
	// #####################################################################################################

	MQamMapper BA1;

	DiscreteToContinuousTime BA2;

	PulseShaper BA3;

	Sink BA4;

	SuperBlockInterface BA5;

	Demux_1_2 BA6;

	Demux_1_2 BA8;

	Demux_1_2 BA9;

	BinarySource BA11;

	Sink BA12;

	MessageProcessorAlice BA7;

	BinarySource BA13;

	Sink BA14;

	Sink BA15;



public:
	/* input parameters*/

	double RateOfPhotons{ 1e3 };
	int StringPhotonsLength{ 12 };

	// Methods
	AliceQKD(vector <Signal*> &inputSignals, vector <Signal*> &outputSignals);

	// Set methods
	void set(int opt);

	void setM(int mValue) { BA1.m = mValue; };
	int const getM(void) { return BA1.m; };

	void setIqAmplitudes(vector<t_iqValues> iqAmplitudesValues) { BA1.m = (t_integer)iqAmplitudesValues.size(); BA1.iqAmplitudes.resize(BA1.m); BA1.iqAmplitudes = iqAmplitudesValues; };
	vector<t_iqValues> const getIqAmplitudes(void) { return BA1.iqAmplitudes; };

	void setNumberOfSamplesPerSymbol(int n) { BA2.setNumberOfSamplesPerSymbol(n); };
	int const getNumberOfSamplesPerSymbol(void) { return BA2.getNumberOfSamplesPerSymbol(); };

	void setRollOffFactor(double rOffFactor) { BA3.setRollOffFactor(rOffFactor); };
	double const getRollOffFactor(void) { return BA3.getRollOffFactor(); };

	void setSeeBeginningOfImpulseResponse(bool sBeginningOfImpulseResponse) { BA3.setSeeBeginningOfImpulseResponse(sBeginningOfImpulseResponse); };
	double const getSeeBeginningOfImpulseResponse(void) { return BA3.getSeeBeginningOfImpulseResponse(); };

	void setPulseShaperFilter(PulseShaperFilter fType) { BA3.setFilterType(fType); };

	void setNumberOfSamples(long int nOfSamples) { BA4.setNumberOfSamples(nOfSamples); BA12.setNumberOfSamples(nOfSamples); BA14.setNumberOfSamples(nOfSamples); BA15.setNumberOfSamples(nOfSamples); };

	void setBitStream(string bitstream) { BA11.setBitStream(bitstream); BA13.setBitStream(bitstream); };

	void setMode(BinarySourceMode m) { BA11.setMode(m); BA13.setMode(m); };

	void setDisplayNumberOfSamples(bool opt) { BA4.setDisplayNumberOfSamples(opt); };

	void setSymbolPeriodMIA(double sPeriod) { BA9.setSymbolPeriod(sPeriod); };

};

#endif
