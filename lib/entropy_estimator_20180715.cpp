# include "entropyEst.h"
# include <iostream>
# include <fstream>
# include <numeric>     

using namespace std;

entropyEst::entropy_estimator(vector<Signal *> &InputSig, int window) {
	
	numberOfInputSignals = (int)InputSig.size();

	inputSignals = InputSig;

	w = window;

}


bool entropyEst::runBlock(void)
{
	int ready = inputSignals[0]->ready();

	int process;
	double p_one;

	process = ready;
	if (process == 0) return false;
	
	float n_windows;							// Nº of windows to compute entropy

	if ((process % w) != 0) {					// Considers a single window		
		n_windows = 1;
	}
	else
	{
		n_windows = process / w;				// Considers multiple windows
	}

	ofstream fOut("entropy_est.txt");
	fOut << "-------- Parameters ---------" << "\n";
	fOut << "Nº of bits: " << process << endl;
	fOut << "Nº of windows: " << n_windows << endl;
	fOut << "Window size: " << process / n_windows << endl;


	t_binary dat;
	int n_one = 0;

	vector<double> h(n_windows);		    // Entropy estimations
	int k = 0;
	int b = 0;

	for (int i = 0; i < process; i++) {
		
		(inputSignals[0])->bufferGet(&dat);
		//cout << dat << '\n';

		if (dat == 1) {
			n_one = n_one + 1;
		}
		
		k++;

		if (k == w && b <= n_windows && (process % w) == 0) {
			p_one = (double) n_one / w;

			if (p_one == 1 || p_one == 0)
				h[b] = 0;					// Info theory convention 0xlog2(0) = 0
			else {
				h[b] = (double) -p_one * log2(p_one) - (1 - p_one) * log2(1 - p_one);
			}

			k = 0;
			b++;
			n_one = 0;
		}
	
	}

	if ((process % w) != 0) {

		p_one = (double) n_one / process;
		
		
		if (p_one == 1 || p_one == 0)
			h[0] = 0;					// Info theory convention 0xlog2(0) = 0
		else {
			h[0] = (double) -p_one * log2(p_one) - (1 - p_one) * log2(1 - p_one);
		}


		fOut << "Warning: nº of bits must be a multiple of window size";

	}
	
	numberOfSamples = numberOfSamples - process;


	double sum = 0;
	double sum2 = 0;
	
	// Compute the mean and variance
	for (int i = 0; i < n_windows; i++) {				// Mean
		sum = sum + h[i];
		sum2 = sum2 + (h[i]* h[i]);
	}

	double mean = sum / n_windows;

	double var = (sum2 - (sum*sum) / n_windows) / n_windows;


	fOut << "\n\n" << "---------- Results ----------" << "\n";
	fOut << "Entropy Mean = " << mean << endl;
	fOut << "Entropy Variance = " << var << "\n" << endl;

	fOut << "Entropy Estimations " << "\n";
	for (const auto &e : h) fOut << e << endl;

	fOut.close();

	return true;
}
