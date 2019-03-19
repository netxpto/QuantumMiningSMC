#include "netxpto.h"
#include "hilbertFilter.h"
#include "overlapSave_20180208.h"


void HilbertRoot::initialize_HilbertRoot(void)
{
		///////////////////////////////////////////////////////////////////////////////
		/////////////////////////////////// FREQ //////////////////////////////////////
		///////////////////////////////////////////////////////////////////////////////
	
		outputSignals[0]->symbolPeriod = inputSignals[0]->symbolPeriod;
		outputSignals[0]->samplingPeriod = inputSignals[0]->samplingPeriod;
		outputSignals[0]->samplesPerSymbol = inputSignals[0]->samplesPerSymbol;

		/*if (!getSeeBeginningOfImpulseResponse()) {
			int aux = (int)(((double)impulseResponseLength) / 2) + 1;
			outputSignals[0]->setFirstValueToBeSaved(aux);
		}*/

		if (saveTransferFunction)
		{
			ofstream fileHandler("./signals/" + transferFunctionFilename, ios::out);
			fileHandler << "// ### HEADER TERMINATOR ###\n";

			double samplingPeriod = inputSignals[0]->samplingPeriod;
			t_real fWindow = 1 / samplingPeriod;
			t_real df = fWindow / transferFunction.size();

			t_real f;
			for (int k = 0; k < transferFunction.size(); k++)
			{
				f = -fWindow / 2 + k * df;
				fileHandler << f << " " << transferFunction[k] << "\n";
			}
			fileHandler.close();
		}
};


vector<t_complex> overlapSaveForTransferFunction(std::vector<std::complex<double> > &inTimeDomainComplex1, std::vector<std::complex<double> > &inTimeDomainComplex2, std::vector<std::complex<double> > &inTimeDomainFilterComplex);
bool HilbertRoot::runBlock(void) {

		bool alive{ false };

		int ready = inputSignals[0]->ready();
		int space = outputSignals[0]->space();
		int process = min(ready, space);
		if (process == 0) return false;


		vector<t_real> re(process);
		vector<t_real> im(process);
		vector<t_complex> IN1(process);
		vector<t_complex> OUTaux;
		vector<t_complex> OUT;
		t_real input;
		vector<double> inputBufferTimeDomain(process);
		vector<t_complex>  currentCopyAux;
		vector<double> c(2*process);
		vector<double> d(2*process);
		vector<complex<double>> hilbertTransformerFrequencyDomain(2*process);
		
		
		/////////////////////////////// previousCopy & currentCopy ///////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////////////
		for (int i = 0; i < process; i++)					// Get the Input signal as a vector of size "n"
		{
			inputSignals[0]->bufferGet(&input);
			inputBufferTimeDomain.at(i) = input;
		}


		for (int i = 0; i < process; i++)
		{
			re[i] = inputBufferTimeDomain.at(i);		    // Real part of input
			im[i] = 0;										// Imaginary part which is manipulated as "0"
		}
		currentCopyAux = ReImVect2ComplexVector(re, im);	// CURRENT COPY complex form


		vector<t_complex> pcInitialize(process);
		if (K == 0)											// For the first data block only
		{
			previousCopy = pcInitialize;					// PREVIOUS COPY
		}

		vector<t_complex> currentCopy(previousCopy.size());	// size modification of currentCopy
		for (unsigned int i = 0; i < currentCopyAux.size(); i++)
		{
			currentCopy[i] = currentCopyAux[i];
		}

		vector<t_complex> yAux;
		vector<t_complex> y(process);
		yAux = overlapSaveForTransferFunction(currentCopy, previousCopy, transferFunction);

		previousCopy = currentCopy;
		K = K + 1;

		for (unsigned int i = 0; i < process; i++)
		{
			y[i] = yAux[previousCopy.size()+i];
		}

		for (unsigned int i = 0; i < process; i++)
		{
			t_real val;
			val = y[i].real();
			outputSignals[0]->bufferPut((t_real)(val));
		}

		return true;
	
};

vector<t_complex> overlapSaveForTransferFunction(std::vector<std::complex<double> > &inTimeDomainComplex1, std::vector<std::complex<double> > &inTimeDomainComplex2, std::vector<std::complex<double> > &inTimeDomainFilterComplex)
{
	vector <t_complex> current  = inTimeDomainComplex1;
	vector <t_complex> previous = inTimeDomainComplex2;
	vector<complex<double>> data = inTimeDomainComplex2;
	vector<complex<double>> hilbertFilter = inTimeDomainFilterComplex;
	
	for (unsigned int i = 0; i < current.size(); i++)
	{
		data.push_back(current[i]);
	}

	if (hilbertFilter.size() == data.size())
	{
		hilbertFilter = hilbertFilter;
	}
	else
	{
		size_t NS = data.size();
		vector<double> c(NS);
		vector<double> d(NS);
		vector<complex<double>> hilbertTransformerFrequencyDomain(NS);
		for (int i = 0; i < NS; i++)
		{
			if ((i>0) && (i<NS / 2))
			{
				c.at(i) = 0;
				d.at(i) = -1;
			}

			if (i >= NS / 2)
			{
				c.at(i) = 0;
				d.at(i) = 1;
			}

			if (i == 0)
			{
				c.at(i) = 0;
				d.at(i) = 0;
			}
		}

		hilbertFilter = ReImVect2ComplexVector(c, d);	// create complex vector from real and imag data vector of the hilbert transformer
	
	}

	vector<complex<double>> dataModifiedAux(data.size());
	dataModifiedAux = data;
	vector<complex<double>> dataModifiedFreq(data.size());
	dataModifiedFreq = fft(dataModifiedAux);


	for (unsigned int i = 0; i < dataModifiedFreq.size(); i++)
	{
		dataModifiedFreq[i] = dataModifiedFreq[i] * hilbertFilter[i];
	}


	vector<complex<double>> dataModifiedTime(data.size());
	dataModifiedTime = ifft(dataModifiedFreq);

	for (unsigned int i = 0; i < dataModifiedTime.size(); i++)
	{
		dataModifiedTime[i] = dataModifiedTime[i] ;
	}


	return dataModifiedTime;
}



void HilbertTransform::initialize(void)
{
	double samplingPeriod = inputSignals[0]->samplingPeriod;
	double symbolPeriod = inputSignals[0]->symbolPeriod;

	int process = transferFunctionFrequencyLength;

	vector<double> c(2*process);
	vector<double> d(2*process);
	vector<complex<double>> hilbertTransformerFrequencyDomain(2*process);

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////// Hilbert transformer in frequency domain ///////////////
	for (int i = 0; i < 2 *process; i++)
	{
		if ((i>0) && (i<2 * process / 2))
		{
			c.at(i) = 0;
			d.at(i) = -1;
		}

		if (i >= 2 * process / 2)
		{
			c.at(i) = 0;
			d.at(i) = 1;
		}

		if (i == 0)
		{
			c.at(i) = 0;
			d.at(i) = 0;
		}
	}

	transferFunction = ReImVect2ComplexVector(c, d);	// create complex vector from real and imag data vector of the hilbert transformer
	transferFunctionLength = transferFunction.size();

	initialize_HilbertRoot();
}
