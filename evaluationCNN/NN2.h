#pragma once
class NN2
{
public:
	NN2();
	~NN2();
public:
	int X1m = 100;
	int X2m = 100;
	int X3m = 100;
	float X1[100][100];
	float X2[100][100];
	float X3[100];

	void setInitialRandom();
	void loadDataFromFile(const char* filename);
	void saveDataToFile(const char* filename);
	float energyFunctional();
	float getDerivativeX1(int k, int j);
	float getDerivativeX2(int j, int i);
	float getDerivativeX3(int i);

	void runNN(int vNum);
	void coutValues();
	void coutX1();
	void coutX2();
	void coutX3();
	void coutX1X2X3();

};

