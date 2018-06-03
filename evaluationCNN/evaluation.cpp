#include "evaluation.h"


using namespace std;

float getDist(float x, float y)
{
	return (x - TEE_X)*(x - TEE_X) + (y - TEE_Y)*(y - TEE_Y);
}

//! is a Stone in House
bool isInHouse(float x, float y)
{
	if (getDist(x, y) < (HOUSE_R + STONE_R)*(HOUSE_R + STONE_R)) {
		return true;
	}
	else {
		return false;
	}
}

void getRanking(int *rank, const GAMESTATE* const gs)
{
	// init array
	for (int i = 0; i < 16; i++) {
		rank[i] = i;
	}

	// sort
	int tmp;
	for (int i = 1; i < 16; i++) {
		for (int j = i; j > 0; j--) {
			if (getDist(gs->body[rank[j]][0], gs->body[rank[j]][1]) < getDist(gs->body[rank[j - 1]][0], gs->body[rank[j - 1]][1])) {
				// swap
				tmp = rank[j];
				rank[j] = rank[j - 1];
				rank[j - 1] = tmp;
			}
			else {
				break;
			}
		}
	}
	for (int i = 0; i < 16; i++) {
		if (!isInHouse(gs->body[rank[i]][0], gs->body[rank[i]][1])) {
			rank[i] = -1;
		}
	}
}

float countInHouse(const GAMESTATE* const gs, int itsme) {
	float count = 0;
	float house2 = HOUSE_R * HOUSE_R / 9;
	for (int stone = 0; stone < 16; stone++) {
		float stoneX = gs->body[stone][0];
		float stoneY = gs->body[stone][1];
		if (stoneX > 0.0 && stoneY > 0.0) {
			float dist2 = getDist(stoneX, stoneY);
			if (stone % 2 == itsme) {
				if (dist2 < house2) {
					count += 1.0;
				}
				else if (dist2 < house2 * 4) {
					count += 0.5;
				}
				else if (dist2 < house2 * 9) {
					count += 0.25;
				}
				/*else if (TEE_X - 2 * STONE_R < stoneX && stoneX < TEE_X + 2 * STONE_R
				&& TEE_Y + HOUSE_R < stoneY && stoneY < TEE_Y + 2 * HOUSE_R) {
				count += 1.0;
				}*/
			}
			else {
				if (dist2 < house2) {
					count -= 1.0;
				}
				else if (dist2 < house2 * 4) {
					count -= 0.5;
				}
				else if (dist2 < house2 * 9) {
					count -= 0.25;
				}
			}
		}

	}
	return count;
}

bool isSelfCommitted(const GAMESTATE* const gs, int stone) {
	float x0 = gs->body[stone][0];
	float y0 = gs->body[stone][1];
	for (int s = stone % 2; s < 16; s += 2) {
		if (s != stone) {
			float x1 = gs->body[s][0];
			float y1 = gs->body[s][1];
			if ((x0 - x1) * (x0 - x1) + (y0 - y1) * (y0 - y1) < HOUSE_R * HOUSE_R * 0.4356f) {// 0.4356 = 0.66^2
				if (y1 - y0 > (x1 - x0) && y1 - y0 > -(x1 - x0)) {// 1.0f = tan(pi/4)
					return true;
				}
			}
		}
	}
	return false;
}

bool isOpponentCommitted(const GAMESTATE* const gs, int stone) {
	float x0 = gs->body[stone][0];
	float y0 = gs->body[stone][1];
	for (int s = (1 - stone % 2); s < 16; s += 2) {
		if (s != stone) {
			float x1 = gs->body[s][0];
			float y1 = gs->body[s][1];
			if ((x0 - x1) * (x0 - x1) + (y0 - y1) * (y0 - y1) < HOUSE_R * HOUSE_R * 0.4356f) {// 0.4356 = 0.66^2
				if (y1 - y0 > 1.0f * (x1 - x0) && y1 - y0 > -1.0f * (x1 - x0)) {// 1.0 = tan(pi/4)
					return true;
				}
			}
		}
	}
	return false;
}

bool isGuarded(const GAMESTATE* const gs, int stone) {
	float x0 = gs->body[stone][0];
	float y0 = gs->body[stone][1];
	for (int s = 0; s < 16; s++) {
		if (s != stone) {
			float x1 = gs->body[s][0];
			float y1 = gs->body[s][1];
			if (y0 + 0.75f * HOUSE_R < y1 && y1 < y0 + 1.5f * HOUSE_R) {
				if (x0 - 2 * STONE_R < x1 && x1 < x0 + 2 * STONE_R) {
					return true;
				}
			}
		}
	}
	return false;
}


float makeEstimate(const GAMESTATE* const gstmp, int shotNum, int itsme, bool lastFlag, int oldRank[16]) {
	float estimate = 0;
	int rank[16];
	getRanking(rank, gstmp);
	bool last = lastFlag || (shotNum >= 12);
	bool lastlast = (shotNum >= 14);
	if (!last) {// 通常時
				// ティーからの距離で石の配置を評価する。
		estimate += (COUNT_IN_HOUSE * countInHouse(gstmp, itsme));
		//if (wasHisDoubleTakeOutShot(oldRank, rank, itsme)) {//double takeout shot成功させられたとき
		//	estimate -= HIS_DOUBLE_TAKE;
		//}
		//if (wasMyDoubleTakeOutShot(oldRank, rank, itsme)) {//double takeout shot成功したとき
		//	estimate += MY_DOUBLE_TAKE;
		//}
		if (rank[0] >= 0 && rank[0] % 2 == itsme) {// No1が自分の石
			estimate += NO1_STONE;
			if (rank[1] >= 0 && rank[1] % 2 == itsme) {// No2が自分の石
				estimate += NO2_STONE;
				if (rank[2] >= 0 && rank[2] % 2 == itsme) {// No3が自分の石
					estimate += NO3_STONE;
					if (rank[3] >= 0 && rank[3] % 2 == itsme) {// No4が自分の石
						estimate += NO3_STONE;
						if (rank[4] >= 0 && rank[4] % 2 == itsme) {// No5が自分の石
							estimate += NO3_STONE;
						}
					}
				}
			}
			else if (rank[1] >= 0 && rank[1] % 2 != itsme) {// no2が敵
				estimate -= NO2_STONE;
				if (rank[2] >= 0 && rank[2] % 2 != itsme) {// no3が敵
					estimate -= NO3_STONE;
					if (rank[3] >= 0 && rank[3] % 2 != itsme) {// No4が敵の石
						estimate -= NO3_STONE;
						if (rank[4] >= 0 && rank[4] % 2 != itsme) {// No5が敵の石
							estimate -= NO3_STONE;
						}
					}
				}
			}
			//  自分の石がNo1で，それにガードがついている時には評価する。
			if (isGuarded(gstmp, rank[0])) {
				estimate += GUARDED;
			}
			//   自分の石がNo1で，それに味方の石がついている時にはマイナス評価する。
			if (isSelfCommitted(gstmp, rank[0])) {
				estimate -= SELF_COMMITTED;
			}
			//   自分の石がNo1で，それに敵の石がついている時にはマイナス評価する。
			else if (isOpponentCommitted(gstmp, rank[0])) {
				estimate -= OPPONENT_COMMITTED;
			}
		}
		else if (rank[0] >= 0 && rank[0] % 2 != itsme % 2) {//相手がNO1
			estimate -= NO1_STONE;
			if (rank[1] >= 0 && rank[1] % 2 == itsme) {// 自分がno2
				estimate += NO2_STONE;
				if (rank[2] >= 0 && rank[2] % 2 == itsme) {// 自分がno3
					estimate += NO3_STONE;
					if (rank[3] >= 0 && rank[3] % 2 == itsme) {// No4が自分の石
						estimate += NO3_STONE;
						if (rank[4] >= 0 && rank[4] % 2 == itsme) {// No5が自分の石
							estimate += NO3_STONE;
						}
					}
				}
			}
			else if (rank[1] >= 0 && rank[1] % 2 != itsme) {// no2 が敵
				estimate -= NO2_STONE;
				if (rank[2] >= 0 && rank[2] % 2 != itsme) {//敵がno3
					estimate -= NO3_STONE;
					if (rank[3] >= 0 && rank[3] % 2 != itsme) {// No4が敵の石
						estimate -= NO3_STONE;
						if (rank[4] >= 0 && rank[4] % 2 != itsme) {// No5が敵の石
							estimate -= NO3_STONE;
						}
					}
				}
			}
			//  rank[0]が相手の石で，それにガードがついている時にはマイナス評価する。
			if (isGuarded(gstmp, rank[0])) {
				estimate -= GUARDED;
			}
			//   rank[0]が相手の石で，それに敵の石がついている時にはプラス評価する。
			if (isSelfCommitted(gstmp, rank[0])) {
				estimate += SELF_COMMITTED;
			}
			//   rank[0]が相手の石で，それに味方の石がついている時にはプラス評価する。
			else if (isOpponentCommitted(gstmp, rank[0])) {
				estimate += OPPONENT_COMMITTED;
			}
		}
	}
	else { // if(last)　終了時
		//if (wasHisDoubleTakeOutShot(oldRank, rank, itsme)) {//double takeout shot成功させられたとき
		//	estimate -= HIS_DOUBLE_TAKE;
		//}
		//if (wasMyDoubleTakeOutShot(oldRank, rank, itsme)) {//double takeout shot成功したとき
		//	estimate += MY_DOUBLE_TAKE;
		//}
		if (rank[0] >= 0 && rank[0] % 2 == itsme) {// 自分の石がNo1
			estimate += LAST_NO1_STONE;
			if (rank[1] >= 0 && rank[1] % 2 == itsme) {// 自分の石がno2
				estimate += LAST_NO2_STONE;
				if (rank[2] >= 0 && rank[2] % 2 == itsme) {// 自分の石がno3
					estimate += LAST_NO3_STONE;
					if (rank[3] >= 0 && rank[3] % 2 == itsme) {// No4が自分の石
						estimate += NO3_STONE;
						if (rank[4] >= 0 && rank[4] % 2 == itsme) {// No5が自分の石
							estimate += NO3_STONE;
						}
					}
				}
			}
			else if (rank[1] >= 0 && rank[1] % 2 != itsme) {// no2 が敵
				estimate -= LAST_NO2_STONE;
				if (rank[2] >= 0 && rank[2] % 2 != itsme) {// no3 が敵
					estimate -= LAST_NO3_STONE;
					if (rank[3] >= 0 && rank[3] % 2 != itsme) {// No4が敵の石
						estimate -= NO3_STONE;
						if (rank[4] >= 0 && rank[4] % 2 != itsme) {// No5が敵の石
							estimate -= NO3_STONE;
						}
					}
				}
			}
		}
		else if (rank[0] >= 0 && rank[0] % 2 != itsme) {//相手がNO1
			estimate -= LAST_NO1_STONE;
			if (rank[1] >= 0 && rank[1] % 2 == itsme) {// 自分がno2
				if (!lastlast) estimate += LAST_NO2_STONE;
				if (rank[2] >= 0 && rank[2] % 2 == itsme) {// かつ自分
					if (!lastlast) estimate += LAST_NO3_STONE;
					if (rank[3] >= 0 && rank[3] % 2 == itsme) {// No4が自分の石
						if (!lastlast) estimate += NO3_STONE;
						if (rank[4] >= 0 && rank[4] % 2 == itsme) {// No5が自分の石
							if (!lastlast) estimate += NO3_STONE;
						}
					}
				}
			}
			else if (rank[1] >= 0 && rank[1] % 2 != itsme) {// no2 が敵
				estimate -= LAST_NO2_STONE;
				if (rank[2] >= 0 && rank[2] % 2 != itsme) {// no3が敵
					estimate -= LAST_NO3_STONE;
					if (rank[3] >= 0 && rank[3] % 2 != itsme) {// No4が敵の石
						estimate -= NO3_STONE;
						if (rank[4] >= 0 && rank[4] % 2 != itsme) {// No5が敵の石
							estimate -= NO3_STONE;
						}
					}
				}
			}
		}
	}
	for (int r = 0; r < 16; r++) {
		oldRank[r] = rank[r];
	}
	return estimate;
}


void setGS(GAMESTATE* gs, char* str1, char* str2) {
	char * tmp, *ctx;
	tmp = strtok_s(str1, " ", &ctx);// tmp = "POSITION=POSITION"
	for (int i = 0; i < 32; i++) {
		tmp = strtok_s(NULL, " ", &ctx);
		gs->body[i / 2][i % 2] = (float)atof(tmp);
	}
	tmp = strtok_s(str2, " ", &ctx);// tmp = "SETSTATE=SETSTATE "
	for (int i = 0; i < 4; i++) {
		//	string s1[] = std::split()
		tmp = strtok_s(NULL, " ", &ctx);
		switch (i) {
		case 0:
			gs->ShotNum = (int)atoi(tmp);
			break;
		case 1:
			gs->CurEnd = (int)atoi(tmp);
			break;
		case 2:
			gs->LastEnd = (int)atoi(tmp);
			break;
		case 3:
			gs->WhiteToMove = (int)atoi(tmp);
		}
	}
}


void makeEstimateVector(const GAMESTATE* const gs, int shotNum, int itsme, float est[100]) {
	// length of est is 50
	//0 - 23 stone position
	for (int i = 0; i < 100; i++) {
		est[i] = 0.0f;
	}
	float count = 0;
	float house2 = HOUSE_R * HOUSE_R / 9;
	for (int stone = 0; stone < 16; stone++) {
		float stoneX = gs->body[stone][0];
		float stoneY = gs->body[stone][1];
		if (stoneX > 0.0 && stoneY > 0.0) {
			stoneX -= TEE_X;
			stoneY -= TEE_Y;
			float dist2 = stoneX * stoneX + stoneY * stoneY;
			if (stone % 2 == itsme) {//味方の石
				if (dist2 < house2) {
					est[0] += 1.0f;
				}
				else if (dist2 < house2 * 4) {
					if (stoneY >= stoneX) {
						if (stoneY >= -stoneX) {
							est[1] += 1.0f;
						}
						else {
							est[5] += 1.0f;
						}
					}
					if (stoneY < stoneX) {
						if (stoneY >= -stoneX) {
							est[7] += 1.0f;
						}
						else {
							est[3] += 1.0f;
						}
					}
				}
				else if (dist2 < house2 * 9) {
					if (stoneY >= stoneX) {
						if (stoneY >= -stoneX) {
							est[2] += 1.0f;
						}
						else {
							est[6] += 1.0f;
						}
					}
					else {
						if (stoneY >= -stoneX) {
							est[8] += 1.0f;
						}
						else {
							est[4] += 1.0f;
						}
					}
				}
				else if (HOUSE_R < stoneY && stoneY < 2 * HOUSE_R) {
					if (-2*STONE_R <= stoneX && stoneX <= 2*STONE_R) {
						est[18] += 1.0f;//18：正面ガードポジションにある味方の石の個数
					}
					else if (-6 * STONE_R <= stoneX && stoneX <= -2 * STONE_R) {
						est[20] += 1.0f;//20：左ガードポジションにある味方の石の個数
					}
					else if (2 * STONE_R <= stoneX && stoneX <= 6 * STONE_R) {
						est[22] += 1.0f;//22：右ガードポジションにある味方の石の個数
					}
				}
			}
			else {//敵の石
				if (dist2 < house2) {
					est[9] += 1.0f;
				}
				else if (dist2 < house2 * 4) {
					if (stoneY >= stoneX) {
						if (stoneY >= -stoneX) {
							est[10] += 1.0f;
						}
						else {
							est[14] += 1.0f;
						}
					}
					else {
						if (stoneY >= -stoneX) {
							est[16] += 1.0f;
						}
						else {
							est[12] += 1.0f;
						}
					}
				}
				else if (dist2 < house2 * 9) {
					if (stoneY >= stoneX) {
						if (stoneY >= -stoneX) {
							est[11] += 1.0f;
						}
						else {
							est[15] += 1.0f;
						}
					}
					else {
						if (stoneY >= -stoneX) {
							est[17] += 1.0f;
						}
						else {
							est[13] += 1.0f;
						}
					}
				}
				else if (HOUSE_R < stoneY && stoneY < 2 * HOUSE_R) {
					if (-2 * STONE_R <= stoneX && stoneX <= 2 * STONE_R) {
						est[19] += 1.0f;//19：正面ガードポジションにある敵の石の個数
					}
					else if (-6 * STONE_R <= stoneX && stoneX <= -2 * STONE_R) {
						est[21] += 1.0f;//21：左ガードポジションにある敵の石の個数
					}
					else if (2 * STONE_R <= stoneX && stoneX <= 6 * STONE_R) {
						est[23] += 1.0f;//23：右ガードポジションにある敵の石の個数
					}
				}

			}
		}
	}
	//24 - 41
	int rank[16];
	getRanking(rank, gs);
	if (rank[0] != -1 && rank[0] % 2 == itsme) {	//	24：no1 が自分であるかどうか
		est[24] = 1.0f;
		if (rank[1] != -1 && rank[1] % 2 == itsme) {	//	26：no1 が自分で、no2が自分であるかどうか
			est[26] = 1.0f;
			if (rank[2] != -1 && rank[2] % 2 == itsme) {	//	27：no1 が自分で、no2, no3が自分であるかどうか
				est[27] = 1.0f;
				if (rank[3] != -1 && rank[3] % 2 == itsme) {//	28：no1 が自分で、no2, no3, no4が自分であるかどうか
					est[28] = 1.0f;
					if (rank[4] != -1 && rank[4] % 2 == itsme) {//	29：no1 が自分で、no2, -no5がすべて自分であるかどうか
						est[29] = 1.0f;
					}
				}
			}
		}
		if (rank[1] != -1 && rank[1] % 2 != itsme) {//	30：no1 が自分で、no2が敵であるかどうか
			est[30] = 1.0f;
			if (rank[2] != -1 && rank[2] % 2 != itsme) {//	31：no1 が自分で、no2, no3が敵であるかどうか
				est[31] = 1.0f;
				if (rank[3] != -1 && rank[3] % 2 != itsme) {//	32：no1 が自分で、no2, no3, no4が敵であるかどうか
					est[32] = 1.0f;
					if (rank[4] != -1 && rank[4] % 2 != itsme) {//	33：no1 が自分で、no2, -no5がすべて敵であるかどうか
						est[33] = 1.0f;
					}
				}
			}
		}
	}
	if (rank[0] != -1 && rank[0] % 2 != itsme) {//	25：no1が敵であるかどうか
		est[25] = 1.0f;
		if (rank[1] != -1 && rank[1] % 2 == itsme) {//	34：no1 が敵で、no2が自分であるかどうか
			est[34] = 1.0f;
			if (rank[2] != -1 && rank[2] % 2 == itsme) {//	35：no1 が敵で、no2, no3が自分であるかどうか
				est[35] = 1.0f;
				if (rank[3] != -1 && rank[3] % 2 == itsme) {//	36：no1 が敵で、no2, no3, no4が自分であるかどうか
					est[36] = 1.0f;
					if (rank[4] != -1 && rank[4] % 2 == itsme) {//	37：no1 が敵で、no2, -no5がすべて自分であるかどうか
						est[37] = 1.0f;
					}
				}
			}
		}
		if (rank[1] != -1 && rank[1] % 2 != itsme) {//	38：no1 が敵で、no2が敵であるかどうか
			est[38] = 1.0f;
			if (rank[2] != -1 && rank[2] % 2 != itsme) {//	39：no1 が敵で、no2, no3が敵であるかどうか
				est[39] = 1.0f;
				if (rank[3] != -1 && rank[3] % 2 != itsme) {//	40：no1 が敵で、no2, no3, no4が敵であるかどうか
					est[40] = 1.0f;
					if (rank[4] != -1 && rank[4] % 2 != itsme) {//	41：no1 が敵で、no2, -no5がすべて敵であるかどうか
						est[41] = 1.0f;
					}
				}
			}
		}
	}
	//42 - 61：no1にコミットする他石について
	if (rank[0] != -1 && rank[0] % 2 == itsme) {
		float x0 = gs->body[rank[0]][0];
		float y0 = gs->body[rank[0]][1];
		float house2 = HOUSE_R * HOUSE_R / 9.0f;
		for (int s = 0; s < 16; s++) {
			if (s != rank[0]) {
				float x1 = gs->body[s][0];
				float y1 = gs->body[s][1];
				if (x1 > 0.0f && y1 > 0.0f) {
					x1 -= x0;
					y1 -= y0;
					float dist2 = x1 * x1 + y1 * y1;
					if (dist2 < house2 && 1.732f * y1 > x1 && 1.732f * y1 > -x1) {
						if (s % 2 == itsme) {//	42：no1 が自分で、距離1 / 3以内でコミットする自分の石がある。（角度60度）
							est[42] = 1.0f;
						}
						else if (s % 2 != itsme) {//	47：no1 が自分で、距離1 / 3以内でコミットする敵の石がある
							est[47] = 1.0f;
						}
					}
					else if (house2 <= dist2 && dist2 < house2 * 4 && y1 > x1 && y1 > -x1) {
						if (s % 2 == itsme) {//	43：no1 が自分で、距離1 / 3 - 2 / 3でコミットする自分の石がある。（角度45度）
							est[43] = 1.0f;
						}
						else if (s % 2 != itsme) {//	48：no1 が自分で、距離1 / 3 - 2 / 3でコミットする敵の石がある。
							est[48] = 1.0f;
						}
					}
					else if (house2 * 4 <= dist2 && dist2 < house2 * 9 && y1 > 1.732f * x1 && y1 > -1.732f * x1) {
						if (s % 2 == itsme) { //	44：no1 が自分で、距離2 / 3 - 3 / 3でコミットする自分の石がある。（角度30度）
							est[44] = 1.0f;
						}
						else if (s % 2 != itsme) {//	49：no1 が自分で、距離2 / 3 - 3 / 3でコミットする敵の石がある。
							est[49] = 1.0f;
						}
					}
					else if (house2 * 9 <= dist2 && dist2 < house2 * 25 && -2 * STONE_R < x1 && x1 < 2 * STONE_R) {
						if (s % 2 == itsme) {//	45：no1 が自分で、距離3 / 3 - 5 / 3でコミットする自分の石がある。（石の差渡し＊２の範囲）
							est[45] = 1.0f;
						}
						else if (s % 2 != itsme) {//	50：no1 が自分で、距離3 / 3 - 5 / 3でコミットする敵の石がある。
							est[50] = 1.0f;
						}
					}
					else if (house2 * 25 <= dist2 && -2 * STONE_R < x1 && x1 < 2 * STONE_R) {
						if (s % 2 == itsme) {//	46：no1 が自分で、距離5 / 3以上でコミットする自分の石がある。
							est[46] = 1.0f;
						}
						else if (s % 2 != itsme) {//	51：no1 が自分で、距離5 / 3以上でコミットする敵の石がある。
							est[51] = 1.0f;
						}
					}
				}
			}
		}
	}
	if (rank[0] != -1 && rank[0] % 2 != itsme) {
		float x0 = gs->body[rank[0]][0];
		float y0 = gs->body[rank[0]][1];
		float house2 = HOUSE_R * HOUSE_R / 9.0f;
		for (int s = 0; s < 16; s++) {
			if (s != rank[0]) {
				float x1 = gs->body[s][0];
				float y1 = gs->body[s][1];
				if (x1 > 0.0f && y1 > 0.0f) {
					x1 -= x0;
					y1 -= y0;
					float dist2 = x1 * x1 + y1 * y1;
					if (dist2 < house2 && 1.732f * y1 > x1 && 1.732f * y1 > -x1) {
						if (s % 2 == itsme) {//	52：no1 が敵で、距離1 / 3以内でコミットする自分の石がある。
							est[52] = 1.0f;
						}
						else if (s % 2 != itsme) {//	57：no1 が敵で、距離1 / 3以内でコミットする敵の石がある。
							est[57] = 1.0f;
						}
					}
					else if (house2 <= dist2 && dist2 < house2 * 4 && y1 > x1 && y1 > -x1) {
						if (s % 2 == itsme) {//	53：no1 が敵で、距離1 / 3 - 2 / 3でコミットする自分の石がある。
							est[53] = 1.0f;
						}
						else if (s % 2 != itsme) {//	58：no1 が敵で、距離1 / 3 - 2 / 3でコミットする敵の石がある。
							est[58] = 1.0f;
						}
					}
					else if (house2 * 4 <= dist2 && dist2 < house2 * 9 && y1 > 1.732f * x1 && y1 > -1.732f * x1) {
						if (s % 2 == itsme) { //	54：no1 が敵で、距離2 / 3 - 3 / 3でコミットする自分の石がある。
							est[54] = 1.0f;
						}
						else if (s % 2 != itsme) {//	59：no1 が敵で、距離2 / 3 - 3 / 3でコミットする敵の石がある。
							est[59] = 1.0f;
						}
					}
					else if (house2 * 9 <= dist2 && dist2 < house2 * 25 && -2 * STONE_R < x1 && x1 < 2 * STONE_R) {
						if (s % 2 == itsme) {//	55：no1 が敵で、距離3 / 3 - 5 / 3でコミットする自分の石がある。
							est[55] = 1.0f;
						}
						else if (s % 2 != itsme) {//	60：no1 が敵で、距離3 / 3 - 5 / 3でコミットする敵の石がある。
							est[60] = 1.0f;
						}
					}
					else if (house2 * 25 <= dist2 && -2 * STONE_R < x1 && x1 < 2 * STONE_R) {
						if (s % 2 == itsme) {//	56：no1 が敵で、距離5 / 3以上でコミットする自分の石がある。 / 
							est[56] = 1.0f;
						}
						else if (s % 2 != itsme) {//	61：no1 が敵で、距離5 / 3以上でコミットする敵の石がある。
							est[61] = 1.0f;
						}
					}
				}
			}
		}
	}
	if (rank[1] != -1 && rank[1] % 2 == itsme) {
		float x0 = gs->body[rank[1]][0];
		float y0 = gs->body[rank[1]][1];
		float house2 = HOUSE_R * HOUSE_R / 9.0f;
		for (int s = 0; s < 16; s++) {
			if (s != rank[1]) {
				float x1 = gs->body[s][0];
				float y1 = gs->body[s][1];
				if (x1 > 0.0f && y1 > 0.0f) {
					x1 -= x0;
					y1 -= y0;
					float dist2 = x1 * x1 + y1 * y1;
					if (dist2 < house2 && 1.732f * y1 > x1 && 1.732f * y1 > -x1) {
						if (s % 2 == itsme) {//	42：no2 が自分で、距離1 / 3以内でコミットする自分の石がある。（角度60度）
							est[62] = 1.0f;
						}
						else if (s % 2 != itsme) {//	47：no2 が自分で、距離1 / 3以内でコミットする敵の石がある
							est[67] = 1.0f;
						}
					}
					else if (house2 <= dist2 && dist2 < house2 * 4 && y1 > x1 && y1 > -x1) {
						if (s % 2 == itsme) {//	43：no2 が自分で、距離1 / 3 - 2 / 3でコミットする自分の石がある。（角度45度）
							est[63] = 1.0f;
						}
						else if (s % 2 != itsme) {//	48：no2 が自分で、距離1 / 3 - 2 / 3でコミットする敵の石がある。
							est[68] = 1.0f;
						}
					}
					else if (house2 * 4 <= dist2 && dist2 < house2 * 9 && y1 > 1.732f * x1 && y1 > -1.732f * x1) {
						if (s % 2 == itsme) { //	44：no2 が自分で、距離2 / 3 - 3 / 3でコミットする自分の石がある。（角度30度）
							est[64] = 1.0f;
						}
						else if (s % 2 != itsme) {//	49：no2 が自分で、距離2 / 3 - 3 / 3でコミットする敵の石がある。
							est[69] = 1.0f;
						}
					}
					else if (house2 * 9 <= dist2 && dist2 < house2 * 25 && -2 * STONE_R < x1 && x1 < 2 * STONE_R) {
						if (s % 2 == itsme) {//	45：no2 が自分で、距離3 / 3 - 5 / 3でコミットする自分の石がある。（石の差渡し＊２の範囲）
							est[65] = 1.0f;
						}
						else if (s % 2 != itsme) {//	50：no2 が自分で、距離3 / 3 - 5 / 3でコミットする敵の石がある。
							est[70] = 1.0f;
						}
					}
					else if (house2 * 25 <= dist2 && -2 * STONE_R < x1 && x1 < 2 * STONE_R) {
						if (s % 2 == itsme) {//	46：no2 が自分で、距離5 / 3以上でコミットする自分の石がある。
							est[66] = 1.0f;
						}
						else if (s % 2 != itsme) {//	51：no2 が自分で、距離5 / 3以上でコミットする敵の石がある。
							est[71] = 1.0f;
						}
					}
				}
			}
		}
	}
	if (rank[1] != -1 && rank[1] % 2 != itsme) {
		float x0 = gs->body[rank[1]][0];
		float y0 = gs->body[rank[1]][1];
		float house2 = HOUSE_R * HOUSE_R / 9.0f;
		for (int s = 0; s < 16; s++) {
			if (s != rank[1]) {
				float x1 = gs->body[s][0];
				float y1 = gs->body[s][1];
				if (x1 > 0.0f && y1 > 0.0f) {
					x1 -= x0;
					y1 -= y0;
					float dist2 = x1 * x1 + y1 * y1;
					if (dist2 < house2 && 1.732f * y1 > x1 && 1.732f * y1 > -x1) {
						if (s % 2 == itsme) {//	52：no2 が敵で、距離1 / 3以内でコミットする自分の石がある。
							est[72] = 1.0f;
						}
						else if (s % 2 != itsme) {//	57：no2 が敵で、距離1 / 3以内でコミットする敵の石がある。
							est[77] = 1.0f;
						}
					}
					else if (house2 <= dist2 && dist2 < house2 * 4 && y1 > x1 && y1 > -x1) {
						if (s % 2 == itsme) {//	53：no2 が敵で、距離1 / 3 - 2 / 3でコミットする自分の石がある。
							est[73] = 1.0f;
						}
						else if (s % 2 != itsme) {//	58：no2 が敵で、距離1 / 3 - 2 / 3でコミットする敵の石がある。
							est[78] = 1.0f;
						}
					}
					else if (house2 * 4 <= dist2 && dist2 < house2 * 9 && y1 > 1.732f * x1 && y1 > -1.732f * x1) {
						if (s % 2 == itsme) { //	54：no2 が敵で、距離2 / 3 - 3 / 3でコミットする自分の石がある。
							est[74] = 1.0f;
						}
						else if (s % 2 != itsme) {//	59：no2 が敵で、距離2 / 3 - 3 / 3でコミットする敵の石がある。
							est[79] = 1.0f;
						}
					}
					else if (house2 * 9 <= dist2 && dist2 < house2 * 25 && -2 * STONE_R < x1 && x1 < 2 * STONE_R) {
						if (s % 2 == itsme) {//	55：no2 が敵で、距離3 / 3 - 5 / 3でコミットする自分の石がある。
							est[75] = 1.0f;
						}
						else if (s % 2 != itsme) {//	60：no2 が敵で、距離3 / 3 - 5 / 3でコミットする敵の石がある。
							est[80] = 1.0f;
						}
					}
					else if (house2 * 25 <= dist2 && -2 * STONE_R < x1 && x1 < 2 * STONE_R) {
						if (s % 2 == itsme) {//	56：no2 が敵で、距離5 / 3以上でコミットする自分の石がある。 / 
							est[76] = 1.0f;
						}
						else if (s % 2 != itsme) {//	61：no2 が敵で、距離5 / 3以上でコミットする敵の石がある。
							est[81] = 1.0f;
						}
					}
				}
			}
		}
	}
}

