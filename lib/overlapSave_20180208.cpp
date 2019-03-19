# include "overlapSave_20180208.h"

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

vector<complex<double>> ReImVect2ComplexVector(vector<double> &v1_real, vector<double> &v1_imag)
{
	vector<complex<double>> v_out(v1_real.size());

	for (size_t i = 0; i < v1_real.size(); ++i)
	{
		complex<double> iNum(v1_real[i], v1_imag[i]);
		v_out.at(i) = iNum;
	}

	return v_out;
}

vector<complex<double>> fft(vector<complex<double> > &vec, int sign)
{
	vector<complex<double>> OUT(vec.size());
	vector<complex<double>> fftData(vec.size());
	vector<complex<double>> ifftData(vec.size());

	if (sign == -1)
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
		expTable[k] = std::exp(complex<double>(0, -2 * M_PI * k / n));

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
		expTable[k] = std::exp(complex<double>(0, -angle));
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



std::vector<std::complex<double>> overlapSave(std::vector<std::complex<double> > &inTimeDomainComplex, std::vector<std::complex<double> > &inTimeDomainFilterComplex)
{
	size_t M = inTimeDomainFilterComplex.size();  // Initial size of filter
	size_t N;									  // FFT size

	if ((M & (M - 1)) == 0)						  // Is power of 2
	{
		N = M*2;
	}
	else
	{
		size_t m = 1;
		while (m <= M)					// This calculates next value of power of 2
		{
			m *= 2;
		}

		N = m;
	}

	// Now, check if the value of (N-M) is less than or equal to the 25% of N,
	// then double the size of the N.
	
	/*if (int(N - M) <= 0.25*N) 
	{
		N = N * 2;
	}*/



	size_t L = N-M+1;			// Size of data block (Here, we used fix 50% overlap)
	size_t overlap = N - L;		// size of overlap

	size_t Dl = inTimeDomainComplex.size();
	int extraZero = 0;
	if (Dl % L == 0)
	{
		inTimeDomainComplex = inTimeDomainComplex;
	}
	else
	{
		// This changes the length of data if it's not interger multiple of L
		size_t Dlnew = inTimeDomainComplex.size();
		while (Dlnew % L != 0)
		{
			inTimeDomainComplex.push_back((0, 0));
			Dlnew = inTimeDomainComplex.size();
			extraZero += 1;
		}
	}



	vector <complex<double>>  filterTD(N); // Time domain filter
	vector <complex<double>>  filterFD(N); // Freq domain filter

	for (unsigned int i = 0; i < inTimeDomainFilterComplex.size(); i++)
	{
		filterTD[i] = inTimeDomainFilterComplex[i];
		filterTD[i] = filterTD[i] * (sqrt(filterTD.size())); //Scaling ny sqrt(N)
	}
	filterFD = fft(filterTD);



	//////////////////////////////// Section 4 /////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////
	////////////////////////////// Create Matrix ///////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////


	size_t nr = inTimeDomainComplex.size() / L;

	vector < vector<complex<double>> > matrix;
	vector <complex<double>> tempVector(L); // Without overlap
	vector <complex<double>> tempVectorA(N); // With overlap


	unsigned int position = 0;

	for (unsigned int i = 0; i < nr; i++)
	{

		for (unsigned int j = 0; j < L; j++)
		{
			tempVector[j] = inTimeDomainComplex[position + j]; // Take data withot overlap
		}


		if (i == 0)
		{
			for (unsigned int k = 0; k < tempVector.size(); k++)
			{
				tempVectorA[overlap + k] = tempVector[k];  // For first block, overlap is zero
			}
		}
		else
		{
			vector <complex<double>> tempVectorM = matrix[i - 1];
			for (unsigned int m = 0; m < overlap; m++)
			{
				tempVectorA[m] = tempVectorM[L + m];
			}
			for (unsigned int n = 0; n < L; n++)
			{
				tempVectorA[overlap + n] = tempVector[n];
			}
		}


		matrix.push_back(tempVectorA);
		position = position + L;
	}


	//////////////////////////////// Section 4 /////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////// Convolution ////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////

	for (unsigned int p = 0; p < matrix.size(); p++)
	{
		vector <complex<double>> dataBlock = matrix[p];
		vector <complex<double>> dataBlockTD = dataBlock;

		dataBlockTD = fft(dataBlockTD);	 // Here to very with MATLAB, multiply this data with SQRT(N)

		for (unsigned int q = 0; q < dataBlockTD.size(); q++)
		{
			dataBlockTD[q] = dataBlockTD[q] * filterFD[q]; // Multiplication with filter data
		}

		dataBlockTD = ifft(dataBlockTD);				  // ifft to get filtered data

		matrix[p] = dataBlockTD;						  // save data to matrix
	}

	// Next, create vector from a matrix by disarding the overlap 
	vector <complex<double>> y_aux(inTimeDomainComplex.size());
	unsigned int startPosition = 0;

	for (unsigned int r = 0; r < matrix.size(); r++)
	{

		vector <complex<double>> tempVectorB = matrix[r];

		for (unsigned int s = 0; s < L; s++)
		{
			y_aux[startPosition + s] = tempVectorB[overlap + s];
		}

		startPosition = startPosition + L;
	}

	// Next, discard last "extrazero" samples. 
	vector <complex<double>> y(inTimeDomainComplex.size() - extraZero);

	for (unsigned int t = 0; t < y_aux.size() - extraZero; t++)
	{
		y[t] = y_aux[t];
	}
	
	//////////////////////////////// Section 6 ///////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////
	/////////////////////////////// Display Section //////////////////////////////
	//////////////////////////////////////////////////////////////////////////////
	/*
	// Display length of data
	cout << "\nTotal length of of data,     Dl = " << count << endl;
	cout << "\nTotal length of of Filter,	M = " << countFilter << endl;
	cout << "\nTotal length of of Filter after zero padding = " << filterFD.size() << endl;
	cout << "\nTotal length of of data after zero padding,    Dlnew = " << inTimeDomainComplex.size() << " (This is because to make the data length of itnger multiple of L)" << endl;

	cout << endl << endl;

	cout << "length of data block size,  L = " << L << endl;
	cout << "length of FFT size,         N = " << N << endl;
	cout << "Length of overlap,    overlap = " << overlap << endl;*/

	return y;

}

std::vector<std::complex<double>> overlapSaveRealTime(std::vector<std::complex<double> > &inTimeDomainComplex1, std::vector<std::complex<double> > &inTimeDomainComplex2, std::vector<std::complex<double> > &inTimeDomainFilterComplex)
{
	vector<complex<double>> currentCopy = inTimeDomainComplex1;
	vector<complex<double>> previousCopy = inTimeDomainComplex2;
	vector<complex<double>> dataBlock = inTimeDomainComplex2;
	vector<complex<double>> filter = inTimeDomainFilterComplex;
	size_t overlap = previousCopy.size();

	for (unsigned int i = 0; i < currentCopy.size(); i++)
	{
		dataBlock.push_back(currentCopy[i]);
	}

	size_t N = dataBlock.size();

	if ((N & (N - 1)) == 0)
	{
		N = N;
	}
	else
	{
		size_t m = 1;
		while (m <= N)					// This calculates next value of power of 2
		{
			m *= 2;
		}

		N = m;
	}

	vector<complex<double>> dataBlockModified(N);
	for (unsigned int i = 0; i < dataBlock.size(); i++)
	{
		dataBlockModified[i] = dataBlock[i];
	}


	vector<complex<double>> filterkModified(N);
	for (unsigned int i = 0; i < filter.size(); i++)
	{
		filterkModified[i] = filter[i];
	}

	vector<complex<double>> y(N);

	y = overlapSave(dataBlock,filter);

	

	return y;
}
