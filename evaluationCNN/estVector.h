#pragma once

//estVector.h

#include <fstream>
#include <iostream>
#include <string>
#include <vector>


typedef class _estVector {
public:
	int ShotNum;
	float est[100];
	float evaluation;

public:
	_estVector();

} ESTIMATION;

void readDataFromFile(const char* filename,int SN);

extern std::vector <ESTIMATION> estVec;



