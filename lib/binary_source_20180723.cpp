# include "binary_source_20180723.h"

using namespace std;

void BinarySource::initialize(void) {

	numberOfOutputSignals = (int) outputSignals.size();

	for (unsigned int i = 0; i < numberOfOutputSignals; ++i) {
		outputSignals[i]->setSymbolPeriod(bitPeriod);
		outputSignals[i]->setSamplingPeriod(outputSignals[i]->getSymbolPeriod());
		outputSignals[i]->setSamplesPerSymbol(1.0);
		outputSignals[i]->setFirstValueToBeSaved(1);
	}
}

bool BinarySource::runBlock(void) {

	long int space = outputSignals[0]->space();
	for (unsigned int k = 1; k < numberOfOutputSignals; k++) {
		long int aux = outputSignals[k]->space();
		space = std::min(space, aux);
	}

	long int process;
	if (numberOfBits >= 0) {
		process = std::min(space, numberOfBits);
	}
	else {
		process = space;
	}

	if (process <= 0) return false;

	if (mode == BinarySourceMode::PseudoRandom) {

			if (acumul.size() == 0) {

				acumul.resize(33);

				for (int i = 0; i < 32; i += 2) {
					acumul[i] = 1;
					acumul[i + 1] = 0;
				}
				acumul[5] = 0;
				acumul[4] = 0;
				acumul[3] = 1;
				acumul[2] = 0;
				acumul[1] = 1;
				acumul[0] = 0;
			}

			int len = patternLength;

			vector<int>& ac = acumul;

			for (int k = 0; k < process; k++) {
				t_binary aux = (t_binary)ac[len];
				for (unsigned int l = 0; l < numberOfOutputSignals; ++l) {
					outputSignals[l]->bufferPut((t_binary)aux);
				}
				numberOfBits--;

				for (int i = len; i > 0; --i) ac[i] = ac[i - 1];

				switch (len) {
				case 1:
					ac[0] = ac[1];
					break;
				case 2:
					ac[0] = (ac[2] + ac[1]) % 2;
					break;
				case 3:
					ac[0] = (ac[3] + ac[1]) % 2;
					break;
				case 4:
					ac[0] = (ac[4] + ac[1]) % 2;
					break;
				case 5:
					ac[0] = (ac[5] + ac[2]) % 2;
					break;
				case 6:
					ac[0] = (ac[6] + ac[1]) % 2;
					break;
				case 7:
					ac[0] = (ac[7] + ac[1]) % 2;
					break;
				case 8:
					ac[0] = (ac[8] + ac[4] + ac[3] + ac[2]) % 2;
					break;
				case 9:
					ac[0] = (ac[9] + ac[4]) % 2;
					break;
				case 10:
					ac[0] = (ac[10] + ac[3]) % 2;
					break;
				case 11:
					ac[0] = (ac[11] + ac[2]) % 2;
					break;
				case 12:
					ac[0] = (ac[12] + ac[6] + ac[4] + ac[1]) % 2;
					break;
				case 13:
					ac[0] = (ac[13] + ac[4] + ac[3] + ac[1]) % 2;
					break;
				case 14:
					ac[0] = (ac[14] + ac[5] + ac[3] + ac[1]) % 2;
					break;
				case 15:
					ac[0] = (ac[15] + ac[1]) % 2;
					break;
				case 16:
					ac[0] = (ac[16] + ac[5] + ac[3] + ac[2]) % 2;
					break;
				case 17:
					ac[0] = (ac[17] + ac[3]) % 2;
					break;
				case 18:
					ac[0] = (ac[18] + ac[5] + ac[2] + ac[1]) % 2;
					break;
				case 19:
					ac[0] = (ac[19] + ac[5] + ac[2] + ac[1]) % 2;
					break;
				case 20:
					ac[0] = (ac[20] + ac[3]) % 2;
					break;
				case 21:
					ac[0] = (ac[21] + ac[2]) % 2;
					break;
				case 22:
					ac[0] = (ac[22] + ac[1]) % 2;
					break;
				case 23:
					ac[0] = (ac[23] + ac[5]) % 2;
					break;
				case 24:
					ac[0] = (ac[24] + ac[4] + ac[3] + ac[1]) % 2;
					break;
				case 25:
					ac[0] = (ac[25] + ac[3]) % 2;
					break;
				case 26:
					ac[0] = (ac[26] + ac[6] + ac[2] + ac[1]) % 2;
					break;
				case 27:
					ac[0] = (ac[27] + ac[5] + ac[2] + ac[1]) % 2;
					break;
				case 28:
					ac[0] = (ac[28] + ac[3]) % 2;
					break;
				case 29:
					ac[0] = (ac[29] + ac[2]) % 2;
					break;
				case 30:
					ac[0] = (ac[30] + ac[6] + ac[4] + ac[2]) % 2;
					break;
				case 31:
					ac[0] = (ac[31] + ac[3]) % 2;
					break;
				case 32:
					ac[0] = (ac[32] + ac[7] + ac[5] + ac[3] + ac[2] + ac[1]) % 2;
					break;
				}
			}
		}
	
	if (mode == BinarySourceMode::Random) {

		//std::random_device rd;
		//std::mt19937 gen(rd()); // I think using rd to seed the mt19937 may always produce the same sequence
		//std::uniform_int_distribution<> dis(0, 1);
		//std::default_random_engine rd((unsigned int)time(0));
		//std::uniform_real_distribution<> dis(0, 1);

		std::default_random_engine generator;
		std::uniform_real_distribution<double> distribution(0.0, 1.0);
		unsigned int seed = (unsigned int)chrono::system_clock::now().time_since_epoch().count();
		generator.seed(seed);

		for (int k = 0; k < process; k++) {
			t_binary aux;
			double number = distribution(generator);
			if (number < probabilityOfZero) {
				aux = 0;
			}
			else {
				aux = 1;
			};
			for (auto k = 0; k < numberOfOutputSignals; k++) {
				outputSignals[k]->bufferPut((t_binary)aux);
			}
			numberOfBits--;
		}
	}
	
	if (mode == BinarySourceMode::DeterministicCyclic) {
		std::vector<char> values(bitStream.begin(), bitStream.end());
		int valuesSize = (int) values.size();
		for (int k = 0; k < process; k++) {
			t_binary aux = (t_binary)(values[posBitStream++] - '0');
			for (unsigned int k = 0; k < numberOfOutputSignals; k++) {
				outputSignals[k]->bufferPut((t_binary)aux);
			}
			numberOfBits--;
			posBitStream = posBitStream % valuesSize;
		}

	}

	if (mode == BinarySourceMode::DeterministicAppendZeros){
		std::vector<char> values(bitStream.begin(), bitStream.end());
		int valuesSize = (int) values.size();
		t_binary aux;
		for (int k = 0; k < process; k++) {
			if (posBitStream == valuesSize) {
				aux = 0;
			}
			else {
				aux = (t_binary)(values[posBitStream++] - '0');
			}
			for (unsigned int k = 0; k < numberOfOutputSignals; k++) {
				outputSignals[k]->bufferPut((t_binary)aux);
			}
			numberOfBits--;
		}
	}

	if (mode == BinarySourceMode::AsciiFileAppendZeros) {
		ifstream asciiFile(asciiFilePath, ios::binary);
		if (asciiFile.is_open()) {
			//Determines the size of the file (in bytes)
			asciiFile.seekg(0, asciiFile.end);
			int fileSize = (int)asciiFile.tellg();
			asciiFile.seekg(nextCharacterIndex); //Reads from where it left off
			//Reads from file to BUFFER
			int numCharactersToRead = process / 8; //Number of full characters that are possible to read
			int numRead = 0; //Number of bytes/characters read
			unsigned int remaining = process;
			char aux;
			//If nextBitIndex != 0, means that the previous character was not fully read
			//So it finishes reading it
			if (nextBitIndex != 0) {
				asciiFile.get(aux);
				bitset<8> bitArray(aux);
				for (auto i = bitArray.size() - 1 - nextBitIndex; i >= 0; i--) {
					for (unsigned int j = 0; j < numberOfOutputSignals; j++) {
						outputSignals[j]->bufferPut((t_binary)bitArray[i]);
					}
					numberOfBits--;
					remaining--;
				}
				nextCharacterIndex++;
				numRead++;
				nextBitIndex = 0;
			}
			//While there is available space in the buffer and there are characters to be read in the file
			while (numRead < numCharactersToRead && asciiFile.get(aux)) {
				numRead++;
				bitset<8> bitArray(aux);
				for (auto i = bitArray.size() - 1; i >= 0; i--) {
					for (unsigned int j = 0; j < numberOfOutputSignals; j++) {
						outputSignals[j]->bufferPut((t_binary)bitArray[i]);
					}
				}
				nextCharacterIndex++;
				numberOfBits -= 8;
				remaining -= 8;
			}
			//When the program exists the previous loop, two things may happen:
			//- The file was fully read and we must choose if we are going to append zeros or not
			//- The number of bits to process is lower than 8 (remaining < 8)
			//The file was fully read
			if (nextCharacterIndex == fileSize) {
				nextCharacterIndex = 0;
				nextBitIndex = 0;
				//Appends zeros
				for (unsigned int i = 0; i < remaining; i++) {
					for (unsigned int j = 0; j < numberOfOutputSignals; j++) {
						outputSignals[j]->bufferPut((t_binary)0);
					}
					numberOfBits--;
				}
				return false;
			}
			else {
				//Generates the remaining number of bits
				//Does not increment nextCharacterIndex since the next character
				//to be read is still the current one
				asciiFile.get(aux);
 				bitset<8> bitArray(aux);
				for (auto i = bitArray.size() - 1; i >= bitArray.size() - remaining; i--) {
					for (unsigned int j = 0; j < numberOfOutputSignals; j++) {
						outputSignals[j]->bufferPut((t_binary)bitArray[i]);
					}
					nextBitIndex++;
					numberOfBits--;
				}
			}
			asciiFile.close();
		}
		else {
			cerr << "ERROR: Could not open file " << this->asciiFilePath << endl;
			exit(1);
		}
	}

	if (mode == BinarySourceMode::AsciiFileCyclic) {
		ifstream asciiFile(asciiFilePath, ios::binary);
		if (asciiFile.is_open()) {
			//Determines the size of the file (in bytes)
			asciiFile.seekg(0, asciiFile.end);
			int fileSize = (int)asciiFile.tellg();
			asciiFile.seekg(nextCharacterIndex); //Reads from where it left off
												 //Reads from file to BUFFER
			int numCharactersToRead = process / 8; //Number of full characters that are possible to read
			int numRead = 0; //Number of bytes/characters read
			unsigned int remaining = process;
			char aux;
			//If nextBitIndex != 0, means that the previous character was not fully read
			//So it finishes reading it
			if (nextBitIndex != 0) {
				asciiFile.get(aux);
				bitset<8> bitArray(aux);
				for (auto i = bitArray.size() - 1 - nextBitIndex; i >= 0; i--) {
					for (unsigned int j = 0; j < numberOfOutputSignals; j++) {
						outputSignals[j]->bufferPut((t_binary)bitArray[i]);
					}
					numberOfBits--;
					remaining--;
				}
				nextCharacterIndex++;
				numRead++;
				nextBitIndex = 0;
			}
			//While there is available space in the buffer and there are characters to be read in the file
			while (numRead < numCharactersToRead && asciiFile.get(aux)) {
				numRead++;
				bitset<8> bitArray(aux);
				for (auto i = bitArray.size() - 1; i >= 0; i--) {
					for (unsigned int j = 0; j < numberOfOutputSignals; j++) {
						outputSignals[j]->bufferPut((t_binary)bitArray[i]);
					}
				}
				nextCharacterIndex++;
				numberOfBits -= 8;
				remaining -= 8;
			}
			//When the program exists the previous loop, two things may happen:
			//- The file was fully read and we must choose if we are going to append zeros or not
			//- The number of bits to process is lower than 8 (remaining < 8)
			//The file was fully read
			if (nextCharacterIndex == fileSize) {
				nextCharacterIndex = 0;
				nextBitIndex = 0;
				return true;
			}
			else {
				//Generates the remaining number of bits
				//Does not increment nextCharacterIndex since the next character
				//to be read is still the current one
				asciiFile.get(aux);
				bitset<8> bitArray(aux);
				for (auto i = bitArray.size() - 1; i >= bitArray.size() - remaining; i--) {
					for (unsigned int j = 0; j < numberOfOutputSignals; j++) {
						outputSignals[j]->bufferPut((t_binary)bitArray[i]);
					}
					nextBitIndex++;
					numberOfBits--;
				}
			}
			asciiFile.close();
		}
		else {
			cerr << "ERROR: Could not open file " << this->asciiFilePath << endl;
			exit(1);
		}
	}
	
	return true;
}
