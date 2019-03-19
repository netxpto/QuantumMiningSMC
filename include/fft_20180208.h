# ifndef PROGRAM_INCLUDE_FFT_H_
# define PROGRAM_INCLUDE_FFT_H_

# include <vector>
# include <complex>
#include <algorithm>

using namespace std;

//########################################################################################################################################################
//############################################################### FFT MODULE #############################################################################
//########################################################################################################################################################

vector<complex<double>> ReImVect2ComplexVector(vector<double> &v1_real, vector<double> &v1_imag);
vector<complex <double>> complexVectorMultiplication(vector<complex <double>> &v1_in, vector<complex <double>> &v2_in);


std::vector<std::complex<double>> fft(std::vector<std::complex<double>> &vec, int sign);
std::vector<std::complex<double>> fft(std::vector<std::complex<double> > &vec);
std::vector<std::complex<double>> ifft(std::vector<std::complex<double> > &vec);
void transformRadix2(std::vector<std::complex<double> > &vec);
void transformBluestein(std::vector<std::complex<double> > &vec);
void convolve(const std::vector<std::complex<double> > &vecx, const std::vector<std::complex<double> > &vecy, std::vector<std::complex<double> > &vecout);

# endif // PROGRAM_INCLUDE_FFT_H_
