#include "global.h"
#include "Preference.h"
#include "XmlFile.h"
#include "RageLog.h"
#include "LuaManager.h"
#include "MessageManager.h"
#include "SubscriptionManager.h"

static SubscriptionManager<IPreference> m_Subscribers;

IPreference::IPreference( std::string const & sName ):
	m_sName( sName ),
	m_bIsStatic( false )
{
	m_Subscribers.Subscribe( this );
}

IPreference::~IPreference()
{
	m_Subscribers.Unsubscribe( this );
}

IPreference *IPreference::GetPreferenceByName( std::string const &sName )
{
	Rage::ci_ascii_string ciName{ sName.c_str() };
	for (auto *p: *m_Subscribers.m_pSubscribers)
	{
		if (ciName == p->GetName())
		{
			return p;
		}
	}

	return nullptr;
}

void IPreference::LoadAllDefaults()
{
	for (auto *p: *m_Subscribers.m_pSubscribers)
	{
		p->LoadDefault();
	}
}

void IPreference::ReadAllPrefsFromNode( const XNode* pNode, bool bIsStatic )
{
	ASSERT( pNode != nullptr );
	for (auto *p: *m_Subscribers.m_pSubscribers)
	{
		p->ReadFrom( pNode, bIsStatic );
	}
}

void IPreference::SavePrefsToNode( XNode* pNode )
{
	for (auto *p: *m_Subscribers.m_pSubscribers)
	{
		p->WriteTo( pNode );
	}
}

void IPreference::ReadAllDefaultsFromNode( const XNode* pNode )
{
	if( pNode == nullptr )
	{
		return;
	}
	for (auto *p: *m_Subscribers.m_pSubscribers)
	{
		p->ReadDefaultFrom( pNode );
	}
}

void IPreference::PushValue( lua_State *L ) const
{
	if( LOG )
		LOG->Trace( "The preference value \"%s\" is of a type not supported by Lua", m_sName.c_str() );

	lua_pushnil( L );
}

void IPreference::SetFromStack( lua_State *L )
{
	if( LOG )
		LOG->Trace( "The preference value \"%s\" is of a type not supported by Lua", m_sName.c_str() );

	lua_pop( L, 1 );
}

void IPreference::ReadFrom( const XNode* pNode, bool bIsStatic )
{
	std::string sVal;
	if( pNode->GetAttrValue(m_sName, sVal) )
	{
		FromString( sVal );
		m_bIsStatic = bIsStatic;
	}
}

void IPreference::WriteTo( XNode* pNode ) const
{
	if( !m_bIsStatic )
		pNode->AppendAttr( m_sName, ToString() );
}

/* Load our value from the node, and make it the new default. */
void IPreference::ReadDefaultFrom( const XNode* pNode )
{
	std::string sVal;
	if( !pNode->GetAttrValue(m_sName, sVal) )
		return;
	SetDefaultFromString( sVal );
}

void BroadcastPreferenceChanged( const std::string& sPreferenceName )
{
	if( MESSAGEMAN )
		MESSAGEMAN->Broadcast( sPreferenceName+"Changed" );
}

/*
 * (c) 2001-2004 Chris Danford, Chris Gomez
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
