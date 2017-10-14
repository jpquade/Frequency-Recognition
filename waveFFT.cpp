#include <iostream>
#include <cmath>
#include <fstream>
#include <cstdlib>
#include <cmath>
#include <iterator>
#include <string>
#include "fft/FFTRealFixLen.h"

// function prototypes
void sinWaveGenerator1();	// first attempt at sin wave generator
double* sinWaveGenerator2();	// second attempt at sin wave generator
double* sinWaveGenerator3(int euclid, double sampleRate);	// third attempt at sin wave generator
void fileCreation(double value[], int dataAmount, std::string fileName);	// outputs to a file
void fftConversion();	// converts waveform into fft

int main()
{
	
	fftConversion();

	return 0;
}

void sinWaveGenerator1()
{
	int sampleRate = 44100;		// samples per second
	double samplesSecond = 1000;	// multiplier for cycles per second (frequency)
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
	double ordinaryFrequency = 100;			// number of oscillations (cycles) each second
	double angularFrequency = 2 *M_PI * ordinaryFrequency / 360;	// 2*pi*f measure of rotation
	double phase = 0;					// phase shift
//	int count = 0;	
	//double waveForm;

//	std::ofstream outFile("record3.txt", std::ios::out);

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

double* sinWaveGenerator3(int euclid, double sampleRate)
{		
				
	double amplitude = 1;					// peak deviation of the function from zero
	double ordinaryFrequency = 100;			// number of oscillations (cycles) each second
	double angularFrequency = 2 * M_PI * ordinaryFrequency;	// 2*pi*f measure of rotation
	double phase = 0;					// phase shift
	double *value = new double[euclid];
	double sampleFrequency = 1 / sampleRate;

	for(int i = 0; i < euclid; i++)
	{		
		value[i] = amplitude * sin(angularFrequency * sampleFrequency + phase);	// number of cycles per number of sample
		sampleFrequency += (1 / sampleRate);
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
	const int exponentOfTwo = 10;	// 2^exponentOfTwo
	int euclid = pow(2, exponentOfTwo);	// 2^exponentOfTwo result
	double *preFFT;			// array to be converted to FFT
//	double preFFT[euclid];		// array to be converted to FFT
//	double *postFFT;		// array of FFT conversion
	double postFFT[euclid];		// array of FFT conversion
	double conversionFFT[(euclid / 2) - 2];	// converting FFT into readable data for plotting
	double sampleRate = 1500;	
	double binFrequency = (double)sampleRate / (double)euclid;		// sample rate / FFT size
	double bins[euclid];	// stores the bin size data
	std::string file1 = "1sinWave";
	std::string file2 = "2FFTInitial";
	std::string file3 = "3FFTBins";	
	std::string file4 = "4FFTAmplitude";
	
	ffft::FFTRealFixLen <exponentOfTwo> fft;
	
	preFFT = sinWaveGenerator3(euclid, sampleRate);
	fileCreation(preFFT, euclid, file1);

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
	
//	fileCreation(postFFT, euclid, file2);	// generates initial fft data

	for(int i = 0; i < euclid; i++)
	{
		bins[i] = (double)i * binFrequency;
	}

//	fileCreation(bins, euclid, file3);	// generates bin size data
	
	
//	for(int i = 1; i < ((euclid / 2) - 1); i++)
	for(int i = 1; i < (euclid / 2); i++)
	{
		conversionFFT[i] = postFFT[i] + postFFT[(euclid / 2) + i];	// FFT a + bi removes the first and middle value
	}	
	
//	fileCreation(conversionFFT, ((euclid / 2) - 1), file3);	// formats and reconfigures fft for analysis
//	fileCreation(conversionFFT, (euclid / 2), file3);	// formats and reconfigures fft for analysis	
}
