# ifndef PROGRAM_INCLUDE_NETXPTO_H_
# define PROGRAM_INCLUDE_NETXPTO_H_

# include <iostream>
# include <fstream>
# include <vector>
# include <string>
# include <complex>
# include <algorithm>	// bind1st
# include <functional>	// bind1st


const int MAX_NAME_SIZE = 256;					// Maximum size of names
const long int MAX_Sink_LENGTH = 100000;		// Maximum Sink Block number of values
const int MAX_BUFFER_LENGTH = 10000;			// Maximum Signal buffer length
const int DEFAULT_BUFFER_LENGTH = 512;			// Default buffer length
const int MAX_TOPOLOGY_SIZE = 100;				// Maximum System topology size 
const int MAX_TAPS = 1000;						// Maximum Taps Number
const double PI = 3.1415926535897932384;		// Value of pi
const double SPEED_OF_LIGHT = 299792458;		// Speed of light in vaccum
const double PLANCK_CONSTANT = 6.626070040e-34; // NIST
const int MAX_NUMBER_OF_PATHS = 2;

using namespace std;


typedef int t_integer;
typedef unsigned int t_uinteger;
typedef double t_real;
typedef complex<t_real> t_complex;
typedef struct { t_complex x; t_complex y; } t_complex_xy;
typedef struct { t_real probabilityAmplitude;  t_real polarization; } t_photon;
typedef struct { t_photon path[MAX_NUMBER_OF_PATHS]; } t_photon_mp;
typedef complex<t_real> t_iqValues;
typedef struct { string fieldName; string fieldValue; } t_message_field;
typedef vector<t_message_field> t_message;

typedef unsigned int t_binary;

enum signal_value_type {BinaryValue, IntegerValue, UIntegerValue, RealValue, ComplexValue, ComplexValueXY, PhotonValue, PhotonValueMP, Message};


//########################################################################################################################################################
//############################################################## SIGNALS DECLARATION AND DEFINITION ######################################################
//########################################################################################################################################################

// Root class for signals
template<class T>
class Signal {

public:

	/* Signal constructor */
	Signal() : signalBuffer(vector<T>(DEFAULT_BUFFER_LENGTH)) {};
	Signal(string fName) : signalBuffer(vector<T>(DEFAULT_BUFFER_LENGTH)), fileName{ fName } {};
	Signal(int bLength) : signalBuffer(vector<T>(bLength)) {};
	Signal(string fName, int bLength) : signalBuffer(vector<T>(bLength)), fileName{ fName } {};

	/* Signal destructor */
	~Signal() {};					

	/* Methods */

	void close();									// Empty the signal buffer and close the signal file
	int space();									// Returns the signal buffer space
	int ready();									// Returns the number of samples in the buffer ready to be processed
	void writeHeader();								// Opens the signal file in the default signals directory, \signals, and writes the signal header
	void writeHeader(string signalPath);						// Opens the signal file in the signalPath directory, and writes the signal header

	void setBuffer(void* ptr) { buffer = ptr; return; };
	void* getBuffer() { return buffer; };

	void bufferPut(T value) {							// Puts a value in the buffer
		(static_cast<T *>(buffer))[inPosition] = value;
		if (bufferEmpty) bufferEmpty = false;
		inPosition++;
		if (inPosition == bufferLength) {
			inPosition = 0;
			if (saveSignal) {
				if (firstValueToBeSaved <= bufferLength) {
					char *ptr = (char *)buffer;
					ptr = ptr + (firstValueToBeSaved - 1) * sizeof(T);
					ofstream fileHandler("./" + folderName + "/" + fileName, ios::out | ios::binary | ios::app);
					int auxSizeOf = sizeof(T);
					fileHandler.write(ptr, (bufferLength - (firstValueToBeSaved - 1)) * sizeof(T));
					fileHandler.close();
					firstValueToBeSaved = 1;
				}
				else {
					firstValueToBeSaved = firstValueToBeSaved - bufferLength;
				}
			}
		}
		if (inPosition == outPosition) bufferFull = true;
	};

	void virtual bufferGet();
	void virtual bufferGet(t_binary *valueAddr);
	void virtual bufferGet(t_integer *valueAddr);
	void virtual bufferGet(t_real *valueAddr);
	void virtual bufferGet(t_complex *valueAddr);
	void virtual bufferGet(t_complex_xy *valueAddr);
	void virtual bufferGet(t_photon *valueAddr);
	void virtual bufferGet(t_photon_mp *valueAddr);
	void virtual bufferGet(t_message *valueAdr);

	void setSaveSignal(bool sSignal) { saveSignal = sSignal; };
	bool const getSaveSignal() { return saveSignal; };

	void setType(string sType, signal_value_type vType) { type = sType; valueType = vType; };
	void setType(string sType) { type = sType; };
	string getType() { return type; };

	void setInPosition(int iPosition) { inPosition = iPosition; };
	int getInPosition() { return inPosition; };

	void setOutPosition(int oPosition) { outPosition = oPosition; };
	int getOutPosition() { return outPosition; };

	void setBufferEmpty(bool bEmpty) { bufferEmpty = bEmpty; };
	bool getBufferEmpty() { return bufferEmpty; };

	void setBufferFull(bool bFull) { bufferFull = bFull; };
	bool getBufferFull() { return bufferFull; };


	void setValueType(signal_value_type vType) { valueType = vType; };
	signal_value_type getValueType() { return valueType; };

	void setFileName(string fName) { fileName = fName; };
	string getFileName() { return fileName; };

	void setFolderName(string fName) { folderName = fName; };
	string getFolderName() { return folderName; };

	void setBufferLength(int bLength) { bufferLength = bLength; signalBUffer.resize(bLength); };
	int getBufferLength() { return bufferLength; };

	void setFirstValueToBeSaved(long int fValueToBeSaved) { firstValueToBeSaved = fValueToBeSaved; };
	long int getFirstValueToBeSaved() { return firstValueToBeSaved; };

	void setNumberOfValuesToBeSaved(long int nOfValuesToBeSaved) { numberOfValuesToBeSaved = nOfValuesToBeSaved; };
	long int getNumberOfValuesToBeSaved() { return numberOfValuesToBeSaved; };

	void setSymbolPeriod(double sPeriod) { symbolPeriod = sPeriod; samplesPerSymbol = symbolPeriod / samplingPeriod; };
	double getSymbolPeriod() { return symbolPeriod; };

	void setSamplingPeriod(double sPeriod) { samplingPeriod = sPeriod; samplesPerSymbol = symbolPeriod / samplingPeriod; };
	double getSamplingPeriod() { return samplingPeriod; };

	void setSamplesPerSymbol(double sPeriod) { samplesPerSymbol = sPeriod; symbolPeriod = samplesPerSymbol * samplingPeriod; samplingPeriod = symbolPeriod / samplesPerSymbol; };
	double getSamplesPerSymbol() { return samplesPerSymbol; };

	void setCentralFrequency(double cFrequency) { centralFrequency = cFrequency; centralWavelength = SPEED_OF_LIGHT / centralFrequency; }
	double getCentralFrequency() { return centralFrequency; };

	void setCentralWavelength(double cWavelength) { centralWavelength = cWavelength; centralFrequency = SPEED_OF_LIGHT / centralWavelength; }
	double getCentralWavelength() { return centralWavelength; }

private:

	/* State Variables */
	vector<T> SignalBuffer;

	vector<T> *buffer{ &SignalBuffer };						// Pointer to Signalbuffer
	int inPosition{ 0 };							// Next position for the buffer input values
	int outPosition{ 0 };							// Next position for the buffer output values
	bool bufferEmpty{ true };						// Flag bufferEmpty
	bool bufferFull{ false };						// Flag bufferFull
	long int count;									// Number of values that have already entered in the buffer

	long int numberOfSavedValues{ 0 };				// Number of saved values

	/* Parameters */

	long int firstValueToBeSaved{ 1 };				// First value (>= 1) to be saved
	bool saveSignal{ true };

	string type;									// Signal type
	signal_value_type valueType;					// Signal samples type

	string fileName{ "" };							// Name of the file where data values are going to be saved
	string folderName{ "signals" };					// folder where signals are going to be saved by default

	long int numberOfValuesToBeSaved{ -1 };			// Number of values to be saved, if -1 all values are going to be saved

	int bufferLength{ DEFAULT_BUFFER_LENGTH };		// Buffer length

	double symbolPeriod{ 1.0 };						// Signal symbol period (it is the inverse of the symbol rate)
	double samplingPeriod{ 1.0 };					// Signal sampling period (it is the time space between two samples)
	double samplesPerSymbol{ 1.0 };					// samplesPerSymbol = symbolPeriod / samplingPeriod;

	double centralWavelength{ 1550E-9 };
	double centralFrequency{ SPEED_OF_LIGHT / centralWavelength };

};


/* TimeDiscreteAmplitudeDiscrete*/

class TimeDiscreteAmplitudeDiscreteInteger : public Signal<t_integer> {
public:
	TimeDiscreteAmplitudeDiscreteInteger() { setType("TimeDiscreteAmplitudeDiscreteInteger", IntegerValue); };
	TimeDiscreteAmplitudeDiscreteInteger(string fName) : Signal(fName) { setType("TimeDiscreteAmplitudeDiscreteInteger", IntegerValue); };
	TimeDiscreteAmplitudeDiscreteInteger(int bLength) : Signal(bLength) { setType("TimeDiscreteAmplitudeDiscreteInteger", IntegerValue); };
	TimeDiscreteAmplitudeDiscreteInteger(string fName, int bLength) : Signal(fName, bLength) { setType("TimeDiscreteAmplitudeDiscreteInteger", IntegerValue); };
};

class TimeDiscreteAmplitudeDiscreteUInteger : public Signal<t_uinteger> {
public:
	TimeDiscreteAmplitudeDiscreteUInteger() { setType("TimeDiscreteAmplitudeDiscreteUInteger", UIntegerValue); };
	TimeDiscreteAmplitudeDiscreteUInteger(string fName) : Signal(fName) { setType("TimeDiscreteAmplitudeDiscreteUInteger", UIntegerValue); };
	TimeDiscreteAmplitudeDiscreteUInteger(int bLength) : Signal(bLength) { setType("TimeDiscreteAmplitudeDiscreteUInteger", UIntegerValue); };
	TimeDiscreteAmplitudeDiscreteUInteger(string fName, int bLength) : Signal(fName, bLength) { setType("TimeDiscreteAmplitudeDiscreteUInteger", UIntegerValue); };
};

class TimeDiscreteAmplitudeDiscreteReal : public Signal<t_real> {
public:
	TimeDiscreteAmplitudeDiscreteReal() { setType("TimeDiscreteAmplitudeDiscreteReal", RealValue); };
	TimeDiscreteAmplitudeDiscreteReal(string fName) : Signal(fName) { setType("TimeDiscreteAmplitudeDiscreteReal", RealValue); };
	TimeDiscreteAmplitudeDiscreteReal(int bLength) : Signal(bLength) { setType("TimeDiscreteAmplitudeDiscreteReal", RealValue); };
	TimeDiscreteAmplitudeDiscreteReal(string fName, int bLength) : Signal(fName, bLength) { setType("TimeDiscreteAmplitudeDiscreteReal", RealValue); };
};

class TimeDiscreteAmplitudeDiscreteComplex : public Signal<t_complex> {
public:
	TimeDiscreteAmplitudeDiscreteComplex() { setType("TimeDiscreteAmplitudeDiscreteComplex", ComplexValue); };
	TimeDiscreteAmplitudeDiscreteComplex(string fName) : Signal(fName) { setType("TimeDiscreteAmplitudeDiscreteComplex", ComplexValue); };
	TimeDiscreteAmplitudeDiscreteComplex(int bLength) : Signal(bLength) { setType("TimeDiscreteAmplitudeDiscreteComplex", ComplexValue); };
	TimeDiscreteAmplitudeDiscreteComplex(string fName, int bLength) : Signal(fName, bLength) { setType("TimeDiscreteAmplitudeDiscreteComplex", ComplexValue); };
};

class TimeDiscreteAmplitudeDiscreteComplexXY : public Signal<t_complex_xy> {
public:
	TimeDiscreteAmplitudeDiscreteComplexXY() { setType("TimeDiscreteAmplitudeDiscreteComplexXY", ComplexValueXY); };
	TimeDiscreteAmplitudeDiscreteComplexXY(string fName) : Signal(fName) { setType("TimeDiscreteAmplitudeDiscreteComplexXY", ComplexValueXY); };
	TimeDiscreteAmplitudeDiscreteComplexXY(int bLength) : Signal(bLength) { setType("TimeDiscreteAmplitudeDiscreteComplexXY", ComplexValueXY); };
	TimeDiscreteAmplitudeDiscreteComplexXY(string fName, int bLength) : Signal(fName, bLength) { setType("TimeDiscreteAmplitudeDiscreteComplexXY", ComplexValueXY); };
};

/* TimeContinuousAmplitudeDiscrete*/

class TimeContinuousAmplitudeDiscreteInteger : public Signal<t_integer> {
public:
	TimeContinuousAmplitudeDiscreteInteger() { setType("TimeContinuousAmplitudeDiscreteInteger", IntegerValue); };
	TimeContinuousAmplitudeDiscreteInteger(string fName) : Signal(fName) { setType("TimeContinuousAmplitudeDiscreteInteger", IntegerValue); };
	TimeContinuousAmplitudeDiscreteInteger(int bLength) : Signal(bLength) { setType("TimeContinuousAmplitudeDiscreteInteger", IntegerValue); };
	TimeContinuousAmplitudeDiscreteInteger(string fName, int bLength) : Signal(fName, bLength) { setType("TimeContinuousAmplitudeDiscreteInteger", IntegerValue); };
};

class TimeContinuousAmplitudeDiscreteUInteger : public Signal<t_uinteger> {
public:
	TimeContinuousAmplitudeDiscreteUInteger() { setType("TimeContinuousAmplitudeDiscreteUInteger", UIntegerValue); };
	TimeContinuousAmplitudeDiscreteUInteger(string fName) : Signal(fName) { setType("TimeContinuousAmplitudeDiscreteUInteger", UIntegerValue); };
	TimeContinuousAmplitudeDiscreteUInteger(int bLength) : Signal(bLength) { setType("TimeContinuousAmplitudeDiscreteUInteger", UIntegerValue); };
	TimeContinuousAmplitudeDiscreteUInteger(string fName, int bLength) : Signal(fName, bLength) { setType("TimeContinuousAmplitudeDiscreteUInteger", UIntegerValue); };
};

class TimeContinuousAmplitudeDiscreteReal : public Signal<t_real> {
public:
	TimeContinuousAmplitudeDiscreteReal() { setType("TimeContinuousAmplitudeDiscreteReal", RealValue); };
	TimeContinuousAmplitudeDiscreteReal(string fName) : Signal(fName) { setType("TimeContinuousAmplitudeDiscreteReal", RealValue); };
	TimeContinuousAmplitudeDiscreteReal(int bLength) : Signal(bLength) { setType("TimeContinuousAmplitudeDiscreteReal", RealValue); };
	TimeContinuousAmplitudeDiscreteReal(string fName, int bLength) : Signal(fName, bLength) { setType("TimeContinuousAmplitudeDiscreteReal", RealValue); };
};

class TimeContinuousAmplitudeDiscreteComplex : public Signal<t_complex> {
public:
	TimeContinuousAmplitudeDiscreteComplex() { setType("TimeContinuousAmplitudeDiscreteComplex", ComplexValue); };
	TimeContinuousAmplitudeDiscreteComplex(string fName) : Signal(fName) { setType("TimeContinuousAmplitudeDiscreteComplex", ComplexValue); };
	TimeContinuousAmplitudeDiscreteComplex(int bLength) : Signal(bLength) { setType("TimeContinuousAmplitudeDiscreteComplex", ComplexValue); };
	TimeContinuousAmplitudeDiscreteComplex(string fName, int bLength) : Signal(fName, bLength) { setType("TimeContinuousAmplitudeDiscreteComplex", ComplexValue); };
};

class TimeContinuousAmplitudeDiscreteComplexXY : public Signal<t_complex_xy> {
public:
	TimeContinuousAmplitudeDiscreteComplexXY() { setType("TimeContinuousAmplitudeDiscreteComplexXY", ComplexValueXY); };
	TimeContinuousAmplitudeDiscreteComplexXY(string fName) : Signal(fName) { setType("TimeContinuousAmplitudeDiscreteComplexXY", ComplexValueXY); };
	TimeContinuousAmplitudeDiscreteComplexXY(int bLength) : Signal(bLength) { setType("TimeContinuousAmplitudeDiscreteComplexXY", ComplexValueXY); };
	TimeContinuousAmplitudeDiscreteComplexXY(string fName, int bLength) : Signal(fName, bLength) { setType("TimeContinuousAmplitudeDiscreteComplexXY", ComplexValueXY); };
};

/* TimeDiscreteAmplitudeContinuous*/

class TimeDiscreteAmplitudeContinuousInteger : public Signal<t_integer> {
public:
	TimeDiscreteAmplitudeContinuousInteger() { setType("TimeDiscreteAmplitudeContinuousInteger", IntegerValue); };
	TimeDiscreteAmplitudeContinuousInteger(string fName) : Signal(fName) { setType("TimeDiscreteAmplitudeContinuousInteger", IntegerValue); };
	TimeDiscreteAmplitudeContinuousInteger(int bLength) : Signal(bLength) { setType("TimeDiscreteAmplitudeContinuousInteger", IntegerValue); };
	TimeDiscreteAmplitudeContinuousInteger(string fName, int bLength) : Signal(fName, bLength) { setType("TimeDiscreteAmplitudeContinuousInteger", IntegerValue); };
};

class TimeDiscreteAmplitudeContinuousUInteger : public Signal<t_uinteger> {
public:
	TimeDiscreteAmplitudeContinuousUInteger() { setType("TimeDiscreteAmplitudeContinuousUInteger", UIntegerValue); };
	TimeDiscreteAmplitudeContinuousUInteger(string fName) : Signal(fName) { setType("TimeDiscreteAmplitudeContinuousUInteger", UIntegerValue); };
	TimeDiscreteAmplitudeContinuousUInteger(int bLength) : Signal(bLength) { setType("TimeDiscreteAmplitudeContinuousUInteger", UIntegerValue); };
	TimeDiscreteAmplitudeContinuousUInteger(string fName, int bLength) : Signal(fName, bLength) { setType("TimeDiscreteAmplitudeContinuousUInteger", UIntegerValue); };
};

class TimeDiscreteAmplitudeContinuousReal : public Signal<t_real> {
public:
	TimeDiscreteAmplitudeContinuousReal() { setType("TimeDiscreteAmplitudeContinuousReal", RealValue); };
	TimeDiscreteAmplitudeContinuousReal(string fName) : Signal(fName) { setType("TimeDiscreteAmplitudeContinuousReal", RealValue); };
	TimeDiscreteAmplitudeContinuousReal(int bLength) : Signal(bLength) { setType("TimeDiscreteAmplitudeContinuousReal", RealValue); };
	TimeDiscreteAmplitudeContinuousReal(string fName, int bLength) : Signal(fName, bLength) { setType("TimeDiscreteAmplitudeContinuousReal", RealValue); };
};

class TimeDiscreteAmplitudeContinuousComplex : public Signal<t_complex> {
public:
	TimeDiscreteAmplitudeContinuousComplex() { setType("TimeDiscreteAmplitudeContinuousComplex", ComplexValue); };
	TimeDiscreteAmplitudeContinuousComplex(string fName) : Signal(fName) { setType("TimeDiscreteAmplitudeContinuousComplex", ComplexValue); };
	TimeDiscreteAmplitudeContinuousComplex(int bLength) : Signal(bLength) { setType("TimeDiscreteAmplitudeContinuousComplex", ComplexValue); };
	TimeDiscreteAmplitudeContinuousComplex(string fName, int bLength) : Signal(fName, bLength) { setType("TimeDiscreteAmplitudeContinuousComplex", ComplexValue); };
};

class TimeDiscreteAmplitudeContinuousComplexXY : public Signal<t_complex_xy> {
public:
	TimeDiscreteAmplitudeContinuousComplexXY() { setType("TimeDiscreteAmplitudeContinuousComplexXY", ComplexValueXY); };
	TimeDiscreteAmplitudeContinuousComplexXY(string fName) : Signal(fName) { setType("TimeDiscreteAmplitudeContinuousComplexXY", ComplexValueXY); };
	TimeDiscreteAmplitudeContinuousComplexXY(int bLength) : Signal(bLength) { setType("TimeDiscreteAmplitudeContinuousComplexXY", ComplexValueXY); };
	TimeDiscreteAmplitudeContinuousComplexXY(string fName, int bLength) : Signal(fName, bLength) { setType("TimeDiscreteAmplitudeContinuousComplexXY", ComplexValueXY); };
};


/* TimeContinuousAmplitudeContinuous*/

class TimeContinuousAmplitudeContinuousInteger : public Signal<t_integer> {
public:
	TimeContinuousAmplitudeContinuousInteger() { setType("TimeContinuousAmplitudeContinuousInteger", IntegerValue); };
	TimeContinuousAmplitudeContinuousInteger(string fName) : Signal(fName) { setType("TimeContinuousAmplitudeContinuousInteger", IntegerValue); };
	TimeContinuousAmplitudeContinuousInteger(int bLength) : Signal(bLength) { setType("TimeContinuousAmplitudeContinuousInteger", IntegerValue); };
	TimeContinuousAmplitudeContinuousInteger(string fName, int bLength) : Signal(fName, bLength) { setType("TimeContinuousAmplitudeContinuousInteger", IntegerValue); };
};

class TimeContinuousAmplitudeContinuousUInteger : public Signal<t_uinteger> {
public:
	TimeContinuousAmplitudeContinuousUInteger() { setType("TimeContinuousAmplitudeContinuousUInteger", UIntegerValue); };
	TimeContinuousAmplitudeContinuousUInteger(string fName) : Signal(fName) { setType("TimeContinuousAmplitudeContinuousUInteger", UIntegerValue); };
	TimeContinuousAmplitudeContinuousUInteger(int bLength) : Signal(bLength) { setType("TimeContinuousAmplitudeContinuousUInteger", UIntegerValue); };
	TimeContinuousAmplitudeContinuousUInteger(string fName, int bLength) : Signal(fName, bLength) { setType("TimeContinuousAmplitudeContinuousUInteger", UIntegerValue); };
};

class TimeContinuousAmplitudeContinuousReal : public Signal<t_real> {
public:
	TimeContinuousAmplitudeContinuousReal() { setType("TimeContinuousAmplitudeContinuousReal", RealValue); };
	TimeContinuousAmplitudeContinuousReal(string fName) : Signal(fName) { setType("TimeContinuousAmplitudeContinuousReal", RealValue); };
	TimeContinuousAmplitudeContinuousReal(int bLength) : Signal(bLength) { setType("TimeContinuousAmplitudeContinuousReal", RealValue); };
	TimeContinuousAmplitudeContinuousReal(string fName, int bLength) : Signal(fName, bLength) { setType("TimeContinuousAmplitudeContinuousReal", RealValue); };
};

class TimeContinuousAmplitudeContinuousComplex : public Signal<t_complex> {
public:
	TimeContinuousAmplitudeContinuousComplex() { setType("TimeContinuousAmplitudeContinuousComplex", ComplexValue); };
	TimeContinuousAmplitudeContinuousComplex(string fName) : Signal(fName) { setType("TimeContinuousAmplitudeContinuousComplex", ComplexValue); };
	TimeContinuousAmplitudeContinuousComplex(int bLength) : Signal(bLength) { setType("TimeContinuousAmplitudeContinuousComplex", ComplexValue); };
	TimeContinuousAmplitudeContinuousComplex(string fName, int bLength) : Signal(fName, bLength) { setType("TimeContinuousAmplitudeContinuousComplex", ComplexValue); };
};

class TimeContinuousAmplitudeContinuousComplexXY : public Signal<t_complex_xy> {
public:
	TimeContinuousAmplitudeContinuousComplexXY() { setType("TimeContinuousAmplitudeContinuousComplexXY", ComplexValueXY); };
	TimeContinuousAmplitudeContinuousComplexXY(string fName) : Signal(fName) { setType("TimeContinuousAmplitudeContinuousComplexXY", ComplexValueXY); };
	TimeContinuousAmplitudeContinuousComplexXY(int bLength) : Signal(bLength) { setType("TimeContinuousAmplitudeContinuousComplexXY", ComplexValueXY); };
	TimeContinuousAmplitudeContinuousComplexXY(string fName, int bLength) : Signal(fName, bLength) { setType("TimeContinuousAmplitudeContinuousComplexXY", ComplexValueXY); };
};

/* Specific Signals */

class Binary : public Signal<t_binary> {
public:
	Binary() { setType("Binary", BinaryValue); };
	Binary(string fName) : Signal(fName) { setType("Binary", BinaryValue); };
	Binary(int bLength) : Signal(bLength) { setType("Binary", BinaryValue); };
	Binary(string fName, int bLength) : Signal(fName, bLength) { setType("Binary", BinaryValue); };
};

class BandpassSignal : public Signal<t_complex> {
public:
	BandpassSignal() { setType("BandpassSignal", ComplexValue); };
	BandpassSignal(string fName) : Signal(fName) { setType("BandpassSignal", ComplexValue); };
	BandpassSignal(int bLength) : Signal(bLength) { setType("BandpassSignal", ComplexValue); };
	BandpassSignal(string fName, int bLength) : Signal(fName, bLength) { setType("BandpassSignal", ComplexValue); };
};

// the setType is BandpassSignal to garantee the compatibility with the Visualizer
class OpticalSignal : public Signal<t_complex> {
public:
	OpticalSignal() { setType("BandpassSignal", ComplexValue); };
	OpticalSignal(string fName) : Signal(fName) { setType("BandpassSignal", ComplexValue); };
	OpticalSignal(int bLength) : Signal(bLength) { setType("BandpassSignal", ComplexValue); };
	OpticalSignal(string fName, int bLength) : Signal(fName, bLength) { setType("BandpassSignal", ComplexValue); };
};

class OpticalSignalXY : public Signal<t_complex_xy> {
public:
	OpticalSignalXY() { setType("OpticalSignalXY", ComplexValueXY); };
	OpticalSignalXY(string fName) : Signal(fName) { setType("OpticalSignalXY", ComplexValueXY); };
	OpticalSignalXY(int bLength) : Signal(bLength) { setType("OpticalSignalXY", ComplexValueXY); };
	OpticalSignalXY(string fName, int bLength) : Signal(fName, bLength) { setType("OpticalSignalXY", ComplexValueXY); };
};


class PhotonStream : public Signal<t_photon> {
public:
	PhotonStream() { setType("PhotonStream", PhotonValue); };
	PhotonStream(string fName) : Signal(fName) { setType("PhotonStream", PhotonValue); };
	PhotonStream(int bLength) : Signal(bLength) { setType("PhotonStream", PhotonValue); };
	PhotonStream(string fName, int bLength) : Signal(fName, bLength) { setType("PhotonStream", PhotonValue); };
};



class PhotonStreamXY : public Signal {

public:
	PhotonStreamXY(string fName) { setType("PhotonStreamXY", ComplexValueXY); setFileName(fName); if (buffer == nullptr) buffer = new t_complex_xy[getBufferLength()]; }
	PhotonStreamXY(string fName, int bLength) { setType("PhotonStreamXY", ComplexValueXY); setFileName(fName); setBufferLength(bLength); }
	PhotonStreamXY(int bLength) { setType("PhotonStreamXY", ComplexValueXY); setBufferLength(bLength); }
	PhotonStreamXY() { setType("PhotonStreamXY", ComplexValueXY); if (buffer == nullptr) buffer = new t_complex_xy[getBufferLength()]; }

	void setBufferLength(int bLength) { Signal::setBufferLength(bLength); delete[] buffer; if (bLength != 0) buffer = new t_complex_xy[bLength]; };
};

class PhotonStreamMP : public Signal {

public:
	PhotonStreamMP(int bLength) { setType("PhotonStreamMP", PhotonValueMP); setBufferLength(bLength); }
	PhotonStreamMP() { setType("PhotonStreamMP", PhotonValueMP); if (buffer == nullptr) buffer = new t_photon_mp[getBufferLength()]; }

	void setBufferLength(int bLength) { Signal::setBufferLength(bLength); delete[] buffer; if (bLength != 0) buffer = new t_photon_mp[bLength]; };

};



class Messages : public Signal {
public:
	Messages(string fName) { setType("Message", Message); setFileName(fName); if (buffer == nullptr) buffer = new t_message[getBufferLength()]; }
	Messages(string fName, int bLength) { setType("Message", Message); setFileName(fName); setBufferLength(bLength); }
	Messages(int bLength) { setType("Message", Message); setBufferLength(bLength); }
	Messages() { setType("Message", Message); if (buffer == nullptr) buffer = new t_message[getBufferLength()]; }

	void setBufferLength(int bLength) { Signal::setBufferLength(bLength); delete[] buffer; if (bLength != 0) buffer = new t_message[bLength]; };
	
	void bufferPut(t_message);
};

//########################################################################################################################################################
//########################################################## GENERIC BLOCK DECLARATIONS AND DEFINITIONS ##################################################
//########################################################################################################################################################


// Descrives a generic Block.
class Block {

 public:

	/* Input Parameters */
	int numberOfInputSignals;
	int numberOfOutputSignals;
	vector<Signal *> inputSignals;
	vector<Signal *> outputSignals;

	/* Methods */
	Block(){};
	Block(vector<Signal*> &InputSig, vector<Signal*> &OutputSig);

	void initializeBlock(vector<Signal*> &InputSig, vector<Signal*> &OutputSig);
	void initializeBlock();
	virtual void initialize(void){};

	virtual bool runBlock();

	void terminateBlock();
	virtual void terminate(void){};
};


class SuperBlock : public Block {

	/* State Variables */

	vector<Block*> moduleBlocks;

	/* Input Parameters */

	bool saveInternalSignals{ false };

public:


	/* Methods */
	SuperBlock() {};
	SuperBlock(vector<Signal *> &inputSignal, vector<Signal *> &outputSignal) :Block(inputSignal, outputSignal){ setSaveInternalSignals(false); };

	void initialize(void);

	bool runBlock(void);

	void terminate(void);

	/* Set Methods */

	void setModuleBlocks(vector<Block*> mBlocks){ moduleBlocks = mBlocks; };

	void setSaveInternalSignals(bool sSignals);
	bool const getSaveInternalSignals(void){ return saveInternalSignals; };

};

class FIR_Filter : public Block {

	/* State Variable */
	vector<t_real> delayLine;

	bool saveImpulseResponse{ true };
	string impulseResponseFilename{ "impulse_response.imp" };

	/* Input Parameters */
	bool seeBeginningOfImpulseResponse{ true };

public:

	/* State Variable */
	vector<t_real> impulseResponse;

	/* Input Parameters */
	int impulseResponseLength;							// filter order + 1 (filter order = number of delays)


	/* Methods */
	FIR_Filter() {};
	FIR_Filter(vector<Signal *> &InputSig, vector<Signal *> OutputSig) :Block(InputSig, OutputSig){};

	void initializeFIR_Filter(void);

	bool runBlock(void);

	void terminate(void){};

	void setSaveImpulseResponse(bool sImpulseResponse) { saveImpulseResponse = sImpulseResponse; };
	bool getSaveImpulseResponse(void){ return saveImpulseResponse; };

	void setImpulseResponseLength(int iResponseLength) { impulseResponseLength = iResponseLength; };
	int const getImpulseResponseLength(){ return impulseResponseLength; }

	void setSeeBeginningOfImpulseResponse(bool sBeginning){ seeBeginningOfImpulseResponse = sBeginning; };
	bool const getSeeBeginningOfImpulseResponse(){ return seeBeginningOfImpulseResponse; };

};


class FD_Filter : public Block {
	
	/* State Variable */
	
	vector<t_real> inputBufferTimeDomain;
	vector<t_real> outputBufferTimeDomain;

	int inputBufferPointer{ 0 };
	int outputBufferPointer{ 0 };
	
	/* Input Parameters */
	bool saveTransferFunction{ true };
	string transferFunctionFilename{ "transfer_function.tfn" };
	int transferFunctionLength{ 128 };

	int inputBufferTimeDomainLength{ transferFunctionLength };
	int outputBufferTimeDomainLength{ transferFunctionLength };

public:
	/* State Variable */
	vector<t_complex> transferFunction;

	/* Methods */
	FD_Filter() {};
	FD_Filter(vector<Signal *> &InputSig, vector<Signal *> OutputSig) :Block(InputSig, OutputSig) {};

	void initializeFD_Filter(void);

	bool runBlock(void);

	void terminate(void) {};

	void setInputBufferTimeDomainLength(int iBufferTimeDomainLength) { inputBufferTimeDomainLength = iBufferTimeDomainLength; };
	int const getInputBufferTimeDomainLength() { return inputBufferTimeDomainLength; }

	void setOutputBufferTimeDomainLength(int oBufferTimeDomainLength) { outputBufferTimeDomainLength = oBufferTimeDomainLength; };
	int const getOutputBufferTimeDomainLength() { return outputBufferTimeDomainLength; }

	void setInputBufferPointer(int iBufferPointer) { inputBufferPointer = iBufferPointer; };
	int const getInputBufferPointer() { return inputBufferPointer; }

	void setOutputBufferPointer(int oBufferPointer) { outputBufferPointer = oBufferPointer; };
	int const getOutputBufferPointer() { return outputBufferPointer; }

	void setSaveTransferFunction(bool sTransferFunction) { saveTransferFunction = sTransferFunction; };
	bool getSaveTransferFunction(void) { return saveTransferFunction; };

	void setTransferFunctionLength(int iTransferFunctionLength) { transferFunctionLength = iTransferFunctionLength; };
	int const getTransferFunctionLength() { return transferFunctionLength; };

};


// Generates a complex signal knowing the real part and the complex part.
class RealToComplex : public Block {
 public:
	 RealToComplex(vector<Signal *> &InputSig, vector<Signal *> &OutputSig);
	 bool runBlock(void);
 //private:
};

// Separates the complex signal into two parts: real and imaginary.
/*class ComplexToReal : public Block {
 public:
	 ComplexToReal(vector<Signal *> &InputSig, vector<Signal *> &OutputSig);
	 bool runBlock(void);
 //private:
};*/




//########################################################################################################################################################
//########################################################## GENERIC SYSTEM DECLARATIONS AND DEFINITIONS #################################################
//########################################################################################################################################################


class System {

 public:
  System(vector<Block *> &MainSystem);	
  void terminate();										
  void run();
  void run(string signalPath);

  string signalsFolder{ "signals" };
  char fileName[MAX_NAME_SIZE];  // Name of the file with system description (.sdf)
  char name[MAX_NAME_SIZE];  // System Name
  int numberOfBlocks;  // Number of system Blocks
  int (*topology)[MAX_TOPOLOGY_SIZE];  // Relationship matrix
  vector<Block *> SystemBlocks;  // Pointer to an array of pointers to Block objects
};

//########################################################################################################################################################
//############################################################### GENERIC DSP FUNCTIONS ##################################################################
//########################################################################################################################################################


class OverlapMethod
{

public:

	void overlapSaveSymComplexIn(vector<complex <double>> &v_in, vector<complex <double>> &v_out, vector<complex <double>> Hf, int NFFT);
	void overlapSaveSyRealIn(vector<double> &v_in, vector<double> &v_out, vector<double> Hf, int NFFT);
	void overlapSaveAsym(vector<double> &real_in, vector<double> &imag_in, vector<double> &real_out, vector<double> &imag_out, vector<double> h_real, vector<double> h_imag, int M, int L, int N);
	void overlapSaveSym(vector<double> &real_in, vector<double> &imag_in, vector<double> &real_out, vector<double> &imag_out, vector<double> h_real, vector<double> h_imag, int NFFT);
	void checkSize(vector<double> &real_in, vector<double> &imag_in, int L);

};




class Fft
{

public:
	vector<complex <double>> directTransformInReal(vector<double> real);

	vector<double> inverseTransformInCP(vector<complex <double>> &In);
	
	void directTransform(vector<double> &real, vector<double> &imag);
	void inverseTransform(vector<double> &real, vector<double> &imag);
	void transformRadix2(vector<double> &real, vector<double> &imag);
	void transformBluestein(vector<double> &real, vector<double> &imag);
	void convolve(const vector<double> &x, const vector<double> &y, vector<double> &out);
	void convolve(const vector<double> &xreal, const vector<double> &ximag, const vector<double> &yreal, const vector<double> &yimag, vector<double> &outreal, vector<double> &outimag);
	
	void Radix2(vector<double> &real, vector<double> &imag, int m);
	void Bluestein(vector<double> &real, vector<double> &imag, int m);
};


class ComplexMult
{
public:
	void CMultVector(vector<double> &v1_real, vector<double> &v1_imag, vector<double> v2_real, vector<double> v2_imag);
	void CMultVector_Loop(vector<double> &v1_real, vector<double> &v1_imag, vector<double> v2_real, vector<double> v2_imag);
	vector<complex <double>> CMultVectorInCP(vector<complex <double>> &v1_in, vector<complex <double>> &v2_in);
	void ComplexVect2ReImVect(vector<complex <double>> &v_in, vector<double> &v1_real, vector<double> &v1_imag);
	void CMultVector_InComplex(vector<complex <double>> &v1_in, vector<complex <double>> &v2_in);
	void ReImVect2ComplexVect(vector<double> &v1_real, vector<double> &v1_imag, vector<complex <double>> &v_out);

	vector<complex<double>> ReImVect2ComplexVector(vector<double> &v1_real, vector<double> &v1_imag);
	vector<complex <double>> complexVectorMultiplication(vector<complex <double>> &v1_in, vector<complex <double>> &v2_in);
};


///////////////////// Fast Fourier Transform ////////////////////////

class FourierTransform
{
public:
	vector <complex<double>> transform(vector<complex<double>>IN, int m);
};

# endif // PROGRAM_INCLUDE_netxpto_H_
