#pragma once
# ifndef PROGRAM_INCLUDE_OVERLAP_AND_SAVE_H_
# define PROGRAM_INCLUDE_OVERLAP_AND_SAVE_H_

# include <vector>
# include <complex>
#include <algorithm>

using namespace std;

//########################################################################################################################################################
//############################################################### FFT MODULE #############################################################################
//########################################################################################################################################################

vector<complex<double>> ReImVect2ComplexVector(vector<double> &v1_real, vector<double> &v1_imag);

std::vector<std::complex<double>> fft(std::vector<std::complex<double>> &vec, int sign);
std::vector<std::complex<double>> fft(std::vector<std::complex<double> > &vec);
std::vector<std::complex<double>> ifft(std::vector<std::complex<double> > &vec);
void transformRadix2(std::vector<std::complex<double> > &vec);
void transformBluestein(std::vector<std::complex<double> > &vec);
void convolve(const std::vector<std::complex<double> > &vecx, const std::vector<std::complex<double> > &vecy, std::vector<std::complex<double> > &vecout);

std::vector<std::complex<double>> overlapSave(std::vector<std::complex<double> > &inTimeDomainComplex, std::vector<std::complex<double> > &inTimeDomainFilterComplex);
std::vector<std::complex<double>> overlapSaveRealTime(std::vector<std::complex<double> > &inTimeDomainComplex1, std::vector<std::complex<double> > &inTimeDomainComplex2, std::vector<std::complex<double> > &inTimeDomainFilterComplex);


# endif // PROGRAM_INCLUDE_FFT_H_
