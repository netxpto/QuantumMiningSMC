// Photodiode noise test.
// Testing noise for constant and variable power input.

#include "netxpto_20180118.h"

#include "local_oscillator_20180118.h"
#include "photoelectron_generator_20180302.h"
#include "snr_photoelectron_generator_20180309.h"
#include "sink_20180118.h"

#include "m_qam_transmitter_20180118.h"


int main(int argc, char* argv[])
{	
	
	// #########################################################################
	// ######################## System Input Parameters ########################
	// #########################################################################

	// Simulation settings
	int numberOfSnrSamples = 100;
	

	// Signal setting
	int bufferLength = 512;
	int samplesPerSymbol(10);
	double bitPeriod = 1.0E-9;

	// Oscillator settings
	double wavelength = 1.55e-6;
	double samplePeriod = bitPeriod / samplesPerSymbol;  // Talvez isto não seja preciso
	double powerUnit = PLANCK_CONSTANT*SPEED_OF_LIGHT / (samplePeriod*wavelength);


	// Local oscillator parameters
	double photonNumber1         = 100;
	double localOscillatorPower = powerUnit * photonNumber1;
	double localOscillatorPhase = 0;
	double SNR = 0;

	// Power override
	//localOscillatorPower = 1e-9; // Watts
	//localOscillatorPower = atof(argv[1]);


	// Photodiode parameters
	double photodiodeEfficiency = 1;
	bool   photodiodeShotNoise  = true;


	// SNR parameters
	double snrTWindow = 1e-8;
	//double snrTWindow = 0.0;
	

	// Time Window override
	//snrTWindow = atof(argv[2]);


	// #########################################################################
	// ################ Signals Declaration and Inicialization #################
	// #########################################################################

	// Sinal óptico 
	OpticalSignal S0("S0.sgn");
	S0.setBufferLength(bufferLength);

	// Photoelectron Generator electric output signal
	TimeDiscreteAmplitudeContinuousReal S1("S1.sgn");
	S1.setBufferLength(bufferLength);
	

	// The simulation ends when the final buffer (input of the sink) is full.
	// When a value is written in the buffer, it is also written in the output
	// file. Therefore, we will manipulate the bufferLength of this final signal
	// to force a given number of output SNR samples.

	// SNR Phoelectron Generator electric output signal
	TimeDiscreteAmplitudeContinuousReal S2("S2.sgn");
	S2.setBufferLength(numberOfSnrSamples);




	// #########################################################################
	// ################# Blocks Declaration and Inicialization #################
	// #########################################################################

	// Detection of a constant signal

	LocalOscillator B0{ vector<Signal*> {}, vector<Signal*> {&S0} };
	B0.setOpticalPower(localOscillatorPower);
	B0.setPhase(localOscillatorPhase);
	B0.setSamplingPeriod(samplePeriod);
	B0.setSymbolPeriod(bitPeriod);
	B0.setSignaltoNoiseRatio(SNR);

	PhotoelectronGenerator B1{ vector<Signal*> {&S0}, vector<Signal*> {&S1} };
	B1.setShotNoise(photodiodeShotNoise);
	B1.setEfficiency(photodiodeEfficiency);


	SnrPhotoelectronGenerator B2 { vector<Signal*> {&S1}, vector<Signal*> {&S2} };
	B2.setTWindow(snrTWindow);
	

	Sink B3{ vector<Signal*> {&S2}, vector<Signal*> {} };

	// The number of samples is 0, because the number of output SNR samples is
	// given by the size of the input buffer to this sink.
	// This is not a good design.
	B3.setNumberOfSamples(0);
	B3.setDisplayNumberOfSamples(true);


	

	// #########################################################################
	// ################# System Declaration and Inicialization #################
	// #########################################################################

	System MainSystem{ vector<Block*> {&B0, &B1, &B2, &B3} };
	//System MainSystem{ vector<Block*> {&B0, &B1} };




	// #########################################################################
	// ############################## System Run ###############################
	// #########################################################################

	MainSystem.run();



    return 0;
}
