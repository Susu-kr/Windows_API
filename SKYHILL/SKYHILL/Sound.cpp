#include "Sound.h"

FMOD_SYSTEM* CSound::s_system;

CSound::CSound(const char * path, bool loop)
{
	if (loop) {
		FMOD_System_CreateSound(s_system, path, FMOD_LOOP_NORMAL, 0, &m_sound[0]);
	}
	else {
		FMOD_System_CreateSound(s_system, path, FMOD_DEFAULT, 0, &m_sound[0]);
	}

	m_channel = nullptr;
	m_volume = SOUND_DEFAULT;
}

CSound::~CSound()
{
	FMOD_Sound_Release(*m_sound);
}

int CSound::Init()
{
	FMOD_System_Create(&s_system);
	FMOD_System_Init(s_system, 32, FMOD_INIT_NORMAL, NULL);
	return 0;
}

int CSound::Release()
{
	FMOD_System_Close(s_system);
	FMOD_System_Release(s_system);
	return 0;
}

void CSound::Add_effect(const char * path, int cnt)
{
	FMOD_System_CreateSound(s_system, path, FMOD_DEFAULT, 0, &m_sound[cnt]);
	m_channel = nullptr;
	e_volume = SOUND_DEFAULT;
}

int CSound::play(int cnt)
{
	FMOD_System_PlaySound(s_system, m_sound[cnt], NULL, false, &m_channel);
	return 0;
}

int CSound::pause()
{
	FMOD_Channel_SetPaused(m_channel, true);
	return 0;
}

int CSound::resume()
{
	FMOD_Channel_SetPaused(m_channel, false);
	return 0;
}

int CSound::stop()
{
	FMOD_Channel_Stop(m_channel);
	return 0;
}

int CSound::volumeUp()
{
	if (m_volume < SOUND_MAX) {
		m_volume += SOUND_WEIGHT;
	}

	FMOD_Channel_SetVolume(m_channel, m_volume);

	return 0;
}

int CSound::volumeDown()
{
	if (m_volume > SOUND_MIN) {
		m_volume -= SOUND_WEIGHT;
	}

	FMOD_Channel_SetVolume(m_channel, m_volume);

	return 0;
}

int CSound::e_volumeUp()
{
	if (e_volume < SOUND_MAX) {
		e_volume += SOUND_WEIGHT;
	}

	FMOD_Channel_SetVolume(m_channel, e_volume);
	return 0;
}

int CSound::e_volumeDown()
{
	if (e_volume > SOUND_MIN) {
		e_volume -= SOUND_WEIGHT;
	}

	FMOD_Channel_SetVolume(m_channel, e_volume);
	return 0;
}

int CSound::Update()
{
	FMOD_Channel_IsPlaying(m_channel, &m_bool);

	if (m_bool) {
		FMOD_System_Update(s_system);
	}
	return 0;
}
