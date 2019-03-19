#include <algorithm>
#include <complex>
#include <fstream>

#include "netxpto_20180815.h"
#include "load_signal_20190205.h"


void LoadSignal::initialize(void){


//############################################################################################
//################################# Reading From Signal Header ###############################
//############################################################################################
	
	ifstream inFile;
	inFile.open("./signals/" + sgnFileName);

	if (!inFile) cout << "ERROR: load_signal.cpp (can't open input file!)" << "\n";
	
	int headerLines = 4;
	vector<string> line(headerLines);
	

	for (int i = 0; i < headerLines; i++)
	{
		getline(inFile, line[i]);
		headerLength += line[i].length() + 2;
	}
/*	// this code checks if carrier return (\r) is used, if so it subtracts the length
	inFile.clear();
	inFile.seekg(0, ios::beg);
	char * buffer;
	buffer = new char[headerLength];
	inFile.read(buffer, headerLength);
	char * pch;
	pch = strchr(buffer, '\r\n');

	//if (pch == NULL) headerLength -= 4;

	inFile.close(); */

	double symbolPeriod = stod(line[1].substr(18));
	double samplingPeriod = stod(line[2].substr(20));

	outputSignals[0]->setSymbolPeriod(symbolPeriod);
	outputSignals[0]->setSamplingPeriod(samplingPeriod);
}


bool LoadSignal::runBlock(void){



	ifstream inFile;
	inFile.open("./signals/" + sgnFileName, ios::binary);
	inFile.seekg(headerLength);


	int space = outputSignals[0]->space();

	if (space == 0 || endOfFile) return false;

	if (!inFile) {
		cout << "ERROR: load_signal.cpp (can't open input file!)" << "\n";
		return false;
	}

	signal_value_type signalTypeOut = outputSignals[0]->getValueType();
	switch (signalTypeOut)
	{
	case signal_value_type::t_binary:
	{
		t_binary out;

		int length = sizeof(unsigned int);
		char * memblock;
		memblock = new char[length];

		for (int i = 0; i < count; i++) inFile.read(memblock, length);

		int i = 0;
		do
		{
			i++;
			inFile.read(memblock, length);
			if (inFile.rdstate())
			{
				endOfFile = true;
				break;
			}
			unsigned int * binary_values = (unsigned int*)memblock;
			out = *binary_values;
			outputSignals[0]->bufferPut(out);
		} while (i < space);

		count += i;
		break;
	}
/*	case signal_value_type::t_integer:
	{
		t_integer out;

		int length = sizeof(int);
		char * memblock;
		memblock = new char[length];

		for (size_t i = 0; i < count; i++) inFile.read(memblock, length);

		int i = 0;
		do
		{
			i++;
			inFile.read(memblock, length);
			if (inFile.rdstate())
			{
				endOfFile = true;
				break;
			}
			int * integer_values = (int*)memblock;
			out = *integer_values;
			outputSignals[0]->bufferPut(out);
		} while (i < space);

		count += i;
		break;
	}*/
	case signal_value_type::t_real:
	{
		t_real out;
		
		int length = sizeof(double);
		char * memblock;
		memblock = new char[length];

		for (int i = 0; i < count; i++) inFile.read(memblock, length);

		int i = 0;
		do
		{
			i++;
			inFile.read(memblock, length);
			if (inFile.rdstate()) 
			{
				endOfFile = true;
				break;
			}
			double * double_values = (double*)memblock;
			out = *double_values;
			outputSignals[0]->bufferPut(out);
		} while ( i < space );

		count += i;
		break;
	}
	case signal_value_type::t_complex:
	{
		t_complex out;

		int length = sizeof(double);
		char * memblockReal;
		char * memblockImag;
		memblockReal = new char[length];
		memblockImag = new char[length];

		for (int i = 0; i < 2 * count; i++) inFile.read(memblockReal, length);

		int i = 0;
		do
		{
			i++;

			inFile.read(memblockReal, length);
			inFile.read(memblockImag, length);
			if (inFile.rdstate())
			{
				endOfFile = true;
				break;
			}
			double * real = (double*)memblockReal;
			double * imag = (double*)memblockImag;
			out = complex<double>(*real, *imag);
			outputSignals[0]->bufferPut(out);
		} while (i < space);

		count += i;
		break;
	}
	case signal_value_type::t_complex_xy:
	{
		t_complex_xy out;

		int length = sizeof(double);
		char * memblockRealX;
		char * memblockImagX;
		memblockRealX = new char[length];
		memblockImagX = new char[length];
		char * memblockRealY;
		char * memblockImagY;
		memblockRealY = new char[length];
		memblockImagY = new char[length];

		for (int i = 0; i < 4 * count; i++) inFile.read(memblockRealX, length);

		int i = 0;
		do
		{
			i++;

			inFile.read(memblockRealX, length);
			inFile.read(memblockImagX, length);
			inFile.read(memblockRealY, length);
			inFile.read(memblockImagY, length);
			if (inFile.rdstate())
			{
				endOfFile = true;
				break;
			}
			double * realX = (double*)memblockRealX;
			double * imagX = (double*)memblockImagX;
			double * realY = (double*)memblockRealY;
			double * imagY = (double*)memblockImagY;
			t_complex outX(*realX, *imagX);
			t_complex outY(*realY, *imagY);
			out = { outX, outY };
			outputSignals[0]->bufferPut(out);
		} while (i < space);

		count += i;
		break;
	}
/*	case signal_value_type::t_photon:
	{
		t_photon out;
		
		int length = sizeof(double);
		char * memblockProbability;
		char * memblockPolariztion;
		memblockProbability = new char[length];
		memblockPolariztion = new char[length];
		
		for (size_t i = 0; i < 2 * count; i++) inFile.read(memblockProbability, length);

		int i = 0;
		do
		{
			i++;

			inFile.read(memblockProbability, length);
			inFile.read(memblockPolariztion, length);
			if (inFile.rdstate())
			{
				endOfFile = true;
				break;
			}
			double * probability = (double*)memblockProbability;
			double * polariztion = (double*)memblockPolariztion;
			
			
			out = { *probability, *polariztion};
			outputSignals[0]->bufferPut(out);
		} while (i < space);

		count += i;
		break;
	}
	case signal_value_type::t_photon_mp:
	{
		t_photon_mp out;

		int length = sizeof(double);
		char * memblockProbability1;
		char * memblockPolariztion1;
		memblockProbability1 = new char[length];
		memblockPolariztion1 = new char[length];
		char * memblockProbability2;
		char * memblockPolariztion2;
		memblockProbability2 = new char[length];
		memblockPolariztion2 = new char[length];

		for (size_t i = 0; i < 4 * count; i++) inFile.read(memblockProbability1, length);

		int i = 0;
		do
		{
			i++;

			inFile.read(memblockProbability1, length);
			inFile.read(memblockPolariztion1, length);
			inFile.read(memblockProbability2, length);
			inFile.read(memblockPolariztion2, length);
			if (inFile.rdstate())
			{
				endOfFile = true;
				break;
			}
			double * probability1 = (double*)memblockProbability1;
			double * polariztion1 = (double*)memblockPolariztion1;
			double * probability2 = (double*)memblockProbability2;
			double * polariztion2 = (double*)memblockPolariztion2;
			out.path[0] = { *probability1, *polariztion1 };
			out.path[1] = { *probability2, *polariztion2 };

			outputSignals[0]->bufferPut(out);
		} while (i < space);

		count += i;
		break;
	}*/
	case signal_value_type::t_photon_mp_xy:
	{
		t_photon_mp_xy out;
		
		int length = sizeof(double);
		char * memblockRealX1;
		char * memblockImagX1;
		memblockRealX1 = new char[length];
		memblockImagX1 = new char[length];
		char * memblockRealY1;
		char * memblockImagY1;
		memblockRealY1 = new char[length];
		memblockImagY1 = new char[length];

		char * memblockRealX2;
		char * memblockImagX2;
		memblockRealX2 = new char[length];
		memblockImagX2 = new char[length];
		char * memblockRealY2;
		char * memblockImagY2;
		memblockRealY2 = new char[length];
		memblockImagY2 = new char[length];


		for (int i = 0; i < 8 * count; i++) inFile.read(memblockRealX1, length);

		int i = 0;
		do
		{
			i++;

			inFile.read(memblockRealX1, length);
			inFile.read(memblockImagX1, length);
			inFile.read(memblockRealY1, length);
			inFile.read(memblockImagY1, length); 
			
			inFile.read(memblockRealX2, length);
			inFile.read(memblockImagX2, length);
			inFile.read(memblockRealY2, length);
			inFile.read(memblockImagY2, length);

			if (inFile.rdstate())
			{
				endOfFile = true;
				break;
			}
			double * realX1 = (double*)memblockRealX1;
			double * imagX1 = (double*)memblockImagX1;
			double * realY1 = (double*)memblockRealY1;
			double * imagY1 = (double*)memblockImagY1;
			t_complex outX1(*realX1, *imagX1);
			t_complex outY1(*realY1, *imagY1);
			out.path[0] = { outX1, outY1 };

			double * realX2 = (double*)memblockRealX2;
			double * imagX2 = (double*)memblockImagX2;
			double * realY2 = (double*)memblockRealY2;
			double * imagY2 = (double*)memblockImagY2;
			t_complex outX2(*realX2, *imagX2);
			t_complex outY2(*realY2, *imagY2);
			out.path[1] = { outX2, outY2 };


			outputSignals[0]->bufferPut(out);
		} while (i < space);

		count += i;
		break;
	}
	default:
	{
		if (!inFile) {
			cout << "ERROR: load_signal.cpp (unrecognized signal type!)" << "\n";
			return false;
		}
		break;
	}
	}
	
	inFile.close();
	
	return true;
}
