# include "m_qam_transmitter.h"


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
			S8.setBufferLength(512);
			B7.initializeBlock(vector<Signal*> { &S6, &S7 }, vector<Signal*> { &S8 });
		} 
		else {
			S8_xy.setBufferLength(512);
			B7.initializeBlock(vector<Signal*> { &S6, &S7 }, vector<Signal*> { &S8_xy });
		}
		setModuleBlocks({ &B1, &B2, &B3, &B4, &B5, &B6, &B7 });
		break;
	case 2:
		B1.initializeBlock(vector<Signal*> {}, vector<Signal*> { &S1, &S9 });
		B2.initializeBlock(vector<Signal*> { &S1 }, vector<Signal*> { &S2, &S3 });
		B3.initializeBlock(vector<Signal*> { &S2 }, vector<Signal*> { &S4 });
		B4.initializeBlock(vector<Signal*> { &S3 }, vector<Signal*> { &S5 });
		B5.initializeBlock(vector<Signal*> { &S4 }, vector<Signal*> { &S6 });
		B6.initializeBlock(vector<Signal*> { &S5 }, vector<Signal*> { &S7 });
		if (sType == ComplexValue) {
			S8.setBufferLength(512);
			S10.setBufferLength(512);
			B7.initializeBlock(vector<Signal*> { &S6, &S7 }, vector<Signal*> { &S8 });
			B8.initializeBlock(vector<Signal*> {&S8, &S9}, vector<Signal*> {&S10, &S11});
		}
		else {
			S8_xy.setBufferLength(512);
			S10_xy.setBufferLength(512);
			B7.initializeBlock(vector<Signal*> { &S6, &S7 }, vector<Signal*> { &S8_xy });
			B8.initializeBlock(vector<Signal*> {&S8_xy, &S9}, vector<Signal*> {&S10_xy, &S11});
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