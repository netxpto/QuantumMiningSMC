# include "overlap_save_20180208.h"
# include "fft_20180208.h"
#include <iostream>

#ifndef M_PI
#define M_PI  3.14159265358979323846
#endif


//########################################################################################################################################################
//############################################################### FFT Module #############################################################################
//########################################################################################################################################################

////////////
//// 13 ////
////////////
std::vector<std::complex<double>> overlapSave(std::vector<std::complex<double> > &inTimeDomainComplex, std::vector<std::complex<double> > &inTimeDomainFilterComplex)
{
	size_t impulseResponseLength = inTimeDomainFilterComplex.size();  // Initial size of filter
	size_t N;									  // FFT size

	if ((impulseResponseLength & (impulseResponseLength - 1)) == 0)						  // Is power of 2
	{
		N = impulseResponseLength * 2;
	}
	else
	{
		size_t m = 1;
		while (m <= impulseResponseLength)					// This calculates next value of power of 2
		{
			m *= 2;
		}

		N = m;
	}

	// Now, check if the value of (N-impulseResponseLength) is less than or equal to the 25% of N,
	// then double the size of the N.

	size_t L = N - impulseResponseLength + 1;			// Size of data block (Here, we used fix 50% overlap)
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
	filterFD = ifft(filterTD);



	//////////////////////////////// Section 4 /////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////
	////////////////////////////// Create Matrix ///////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////


	size_t nr = inTimeDomainComplex.size() / L;

	vector < vector<complex<double>> > matrix;
	vector <complex<double>> tempVector(L); // Without overlap
	vector <complex<double>> tempVectorA(N); // With overlap


	size_t position = 0;

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

		dataBlockTD = ifft(dataBlockTD);	 // Here to very with MATLAB, multiply this data with SQRT(N)

		for (unsigned int q = 0; q < dataBlockTD.size(); q++)
		{
			dataBlockTD[q] = dataBlockTD[q] * filterFD[q]; // Multiplication with filter data
		}

		dataBlockTD = fft(dataBlockTD);				  // ifft to get filtered data

		matrix[p] = dataBlockTD;						  // save data to matrix
	}

	// Next, create vector from a matrix by disarding the overlap 
	vector <complex<double>> y_aux(inTimeDomainComplex.size());
	size_t startPosition = 0;

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


	return y;

}

////////////
//// 14 ////
////////////
std::vector<std::complex<double>> overlapSave(std::vector<std::complex<double> > &inTimeDomainComplex1, std::vector<std::complex<double> > &inTimeDomainComplex2, std::vector<std::complex<double> > &inTimeDomainFilterComplex)
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

	vector<complex<double>> y(N);

	y = overlapSave(dataBlock, filter);

	return y;
}
std::vector<std::complex<double>> overlapSaveImpulseResponse(std::vector<std::complex<double> > &inTimeDomainComplex1, std::vector<std::complex<double> > &inTimeDomainComplex2, std::vector<std::complex<double> > &inTimeDomainFilterComplex)
{
	vector<complex<double>> currentCopy = inTimeDomainComplex1;
	vector<complex<double>> previousCopy = inTimeDomainComplex2;
	vector<complex<double>> filter = inTimeDomainFilterComplex;

	size_t M = filter.size();
	size_t L = currentCopy.size();
	size_t N = L + M - 1;    // N-Point FFT
	size_t overlap = M - 1;

	vector<complex<double>> dataBlock;
	for (unsigned int i = 0; i < overlap; i++){	dataBlock.push_back(previousCopy[previousCopy.size()-overlap + i]); }

	for (unsigned int i = 0; i < L; i++){ dataBlock.push_back(currentCopy[i]); }

	vector<complex<double>> dataBlockFFT; // FFT data
	dataBlockFFT = ifft(dataBlock);
	for (unsigned int i = 0; i < N; i++){ dataBlockFFT[i] = dataBlockFFT[i] * sqrt(N);}

	vector<complex<double>> filterFD(N); // FFT filter
	for (unsigned int i = 0; i < M; i++) { filterFD[i] = filter[i]; } // zero padding to the filter
	ifft(filterFD);

	vector<complex<double>> Y(N);
	for (unsigned int i = 0; i < N; i++) { Y[i] = dataBlockFFT[i] * filterFD[i]; } // Multiplications

	fft(Y); // IFFT 

	vector<complex<double>> out(L);
	for (unsigned int i = 0; i < L; i++) { out[i]=Y[overlap+i]; }

	return out;
}

////////////
//// 15 ////
////////////
std::vector<std::complex<double>> overlapSaveTransferFunction(std::vector<std::complex<double> > &inTimeDomainComplex1, std::vector<std::complex<double> > &inTimeDomainComplex2, std::vector<std::complex<double> > &inTimeDomainFilterComplex)
{
	vector<complex<double>> currentCopy = inTimeDomainComplex1;
	vector<complex<double>> previousCopy = inTimeDomainComplex2;
	vector<complex<double>> filter = inTimeDomainFilterComplex;

	vector<complex<double>> dataBlock(previousCopy .size()+currentCopy.size());
	
	if (dataBlock.size() == filter.size())
	{
	for (unsigned int i = 0; i < previousCopy.size(); i++) { dataBlock[i] = previousCopy[i]; }
	for (unsigned int i = 0; i < currentCopy.size(); i++) { dataBlock[previousCopy.size() + i] = currentCopy[ i]; }

	vector<complex<double>> dataBlockFFT; // FFT data
	dataBlockFFT = ifft(dataBlock);

	vector<complex<double>> Y(dataBlock.size());
	for (unsigned int i = 0; i < dataBlock.size(); i++) { Y[i] = dataBlockFFT[i] * filter[i]; } // Multiplications

	fft(Y); // IFFT 

	vector<complex<double>> out(previousCopy.size() + currentCopy.size());
	for (unsigned int i = 0; i <out.size(); i++) { out[i] = Y[i]; } // remove part corrupted due to aliazing (Suppose length of TF is 512)

	vector<complex<double>> y;
	for (size_t i = 0; i < currentCopy.size(); i++)
	{
		y.push_back(out[previousCopy.size() - filter.size() / 4 + i]);
	}



	return y;

	}
	else
	{
		for (unsigned int i = 0; i < previousCopy.size(); i++) { dataBlock[i] = previousCopy[i]; }
		for (unsigned int i = 0; i < currentCopy.size(); i++) { dataBlock[previousCopy.size() + i] = currentCopy[i]; }

		size_t N = filter.size();
		size_t L = dataBlock.size();
		size_t overlap = N/2;
		size_t startingPoint = previousCopy.size() - overlap;

		vector<complex<double>> data = dataBlock;

		//vector<complex<double>> data(L - startingPoint);

		//for ( int i = 0; i <data.size(); i++) { data[i] = dataBlock[startingPoint + i]; }

		size_t nr = data.size() / (N/2);
		size_t position = 0;

		vector<complex<double>> tempVector(N);
		vector < vector<complex<double>> > matrix;


		for (size_t i = 0; i < nr - 1; i++)
		{
			for (size_t i = 0; i < N; i++)
			{
				tempVector[i] = data[position + i];
			}

			matrix.push_back(tempVector);
			position = position + (N / 2);

		}


		for (size_t i = 0; i < matrix.size(); i++)
		{
			vector<complex<double>> dataBlockFFT;
			dataBlockFFT = ifft(matrix[i]);

			vector<complex<double>> Y(dataBlockFFT.size());
			for (unsigned int i = 0; i < dataBlockFFT.size(); i++) { Y[i] = dataBlockFFT[i] * filter[i]; } // Multiplications

			fft(Y); // IFFT 

			matrix[i] = Y;
		}

		
		vector<complex<double>> out;
		vector<complex<double>> outAux;

		for (size_t i = 0; i < matrix.size(); i++)
		{
			outAux = matrix[i];
			if (i == 0)
			{
				for (size_t i = 0; i < outAux.size()- outAux.size()/4; i++)
				out.push_back(outAux[i]);
			}
			else if (i == matrix.size() - 1)
			{
				for (size_t i = 0; i < outAux.size() - outAux.size() / 4; i++)
					out.push_back(outAux[outAux.size() / 4 + i]);
			}
			else
			{
				for (size_t i = 0; i < outAux.size() - outAux.size() / 2; i++)
					out.push_back(outAux[outAux.size() / 4 + i]);
			}

		}

		vector<complex<double>> y;
		for (size_t i = 0; i < currentCopy.size(); i++)
		{
			y.push_back(out[previousCopy.size()-filter.size()/4 + i]);
		}



		return y;

	}
}

////////////
//// 16 ////
////////////
std::vector<std::complex<double>> conv(std::vector<std::complex<double>> &inTimeDomainComplex1, std::vector<std::complex<double>> &inTimeDomainComplex2)
{
	// Here we will calculate the length of the linear convolution L = M+N-1
	size_t M = inTimeDomainComplex1.size();
	size_t N = inTimeDomainComplex2.size();

	size_t L = M + N - 1;

	vector<std::complex<double>> xn(L);
	vector<std::complex<double>> hn(L);

	// Change length of both vectr by zero padding
	for (unsigned int i = 0; i < L; i++)
	{
		xn[i] = inTimeDomainComplex1[i];
		hn[i] = inTimeDomainComplex2[i];
	}

	// Calculate linear convolution using Circular convolution conversion
	vector<std::complex<double>> XN(L);
	vector<std::complex<double>> HN(L);
	vector<std::complex<double>> y(L);

	XN = fft(xn);
	HN = fft(hn);
	y = complexVectorMultiplication(XN, HN);
	y = ifft(y);

	return y;
}

////////////
//// 17 ////
////////////
std::vector<std::complex<double>> circularConv(std::vector<std::complex<double>> &inTimeDomainComplex1, std::vector<std::complex<double>> &inTimeDomainComplex2) 

{
	// Here we will calculate the length of the linear convolution L = min(impulseResponseLength,N)
	size_t M = inTimeDomainComplex1.size();
	size_t N = inTimeDomainComplex2.size();

	size_t L = min(M, N);

	vector<std::complex<double>> xn(L);
	vector<std::complex<double>> hn(L);

	// Change length of both vectr by zero padding
	for (unsigned int i = 0; i < L; i++)
	{
		xn[i] = inTimeDomainComplex1[i];
		hn[i] = inTimeDomainComplex2[i];
	}

	// Calculate linear convolution using Circular convolution conversion
	vector<std::complex<double>> XN(L);
	vector<std::complex<double>> HN(L);
	vector<std::complex<double>> y(L);

	XN = fft(xn);
	HN = fft(hn);
	y = complexVectorMultiplication(XN, HN);
	y = ifft(y);

	return y;

}

