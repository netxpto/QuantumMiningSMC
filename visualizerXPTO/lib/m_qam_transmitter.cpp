# include "m_qam_transmitter.h"


void MQamTransmitter::set(int opt) {

	// Basic Configuration
 
	if (opt==0) {
		setMode(PseudoRandom);
		setBitPeriod(1.0 / 50e9);
		setPatternLength(5);
		setNumberOfBits(10000);
		setNumberOfSamplesPerSymbol(32);
		setRollOffFactor(0.9);
		setIqAmplitudes({ { 1, 1 }, { -1, 1 }, { -1, -1 }, { 1, -1 } });
		setOutputOpticalPower_dBm(0);
		setSaveInternalSignals(true);
	}

	return;
}