#pragma once

#include <iostream>

class NN
{
public:
	int X1m = 100;
	int X2m = 200;
	float X1[100][200];
	float X2[200];
	NN();
	~NN();

	void setInitialRandom();
	void loadDataFromFile(const char* filename);
	void saveDataToFile(const char* filename);
	float energyFunctional();
	float getDerivativeX1(int k, int j);
	float getDerivativeX2(int j);

	void runNN(int vNum);
	void coutValues();
	void coutX1();
	void coutX2();
	void coutX1X2();
};

