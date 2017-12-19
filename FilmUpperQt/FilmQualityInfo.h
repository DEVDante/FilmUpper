#pragma once

class FilmQualityInfo {
public:
	int FrameSizeX;
	int FrameSizeY;
	FrameRate FrameRate;
};

class FrameRate {
public:
	int num;
	int den;

	FrameRate(int n, int d) {
		num = n;
		den = d;
	}

	double getNumericalRate() {
		return num / den;
	}
};