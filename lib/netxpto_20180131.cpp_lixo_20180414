# include <complex>
# include <fstream>
# include <iostream>
# include <math.h>
# include <stdio.h>
# include <string>
# include <strstream>
# include <algorithm>


# include "netxpto_20180118.h"


using namespace std;

#ifndef M_PI
#define M_PI  3.14159265358979323846
#endif

//########################################################################################################################################################
//######################################################### SIGNALS FUNCTIONS IMPLEMENTATION #############################################################
//########################################################################################################################################################

void Signal::close() {

	if (saveSignal && (inPosition >= firstValueToBeSaved)) {
		
		if (!(type == "Message")) {
			char *ptr = (char *)buffer;

			ofstream fileHandler;
			fileHandler.open("./signals/" + fileName, ios::out | ios::binary | ios::app);

			if (type == "Binary") {
				ptr = ptr + (firstValueToBeSaved - 1) * sizeof(t_binary);
				fileHandler.write((char *)ptr, (inPosition - (firstValueToBeSaved - 1)) * sizeof(t_binary));
			}
			else if (type == "TimeContinuousAmplitudeContinuousComplex" || type == "BandpassSignal") {
				ptr = ptr + (firstValueToBeSaved - 1) * sizeof(t_complex);
				fileHandler.write((char *)ptr, (inPosition - (firstValueToBeSaved - 1)) * sizeof(t_complex));
			}
			else {
				ptr = ptr + (firstValueToBeSaved - 1) * sizeof(t_real);
				fileHandler.write((char *)ptr, (inPosition - (firstValueToBeSaved - 1)) * sizeof(t_real));
			}
			inPosition = 0;
			fileHandler.close();
		}
		else if (type == "Message") {
			int fValueToBeSaved = getFirstValueToBeSaved();
			int bLength = getBufferLength();
			
			if (fValueToBeSaved < (inPosition+1)) {
				t_message *ptr = (t_message *)buffer;
				ptr = ptr + (fValueToBeSaved - 1);

				ofstream fileHandler("./" + folderName + "/" + fileName, ios::out | ios::app);

				for (int msg = fValueToBeSaved; msg < (inPosition+1); msg++) {
					for (unsigned int fld = 0; fld < (*ptr).size(); fld++) {
						fileHandler << ptr->at(fld).fieldName + "\t" + ptr->at(fld).fieldValue + "\t";
					}
					fileHandler << "\n";
					ptr++;
				}
				fileHandler.close();
				setFirstValueToBeSaved(1);
			}
		}
	}
};

int Signal::space() {

	if (bufferFull) return 0;
	if (inPosition == outPosition) return bufferLength;
	if (inPosition < outPosition) return (outPosition - inPosition);
	if (outPosition >= 0) return (bufferLength - inPosition + outPosition);
	if (outPosition == -1) return (bufferLength - inPosition);
	return -1;

};

int Signal::ready() {

	if (bufferEmpty) return 0;

	if (outPosition == inPosition) {
		return bufferLength;
	}
	else {
		if (outPosition == -1) return 0;
		if (inPosition > outPosition) {
			return (inPosition - outPosition);
		}
		else {
			return (bufferLength - outPosition + inPosition);
		}

	}
};

void Signal::writeHeader(){

	ofstream headerFile;

	if (saveSignal && (!fileName.empty())) {

		headerFile.open("./" + folderName + "/" + fileName, ios::out);

		headerFile << "Signal type: " << type << "\n";
		headerFile << "Symbol Period (s): " << symbolPeriod << "\n";
		headerFile << "Sampling Period (s): " << samplingPeriod << "\n";
		//headerFile << "Samples per Symbol (s): " << samplesPerSymbol << "\n";

		headerFile << "// ### HEADER TERMINATOR ###\n";

		headerFile.close();
	}
};

void Signal::writeHeader(string signalPath){

	ofstream headerFile;

	if (saveSignal && (!fileName.empty())) {

		headerFile.open("./" + signalPath + "/" + fileName, ios::out);

		headerFile << "Signal type: " << type << "\n";
		headerFile << "Symbol Period (s): " << symbolPeriod << "\n";
		headerFile << "Sampling Period (s): " << samplingPeriod << "\n";
		//headerFile << "Samples per Symbol (s): " << samplesPerSymbol << "\n";

		headerFile << "// ### HEADER TERMINATOR ###\n";

		headerFile.close();
	}
};

void Signal::bufferGet() {
	if (bufferFull) bufferFull = false;
	outPosition++;
	if (outPosition == bufferLength) outPosition = 0;
	if (outPosition == inPosition) bufferEmpty = true;
	return;
};

void Signal::bufferGet(t_binary *valueAddr) {
	*valueAddr = static_cast<t_binary *>(buffer)[outPosition];
	if (bufferFull) bufferFull = false;
	outPosition++;
	if (outPosition == bufferLength) outPosition = 0;
	if (outPosition == inPosition) bufferEmpty = true;
	return;
};

void Signal::bufferGet(t_integer *valueAddr) {
	*valueAddr = static_cast<t_integer *>(buffer)[outPosition];
	if (bufferFull) bufferFull = false;
	outPosition++;
	if (outPosition == bufferLength) outPosition = 0;
	if (outPosition == inPosition) bufferEmpty = true;
	return;
};

void Signal::bufferGet(t_real *valueAddr) {
	*valueAddr = static_cast<t_real *>(buffer)[outPosition];
	if (bufferFull) bufferFull = false;
	outPosition++;
	if (outPosition == bufferLength) outPosition = 0;
	if (outPosition == inPosition) bufferEmpty = true;
	return;
};

void Signal::bufferGet(t_complex *valueAddr) {
	*valueAddr = static_cast<t_complex *>(buffer)[outPosition];
	if (bufferFull) bufferFull = false;
	outPosition++;
	if (outPosition == bufferLength) outPosition = 0;
	if (outPosition == inPosition) bufferEmpty = true;
	return;
};

void Signal::bufferGet(t_complex_xy *valueAddr) {
	*valueAddr = static_cast<t_complex_xy *>(buffer)[outPosition];
	if (bufferFull) bufferFull = false;
	outPosition++;
	if (outPosition == bufferLength) outPosition = 0;
	if (outPosition == inPosition) bufferEmpty = true;
	return;
};

void Signal::bufferGet(t_photon *valueAddr) {
	*valueAddr = static_cast<t_photon *>(buffer)[outPosition];
	if (bufferFull) bufferFull = false;
	outPosition++;
	if (outPosition == bufferLength) outPosition = 0;
	if (outPosition == inPosition) bufferEmpty = true;
	return;
};

void Signal::bufferGet(t_photon_mp *valueAddr) {
	*valueAddr = static_cast<t_photon_mp *>(buffer)[outPosition];
	if (bufferFull) bufferFull = false;
	outPosition++;
	if (outPosition == bufferLength) outPosition = 0;
	if (outPosition == inPosition) bufferEmpty = true;
	return;
};

void Signal::bufferGet(t_message *valueAddr) {
	*valueAddr = static_cast<t_message *>(buffer)[outPosition];
	if (bufferFull) bufferFull = false;
	outPosition++;
	if (outPosition == bufferLength) outPosition = 0;
	if (outPosition == inPosition) bufferEmpty = true;
	return;
};

void Messages::bufferPut(t_message value) {
	(static_cast<t_message*>(buffer))[getInPosition()] = value;
	if (getBufferEmpty()) setBufferEmpty(false);
	setInPosition(getInPosition()+1);
	if (getInPosition() == getBufferLength()) {
		setInPosition(0);
		if (getSaveSignal()) {
			int fValueToBeSaved = getFirstValueToBeSaved();
			int bLength = getBufferLength();
			if (fValueToBeSaved <= getBufferLength()) {
				t_message *ptr = (t_message *)buffer;
				ptr = ptr + (fValueToBeSaved - 1);
				ofstream fileHandler("./" + getFolderName() + "/" + getFileName(), ios::out | ios::app);
				for (int msg = fValueToBeSaved; msg <= bLength; msg++) {
						for (unsigned int fld = 0; fld < value.size(); fld++) {
							fileHandler << ptr->at(fld).fieldName + "\t" + ptr->at(fld).fieldValue + "\t";
						}
						fileHandler << "\n";
						ptr++;
				}
				fileHandler.close();
				setFirstValueToBeSaved(1);
			}
			else {
				setFirstValueToBeSaved(fValueToBeSaved - bLength);
			}
		}
	}
	if (getInPosition() == getOutPosition()) setBufferFull(true);
};

//########################################################################################################################################################
//###################################################### GENERAL BLOCKS FUNCTIONS IMPLEMENTATION #########################################################
//########################################################################################################################################################

Block::Block(vector<Signal*> &InputSig, vector<Signal*> &OutputSig) {

	numberOfInputSignals = (int) InputSig.size();
	numberOfOutputSignals = (int) OutputSig.size();

	inputSignals = InputSig;
	outputSignals = OutputSig;
}

void Block::initializeBlock(vector<Signal*> &InputSig, vector<Signal*> &OutputSig) {
	numberOfInputSignals = (int) InputSig.size();
	numberOfOutputSignals = (int) OutputSig.size();

	inputSignals = InputSig;
	outputSignals = OutputSig;
}

void Block::initializeBlock(void) {

	for (int unsigned j = 0; j<inputSignals.size(); j++) {
		inputSignals[j]->writeHeader();
	}

	initialize();

}

bool Block::runBlock(void) {
	return false;
}

void Block::terminateBlock(void) {

  terminate();

  for (int i = 0; i < numberOfInputSignals; i++)
    inputSignals[i]->close();

}

void SuperBlock::initialize(void){

	for (int unsigned i = 0; i < moduleBlocks.size(); i++) {
		moduleBlocks[i]->initializeBlock();
	}

	for (int unsigned j = 0; j<(moduleBlocks[moduleBlocks.size() - 1]->outputSignals).size(); j++)
		moduleBlocks[moduleBlocks.size() - 1]->outputSignals[j]->writeHeader();

	for (unsigned int i = 0; i < outputSignals.size(); i++) {
		outputSignals[i]->setSymbolPeriod(moduleBlocks[moduleBlocks.size() - 1]->outputSignals[i]->getSymbolPeriod());
		outputSignals[i]->setSamplingPeriod(moduleBlocks[moduleBlocks.size() - 1]->outputSignals[i]->getSamplingPeriod());
		outputSignals[i]->setFirstValueToBeSaved(moduleBlocks[moduleBlocks.size() - 1]->outputSignals[i]->getFirstValueToBeSaved());
	}

};

bool SuperBlock::runBlock() {


	bool alive{ false };
	bool proceed{ false };

	do {

		proceed = false;

		for (unsigned int i = 0; i < moduleBlocks.size(); i++) {
			bool aux = moduleBlocks[i]->runBlock();
			proceed = (proceed || aux);
			alive = (alive || proceed);
		}

		for (unsigned int i = 0; i < outputSignals.size(); i++) {
			int ready = moduleBlocks[moduleBlocks.size() - 1]->outputSignals[i]->ready();
			int space = outputSignals[i]->space();
			int length = (ready <= space) ? ready : space;

			signal_value_type sType = moduleBlocks[moduleBlocks.size() - 1]->outputSignals[i]->getValueType();
			switch (sType) {
				case BinaryValue:
					for (int j = 0; j < length; j++) {
						t_binary signalValue;
						moduleBlocks[moduleBlocks.size() - 1]->outputSignals[i]->bufferGet(&signalValue);
						outputSignals[i]->bufferPut(signalValue);
					}
					break;
				case RealValue:
					for (int j = 0; j < length; j++) {
						t_real signalValue;
						moduleBlocks[moduleBlocks.size() - 1]->outputSignals[i]->bufferGet(&signalValue);
						outputSignals[i]->bufferPut(signalValue);
					}
					break;
				case ComplexValue:
					for (int j = 0; j < length; j++) {
						t_complex signalValue;
						moduleBlocks[moduleBlocks.size() - 1]->outputSignals[i]->bufferGet(&signalValue);
						outputSignals[i]->bufferPut(signalValue);
					}
					break;
				case ComplexValueXY:
					for (int j = 0; j < length; j++) {
						t_complex_xy signalValueXY;
						moduleBlocks[moduleBlocks.size() - 1]->outputSignals[i]->bufferGet(&signalValueXY);
						outputSignals[i]->bufferPut(signalValueXY);
					}
					break;
				default:
					cout << "ERRO: netxpto.cpp (SuperBlock)" << "\n";
					return false;

			}

			
		}

	} while (proceed);

	return alive;
}


void SuperBlock::terminate() {

	for (int unsigned i = 0; i < moduleBlocks.size(); i++) {
		moduleBlocks[i]->terminateBlock();
	}


	for (int unsigned j = 0; j<(moduleBlocks[moduleBlocks.size() - 1]->outputSignals).size(); j++)
		moduleBlocks[moduleBlocks.size() - 1]->outputSignals[j]->close();

}

void SuperBlock::setSaveInternalSignals(bool sInternalSignals) {

	for (int unsigned i = 0; i < moduleBlocks.size(); i++) {
		for (int unsigned j = 0; j < (moduleBlocks[i]->inputSignals).size(); j++)
			moduleBlocks[i]->inputSignals[j]->setSaveSignal(sInternalSignals);
		for (int unsigned j = 0; j < (moduleBlocks[i]->outputSignals).size(); j++)
			moduleBlocks[i]->outputSignals[j]->setSaveSignal(sInternalSignals);
	}
}


void FIR_Filter::initializeFIR_Filter(void) {

	outputSignals[0]->setSymbolPeriod(inputSignals[0]->getSymbolPeriod());
	outputSignals[0]->setSamplingPeriod(inputSignals[0]->getSamplingPeriod());
	outputSignals[0]->setSamplesPerSymbol(inputSignals[0]->getSamplesPerSymbol());

	if (!getSeeBeginningOfImpulseResponse()) {
		int aux = (int)(((double)impulseResponseLength) / 2) + 1;
		outputSignals[0]->setFirstValueToBeSaved(aux);
	}

	delayLine.resize(impulseResponseLength, 0);

	if (saveImpulseResponse) {
		ofstream fileHandler("./signals/" + impulseResponseFilename, ios::out);
		fileHandler << "// ### HEADER TERMINATOR ###\n";

		t_real t;
		double samplingPeriod = inputSignals[0]->getSamplingPeriod();
		for (int i = 0; i < impulseResponseLength; i++) {
			t = -impulseResponseLength / 2 * samplingPeriod + i * samplingPeriod;
			fileHandler << t << " " << impulseResponse[i] << "\n";
		}
		fileHandler.close();
	}

};

bool FIR_Filter::runBlock(void) {

	
	int ready = inputSignals[0]->ready();
	int space = outputSignals[0]->space();

	int process = min(ready, space);

	if (process == 0) return false;

	for (int i = 0; i < process; i++) {
		t_real val;
		(inputSignals[0])->bufferGet(&val);
		if (val != 0) {
			vector<t_real> aux(impulseResponseLength, 0.0);
			transform(impulseResponse.begin(), impulseResponse.end(), aux.begin(), bind1st(multiplies<t_real>(), val));
			transform(aux.begin(), aux.end(), delayLine.begin(), delayLine.begin(), plus<t_real>());
		}
		outputSignals[0]->bufferPut((t_real)(delayLine[0]));
		rotate(delayLine.begin(), delayLine.begin() + 1, delayLine.end());
		delayLine[impulseResponseLength - 1] = 0.0;
	}

	return true;
};


void FD_Filter::initializeFD_Filter(void) {

	outputSignals[0]->setSymbolPeriod(inputSignals[0]->getSymbolPeriod());
	outputSignals[0]->setSamplingPeriod(inputSignals[0]->getSamplingPeriod());
	outputSignals[0]->setSamplesPerSymbol(inputSignals[0]->getSamplesPerSymbol());

/*	if (!getSeeBeginningOfImpulseResponse()) {
		int aux = (int)(((double)impulseResponseLength) / 2) + 1;
		outputSignals[0]->setFirstValueToBeSaved(aux);
	}*/

/*	delayLine.resize(impulseResponseLength, 0);*/

	
	inputBufferTimeDomain.resize(inputBufferTimeDomainLength);
	outputBufferTimeDomain.resize(inputBufferTimeDomainLength);

	inputBufferPointer = inputBufferTimeDomainLength / 2 ;
	outputBufferPointer = inputBufferTimeDomainLength;

	//raisedCosineTF(transferFunction, transferFunctionLength, rollOffFactor, samplingPeriod, symbolPeriod);

	if (saveTransferFunction) {
		ofstream fileHandler("./signals/" + transferFunctionFilename, ios::out);
		fileHandler << "// ### HEADER TERMINATOR ###\n";

		double samplingPeriod = inputSignals[0]->getSamplingPeriod();
		t_real fWindow = 1/samplingPeriod;
		t_real df = fWindow / transferFunctionLength;

		t_real f;
		for (int k = 0; k < transferFunctionLength; k++) {
			f = -fWindow / 2 + k * df;
			fileHandler << f << " " << transferFunction[k] << "\n";
		}
		fileHandler.close();
	}

};

/*void FD_Filter::OverlapSaveMethod(void) {

	int NFFT = transferFunctionLength;
	int Nblocks = 2 * (inputBufferTimeDomain.size() / NFFT);

	vector<double> real_temp(NFFT, 0);
	vector<double> imag_temp(NFFT, 0);
	vector<double> real_temp_copy(NFFT, 0);
	vector<double> imag_temp_copy(NFFT, 0);
	vector<t_real> H_real(NFFT, 0);
	vector<t_real> H_imag(NFFT, 0);
	Fft fft;
	ComplexMult CMult;
	CMult.ComplexVect2ReImVect(transferFunction, H_real, H_imag);
	//fft.directTransform(h_real, h_imag);

	for (int k = 0; k < Nblocks; k++) {

		if (k == Nblocks - 1) {
			copy(inputBufferTimeDomain.begin(), inputBufferTimeDomain.begin() + (NFFT / 2), real_temp.begin() + (NFFT / 2));
			//copy(imag_in.begin(), imag_in.begin() + (NFFT / 2), imag_temp.begin() + (NFFT / 2));

			copy(inputBufferTimeDomain.end() - (NFFT / 2), inputBufferTimeDomain.end(), real_temp.begin());
			//copy(imag_in.end() - (NFFT / 2), imag_in.end(), imag_temp.begin());
		}
		else {
			copy(inputBufferTimeDomain.begin() + k*(NFFT / 2), inputBufferTimeDomain.begin() + ((k + 1)*NFFT - k*(NFFT / 2)), real_temp.begin());
			//copy(imag_in.begin() + k*(NFFT / 2), imag_in.begin() + ((k + 1)*NFFT - k*(NFFT / 2)), imag_temp.begin());
		}

		// coping real_temp/imag_temp into real_temp_copy/imag_temp_copy vectors
		real_temp_copy = real_temp;
		imag_temp_copy = imag_temp;

		// Computation of FFT of each block
		fft.directTransform(real_temp_copy, imag_temp_copy);

		// Multiplication with the transfer function
		CMult.CMultVector_Loop(real_temp_copy, imag_temp_copy, H_real, H_imag);

		// Computation of IFFT of each block
		fft.inverseTransform(real_temp_copy, imag_temp_copy);

		// Removing the samples symetrically and assign to the output
		if (k == Nblocks - 1) {
			copy(real_temp_copy.begin() + (NFFT / 4), real_temp_copy.begin() + (NFFT / 2), outputBufferTimeDomain.end() - (NFFT / 4));
			//copy(imag_temp_copy.begin() + (NFFT / 4), imag_temp_copy.begin() + (NFFT / 2), imag_out.end() - (NFFT / 4));

			copy(real_temp_copy.begin() + (NFFT / 2), real_temp_copy.begin() + NFFT, outputBufferTimeDomain.begin());
			//copy(imag_temp_copy.begin() + (NFFT / 2), imag_temp_copy.begin() + NFFT, imag_out.begin());
		}
		else {
			copy(real_temp_copy.begin() + (NFFT / 4), real_temp_copy.end() - (NFFT / 4), outputBufferTimeDomain.begin() + ((NFFT / 4) + k*(NFFT / 2)));
			//copy(imag_temp_copy.begin() + (NFFT / 4), imag_temp_copy.end() - (NFFT / 4), imag_out.begin() + ((NFFT / 4) + k*(NFFT / 2)));
		}
	}

}

void FD_Filter::overlapSaveZPRealIn(void) {
	int NFFT = transferFunctionLength;
	int Nblocks = 2 * (inputBufferTimeDomain.size() / NFFT);

	vector<double> var_temp(NFFT, 0);
	vector<double> real_temp_copy(NFFT, 0);
	vector<double> imag_temp_copy(NFFT, 0);
	vector<double> H_real(NFFT, 0);
	vector<double> H_imag(NFFT, 0);
	vector<double> v_in_temp(NFFT / 2, 0);
	vector<double> v_out_temp(NFFT / 2, 0);

	Fft fft;
	ComplexMult CMult;

	CMult.ComplexVect2ReImVect(transferFunction, H_real, H_imag);

	for (int k = 0; k < Nblocks; k++) {

		copy(inputBufferTimeDomain.begin() + k*(NFFT / 2), inputBufferTimeDomain.begin() + ((k + 1)*(NFFT / 2)), v_in_temp.begin());

		copy(v_in_temp.begin(), v_in_temp.end(), var_temp.begin() + (NFFT / 2));
		real_temp_copy = var_temp;
		rotate(var_temp.begin(), var_temp.begin() + NFFT / 2, var_temp.end());
		// Computation of FFT of each block

		fft.directTransform(real_temp_copy, imag_temp_copy);

		// Multiplication with the transfer function
		CMult.CMultVector_Loop(real_temp_copy, imag_temp_copy, H_real, H_imag);

		// Computation of IFFT of each block
		fft.inverseTransform(real_temp_copy, imag_temp_copy);

		// Removing the samples symetrically and assign to the output

		copy(real_temp_copy.begin() + (NFFT / 2), real_temp_copy.end(), v_out_temp.begin());

		copy(v_out_temp.begin(), v_out_temp.end(), outputBufferTimeDomain.begin() + k*(NFFT / 2));

	}
}*/

bool FD_Filter::runBlock(void) {

	
	Fft fft;
	ComplexMult CMult;

	bool alive{ false };

	int ready = inputSignals[0]->ready();
	//inputBufferTimeDomain.resize(ready);
	//outputBufferTimeDomain.resize(ready);
	int space = (int) inputBufferTimeDomain.size() - inputBufferPointer;

	int process1 = min(ready, space);
	//int process1 = ready;
	if (process1 > 0) alive = true;

	// read incoming samples to the input buffer
	for (int k = 0; k < process1; k++) {
		t_real val;
		inputSignals[0]->bufferGet(&val);
		inputBufferTimeDomain[inputBufferPointer] = val;
		inputBufferPointer++;
	}
	//overlapSaveZPRealIn();
	//OverlapSaveMethod();
	if ((inputBufferPointer == inputBufferTimeDomain.size()) && (outputBufferPointer == outputBufferTimeDomain.size()))
	{
		outputBufferTimeDomain = fft.inverseTransformInCP(CMult.CMultVectorInCP(fft.directTransformInReal(inputBufferTimeDomain), transferFunction));
		rotate(inputBufferTimeDomain.begin(), inputBufferTimeDomain.begin() + inputBufferTimeDomain.size()/2, inputBufferTimeDomain.end());
		inputBufferPointer = (int) inputBufferTimeDomain.size()/2;
		outputBufferPointer = (int) outputBufferTimeDomain.size()/2;
	};

	//space = outputSignals[0]->space();
	//ready = outputBufferTimeDomain.size() - outputBufferPointer;

	int process2 = min(ready, space);
	if (process2 > 0) alive = true;
	//int process2 = outputBufferTimeDomain.size();
	for (int k = 0; k < process2; k++) {
		t_real val = outputBufferTimeDomain[outputBufferPointer];
		(outputSignals[0])->bufferPut(&val);
		outputBufferPointer++;
	}
	//alive = false;

	return alive;
};
     

/*2016-08-03
DiscreteToContinuousTime::DiscreteToContinuousTime(vector<Signal *> &InputSig, vector<Signal *> &OutputSig) {

  numberOfInputSignals = InputSig.size();
  numberOfOutputSignals = OutputSig.size();

  inputSignals = InputSig;
  outputSignals = OutputSig;

  outputSignals[0]->symbolPeriod = (inputSignals[0]->symbolPeriod);
  outputSignals[0]->samplingPeriod = (inputSignals[0]->samplingPeriod) / numberOfSamplesPerSymbol;

  
}
*/



RealToComplex::RealToComplex(vector <Signal *> &InputSig, vector <Signal *> &OutputSig) {

  numberOfInputSignals = (int) InputSig.size();
  numberOfOutputSignals = (int) OutputSig.size();

  inputSignals = InputSig;
  outputSignals = OutputSig;

  outputSignals[0]->setSymbolPeriod(inputSignals[0]->getSymbolPeriod());
  outputSignals[0]->setSamplingPeriod(inputSignals[0]->getSamplingPeriod());
}

bool RealToComplex::runBlock(void) {

	int ready0 = inputSignals[0]->ready();
	int ready1 = inputSignals[1]->ready();
	int ready = min(ready0, ready1);

	int space = outputSignals[0]->space();

	int process = min(ready, space);

	if (process == 0) return false;

	t_real re;
	t_real im;
	for (int i = 0; i < process; i++) {

		inputSignals[0]->bufferGet(&re);
		inputSignals[1]->bufferGet(&im);

		complex<t_real> myComplex( re, im);

		outputSignals[0]->bufferPut(myComplex);

	}

	return true;
}
//
//ComplexToReal::ComplexToReal(vector<Signal *> &InputSig, vector<Signal *> &OutputSig) {
//
//  numberOfInputSignals = 1;
//  numberOfOutputSignals = 2;
//  InputSignals = InputSig;
//  OutputSignals = OutputSig;
//
//  outputSignals[0]->period = 2*InputSignals[0]->period;
//  outputSignals[1]->period = 2*InputSignals[0]->period;
//  static_cast<TimeContinuousAmplitudeContinuousReal*>(outputSignals[0])->samplingTime =
//  2*static_cast<TimeContinuousAmplitudeContinuousComplex*>(InputSignals[0])->samplingTime;
//  static_cast<TimeContinuousAmplitudeContinuousReal*>(outputSignals[1])->samplingTime =
//  2 * static_cast<TimeContinuousAmplitudeContinuousComplex*>(InputSignals[0])->samplingTime;
//
//}
//
//bool ComplexToReal::runBlock(void){
//
//  FILE *file = fopen(outputSignals[0]->fileName, "a");
//  FILE *file2 = fopen(outputSignals[1]->fileName, "a");
//
//  int length_in = InputSignals[0]->bufferLength;
//  int length_out1 = outputSignals[0]->bufferLength;
//  int length_out2 = outputSignals[1]->bufferLength;
//
//  double *auxil1 = static_cast<double*>(outputSignals[0]->buffer);
//  double *auxil2 = static_cast<double*>(outputSignals[1]->buffer);
//
//  if (InputSignals[0]->outPosition >= length_in ||
//      outputSignals[0]->inPosition >= length_out1 ||
//      outputSignals[1]->inPosition >= length_out2) {
//
//    fclose(file);
//    fclose(file2);
//    return false;
//
//  } else {
//    for (int i = InputSignals[0]->outPosition; i < length_in; i ++) {
//      if (outputSignals[0]->inPosition < length_out1 &&
//          outputSignals[1]->inPosition < length_out2) {
//
//        auxil1[i] = static_cast<complex<double>*>(InputSignals[0]->buffer)[i].real();
//        auxil2[i] = static_cast<complex<double>*>(InputSignals[0]->buffer)[i].imag();
//
//        InputSignals[0]->outPosition++;
//        outputSignals[0]->inPosition++;
//        outputSignals[1]->inPosition++;
//        outputSignals[0]->count++;
//        outputSignals[1]->count++;
//
//        // File 1
//        if ((outputSignals[0]->count >= outputSignals[0]->firstValueToBeSaved) &&
//            (outputSignals[0]->numberOfSavedValues <
//             outputSignals[0]->numberOfValuesToBeSaved)) {
//
//          fprintf(file, "%7.4f,",
//                 (static_cast<double*>(outputSignals[0]->buffer))[outputSignals[0]->inPosition - 1]);
//
//          outputSignals[0]->numberOfSavedValues++;
//
//          if (outputSignals[0]->numberOfSavedValues ==
//              outputSignals[0]->numberOfValuesToBeSaved)
//            fprintf(file, "\0");
//        }
//
//        // File 2
//        if ((outputSignals[1]->count >= outputSignals[1]->firstValueToBeSaved) &&
//            (outputSignals[1]->numberOfSavedValues <
//             outputSignals[1]->numberOfValuesToBeSaved)) {
//
//          fprintf(file2, "%7.4f,",
//                 (static_cast<double*>(outputSignals[1]->buffer))[outputSignals[1]->inPosition - 1]);
//
//          outputSignals[1]->numberOfSavedValues++;
//
//          if (outputSignals[1]->numberOfSavedValues ==
//              outputSignals[1]->numberOfValuesToBeSaved)
//            fprintf(file2, "\0");
//        }
//
//      } else {
//        break;
//      }
//    }
//
//    outputSignals[0]->outPosition = 0;
//    outputSignals[1]->outPosition = 0;
//
//    if (InputSignals[0]->outPosition == length_in) {
//      InputSignals[0]->inPosition = 0;
//      fclose(file);
//      fclose(file2);
//      return true;
//    } else {
//      fclose(file);
//      fclose(file2);
//      return false;
//    }
//  }
//
//}
//
//ContinuousToDiscreteTime::ContinuousToDiscreteTime(vector<Signal *> &InputSig, vector<Signal *> &OutputSig, double time){
//
//  numberOfInputSignals = 1;
//  numberOfOutputSignals = 1;
//  InputSignals = InputSig;
//  OutputSignals = OutputSig;
//
//  timeSampling = time;
//  outputSignals[0]->period = timeSampling;
//
//  static_cast<TimeDiscreteAmplitudeContinuousReal*>(outputSignals[0])->samplingPeriod = timeSampling;
//
//}
//
//bool ContinuousToDiscreteTime::runBlock(void){
//
//  FILE *file = fopen(outputSignals[0]->fileName, "a");
//
//  int length_in = InputSignals[0]->bufferLength;
//  int length_out = outputSignals[0]->bufferLength;
//  double nS = timeSampling / 
//              static_cast<TimeContinuousAmplitudeContinuousReal*>(InputSignals[0])->samplingTime;
//
//  int nSamples = static_cast<int>(nS);
//
//  double *auxil = static_cast<double*>(outputSignals[0]->buffer);
//
//  if (InputSignals[0]->outPosition >= length_in || outputSignals[0]->inPosition >= length_out) {
//    fclose(file);
//    return false;
//  } else {
//
//    for (int i = InputSignals[0]->outPosition; i < length_in; i += nSamples) {
//
//      if (outputSignals[0]->inPosition < length_out) {
//
//        auxil[i/nSamples] = (static_cast<double*>(InputSignals[0]->buffer))[i];
//
//        outputSignals[0]->inPosition++;
//        outputSignals[0]->count++;
//
//        // File 
//        if ((outputSignals[0]->count >= outputSignals[0]->firstValueToBeSaved) &&
//            (outputSignals[0]->numberOfSavedValues <
//             outputSignals[0]->numberOfValuesToBeSaved)) {
//
//            fprintf(file, "%6.3f,",
//            (static_cast<double*>(outputSignals[0]->buffer))[outputSignals[0]->inPosition - 1]);
//
//            outputSignals[0]->numberOfSavedValues++;
//
//            if (outputSignals[0]->numberOfSavedValues ==
//                outputSignals[0]->numberOfValuesToBeSaved)
//              fprintf(file, "\0");
//         }
//
//         InputSignals[0]->outPosition += nSamples;
//
//       } else {
//        break;
//      }
//    }
//
//    outputSignals[0]->outPosition = 0;
//
//    if (InputSignals[0]->outPosition == length_in) {
//      InputSignals[0]->inPosition = 0;
//      fclose(file);
//      return true;
//    } else {
//      fclose(file);
//      return false;
//    }
//  }
//
//}
//


//########################################################################################################################################################
//########################################################## GENERIC SYSTEM DECLARATIONS AND DEFINITIONS #################################################
//########################################################################################################################################################



System::System(vector<Block *> &Blocks) 
{
	SystemBlocks = Blocks;

	for (int unsigned i = 0; i < SystemBlocks.size(); i++) 
	{
		SystemBlocks[i]->initializeBlock();
	}
}

void System::run() {

	/*2016-08-02
	for (int unsigned i = 0; i < SystemBlocks.size(); i++) {
		for (int unsigned j = 0; j<(SystemBlocks[i]->inputSignals).size(); j++) {
			(SystemBlocks[i]->inputSignals[j])->writeHeader();
		}
	}
	*/

	/*2016-08-02
	for (int unsigned i = 0; i < SystemBlocks.size(); i++) {
		SystemBlocks[i]->initializeBlock();
	}
	*/
	int l = 0;
	bool Alive;
	do {
		Alive = false;
		for (unsigned int i = 0; i < SystemBlocks.size(); i++) {

			bool aux = SystemBlocks[i]->runBlock();

			Alive = (Alive || aux);
		}
		l++;
	} while (Alive);

	for (int unsigned i = 0; i < SystemBlocks.size(); i++) {		
		SystemBlocks[i]->terminateBlock();
	}
}

void System::run(string signalPath) {

	
	// The signalPath sub-folder must already exist
	for (int unsigned i = 0; i < SystemBlocks.size(); i++) {
		for (int unsigned j = 0; j<(SystemBlocks[i]->inputSignals).size(); j++) {
			(SystemBlocks[i]->inputSignals[j])->writeHeader(signalPath);
		}
	}

	bool alive;
	do {
		alive = false;
		for (unsigned int i = 0; i < SystemBlocks.size(); i++) {
			alive = alive || SystemBlocks[i]->runBlock();
		}
	} while (alive);

	for (int unsigned i = 0; i < SystemBlocks.size(); i++) {
		SystemBlocks[i]->terminateBlock();
	}
}


//########################################################################################################################################################
//############################################################### GENERIC DSP FUNCTIONS ##################################################################
//########################################################################################################################################################


void OverlapMethod::overlapSaveSyRealIn(vector<double> &v_in, vector<double> &v_out, vector<double> Hf, int NFFT) {

	int Nblocks = 2 * ((int) v_in.size() / NFFT);

	vector<double> var_temp(NFFT, 0);
	vector<double> real_temp_copy(NFFT, 0);
	vector<double> imag_temp_copy(NFFT, 0);
	vector<double> H_real(NFFT, 0);
	vector<double> H_imag(NFFT, 0);

	Fft fft;
	ComplexMult CMult;

	H_real = Hf;
	fft.directTransform(H_real, H_imag);

	for (int k = 0; k < Nblocks; k++) {

		if (k == Nblocks - 1) 
		{
			copy(v_in.begin(), v_in.begin() + (NFFT / 2), var_temp.begin() + (NFFT / 2));
			copy(v_in.end() - (NFFT / 2), v_in.end(), var_temp.begin());
		}
		else
		{
			copy(v_in.begin() + k*(NFFT / 2), v_in.begin() + ((k + 1)*NFFT - k*(NFFT / 2)), var_temp.begin());
		}
		real_temp_copy = var_temp;

		// Computation of FFT of each block

		fft.directTransform(real_temp_copy, imag_temp_copy);

		// Multiplication with the transfer function
		CMult.CMultVector_Loop(real_temp_copy, imag_temp_copy, H_real, H_imag);

		// Computation of IFFT of each block
		fft.inverseTransform(real_temp_copy, imag_temp_copy);



		// Removing the samples symetrically and assign to the output
		if (k == Nblocks - 1) {
			copy(real_temp_copy.begin() + (NFFT / 4), real_temp_copy.begin() + (NFFT / 2), v_out.end() - (NFFT / 4));
			copy(real_temp_copy.begin() + (NFFT / 2), real_temp_copy.begin() + 3 * NFFT / 4, v_out.begin());
		}
		else {
			copy(real_temp_copy.begin() + (NFFT / 4), real_temp_copy.end() - (NFFT / 4), v_out.begin() + ((NFFT / 4) + k*(NFFT / 2)));
		}
	}

}

// Private function prototypes
static size_t reverseBits(size_t x, unsigned int n);

vector<complex <double>> Fft::directTransformInReal(std::vector<double> real)
{
	//if (In.real.size() != imag.size())
	//throw "Mismatched lengths";
	ComplexMult CMult;
	vector<double> im(real.size(), 0);
	vector<complex <double>> v_out(real.size(), 0);
	size_t n = real.size();

	if (n == 0)
		return v_out;
	else if ((n & (n - 1)) == 0)  // Is power of 2
		transformRadix2(real, im);
	else  // More complicated algorithm for arbitrary sizes
		transformBluestein(real, im);

	CMult.ReImVect2ComplexVect(real, im, v_out);
	return v_out;
}

std::vector<double> Fft::inverseTransformInCP(std::vector<complex <double>> &In)
{
	ComplexMult CMult;
	vector<double> real(In.size(), 0);
	vector<double> im(In.size(), 0);
	CMult.ComplexVect2ReImVect(In, real, im);
	directTransform(im, real);
	for (int x = 0; x != real.size(); ++x)
	{
		real[x] = real[x] / real.size();
		  im[x] =   im[x] / real.size();
	}

	vector<double> v_out(real.size(), 0);
	v_out = real;
	//CMult.ReImVect2ComplexVect(real, imag, v_out);

	return v_out;
}

void Fft::directTransform(vector<double> &real, vector<double> &imag)
{
	if (real.size() != imag.size())
		throw "Mismatched lengths";

	size_t n = real.size();
	if (n == 0)
		return;
	else if ((n & (n - 1)) == 0)			// Is power of 2
		transformRadix2(real, imag);
	else									// More complicated algorithm for arbitrary sizes
		transformBluestein(real, imag);
}


void Fft::inverseTransform(vector<double> &real, vector<double> &imag)
{
	directTransform(imag, real);					// Inverse function
	for (int x = 0; x != real.size(); ++x)
	{
		real[x] = real[x] / real.size();			// Normalize
		imag[x] = imag[x] / real.size();
	}
}


void Fft::transformRadix2(vector<double> &real, vector<double> &imag) 
{
	// Compute levels = floor(log2(n))
	if (real.size() != imag.size())
		throw "Mismatched lengths";
	size_t n = real.size();
	unsigned int levels;
	{
		size_t temp = n;
		levels = 0;
		while (temp > 1)
		{
			levels++;
			temp >>= 1;			// temp = temp >> 1 (bit wise right shift)
		}
		if (1u << levels != n)
			throw "Length is not a power of 2";
	}

	// Trignometric tables
	vector<double> cosTable(n / 2);
	vector<double> sinTable(n / 2);
	for (size_t i = 0; i < n / 2; i++)
	{
		cosTable[i] = cos(2 * M_PI * i / n);
		sinTable[i] = sin(2 * M_PI * i / n);
	}

	// Bit-reversed addressing permutation
	for (size_t i = 0; i < n; i++) 
	{
		size_t j = reverseBits(i, levels);
		if (j > i) 
		{
			double temp = real[i];
			real[i] = real[j];
			real[j] = temp;
			temp = imag[i];
			imag[i] = imag[j];
			imag[j] = temp;
		}
	}

	// Cooley-Tukey decimation-in-time radix-2 FFT
	for (size_t size = 2; size <= n; size *= 2) 
	{
		size_t halfsize = size / 2;
		size_t tablestep = n / size;
		for (size_t i = 0; i < n; i += size) 
		{
			for (size_t j = i, k = 0; j < i + halfsize; j++, k += tablestep)
			{
				double tpre = real[j + halfsize] * cosTable[k] + imag[j + halfsize] * sinTable[k];
				double tpim = -real[j + halfsize] * sinTable[k] + imag[j + halfsize] * cosTable[k];
				real[j + halfsize] = real[j] - tpre;
				imag[j + halfsize] = imag[j] - tpim;
				real[j] += tpre;
				imag[j] += tpim;
			}
		}
		if (size == n)  // Prevent overflow in 'size *= 2'
			break;
	}
}









void Fft::Radix2(vector<double> &real, vector<double> &imag, int s)
{
	// Compute levels = floor(log2(n))
	if (real.size() != imag.size())
		throw "Mismatched lengths";
	size_t n = real.size();
	unsigned int levels;
	{
		size_t temp = n;
		levels = 0;
		while (temp > 1)
		{
			levels++;
			temp >>= 1;			// temp = temp >> 1 (bit wise right shift)
		}
		if (1u << levels != n)
			throw "Length is not a power of 2";
	}

	// Trignometric tables
	vector<double> cosTable(n / 2);
	vector<double> sinTable(n / 2);
	for (size_t i = 0; i < n / 2; i++)
	{
		cosTable[i] = cos(-s*2 * M_PI * i / n);
		sinTable[i] = sin(-s*2 * M_PI * i / n);
	}

	// Bit-reversed addressing permutation
	for (size_t i = 0; i < n; i++)
	{
		size_t j = reverseBits(i, levels);
		if (j > i)
		{
			double temp = real[i];
			real[i] = real[j];
			real[j] = temp;
			temp = imag[i];
			imag[i] = imag[j];
			imag[j] = temp;
		}
	}

	// Cooley-Tukey decimation-in-time radix-2 FFT
	for (size_t size = 2; size <= n; size *= 2)
	{
		size_t halfsize = size / 2;
		size_t tablestep = n / size;
		for (size_t i = 0; i < n; i += size)
		{
			for (size_t j = i, k = 0; j < i + halfsize; j++, k += tablestep)
			{
				double tpre = real[j + halfsize] * cosTable[k] + imag[j + halfsize] * sinTable[k];
				double tpim = -real[j + halfsize] * sinTable[k] + imag[j + halfsize] * cosTable[k];
				real[j + halfsize] = real[j] - tpre;
				imag[j + halfsize] = imag[j] - tpim;
				real[j] += tpre;
				imag[j] += tpim;
			}
		}
		if (size == n)  // Prevent overflow in 'size *= 2'
			break;
	}
}


void Fft::transformBluestein(vector<double> &real, vector<double> &imag) 
{
	// Find a power-of-2 convolution length m such that m >= n * 2 + 1
	if (real.size() != imag.size())
		throw "Mismatched lengths";
	size_t n = real.size();
	size_t m;
	{
		size_t target;
		if (n > (SIZE_MAX - 1) / 2)
			throw "Vector too large";
		target = n * 2 + 1;
		for (m = 1; m < target; m *= 2) 
		{
			if (SIZE_MAX / 2 < m)
				throw "Vector too large";
		}
	}

	// Trignometric tables
	vector<double> cosTable(n), sinTable(n);
	for (size_t i = 0; i < n; i++) {
		double temp = M_PI * (size_t)((unsigned long long)i * i % ((unsigned long long)n * 2)) / n;
		// Less accurate version if long long is unavailable: double temp = M_PI * i * i / n;
		cosTable[i] = cos(temp);
		sinTable[i] = sin(temp);
	}

	// Temporary vectors and preprocessing
	vector<double> areal(m), aimag(m);
	for (size_t i = 0; i < n; i++) {
		areal[i] = real[i] * cosTable[i] + imag[i] * sinTable[i];
		aimag[i] = -real[i] * sinTable[i] + imag[i] * cosTable[i];
	}
	vector<double> breal(m), bimag(m);
	breal[0] = cosTable[0];
	bimag[0] = sinTable[0];
	for (size_t i = 1; i < n; i++) {
		breal[i] = breal[m - i] = cosTable[i];
		bimag[i] = bimag[m - i] = sinTable[i];
	}

	// Convolution
	vector<double> creal(m), cimag(m);
	convolve(areal, aimag, breal, bimag, creal, cimag);

	// Postprocessing
	for (size_t i = 0; i < n; i++) {
		real[i] = creal[i] * cosTable[i] + cimag[i] * sinTable[i];
		imag[i] = -creal[i] * sinTable[i] + cimag[i] * cosTable[i];
	}
}




void Fft::Bluestein(vector<double> &real, vector<double> &imag, int s)
{
	// Find a power-of-2 convolution length m such that m >= n * 2 + 1
	if (real.size() != imag.size())
		throw "Mismatched lengths";
	size_t n = real.size();
	size_t m;
	{
		size_t target;
		if (n > (SIZE_MAX - 1) / 2)
			throw "Vector too large";
		target = n * 2 + 1;
		for (m = 1; m < target; m *= 2)
		{
			if (SIZE_MAX / 2 < m)
				throw "Vector too large";
		}
	}

	// Trignometric tables
	vector<double> cosTable(n), sinTable(n);
	for (size_t i = 0; i < n; i++) {
		double temp = M_PI * (size_t)((unsigned long long)i * i % ((unsigned long long)n * 2)) / n;
		// Less accurate version if long long is unavailable: double temp = M_PI * i * i / n;
		cosTable[i] =    cos(-s*temp);
		sinTable[i] =    sin(-s*temp);
	}

	// Temporary vectors and preprocessing
	vector<double> areal(m), aimag(m);
	for (size_t i = 0; i < n; i++) {
		areal[i] = real[i] * cosTable[i] + imag[i] * sinTable[i];
		aimag[i] = -real[i] * sinTable[i] + imag[i] * cosTable[i];
	}
	vector<double> breal(m), bimag(m);
	breal[0] = cosTable[0];
	bimag[0] = sinTable[0];
	for (size_t i = 1; i < n; i++) {
		breal[i] = breal[m - i] = cosTable[i];
		bimag[i] = bimag[m - i] = sinTable[i];
	}

	// Convolution
	vector<double> creal(m), cimag(m);
	convolve(areal, aimag, breal, bimag, creal, cimag);

	// Postprocessing
	for (size_t i = 0; i < n; i++) {
		real[i] = creal[i] * cosTable[i] + cimag[i] * sinTable[i];
		imag[i] = -creal[i] * sinTable[i] + cimag[i] * cosTable[i];
	}
}















static size_t reverseBits(size_t x, unsigned int n)
{
	size_t result = 0;
	unsigned int i;
	for (i = 0; i < n; i++, x >>= 1)
		result = (result << 1) | (x & 1);
	return result;
}


void Fft::convolve(const vector<double> &x, const vector<double> &y, vector<double> &out) {
	if (x.size() != y.size() || x.size() != out.size())
		throw "Mismatched lengths";
	size_t n = x.size();
	vector<double> ximag(n), yimag(n), zimag(n);
	convolve(x, ximag, y, yimag, out, zimag);
}


void Fft::convolve(const vector<double> &xreal, const vector<double> &ximag, const vector<double> &yreal, const vector<double> &yimag, vector<double> &outreal, vector<double> &outimag) {
	if (xreal.size() != ximag.size() || xreal.size() != yreal.size() || yreal.size() != yimag.size() || xreal.size() != outreal.size() || outreal.size() != outimag.size())
		throw "Mismatched lengths";

	size_t n = xreal.size();
	vector<double> xr(xreal);
	vector<double> xi(ximag);
	vector<double> yr(yreal);
	vector<double> yi(yimag);

	directTransform(xr, xi);
	directTransform(yr, yi);
	for (size_t i = 0; i < n; i++) {
		double temp = xr[i] * yr[i] - xi[i] * yi[i];
		xi[i] = xi[i] * yr[i] + xr[i] * yi[i];
		xr[i] = temp;
	}
	inverseTransform(xr, xi);
	for (size_t i = 0; i < n; i++) {  // Scaling (because this FFT implementation omits it)
		outreal[i] = xr[i] / n;
		outimag[i] = xi[i] / n;
	}
}

void ComplexMult::CMultVector(vector<double> &v1_real, vector<double> &v1_imag, vector<double> v2_real, vector<double> v2_imag) {


	vector<double> ac_v(v1_real.size(), 0);
	vector<double> bd_v(v1_real.size(), 0);
	vector<double> bc_v(v1_real.size(), 0);
	vector<double> ad_v(v1_real.size(), 0);

	transform(v1_real.begin(), v1_real.end(), v2_real.begin(), ac_v.begin(), multiplies<double>()); // ac

	transform(v1_imag.begin(), v1_imag.end(), v2_imag.begin(), bd_v.begin(), multiplies<double>()); // bd

	transform(v1_imag.begin(), v1_imag.end(), v2_real.begin(), bc_v.begin(), multiplies<double>()); // bc

	transform(v1_real.begin(), v1_real.end(), v2_imag.begin(), ad_v.begin(), multiplies<double>()); // ad

	transform(ac_v.begin(), ac_v.end(), bd_v.begin(), v1_real.begin(), minus<double>()); // ac - bd

	transform(bc_v.begin(), bc_v.end(), ad_v.begin(), v1_imag.begin(), plus<double>()); // bc + ad

}

void ComplexMult::CMultVector_Loop(vector<double> &v1_real, vector<double> &v1_imag, vector<double> v2_real, vector<double> v2_imag) {


	double Real_temp = 0.0;
	double Imag_temp = 0.0;

	for (size_t k = 0; k < v1_real.size(); ++k)
	{

		Real_temp = v1_real.at(k)*v2_real.at(k) - v1_imag.at(k)*v2_imag.at(k);
		Imag_temp = v1_imag.at(k)*v2_real.at(k) + v1_real.at(k)*v2_imag.at(k);

		v1_real.at(k) = Real_temp;
		v1_imag.at(k) = Imag_temp;
	}

}

void ComplexMult::CMultVector_InComplex(vector<complex <double>> &v1_in, vector<complex <double>> &v2_in) {

	for (size_t k = 0; k < v1_in.size(); ++k)
	{
		v1_in.at(k) = v1_in.at(k)*v2_in.at(k);

	}

}

vector<complex <double>> ComplexMult::CMultVectorInCP(vector<complex <double>> &v1_in, vector<complex <double>> &v2_in) {

	vector<complex <double>> v_out(v1_in.size(), 0);
	for (unsigned int k = 0; k < v1_in.size(); ++k)
	{
		v_out.at(k) = v1_in.at(k)*v2_in.at(k);
	}
	return v_out;
}

void ComplexMult::ComplexVect2ReImVect(vector<complex <double>> &v_in, vector<double> &v1_real, vector<double> &v1_imag) {

	for (size_t k = 0; k < v_in.size(); k++) {
		v1_real.at(k) = real(v_in.at(k));
		v1_imag.at(k) = imag(v_in.at(k));
	}

}

void ComplexMult::ReImVect2ComplexVect(vector<double> &v1_real, vector<double> &v1_imag, vector<complex <double>> &v_out) {

	for (size_t i = 0; i < v1_real.size(); ++i) {
		complex<double> iNum(v1_real[i], v1_imag[i]);
		v_out.at(i) = iNum;
	}

}


vector<complex<double>> ComplexMult::ReImVect2ComplexVector(vector<double> &v1_real, vector<double> &v1_imag)
{
	vector<complex<double>> v_out(v1_real.size());

	for (size_t i = 0; i < v1_real.size(); ++i)
	{
		complex<double> iNum(v1_real[i], v1_imag[i]);
		v_out.at(i) = iNum;
	}

	return v_out;
}

vector<complex <double>> ComplexMult::complexVectorMultiplication(vector<complex <double>> &v1_in, vector<complex <double>> &v2_in)
{

	vector<complex <double>> v_out(v1_in.size(), 0);
	for (unsigned int k = 0; k < v1_in.size(); ++k)
	{
		v_out.at(k) = v1_in.at(k)*v2_in.at(k);
	}
	return v_out;
}



////////////  Fast FourierTransform  /////////////// 

vector <complex<double>> FourierTransform::transform(vector<complex<double>>IN, int m)
{
	Fft F;										// Various function for FT
	ComplexMult C;					        // Complex data functionality like split, addition, multiplication etc.
	size_t n = IN.size();						// Size of the vector

	vector <complex<double>> OUT(n);
	vector<double> re(n, 0);
	vector<double> im(n, 0);

	C.ComplexVect2ReImVect(IN, re, im);    // Here we have splitted real and imag data from IN.

	if (n == 0)
		return OUT;
	else if ((n & (n - 1)) == 0)				// Is power of 2 : Radix-2 Algorithim
		F.Radix2(re, im, m);
	else										// More complicated algorithm for arbitrary sizes : Bluestein Algorithim
		F.Bluestein(re, im, m);

	for (unsigned int i = 0; i<re.size(); i++)		    // Devide by the square root of "N"
	{
		re[i] = re[i] / sqrt(re.size());
		im[i] = im[i] / sqrt(re.size());
	}

	C.ReImVect2ComplexVect(re, im, OUT);

	return OUT;
};

