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

// 볼륨 수치 정의
#define SOUND_MAX 1.0f
#define SOUND_MIN 0.0f
#define SOUND_DEFAULT 0.5f
#define SOUND_WEIGHT 0.1f

class CSound
{
private:
	FMOD_SYSTEM *s_system;		// 소리를 관리하는 시스템 생성
	FMOD_SOUND **BG_sound;		// 배경음 (로비, 게임)
	FMOD_SOUND **EFF_sound;		// 효과음 (클릭시 소리, 이동시 소리, 적 효과음, 피격음, 죽는소리)
	FMOD_CHANNEL **BG_channel;	// 배경음 소리채널
	FMOD_CHANNEL *EFF_channel;	// 효과음 소리채널

	int BG_count;				// 배경음 수
	int EFF_count;				// 효과음 수
	float m_volume;				// 배경음 볼륨
	float e_volume;				// 효과음 볼륨
	FMOD_BOOL m_bool;

public:
	CSound(void);
	~CSound(void);

	void CreateBGSound(int count, string * path);		// 배경음 생성
	void CreateEFFSound(int count, string * path);		// 효과음 생성
	void PlaySoundEffect(int nIndex);					// 효과음 재생
	void PlaySoundBG(int nIndex);						// 배경음 재생
	void StopSoundBG(int nIndex);						// 배경음 정지
	void ReleaseSound();								// 소리들 제거
	void Update();										// 소리의 변경사항들을 계속 업데이트 해줘야함
	void volumeUp();									// 배경음 소리 증가
	void volumeDown();									// 배경음 소리 감소
	void e_volumeUp();									// 효과음 소리 증가
	void e_volumeDown();								// 효과음 소리 감소
	float getM_vol() { return m_volume; }				// 옵션 창에서 배경음의 수치를 표현하기 위함
	float getE_vol() { return e_volume; }				// 옵션 창에서 효과음의 수치를 표현하기 위함
};

#endif _SOUND_H_