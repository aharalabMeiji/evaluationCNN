#pragma once

//evaluation.h

#include <string>
#include <iostream>




//! state of the game
typedef struct _GAMESTATE {
	int		ShotNum;		// number of Shot
							// if ShotNum = n, next Shot is (n+1)th shot in this End

	int		CurEnd;			// (number of current end) - 1
	int		LastEnd;		// number of final End
	int		Score[10];		// score of each End (if Score < 0: First player in 1st End scored)
	bool	WhiteToMove;	// Which player will shot next
							// if WhiteToMove = 0: First player in 1st End will shot next, 
							//  else (WhiteToMove = 1) : Second player will shot next

	float	body[16][2];	// body[n][0] : x of cordinate of n th stone
							// body[n][1] : y of cordinate of n th stone

} GAMESTATE, *PGAMESTATE;

// Shot
//! position on a Sheet of Curling
typedef struct _ShotPos {
	float x;
	float y;
	bool angle;

} SHOTPOS, PSHOTPOS;

//! vector of shot
typedef struct _Shotvec {
	float x;
	float y;
	bool angle;

} SHOTVEC, PSHOTVEC;


#define NEXT_EST 1.5f
#define NNEXT_EST 2.25f
#define NNNEXT_EST 3.375f
#define NNNNEXT_EST 5.0625f

#define COUNT_IN_HOUSE 1.0f

#define HIS_DOUBLE_TAKE 1.0f
#define MY_DOUBLE_TAKE 1.0f

#define NO1_STONE 1.0f
#define NO2_STONE 0.9f
#define NO3_STONE 0.81f

#define SHOT_LEFT false
#define SHOT_RIGHT true
#define CENTER_GUARD 0
#define TIGHT_GUARD 1
#define LEFT_GUARD 2
#define RIGHT_GUARD 3
#define ANGLE_ZERO 0
#define ANGLE_LEFT 1
#define ANGLE_RIGHT 2
#define WEAK_ANGLE_ZERO 3
#define WEAK_ANGLE_LEFT 4
#define WEAK_ANGLE_RIGHT 5

#define GUARDED 4.0f
#define SELF_COMMITTED 4.0f
#define OPPONENT_COMMITTED 4.0f

#define LAST_NO1_STONE 2.0f
#define LAST_NO2_STONE 1.8f
#define LAST_NO3_STONE 1.62f

#define DEFENSIVE_WEIGHT 0.0f
#define OFFENSIVE_WEIGHT 0.0f

#define MY_RANDOM 0.145f
#define OPP_RANDOM 0.0f

static const float TEE_X = (float)2.375;    // x of center of house
static const float TEE_Y = (float)4.880;    // y of center of house
static const float HOUSE_R = (float)1.830;  // radius of house
static const float STONE_R = (float)0.145;  // radius of stone

void getRanking(int *rank, const GAMESTATE* const gs);
void setGS(GAMESTATE* gs, char* str1, char* str2);
void makeEstimateVector(const GAMESTATE* const gs, int shotNum, int itsme, float est[100]);
