#pragma once

#include "framework.h"

#ifndef _SOUND_H_
#define _SOUND_H_

// FMOD SOUND
#include "fmod.hpp"

using namespace FMOD;

#define SOUND_MAX 1.0f
#define SOUND_MIN 0.0f
#define SOUND_DEFAULT 0.5f
#define SOUND_WEIGHT 0.1f

class CSound
{
private:
	static FMOD_SYSTEM *s_system;

	FMOD_SOUND *m_sound[5];
	FMOD_CHANNEL *m_channel;

	float m_volume;
	float e_volume;
	FMOD_BOOL m_bool;

public:
	CSound(const char * path, bool loop);
	~CSound();

	static int Init();		// 최초 실행시 딱 한번
	static int Release();	// 프로그램 종료 직전 코드가 실행

	void Add_effect(const char * path, int cnt);
	int play(int cnt);
	int pause();
	int resume();
	int stop();
	int volumeUp();
	int volumeDown();
	int e_volumeUp();
	int e_volumeDown();
	int Update();
};

#endif _SOUND_H_