# include "m_qam_transmitter_20190215.h"


MQamTransmitter::MQamTransmitter(vector<Signal *> &inputSignal, vector<Signal *> &outputSignal) :SuperBlock(inputSignal, outputSignal) {

numberOfInputSignals = inputSignal.size();
numberOfOutputSignals = outputSignal.size();

signal_value_type sType = outputSignals[0]->getValueType();

switch (numberOfOutputSignals) {
	case 1:
		B1.initializeBlock(vector<Signal*> {}, vector<Signal*> { &S1});
		B2.initializeBlock(vector<Signal*> { &S1 }, vector<Signal*> { &S2, &S3 });
		B3.initializeBlock(vector<Signal*> { &S2 }, vector<Signal*> { &S4 });
		B4.initializeBlock(vector<Signal*> { &S3 }, vector<Signal*> { &S5 });
		B5.initializeBlock(vector<Signal*> { &S4 }, vector<Signal*> { &S6 });
		B6.initializeBlock(vector<Signal*> { &S5 }, vector<Signal*> { &S7 });
		if (sType == ComplexValue) {
			S9.setBufferLength(512);
			B7.initializeBlock(vector<Signal*> { &S6, &S7 }, vector<Signal*> { &S9 });
		} 
		else {
			S9_xy.setBufferLength(512);
			B7.initializeBlock(vector<Signal*> { &S6, &S7 }, vector<Signal*> { &S9_xy });
		}
		setModuleBlocks({ &B1, &B2, &B3, &B4, &B5, &B6, &B7 });
		break;
	case 2:
		B1.initializeBlock(vector<Signal*> {}, vector<Signal*> { &S1, &S10 });
		B2.initializeBlock(vector<Signal*> { &S1 }, vector<Signal*> { &S2, &S3 });
		B3.initializeBlock(vector<Signal*> { &S2 }, vector<Signal*> { &S4 });
		B4.initializeBlock(vector<Signal*> { &S3 }, vector<Signal*> { &S5 });
		B5.initializeBlock(vector<Signal*> { &S4 }, vector<Signal*> { &S6 });
		B6.initializeBlock(vector<Signal*> { &S5 }, vector<Signal*> { &S7 });
		if (sType == ComplexValue) {
			S9.setBufferLength(512);
			S11.setBufferLength(512);
			B7.initializeBlock(vector<Signal*> { &S6, &S7 }, vector<Signal*> { &S9 });
			B8.initializeBlock(vector<Signal*> {&S9, &S10}, vector<Signal*> {&S11, &S12});
		}
		else {
			S9_xy.setBufferLength(512);
			S11_xy.setBufferLength(512);
			B7.initializeBlock(vector<Signal*> { &S6, &S7 }, vector<Signal*> { &S9_xy });
			B8.initializeBlock(vector<Signal*> {&S9_xy, &S10}, vector<Signal*> {&S11_xy, &S12});
		}
		setModuleBlocks({ &B1, &B2, &B3, &B4, &B5, &B6, &B7, &B8 });
		break;
	default:
		cout << "ERRO: m_qam_transmitter.cpp" << "\n";
}

};

void MQamTransmitter::set(int opt) {

	// Basic Configuration
 
	if (opt==0) {
		setMode(DeterministicAppendZeros);
		setBitPeriod(1.0 / 50e9);
		setPatternLength(5);
		setNumberOfBits(10000);
		setNumberOfSamplesPerSymbol(16); //antes 32
		setRollOffFactor(0.3);
		setIqAmplitudes({ { 1.0, 1.0 },{ -1.0, 1.0 },{ -1.0, -1.0 },{ 1.0, -1.0 } });
		setOutputOpticalPower_dBm(0); //antes 0
		setSaveInternalSignals(true);
	}

	return;
}

void MQamTransmitter::setDirName(string newDirName) {
	dirName = newDirName;
	S1.setFolderName(dirName);
	S2.setFolderName(dirName);
	S3.setFolderName(dirName);
	S4.setFolderName(dirName);
	S5.setFolderName(dirName);
	S6.setFolderName(dirName);
	S7.setFolderName(dirName);
//	S8.setFolderName(dirName);
	S9.setFolderName(dirName);
	S10.setFolderName(dirName);
	S11.setFolderName(dirName);
	S12.setFolderName(dirName);
}