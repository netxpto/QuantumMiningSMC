# include "fft_20180208.h"

#ifndef M_PI
#define M_PI  3.14159265358979323846
#endif


//########################################################################################################################################################
//############################################################### FFT Module #############################################################################
//########################################################################################################################################################

static size_t reverseBits(size_t x, unsigned int n)
{
	size_t result = 0;
	unsigned int i;
	for (i = 0; i < n; i++, x >>= 1)
		result = (result << 1) | (x & 1);
	return result;
}

vector<complex<double>> reImVect2ComplexVector(vector<double> &v1_real, vector<double> &v1_imag)
{
	vector<complex<double>> v_out(v1_real.size());

	for (size_t i = 0; i < v1_real.size(); ++i)
	{
		complex<double> iNum(v1_real[i], v1_imag[i]);
		v_out.at(i) = iNum;
	}

	return v_out;
}

	complex<double> reIm2Complex(double &v1_real, double &v1_imag)
{
	complex<double> v_out;
	complex<double> iNum(v1_real, v1_imag);
	v_out = iNum;
	
	return v_out;
}

vector<complex <double>> complexVectorMultiplication(vector<complex <double>> &v1_in, vector<complex <double>> &v2_in)
{
	vector<complex <double>> v_out(v1_in.size(), 0);
	for (unsigned int k = 0; k < v1_in.size(); ++k)
	{
		v_out.at(k) = v1_in.at(k)*v2_in.at(k);
	}
	return v_out;
}

vector<complex<double>> fft(vector<complex<double> > &vec, int sign)
{
	vector<complex<double>> OUT(vec.size());
	vector<complex<double>> fftData(vec.size());
	vector<complex<double>> ifftData(vec.size());

	if (sign == 1)
	{
		fftData = fft(vec);
		OUT = fftData;
	}

	else

	{
		ifftData = ifft(vec);
		OUT = ifftData;
	}

	return OUT;
}

vector<complex<double>> fft(vector<complex<double> > &vec)
{
	size_t n = vec.size();

	if (n == 0)
		return vec;
	else if ((n & (n - 1)) == 0)  // Is power of 2
		transformRadix2(vec);
	else  // More complicated algorithm for arbitrary sizes
		transformBluestein(vec);

	double N = sqrt(static_cast<double>(n));
	for (size_t i = 0; i < n; i++)  // Scaling (because this FFT implementation omits it)
		vec[i] = vec[i] / N;


	return vec;
}

vector<complex<double>> ifft(vector<complex<double> > &vec)
{

	size_t n = vec.size();
	double N = sqrt(static_cast<double>(n));

	std::transform(vec.cbegin(), vec.cend(), vec.begin(), static_cast<complex<double>(*)(const complex<double> &)>(std::conj));

	fft(vec);
	for (size_t i = 0; i < n; i++)  // Scaling (because this FFT implementation omits it)
		vec[i] = vec[i] * N;

	std::transform(vec.cbegin(), vec.cend(), vec.begin(), static_cast<complex<double>(*)(const complex<double> &)>(std::conj));


	for (size_t i = 0; i < n; i++)  // Scaling (because this FFT implementation omits it)
		vec[i] = vec[i] / N;

	return vec;
}

void transformRadix2(vector<complex<double> > &vec)
{
	// Length variables
	size_t n = vec.size();
	int levels = 0;  // Compute levels = log2(n)
	for (size_t temp = n; temp > 1U; temp >>= 1)
		levels++;
	if (static_cast<size_t>(1U) << levels != n)
		throw "Length is not a power of 2";

	// Trignometric table
	vector<complex<double> > expTable(n / 2);
	for (size_t k = 0; k < n / 2; k++)
		expTable[k] = std::exp(complex<double>(0, 2 * M_PI * k / n));

	// Bit-reversed addressing permutation
	for (size_t i = 0; i < n; i++)
	{
		size_t j = reverseBits(i, levels);
		if (j > i)
			std::swap(vec[i], vec[j]);
	}

	// Cooley-Tukey decimation-in-time radix-2 FFT
	for (size_t size = 2; size <= n; size *= 2)
	{
		size_t halfsize = size / 2;
		size_t tablestep = n / size;

		for (size_t i = 0; i < n; i += size)	// Loop for intermediate stage
		{
			for (size_t j = i, k = 0; j < i + halfsize; j++, k += tablestep)
			{

				complex<double> temp = vec[j + halfsize] * expTable[k];
				vec[j + halfsize] = vec[j] - temp;
				vec[j] += temp;
			}
		}
		if (size == n)  // Prevent overflow in 'size *= 2'
			break;
	}
}

void transformBluestein(vector<complex<double> > &vec) {
	// Find a power-of-2 convolution length m such that m >= n * 2 + 1
	size_t n = vec.size();
	size_t m = 1;
	while (m / 2 <= n) {
		if (m > SIZE_MAX / 2)
			throw "Vector too large";
		m *= 2;
	}

	// Trignometric table
	vector<complex<double> > expTable(n);
	for (size_t k = 0; k < n; k++)
	{
		unsigned long long temp = static_cast<unsigned long long>(k) * k; // static_cast converts int "i" to unsigned long long "i". // temp = k^2
		temp %= static_cast<unsigned long long>(n) * 2;				// Modulo operation ==> temp = temp % static_cast<unsigned long long>(n) * 2 // temp = k^2 % 2n
		double angle = M_PI * temp / n;
		// Less accurate alternative if long long is unavailable: double angle = M_PI * i * i / n;
		expTable[k] = std::exp(complex<double>(0, angle));
	}

	// Temporary vectors and preprocessing
	vector<complex<double> > av(m);
	for (size_t i = 0; i < n; i++)
		av[i] = vec[i] * expTable[i];
	vector<complex<double> > bv(m);
	bv[0] = expTable[0];
	for (size_t i = 1; i < n; i++)
		bv[i] = bv[m - i] = std::conj(expTable[i]);

	// Convolution
	vector<complex<double> > cv(m);
	convolve(av, bv, cv);					// Here size of cv equal to m

											// Postprocessing
	for (size_t i = 0; i < n; i++)
		vec[i] = cv[i] * expTable[i];
}

void convolve(const vector<complex<double> > &xvec, const vector<complex<double> > &yvec, vector<complex<double> > &outvec)
{

	size_t m = xvec.size();
	if (m != yvec.size() || m != outvec.size())
		throw "Mismatched lengths";
	vector<complex<double> > xv = xvec;
	vector<complex<double> > yv = yvec;
	double N = sqrt(static_cast<double>(m));

	fft(xv);
	for (size_t i = 0; i < m; i++)  // Scaling by sqrt(N)
		xv[i] = xv[i] * N;

	fft(yv);
	for (size_t i = 0; i < m; i++)  // Scalingby sqrt(N)
		yv[i] = yv[i] * N;

	for (size_t i = 0; i < m; i++)
		xv[i] *= yv[i];

	ifft(xv);
	for (size_t i = 0; i < m; i++)  // Scaling by sqrt(N)
		xv[i] = xv[i] * N;

	for (size_t i = 0; i < m; i++)  // Scaling (because this FFT implementation omits it)
		outvec[i] = xv[i] / static_cast<double>(m);
}

vector<complex<double>> fftshift(vector<complex<double>> &vec)
{
	unsigned long long N = vec.size();
	vector<complex<double>> output;

	if (N % 2 == 0)
	{
		for (unsigned long long i = N / 2; i < vec.size(); i++)
		{
			output.push_back(vec[i]);
		}

		for (unsigned long long i = 0; i < N / 2; i++)
		{
			output.push_back(vec[i]);
		}
	}
	else
	{
		N = N + 1;
		for (unsigned long long i = N / 2; i < vec.size(); i++)
		{
			output.push_back(vec[i]);
		}

		for (unsigned long long i = 0; i < N / 2; i++)
		{
			output.push_back(vec[i]);
		}

	}

	return output;
	
}

vector<std::complex<double>> ifftshift(std::vector<std::complex<double>> &vec)
{
	unsigned long long N = vec.size();
	vector<std::complex<double>> output;

	if (N % 2 == 0)
	{
		for (unsigned long long i = N / 2; i < vec.size(); i++)
		{
			output.push_back(vec[i]);
		}

		for (unsigned long long i = 0; i < N / 2; i++)
		{
			output.push_back(vec[i]);
		}
	}
	else
	{
		N = N + 1;
		for (unsigned long long i = (N / 2)-1; i < vec.size(); i++)
		{
			output.push_back(vec[i]);
		}

		for (unsigned long long i = 0; i < (N / 2)-1; i++)
		{
			output.push_back(vec[i]);
		}

	}


	return output;
}

std::vector<std::complex<double>> hilbert(std::vector<std::complex<double>> vec)
{
	vector<std::complex<double>> in;
	vector<std::complex<double>> out;

	in = vec;
	size_t size = in.size();

	vector<std::complex<double>> A(size);
	vector<std::complex<double>> B(size);

	A = ifft(in);

	std::complex<double> Z = { 2,0 };

	for (auto j = 0; j < size; j++)
	{
		if (j == 0)
			A[j] = A[j];
		else if (j>0 && j<size / 2)
			A[j] = Z*A[j];
		else
			A[j] = 0;
	}

	B = fft(A);

	out = B;
	return out;
}



//########################################################################################################################################################
//########################################################### Optimized FFT Module #######################################################################
//########################################################################################################################################################


vector<complex<double>> fft(vector<complex<double> > &vec, int sign, int factor)
{
	vector<complex<double>> OUT(vec.size());
	vector<complex<double>> fftData(vec.size());
	vector<complex<double>> ifftData(vec.size());

	if (sign == 1 && factor == 0)
	{
		fftData = fft(vec);
		OUT = fftData;
	}

	else if (sign == 1 && factor == 1)
	{
		fftData = fftOptimized(vec);
		OUT = fftData;
	}

	else if (sign == -1 && factor == 1)
	{
		fftData = ifftOptimized(vec);
		OUT = fftData;
	}
	else
	{
		ifftData = ifft(vec);
		OUT = ifftData;
	}

	return OUT;
}

vector<complex<double>> fftOptimized(vector<complex<double> > &vec)
{
	size_t n = vec.size();

	if (n == 0)
		return vec;
	else if ((n & (n - 1)) == 0)  // For power of 2
		transformRadix2Optimized(vec);
	else  
		transformBluesteinOptimized(vec); // For arbitrary size

	return vec;
}

vector<complex<double>> ifftOptimized(vector<complex<double> > &vec)
{
	size_t m = vec.size();
	std::transform(vec.cbegin(), vec.cend(), vec.begin(), static_cast<complex<double>(*)(const complex<double> &)>(std::conj));
	fftOptimized(vec);
	std::transform(vec.cbegin(), vec.cend(), vec.begin(), static_cast<complex<double>(*)(const complex<double> &)>(std::conj));

	for (size_t i = 0; i < m; i++)
		vec[i] = vec[i] / static_cast<double>(m);
	return vec;
}

void transformRadix2Optimized(vector<complex<double> > &vec)
{
	// Length variables
	size_t n = vec.size();
	int levels = 0;  // Compute levels = log2(n)
	for (size_t temp = n; temp > 1U; temp >>= 1)
		levels++;
	if (static_cast<size_t>(1U) << levels != n)
		throw "Length is not a power of 2";

	// Trignometric table
	vector<complex<double> > expTable(n / 2);
	for (size_t k = 0; k < n / 2; k++)
		expTable[k] = std::exp(complex<double>(0, 2 * M_PI * k / n));

	// Bit-reversed addressing permutation
	for (size_t i = 0; i < n; i++)
	{
		size_t j = reverseBits(i, levels);
		if (j > i)
			std::swap(vec[i], vec[j]);
	}

	// Cooley-Tukey decimation-in-time radix-2 FFT
	for (size_t size = 2; size <= n; size *= 2)
	{
		size_t halfsize = size / 2;
		size_t tablestep = n / size;

		for (size_t i = 0; i < n; i += size)	// Loop for intermediate stage
		{
			for (size_t j = i, k = 0; j < i + halfsize; j++, k += tablestep)
			{

				complex<double> temp = vec[j + halfsize] * expTable[k];
				vec[j + halfsize] = vec[j] - temp;
				vec[j] += temp;
			}
		}
		if (size == n)  // Prevent overflow in 'size *= 2'
			break;
	}
}

void transformBluesteinOptimized(vector<complex<double> > &vec) {
	// Find a power-of-2 convolution length m such that m >= n * 2 + 1
	size_t n = vec.size();
	size_t m = 1;
	while (m / 2 <= n) {
		if (m > SIZE_MAX / 2)
			throw "Vector too large";
		m *= 2;
	}

	// Trignometric table
	vector<complex<double> > expTable(n);
	for (size_t k = 0; k < n; k++)
	{
		unsigned long long temp = static_cast<unsigned long long>(k) * k; // static_cast converts int "i" to unsigned long long "i". // temp = k^2
		temp %= static_cast<unsigned long long>(n) * 2;				// Modulo operation ==> temp = temp % static_cast<unsigned long long>(n) * 2 // temp = k^2 % 2n
		double angle = M_PI * temp / n;
		// Less accurate alternative if long long is unavailable: double angle = M_PI * i * i / n;
		expTable[k] = std::exp(complex<double>(0, angle));
	}

	// Temporary vectors and preprocessing
	vector<complex<double> > av(m);
	for (size_t i = 0; i < n; i++)
		av[i] = vec[i] * expTable[i];
	vector<complex<double> > bv(m);
	bv[0] = expTable[0];
	for (size_t i = 1; i < n; i++)
		bv[i] = bv[m - i] = std::conj(expTable[i]);

	// Convolution
	vector<complex<double> > cv(m);
	convolveOptimized(av, bv, cv);					// Here size of cv equal to m

											// Postprocessing
	for (size_t i = 0; i < n; i++)
		vec[i] = cv[i] * expTable[i];
}

void convolveOptimized(const vector<complex<double> > &xvec, const vector<complex<double> > &yvec, vector<complex<double> > &outvec)
{

	size_t m = xvec.size();
	if (m != yvec.size() || m != outvec.size())
		throw "Mismatched lengths";
	vector<complex<double> > xv = xvec;
	vector<complex<double> > yv = yvec;
	double N = sqrt(static_cast<double>(m));

	fft(xv);
	fft(yv);

	for (size_t i = 0; i < m; i++)
		xv[i] *= yv[i];

	ifft(xv);

	for (size_t i = 0; i < m; i++)
		outvec[i] = xv[i] / static_cast<double>(m);
}
