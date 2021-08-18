#include "Sound.h"

CSound::CSound(void)
{
	FMOD_System_Create(&s_system);
	FMOD_System_Init(s_system, 32, FMOD_INIT_NORMAL, NULL);
}

CSound::~CSound()
{
	FMOD_System_Close(s_system);
	FMOD_System_Release(s_system);
}

void CSound::CreateBGSound(int count, string * path)
{
	// 배경음 생성
	BG_count = count;
	BG_sound = new FMOD_SOUND*[count];
	BG_channel = new FMOD_CHANNEL*[count];
	for (int i = 0; i < count; i++)
	{
		FMOD_System_CreateSound(s_system, path[i].data()
			, FMOD_LOOP_NORMAL, 0, &BG_sound[i]);
	}
	m_volume = SOUND_DEFAULT;
}

void CSound::CreateEFFSound(int count, string * path)
{
	// 효과음 생성
	EFF_count = count;
	EFF_sound = new FMOD_SOUND*[count];
	//EFF_channel = new FMOD_CHANNEL*[count];

	for (int i = 0; i < count; i++)
	{
		FMOD_System_CreateSound(s_system, path[i].data()
			, FMOD_DEFAULT, 0, &EFF_sound[i]);
	}
	e_volume = SOUND_DEFAULT;

}
void CSound::PlaySoundEffect(int nIndex)
{
	if (nIndex < EFF_count)
		FMOD_System_PlaySound(s_system, EFF_sound[nIndex], NULL, false, &EFF_channel);
	FMOD_Channel_SetVolume(EFF_channel, e_volume);
}

void CSound::PlaySoundBG(int nIndex)
{
	if (nIndex < BG_count)
		FMOD_System_PlaySound(s_system, BG_sound[nIndex], NULL, false, &BG_channel[nIndex]);
	FMOD_Channel_SetVolume(BG_channel[nIndex], m_volume);
}

void CSound::StopSoundBG(int nIndex)
{
	if (nIndex < BG_count)
		FMOD_Channel_Stop(BG_channel[nIndex]);
}

void CSound::ReleaseSound()
{
	int i;
	delete[] BG_channel;
	for (i = 0; i < BG_count; i++)
		FMOD_Sound_Release(BG_sound[i]);
	delete[] BG_sound;

	for (i = 0; i < EFF_count; i++)
		FMOD_Sound_Release(EFF_sound[i]);
	delete[] EFF_sound;
}

void CSound::Update()
{
	if (!s_system) {
		FMOD_System_Update(s_system);
	}
}

void CSound::volumeUp()
{
	if (m_volume < SOUND_MAX) {
		m_volume += SOUND_WEIGHT;
	}
	for(int i = 0 ; i < BG_count; i++)
		FMOD_Channel_SetVolume(BG_channel[i], m_volume);
}

void CSound::volumeDown()
{
	if (m_volume > SOUND_MIN + 0.1) {
		m_volume -= SOUND_WEIGHT;
	}
	for (int i = 0; i < BG_count; i++)
		FMOD_Channel_SetVolume(BG_channel[i], m_volume);
}

void CSound::e_volumeUp()
{
	if (e_volume < SOUND_MAX) {
		e_volume += SOUND_WEIGHT;
	}

}

void CSound::e_volumeDown()
{
	if (e_volume > SOUND_MIN + 0.1) {
		e_volume -= SOUND_WEIGHT;
	}

}

