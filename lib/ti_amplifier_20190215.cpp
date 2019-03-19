#include "ti_amplifier_20190215.h"


TI_Amplifier::TI_Amplifier(vector<Signal *> &inputSignal, vector<Signal *> &outputSignal) :SuperBlock(inputSignal, outputSignal) {
/*	inputSignals = inputSignal;
	outputSignals = outputSignal;

	numberOfInputSignals = inputSignal.size();
	numberOfOutputSignals = outputSignal.size();
	*/
	setSaveInternalSignals(true);
	B1.initializeBlock(vector<Signal *> { }, vector<Signal *> {&TI_AMP01});                             // Noise
	B2.initializeBlock(vector<Signal *> {inputSignals[0], &TI_AMP01}, vector<Signal *> {&TI_AMP02});    // Add
	B3.initializeBlock(vector<Signal *> {&TI_AMP02}, vector<Signal *> {&TI_AMP03});                     // Amp
	B4.initializeBlock(vector<Signal *> {&TI_AMP03}, vector<Signal *> {&TI_AMP04});                     // Filter
	setModuleBlocks({ &B1, &B2, &B3, &B4 });
	
};

void TI_Amplifier::setDirName(string newDirName) {
	dirName = newDirName;
	TI_AMP01.setFolderName(dirName);
	TI_AMP02.setFolderName(dirName);
	TI_AMP03.setFolderName(dirName);
	TI_AMP04.setFolderName(dirName);
}