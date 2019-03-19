# include <algorithm>  // min()
# include <math.h>     // cos(), sin()

# include "netxpto.h"
# include "polarization_beam_splitter.h"

void PolarizationBeamSplitter::initialize(void) {

	outputSignals[0]->symbolPeriod = inputSignals[0]->symbolPeriod;
	outputSignals[0]->samplingPeriod = inputSignals[0]->samplingPeriod;
	outputSignals[0]->samplesPerSymbol = inputSignals[0]->samplesPerSymbol;
	outputSignals[0]->setFirstValueToBeSaved(inputSignals[0]->getFirstValueToBeSaved());

}

bool PolarizationBeamSplitter::runBlock(void) {

	int ready = inputSignals[0]->ready();
	int space = outputSignals[0]->space();

	int length = min(ready, space);

	if (length <= 0) return false;

	for (int i = 0; i < length; i++) {
		t_photon photon;
		inputSignals[0]->bufferGet(&photon);

		t_photon_mp photonMP;

		if (photon.probabilityAmplitude > 0.0) {
			t_real pAmplitude0 = photon.probabilityAmplitude*cos(photon.polarization * PI / 180.0);
			t_real pAmplitude90 = photon.probabilityAmplitude*sin(photon.polarization * PI / 180.0);

			photonMP.path[0].probabilityAmplitude = pAmplitude0;
			photonMP.path[0].polarization = 0.0;

			photonMP.path[1].probabilityAmplitude = pAmplitude90;
			photonMP.path[1].polarization = 90.0;
		}
		else {
			photonMP.path[0].probabilityAmplitude = 0.0;
			photonMP.path[0].polarization = 0.0;

			photonMP.path[1].probabilityAmplitude = 0.0;
			photonMP.path[1].polarization = 90.0;

		}

		outputSignals[0]->bufferPut((t_photon_mp) photonMP);

	}

	return true;

};
