# include "netxpto.h"
# include "analog_signal_generator.h"

# include <algorithm>
# include <complex>
# include <iostream>
# include <fstream>
# include <random>

using namespace std;

void AnalogSignalGenerator::initialize(void)
{
	outputSignals[0]->setSamplingPeriod(samplingPeriod);
	outputSignals[0]->setSymbolPeriod(symbolPeriod);
	//outputSignals[0]->setFirstValueToBeSaved(1);


}


bool AnalogSignalGenerator::runBlock(void)
{
	int space = outputSignals[0]->space();
	int process = space;

	if (process == 0) return false;

	double dt = outputSignals[0]->getSamplingPeriod();
	t_real phase{ 0 };
	t_real phase1{ 0 };
	t_real phase2{ 0 };
	t_real phase3{ 0 };
	t_real phase4{ 0 };
	t_real phase5{ 0 };
	t_real phase6{ 0 };
	t_real phase7{ 0 };
	t_real phase8{ 0 };
	t_real phase9{ 0 };
	t_real phase10{ 0 };
	t_real phase11{ 0 };
	t_real phase12{ 0 };


	for (int i = 0; i < process; i++)
	{
	     
		 phase  = phase + 2 * PI*analogSingalFrequency * dt;
		 phase1 = phase1 + 2 * PI*1.25*analogSingalFrequency * dt;
		 phase2 = phase2 + 2 * PI*1.5*analogSingalFrequency * dt;
		 phase3 = phase3 + 2 * PI*1.75*analogSingalFrequency * dt;
		 phase4 = phase4 + 2 * PI*2* analogSingalFrequency * dt;
		 phase5 = phase5 + 2 * PI*2.85*analogSingalFrequency * dt;
		 phase6 = phase6 + 2 * PI*3.45*analogSingalFrequency * dt;
		 phase7 = phase7 + 2 * PI*4.6*analogSingalFrequency * dt;
		 phase8 = phase8 + 2 * PI*4.75*analogSingalFrequency * dt;
		 phase9 = phase9 + 2 * PI*5* analogSingalFrequency * dt;
		 phase10 = phase10 + 2 * PI*5.20*analogSingalFrequency * dt;
		 phase11 = phase11 + 2 * PI*5.55*analogSingalFrequency * dt;
		 phase12 = phase12 + 2 * PI*5.65*analogSingalFrequency * dt;
	

		t_real signal = analogSignalAmplitude*cos(phase) + analogSignalAmplitude*cos(phase1) + analogSignalAmplitude*cos(phase2) - analogSignalAmplitude*cos(phase3) - analogSignalAmplitude*cos(phase4) + analogSignalAmplitude*cos(phase5) + analogSignalAmplitude*cos(phase6)-
			analogSignalAmplitude*cos(phase7)- analogSignalAmplitude*cos(phase8)+ analogSignalAmplitude*cos(phase9)- analogSignalAmplitude*cos(phase10)- analogSignalAmplitude*cos(phase11)+ analogSignalAmplitude*cos(phase12);
		
		outputSignals[0]->bufferPut((t_real)signal);

	}

	return true;

}