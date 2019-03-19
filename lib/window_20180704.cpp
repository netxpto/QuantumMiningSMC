# include "window_20180704.h"


#ifndef PI
#define PI  3.14159265358979323846
#endif

vector<double> getWindow(WindowType windowType, int windowSize) {
	vector<double> wn(windowSize);
	switch (windowType)
	{
		case Hamming:
			for (int x = 0; x < windowSize; x++) {
				wn[x] = 0.54 - 0.46*cos(2 * PI*x / (windowSize - 1));
			}
			return wn;

		case None:
			for (int x = 0; x < windowSize; x++) { 
				wn[x] = 1;
			}
			return wn;

		case Hann:
			for (int x = 0; x < windowSize; x++) {
				wn[x] = 0.5 *(1 - cos(2 * PI*x / (windowSize - 1)));
			}
			return wn;
	}
	return wn;
}
