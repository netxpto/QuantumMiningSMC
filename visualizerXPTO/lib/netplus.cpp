# include <complex>
# include <fstream>
# include <iostream>
# include <math.h>
# include <stdio.h>
# include <string>
# include <strstream>
# include <algorithm>


# include "netplus.h"


using namespace std;

//########################################################################################################################################################
//######################################################### SIGNALS FUNCTIONS IMPLEMENTATION #############################################################
//########################################################################################################################################################

void Signal::close() {

	if (saveSignal && (inPosition >= firstValueToBeSaved)) {
		char *ptr = (char *)buffer;


		ofstream fileHandler;
		fileHandler.open("./signals/" + fileName, ios::out | ios::binary | ios::app);
		
		if (type == "Binary") {
			ptr = ptr + (firstValueToBeSaved - 1)*sizeof(t_binary);
			fileHandler.write((char *)ptr, (inPosition - (firstValueToBeSaved - 1))*sizeof(t_binary));
		}
		else if (type == "TimeContinuousAmplitudeContinuousComplex") {
			ptr = ptr + (firstValueToBeSaved - 1)*sizeof(t_complex);
			fileHandler.write((char *)ptr, (inPosition - (firstValueToBeSaved - 1))*sizeof(t_complex));
		}
		else {
			ptr = ptr + (firstValueToBeSaved - 1)*sizeof(t_real);
			fileHandler.write((char *)ptr, (inPosition - (firstValueToBeSaved - 1))*sizeof(t_real));
		}

		fileHandler.close();
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
			return (bufferLength - outPosition + inPosition + 1);
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


//########################################################################################################################################################
//###################################################### GENERAL BLOCKS FUNCTIONS IMPLEMENTATION #########################################################
//########################################################################################################################################################

Block::Block(vector<Signal*> &InputSig, vector<Signal*> &OutputSig) {

	numberOfInputSignals = InputSig.size();
	numberOfOutputSignals = OutputSig.size();

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

			t_complex signalValue;
			for (int j = 0; j < length; j++) {
				moduleBlocks[moduleBlocks.size() - 1]->outputSignals[i]->bufferGet(&signalValue);
				outputSignals[i]->bufferPut(signalValue);
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
		moduleBlocks[moduleBlocks.size() - 1]->outputSignals[0]->close();

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

	outputSignals[0]->symbolPeriod = inputSignals[0]->symbolPeriod;
	outputSignals[0]->samplingPeriod = inputSignals[0]->samplingPeriod;
	outputSignals[0]->samplesPerSymbol = inputSignals[0]->samplesPerSymbol;

	if (!getSeeBeginningOfImpulseResponse()) {
		int aux = (int)(((double)impulseResponseLength) / 2) + 1;
		outputSignals[0]->setFirstValueToBeSaved(aux);
	}

	delayLine.resize(impulseResponseLength, 0);

	if (saveImpulseResponse) {
		ofstream fileHandler("./signals/" + impulseResponseFilename, ios::out);
		fileHandler << "// ### HEADER TERMINATOR ###\n";

		t_real t;
		double samplingPeriod = inputSignals[0]->samplingPeriod;
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

  numberOfInputSignals = InputSig.size();
  numberOfOutputSignals = OutputSig.size();

  inputSignals = InputSig;
  outputSignals = OutputSig;

  outputSignals[0]->symbolPeriod = inputSignals[0]->symbolPeriod;
  outputSignals[0]->samplingPeriod = inputSignals[0]->samplingPeriod;
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



System::System(vector<Block *> &Blocks) {

	SystemBlocks = Blocks;

	for (int unsigned i = 0; i < SystemBlocks.size(); i++) {
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

	bool Alive;
	do {
		Alive = false;
		for (unsigned int i = 0; i < SystemBlocks.size(); i++) {
			bool aux = SystemBlocks[i]->runBlock();
			Alive = (Alive || aux);
		}
	} while (Alive);

	for (int unsigned i = 0; i < SystemBlocks.size(); i++) {
		SystemBlocks[i]->terminateBlock();
	}
}

void System::run(string signalPath) {

	
	// The signalPath sub-folder must already exists
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

