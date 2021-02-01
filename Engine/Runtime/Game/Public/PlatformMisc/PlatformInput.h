﻿// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

#include "Key.h"

struct KeyboardState
{
	friend struct KeyboardCompare;

private:
	std::vector<char> states;

public:
	KeyboardState();
	KeyboardState(const KeyboardState& rh);
	KeyboardState(KeyboardState&& rh) noexcept;
	KeyboardState(const std::vector<char>& inKeyStates);
	~KeyboardState();

	bool HasKeyDown(EKey inKey) const;
	bool HasKeyUp(EKey inKey) const;

	KeyboardState& operator =(const KeyboardState& rh);
	KeyboardState& operator =(KeyboardState&& rh);
};

struct KeyboardCompare
{
private:
	std::vector<char> keyDown;
	std::vector<char> keyUp;

public:
	KeyboardCompare();
	~KeyboardCompare();

	void Compare(const KeyboardState& lh, const KeyboardState& rh);

	bool IsKeyDown(EKey inKey) const;
	bool IsKeyUp(EKey inKey) const;
};

struct CursorState
{
private:
	int32 cursorX;
	int32 cursorY;
	bool bInitialized : 1;
	bool bCursorVisible : 1;

public:
	CursorState();
	CursorState(int32 cursorX, int32 cursorY, bool bCursorVisible);
	
	int32 GetCursorX() const;
	int32 GetCursorY() const;
	bool IsCursorVisible() const;
	bool IsInitialized() const;
};

struct CursorCompare
{
private:
	int32 cursorDeltaX;
	int32 cursorDeltaY;
	float dpiDeltaX;
	float dpiDeltaY;

public:
	CursorCompare();

	void Compare(const CursorState& lh, const CursorState& rh);

	int32 GetDeltaX() const;
	int32 GetDeltaY() const;

	float GetDeltaXByDpi() const;
	float GetDeltaYByDpi() const;
};

class PlatformInput : virtual public Object
{
	friend struct CursorCompare;

	static bool bCaptureCursor;
	static std::vector<char> bKeyboardStates;

	static int32 cursorX;
	static int32 cursorY;
	static bool bCursorVisible;

public:
	static void Tick();

	static KeyboardState GetKeyboardState();
	static CursorState GetCursorState();
	static void SetCursorVisible(bool value);
	static bool IsInputActive();

private:
	static void SetCursorToHwndCenter();
	static void GetCenterCursor(int32& x, int32& y);
};