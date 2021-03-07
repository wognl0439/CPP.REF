﻿// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Engine.h"

#include "GameInstance.h"
#include "Logging/LogMacros.h"
#include "Diagnostics/ScopedCycleCounter.h"
#include "Windows/CoreWindow.h"
#include "DirectX/DirectXCommon.h"
#include "DirectX/DirectXDeviceBundle.h"
#include "DirectX/DirectXCommandQueue.h"
#include "DirectX/DirectXSwapChain.h"
#include "DirectX/DirectXImmediateContext.h"
#include "DirectX/DirectXAutoFence.h"

using namespace std;
using namespace std::chrono;

DEFINE_STATS_GROUP(Engine);

Engine::Engine() : Super()
	, gameInstance(nullptr)
{

}

Engine::~Engine()
{

}

void Engine::Initialize(GameInstance* gameInstance)
{
	this->gameInstance = gameInstance;
	deviceBundle = NewObject<DirectXDeviceBundle>();
	DirectXNew(primaryQueue, DirectXCommandQueue, deviceBundle.Get());

	swapChain = NewObject<DirectXSwapChain>(deviceBundle.Get(), primaryQueue.Get(), gameInstance->MainWindow, DXGI_FORMAT_B8G8R8A8_UNORM);
	DirectXNew(immediateContext, DirectXImmediateContext, deviceBundle.Get(), primaryQueue.Get(), D3D12_COMMAND_LIST_TYPE_DIRECT);
	DirectXNew(immediateFence, DirectXAutoFence, deviceBundle.Get());

	gameInstance->MainWindow->Sizing += bind_delegate(MainWindow_OnSizing);
}

void Engine::Tick()
{
	QUICK_SCOPED_CYCLE_COUNTER(Engine, Tick);

	Render();
}

void Engine::Shutdown()
{
	CycleStatsGroup::ReadyToShutdown();
}

void Engine::Render()
{
	immediateFence->Wait();

	swapChain->Present();

	immediateFence->AcceptSignal(primaryQueue.Get());
}

void Engine::MainWindow_OnSizing(int32 x, int32 y)
{
	if (x * y != 0)
	{
		swapChain->ResizeBuffers(x, y);
		SE_LOG(LogEngine, Verbose, L"SwapChain was resize to [{0}x{1}].", x, y);
	}
	else
	{
		SE_LOG(LogEngine, Verbose, L"Window size is nearly zero. Skip resizing swap chain.");
	}
}