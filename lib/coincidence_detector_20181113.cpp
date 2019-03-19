#include "../include/coincidence_detector_20181113.h"

void CoincidenceDetector::initialize(void) {

	outputSignals[0]->setSymbolPeriod(inputSignals[0]->getSymbolPeriod());
	outputSignals[0]->setSamplingPeriod(inputSignals[0]->getSymbolPeriod());
	outputSignals[0]->setFirstValueToBeSaved(inputSignals[0]->getFirstValueToBeSaved());
}

bool CoincidenceDetector::runBlock(void) {

	int ready_0 = inputSignals[0]->ready();
	int ready_1 = inputSignals[1]->ready();
	int ready = min(ready_0, ready_1);

	ready = min(ready, inputSignals[2]->ready());

	auto space = MAX_BUFFER_LENGTH;
	for (auto k : outputSignals) space = min(space, k->space());

	int process = min(ready, space);

	signal_value_type inSignalType_0 = inputSignals[0]->getValueType();
	signal_value_type inSignalType_1 = inputSignals[1]->getValueType();
	process = ((inSignalType_0 != inSignalType_1) && inSignalType_0!=signal_value_type::t_real) ? 0 : process;

	if (process <= 0) return false;

	for (auto k = 0; k < process; k++) {
		t_real clock_, spd_in_0, spd_in_1;
		
		inputSignals[0]->bufferGet(&spd_in_0);
		inputSignals[1]->bufferGet(&spd_in_1);
		inputSignals[2]->bufferGet(&clock_);

		on = (clock_ == 1.0) ? true : on;
		
		if (clock_ == 0.0) {
			if (on == true) {
				on = false;
				outputSignals[0]->bufferPut((t_real)value);
				value = 3.0;
			}
		}

		if (on == true) {
			value = ((spd_in_0 > decisionLevel) && value == 3.0) ? 0.0 : value;	// SPD0 clicks
			value = ((spd_in_1 > decisionLevel) && value == 3.0) ? 1.0 : value; // SPD1 clicks
			value = ((spd_in_1 > decisionLevel) && value == 0.0) ? 2.0 : value; // SPD0 & SPD1 click
			value = ((spd_in_0 > decisionLevel) && value == 1.0) ? 2.0 : value; // SPD0 & SPD1 click
		}
	}

	return true;
};
