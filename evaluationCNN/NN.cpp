#include <random>  
#include "NN.h"
#include "estVector.h"

using namespace std;

NN::NN()
{
}


NN::~NN()
{
}

void NN::setInitialRandom(){
	random_device rd;   // non-deterministic generator  
	mt19937 gen(rd());  // to seed mersenne twister.  
	for (int k = 0; k < X1m; k++) {
		for (int j = 0; j < X2m; j++) {
			int a = (gen() % 200 - 100);
			X1[k][j] = 0.001f * a;
		}
	}
	for (int j = 0; j < X2m; j++) {
		int a = (gen() % 200 - 100);
		X2[j] = 0.001f * a;
	}
}

void NN::loadDataFromFile(const char* filename){

	ifstream ifs(filename);// ファイルを開く
	char str[40];
	if (ifs.fail())	{
		std::cerr << "失敗" << std::endl;
		return;
	}
	for (int k = 0; k < X1m; k++) {
		for (int j = 0; j < X2m; j++) {
			if (ifs.getline(str, 40 - 1)) {
				X1[k][j] = (float)atof(str);
			}
			else {
				std::cerr << "失敗" << std::endl;
				return;
			}
		}
	}
	for (int j = 0; j < X2m; j++) {
		if (ifs.getline(str, 40 - 1)) {
			X2[j] = (float)atof(str);
		}
		else {
			std::cerr << "失敗" << std::endl;
			return;
		}
	}
}

void NN::saveDataToFile(const char* filename) {
	ofstream ofs(filename);// ファイルを開く
	for (int k = 0; k < X1m; k++) {
		for (int j = 0; j < X2m; j++) {
			ofs << X1[k][j] << endl;
		}
	}
	for (int j = 0; j < X2m; j++) {
		ofs << X2[j] << endl;
	}
	ofs.close();
}

float sigmoid(float x) {
	return 1 / (1 + expf(-x));
}

float NN::energyFunctional(){
	float ret = 0.0f;
	float v, w;
	for(ESTIMATION e : estVec) {
		v = 0.0f;
		for (int j = 0; j < X2m; j++) {
			w = 0.0f;
			for (int k = 0; k < X1m; k++) {
				w += (e.est[k] * X1[k][j]);
			}
			//v += (w *X2[j]);
			v += (sigmoid(w) *X2[j]);
		}
		float a = (e.evaluation - v);
		if (a < 0)a = -a;
		ret += a;
	}
	return ret;
}

float NN::getDerivativeX1(int k, int j) {
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

float NN::getDerivativeX2(int j) {
	if (j < 0 || j >= X2m) {
		return 0.0f;
	}
	float val0 = energyFunctional();
	float tmp = X2[j];
	X2[j] = tmp + 0.01f;
	float val1 = energyFunctional();
	X2[j] = tmp;
	return val1 - val0;
}

void NN::runNN(int vNum) {
	for (int j = 0; j < X2m; j++) {
		float dd = getDerivativeX2(j);
		if (dd != 0.0) {
			float fn = 0.001f / dd;
			if (fn < -0.1f) fn = -0.1f;
			if (fn > 0.1f) fn = 0.1f;
			X2[j] -= fn;
		}
	}
	cout << "X2:" << energyFunctional() / vNum << endl;
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


void NN::coutX1() 
{
	cout << "float X1["<<X1m<<"]["<<X2m<<"]={ "<<endl;
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

void NN::coutX2()
{
	cout << "float X2[" << X2m << "]={ " << endl;
	for (int j = 0; j < X2m; j++) {
		cout << X2[j];
		if (j < X2m - 1) {
			cout << ",";
		}
	}
	cout << "};" << endl;
}

void NN::coutX1X2()
{
	cout << "float X1X2[" << X1m << "]={ " << endl;
	for (int k = 0; k<X1m; k++) {
		float v = 0.0f;
		for (int j = 0; j < X2m; j++) {
			v += (X1[k][j] * X2[j]);
		}
		cout << v;
		if (k < X1m - 1) {
			cout << ",";
		}
	}
	cout << "};" << endl;
}


void NN::coutValues()
{
	float ret = 0.0f;
	float v, w;
	for (ESTIMATION e : estVec) {
		v = 0.0f;
		for (int j = 0; j < X2m; j++) {
			w = 0.0f;
			for (int k = 0; k < X1m; k++) {
				w += (e.est[k] * X1[k][j]);
			}
			//v += (w *X2[j]);
			v += (sigmoid(w) *X2[j]);
		}
		cout << e.evaluation << "=" << v << endl;
	}
}

