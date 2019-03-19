# ifndef PROGRAM_INCLUDE_PSD_ESTIMATOR_
# define PROGRAM_INCLUDE_PSD_ESTIMATOR_

# include "netxpto_20180118.h"
# include "window_20180704.h"
# include <vector>

// Available methods: Welch's Periodogram, Bartlett Periodogram or Correlogram
enum PSDEstimatorMethod{ WelchPgram, BartlettPgram, Correlogram};

// Estimates the PSD of a signal
class PSDEstimator : public Block {


public:
	PSDEstimator() {};
	PSDEstimator(vector<Signal *> &InputSig, vector<Signal *> &OutputSig) :Block(InputSig, OutputSig) {};

	void initialize(void);
	bool runBlock(void);

	void setMeasuredIntervalSize(int misz) { measuredIntervalSize= misz; }
	int getMeasuredIntervalSize(void) { return measuredIntervalSize; }

	void setSegmentSize(int sz) { segmentSize = sz; }
	int getSegmentSize(void) { return segmentSize; }

	void setOverlapCount(int olp) { overlapCount = olp; overlapPercent =  overlapCount/segmentSize;}
	int getOverlapCount(void) { return overlapCount; }

	void setOverlapPercent(double olpP) { overlapPercent = olpP; overlapCount = segmentSize*overlapPercent;}
	double getOverlapPercent(void) { return overlapPercent; }

	void setConfidence(double P) { alpha = 1-P; }
	double getConfidence(void) { return 1 - alpha; }

	void setWindowType(WindowType wd) { windowType = wd; }
	WindowType getWindowType(void) { return windowType; }

	void setFilename(string fname) { filename = fname; }
	string getFilename(void) { return filename; }

	vector<complex<double>> fftshift(vector<complex<double>> &vec);

	void setRollOff(double rollOff) { rollOffComp = rollOff; }
	double getRollOff(void) { return rollOffComp; }


	void setEstimatorMethod(PSDEstimatorMethod mtd) { method = mtd; }
	PSDEstimatorMethod getEstimatorMethod(void) { return method; }

private:
	bool firstTime{ true };
	vector <complex<double>> measuredInterval;
	WindowType windowType = Hanning;
	vector<double> window;
	vector<double> frequencies;
	vector<double> totalPeriodogramSum;
	vector<double> totalPeriodogramVar;
	int totalSummed = 0;
	double ignoreInitialSamples = 513;
	bool firstPass = true;
	int measuredIntervalSize = 2048;
	int currentSize = 0;
	int segmentSize = 512;
	double overlapPercent = 0.50;
	int overlapCount = 256;
	double alpha = 0.05;
	double z;
	double U;
	double rollOffComp = 0.9;
	string filename = "signals/PSD.txt";
	PSDEstimatorMethod method = WelchPgram;
};


#endif // !PROGRAM_INCLUDE_PSD_ESTIMATOR_
