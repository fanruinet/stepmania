#include "global.h"
#include "GameConstantsAndTypes.h"
#include "GameState.h"
#include "RageUtil.h"
#include "ThemeMetric.h"
#include "EnumHelper.h"
#include "Foreach.h"
#include "LuaManager.h"
#include "GameManager.h"
#include "LocalizedString.h"
#include "PlayerNumber.h"
#include <float.h>


static vector<RString> GenerateRankingToFillInMarker()
{
	vector<RString> vRankings;
	FOREACH_ENUM( PlayerNumber, pn )
		vRankings.push_back( ssprintf("#P%d#", pn+1) );
	return vRankings;
}
extern const vector<RString> RANKING_TO_FILL_IN_MARKER( GenerateRankingToFillInMarker() );

extern const RString GROUP_ALL = "---Group All---";

static const char *RadarCategoryNames[] = {
	"Stream",
	"Voltage",
	"Air",
	"Freeze",
	"Chaos",
	"TapsAndHolds",
	"Jumps",
	"Holds",
	"Mines",
	"Hands",
	"Rolls",
	"MinMidiNote",
	"MaxMidiNote",
};
XToString( RadarCategory );
XToLocalizedString( RadarCategory );
LuaFunction( RadarCategoryToLocalizedString, RadarCategoryToLocalizedString(Enum::Check<RadarCategory>(L, 1)) );
LuaXType( RadarCategory );

RString StepsTypeToString( StepsType st )
{
	RString s = GameManager::GetStepsTypeInfo( st ).szName; // "dance-single"
	return s;
}
namespace StringConversion { template<> RString ToString<StepsType>( const StepsType &value ) { return StepsTypeToString(value); } }

LuaXType( StepsType );


static const char *PlayModeNames[] = {
	"Regular",
	"Nonstop",
	"Oni",
	"Endless",
	"Battle",
	"Rave",
};
XToString( PlayMode );
XToLocalizedString( PlayMode );
StringToX( PlayMode );
LuaFunction( PlayModeToLocalizedString, PlayModeToLocalizedString(Enum::Check<PlayMode>(L, 1)) );
LuaXType( PlayMode );

RankingCategory AverageMeterToRankingCategory( int iAverageMeter )
{
	if(      iAverageMeter <= 3 )	return RANKING_A;
	else if( iAverageMeter <= 6 )	return RANKING_B;
	else if( iAverageMeter <= 9 )	return RANKING_C;
	else							return RANKING_D;
}


static const char *RankingCategoryNames[] = {
	"a",
	"b",
	"c",
	"d",
};
XToString( RankingCategory );
StringToX( RankingCategory );


static const char *PlayerControllerNames[] = {
	"Human",
	"Autoplay",
	"Cpu",
};
XToString( PlayerController );
StringToX( PlayerController );
XToLocalizedString( PlayerController );
LuaXType( PlayerController );


static const char *HealthStateNames[] = {
	"Hot",
	"Alive",
	"Danger",
	"Dead",
};
XToString( HealthState );
LuaXType( HealthState );


static const char *CoinModeNames[] = {
	"Home",
	"Pay",
	"Free",
};
XToString( CoinMode );
StringToX( CoinMode );
LuaXType( CoinMode );


static const char *PremiumNames[] = {
	"Off",
	"DoubleFor1Credit",
	"2PlayersFor1Credit",
};
XToString( Premium );
StringToX( Premium );
XToLocalizedString( Premium );
LuaXType( Premium );


static const char *SortOrderNames[] = {
	"Preferred",
	"Group",
	"Title",
	"BPM",
	"Popularity",
	"TopGrades",
	"Artist",
	"Genre",
	"EasyMeter",
	"MediumMeter",
	"HardMeter",
	"ChallengeMeter",
	"DoubleEasyMeter",
	"DoubleMediumMeter",
	"DoubleHardMeter",
	"DoubleChallengeMeter",
	"ModeMenu",
	"AllCourses",
	"Nonstop",
	"Oni",
	"Endless",
	"Length",
	"Roulette",
};
XToString( SortOrder );
StringToX( SortOrder );
LuaXType( SortOrder );
XToLocalizedString( SortOrder );
LuaFunction( SortOrderToLocalizedString, SortOrderToLocalizedString(Enum::Check<SortOrder>(L, 1)) );


static const char *TapNoteScoreNames[] = {
	"None",
	"HitMine",
	"AvoidMine",
	"CheckpointMiss",
	"Miss",
	"W5",
	"W4",
	"W3",
	"W2",
	"W1",
	"CheckpointHit",
};
XToString( TapNoteScore );
LuaXType( TapNoteScore );
TapNoteScore StringToTapNoteScore( const RString &s )
{
	// new style
	if	   ( s == "None" )		return TNS_None;
	else if( s == "HitMine" )	return TNS_HitMine;
	else if( s == "AvoidMine" )	return TNS_AvoidMine;
	else if( s == "CheckpointHit" )	return TNS_CheckpointHit;
	else if( s == "CheckpointMiss" )return TNS_CheckpointMiss;
	else if( s == "Miss" )		return TNS_Miss;
	else if( s == "W5" )		return TNS_W5;
	else if( s == "W4" )		return TNS_W4;
	else if( s == "W3" )		return TNS_W3;
	else if( s == "W2" )		return TNS_W2;
	else if( s == "W1" )		return TNS_W1;

	// for backward compatibility
	else if( s == "Boo" )		return TNS_W5;
	else if( s == "Good" )		return TNS_W4;
	else if( s == "Great" )		return TNS_W3;
	else if( s == "Perfect" )	return TNS_W2;
	else if( s == "Marvelous" )	return TNS_W1;

	return TapNoteScore_Invalid;
}
XToLocalizedString( TapNoteScore );
LuaFunction( TapNoteScoreToLocalizedString, TapNoteScoreToLocalizedString(Enum::Check<TapNoteScore>(L, 1)) );


static const char *HoldNoteScoreNames[] = {
	"None",
	"LetGo",
	"Held",
};
XToString( HoldNoteScore );
LuaXType( HoldNoteScore );
HoldNoteScore StringToHoldNoteScore( const RString &s )
{
	// for backward compatibility
	if     ( s == "NG" )	return HNS_LetGo;
	else if( s == "OK" )	return HNS_Held;

	// new style
	else if( s == "None" )	return HNS_None;
	else if( s == "LetGo" )	return HNS_LetGo;
	else if( s == "Held" )	return HNS_Held;

	return HoldNoteScore_Invalid;
}
XToLocalizedString( HoldNoteScore );

static const char *TimingWindowNames[] = {
	"W1",
	"W2",
	"W3",
	"W4",
	"W5",
	"Mine",
	"Attack",
	"Hold",
	"Roll",
};
XToString( TimingWindow );

static const char *ScoreEventNames[] = {
	"CheckpointHit",
	"W1",
	"W2",
	"W3",
	"W4",
	"W5",
	"Miss",
	"HitMine",
	"CheckpointMiss",
	"Held",
	"LetGo",
};
XToString( ScoreEvent );

static const char *MemoryCardStateNames[] = {
	"ready",
	"checking",
	"late",
	"error",
	"removed",
	"none",
};
XToString( MemoryCardState );
LuaXType( MemoryCardState );

static const char *StageAwardNames[] = {
	"FullComboW3",
	"SingleDigitW3",
	"OneW3",
	"FullComboW2",
	"SingleDigitW2",
	"OneW2",
	"FullComboW1",
	"80PercentW3",
	"90PercentW3",
	"100PercentW3",
};
XToString( StageAward );
XToLocalizedString( StageAward );
StringToX( StageAward );
LuaFunction( StageAwardToLocalizedString, StageAwardToLocalizedString(Enum::Check<StageAward>(L, 1)) );
LuaXType( StageAward );

// Numbers are intentially not at the front of these strings so that the 
// strings can be used as XML entity names.
// Numbers are intentially not at the back so that "1000" and "10000" don't 
// conflict when searching for theme elements.
static const char *PeakComboAwardNames[] = {
	"1000",
	"2000",
	"3000",
	"4000",
	"5000",
	"6000",
	"7000",
	"8000",
	"9000",
	"10000",
};
XToString( PeakComboAward );
XToLocalizedString( PeakComboAward );
StringToX( PeakComboAward );
LuaFunction( PeakComboAwardToLocalizedString, PeakComboAwardToLocalizedString(Enum::Check<PeakComboAward>(L, 1)) );
LuaXType( PeakComboAward );


void DisplayBpms::Add( float f )
{
	vfBpms.push_back( f );
}

float DisplayBpms::GetMin() const
{
	float fMin = FLT_MAX;
	FOREACH_CONST( float, vfBpms, f )
	{
		if( *f != -1 )
			fMin = min( fMin, *f );
	}
	if( fMin == FLT_MAX )
		return 0;
	else
		return fMin;
}

float DisplayBpms::GetMax() const
{
	float fMax = 0;
	FOREACH_CONST( float, vfBpms, f )
	{
		if( *f != -1 )
			fMax = max( fMax, *f );
	}
	return fMax;
}

bool DisplayBpms::BpmIsConstant() const
{
	return fabsf( GetMin() - GetMax() ) < 0.001f;
}

bool DisplayBpms::IsSecret() const
{
	FOREACH_CONST( float, vfBpms, f )
	{
		if( *f == -1 )
			return true;
	}
	return false;
}

static const char *StyleTypeNames[] = {
	"OnePlayerOneSide",
	"TwoPlayersTwoSides",
	"OnePlayerTwoSides",
	"TwoPlayersSharedSides",
};
XToString( StyleType );
StringToX( StyleType );
LuaXType( StyleType );

static const char *GoalTypeNames[] = {
	"Calories",
	"Time",
	"None",
};
XToString( GoalType );
StringToX( GoalType );
LuaXType( GoalType );

static const char *EditModeNames[] = {
	"Practice",
	"CourseMods",
	"Home",
	"Full"
};
XToString( EditMode );
StringToX( EditMode );
LuaXType( EditMode );

static const char *StageNames[] = {
	"1st",
	"2nd",
	"3rd",
	"4th",
	"5th",
	"6th",
	"Next",
	"Final",
	"Extra1",
	"Extra2",
	"Nonstop",
	"Oni",
	"Endless",
	"Event",
	"Demo",
};
XToString( Stage );
LuaXType( Stage );
XToLocalizedString( Stage );
LuaFunction( StageToLocalizedString, StageToLocalizedString(Enum::Check<Stage>(L, 1)) );

static const char *EarnedExtraStageNames[] = {
	"No",
	"Extra1",
	"Extra2",
};
XToString( EarnedExtraStage );
LuaXType( EarnedExtraStage );


static const char *MultiPlayerStatusNames[] = {
	"Joined",
	"NotJoined",
	"Unplugged",
	"MissingMultitap",
};
XToString( MultiPlayerStatus );


/*
 * (c) 2001-2004 Chris Danford
 * All rights reserved.
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, and/or sell copies of the Software, and to permit persons to
 * whom the Software is furnished to do so, provided that the above
 * copyright notice(s) and this permission notice appear in all copies of
 * the Software and that both the above copyright notice(s) and this
 * permission notice appear in supporting documentation.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT OF
 * THIRD PARTY RIGHTS. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR HOLDERS
 * INCLUDED IN THIS NOTICE BE LIABLE FOR ANY CLAIM, OR ANY SPECIAL INDIRECT
 * OR CONSEQUENTIAL DAMAGES, OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS
 * OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
 * OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 */
