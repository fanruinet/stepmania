#include "global.h"
#include "CommonMetrics.h"
#include "RageUtil.h"
#include "GameManager.h"
#include "RageLog.h"
#include "GameState.h"
#include "ProductInfo.h"
#include "LuaManager.h"

using std::vector;

ThemeMetric<std::string>		CommonMetrics::OPERATOR_MENU_SCREEN		("Common","OperatorMenuScreen");
ThemeMetric<std::string>		CommonMetrics::FIRST_ATTRACT_SCREEN		("Common","FirstAttractScreen");
ThemeMetric<std::string>		CommonMetrics::DEFAULT_MODIFIERS		("Common","DefaultModifiers" );
LocalizedString				CommonMetrics::WINDOW_TITLE				("Common","WindowTitle");
ThemeMetric<int>			CommonMetrics::MAX_COURSE_ENTRIES_BEFORE_VARIOUS("Common","MaxCourseEntriesBeforeShowVarious");
ThemeMetric<float>			CommonMetrics::TICK_EARLY_SECONDS		("ScreenGameplay","TickEarlySeconds");
ThemeMetricDifficultiesToShow	CommonMetrics::DIFFICULTIES_TO_SHOW		("Common","DifficultiesToShow");
ThemeMetricCourseDifficultiesToShow	CommonMetrics::COURSE_DIFFICULTIES_TO_SHOW	("Common","CourseDifficultiesToShow");
ThemeMetricStepsTypesToShow	CommonMetrics::STEPS_TYPES_TO_SHOW		("Common","StepsTypesToHide");
ThemeMetric<bool>			CommonMetrics::AUTO_SET_STYLE			("Common","AutoSetStyle");
ThemeMetric<int>			CommonMetrics::PERCENT_SCORE_DECIMAL_PLACES	("Common","PercentScoreDecimalPlaces");

ThemeMetricDifficultiesToShow::ThemeMetricDifficultiesToShow( const std::string& sGroup, const std::string& sName ) :
	ThemeMetric<std::string>(sGroup,sName)
{
	// re-read because ThemeMetric::ThemeMetric calls ThemeMetric::Read, not the derived one
	if( IsLoaded() )
		Read();
}
void ThemeMetricDifficultiesToShow::Read()
{
	ASSERT(Rage::ends_with(GetName(), "ToShow"));

	ThemeMetric<std::string>::Read();

	m_v.clear();

	auto v = Rage::split( ThemeMetric<std::string>::GetValue(), "," );
	if(v.empty())
	{
		LuaHelpers::ReportScriptError("DifficultiesToShow must have at least one entry.");
		return;
	}

	for (auto const &i: v)
	{
		Difficulty d = StringToDifficulty( i );
		if( d == Difficulty_Invalid )
		{
			LuaHelpers::ReportScriptErrorFmt("Unknown difficulty \"%s\" in CourseDifficultiesToShow.", i.c_str());
		}
		else
		{
			m_v.push_back( d );
		}
	}
}
const vector<Difficulty>& ThemeMetricDifficultiesToShow::GetValue() const { return m_v; }


ThemeMetricCourseDifficultiesToShow::ThemeMetricCourseDifficultiesToShow( const std::string& sGroup, const std::string& sName ) :
	ThemeMetric<std::string>(sGroup,sName)
{
	// re-read because ThemeMetric::ThemeMetric calls ThemeMetric::Read, not the derived one
	if( IsLoaded() )
		Read();
}
void ThemeMetricCourseDifficultiesToShow::Read()
{
	ASSERT(Rage::ends_with(GetName(), "ToShow"));

	ThemeMetric<std::string>::Read();

	m_v.clear();

	auto v = Rage::split( ThemeMetric<std::string>::GetValue(), "," );
	if(v.empty())
	{
		LuaHelpers::ReportScriptError("CourseDifficultiesToShow must have at least one entry.");
		return;
	}

	for (auto const &i: v)
	{
		CourseDifficulty d = StringToDifficulty( i );
		if( d == Difficulty_Invalid )
		{
			LuaHelpers::ReportScriptErrorFmt("Unknown CourseDifficulty \"%s\" in CourseDifficultiesToShow.", i.c_str());
		}
		else
		{
			m_v.push_back( d );
		}
	}
}
const vector<CourseDifficulty>& ThemeMetricCourseDifficultiesToShow::GetValue() const { return m_v; }

static void RemoveStepsTypes( vector<StepsType>& inout, std::string sStepsTypesToRemove )
{
	auto v = Rage::split(sStepsTypesToRemove, ",");
	if( v.size() == 0 ) return; // Nothing to do!

	// subtract StepsTypes
	for (auto const &i: v)
	{
		StepsType st = GAMEMAN->StringToStepsType(i);
		if( st == StepsType_Invalid )
		{
			LuaHelpers::ReportScriptErrorFmt( "Invalid StepsType value '%s' in '%s'", i.c_str(), sStepsTypesToRemove.c_str() );
			continue;
		}

		const vector<StepsType>::iterator iter = find( inout.begin(), inout.end(), st );
		if( iter != inout.end() )
			inout.erase( iter );
	}
}
ThemeMetricStepsTypesToShow::ThemeMetricStepsTypesToShow( const std::string& sGroup, const std::string& sName ) :
	ThemeMetric<std::string>(sGroup,sName)
{
	// re-read because ThemeMetric::ThemeMetric calls ThemeMetric::Read, not the derived one
	if( IsLoaded() )
		Read();
}
void ThemeMetricStepsTypesToShow::Read()
{
	ASSERT(Rage::ends_with(GetName(), "ToHide"));

	ThemeMetric<std::string>::Read();

	m_v.clear();
	GAMEMAN->GetStepsTypesForGame( GAMESTATE->m_pCurGame, m_v );

	RemoveStepsTypes( m_v, ThemeMetric<std::string>::GetValue() );
}
const vector<StepsType>& ThemeMetricStepsTypesToShow::GetValue() const { return m_v; }


std::string CommonMetrics::LocalizeOptionItem( const std::string &s, bool bOptional )
{
	if( bOptional && !THEME->HasString("OptionNames",s) )
		return s;
	return THEME->GetString( "OptionNames", s );
}

LuaFunction( LocalizeOptionItem, CommonMetrics::LocalizeOptionItem(SArg(1),true) );

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
