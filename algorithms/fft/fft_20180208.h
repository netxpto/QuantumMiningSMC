# ifndef PROGRAM_INCLUDE_FFT_H_
# define PROGRAM_INCLUDE_FFT_H_

# include <vector>
# include <complex>
#include <algorithm>

using namespace std;

//########################################################################################################################################################
//############################################################### FFT MODULE #############################################################################
//########################################################################################################################################################
static size_t reverseBits(size_t x, unsigned int n);
vector<complex<double>> reImVect2ComplexVector(vector<double> &v1_real, vector<double> &v1_imag);
complex<double> reIm2Complex(double &v1_real, double &v1_imag);

vector<complex <double>> complexVectorMultiplication(vector<complex <double>> &v1_in, vector<complex <double>> &v2_in);

vector<complex<double>> fft(vector<complex<double> > &vec, int sign, int factor);
std::vector<std::complex<double>> fft(std::vector<std::complex<double>> &vec, int sign); 

std::vector<std::complex<double>> fft(std::vector<std::complex<double> > &vec);
std::vector<std::complex<double>> ifft(std::vector<std::complex<double> > &vec);
void transformRadix2(std::vector<std::complex<double> > &vec);
void transformBluestein(std::vector<std::complex<double> > &vec);
void convolve(const std::vector<std::complex<double> > &vecx, const std::vector<std::complex<double> > &vecy, std::vector<std::complex<double> > &vecout);


std::vector<std::complex<double>> fftOptimized(std::vector<std::complex<double> > &vec);
std::vector<std::complex<double>> ifftOptimized(std::vector<std::complex<double> > &vec);
void transformRadix2Optimized(std::vector<std::complex<double> > &vec);
void transformBluesteinOptimized(std::vector<std::complex<double> > &vec);
void convolveOptimized(const std::vector<std::complex<double> > &vecx, const std::vector<std::complex<double> > &vecy, std::vector<std::complex<double> > &vecout);


std::vector<std::complex<double>> fftshift(std::vector<std::complex<double>> &vec);
std::vector<std::complex<double>> ifftshift(std::vector<std::complex<double>> &vec);

std::vector<std::complex<double>> hilbert(std::vector<std::complex<double>> vec);


# endif // PROGRAM_INCLUDE_FFT_H_
