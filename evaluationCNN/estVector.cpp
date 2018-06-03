//estVector.cpp

#include "estVector.h"
#include "evaluation.h"

using namespace std;

_estVector::_estVector() {}

vector<ESTIMATION> estVec;

float value_of_gs(GAMESTATE* gs) {
	float ret = 0.0;
	if (gs->ShotNum == 16) {
		int rank[16];
		getRanking(rank,gs);
		if(rank[0]%2==0){
			int i=0;
			while (rank[i++] % 2 == 0) {
				ret += 1.0f;
			}
		}
		else {
			int i = 0;
			while (rank[i++] % 2 == 1) {
				ret -= 1.0f;
			}
		}
	}
	return ret;
}

void readDataFromFile(const char* filename, int SN) {
	ESTIMATION est = ESTIMATION();
	est.ShotNum = SN;
	ifstream ifs(filename);// ファイルを開く
	char str1[400];
	char str2[400];
	GAMESTATE* gs = (GAMESTATE *)malloc(sizeof(GAMESTATE));//ShotNum投めのｇｓ
	GAMESTATE* gsnext = (GAMESTATE *)malloc(sizeof(GAMESTATE));//ShotNum投目のｇｓ
	if (ifs.fail())
	{
		std::cerr << filename << " 失敗" << std::endl;
		return ;
	}
	cout << filename << " 成功" << endl;
	while (ifs.getline(str1, 400 - 1))//一行読み込む
	{
		if (strncmp(str1, "POSITION", 8) == 0) {//石の座標を読み込んだらstr2へ保管
			strcpy_s(str2, str1);
		}
		if (strncmp(str1, "SETSTATE", 8) == 0) {//SETSTATEの行を読んだら作業開始
			setGS(gs, str2, str1);// gsに内容を格納
			if (gs->ShotNum == est.ShotNum) {//指定するShotNumについてだけ作業を行う
				//for (int stone = 0; stone < 16; stone++) {//デバッグ
				//	cout << "("<<gs->body[stone][0] << "," << gs->body[stone][1] << ")";
				//}
				//cout << endl;
				makeEstimateVector(gs, gs->ShotNum, gs->ShotNum % 2, est.est);//評価ベクトルの作成
				while (ifs.getline(str1, 400 - 1)) {
					if (strncmp(str1, "POSITION", 8) == 0) {//次のposition行を探す
						strcpy_s(str2, str1);//見つけたらstr2へ格納
						char *tmp, *ctx;// gsnextへ内容を格納
						tmp = strtok_s(str1, " ", &ctx);// 
						for (int i = 0; i < 32; i++) {
							tmp = strtok_s(NULL, " ", &ctx);
							gsnext->body[i / 2][i % 2] = (float)atof(tmp);
						}
						gsnext->ShotNum = est.ShotNum+1;// ShotNum=16だったりすると，SETSTATE行がなかったりするので，このようにした
						//for (int stone = 0; stone < 16; stone++) {//デバッグ
						//	cout << gsnext->body[stone][0] << " " << gsnext->body[stone][1] << ":";
						//}
						//cout << endl;
						est.evaluation = - value_of_gs(gsnext);//gsnextの盤面評価の符号を変えたものをgsの現時点での盤面評価とする
						estVec.push_back(est);//配列estに格納
						break;
					}
				}
			}
		}
	}


}