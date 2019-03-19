# ifndef PROGRAM_INCLUDE_NETXPTO_H_
# define PROGRAM_INCLUDE_NETXPTO_H_

// ####################################################################################################
// #
// # STL header files
// #
// ####################################################################################################

# include <algorithm>	
# include <array>
# include <bitset>
# include <chrono>
# include <cmath>
# include <complex>
# include  <cctype> 
# include <ctime>
# include  <filesystem>
# include <fstream>
# include <functional>	
# include <iostream>
# include  <locale>
# include <map>
# include <random>
# include <sstream>
# include <vector>
# include <strstream>
# include <string>



// ####################################################################################################
// #
// # Alias for ISO C++ built-in types
// #
// ####################################################################################################

using t_unsigned_long = unsigned long int;
using t_unsigned = unsigned int;
using t_integer = int;


// ####################################################################################################
// #
// # Global constants
// #
// ####################################################################################################

const int MAX_NAME_SIZE = 256;					// Maximum size of names (file names,..)
const t_integer MAX_BUFFER_LENGTH = 10000;			// Maximum Signal buffer length
const int DEFAULT_BUFFER_LENGTH = 512;
const int MAX_TOPOLOGY_SIZE = 100;				// Maximum System topology size 
const int MAX_TAPS = 1000;						// Maximum Taps Number
const double PI = 3.1415926535897932384;		// Value of pi
const double SPEED_OF_LIGHT = 299792458;		// Speed of light in vaccum
const double PLANCK_CONSTANT = 6.626070040e-34; // NIST
const int MAX_NUMBER_OF_PATHS = 2;

using namespace std;							// to be deleted 4/9/2018

// ####################################################################################################
// #
// # Random engine
// #
// ####################################################################################################

using random_engine = std::default_random_engine;

// ####################################################################################################
// #
// # Alias for NetXpto signal value types
// #
// ####################################################################################################

using t_binary = unsigned int;
using t_integer = int;
using t_real = double;
using t_complex = complex<t_real>;
using t_complex_xy = struct { t_complex x; t_complex y; };
//using t_photon = struct { t_real probabilityAmplitude;  t_real polarization; }; // isto é para eliminar
//using t_photon_mp = struct { t_photon path[MAX_NUMBER_OF_PATHS]; };				// isto é para eliminar
using t_complex_xy_mp = struct { t_complex x; t_complex y; bool processed{ false }; };
using t_photon_mp_xy = struct { t_complex_xy_mp path[MAX_NUMBER_OF_PATHS]; };
using t_iqValues = complex<t_real>;
using t_message = struct {	string messageType;	string messageDataLength; 	string messageData; int size() { return 3; }};

enum class signal_value_type { t_binary, t_integer, t_real, t_complex, t_complex_xy, t_photon, t_photon_mp, t_photon_mp_xy, t_iqValues, t_message };

// #######################################################################################################
// #
// # Operator << overloading
// #
// #######################################################################################################

template<typename T>
std::ostream& operator<<(std::ostream &out, const complex<T> &cx) 
{ 
	out << cx.real() << " + i " << cx.imag();
	return out;
}

// ####################################################################################################
// #
// # NetXpto enuerate class types
// #
// ####################################################################################################

enum class signal_type { Binary, TimeDiscreteAmplitudeContinuousReal, TimeContinuousAmplitudeContinuousReal, OpticalSignal, PhotonStreamXY, PhotonStreamMP, PhotonStreamMPXY };

//enum class signal_write_mode {Binary, Ascii};

//########################################################################################################################################################
//############################################################## SIGNALS DECLARATION #####################################################################
//########################################################################################################################################################

//########################################################## SIGNALS ABSTRACT CLASS ######################################################################
// The hierarchy of the signals are the following: the root is the abstract SIGNAL class, where all comum data and functions are declared,
// the second level is a template where value dependent data and member functions are declared,
// the third level is where signal type specificities are declared.
// All signals have associated a circular buffer (which is used as a temporary storage, before and after processing) and a file (permanente storage)
//#########################################################################################################################################################

// Signal abstract root class

class Signal {

public:

	// Signals constructors
	explicit Signal() {};
	explicit Signal(string fileName) : fileName{ fileName }, saveSignal{ true } {};
	explicit Signal(string fileName, t_unsigned_long bLength) : fileName{ fileName }, bufferLength{ bLength }, saveSignal{ true } {};
	explicit Signal(string fileName, string folderName) : fileName{ fileName }, folderName{ folderName }, saveSignal{ true } {};
	explicit Signal(string fileName, string folderName, t_unsigned_long bLength) : fileName{ fileName }, folderName{ folderName }, bufferLength{ bLength }, saveSignal{ true } {};
	explicit Signal(t_unsigned_long bLength) : bufferLength{ bLength } {};

	// Signal destructors
	~Signal() { if (!(valueType == signal_value_type::t_message)) { delete[] buffer; }; };	

	// Buffer manipulation funtions
	t_integer ready();										// Returns the number of samples in the buffer ready to be processed
	t_integer space();										// Returns the signal buffer space without originating signal overlap
	template<typename T> void bufferPut(T value);			// Puts a value in the buffer (at the end)
	template<typename T> void bufferGet(T* valueAddr);		// Returns a value from the buffer (from the begining)	
	void bufferGet();

	// File manipulation
	void writeHeader();								// Opens the signal file in the default signals directory, \signals, and writes the signal header
	void writeHeader(string signalPath);			// Opens the signal file in the signalPath directory, and writes the signal header

	// Buffer and File manipulation
	void close();									// Empty the signal buffer and close the signal file

	void reset() { inPosition = 0; outPosition = inPosition; bufferEmpty = true; bufferFull = false; }

	// Change signal parameters
	void setSaveSignal(bool sSignal) { saveSignal = sSignal; };
	bool getSaveSignal() const { return saveSignal; };

	void setType(string sType, signal_value_type vType) { type = sType; valueType = vType; };
	void setType(string sType) { type = sType; };
	string getType() const { return type; };

	void setInPosition(int iPosition) { inPosition = iPosition; };
	int getInPosition() const { return inPosition; };

	void setOutPosition(int oPosition) { outPosition = oPosition; };
	int getOutPosition() const { return outPosition; };

	void setBufferEmpty(bool bEmpty) { bufferEmpty = bEmpty; };
	bool getBufferEmpty() const { return bufferEmpty; };

	void setBufferFull(bool bFull) { bufferFull = bFull; };
	bool getBufferFull() const { return bufferFull; };

	//void setValueType(signal_value_type vType) { valueType = vType; };
	signal_value_type getValueType() const { return valueType; };

	void setFileName(string fName) { fileName = fName; };
	string getFileName() const { return fileName; };

	void setFolderName(string fName) { folderName = fName; };
	string getFolderName() const { return folderName; };

	//void setBufferLength(t_unsigned_long bLength) { bufferLength = bLength; };
	int getBufferLength() const { return bufferLength; };

	void setFirstValueToBeSaved(t_unsigned_long fValueToBeSaved) { firstValueToBeSaved = fValueToBeSaved; };
	t_unsigned_long getFirstValueToBeSaved() const { return firstValueToBeSaved; };

	void setNumberOfValuesToBeSaved(t_unsigned_long nOfValuesToBeSaved) { numberOfValuesToBeSaved = nOfValuesToBeSaved; };
	t_unsigned_long getNumberOfValuesToBeSaved() const { return numberOfValuesToBeSaved; };

	void setSymbolPeriod(double sPeriod) { symbolPeriod = sPeriod; samplesPerSymbol = symbolPeriod / samplingPeriod; };
	double getSymbolPeriod() const { return symbolPeriod; };

	void setSamplingPeriod(double sPeriod) { samplingPeriod = sPeriod; samplesPerSymbol = symbolPeriod / samplingPeriod; };
	double getSamplingPeriod() const { return samplingPeriod; };

	void setSamplesPerSymbol(double sPeriod) { samplesPerSymbol = sPeriod; symbolPeriod = samplesPerSymbol * samplingPeriod; samplingPeriod = symbolPeriod / samplesPerSymbol; };
	double getSamplesPerSymbol() const { return samplesPerSymbol; };

	void setCentralFrequency(double cFrequency) { centralFrequency = cFrequency; centralWavelength = SPEED_OF_LIGHT / centralFrequency; }
	double getCentralFrequency() const { return centralFrequency; };

	void setCentralWavelength(double cWavelength) { centralWavelength = cWavelength; centralFrequency = SPEED_OF_LIGHT / centralWavelength; }
	double getCentralWavelength() const { return centralWavelength; }

	template<typename t, signal_type sType, signal_value_type vType> friend class BaseSignal;

private:

	/* Circular buffer state variables */
	void* buffer{ nullptr };											// Pointer to buffer
	t_unsigned inPosition{ 0 };									// Next position for the buffer input values
	t_unsigned outPosition{ 0 };									// Next position for the buffer output values
	bool bufferEmpty{ true };											// Flag bufferEmpty
	bool bufferFull{ false };											// Flag bufferFull
	const t_unsigned bufferLength{ DEFAULT_BUFFER_LENGTH };		// Buffer length

	t_unsigned_long numberOfSavedValues{ 0 };							// Number of saved values
	t_unsigned_long count;												// Number of values that have already entered in the buffer

	/* Input Parameters */

	t_unsigned_long firstValueToBeSaved{ 1 };						// First value (>= 1) to be saved
	bool saveSignal{ false };

	string type;											// Signal type
	signal_value_type valueType;							// Signal samples type

	string fileName{ "" };							// Name of the file where data values are going to be saved
	string folderName{ "signals" };					// folder where signals are going to be saved by default
	bool headerWritten{ false };

	long int numberOfValuesToBeSaved{ -1 };			// Number of values to be saved, if -1 all values are going to be saved

	double symbolPeriod{ 1.0 };						// Signal symbol period (it is the inverse of the symbol rate)
	double samplingPeriod{ 1.0 };					// Signal sampling period (it is the time space between two samples)
	double samplesPerSymbol{ 1.0 };					// samplesPerSymbol = symbolPeriod / samplingPeriod;

	double centralWavelength{ 1550E-9 };
	double centralFrequency{ SPEED_OF_LIGHT / centralWavelength };
};

template<typename T, signal_type sType, signal_value_type vType>
class BaseSignal : public Signal {

public:

	using value_type = T;

	BaseSignal() : Signal() { setBuffer(); }
	BaseSignal(string fileName) : Signal(fileName) { setBuffer(); }
	BaseSignal(string fileName, t_unsigned_long bLength) : Signal(fileName, bLength) { setBuffer(); }
	BaseSignal(string fileName, string folderName) : Signal(fileName, folderName) { setBuffer(); }
	BaseSignal(string fileName, string folderName, t_unsigned_long bLength) : Signal(fileName, folderName, bLength) { setBuffer(); }
	BaseSignal(t_unsigned_long bLength) : Signal(bLength) { setBuffer(); }

	value_type bufferGet()
	{
		if (bufferEmpty) return value_type();

		value_type value = static_cast<value_type *>(buffer)[outPosition];

		bufferFull = false;

		outPosition = (outPosition + 1) % bufferLength;

		bufferEmpty = outPosition == inPosition;

		return value;
	}

	/*
	void bufferPut(value_type value)
	{
		(static_cast<T *>(buffer))[inPosition] = value;

		if (bufferFull)
		{
			outPosition = (outPosition + 1) % bufferLength;
		}

		inPosition = (inPosition + 1) % bufferLength;

		bufferEmpty = false;
		bufferFull = inPosition == outPosition;

		if (bufferFull)
		{
			if (saveSignal)
			{
				if (!headerWritten) writeHeader();

				if (firstValueToBeSaved <= bufferLength)
				{
					char *ptr = (char *)buffer;
					ptr = ptr + (firstValueToBeSaved - 1) * sizeof(T);
					ofstream fileHandler{ "./" + folderName + "/" + fileName, ios::out | ios::binary | ios::app };
					fileHandler.write(ptr, (bufferLength - (firstValueToBeSaved - 1)) * sizeof(T));
					fileHandler.close();
					firstValueToBeSaved = 1;
				}
				else
				{
					firstValueToBeSaved = firstValueToBeSaved - bufferLength;
				}
			}
		}
	}
	*/
	value_type& operator[](int index) { return (static_cast<value_type*>(buffer)[(outPosition + index) % bufferLength]); }

private:

	void setBuffer() {

		string typeName;
		switch (sType) {
			case signal_type::Binary:
				typeName = "Binary";
				break;
			case signal_type::TimeDiscreteAmplitudeContinuousReal:
				typeName = "TimeDiscreteAmplitudeContinuousReal";
				break;
			case signal_type::TimeContinuousAmplitudeContinuousReal:
				typeName = "TimeContinuousAmplitudeContinuousReal";
				break;
			case signal_type::OpticalSignal:
				typeName = "OpticalSignal";
				break;
			case signal_type::PhotonStreamXY:
				typeName = "PhotonStreamXY";
				break;
			case signal_type::PhotonStreamMP:
				typeName = "PhotonStreamMP";
				break;
			case signal_type::PhotonStreamMPXY:
				typeName = "PhotonStreamMPXY";
				break;
			default:
				cout << "Error: netxpto_20180815.h - typeName not defined\n";
		}

		setType(typeName, vType);
		if (buffer == nullptr) buffer = (value_type*) (new value_type[getBufferLength()]);
	}

};

using Binary = BaseSignal< t_binary, signal_type::Binary, signal_value_type::t_binary> ;
using TimeDiscreteAmplitudeContinuousReal = BaseSignal<t_real, signal_type::TimeDiscreteAmplitudeContinuousReal, signal_value_type::t_real>;
using TimeContinuousAmplitudeContinuousReal = BaseSignal<t_real, signal_type::TimeContinuousAmplitudeContinuousReal, signal_value_type::t_real>;
using OpticalSignal = BaseSignal<t_complex, signal_type::TimeContinuousAmplitudeContinuousReal, signal_value_type::t_real>;
using PhotonStreamXY = BaseSignal<t_complex_xy, signal_type::PhotonStreamXY, signal_value_type::t_complex_xy>;
//using PhotonStreamMP = BaseSignal<t_photon_mp, signal_type::PhotonStreamMP, signal_value_type::t_photon_mp>;
using PhotonStreamMPXY = BaseSignal<t_photon_mp_xy, signal_type::PhotonStreamMPXY, signal_value_type::t_photon_mp_xy>;


/*
class TimeDiscrete : public Signal {
public:
	TimeDiscrete(string fName) { setFileName(fName); }
	TimeDiscrete(string fileName,string folderName) : Signal(fileName,folderName) { }
	TimeDiscrete(){}
};


class TimeDiscreteAmplitudeDiscrete : public Signal {
public:
	TimeDiscreteAmplitudeDiscrete(string fName) { setFileName(fName); }
	TimeDiscreteAmplitudeDiscrete(string fileName, string folderName) : Signal(fileName, folderName) { }
	TimeDiscreteAmplitudeDiscrete(){}
};


class TimeDiscreteAmplitudeContinuous : public Signal {
public:
	TimeDiscreteAmplitudeContinuous(string fName) { setFileName(fName); }
	TimeDiscreteAmplitudeContinuous(string fileName, string folderName) : Signal(fileName, folderName) { }
	TimeDiscreteAmplitudeContinuous(){}
};


class TimeDiscreteAmplitudeDiscreteReal : public Signal {
public:
	TimeDiscreteAmplitudeDiscreteReal(string fName) { setType("TimeDiscreteAmplitudeDiscreteReal", signal_value_type::RealValue); setFileName(fName); if (buffer == nullptr) buffer = new t_real[getBufferLength()]; }
	TimeDiscreteAmplitudeDiscreteReal(string fileName, string folderName) : Signal(fileName,folderName){ setType("TimeDiscreteAmplitudeDiscreteReal", signal_value_type::RealValue); if (buffer == nullptr) buffer = new t_real[getBufferLength()]; }
	TimeDiscreteAmplitudeDiscreteReal(string fName, t_unsigned_long bLength) { setType("TimeDiscreteAmplitudeDiscreteReal", signal_value_type::RealValue); setFileName(fName); setBufferLength(bLength); }
	TimeDiscreteAmplitudeDiscreteReal(t_unsigned_long bLength) { setType("TimeDiscreteAmplitudeDiscreteReal", signal_value_type::RealValue); setBufferLength(bLength); }
	TimeDiscreteAmplitudeDiscreteReal(){ setType("TimeDiscreteAmplitudeDiscreteReal", signal_value_type::RealValue); if (buffer == nullptr) buffer = new t_real[getBufferLength()]; }

//	void setBufferLength(t_unsigned_long bLength) { Signal::setBufferLength(bLength); delete[] buffer; if (bLength != 0) buffer = new t_real[bLength]; };
};


class TimeDiscreteAmplitudeDiscreteComplex : public Signal {
	
public:
	TimeDiscreteAmplitudeDiscreteComplex(string fName) { setFileName(fName); }
	TimeDiscreteAmplitudeDiscreteComplex(string fileName, string folderName) : Signal(fileName, folderName) { }
	TimeDiscreteAmplitudeDiscreteComplex() {}
};
*/
/*
class Binary : public Signal {
	
public:
	using value_type = t_binary;

	Binary() : Signal() { setBuffer(); }
	Binary(string fileName) : Signal(fileName) { setBuffer(); }
	Binary(string fileName, t_unsigned_long bLength) : Signal(fileName, bLength) { setBuffer(); }
	Binary(string fileName, string folderName) : Signal(fileName, folderName) { setBuffer(); }
	Binary(string fileName, string folderName, t_unsigned_long bLength) : Signal(fileName, folderName, bLength) { setBuffer(); }
	Binary(t_unsigned_long bLength) : Signal(bLength) { setBuffer(); }
	Binary(string fileName, bool sSignal) : Signal(fileName, sSignal) { setBuffer(); }
	Binary(string fileName, t_unsigned_long bLength, bool sSignal) : Signal(fileName, bLength, sSignal) { setBuffer(); }
	Binary(string fileName, string folderName, bool sSignal) : Signal(fileName, folderName, sSignal) { setBuffer(); }
	Binary(string fileName, string folderName, t_unsigned_long bLength, bool sSignal) : Signal(fileName, folderName, bLength, sSignal) { setBuffer(); }

private:

	void setBuffer() { setType("Binary", signal_value_type::BinaryValue); if (buffer == nullptr) buffer = new value_type[getBufferLength()]; }

};
*/
/*
class TimeDiscreteAmplitudeContinuousReal : public Signal {
public:
	TimeDiscreteAmplitudeContinuousReal(string fName) { setType("TimeDiscreteAmplitudeContinuousReal", signal_value_type::RealValue); setFileName(fName); if (buffer == nullptr) buffer = new t_real[getBufferLength()]; }
	TimeDiscreteAmplitudeContinuousReal(string fileName, string folderName) : Signal(fileName,folderName){ setType("TimeDiscreteAmplitudeContinuousReal", signal_value_type::RealValue); if (buffer == nullptr) buffer = new t_real[getBufferLength()];}
	TimeDiscreteAmplitudeContinuousReal(string fName, t_unsigned_long bLength) { setType("TimeDiscreteAmplitudeContinuousReal", signal_value_type::RealValue); setFileName(fName); setBufferLength(bLength); }
	TimeDiscreteAmplitudeContinuousReal(t_unsigned_long bLength) { setType("TimeDiscreteAmplitudeContinuousReal", signal_value_type::RealValue); setBufferLength(bLength); }
	TimeDiscreteAmplitudeContinuousReal(){ setType("TimeDiscreteAmplitudeContinuousReal", signal_value_type::RealValue); if (buffer == nullptr) buffer = new t_real[getBufferLength()]; }

	void setBufferLength(t_unsigned_long bLength) { Signal::setBufferLength(bLength); delete[] buffer; if (bLength != 0) buffer = new t_real[bLength]; };
};


class TimeDiscreteAmplitudeContinuousComplex : public Signal {
public:
	TimeDiscreteAmplitudeContinuousComplex(string fName) { setType("TimeDiscreteAmplitudeContinuousComplex", signal_value_type::ComplexValue); setFileName(fName); if (buffer == nullptr) buffer = new t_complex[getBufferLength()]; }
	TimeDiscreteAmplitudeContinuousComplex(string fileName, string folderName) : Signal(fileName,folderName){ setType("TimeDiscreteAmplitudeContinuousComplex", signal_value_type::ComplexValue); if (buffer == nullptr) buffer = new t_complex[getBufferLength()];}
	TimeDiscreteAmplitudeContinuousComplex(string fName, t_unsigned_long bLength) { setType("TimeDiscreteAmplitudeContinuousComplex", signal_value_type::ComplexValue); setFileName(fName); setBufferLength(bLength); }
	TimeDiscreteAmplitudeContinuousComplex(t_unsigned_long bLength) { setType("TimeDiscreteAmplitudeContinuousComplex", signal_value_type::ComplexValue); setBufferLength(bLength); }
	TimeDiscreteAmplitudeContinuousComplex(){ setType("TimeDiscreteAmplitudeContinuousComplex", signal_value_type::ComplexValue); if (buffer == nullptr) buffer = new t_complex[getBufferLength()]; }

	void setBufferLength(t_unsigned_long bLength) { Signal::setBufferLength(bLength); delete[] buffer; if (bLength != 0) buffer = new t_complex[bLength]; };
};


class TimeContinuous : public Signal {
public:
	TimeContinuous(string fileName) : Signal(fileName) { }
	TimeContinuous(string fileName, string folderName) : Signal(fileName, folderName) { }
	TimeContinuous(){}
};

class TimeContinuousAmplitudeDiscrete : public Signal {
public:
	TimeContinuousAmplitudeDiscrete(string fileName) : Signal(fileName) { }
	TimeContinuousAmplitudeDiscrete(string fileName, string folderName) : Signal(fileName, folderName) { }
	TimeContinuousAmplitudeDiscrete(){}
};


class TimeContinuousAmplitudeContinuous : public Signal {
public:
	TimeContinuousAmplitudeContinuous(string fName) { setType("TimeContinuousAmplitudeContinuous", signal_value_type::RealValue);  setFileName(fName); if (buffer == nullptr) buffer = new t_real[getBufferLength()]; }
	TimeContinuousAmplitudeContinuous(string fileName, string folderName) : Signal(fileName, folderName) { setType("TimeContinuousAmplitudeContinuous", signal_value_type::RealValue); if (buffer == nullptr) buffer = new t_real[getBufferLength()];	}
	TimeContinuousAmplitudeContinuous(string fName, t_unsigned_long bLength) { setType("TimeContinuousAmplitudeContinuous", signal_value_type::RealValue);  setFileName(fName); setBufferLength(bLength); }
	TimeContinuousAmplitudeContinuous(t_unsigned_long bLength) { setType("TimeContinuousAmplitudeContinuous", signal_value_type::RealValue);  setBufferLength(bLength); }
	TimeContinuousAmplitudeContinuous() { setType("TimeContinuousAmplitudeContinuous", signal_value_type::RealValue); if (buffer == nullptr) buffer = new t_real[getBufferLength()]; }

	void setBufferLength(t_unsigned_long bLength) { Signal::setBufferLength(bLength); delete[] buffer; if (bLength != 0) buffer = new t_real[bLength]; };
};


class TimeContinuousAmplitudeDiscreteReal : public Signal {
public:
	TimeContinuousAmplitudeDiscreteReal(string fName) { setType("TimeContinuousAmplitudeDiscreteReal", signal_value_type::RealValue);  setFileName(fName); if (buffer == nullptr) buffer = new t_real[getBufferLength()]; }
	TimeContinuousAmplitudeDiscreteReal(string fileName, string folderName) : Signal(fileName, folderName) { setType("TimeContinuousAmplitudeDiscreteReal", signal_value_type::RealValue); if (buffer == nullptr) buffer = new t_real[getBufferLength()];}
	TimeContinuousAmplitudeDiscreteReal(string fName, t_unsigned_long bLength) { setType("TimeContinuousAmplitudeDiscreteReal", signal_value_type::RealValue);  setFileName(fName); setBufferLength(bLength); }
	TimeContinuousAmplitudeDiscreteReal(t_unsigned_long bLength) { setType("TimeContinuousAmplitudeDiscreteReal", signal_value_type::RealValue);  setBufferLength(bLength); }
	TimeContinuousAmplitudeDiscreteReal(){ setType("TimeContinuousAmplitudeDiscreteReal", signal_value_type::RealValue); if (buffer == nullptr) buffer = new t_real[getBufferLength()]; }

	void setBufferLength(t_unsigned_long bLength) { Signal::setBufferLength(bLength); delete[] buffer; if (bLength != 0) buffer = new t_real[bLength]; };
};


class TimeContinuousAmplitudeDiscreteComplex : public Signal {
public:
	TimeContinuousAmplitudeDiscreteComplex(string fName) { setType("TimeContinuousAmplitudeDiscreteComplex", signal_value_type::ComplexValue); setFileName(fName); if (buffer == nullptr) buffer = new t_complex[getBufferLength()]; }
	TimeContinuousAmplitudeDiscreteComplex(string fileName, string folderName) : Signal(fileName, folderName) { setType("TimeContinuousAmplitudeDiscreteComplex", signal_value_type::ComplexValue); if (buffer == nullptr) buffer = new t_complex[getBufferLength()];}
	TimeContinuousAmplitudeDiscreteComplex(string fName, t_unsigned_long bLength) { setType("TimeContinuousAmplitudeDiscreteComplex", signal_value_type::ComplexValue); setFileName(fName); setBufferLength(bLength); }
	TimeContinuousAmplitudeDiscreteComplex(t_unsigned_long bLength) { setType("TimeContinuousAmplitudeDiscreteComplex", signal_value_type::ComplexValue); setBufferLength(bLength); }
	TimeContinuousAmplitudeDiscreteComplex(){ setType("TimeContinuousAmplitudeDiscreteComplex", signal_value_type::ComplexValue); if (buffer == nullptr) buffer = new t_complex[getBufferLength()]; }

	void setBufferLength(t_unsigned_long bLength) { Signal::setBufferLength(bLength); delete[] buffer; if (bLength != 0) buffer = new t_complex[bLength]; };
};


class TimeContinuousAmplitudeContinuousReal : public Signal {
public:
	TimeContinuousAmplitudeContinuousReal(string fName) { setType("TimeContinuousAmplitudeContinuousReal", signal_value_type::RealValue); setFileName(fName); if (buffer == nullptr) buffer = new t_real[getBufferLength()]; }
	TimeContinuousAmplitudeContinuousReal(string fileName,string folderName) : Signal(fileName, folderName) { setType("TimeContinuousAmplitudeContinuousReal", signal_value_type::RealValue); if (buffer == nullptr) buffer = new t_real[getBufferLength()];}
	TimeContinuousAmplitudeContinuousReal(string fName, t_unsigned_long bLength) { setType("TimeContinuousAmplitudeContinuousReal", signal_value_type::RealValue); setFileName(fName); setBufferLength(bLength); }
	TimeContinuousAmplitudeContinuousReal(t_unsigned_long bLength) { setType("TimeContinuousAmplitudeContinuousReal", signal_value_type::RealValue); setBufferLength(bLength); }
	TimeContinuousAmplitudeContinuousReal(){ setType("TimeContinuousAmplitudeContinuousReal", signal_value_type::RealValue); if (buffer == nullptr) buffer = new t_real[getBufferLength()]; }

	void setBufferLength(t_unsigned_long bLength) { Signal::setBufferLength(bLength); delete[] buffer; if (bLength != 0) buffer = new t_real[bLength]; };

};


class TimeContinuousAmplitudeContinuousComplex : public Signal {
public:
	TimeContinuousAmplitudeContinuousComplex(string fName) { setType("TimeContinuousAmplitudeContinuousComplex", signal_value_type::ComplexValue); setFileName(fName); if (buffer == nullptr) buffer = new t_complex[getBufferLength()]; }
	TimeContinuousAmplitudeContinuousComplex(string fileName, string folderName) : Signal(fileName, folderName) { setType("TimeContinuousAmplitudeContinuousComplex", signal_value_type::ComplexValue); if (buffer == nullptr) buffer = new t_complex[getBufferLength()];}
	TimeContinuousAmplitudeContinuousComplex(string fName, t_unsigned_long bLength) { setType("TimeContinuousAmplitudeContinuousComplex", signal_value_type::ComplexValue); setFileName(fName); setBufferLength(bLength); }
	TimeContinuousAmplitudeContinuousComplex(t_unsigned_long bLength) { setType("TimeContinuousAmplitudeContinuousComplex", signal_value_type::ComplexValue); setBufferLength(bLength); }
	TimeContinuousAmplitudeContinuousComplex(){ setType("TimeContinuousAmplitudeContinuousComplex", signal_value_type::ComplexValue); if (buffer == nullptr) buffer = new t_complex[getBufferLength()]; }

	void setBufferLength(t_unsigned_long bLength) { Signal::setBufferLength(bLength); delete[] buffer; if (bLength != 0) buffer = new t_complex[bLength]; };
};

class BandpassSignal : public Signal {
public:
	BandpassSignal(string fName) { setType("BandpassSignal", signal_value_type::ComplexValue); setFileName(fName); if (buffer == nullptr) buffer = new t_complex[getBufferLength()]; }
	BandpassSignal(string fileName, string folderName) : Signal(fileName, folderName) { setType("BandpassSignal", signal_value_type::ComplexValue); setFileName(fileName); if (buffer == nullptr) buffer = new t_complex[getBufferLength()];}
	BandpassSignal(string fName, t_unsigned_long bLength) { setType("BandpassSignal", signal_value_type::ComplexValue); setFileName(fName); setBufferLength(bLength); }
	BandpassSignal(t_unsigned_long bLength) { setType("BandpassSignal", signal_value_type::ComplexValue); setBufferLength(bLength); }
	BandpassSignal(){ setType("BandpassSignal", signal_value_type::ComplexValue); if (buffer == nullptr) buffer = new t_complex[getBufferLength()]; }

	void setBufferLength(t_unsigned_long bLength) { Signal::setBufferLength(bLength); delete[] buffer; if (bLength != 0) buffer = new t_complex[bLength]; };

};

// the setType is BandpassSignal to garantee the compatibility with the Visualizer
class OpticalSignal : public Signal {
public:
	OpticalSignal(string fName) { setType("BandpassSignal", signal_value_type::ComplexValue); setFileName(fName); if (buffer == nullptr) buffer = new t_complex[getBufferLength()]; }
	OpticalSignal(string fileName, string folderName) : Signal(fileName,folderName) { setType("BandpassSignal", signal_value_type::ComplexValue); if (buffer == nullptr) buffer = new t_complex[getBufferLength()];}
	OpticalSignal(string fName, t_unsigned_long bLength) { setType("BandpassSignal", signal_value_type::ComplexValue); setFileName(fName); setBufferLength(bLength); }
	OpticalSignal(t_unsigned_long bLength) { setType("BandpassSignal", signal_value_type::ComplexValue); setBufferLength(bLength); }
	OpticalSignal() { setType("BandpassSignal", signal_value_type::ComplexValue); if (buffer == nullptr) buffer = new t_complex[getBufferLength()]; }

	void setBufferLength(t_unsigned_long bLength) { Signal::setBufferLength(bLength); delete[] buffer; if (bLength != 0) buffer = new t_complex[bLength]; };

};


class OpticalSignalXY : public Signal {
public:
	OpticalSignalXY(string fName) { setType("OpticalSignalXY", signal_value_type::ComplexValueXY); setFileName(fName); if (buffer == nullptr) buffer = new t_complex_xy[getBufferLength()]; }
	OpticalSignalXY(string fileName, string folderName) : Signal(fileName, folderName) { setType("OpticalSignalXY", signal_value_type::ComplexValueXY); if (buffer == nullptr) buffer = new t_complex_xy[getBufferLength()];}
	OpticalSignalXY(string fName, t_unsigned_long bLength) { setType("OpticalSignalXY", signal_value_type::ComplexValueXY); setFileName(fName); setBufferLength(bLength); }
	OpticalSignalXY(t_unsigned_long bLength) { setType("OpticalSignalXY", signal_value_type::ComplexValueXY); setBufferLength(bLength); }
	OpticalSignalXY() { setType("OpticalSignalXY", signal_value_type::ComplexValueXY); if (buffer == nullptr) buffer = new t_complex_xy[getBufferLength()]; }

	void setBufferLength(t_unsigned_long bLength) { Signal::setBufferLength(bLength); delete[] buffer; if (bLength != 0) buffer = new t_complex_xy[bLength]; };
};
*/

/*class MultiModeBandpassSignal : BandpassSignal {
public:
	MultiModeBandpassSignal(int nBandpassSignals) {

	};
private:
	int numberOfBandpassSignals;
	vector<BandpassSignal> bandpasslSignals;
	vector<double> centralWavelengths;
	vector<double> centralFrequencies;
};*/
/*
class PhotonStream : public Signal {

public:
	PhotonStream(string fileName, string folderName) : Signal(fileName, folderName) { setType("PhotonStream", signal_value_type::PhotonValue); if (buffer == nullptr) buffer = new t_photon[getBufferLength()];}
	PhotonStream(t_unsigned_long bLength) { setType("PhotonStream", signal_value_type::PhotonValue); setBufferLength(bLength); }
	PhotonStream() { setType("PhotonStream", signal_value_type::PhotonValue); if (buffer == nullptr) buffer = new t_photon[getBufferLength()]; }

	void setBufferLength(t_unsigned_long bLength) { Signal::setBufferLength(bLength); delete[] buffer; if (bLength != 0) buffer = new t_photon[bLength]; };
};

class PhotonStreamXY : public Signal {

public:
	PhotonStreamXY(string fName) { setType("PhotonStreamXY", signal_value_type::ComplexValueXY); setFileName(fName); if (buffer == nullptr) buffer = new t_complex_xy[getBufferLength()]; }
	PhotonStreamXY(string fileName, string folderName) : Signal(fileName, folderName) { setType("PhotonStreamXY", signal_value_type::ComplexValueXY); if (buffer == nullptr) buffer = new t_complex_xy[getBufferLength()];	}
	PhotonStreamXY(string fName, t_unsigned_long bLength) { setType("PhotonStreamXY", signal_value_type::ComplexValueXY); setFileName(fName); setBufferLength(bLength); }
	PhotonStreamXY(t_unsigned_long bLength) { setType("PhotonStreamXY", signal_value_type::ComplexValueXY); setBufferLength(bLength); }
	PhotonStreamXY() { setType("PhotonStreamXY", signal_value_type::ComplexValueXY); if (buffer == nullptr) buffer = new t_complex_xy[getBufferLength()]; }

	void setBufferLength(t_unsigned_long bLength) { Signal::setBufferLength(bLength); delete[] buffer; if (bLength != 0) buffer = new t_complex_xy[bLength]; };
};

class PhotonStreamMP : public Signal {

public:
	PhotonStreamMP(string fileName, string folderName) : Signal(fileName, folderName) { setType("PhotonStreamMP", signal_value_type::PhotonValueMP); if (buffer == nullptr) buffer = new t_photon_mp[getBufferLength()]; }
	PhotonStreamMP(t_unsigned_long bLength) { setType("PhotonStreamMP", signal_value_type::PhotonValueMP); setBufferLength(bLength); }
	PhotonStreamMP() { setType("PhotonStreamMP", signal_value_type::PhotonValueMP); if (buffer == nullptr) buffer = new t_photon_mp[getBufferLength()]; }

	void setBufferLength(t_unsigned_long bLength) { Signal::setBufferLength(bLength); delete[] buffer; if (bLength != 0) buffer = new t_photon_mp[bLength]; };

};

class PhotonStreamMPXY : public Signal {

public:
	PhotonStreamMPXY(string fName) { setType("PhotonStreamMPXY", signal_value_type::PhotonValueMPXY); setFileName(fName); if (buffer == nullptr) buffer = new t_photon_mp_xy[getBufferLength()]; }
	PhotonStreamMPXY(string fileName, string folderName) : Signal(fileName, folderName) { setType("PhotonStreamMPXY", signal_value_type::PhotonValueMPXY); if (buffer == nullptr) buffer = new t_photon_mp_xy[getBufferLength()];}
	PhotonStreamMPXY(string fName, t_unsigned_long bLength) { setType("PhotonStreamMPXY", signal_value_type::PhotonValueMPXY); setFileName(fName); setBufferLength(bLength); }
	PhotonStreamMPXY(t_unsigned_long bLength) { setType("PhotonStreamMPXY", signal_value_type::PhotonValueMPXY); setBufferLength(bLength); }
	PhotonStreamMPXY() { setType("PhotonStreamMPXY", signal_value_type::PhotonValueMPXY); if (buffer == nullptr) buffer = new t_photon_mp_xy[getBufferLength()]; }

	void setBufferLength(t_unsigned_long bLength) { setBufferLength(bLength); delete[] buffer; if (bLength != 0) buffer = new t_photon_mp_xy[bLength]; };
};

class Messages : public Signal {
public:
	Messages(string fName) { setType("Message", signal_value_type::Message); setFileName(fName); if (buffer == nullptr) buffer = new t_message[getBufferLength()]; }
	Messages(string fileName, string folderName) : Signal(fileName, folderName) { setType("Message", signal_value_type::Message); if (buffer == nullptr) buffer = new t_message[getBufferLength()];}
	Messages(string fName, t_unsigned_long bLength) { setType("Message", signal_value_type::Message); setFileName(fName); setBufferLength(bLength); }
	Messages(t_unsigned_long bLength) { setType("Message", signal_value_type::Message); setBufferLength(bLength); }
	Messages() { setType("Message", signal_value_type::Message); if (buffer == nullptr) buffer = new t_message[getBufferLength()]; }

	void setBufferLength(t_unsigned_long bLength) { Signal::setBufferLength(bLength); delete[] buffer; if (bLength != 0) buffer = new t_message[bLength]; };
	
	void bufferPut(t_message);
};
*/

//########################################################################################################################################################
//########################################################## GENERIC BLOCK DECLARATIONS AND DEFINITIONS ##################################################
//########################################################################################################################################################

// Descrives a generic Block.
class Block {

public:

	/* Methods */
	Block(){};
	Block(vector<Signal*> &InputSig, vector<Signal*> &OutputSig);
	Block(initializer_list<Signal*> InputSig, initializer_list<Signal*> OutputSig);


	//void initializeBlock(vector<Signal*> InputSig, vector<Signal*> OutputSig);
	void initializeBlock();

	virtual void initialize(void){};

	virtual bool runBlock();

	void terminateBlock();
	virtual void terminate(void){};

	void setNumberOfInputSignals(int nOfInputSignal) { numberOfInputSignals = nOfInputSignal; };
	int getNumberOfInputSignals() { return numberOfInputSignals; };

	void setNumberOfOutputSignals(int nOfOutputSignal) { numberOfOutputSignals = nOfOutputSignal; };
	int getNumberOfOutputSignals() { return numberOfOutputSignals; };

	vector<Signal *> inputSignals;
	vector<Signal *> outputSignals;

	int numberOfInputSignals;	// passar para private, 2018/07/15
	int numberOfOutputSignals;  // passar para private, 2018/07/15

};


//########################################################################################################################################################
//########################################################## GENERIC SYSTEM DECLARATIONS AND DEFINITIONS #################################################
//########################################################################################################################################################


class System {

public:

	//######################################################################################################

	System() {};
	void setSystem(initializer_list<Block *> MainSystem);

	System(initializer_list<Block *> MainSystem);
	//System(initializer_list<Block> MainSystme);
	//System(vector<Block *> MainSystem);
	System(initializer_list<Block *> MainSystem, string signalsFolderName, vector<string> list);
	//System(vector<Block *> MainSystem, string signalsFolderName, vector<string> list);

	bool run();
	bool run(string signalPath);
	void terminate();

	//########################################################################################################

	void setLogValue(bool value);
	bool getLogValue() { return logValue; };
	void setOpenFile(bool value);
	bool getOpenFile() { return openFile; };
	void setLogFileName(string newName);
	string getLogFileName() { return logFileName; };
	void setSignalsFolderName(string newName);
	string getSignalsFolderName() { return signalsFolder; };
	void setLoadedInputParameters(vector<string> loadedInputParams);
	vector<string> getLoadedInputParameters() { return loadedInputParameters; };
	size_t getSystemBlocksSize() { return SystemBlocks.size(); };
	vector<Block *> getSystemBlocks() { return SystemBlocks; };
	
	//########################################################################################################

private:
	string signalsFolder{ "signals" };
	char fileName[MAX_NAME_SIZE];  // Name of the file with system description (.sdf)
	char name[MAX_NAME_SIZE];  // System Name
	int numberOfBlocks;  // Number of system Blocks
	int(*topology)[MAX_TOPOLOGY_SIZE];  // Relationship matrix
	vector<Block *> SystemBlocks;  // Pointer to an array of pointers to Block objects
								   //Log File Inputs
	string logFileName{ "log.txt" }; // The name of the file where the debug info will be written
	bool logValue{ true }; // Will write debug info if true
	bool openFile{ true };
	ofstream logFile;
	vector<string> loadedInputParameters;
};

//########################################################################################################################################################
//#################################################### GENERIC SUPER BLOCK DECLARATIONS AND DEFINITIONS ##################################################
//########################################################################################################################################################


class SuperBlock : public Block {

	/* State Variables */

	vector<Block*> moduleBlocks;
	vector<Block*> blocks;
	System superBlockSystem;

	/* Input Parameters */

	bool saveInternalSignals{ false };

public:


	/* Methods */
	//SuperBlock() {};
	//SuperBlock(vector<Signal *> &inputSignals, vector<Signal *> &outputSignal) : Block(inputSignals, outputSignals) { superBlockSystem.setLogValue(false); };
	SuperBlock(initializer_list<Signal *> inputSignals, initializer_list<Signal *> outputSignals) : Block(inputSignals, outputSignals) { superBlockSystem.setLogValue(false); };

	void initialize(void);

	virtual bool runBlock(string signalPath);

	void terminate(void);

	/* Set Methods */

	//void setBlocks(vector<Block*> blks) { blocks = blks; };
	void setSuperBlockSystem(initializer_list<Block*> blks) { superBlockSystem.setSystem(blks); };

	void setSaveInternalSignals(bool sSignals);
	bool const getSaveInternalSignals(void) { return saveInternalSignals; };

	void setLogValue(bool value) { superBlockSystem.setLogValue(value);	};
	void setOpenFile(bool value) { superBlockSystem.setOpenFile(value); };
	void setLogFileName(string newName) { superBlockSystem.setLogFileName(newName); };
	void setSignalsFolderName(string newName) { superBlockSystem.setSignalsFolderName(newName); };
	void setLoadedInputParameters(vector<string> loadedInputParams) { superBlockSystem.setLoadedInputParameters(loadedInputParams); };
private:
	ofstream logFileSP;
};

//########################################################################################################################################################
//###################################################################### LINEAR FILTERS ##################################################################
//########################################################################################################################################################


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
	FIR_Filter(initializer_list<Signal *> InputSig, initializer_list<Signal *> OutputSig) :Block(InputSig, OutputSig) {};
	FIR_Filter(vector<Signal *> &InputSig, vector<Signal *> &OutputSig) :Block(InputSig, OutputSig) {};
	FIR_Filter() {};

	void initializeFIR_Filter(void);

	bool runBlock(void);

	void terminate(void){};

	void setSaveImpulseResponse(bool sImpulseResponse) { saveImpulseResponse = sImpulseResponse; };
	bool getSaveImpulseResponse(void){ return saveImpulseResponse; };

	void setImpulseResponseLength(int iResponseLength) { impulseResponseLength = iResponseLength; };
	int const getImpulseResponseLength(){ return impulseResponseLength; }

	void setImpulseResponseFilename(string fName) { impulseResponseFilename = fName; };
	string getImpulseResponseFilename() { return impulseResponseFilename; };

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
	FD_Filter(vector<Signal *> &InputSig, vector<Signal *> &OutputSig) :Block(InputSig, OutputSig) {};
	FD_Filter(initializer_list<Signal *> InputSig, initializer_list<Signal *> OutputSig) :Block(InputSig, OutputSig) {};

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
//############################################################### GENERIC DSP FUNCTIONS ##################################################################
//########################################################################################################################################################


/*class OverlapMethod
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
};*/


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

class FourierTransform {

public:

	vector<complex<double>> fft(vector<complex<double> > &vec, int sign);
	vector<complex<double>> fft(std::vector<std::complex<double> > &vec);
	vector<complex<double>> ifft(std::vector<std::complex<double> > &vec);
	void transformRadix2(std::vector<std::complex<double> > &vec);
	void transformBluestein(std::vector<std::complex<double> > &vec);
	void convolve(
		const std::vector<std::complex<double> > &vecx,
		const std::vector<std::complex<double> > &vecy,
		std::vector<std::complex<double> > &vecout);
};

///////////////////// System Parameters ////////////////////////

class SystemInputParameters {
private:
	vector<string> loadedInputParameters;
	string inputParametersFileName{ "input_parameters_0.txt" }; //name of the file from where the input parameters will be read
	string outputFolderName{ "signals" };
	enum ParameterType { INT, DOUBLE, BOOL }; //types of parameters
											  //A parameter can only be of 1 type
	class Parameter {
	private:
		//Type of the parameter. this type should be hidden from the user
		ParameterType type;
		union { //pointers to the parameter
			int* i;
			double* d;
			bool* b;
		};

	public:
		//Changes the value of the parameter
		void setValue(int value);
		void setValue(double value);
		void setValue(bool value);
		ParameterType getType();
		//Constructor for parameter of type int
		Parameter(int* elem);
		//Constructor for parameter of type double
		Parameter(double* elem);
		//Constructor for parameter of type bool
		Parameter(bool* elem);
	};

	int parseInt(string str);
	double parseDouble(string str);
	bool parseBool(string str);
	vector<string> split(const string & text, char sep);
	map<string, Parameter*> parameters = map<string, Parameter*>(); //Maps the names of the variables to the addresses of the parameters

public:
	string getInputParametersFileName() { return inputParametersFileName; }
	string getOutputFolderName() { return outputFolderName; }
	vector<string> getLoadedInputParameters() { return loadedInputParameters; }
	void readSystemInputParameters();
	//Adds the parameter to the map
	void addInputParameter(string name, int* variable);
	void addInputParameter(string name, double* variable);
	void addInputParameter(string name, bool* variable);
	/* Default empty constructor. Initializes the map */
	SystemInputParameters(){}
	SystemInputParameters(int argc,char*argv[]);
	/* Deletes all heap memory occupied by the parameters */
	~SystemInputParameters();
};

//########################################################################################################################################################
//############################################################### RANDOM NUMBER GENERATORS ##################################################################
//########################################################################################################################################################


// Generates integer numbers uniformely distribuited in the interval [lo,hi)
template<typename T = random_engine >
class UniformRandomIntegerNumbers {
	UniformRandomIntegerNumbers(int low, int high) : r(bind(uniform_int_distribution<>(low, high), T())) { };
	UniformRandomIntegerNumbers(long int seed, int low, int high) : r(bind(uniform_int_distribution<>(low, high), T(seed))) { };
	int operator()() const { return r(); }
private:
	function<int()> r;
};

// Generates real numbers (double) uniformely distribuited in the interval [lo,hi)
template<typename T= random_engine >
class UniformRandomRealNumbers {
public:
	UniformRandomRealNumbers(double low, double high) :r(bind(uniform_real_distribution<>(low, high), T())) { };
	UniformRandomRealNumbers(long int seed, double low, double high) :r(bind(uniform_real_distribution<>(low, high), T(seed))) { };
	double operator()() { return r(); };
private:
	function<double()> r;
};


# endif // PROGRAM_INCLUDE_netxpto_H_
