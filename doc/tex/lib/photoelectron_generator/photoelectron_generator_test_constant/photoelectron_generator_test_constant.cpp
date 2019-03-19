// Photodiode noise test.
// Testing noise for constant and variable power input.

#include "netxpto_20180118.h"

#include "local_oscillator_20180118.h"
#include "photoelectron_generator_20180302.h"
#include "sink_20180118.h"

//#include "m_qam_transmitter_20180118.h"


int main()
{

	// #########################################################################
	// ######################## System Input Parameters ########################
	// #########################################################################

	// Simulation settings
	int numberOfBitsGenerated(1);
	int bufferLength = 512;

	// Signal setting
	int samplesPerSymbol(50);
	double bitPeriod = 1.0E-9;

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

	// Photodiode parameters
	double photodiodeEfficiency = 1.0;
	bool   photodiodeShotNoise  = true;




	// #########################################################################
	// ################ Signals Declaration and Inicialization #################
	// #########################################################################

	// Sinal óptico 
	OpticalSignal S0("S0.sgn");
	S0.setBufferLength(bufferLength);

	// Photoelectron Generator electric output signal
	// To force a certain number of output samples, this signal's bufferLength
	// is equal to that same number.
	TimeDiscreteAmplitudeContinuousReal S1("S1.sgn");
	S1.setBufferLength(samplesPerSymbol*numberOfBitsGenerated);




	// #########################################################################
	// ################# Blocks Declaration and Inicialization #################
	// #########################################################################

	// Detection of a constant signal

	LocalOscillator B0{ vector<Signal*> {}, vector<Signal*> {&S0} };
	B0.setOpticalPower(localOscillatorPower1);
	B0.setPhase(localOscillatorPhase);
	B0.setSamplingPeriod(samplePeriod);
	B0.setSymbolPeriod(bitPeriod);
	B0.setSignaltoNoiseRatio(SNR);

	PhotoelectronGenerator B1{ vector<Signal*> {&S0}, vector<Signal*> {&S1} };
	B1.setShotNoise(photodiodeShotNoise);
	B1.setEfficiency(photodiodeEfficiency);


	Sink B2{ vector<Signal*> {&S1}, vector<Signal*> {} };
	B2.setNumberOfSamples(samplesPerSymbol*numberOfBitsGenerated);
	B2.setDisplayNumberOfSamples(true);
	



	// #########################################################################
	// ################# System Declaration and Inicialization #################
	// #########################################################################

	// Simulation of the continuous setup
	System MainSystem{ vector<Block*> {&B0, &B1, &B2} };




	// #########################################################################
	// ############################## System Run ###############################
	// #########################################################################

	MainSystem.run();



    return 0;
}
