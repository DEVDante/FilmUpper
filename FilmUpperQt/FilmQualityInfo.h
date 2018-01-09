#pragma once

class FrameRate {
public:
	int num;
	int den;

	FrameRate(int n, int d) {
		num = n;
		den = d;
	}

	double getNumericalRate() {
		return (double)num / (double)den;
	}
};

class FilmQualityInfo {
public:
	int Width;
	int Height;
	FrameRate* FrameRate;
	int SampleRate;
};

