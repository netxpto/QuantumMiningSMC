#include "ti_amplifier_20180102.h"


TI_Amplifier::TI_Amplifier(vector<Signal *> &inputSignal, vector<Signal *> &outputSignal) :SuperBlock(inputSignal, outputSignal) {
	inputSignals = inputSignal;
	outputSignals = outputSignal;

	B1.initializeBlock(vector<Signal *> {inputSignals[0]}, vector<Signal *> {&TI_AMP01});
	B2.initializeBlock(vector<Signal *> { }, vector<Signal *> {&TI_AMP02});
	B3.initializeBlock(vector<Signal *> {&TI_AMP01, &TI_AMP02}, vector<Signal *> {&TI_AMP03});

	setModuleBlocks({ &B1, &B2, &B3 });

	/*if (bypassFilter)
	{
		
	}
	else
	{
		B4.initializeBlock(vector<Signal *> {&TI_AMP03}, vector<Signal *> {&TI_AMP04});
		setModuleBlocks({ &B1, &B2, &B3, &B4 });
	}*/

};