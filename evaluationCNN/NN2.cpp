
#include <random>  
#include "NN2.h"
#include "estVector.h"

using namespace std;


NN2::NN2()
{
}


NN2::~NN2()
{
}



void NN2::setInitialRandom() {
	random_device rd;   // non-deterministic generator  
	mt19937 gen(rd());  // to seed mersenne twister.  
	for (int k = 0; k < X1m; k++) {
		for (int j = 0; j < X2m; j++) {
			int a = (gen() % 200 - 100);
			X1[k][j] = 0.001f * a;
		}
	}
	for (int j = 0; j < X2m; j++) {
		for (int i = 0; i < X3m; i++) {
			int a = (gen() % 200 - 100);
			X2[j][i] = 0.001f * a;
		}
	}
	for (int i = 0; i < X3m; i++) {
		int a = (gen() % 200 - 100);
		X3[i] = 0.001f * a;
	}
}

void NN2::loadDataFromFile(const char* filename) {

	ifstream ifs(filename);// ƒtƒ@ƒCƒ‹‚ðŠJ‚­
	char str[40];
	if (ifs.fail()) {
		std::cerr << "Ž¸”s" << std::endl;
		return;
	}
	for (int k = 0; k < X1m; k++) {
		for (int j = 0; j < X2m; j++) {
			if (ifs.getline(str, 40 - 1)) {
				X1[k][j] = (float)atof(str);
			}
			else {
				std::cerr << "Ž¸”s" << std::endl;
				return;
			}
		}
	}
	for (int j = 0; j < X2m; j++) {
		for (int i = 0; i < X3m; i++) {
			if (ifs.getline(str, 40 - 1)) {
				X2[j][i] = (float)atof(str);
			}
			else {
				std::cerr << "Ž¸”s" << std::endl;
				return;
			}
		}
	}
	for (int i = 0; i < X3m; i++) {
		if (ifs.getline(str, 40 - 1)) {
			X3[i] = (float)atof(str);
		}
		else {
			std::cerr << "Ž¸”s" << std::endl;
			return;
		}
	}
}

void NN2::saveDataToFile(const char* filename) {
	ofstream ofs(filename);// ƒtƒ@ƒCƒ‹‚ðŠJ‚­
	for (int k = 0; k < X1m; k++) {
		for (int j = 0; j < X2m; j++) {
			ofs << X1[k][j] << endl;
		}
	}
	for (int j = 0; j < X2m; j++) {
		for (int i = 0; i < X3m; i++) {
			ofs << X2[j][i] << endl;
		}
	}
	for (int i = 0; i < X3m; i++) {
		ofs << X3[i] << endl;
	}
	ofs.close();
}

float NN2::energyFunctional() {
	float ret = 0.0f;
	for (ESTIMATION e : estVec) {
		float w[100];
		for (int j = 0; j < X2m; j++) {
			w[j] = 0.0f;
			for (int k = 0; k < X1m; k++) {
				w[j] += (e.est[k] * X1[k][j]);
			}
		}
		float v[100];
		for (int i = 0; i < X3m; i++) {
			v[i] = 0.0f;
			for (int j = 0; j < X2m; j++) {
				v[i] += (w[j] * X2[j][i]);
			}
		}
		float u = 0.0f;
		for (int i = 0; i < X3m; i++) {
			u += (v[i] * X3[i]);
		}
		float a = (e.evaluation - u);
		if (a < 0)a = -a;
		ret += a;
	}
	return ret;
}

float NN2::getDerivativeX1(int k, int j) {
	if (k < 0 || k >= X1m || j < 0 || j >= X2m) {
		return 0.0f;
	}
	float val0 = energyFunctional();
	float tmp = X1[k][j];
	X1[k][j] = tmp + 0.01f;
	float val1 = energyFunctional();
	X1[k][j] = tmp;
	return val1 - val0;
}

float NN2::getDerivativeX2(int j, int i) {
	if (j < 0 || j >= X2m || i < 0 || i >= X3m) {
		return 0.0f;
	}
	float val0 = energyFunctional();
	float tmp = X2[j][i];
	X2[j][i] = tmp + 0.01f;
	float val1 = energyFunctional();
	X2[j][i] = tmp;
	return val1 - val0;
}

float NN2::getDerivativeX3(int i) {
	if (i < 0 || i >= X3m) {
		return 0.0f;
	}
	float val0 = energyFunctional();
	float tmp = X3[i];
	X3[i] = tmp + 0.01f;
	float val1 = energyFunctional();
	X3[i] = tmp;
	return val1 - val0;
}

void NN2::runNN(int vNum) {
	for (int i = 0; i < X3m; i++) {
		float dd = getDerivativeX3(i);
		if (dd != 0.0) {
			float fn = 0.001f / dd;
			if (fn < -0.1f) fn = -0.1f;
			if (fn > 0.1f) fn = 0.1f;
			X3[i] -= fn;
		}
	}
	cout << "X3:" << energyFunctional() / vNum << endl;
	for (int i = 0; i < X3m; i++) {
		for (int j = 0; j < X2m; j++) {
			float dd = getDerivativeX2(j,i);
			if (dd != 0.0) {
				float fn = 0.001f / dd;
				if (fn < -0.1f) fn = -0.1f;
				if (fn > 0.1f) fn = 0.1f;
				X2[j][i] -= fn;
			}
		}
		cout << "X2:" << i << ":" << energyFunctional() / vNum << endl;
	}
	for (int j = 0; j < X2m; j++) {
		for (int k = 0; k < X1m; k++) {
			float dd = getDerivativeX1(k, j);
			if (dd != 0.0) {
				float fn = 0.001f / dd;
				if (fn < -0.1f) fn = -0.1f;
				if (fn > 0.1f) fn = 0.1f;
				X1[k][j] -= fn;
			}
		}
		cout << "X1:" << j << ":" << energyFunctional() / vNum << endl;
	}
}


void NN2::coutX1()
{
	cout << "float X1[" << X1m << "][" << X2m << "]={ " << endl;
	for (int k = 0; k < X1m; k++) {
		cout << "{ ";
		for (int j = 0; j < X2m; j++) {
			cout << X1[k][j];
			if (j < X2m - 1) {
				cout << ",";
			}
		}
		cout << "}";
		if (k < X1m - 1) {
			cout << "," << endl;
		}
	}
	cout << "};" << endl;
}

void NN2::coutX2()
{
	cout << "float X2[" << X2m << "][" << X3m << "]={ " << endl;
	for (int j = 0; j < X2m; j++) {
		cout << "{ ";
		for (int i = 0; i < X3m; i++) {
			cout << X2[j][i];
			if (i < X3m - 1) {
				cout << ",";
			}
		}
		cout << "}";
		if (j < X2m - 1) {
			cout << "," << endl;
		}
	}
	cout << "};" << endl;
}

void NN2::coutX3()
{
	cout << "float X2[" << X2m << "]={ " << endl;
	for (int i = 0; i < X3m; i++) {
		cout << X3[i];
		if (i < X3m - 1) {
			cout << ",";
		}
	}
	cout << "};" << endl;
}

void NN2::coutX1X2X3()
{
	cout << "float X1X2X3[" << X1m << "]={ " << endl;
	for (int k = 0; k<X1m; k++) {
		float v = 0.0f;
		for (int j = 0; j < X2m; j++) {
			for (int i = 0; i < X3m; i++) {
				v += (X1[k][j] * X2[j][i] * X3[i]);
			}
		}
		cout << v;
		if (k < X1m - 1) {
			cout << ",";
		}
	}
	cout << "};" << endl;
}


void NN2::coutValues()
{
	float ret = 0.0f;
	for (ESTIMATION e : estVec) {
		float v = 0.0f;
		for (int k = 0; k < X1m; k++) {
			for (int j = 0; j < X2m; j++) {
				for (int i = 0; i < X3m; i++) {
					v += (e.est[k] * X1[k][j] * X2[j][i] * X3[i]);
				}
			}
		}
		cout << e.evaluation << "=" << v << endl;
	}
}

