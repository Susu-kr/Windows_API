#pragma once

#ifndef __INPUTMANAGER_H__
#define __INPUTMANAGER_H__

#include <wtypes.h> // USE DWORD

// 사용자의 입력을 관리하는 클래스입니다.

class InputManager
{
private:
	bool is_key_up;
	bool is_key_down;
	bool is_key_left;
	bool is_key_right;
	bool is_key_space;

	void CheckKeyboardInput();

public:

	void Update();

	bool GetKeyUp()  const { return is_key_up; }
	bool GetKeyDown()  const { return is_key_down; }
	bool GetKeyLeft()  const { return is_key_left; }
	bool GetKeyRight()  const { return is_key_right; }
	bool GetKeySpace()  const { return is_key_space; }

	int  GetHorizontal() const;
	int  GetVertical() const;
};

#endif // !__IN