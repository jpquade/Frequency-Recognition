#include <iostream>
#include <cmath>
#include <fstream>
#include <cstdlib>
#include <cmath>
#include <iterator>
#include <string>
#include <time.h>
#include "fft/FFTRealFixLen.h"

// function prototypes
double* sinWaveGenerator(int euclid, double sampleRate);			// sin wave generator
double* randSinWaveGenerator(int euclid, double sampleRate, int frequencyAdded, int euclidMod);	// multiple sin wave generator
double* staticGenerator(int euclid, double sampleRate, int frequencyAdded, int euclidMod);
void fftReader(double bins[], double conversionFFT[], int dataAmount);
void fileCreation(double value[], int dataAmount, std::string fileName);	// outputs to a file
void fftConversion();
								// converts waveform into fft

int main()
{
	srand (time(NULL));	// random seed
	fftConversion();	// start program

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

double* randSinWaveGenerator(int euclid, double sampleRate, int frequencyAdded, int euclidMod)
{		
				
	double amplitude;				// peak deviation of the function from zero
	double ordinaryFrequency;			// number of oscillations (cycles) each second
	double phase = 0;				// phase shift
	double *value = new double[euclid];		// array of sin wave values
	double sampleFrequency = 1 / sampleRate;	// sampling frequency rate	

	std:: cout << "Frequency\tAmplitude" << std::endl;

	for(int j = 0; j < frequencyAdded; j++)
	{
		ordinaryFrequency = (double)(rand() % 100 + 1);				// random frequency between 1-100
//		std::cout << "Frequency" << j << " " << ordinaryFrequency;		// prints out the current frequency
		amplitude = (double)(rand() % 35 + 10);					// random frequency between 10-35
//		std::cout << " Amplitude" << j << " " << amplitude << std::endl;	// prints out the current amplitude
		
		std::cout << ordinaryFrequency << "\t\t" << amplitude << std::endl; 	// prints out current frequency/amplitude

		for(int i = 0; i < euclid; i++)
		{				
			value[i] += amplitude * sin(2 * M_PI * ordinaryFrequency * sampleFrequency + phase);	// number of cycles per number of sample
			sampleFrequency += (1 / sampleRate);	// increments the sample frequency			
		}
		sampleFrequency = 1 / sampleRate;
	}

	return value;
}

double* staticGenerator(int euclid, double sampleRate, int frequencyAdded, int euclidMod)
{
	double *value = new double[euclid];		// array of sin wave values
	double amplitude = (double)(rand() % 8 + 1);	// random peak deviation of the function from zero between 1-8
	double sampleFrequency = 1 / sampleRate;	// sampling frequency rate
	
	std::cout << "Static Amplitude" << " " << amplitude << std::endl;	// prints out the current amplitude	

	for(int i = 0; i < euclid; i++)
	{			
		value[i] += amplitude * sin((double(rand() % 300 + 1)) * sampleFrequency);	// number of cycles per number of sample
		sampleFrequency += (1 / sampleRate);			// increments the sample frequency			
	}
	sampleFrequency = 1 / sampleRate;

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

void fftReader(double bins[], double conversionFFT[], int dataAmount)
{
	
	std::cout << std::endl << "Frequency\tAmplitude" << std::endl;

	for(int i = 0; i < dataAmount; i++)
	{
		if((int)conversionFFT[i] > 5)
		{
			std::cout << bins[i] << "\t\t" << conversionFFT[i] << std::endl;
		}		
	}
	
	std::cout << std::endl;
}

void fftConversion()
{
	const int exponentOfTwo = 14;		// 2^exponentOfTwo
	int euclid = pow(2, exponentOfTwo);	// 2^exponentOfTwo result
	int euclidMod = (euclid / 2) - 1;
	double *preFFT;				// array of sin wave values
//	double preFFT[euclid];			// array to be converted to FFT
//	double *postFFT;			// array of FFT conversion
	double postFFT[euclid];			// array of FFT bins
	double conversionFFT[euclidMod];	// converting FFT into readable data for plotting
	double sampleRate = 15000;		// frequency of sampling rate
	double binSize = (double)euclidMod;		// changed euclidMod to a double
	double binFrequency = sampleRate / (binSize * 2);	// adjusted bin frequency
	double bins[euclidMod];					// stores the bin size data
	std::string file1 = "1sinWave";
	std::string file2 = "2FFTInitial";
	std::string file3 = "3FFTFrequencyBins";	
	std::string file4 = "4FFTAmplitude";
	
	ffft::FFTRealFixLen <exponentOfTwo> fft;
	
	preFFT = randSinWaveGenerator(euclid, sampleRate, 5, euclidMod);	// generates multiple random sin waves

	fileCreation(preFFT, euclid, file1);			// file for sin waves

	fft.do_fft(postFFT, preFFT);			// performs fft on array

	
	fileCreation(postFFT, euclid, file2);		// file for initial fft data

	for(int i = 0; i < euclidMod; i++)
	{
		bins[i] = (double)i * binFrequency;
	}

	fileCreation(bins, euclidMod, file3);		// file for frequency bins
	
	for(int i = 1; i < euclidMod; i++)
	{
		conversionFFT[i] = sqrt(pow(postFFT[i], 2) + pow((postFFT[(euclid / 2) + i]), 2)) / euclidMod;	// FFT (a + bi)  magnitude / numberofbins
	}	
	
	fileCreation(conversionFFT, euclidMod, file4);	// file for aplitudes

	fftReader(bins, conversionFFT, euclidMod);

	for(int i = 0; i < 10; i++)
	{
		preFFT = randSinWaveGenerator(euclid, sampleRate, 5, euclidMod);	// generates multiple random sin waves
		fft.do_fft(postFFT, preFFT);			// performs fft on array

		for(int i = 0; i < euclidMod; i++)
		{
			bins[i] = (double)i * binFrequency;
		}

		for(int i = 1; i < euclidMod; i++)
		{
			conversionFFT[i] = sqrt(pow(postFFT[i], 2) + pow((postFFT[(euclid / 2) + i]), 2)) / euclidMod;	// FFT (a + bi)  magnitude / numberofbins
		}

		fftReader(bins, conversionFFT, euclidMod);
	
	}
}
