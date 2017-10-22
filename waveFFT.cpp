#include <iostream>
#include <cmath>
#include <fstream>
#include <cstdlib>
#include <cmath>
#include <iterator>
#include <string>
#include "fft/FFTRealFixLen.h"

// function prototypes
double* sinWaveGenerator(int euclid, double sampleRate);			// sin wave generator
void fileCreation(double value[], int dataAmount, std::string fileName);	// outputs to a file
void fftConversion();								// converts waveform into fft

int main()
{
	fftConversion();

	return 0;
}

double* sinWaveGenerator(int euclid, double sampleRate)
{		
				
	double amplitude = 10;					// peak deviation of the function from zero
	double ordinaryFrequency = 100;				// number of oscillations (cycles) each second
	double angularFrequency = 2 * M_PI * ordinaryFrequency;	// 2*pi*f measure of rotation
	double phase = 0;					// phase shift
	double *value = new double[euclid];			// array of sin wave values
	double sampleFrequency = 1 / sampleRate;		// sampling frequency rate

	for(int i = 0; i < euclid; i++)
	{		
		value[i] = amplitude * sin(angularFrequency * sampleFrequency + phase);	// number of cycles per number of sample
		sampleFrequency += (1 / sampleRate);					// increments the sample frequency
	}

	return value;
}

void fileCreation(double value[], int dataAmount, std::string fileName)
{
	std::ofstream outFile(fileName.c_str(), std::ios::out);

	if(!outFile)
	{
		std::cerr << "File could not be opened" << std::endl;
		exit(1);
	}
	
	for(int time = 0; time < dataAmount; time++)
	{
		outFile << value[time] << std::endl;	// value to file
	}
}

void fftConversion()
{
	const int exponentOfTwo = 12;		// 2^exponentOfTwo
	int euclid = pow(2, exponentOfTwo);	// 2^exponentOfTwo result
	int euclidMod = (euclid / 2) - 1;
	double *preFFT;				// array of sin wave values
//	double preFFT[euclid];			// array to be converted to FFT
//	double *postFFT;			// array of FFT conversion
	double postFFT[euclid];			// array of FFT bins
	double conversionFFT[euclidMod];	// converting FFT into readable data for plotting
	double sampleRate = 1500;		// frequency of sampling rate
	double binSize = (double)euclidMod;		// bins adjusted
	double binFrequency = sampleRate / (binSize * 2);	// adjusted bin frequency
	double bins[euclid];					// stores the bin size data
	std::string file1 = "1sinWave";
	std::string file2 = "2FFTInitial";
	std::string file3 = "3FFTFrequencyBins";	
	std::string file4 = "4FFTAmplitude";
	
	ffft::FFTRealFixLen <exponentOfTwo> fft;
	
	preFFT = sinWaveGenerator(euclid, sampleRate);	// generates test sin wave
	fileCreation(preFFT, euclid, file1);

	fft.do_fft(postFFT, preFFT);			// performs fft on array

	
	fileCreation(postFFT, euclid, file2);		// generates initial fft data

	for(int i = 0; i < euclidMod; i++)
	{
		bins[i] = (double)i * binFrequency;
	}

	fileCreation(bins, euclid, file3);		// generates bin size data
	
	for(int i = 1; i < euclidMod; i++)
	{
		conversionFFT[i] = sqrt(pow(postFFT[i], 2) + pow((postFFT[(euclid / 2) + i]), 2)) / euclidMod;	// FFT a + bi removes the first and middle value
	}	
	
	fileCreation(conversionFFT, euclidMod, file4);	// formats and reconfigures fft for analysis	
}
