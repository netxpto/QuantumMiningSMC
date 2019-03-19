# ifndef BOB_H_
# define BOB_H_

# include "../include/netxpto_20180815.h"

# include "../include/binary_source_20180815.h"
# include "../include/m_qam_mapper_20180815.h"
# include "../include/discrete_to_continuous_time_20180815.h"
# include "../include/sink_20180815.h"
# include "../include/pulse_shaper_20180815.h"
# include "../include/time_delay_20180815.h"
# include "../include/polarization_rotator_20181113.h"
# include "../include/fork_20180815.h"
# include "../include/polarization_beam_splitter_20181113.h"
# include "../include/single_photon_detector_20181113.h"
# include "../include/coincidence_detector_20181113.h"
# include "../include/clock_20181113.h"

class Bob : public SuperBlock {

public:

	//##############################################################################################################

	Bob(initializer_list<Signal *> inputSig, initializer_list<Signal *> outputSig);

	void initialize(void);
	bool runBlock(void);

	/*Input Parameters*/
	bool logValue{ true };
	string logFileName{ "SuperBlock_Bob.txt" };
	string signalsFolderName{ "signals/SuperBlock_Bob" };
	bool firstTime{ true };

	//##############################################################################################################
	
	void setBitPeriod(double bPeriod) { BinarySource_.setBitPeriod(bPeriod); };
	double getBitPeriod(void) const { return BinarySource_.getBitPeriod(); };
	//the bitPeriod is half to generate the base and the data value

	void setNumberOfBits(long int nOfBits) { BinarySource_.setNumberOfBits(nOfBits); };
	long int getNumberOfBits(void) const { return BinarySource_.getNumberOfBits(); };
	//the numberOfBits are doube to generate the base and the data value

	void setMode(BinarySourceMode m) { BinarySource_.setMode(m); }
	BinarySourceMode getMode(void) const { return BinarySource_.getMode(); };

	void setSeed(long int s) { BinarySource_.setSeed(s); };
	long int getSeed(void) const { return BinarySource_.getSeed(); };

	void setProbabilityOfZero(double pZero) { BinarySource_.setProbabilityOfZero(pZero); };
	double getProbabilityOfZero(void) const { return BinarySource_.getProbabilityOfZero(); };

	void setBitStream(string bStream) { BinarySource_.setBitStream(bStream); };
	string getBitStream(void) const { return BinarySource_.getBitStream(); };

	void setPatternLength(int pLength) { BinarySource_.setPatternLength(pLength); };
	int getPatternLength(void) const { return BinarySource_.getPatternLength(); };

	void setM(int mValue) { MQamMapper_.setM(mValue); };
	void setIqAmplitudes(vector<vector<t_real>> iqAmplitudesValues) { MQamMapper_.setIqAmplitudes(iqAmplitudesValues); };

	void setFirstTime(bool fTime) { MQamMapper_.setFirstTime(fTime); };
	bool getFirstTime() { return MQamMapper_.getFirstTime(); };

	void setNumberOfSamplesPerSymbol(int nSamplesPerSymbol) { DiscreteToContinuousTime_.setNumberOfSamplesPerSymbol(nSamplesPerSymbol); };
	int const getNumberOfSamplesPerSymbol(void) { return DiscreteToContinuousTime_.getNumberOfSamplesPerSymbol(); };
/*
	void setNumberOfSamplesPerSymbolSPD(int nSamplesPerSymbol) { DiscreteToContinuousTime_1.setNumberOfSamplesPerSymbol(nSamplesPerSymbol); };
	int const getNumberOfSamplesPerSymbolSPD(void) { return DiscreteToContinuousTime_1.getNumberOfSamplesPerSymbol(); };

	void setTimeDelaySPD(double tDelay) { TimeDelay_.setTimeDelay(tDelay); };
	double const getTimeDelaySPD() { TimeDelay_.getTimeDelay(); };
*/
	void setImpulseResponseTimeLength(int impResponseTimeLength) { PulseShaper_.setImpulseResponseTimeLength(impResponseTimeLength); };
	int const getImpulseResponseTimeLength(void) { return PulseShaper_.getImpulseResponseTimeLength(); };

	void setFilterType(pulse_shapper_filter_type fType) { PulseShaper_.setFilterType(fType); };
	pulse_shapper_filter_type const getFilterType(void) { return PulseShaper_.getFilterType(); };

	void setRollOffFactor(double rOffFactor) { PulseShaper_.setRollOffFactor(rOffFactor); };
	double const getRollOffFactor() { return PulseShaper_.getRollOffFactor(); };

	void setPulseWidth(double pWidth) { PulseShaper_.setPulseWidth(pWidth); };
	double const getPulseWidth() { return PulseShaper_.getPulseWidth(); };

	void setSPD0Path(int path0) { SinglePhotonDetector_0.setPath(path0); };
	int const getSPD0Path() { return SinglePhotonDetector_0.getPath(); };

	void setSPD1Path(int path1) { SinglePhotonDetector_1.setPath(path1); };
	int const getSPD1Path() { return SinglePhotonDetector_1.getPath(); };

	void setSPDProbabilityDarkCount(double pDarkC) { SinglePhotonDetector_0.setProbabilityDarkCount(pDarkC); SinglePhotonDetector_1.setProbabilityDarkCount(pDarkC); };
	double const getSPD0dc() { return SinglePhotonDetector_0.getProbabilityDarkCount(); };
	double const getSPD1dc() { return SinglePhotonDetector_1.getProbabilityDarkCount(); };

	void setClockPeriod(double cperiod) { Clock_.setClockPeriod(cperiod); };
	double const getClockPeriod() { return Clock_.getClockPeriod(); };

	void setClockPhase(double cphase) { Clock_.setClockPhase(cphase); };
	double const getClockPhase() { Clock_.getClockPhase(); };

	void setClockSamplingPeriod(double csPeriod) { Clock_.setSamplingPeriod(csPeriod); };
	double const getClockSamplingPeriod() { return Clock_.getSamplingPeriod(); };

	void setDecisionLevel(double dLevel) { CoincidenceDetector_.setDecisionLevel(dLevel); };
	double const getDecisionLevel() { return CoincidenceDetector_.getDecisionLevel(); };

	void setFilterType_PSclk(pulse_shapper_filter_type fType) { PulseShaper_cd.setFilterType(fType); };
	pulse_shapper_filter_type const getFilterType_PSclk(void) { return PulseShaper_cd.getFilterType(); };

	void setPulseWidthDetection(double pWidth) { PulseShaper_cd.setPulseWidth(pWidth); };
	double const getPulseWidthDetection() { return PulseShaper_cd.getPulseWidth(); };

	//###############################################################################################################

private:
	
	Binary BS_out_1{ "BS_out_1.sgn" }, BS_out_2{ "BS_out_2.sgn" };
	BinarySource BinarySource_{ {}, { &BS_out_1, &BS_out_2} };

	Sink Sink_BS_out_2{ { &BS_out_2 },{} };

	TimeDiscreteAmplitudeContinuousReal MQM_out{ "MQM_out.sgn" }, MQM_clk{ "MQM_clk.sgn" };
	MQamMapper MQamMapper_{ { &BS_out_1 },{ &MQM_out, &MQM_clk } };
	// the in-phase componente is used to select both the basis, 0 => 0 (linear basis),  1 => PI/4 (diagonal basis)
	// the quadrature componente can be used as a clock

	TimeContinuousAmplitudeContinuousReal DTCT_clk{ "DTCT_clk.sgn" };
	DiscreteToContinuousTime DiscreteToContinuousTime_1{ { &MQM_clk },{ &DTCT_clk } };

	TimeContinuousAmplitudeContinuousReal TD_clk{ "TD_clk.sgn" };
	TimeDelay TimeDelay_{ { &DTCT_clk },{ &TD_clk } };

	TimeContinuousAmplitudeContinuousReal PS_clk{ "PS_clk.sgn" };
	PulseShaper PulseShaper_clk{ { &TD_clk },{ &PS_clk } };

	TimeContinuousAmplitudeContinuousReal Trigger_SPD0{ "Trigger_SPD0.sgn" }, Trigger_SPD1{ "Trigger_SPD1.sgn" };
	Fork Fork_{ { &PS_clk },{ &Trigger_SPD0, &Trigger_SPD1} };

	TimeContinuousAmplitudeContinuousReal DTCT_out{ "DTCT_out.sgn" };
	DiscreteToContinuousTime DiscreteToContinuousTime_{ { &MQM_out },{ &DTCT_out } };

	TimeContinuousAmplitudeContinuousReal PS_out{ "PS_out.sgn" };
	PulseShaper PulseShaper_{ { &DTCT_out },{ &PS_out } };

	PhotonStreamXY Bob_in{ "Bob_in.sgn" }, SPPBS_in{ "SPPBS_in.sgn" };
	PolarizationRotator PolarizationRotator_{ { &Bob_in, &PS_out },{ &SPPBS_in } };

	PhotonStreamMPXY SPPBS_out{ "SPPBS_out.sgn" };
	PolarizationBeamSplitter PolarizationBeamSplitter_{ { &SPPBS_in },{ &SPPBS_out } };

	TimeContinuousAmplitudeContinuousReal SPD0_out{ "SPD0_out.sgn" }, SPD1_out{ "SPD1_out.sgn" };
	SinglePhotonDetector SinglePhotonDetector_0{ { &SPPBS_out, &Trigger_SPD0 },{ &SPD0_out } };
	SinglePhotonDetector SinglePhotonDetector_1{ { &SPPBS_out, &Trigger_SPD1 },{ &SPD1_out } };

	TimeContinuousAmplitudeContinuousReal CLK_cd_in{ "CLK_cd.sgn" };
	Clock Clock_{ {},{ &CLK_cd_in } };

	TimeContinuousAmplitudeContinuousReal TD_clk_1{ "TD_clk_cd.sgn" };
	TimeDelay TimeDelay_clk{ { &CLK_cd_in },{ &TD_clk_1 } };

	TimeContinuousAmplitudeContinuousReal PS_clk_out{ "PS_clk_cd.sgn" };
	PulseShaper PulseShaper_cd{ { &TD_clk_1 },{ &PS_clk_out } };

	TimeDiscreteAmplitudeContinuousReal CD_out{ "CD_out.sgn" };
	CoincidenceDetector CoincidenceDetector_{ { &SPD0_out, &SPD1_out, &PS_clk_out },{ &CD_out } };

	
};

# endif


