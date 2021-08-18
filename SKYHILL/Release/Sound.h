#pragma once
/*
	���� ����Ʈ
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

// ���� ��ġ ����
#define SOUND_MAX 1.0f
#define SOUND_MIN 0.0f
#define SOUND_DEFAULT 0.5f
#define SOUND_WEIGHT 0.1f

class CSound
{
private:
	FMOD_SYSTEM *s_system;		// �Ҹ��� �����ϴ� �ý��� ����
	FMOD_SOUND **BG_sound;		// ����� (�κ�, ����)
	FMOD_SOUND **EFF_sound;		// ȿ���� (Ŭ���� �Ҹ�, �̵��� �Ҹ�, �� ȿ����, �ǰ���, �״¼Ҹ�)
	FMOD_CHANNEL **BG_channel;	// ����� �Ҹ�ä��
	FMOD_CHANNEL *EFF_channel;	// ȿ���� �Ҹ�ä��

	int BG_count;				// ����� ��
	int EFF_count;				// ȿ���� ��
	float m_volume;				// ����� ����
	float e_volume;				// ȿ���� ����
	FMOD_BOOL m_bool;

public:
	CSound(void);
	~CSound(void);

	void CreateBGSound(int count, string * path);		// ����� ����
	void CreateEFFSound(int count, string * path);		// ȿ���� ����
	void PlaySoundEffect(int nIndex);					// ȿ���� ���
	void PlaySoundBG(int nIndex);						// ����� ���
	void StopSoundBG(int nIndex);						// ����� ����
	void ReleaseSound();								// �Ҹ��� ����
	void Update();										// �Ҹ��� ������׵��� ��� ������Ʈ �������
	void volumeUp();									// ����� �Ҹ� ����
	void volumeDown();									// ����� �Ҹ� ����
	void e_volumeUp();									// ȿ���� �Ҹ� ����
	void e_volumeDown();								// ȿ���� �Ҹ� ����
	float getM_vol() { return m_volume; }				// �ɼ� â���� ������� ��ġ�� ǥ���ϱ� ����
	float getE_vol() { return e_volume; }				// �ɼ� â���� ȿ������ ��ġ�� ǥ���ϱ� ����
};

#endif _SOUND_H_