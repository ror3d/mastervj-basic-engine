#include "CinematicPlayer.h"


CCinematicPlayer::CCinematicPlayer()
	: m_CurrentTime(0)
	, m_Duration(0)
	, m_Cycle(false)
	, m_Playing(false)
{
}


CCinematicPlayer::~CCinematicPlayer()
{
}


void CCinematicPlayer::Init( float Duration )
{
	m_Duration = Duration;
	m_Playing = false;
	m_CurrentTime = 0;
}


void CCinematicPlayer::Update( float ElapsedTime )
{
	if ( m_Playing )
	{
		m_CurrentTime += ElapsedTime;
		if ( m_CurrentTime > m_Duration )
		{
			if ( m_Cycle )
			{
				while ( m_CurrentTime > m_Duration )
				{
					m_CurrentTime -= m_Duration;
				}
			}
			OnRestartCycle();
		}
	}
}


void CCinematicPlayer::Stop()
{
	Pause();
	m_CurrentTime = 0;
	OnRestartCycle();
}


void CCinematicPlayer::Play( bool Cycle )
{
	m_Playing = true;
}


void CCinematicPlayer::Pause()
{
	m_Playing = false;
}


void CCinematicPlayer::OnRestartCycle()
{
}


