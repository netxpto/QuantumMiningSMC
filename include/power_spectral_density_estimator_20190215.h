# ifndef PROGRAM_INCLUDE_POWER_SPECTRAL_DENSITY_ESTIMATOR_
# define PROGRAM_INCLUDE_POWER_SPECTRAL_DENSITY_ESTIMATOR_

# include "netxpto_20190215.h"
# include "window_20180704.h"
# include <vector>

// Available methods: Welch's Periodogram, Bartlett Periodogram or Correlogram
enum PowerSpectralDensityEstimatorMethod{ WelchPgram, BartlettPgram, Correlogram};

// Estimates the PSD of a signal
class PowerSpectralDensityEstimator : public Block {


public:
	PowerSpectralDensityEstimator() {};
	PowerSpectralDensityEstimator(vector<Signal *> &InputSig, vector<Signal *> &OutputSig) :Block(InputSig, OutputSig) {};

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

	void setEstimatorMethod(PowerSpectralDensityEstimatorMethod mtd) { method = mtd; }
	PowerSpectralDensityEstimatorMethod getEstimatorMethod(void) { return method; }

	void setActivityState(bool state) { active = state; }
	bool getActivityState(void) { return active; }

private:
	// Block Default parameters
	PowerSpectralDensityEstimatorMethod method = WelchPgram;
	double ignoreInitialSamples = 513;
	WindowType windowType = Hann;
	int measuredIntervalSize = 2048;
	int segmentSize = 512;
	double overlapPercent = 0.50;
	int overlapCount = 256;
	double alpha = 0.05;
	double tolerance = 1e-6;
	string filename = "signals/powerSpectralDensity.txt";

	// Variables used by block
	bool firstTime{ true };
	vector <complex<double>> measuredInterval;
	vector<double> window;
	vector<double> frequencies;
	vector<double> totalPeriodogramSum;
	vector<double> totalPeriodogramVar;
	int totalSummed = 0;
	bool firstPass = true;
	int currentSize = 0;
	double z;
	double U;
	bool active = false;
	
	// Functions used by block
	long double chiSqInvLo(int, double);
	long double chiSqInvHi(int, double);
	double factorial(int);
	long double logFactorial(int num);
	double getChiSq(int, double, double, int);
	long double expSum(double x, int n);
	vector<complex<double>> fftshift(vector<complex<double>> &vec);
};


#endif // !PROGRAM_INCLUDE_POWER_SPECTRAL_DENSITY_ESTIMATOR_
