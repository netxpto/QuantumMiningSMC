# include "netxpto.h"
# include "binary_source.h"
# include "m_qam_mapper.h"	
# include "discrete_to_continuous_time.h"
# include "sink.h"
# include "fork.h"
# include "overlap_save_20180208.h"
# include "pulse_shaper.h"
# include "rf_oscillator.h"
# include "mixer.h"
# include "hilbert_filter_20180306.h"

using namespace std;

int main() {

	// #######################################################################
	// ###################### System Input Parameters ########################
	// #######################################################################

	BinarySourceMode sourceMode{ PseudoRandom };
	int patternLength{ 5 };
	double bitPeriod{ 1.0 / 2.5e9 };
	vector<t_iqValues> iqAmplitudes{ { { 0,0 },{ 1,0 },{ 2,0 },{ 3,0 } } };
	int numberOfBits{ 1000 };		  // -1 will generate long bit sequence.
	int numberOfSamplesPerSymbol{ 16 };
	double rollOffFactor{ 0.3 };
	double BTs{ 0.25 };
	int impulseResponseTimeLength{ 16 };
	int transferFunctionFrequencyLength{ 16 };
	
	// ########################################################################
	// ################ Signals Declaration and Inicialization ################
	// ########################################################################

	Binary S1{ "S1.sgn" };							   // Binary Sequence
	TimeDiscreteAmplitudeDiscreteReal S2{"S2.sgn"};	   // MPAM Signal
	TimeDiscreteAmplitudeDiscreteReal S2b{"S2b.sgn"};  // Not used (Q signal)
	TimeContinuousAmplitudeDiscreteReal S3{"S3.sgn"};  // Discrete to continious time
	TimeContinuousAmplitudeContinuousReal S4{"S4.sgn"};// Pulse Shapping filter
	TimeContinuousAmplitudeContinuousReal S5{"S5.sgn"};// Hilbert filter
	TimeContinuousAmplitudeContinuousReal S6{"S6.sgn"};
	TimeContinuousAmplitudeContinuousReal S7{"S7.sgn"};
	TimeContinuousAmplitudeContinuousReal S8{ "S8.sgn" };
	TimeContinuousAmplitudeContinuousReal S9{ "S9.sgn" };


	// #########################################################################
	// ############### Blocks Declaration and Inicialization ###################
	// #########################################################################

	BinarySource B1{ vector<Signal*> {}, vector<Signal*> { &S1} };
	B1.setMode(sourceMode);
	B1.setPatternLength(patternLength);
	B1.setBitPeriod(bitPeriod);
	B1.setNumberOfBits(numberOfBits);

	MQamMapper B2{ vector<Signal*> { &S1 }, vector<Signal*> { &S2, &S2b } };
	B2.setIqAmplitudes(iqAmplitudes);

	Sink B3{ vector<Signal*> { &S2b }, vector<Signal*> {} };

	DiscreteToContinuousTime B4{ vector<Signal*> { &S2 }, vector<Signal*> { &S3 } };
	B4.setNumberOfSamplesPerSymbol(numberOfSamplesPerSymbol);

	PulseShaper B5{ vector<Signal*> { &S3 }, vector<Signal*> { &S4 } };
	B5.setRollOffFactor( rollOffFactor);
	B5.setImpulseResponseTimeLength(impulseResponseTimeLength);
	B5.setSeeBeginningOfImpulseResponse(false);
	B5.setFilterType(RaisedCosine);

	Fork B6{ vector<Signal*> { &S4 }, vector<Signal*> { &S5, &S6 } };
	

	HilbertTransform B7{ vector<Signal*> { &S6 }, vector<Signal*> { &S7 } };
	B7.setSeeBeginningOfTransferFunction(true);
	B7.setTransferFunctionFrequencyLength(transferFunctionFrequencyLength);

	Sink B8{ vector<Signal*> { &S5 }, vector<Signal*> {} };
	Sink B9{ vector<Signal*> { &S7 }, vector<Signal*> {} };


	// ############################################################################
	// ################ System Declaration and Inicialization #####################
	// ############################################################################

	System MainSystem{ vector<Block*> { &B1, &B2, &B3, &B4, &B5, &B6, &B7, &B8, &B9} };

	// ############################################################################
	// ############################# System Run ###################################
	// ############################################################################

	MainSystem.run();
	cout << "\nExecution Finished, Please hit Enter to exit!";
	getchar();
	return 0;
}
