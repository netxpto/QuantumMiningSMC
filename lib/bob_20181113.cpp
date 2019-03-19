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
	//bool logValue{ true };
	//string logFileName{ "SuperBlock_Bob.txt" };
	//string signalsFolderName{ "signals/SuperBlock_Bob" };
	double SPDProbabilityDarkCount{ 0.0 };
	double clockDetectorPhase{ 0.0 };
	double decisionDetectorLevel{ 0.5 };
	double pulseWidthDetection{ 0.2 };
	BinarySourceMode modeBS{ BinarySourceMode::Random };
	string bitStreamBS{ "00" };
	
	//############################################# Blocks ##########################################################################
	

	BinarySource_.setBitPeriod(1 / (bitRate));
//	BinarySource_.setNumberOfBits(numberOfBits);
	BinarySource_.setMode(modeBS);
//	BinarySource_.setBitStream(bitStreamBS);

	MQamMapper_.setIqAmplitudes(iqAmplitudesValues);
	MQamMapper_.setFirstTime(false);

	DiscreteToContinuousTime_.setNumberOfSamplesPerSymbol(samplesPerSymbol);

	PulseShaper_.setFilterType(filterType);
	PulseShaper_.setPulseWidth(1 / bitRate);
	PulseShaper_.setSaveImpulseResponse(false);

	DiscreteToContinuousTime_1.setNumberOfSamplesPerSymbol(samplesPerSymbol);

//	TimeDelay_.setTimeDelay(1/bitRate/1.5);

	PulseShaper_clk.setFilterType(filterType);
	PulseShaper_clk.setPulseWidth(1 / bitRate / 1.5);

	SinglePhotonDetector_0.setPath(0);
	SinglePhotonDetector_0.setProbabilityDarkCount(SPDProbabilityDarkCount);
	SinglePhotonDetector_1.setPath(1);
	SinglePhotonDetector_1.setProbabilityDarkCount(SPDProbabilityDarkCount);

	Clock_.setClockPeriod(1 / bitRate);
	Clock_.setSamplingPeriod((1 / bitRate) / samplesPerSymbol);

//	TimeDelay_clk.setTimeDelay(1 / bitRate/3);

	PulseShaper_cd.setFilterType(filterType);
	PulseShaper_cd.setPulseWidth(1 / bitRate / 2);
	PulseShaper_cd.setSaveImpulseResponse(true);

	CoincidenceDetector_.setDecisionLevel(decisionDetectorLevel);


	//########################################################### SYSTEM ###################################################################
/*
	setSuperBlockSystem({	&BinarySource_,
							&Sink_BS_out_2,
							&MQamMapper_,
							&DiscreteToContinuousTime_1,
							&TimeDelay_,
							&Fork_,
							&DiscreteToContinuousTime_,
							&PulseShaper_,
							&PolarizationRotator_,
							&PolarizationBeamSplitter_,
							&SinglePhotonDetector_0,
							&SinglePhotonDetector_1,
							&Clock_,
							&TimeDelay_clk,
							&PulseShaper_1,
							&CoincidenceDetector_ });

	setLogValue(logValue);
	setLogFileName(logFileName);
	setSignalsFolderName(signalsFolderName);
*/
};

void Bob::initialize()
{
	Bob_in.setSymbolPeriod(inputSignals[0]->getSymbolPeriod());
	Bob_in.setSamplingPeriod(inputSignals[0]->getSamplingPeriod());
	Bob_in.setFirstValueToBeSaved(inputSignals[0]->getFirstValueToBeSaved());

	setSuperBlockSystem({ &BinarySource_,
		&Sink_BS_out_2,
		&MQamMapper_,
		&DiscreteToContinuousTime_1,
		&TimeDelay_,
		&PulseShaper_clk,
		&Fork_ ,
		&DiscreteToContinuousTime_,
		&PulseShaper_ ,
		&PolarizationRotator_,
		&PolarizationBeamSplitter_,
		&SinglePhotonDetector_0,
		&SinglePhotonDetector_1,
		&Clock_,
		&TimeDelay_clk,
		&PulseShaper_cd,
		&CoincidenceDetector_});

/*	setSuperBlockSystem({ &BinarySource_,
							&Sink_BS_out_2,
							&MQamMapper_,
							&DiscreteToContinuousTime_1,
							&TimeDelay_,
							&Fork_,
							&PulseShaper_spd0,
							&PulseShaper_spd1,
							&DiscreteToContinuousTime_,
							&PulseShaper_,
							&PolarizationRotator_,
							&PolarizationBeamSplitter_,
							&SinglePhotonDetector_0,
							&SinglePhotonDetector_1,
							&Clock_,
							&TimeDelay_clk,
							&PulseShaper_1,
							&CoincidenceDetector_ });
							

		
		*/
		outputSignals[0]->setSymbolPeriod(CD_out.getSymbolPeriod());
		outputSignals[0]->setSamplingPeriod(CD_out.getSamplingPeriod());
		outputSignals[0]->setFirstValueToBeSaved(CD_out.getFirstValueToBeSaved());

		setLogValue(logValue);
		setLogFileName(logFileName);
		setSignalsFolderName(signalsFolderName);
}

bool Bob::runBlock()
{
	auto ready_1 = inputSignals[0]->ready();

	auto space_1{ MAX_BUFFER_LENGTH };

	space_1 = Bob_in.getBufferLength();
	auto space_2 = Bob_in.space();
	space_1 = min(space_1, space_2);
	auto process = min(ready_1, space_1);

	for (auto k = 0; k < process; k++) {
		t_complex_xy photonIn;
		inputSignals[0]->bufferGet(&photonIn);
		Bob_in.bufferPut((t_complex_xy)photonIn);
	}

	bool alive = SuperBlock::runBlock(signalsFolderName);

	if (firstTime) {
		setOpenFile(false);
		firstTime = false;
	}

	auto space{ MAX_BUFFER_LENGTH };

	/* output */
	
	auto ready_0 = CD_out.ready();
	space = outputSignals[0]->space();
	auto process_out = min(ready_0, space);

	if (process_out > 0) alive = true;

	for (auto k = 0; k < process_out; k++) {
		t_real detection_value = CD_out.bufferGet();
		outputSignals[0]->bufferPut((t_real)detection_value);
	}
	
	return alive;
}