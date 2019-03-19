// Photodiode noise test.
// Testing noise for constant and variable power input.

#include "netxpto_20180118.h"

#include "local_oscillator_20180118.h"
#include "photoelectron_generator_20180302.h"
#include "sink_20180118.h"

#include "m_qam_transmitter_20180118.h"


int main()
{

	// #########################################################################
	// ######################## System Input Parameters ########################
	// #########################################################################

	// Simulation settings
	int numberOfBitsGenerated(30);
	int bufferLength = 512;

	// Signal setting
	int samplesPerSymbol(10);
	double bitPeriod = 100e-12;

	// Oscillator settings
	double wavelength = 1.55e-6;
	double samplePeriod = bitPeriod / samplesPerSymbol;  // Talvez isto não seja preciso
	double powerUnit = PLANCK_CONSTANT*SPEED_OF_LIGHT / (samplePeriod*wavelength);


	// Local oscillator parameters
	double photonNumber1         = 0.1;
	double localOscillatorPower1 = powerUnit * photonNumber1;

	// Power override
	localOscillatorPower1 = 1e-9; // Watts

	double localOscillatorPhase  = 0;
	double SNR = 0;

	// Photoelectron Generator parameters
	double photoelectronGeneratorEfficiency = 1.0;
	bool   photoelectronGeneratorShotNoise  = true;

	// MQAM parameters
	double photonNumber2         = 0.5;
	double localOscillatorPower2 = powerUnit * photonNumber2;
	vector<t_iqValues> iqAmplitudeValues = { {0,0}, {0.25,0}, {0.5,0}, {1.0,0} };




	// #########################################################################
	// ################ Signals Declaration and Inicialization #################
	// #########################################################################

	// MQAM output optical signal
	OpticalSignal S0("S0.sgn");
	S0.setBufferLength(bufferLength);

	// Photoelectron output electric signal
	TimeDiscreteAmplitudeContinuousReal S1("S1.sgn");
	S1.setBufferLength(bufferLength);




	// #########################################################################
	// ################# Blocks Declaration and Inicialization #################
	// #########################################################################

	// Detection of a variable signal

	// MQAM
	MQamTransmitter B0{ vector<Signal*> {}, vector<Signal*> {&S0} };
	B0.setNumberOfBits(numberOfBitsGenerated);
	B0.setOutputOpticalPower(localOscillatorPower2);
	B0.setMode(DeterministicCyclic);
	B0.setBitStream("1111010110100100");

	B0.setBitPeriod(bitPeriod);
	B0.setIqAmplitudes(iqAmplitudeValues);
	B0.setRollOffFactor(0.45);
	B0.setPulseShaperFilter(RaisedCosine);
	
	B0.setNumberOfSamplesPerSymbol(samplesPerSymbol);
	B0.setSaveInternalSignals(true);
	B0.setSeeBeginningOfImpulseResponse(true);


	// Photoelectron Generator (Photodiode)
	PhotoelectronGenerator B1{ vector<Signal*> {&S0}, vector<Signal*> {&S1} };
	B1.setShotNoise(photoelectronGeneratorShotNoise);
	B1.setEfficiency(photoelectronGeneratorEfficiency);


	// Photoelectron generator sink
	Sink B2{ vector<Signal*> {&S1}, vector<Signal*> {} };
	B2.setNumberOfSamples(samplesPerSymbol*numberOfBitsGenerated);
	B2.setDisplayNumberOfSamples(true);




	// #########################################################################
	// ################# System Declaration and Inicialization #################
	// #########################################################################

	System MainSystem{ vector<Block*> {&B0, &B1, &B2} };




	// #########################################################################
	// ############################## System Run ###############################
	// #########################################################################

	MainSystem.run();




    return 0;
}
