all: waveFFT

waveFFT : waveFFT.o
	g++ -o waveFFT waveFFT.cpp

clean:
	rm -f waveFFT waveFFT.o 1sinWave 2FFTInitial 3FFTBins 4FFTAmplitude
