#include "stdafx.h"
#include "Player.h"


CPlayer::CPlayer()
{
}

void CPlayer::PickUpKey()
{
	m_key = true;
}

bool CPlayer::HasKey()
{
	return m_key;
}


CPlayer::~CPlayer()
{
}
