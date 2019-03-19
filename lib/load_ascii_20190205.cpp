#include <algorithm>
#include <complex>
#include <fstream>

#include "netxpto_20180815.h"
#include "load_ascii_20190205.h"


void LoadAscii::initialize(void){

	outputSignals[0]->setSymbolPeriod(symbolPeriod);
	outputSignals[0]->setSamplingPeriod(samplingPeriod);
}


bool LoadAscii::runBlock(void){


	ifstream inFile;
	inFile.open(asciiFileName);

	if (!inFile) {
		cout << "ERROR: load_ascii.cpp (can't open input file!)" << "\n";
		return false;
	}

	int space = outputSignals[0]->space();
	signal_value_type sTypeOut = outputSignals[0]->getValueType();

	
	switch (delimiterType)
	{
		case delimiter_type::ConcatenatedValues:
		{
			if (firstTime)
			{
				firstTime = false;
				inFile.seekg(0, inFile.end);
				sizeOfFile = (int)(inFile.tellg());
				ready = sizeOfFile;
				inFile.seekg(0, inFile.beg);
			}
			else
			{
				inFile.seekg(sizeOfFile - ready);
			}

			int process = min(ready, space);
			if (process == 0)
			{
				inFile.close();
				return false;
			}

			ready -= process;

			switch (dataType)
			{
				case signal_value_type::t_binary:
				{
					char in;
					t_binary out;
					for (int i = 0; i < process; i++)
					{
						in = inFile.get();
						if (in == '0') out = 0;
						else out = 1;
						outputSignals[0]->bufferPut(out);
					}
					break;
				}
				default:
					cout << "ERROR: load_ascii.cpp (this is not a good way to save non-binary ascii)" << "\n";
					break;
			}
			break;
		}

		case delimiter_type::CommaSeperatedValues:
		{
			string s;
			if (firstTime)
			{
				firstTime = false;
				streampos startPosition = inFile.tellg();  // stores the position

				while (!inFile.eof()) // this loops counts the number of lines in the file
				{
					getline(inFile, s);
					sizeOfFile++;
				}

				if (s == "") sizeOfFile--; // checks if the final line is blank
				ready = sizeOfFile;
				inFile.clear(); // clears eof flag
				inFile.seekg(startPosition);
			}
			else
			{
				for (auto i = 0; i < sizeOfFile - ready; i++) getline(inFile, s); //this seeks into the point where the code left off in the last run through
			}

			int process = min(ready, space);
			if (process == 0)
			{
				inFile.close();
				return false;
			}

			ready -= process; // this updates the remaining values in the file
		
			switch (dataType)
			{
		
			case signal_value_type::t_binary:
				{
					if (sTypeOut != signal_value_type::t_binary)
					{
						cout << "ERROR: load_ascii.cpp (signal type mismatch!)" << "\n";
						inFile.close();
						return false;
					}
					string in;
					t_binary out;
					for (int i = 0; i < process; i++)
					{
						getline(inFile, in);
						if (in == "0") out = 0;
						else out = 1;
						outputSignals[0]->bufferPut(out);
					}
					break;
				}

				case signal_value_type::t_real:
				{
					if (sTypeOut != signal_value_type::t_real)
					{
						cout << "ERROR: load_ascii.cpp (signal type mismatch!)" << "\n";
						inFile.close();
						return false;
					}
					string in;
					t_real out;
					for (int i = 0; i < process; i++)
					{
						getline(inFile, in);
						out = stod(in);
						outputSignals[0]->bufferPut(out);
					}
					break;
				}

				case signal_value_type::t_complex:
				{
					if (sTypeOut != signal_value_type::t_complex)
					{
						cout << "ERROR: load_ascii.cpp (signal type mismatch!)" << "\n";
						inFile.close();
						return false;
					}
					string line;
					string in;
					double real;
					double imag;
					for (int i = 0; i < process; i++)
					{
						getline(inFile, line);
						stringstream inLine(line);
						for (int j = 0; j < 2; j++)
						{
							getline(inLine, in, ',');
							if (j == 0) real = stod(in);
							else imag = stod(in);
						}
						t_complex out(real, imag);
						outputSignals[0]->bufferPut(out);
					}
					break;
				}
				case signal_value_type::t_complex_xy:
				{
					if (sTypeOut != signal_value_type::t_complex_xy)
					{
						cout << "ERROR: load_ascii.cpp (signal type mismatch!)" << "\n";
						inFile.close();
						return false;
					}
					string line;
					string in;
					double realX;
					double imagX;
					double realY;
					double imagY;
					for (int i = 0; i < process; i++)
					{
						getline(inFile, line);
						stringstream inLine(line);
						for (int j = 0; j < 4; j++)
						{
							getline(inLine, in, ',');
							if (j == 0)      realX = stod(in);
							else if (j == 1) imagX = stod(in);
							else if (j == 2) realY = stod(in);
							else if (j == 3) imagY = stod(in);
						}

						t_complex X(realX, imagX);
						t_complex Y(realY, imagY);
						t_complex_xy out = { X, Y };
						outputSignals[0]->bufferPut(out);
					}
					break;
				}
			}
		break;
		}
	}
	inFile.close();
	return true;
}
