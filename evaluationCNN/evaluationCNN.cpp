// evaluationCNN.cpp : アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include "NN.h"
#include "NN2.h"
#include "estVector.h"
#include "evaluation.h"

using namespace std;




int main()
{
	//int SN = 15;
	//readDataFromFile("data/Ayumu - Ayumu [2018-05-30 131126].dcl", SN);
	//readDataFromFile("data/Ayumu - Ayumu [2018-05-30 133616].dcl", SN);
	//readDataFromFile("data/Ayumu - Ayumu [2018-05-30 140129].dcl", SN);
	//readDataFromFile("data/Ayumu - Ayumu [2018-05-30 142639].dcl", SN);
	//readDataFromFile("data/Ayumu - Ayumu [2018-05-30 145155].dcl", SN);
	//readDataFromFile("data/Ayumu - Ayumu [2018-05-30 151736].dcl", SN);
	//readDataFromFile("data/Ayumu - Ayumu [2018-05-30 154323].dcl", SN);
	//readDataFromFile("data/Ayumu - Ayumu [2018-05-30 160834].dcl", SN);
	//readDataFromFile("data/Ayumu - Ayumu [2018-05-30 163341].dcl", SN);
	//readDataFromFile("data/Ayumu - Ayumu [2018-05-30 165817].dcl", SN);
	//readDataFromFile("data/Ayumu - Ayumu [2018-05-30 172431].dcl", SN);
	//readDataFromFile("data/Ayumu - jiritsu-kun [2018-05-29 214417].dcl", SN);
	//readDataFromFile("data/Ayumu - jiritsu-kun [2018-05-29 215552].dcl", SN);
	//readDataFromFile("data/Ayumu - jiritsu-kun [2018-05-29 220726].dcl", SN);
	//readDataFromFile("data/Ayumu - jiritsu-kun [2018-05-29 221805].dcl", SN);
	//readDataFromFile("data/Ayumu - jiritsu-kun [2018-05-29 222859].dcl", SN);
	//readDataFromFile("data/Ayumu - jiritsu-kun [2018-05-29 224014].dcl", SN);
	//readDataFromFile("data/Ayumu - jiritsu-kun [2018-05-29 225326].dcl", SN);
	//readDataFromFile("data/Ayumu - jiritsu-kun [2018-05-29 230455].dcl", SN);
	//readDataFromFile("data/Ayumu - jiritsu-kun [2018-05-29 231604].dcl", SN);
	//readDataFromFile("data/Ayumu - jiritsu-kun [2018-05-29 232812].dcl", SN);
	//readDataFromFile("data/Ayumu - jiritsu-kun [2018-05-29 233852].dcl", SN);
	//readDataFromFile("data/Ayumu - MogMog [2018-05-31 022907].dcl", SN);
	//readDataFromFile("data/Ayumu - MogMog [2018-05-31 024809].dcl", SN);
	//readDataFromFile("data/Ayumu - MogMog [2018-05-31 030801].dcl", SN);
	//readDataFromFile("data/Ayumu - MogMog [2018-05-31 032759].dcl", SN);
	//readDataFromFile("data/Ayumu - MogMog [2018-05-31 034659].dcl", SN);
	//readDataFromFile("data/Ayumu - MogMog [2018-05-31 040507].dcl", SN);
	//readDataFromFile("data/Ayumu - MogMog [2018-05-31 042516].dcl", SN);
	//readDataFromFile("data/Ayumu - MogMog [2018-05-31 044521].dcl", SN);
	//readDataFromFile("data/Ayumu - MogMog [2018-05-31 050615].dcl", SN);
	//readDataFromFile("data/Ayumu - MogMog [2018-05-31 052606].dcl", SN);
	//readDataFromFile("data/Ayumu - MogMog [2018-05-31 054537].dcl", SN);
	//readDataFromFile("data/MogMog - jiritsu-kun [2018-05-27 213433].dcl", SN);
	//readDataFromFile("data/MogMog - jiritsu-kun [2018-05-27 214425].dcl", SN);
	//readDataFromFile("data/MogMog - jiritsu-kun [2018-05-27 215415].dcl", SN);
	//readDataFromFile("data/MogMog - jiritsu-kun [2018-05-27 220430].dcl", SN);
	//readDataFromFile("data/MogMog - jiritsu-kun [2018-05-27 225714].dcl", SN);
	//readDataFromFile("data/MogMog - jiritsu-kun [2018-05-27 231059].dcl", SN);
	//readDataFromFile("data/MogMog - jiritsu-kun [2018-05-27 232146].dcl", SN);
	//readDataFromFile("data/MogMog - jiritsu-kun [2018-05-27 233220].dcl", SN);
	//readDataFromFile("data/MogMog - jiritsu-kun [2018-05-27 234253].dcl", SN);
	//readDataFromFile("data/MogMog - jiritsu-kun [2018-05-27 235328].dcl", SN);
	//readDataFromFile("data/MogMog - jiritsu-kun [2018-05-28 000422].dcl", SN);

	//for (ESTIMATION v : estVec) {
	//	for (int y = 0; y < 10; y++) {
	//		for (int x = 0; x < 10; x++) {
	//			cout << v.est[y * 10 + x] << " ";
	//		}
	//	}
	//	cout << v.evaluation << endl;
	//}

	//int vNum = estVec.size();

	NN cnn = NN();
	//	cnn.setInitialRandom();
	cnn.loadDataFromFile("CNN15-0605sigmoid.txt");
	//cnn.saveDataToFile("CNN15.txt");

	//cout << cnn.energyFunctional() / vNum<< endl;

	//for (int repeat = 0; repeat < 15; repeat++) {
	//	cnn.runNN(vNum);
	//	cnn.saveDataToFile("CNN15-0606sigmoid.txt");
	//}
	//cnn.coutValues();
	cnn.coutX1();
	cnn.coutX2();


	return 0;
}

