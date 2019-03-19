// Currently mostly copied from BER


# ifndef PROGRAM_INCLUDE_SNR_ESTIMATOR_
# define PROGRAM_INCLUDE_SNR_ESTIMATOR_
# include "netxpto_20180118.h"
# include "window_20180704.h"
# include <vector>
// Currently only powerSpectrum is implemented
enum SNREstimatorMethod{ powerSpectrum, m2m4, ren, constantAmplitudeMoments, qFactor};

// Estimates the SNR of a signal
class SNREstimator : public Block {

	bool firstTime{ true };

public:
	SNREstimator() {};
	SNREstimator(vector<Signal *> &InputSig, vector<Signal *> &OutputSig) :Block(InputSig, OutputSig) {};

	void initialize(void);
	bool runBlock(void);

	void setMeasuredIntervalSize(int misz) { measuredIntervalSize= misz; }
	int getMeasuredIntervalSize(void) { return measuredIntervalSize; }

	void setSegmentSize(int sz) { segmentSize = sz; }
	int getSegmentSize(void) { return segmentSize; }

	void setOverlapCount(int olp) { overlapCount = olp; }
	int getOverlapCount(void) { return overlapCount; }

	void setOverlapPercent(double olp) { overlapCount = floor(segmentSize*olp); }
	double getOverlapPercent(void) { return overlapCount/segmentSize; }

	void setConfidence(double P) { alpha = 1-P; }
	double getConfidence(void) { return 1 - alpha; }

	void setWindowType(WindowType wd) { windowType = wd; }
	WindowType getWindowType(void) { return windowType; }

	void setFilename(string fname) { filename = fname; }
	string getFilename(void) { return filename; }

	vector<complex<double>> fftshift(vector<complex<double>> &vec);

	void setRollOff(double rollOff) { rollOffComp = rollOff; }
	double getRollOff(void) { return rollOffComp; }

	void setNoiseBw(double nBw) {  noiseBw = nBw; }
	double getNoiseBw(void) { return noiseBw; }

	void setEstimatorMethod(SNREstimatorMethod mtd) { method = mtd; }
	SNREstimatorMethod getEstimatorMethod(void) { return method; }

	void setIgnoreInitialSamples(int iis) { ignoreInitialSamples = iis; }
	int getIgnoreInitialSamples(void) { return ignoreInitialSamples; }

	void setActivityState(bool state) { active = state; }
	bool getActivityState(void) { return active; }

private:
	vector <complex<double>> measuredInterval;
	WindowType windowType = Hann;
	vector<double> window;
	vector<double> frequencies;
	vector <double> allSNR;
	int ignoreInitialSamples = 513;
	bool firstPass = true;
	double noiseBw = 32e9;
	int measuredIntervalSize = 4096;
	int currentSize = 0;
	int segmentSize = 1024;
	int overlapCount = 256;
	double alpha = 0.05;
	double z;
	double U;
	double rollOffComp = 0.9;
	string filename = "SNR.txt";
	SNREstimatorMethod method = powerSpectrum;
	bool active = false;
};


#endif // !PROGRAM_INCLUDE_SRN_ESTIMATOR_
