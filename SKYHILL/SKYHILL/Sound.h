#pragma once
/*
	참고 사이트
	https://hombody.tistory.com/44
	https://m.cafe.daum.net/nowcampus/ZuQY/13?
*/
#include "framework.h"

#ifndef _SOUND_H_
#define _SOUND_H_

// FMOD SOUND
#include "fmod.hpp"
#include <string>

using namespace std;
using namespace FMOD;

#define SOUND_MAX 1.0f
#define SOUND_MIN 0.0f
#define SOUND_DEFAULT 0.5f
#define SOUND_WEIGHT 0.1f

class CSound
{
private:
	FMOD_SYSTEM *s_system;
	FMOD_SOUND **BG_sound; // 배경음 (로비, 게임)
	FMOD_SOUND **EFF_sound; // 효과음 (클릭시 소리, 이동시 소리, 적 효과음, 피격음, 죽는소리)
	// 이거 여러가지 메인 음악이랑 효과음 여러개 동시에 작동시키려고 미리 넣어놓기위해서 배열로 선언
	FMOD_CHANNEL **BG_channel;
	FMOD_CHANNEL *EFF_channel;

	int BG_count;
	int EFF_count;
	float m_volume;
	float e_volume;
	FMOD_BOOL m_bool;

public:
	CSound(void);
	~CSound(void);

	void CreateBGSound(int count, string * path);
	void CreateEFFSound(int count, string * path);
	void PlaySoundEffect(int nIndex);
	void PlaySoundBG(int nIndex);
	void StopSoundBG(int nIndex);
	void ReleaseSound();
	void Update();
	void volumeUp();
	void volumeDown();
	void e_volumeUp();
	void e_volumeDown();
	float getM_vol() { return m_volume; }
	float getE_vol() { return e_volume; }

};

#endif _SOUND_H_