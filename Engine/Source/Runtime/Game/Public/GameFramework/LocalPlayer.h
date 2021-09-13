// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "SlateMinimal.h"
#include "Player.h"

class SGameEngine;
class SWindow;
class SSlateShader;
class SRHIDeviceContext;
struct IFrameworkView;

class GAME_API SLocalPlayer : public SPlayer
{
public:
	using Super = SPlayer;

private:
	IFrameworkView* _frameworkView = nullptr;
	SWindow* _slateWindow = nullptr;
	SSlateWindowElementList* _drawElements = nullptr;

public:
	SLocalPlayer();

	void Init(IFrameworkView* frameworkView);
	void Tick(std::chrono::duration<float> elapsedTime);
	void Render(SRHIDeviceContext* deviceContext, SSlateShader* shader);

	inline IFrameworkView* GetFrameworkView() const { return _frameworkView; }
	inline SWindow* GetSlateWindow() const { return _slateWindow; }

private:
	void OnResizedApp(int32 x, int32 y);
};