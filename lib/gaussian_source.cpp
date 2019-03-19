#include <algorithm>
#include <complex>
#include <chrono>
#include <fstream>

#include "netxpto_20180118.h"
#include "gaussian_source.h"
#include <random>


void GaussianSource::initialize(void){

	firstTime = false;

}

bool GaussianSource::runBlock(void) {
	
	int space = outputSignals[0]->space();

	int process = space;

	if (process == 0) return false;

	normal_distribution<double> distribution(average, sqrt(var));
	
	unsigned seed = chrono::system_clock::now().time_since_epoch().count();

	generator.seed(seed);

	for (int i = 0; i < process; i++) {

		double out = distribution(generator);
		
		outputSignals[0]->bufferPut(out);
	}
	return true;
}
