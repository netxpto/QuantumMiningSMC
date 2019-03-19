# ifndef SINK_H_
# define SINK_H_

# include <algorithm>   // std::min
# include <string>		// compare

# include "netxpto_20180418.h"

class Sink : public Block {

	/* State Variables */

	bool displayNumberOfSamples{ false };

private:

	/* Auxiliary Parameters */
	ofstream output_file;

	/* Input Parameters */		
	long int numberOfSamples{ -1 };
	string asciiFilePath{ "file_output_data.txt" };
	long int numberOfBitsToSkipBeforeSave{ 0 };
	long int numberOfBytesToSaveInFile{ 0 };

public:
	/* Methods */
	Sink();
	Sink(vector<Signal *> &InputSig, vector<Signal *> &OutputSig);

	bool runBlock(void);

	void setAsciiFilePath(string newName) { asciiFilePath = newName; };
	string getAsciiFilePath() { return asciiFilePath; };

	void setNumberOfBitsToSkipBeforeSave(long int newValue) { numberOfBitsToSkipBeforeSave = newValue; };
	long int getNumberOfBitsToSkipBeforeSave() { return numberOfBitsToSkipBeforeSave; };

	void setNumberOfBytesToSaveInFile(long int newValue) { numberOfBytesToSaveInFile = newValue; };
	long int getNumberOfBytesToSaveInFile() { return numberOfBytesToSaveInFile; };

	void setNumberOfSamples(long int nOfSamples){ numberOfSamples = nOfSamples; };

	void setDisplayNumberOfSamples(bool opt) { displayNumberOfSamples = opt; };

};

#endif
