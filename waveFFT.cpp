#include <iostream>
#include <cmath>
#include <fstream>
#include <cstdlib>
#include <cmath>
#include <iterator>
#include "fft/FFTRealFixLen.h"

// function prototypes
void sinWaveGenerator1();	// first attempt at sin wave generator
double* sinWaveGenerator2();	// second attempt at sin wave generator
void fileCreation(double value[], int dataAmount);	// outputs to a file
void fileCreation2(double value[], int dataAmount);	// outputs to a file
void fftConversion();	// converts waveform into fft

int main()
{
	
	fftConversion();

	return 0;
}

void sinWaveGenerator1()
{
	int sampleRate = 44100;		// samples per second
	double samplesSecond = 1000;	// multiplier for cycles per second
	double cycle = 2 * M_PI;	// 1 cycle length
	double cycleSecond = cycle * samplesSecond;	// cycles per second

	double value[sampleRate];

	for(int i = 0; i < sampleRate; i++)
	{
		value[i] = sin(cycleSecond * i / sampleRate);	// number of cycles per number of sample
		std::cout << value[i] << std::endl;
	}
}

double* sinWaveGenerator2()
{
	int dataAmount = 512;					// number of samples recorded
	double amplitude = 1;					// peak deviation of the function from zero
	double ordinaryFrequency = 1000;			// number of oscillations (cycles) each second
	double angularFrequency = 2 *M_PI * ordinaryFrequency;	// 2*pi*f measure of rotation
	double phase = 0;					// phase shift
//	int count = 0;	
	//double waveForm;

	std::ofstream outFile("record.txt", std::ios::out);

	double *value = new double[dataAmount];
//	static double value[512];

	for(int time = 0; time < dataAmount; time++)
	{
		value[time] = amplitude * sin(angularFrequency * time + phase);// number of cycles per number of sample
//		std::cout << value[time] << std::endl;	// prints output of waveform

//		fileCreation(outFile, value, time);
		
//		count++;
	}
	return value;
//	fileCreation(value, dataAmount);
//	std::cout << count << std::endl;
}

void fileCreation(double value[], int dataAmount)
{
	std::ofstream outFile("record.txt", std::ios::out);

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

void fileCreation2(double value[], int dataAmount)
{
	std::ofstream outFile("record2.txt", std::ios::out);

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
	const int exponentOfTwo = 9;	// 2^exponentOfTwo
	int euclid = pow(2, exponentOfTwo);	// 2^exponentOfTwo result
	double *preFFT;			// array to be converted to FFT
//	double preFFT[euclid];		// array to be converted to FFT
//	double *postFFT;		// array of FFT conversion
	double postFFT[euclid];		// array of FFT conversion
	double conversionFFT[(euclid / 2) - 2];	// converting FFT into readable data for plotting
	
	ffft::FFTRealFixLen <exponentOfTwo> fft;
	
	preFFT = sinWaveGenerator2();

/*	
	for(int i = 0; i < euclid; i++)
	{
		preFFT[i] = *p;
		*p++;
	}
*/
	fft.do_fft(postFFT, preFFT);		// performs fft on array

//	for(int i = 0; i < euclid; i++)
//	{
//		std::cout << *(postFFT + i) << std::endl;	// prints the contents of the element
//	}


//	std::cout << euclid << std::endl;
	
	fileCreation(postFFT, euclid);	// generates file of results
	
	for(int i = 1; i < ((euclid / 2) - 2); i++)
	{
		conversionFFT[i] = postFFT[i] + postFFT[(euclid / 2) + i];	// FFT a + bi removes the first and middle value
	}	
	
	fileCreation2(conversionFFT, ((euclid / 2) -2));	
}
