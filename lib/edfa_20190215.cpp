# include "edfa_20190215.h"

//using namespace std;

Edfa::Edfa(vector<Signal *> &inputSignal, vector<Signal *> &outputSignal) :SuperBlock(inputSignal, outputSignal) {


	B1.initializeBlock(vector<Signal*> { inputSignals[0] }, vector<Signal*> { &EDFA1 });          // Amplifier
	B2.initializeBlock(vector<Signal*> { }, vector<Signal*> { &EDFA2 });                          // White Noise
	B3.initializeBlock(vector<Signal*> { &EDFA1, &EDFA2 }, vector<Signal*> { outputSignals[0] }); // Add

	B1.setGain(sqrt(pow(10, (powerGain_dB / 10))));
	B2.setNoiseSpectralDensity(PLANCK_CONSTANT*SPEED_OF_LIGHT*(1/wavelength)*(pow(10, powerGain_dB/10)-1) * pow(10, noiseFigure/10)/2);
	


	setModuleBlocks({ &B1, &B2, &B3 });
};

void Edfa::setDirName(string newDirName) {
	dirName = newDirName;
	EDFA1.setFolderName(dirName);
	EDFA2.setFolderName(dirName);
}