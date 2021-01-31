﻿// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"
#include "IEngineTick.h"

#include <chrono>
#include "Logging/LogCategoryBase.h"

interface IRHIBundle;
interface IRHICommandFence;
interface IRHIDeviceBundle;
interface IRHIImmediateCommandList;
interface IRHISwapChain;

class SceneRenderer;
class GameInstance;
class AssetManager;
class GameViewport;

class GAME_API Engine : virtual public Object, virtual public IEngineTick
{
public:
	using Super = Object;
	using This = Engine;

private:
	static Engine* gEngine;

	LogCategoryBase LogEngine;
	GameInstance* gameInstance;
	std::chrono::steady_clock::time_point prev_tick;

	std::vector<TRefPtr<IRHIBundle>> rhiBundles;
	TRefPtr<IRHICommandFence> autoFence;

	IRHIDeviceBundle* deviceBundle;
	IRHIImmediateCommandList* immediateCommandList;
	IRHISwapChain* swapChain;

	TRefPtr<AssetManager> assetManager;
	TRefPtr<GameViewport> gameViewport;

public:
	Engine();
	~Engine() override;

	virtual void Initialize();
	virtual void PostInitialize();
	virtual void Tick();
	virtual void Shutdown();

	vs_property_get(IRHIDeviceBundle*, DeviceBundle);
	IRHIDeviceBundle* DeviceBundle_get() const;
	AssetManager* GetAssetManager() const;
	GameViewport* GetGameViewport() const;

	static Engine* GetInstance();

private:
	void TickWorld();
	void RenderScene();

	void ForEachBundles(std::function<void(IRHIBundle*)> action);
	void LoadEngineDefaultAssets();

	void Application_OnPostSized(int32 x, int32 y);
};