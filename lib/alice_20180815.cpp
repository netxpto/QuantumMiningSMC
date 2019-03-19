# include "../include/alice_20180815.h"

Alice::Alice(initializer_list<Signal *> inputSignals, initializer_list<Signal *> outputSignals) : SuperBlock(inputSignals, outputSignals) {

	//###################################### Default Input Parameters Values #########################################################
	
	double bitRate{ 1e3 };
	int numberOfBits{ 1000 };
	int frameLength{ 10 };
	int headerLength{ 0 };
	vector<vector<t_real>> iqAmplitudesValues{ { 0, 0, 0, 1 },{ PI / 2, 0, 1,1 },{ -PI / 4, 1, 0, 1 },{ PI / 4, 1, 1, 1 } };
	// the in-phase componente is used to select both the basis and the bit, 00 => 0 (basis=0, bit=0), 01 => PI/2, 10 => -PI/4, 11 => PI/
	// the quadrature componente is used as a clock
	pulse_shapper_filter_type filterType{ pulse_shapper_filter_type::Square };
	int samplesPerSymbol{ 16 };
	//bool logValue{ true };
	//string logFileName{ "SuperBlock_Alice.txt" };
	//string signalsFolderName{ "signals/SuperBlock_Alice" };
	
	
	//############################################# Blocks ##########################################################################

	BinarySource_.setBitPeriod(1 / (2 * bitRate));
//	BinarySource_.setNumberOfBits(numberOfBits);

	QuantumFramer_.setFrameLength(frameLength);
	QuantumFramer_.setHeaderLength(headerLength);

	MQamMapper_.setIqAmplitudes(iqAmplitudesValues);

	DiscreteToContinuousTime_Basis.setNumberOfSamplesPerSymbol(samplesPerSymbol);

	PulseShaper_Basis.setFilterType(filterType);
	PulseShaper_Basis.setPulseWidth(1 / bitRate);

	DiscreteToContinuousTime_Bits.setNumberOfSamplesPerSymbol(samplesPerSymbol);

	PulseShaper_Bits.setFilterType(filterType);
	PulseShaper_Bits.setPulseWidth(1 / bitRate);
	
	DiscreteToContinuousTime_.setNumberOfSamplesPerSymbol(samplesPerSymbol);

	PulseShaper_.setFilterType(filterType);
	PulseShaper_.setSaveImpulseResponse(true);

	DiscreteToContinuousTime_1.setNumberOfSamplesPerSymbol(samplesPerSymbol);

	TimeDelay_.setTimeDelay(1 / bitRate / 2);
	TimeDelay_.setTimeDelay(0);

	//########################################################### SYSTEM ###################################################################
/*
	setSuperBlockSystem({	&BinarySource_,
							&Sink_BS_out_1,
							&Sink_BS_out_2,
							&QuantumFramer_,
							&MQamMapper_,
							&DiscreteToContinuousTime_,
							&PulseShaper_,
							&DiscreteToContinuousTime_1,
							&TimeDelay_,
							&SinglePhotonSource_,
							&Polarizer_ });


	setLogValue(logValue);
	setLogFileName(logFileName);
	setSignalsFolderName(signalsFolderName);
*/
};

void Alice::initialize()
{
		setSuperBlockSystem(
			{	&BinarySource_,
				&Sink_BS_out_1,
				&Sink_BS_out_2,
				&QuantumFramer_,
				&MQamMapper_,
				&DiscreteToContinuousTime_Basis,
				&PulseShaper_Basis,
				&DiscreteToContinuousTime_Bits,
				&PulseShaper_Bits,
				&DiscreteToContinuousTime_,
				&PulseShaper_,
				&DiscreteToContinuousTime_1,
				&TimeDelay_,
				&SinglePhotonSource_,
				&Polarizer_ }
		);

		outputSignals[0]->setSymbolPeriod(Alice_out_0.getSymbolPeriod());
		outputSignals[0]->setSamplingPeriod(Alice_out_0.getSamplingPeriod());
		outputSignals[0]->setFirstValueToBeSaved(Alice_out_0.getFirstValueToBeSaved());

		outputSignals[1]->setSymbolPeriod(Alice_out_1.getSymbolPeriod());
		outputSignals[1]->setSamplingPeriod(Alice_out_1.getSamplingPeriod());
		outputSignals[1]->setFirstValueToBeSaved(Alice_out_1.getFirstValueToBeSaved());

		outputSignals[2]->setSymbolPeriod(Alice_out_2.getSymbolPeriod());
		outputSignals[2]->setSamplingPeriod(Alice_out_2.getSamplingPeriod());
		outputSignals[2]->setFirstValueToBeSaved(Alice_out_2.getFirstValueToBeSaved());

		setLogValue(logValue);
		setLogFileName(logFileName);
		setSignalsFolderName(signalsFolderName);
}

bool Alice::runBlock()
{
	bool alive = SuperBlock::runBlock(signalsFolderName);

	if (firstTime) {
		setOpenFile(false);
		firstTime = false;
	}

	auto ready_0 = Alice_out_0.ready();
	auto ready_1 = Alice_out_1.ready();
	auto ready_2 = Alice_out_2.ready();
  	auto ready = min(min(ready_0, ready_1), ready_2);

	auto space{ MAX_BUFFER_LENGTH };
	for (auto k : outputSignals)
	{
		space = min(space, k->space());
	}
	
	auto process = min(ready, space);
	if (process > 0) alive = true;

	for (auto k = 0; k != process; k++)
	{
		
		t_real basis_value	= Alice_out_0.bufferGet();
		outputSignals[0]->bufferPut(basis_value);

		t_real bits_value = Alice_out_1.bufferGet();
		outputSignals[1]->bufferPut(bits_value);

		t_complex_xy photon_xy_value = Alice_out_2.bufferGet();
		outputSignals[2]->bufferPut(photon_xy_value);
	}

	return alive;
}
