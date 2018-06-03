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
	ifstream ifs(filename);// �t�@�C�����J��
	char str1[400];
	char str2[400];
	GAMESTATE* gs = (GAMESTATE *)malloc(sizeof(GAMESTATE));//ShotNum���߂̂���
	GAMESTATE* gsnext = (GAMESTATE *)malloc(sizeof(GAMESTATE));//ShotNum���ڂ̂���
	if (ifs.fail())
	{
		std::cerr << filename << " ���s" << std::endl;
		return ;
	}
	cout << filename << " ����" << endl;
	while (ifs.getline(str1, 400 - 1))//��s�ǂݍ���
	{
		if (strncmp(str1, "POSITION", 8) == 0) {//�΂̍��W��ǂݍ��񂾂�str2�֕ۊ�
			strcpy_s(str2, str1);
		}
		if (strncmp(str1, "SETSTATE", 8) == 0) {//SETSTATE�̍s��ǂ񂾂��ƊJ�n
			setGS(gs, str2, str1);// gs�ɓ��e���i�[
			if (gs->ShotNum == est.ShotNum) {//�w�肷��ShotNum�ɂ��Ă�����Ƃ��s��
				//for (int stone = 0; stone < 16; stone++) {//�f�o�b�O
				//	cout << "("<<gs->body[stone][0] << "," << gs->body[stone][1] << ")";
				//}
				//cout << endl;
				makeEstimateVector(gs, gs->ShotNum, gs->ShotNum % 2, est.est);//�]���x�N�g���̍쐬
				while (ifs.getline(str1, 400 - 1)) {
					if (strncmp(str1, "POSITION", 8) == 0) {//����position�s��T��
						strcpy_s(str2, str1);//��������str2�֊i�[
						char *tmp, *ctx;// gsnext�֓��e���i�[
						tmp = strtok_s(str1, " ", &ctx);// 
						for (int i = 0; i < 32; i++) {
							tmp = strtok_s(NULL, " ", &ctx);
							gsnext->body[i / 2][i % 2] = (float)atof(tmp);
						}
						gsnext->ShotNum = est.ShotNum+1;// ShotNum=16�������肷��ƁCSETSTATE�s���Ȃ������肷��̂ŁC���̂悤�ɂ���
						//for (int stone = 0; stone < 16; stone++) {//�f�o�b�O
						//	cout << gsnext->body[stone][0] << " " << gsnext->body[stone][1] << ":";
						//}
						//cout << endl;
						est.evaluation = - value_of_gs(gsnext);//gsnext�̔Ֆʕ]���̕�����ς������̂�gs�̌����_�ł̔Ֆʕ]���Ƃ���
						estVec.push_back(est);//�z��est�Ɋi�[
						break;
					}
				}
			}
		}
	}


}