# include "netxpto.h"

# include "sink.h"
# include "balanced_beam_splitter.h"
# include "subtractor.h"
# include "photodiode.h"
//# include "TIamplifier.h"
# include "local_oscillator.h"
/*# include "discretizer.h"
# include "delayer.h"
# include "bit_decider.h"
# include "bit_error_rate.h"
# include "testblock.h"
# include "discarder.h"*/
# include "m_qam_transmitter.h"
# include "phaseShifter.h"

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

	TimeDiscreteAmplitudeContinuousReal S11{ "S11.sgn" };

	TimeDiscreteAmplitudeContinuousReal S12{ "S12.sgn" };

	TimeDiscreteAmplitudeContinuousReal S13{ "S13.sgn" };

	TimeDiscreteAmplitudeContinuousReal S14{ "S14.sgn" };

	TimeDiscreteAmplitudeContinuousReal S15{ "S15.sgn" };

	TimeDiscreteAmplitudeContinuousReal S16{ "S16.sgn" };

	TimeDiscreteAmplitudeContinuousReal S17{ "S17.sgn" };

	//Binary S11{ "S11.sgn" };

	//Binary S07{ "S07.sgn" };


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

	BalancedBeamSplitter B3{ vector<Signal*> { &S01 }, vector<Signal*> { &S03, &S04 } };
	t_complex unit1 = 1;
	unit1 = 1 / sqrt(2) * unit1;
	B3.setTransferMatrix({ { unit1, unit1, unit1, -unit1 } });
	B3.setMode(0);

	BalancedBeamSplitter B4{ vector<Signal*> { &S02 }, vector<Signal*> { &S05, &S06 } };
	t_complex unit2 = 1;
	unit2 = 1 / sqrt(2) * unit2;
	B4.setTransferMatrix({ { unit2, unit2, unit2, -unit2 } });
	B4.setMode(0);

	phaseShifter B5{ vector<Signal*> { &S06 }, vector<Signal*> { &S07 } };

	BalancedBeamSplitter B6{ vector<Signal*> { &S03, &S05 }, vector<Signal*> { &S08, &S09 } };
	t_complex unit3 = 1;
	unit3 = 1 / sqrt(2) * unit3;
	B6.setTransferMatrix({ { unit3, unit3, unit3, -unit3 } });
	B6.setMode(1);

	BalancedBeamSplitter B7{ vector<Signal*> { &S04, &S07 }, vector<Signal*> { &S10, &S11 } };
	t_complex unit = 1;
	unit = 1 / sqrt(2) * unit;
	B7.setTransferMatrix({ { unit, unit, unit, -unit } });
	B7.setMode(1);

	Photodiode B8{ vector<Signal*> { &S07, &S08, &S09, &S10 }, vector<Signal*> { &S12, &S13, &S14, &S15 } };
	B8.setResponsivity(1);

	Subtractor B9{ vector<Signal*> { &S12, &S13 }, vector<Signal*> { &S16 } };

	Subtractor B10{ vector<Signal*> { &S14, &S15 }, vector<Signal*> { &S17 } };

	//TIAmplifier B6{ vector<Signal*> { &S07 }, vector<Signal*> { &S08 } };
	//B6.setAmplification(1e6);
	//B6.setNoiseAmplitude(13.593313775018258);

	//Discretizer B7{ vector<Signal*> { &S08 }, vector<Signal*> { &S09 } };
	//B7.setSamplingRate(SamplesPerSymbol);

	//Delayer B8{ vector<Signal*> { &S09 }, vector<Signal*> { &S10 } };
	//B8.setDelay(9);

	//BitDecider B9{ vector<Signal*> { &S10 }, vector<Signal*> { &S11 } };
	//B9.setReferenceValue(0);

	//BitErrorRate B10{ vector<Signal*> { &S11, &MQAM0 }, vector<Signal*> { &S12 } };
	//B10.setZ(1.96);

	//Sink B9{ vector<Signal*> { &S07 }, vector<Signal*> {} };
	//B9.setNumberOfSamples(50000);
	//B9.setDisplayNumberOfSamples(false);

	// #####################################################################################################
	// ########################### System Declaration and Inicialization ###################################
	// #####################################################################################################

	System MainSystem{ vector<Block*> { &B1, &B2, &B3, &B4, &B5, &B6, &B7, &B8, &B9, &B10} };

	// #####################################################################################################
	// #################################### System Run #####################################################
	// #####################################################################################################

	MainSystem.run();

	return 0;

}