# include "netxpto.h"

# include "sink.h"
# include "BalancedBeamSplitter.h"
# include "subtractor.h"
# include "photodiode.h"
# include "TIamplifier.h"
# include "local_oscillator.h"
# include "discretizer.h"
# include "delayer.h"
# include "bit_decider.h"
# include "bit_error_rate.h"
# include "testblock.h"
# include "discarder.h"
# include "m_qam_transmitter.h"

# include "binary_source.h"


#define _USE_MATH_DEFINES // for C
#include <math.h>

int main() {

	// #####################################################################################################
	// ########################### Signals Declaration and Inicialization ##################################
	// #####################################################################################################

	Binary MQAM0{ "MQAM0.sgn" };

	OpticalSignal S00("S00.sgn");

	OpticalSignal S01("S01.sgn");

	OpticalSignal S02("S02.sgn");

	OpticalSignal S03("S03.sgn");

	OpticalSignal S04("S04.sgn");

	TimeContinuousAmplitudeContinuousReal S05{ "S05.sgn" };

	TimeContinuousAmplitudeContinuousReal S06{ "S06.sgn" };

	TimeContinuousAmplitudeContinuousReal S07{ "S07.sgn" };

	TimeContinuousAmplitudeContinuousReal S08{ "S08.sgn" };

	TimeDiscreteAmplitudeContinuousReal S09{ "S09.sgn" };

	TimeDiscreteAmplitudeContinuousReal S10{ "S10.sgn" };

	Binary S11{ "S11.sgn" };

	Binary S12{ "S12.sgn" };


	// #####################################################################################################
	// ########################### Blocks Declaration and Inicialization ###################################
	// #####################################################################################################

	int NumberOfBits(100);
	NumberOfBits = NumberOfBits + 8;
	int SamplesPerSymbol(16);

	//discarder B000{ vector<Signal*> { &MQAM0 }, vector<Signal*> { &MQAM00 } };

	MQamTransmitter B1{ vector<Signal*> { }, vector<Signal*> { &S00, &MQAM0 } };
	B1.setOutputOpticalPower_dBm(-20);
	B1.setMode(PseudoRandom);
	B1.setBitPeriod(1.0 / 50e9);
	B1.setPatternLength(5);
	B1.setNumberOfBits(NumberOfBits);
	B1.setIqAmplitudes({ { 1, 0 },{ -1, 0 } });
	B1.setNumberOfSamplesPerSymbol(SamplesPerSymbol);
	B1.setRollOffFactor(0.3);
	B1.setSaveInternalSignals(false);

	LocalOscillator B2{ vector<Signal*> { &S00 }, vector<Signal*> { &S01, &S02 } };
	B2.setLocalOscillatorOpticalPower_dBm(-10);
	B2.setLocalOscillatorPhase(0.0);

	BalancedBeamSplitter B3{ vector<Signal*> { &S01, &S02 }, vector<Signal*> { &S03,&S04 } };
	t_complex unit = 1;
	unit = 1 / sqrt(2) * unit;
	B3.setTransferMatrix({ { unit, unit, unit, -unit } });

	Photodiode B4{ vector<Signal*> { &S03,&S04 }, vector<Signal*> { &S05, &S06 } };
	B4.setResponsivity(1);

	Subtractor B5{ vector<Signal*> { &S05, &S06 }, vector<Signal*> { &S07 } };

	TIAmplifier B6{ vector<Signal*> { &S07 }, vector<Signal*> { &S08 } };
	B6.setAmplification(1e6);
	B6.setNoiseAmplitude(13.593313775018258);

	Discretizer B7{ vector<Signal*> { &S08 }, vector<Signal*> { &S09 } };
	B7.setSamplingRate(SamplesPerSymbol);

	Delayer B8{ vector<Signal*> { &S09 }, vector<Signal*> { &S10 } };
	B8.setDelay(9);

	BitDecider B9{ vector<Signal*> { &S10 }, vector<Signal*> { &S11 } };
	B9.setReferenceValue(0);

	BitErrorRate B10{ vector<Signal*> { &S11, &MQAM0 }, vector<Signal*> { &S12 } };
	//B10.setZ(1.96);

	Sink B11{ vector<Signal*> { &S12 }, vector<Signal*> {} };
	B11.setNumberOfSamples(50000);
	B11.setDisplayNumberOfSamples(false);

	// #####################################################################################################
	// ########################### System Declaration and Inicialization ###################################
	// #####################################################################################################

	System MainSystem{ vector<Block*> { &B1, &B2, &B3, &B4, &B5, &B6, &B7, &B8, &B9, &B10, &B11} };

	// #####################################################################################################
	// #################################### System Run #####################################################
	// #####################################################################################################

	MainSystem.run();

	return 0;

}