# ifndef PROGRAM_INCLUDE_ADC_H_
# define PROGRAM_INCLUDE_ADC_H_

# include "netxpto_20180418.h"

# include "resample_20180423.h"
# include "quantizer_20180423.h"



// this is a test block for the purpose of beta testing new code
// current code: Building Homodyne superblock.
class ADC : public SuperBlock {

	// #####################################################################################################
	// ################## Internal Signals Declaration and Inicialization ##################################
	// #####################################################################################################


	TimeContinuousAmplitudeDiscreteReal ADC01{ "ADC01.sgn" };  // Resample
	TimeContinuousAmplitudeDiscreteReal ADC02{ "ADC02.sgn" };  // Resample
	//TimeContinuousAmplitudeDiscreteReal ADC03{ "ADC03.sgn" };  // Quntizer
	//TimeContinuousAmplitudeDiscreteReal ADC04{ "ADC04.sgn" };  // Quntizer


	// #####################################################################################################
	// ########################### Blocks Declaration and Inicialization ###################################
	// #####################################################################################################

	Resample B01;

	//Resample B02;

	Quantizer B02;

	//Quantizer B04;

	/* State Variables */

	/* Input Parameters */
	double samplingPeriod{ 1.0 };

	//int samplesToSkip{ 0 };

public:

	/* Methods */

	ADC(vector<Signal *> &inputSignal, vector<Signal *> &outputSignal);

	/* Set Methods */
	
	/*void setResampleOutRateFactor(double OUTsRate) { B01.setOutRateFactor(OUTsRate); B02.setOutRateFactor(OUTsRate); }

	void setQuantizerSamplingPeriod(double sPeriod) { B03.setSamplingPeriod(sPeriod); B04.setSamplingPeriod(sPeriod); }

	void setQuantizerResolution(double nbits) { B03.setResolution(nbits); B04.setResolution(nbits); }
	void setQuantizerMaxValue(double maxvalue) { B03.setMaxValue(maxvalue); B04.setMaxValue(maxvalue);}
	void setQuantizerMinValue(double minvalue) { B03.setMinValue(minvalue); B04.setMinValue(minvalue); }*/
	
	void setResampleOutRateFactor(double OUTsRate) { B01.setOutRateFactor(OUTsRate); }
	void setQuantizerSamplingPeriod(double sPeriod) { B02.setSamplingPeriod(sPeriod); }
	void setQuantizerResolution(double nbits) { B02.setResolution(nbits);  }
	void setQuantizerMaxValue(double maxvalue) { B02.setMaxValue(maxvalue);  }
	void setQuantizerMinValue(double minvalue) { B02.setMinValue(minvalue);  }

	void setSamplingPeriod(double sPeriod) { B02.setSamplingPeriod(sPeriod); };
};

#endif
