# include "../include/bob_20181113.h"

Bob::Bob(initializer_list<Signal *> inputSignals, initializer_list<Signal *> outputSignals) : SuperBlock(inputSignals, outputSignals) {

	//###################################### Default Input Parameters Values #########################################################
	
	double bitRate{ 1e3 };
	int numberOfBits{ 1000 };
	vector<vector<t_real>> iqAmplitudesValues{ { 0, 1 },{ -PI / 4, 1 } };
	// the in-phase componente is used to select the basis, 0 => Linear,  -PI/4 => Diagonal
	// the quadrature componente can be used as a clock
	pulse_shapper_filter_type filterType{ pulse_shapper_filter_type::Square };
	int samplesPerSymbol{ 16 };
	bool logValue{ true };
	string logFileName{ "SuperBlock_Bob.txt" };
	string signalsFolderName{ "signals/SuperBlock_Bob" };
	double SPDProbabilityDarkCount{ 0.0 };
	double clockDetectorPhase{ PI / 2 };
	double decisionDetectorLevel{ 0.5 };
	double pulseWidthDetection{ 0.2 };
	
	//############################################# Blocks ##########################################################################

	BinarySource_.setBitPeriod(1 / (bitRate));
	BinarySource_.setNumberOfBits(numberOfBits);

	MQamMapper_.setIqAmplitudes(iqAmplitudesValues);
	
	DiscreteToContinuousTime_.setNumberOfSamplesPerSymbol(samplesPerSymbol);

	PulseShaper_.setFilterType(filterType);
	PulseShaper_.setPulseWidth(1);
	PulseShaper_.setSaveImpulseResponse(false);
	
	SinglePhotonDetector_0.setPath(0);
	SinglePhotonDetector_0.setProbabilityDarkCount(SPDProbabilityDarkCount);
	SinglePhotonDetector_1.setPath(1);
	SinglePhotonDetector_1.setProbabilityDarkCount(SPDProbabilityDarkCount);

	Clock_.setClockPeriod(1 / bitRate);
	Clock_.setSamplingPeriod((1 / bitRate) / samplesPerSymbol);
	Clock_.setClockPhase(clockDetectorPhase);

	PulseShaper_1.setFilterType(filterType);
	PulseShaper_1.setPulseWidth(0.8);
	PulseShaper_1.setSaveImpulseResponse(true);

	CoincidenceDetector_.setDecisionLevel(decisionDetectorLevel);
	
	
	//########################################################### SYSTEM ###################################################################

	setSuperBlockSystem({	&BinarySource_,
							&Sink_BS_out_2,
							&MQamMapper_,
							&DiscreteToContinuousTime_,
							&PulseShaper_,
							&PolarizationRotator_,
							&PolarizationBeamSplitter_,
							&SinglePhotonDetector_0,
							&SinglePhotonDetector_1,
							&Clock_,
							&PulseShaper_1,
							&CoincidenceDetector_ });

	setLogValue(logValue);
	setLogFileName(logFileName);
	setSignalsFolderName(signalsFolderName);

};

void Bob::initialize()
{
	
	outputSignals[0]->setSymbolPeriod(CD_out.getSymbolPeriod());
	outputSignals[0]->setSamplingPeriod(CD_out.getSamplingPeriod());
	outputSignals[0]->setFirstValueToBeSaved(CD_out.getFirstValueToBeSaved());
	
	SP_in.setSymbolPeriod(inputSignals[0]->getSymbolPeriod());
	SP_in.setSamplingPeriod(inputSignals[0]->getSamplingPeriod());
	SP_in.setFirstValueToBeSaved(inputSignals[0]->getFirstValueToBeSaved());
}

bool Bob::runBlock()
{
	bool alive = SuperBlock::runBlock();

	auto space{ MAX_BUFFER_LENGTH };

	/* Receive photon */
	auto process = min(inputSignals[0]->ready(), SP_in.space());
	if (process > 0) alive = true;

	for (auto k = 0; k != process; k++)
	{
		t_complex_xy sphoton_value;
		inputSignals[0]->bufferGet(&sphoton_value);

		SP_in.bufferPut(sphoton_value);
	}

	/* output */
	auto ready_0 = CD_out.ready();
	space = outputSignals[0]->space();
	auto process_out = min(ready_0, space);

	if (process_out > 0) alive = true;

	for (auto k = 0; k < process_out; k++) {
		t_real detection_value = CD_out.bufferGet();
		outputSignals[0]->bufferPut(detection_value);
	}

	return alive;
}