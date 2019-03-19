# include "overlap_save_20180208.h"

# include <complex>
# include <fstream>
# include <iostream>
# include <math.h>
# include <stdio.h>
# include <string>
# include <strstream>
# include <algorithm>
# include <vector>

using namespace std;

int main()
{
	//////////////////////////////// Section 1 ///////////////////////////////////
	////// Read the time_domain_data.txt and time_domain_filter.txt files  ///////
	//////////////////////////////////////////////////////////////////////////////
	ifstream inFile;
	double ch;
	vector <double> inTimeDomain;
	inFile.open("time_domain_data.txt");

	// First data (at 0th position) appled to the ch it is similar to the "cin".
	inFile >> ch;

	// It'll count the length of the vector to verify with the MATLAB
	int count = 0;

	while (!inFile.eof())
	{
		// push data one by one into the vector
		inTimeDomain.push_back(ch);

		// it'll increase the position of the data vector by 1 and read full vector.s
		inFile >> ch;
		count++;
	}

	inFile.close();	// It is mandatory to close the file at the end.

	ifstream inFileFilter;
	double chFilter;
	vector <double> inTimeDomainFilter;
	inFileFilter.open("time_domain_filter.txt");
	inFileFilter >> chFilter;
	int countFilter = 0;

	while (!inFileFilter.eof())
	{
		inTimeDomainFilter.push_back(chFilter);
		inFileFilter >> chFilter;
		countFilter++;
	}
	inFileFilter.close();

	//////////////////////////////// Section 2 ///////////////////////////////////
	///////////////////////// Real to complex conversion /////////////////////////
	//////////////////////////////////////////////////////////////////////////////
	////////////// For signal data  //////////////
	vector <complex<double>> inTimeDomainComplex(inTimeDomain.size());
	vector <complex<double>> fourierTransformed;
	vector <double> re(inTimeDomain.size());
	vector <double> im(inTimeDomain.size());

	for (unsigned int i = 0; i < inTimeDomain.size(); i++)
	{
		re[i] = inTimeDomain[i]; // Real data of the signal
		im[i] = 0; 		         // Imaginary data of the signal
	}
	// Next, Real and Imaginary vector to complex vector conversion
	inTimeDomainComplex = reImVect2ComplexVector(re, im);
	
	////////////// For filter data  //////////////
	vector <complex<double>> inTimeDomainFilterComplex(inTimeDomainFilter.size());
	vector <double> reFilter(inTimeDomainFilter.size());
	vector <double> imFilter(inTimeDomainFilter.size());

	for (unsigned int i = 0; i < inTimeDomainFilter.size(); i++)
	{
		reFilter[i] = inTimeDomainFilter[i];
		imFilter[i] = 0;
	}
	inTimeDomainFilterComplex = reImVect2ComplexVector(reFilter, imFilter);

	//////////////////////////////// Section 3 /////////////////////////////////
	////////////////////////////// Overlap & save //////////////////////////////
	////////////////////////////////////////////////////////////////////////////
	vector <complex<double>> y;
	y = overlapSave(inTimeDomainComplex, inTimeDomainFilterComplex);

	//////////////////////////////// Section 4 /////////////////////////////////
	//////////////////////////////// Save data /////////////////////////////////
	////////////////////////////////////////////////////////////////////////////
	ofstream outFile;
	complex<double> outFileData;
	outFile.precision(20);
	outFile.open("overlap_save_data.txt");

	for (unsigned int i = 0; i <y.size(); i++)
	{
		outFile << y[i].real() << endl;
		outFile << y[i].imag() << endl;
	}
	outFile.close();

	cout << "Execution finished! Please hit enter to exit." << endl;
	getchar();
	return 0;
}
