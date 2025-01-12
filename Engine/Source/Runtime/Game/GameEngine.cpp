// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "GameEngine.h"
#include "LogGame.h"
#include "IFrameworkView.h"
#include "GameInstance.h"
#include "RHI/RHIDevice.h"
#include "RHI/RHISwapChain.h"
#include "RHI/RHIDeviceContext.h"
#include "RHI/RHICommandQueue.h"
#include "RHI/RHIRenderTargetView.h"
#include "RHI/RHITexture2D.h"
#include "RHI/RHIDepthStencilView.h"
#include "Shaders/ColorShader/ColorVertexFactory.h"
#include "Shaders/ColorShader/ColorShader.h"
#include "Shaders/TransparentShader/TransparentShader.h"
#include "Components/InputComponent.h"
#include "Level/World.h"
#include "Camera/PlayerCameraManager.h"
#include "Scene/Scene.h"
#include "Scene/SceneRenderer.h"
#include "Assets/AssetImporter.h"

using enum ELogVerbosity;

using namespace std;
using namespace std::chrono;

GameEngine* GameEngine::_gEngine = nullptr;

GameEngine::GameEngine(bool bDebug) : Super()
	, _bDebug(bDebug)
{
}

GameEngine::~GameEngine()
{
}

void GameEngine::InitEngine(GameInstance* gameInstance)
{
	LogSystem::Log(LogEngine, Info, L"Initialize engine.");
	_gEngine = this;

	IFrameworkView* frameworkView = gameInstance->GetFrameworkView();

	LogSystem::Log(LogEngine, Info, L"Initialize RHI subsystems.");
	_gameInstance = gameInstance;
	_device = CreateSubobject<RHIDevice>(_bDebug);
	_primaryQueue = _device->GetPrimaryQueue();
	_frameworkViewChain = CreateSubobject<RHISwapChain>(_device, frameworkView, _primaryQueue);
	_deviceContext = CreateSubobject<RHIDeviceContext>(_device);
	_colorVertexFactory = CreateSubobject<ColorVertexFactory>(_device);
	_colorShader = CreateSubobject<ColorShader>(_device);
	_colorShader->Compile(_colorVertexFactory);
	_rtv = CreateSubobject<RHIRenderTargetView>(_device, 3);
	_assimp = CreateSubobject<AssetImporter>(this, _colorVertexFactory);
	_dsv = CreateSubobject<RHIDepthStencilView>(_device, 1);
	_transparentShader = CreateSubobject<TransparentShader>(_device);
	_transparentShader->Compile(_colorVertexFactory);

	LogSystem::Log(LogEngine, Info, L"Register engine tick.");
	frameworkView->Idle += [this]() { TickEngine(); };
	frameworkView->Size += [this](int32 width, int32 height) { ResizedApp(width, height); };

	RegisterRHIGarbageCollector();
}

void GameEngine::RegisterRHIGarbageCollector()
{
	auto gc = [this]()
	{
		int32 count = _primaryQueue->Collect();
		LogSystem::Log(LogEngine, Verbose, L"Collect {} RHI garbages.", count);
	};
	_scheduler.AddSchedule({ .Task = gc, .Delay = 10s, .InitDelay = 10s });
}

void GameEngine::TickEngine()
{
	duration<float> deltaSeconds = 0ns;
	steady_clock::time_point now = steady_clock::now();
	if (_prev.has_value())
	{
		deltaSeconds = now - _prev.value();
	}
	_prev = now;

	GameTick(deltaSeconds);
	RenderTick(deltaSeconds);
}

void GameEngine::ResizedApp(int32 width, int32 height)
{
	if (width == 0 || height == 0)
	{
		return;
	}

	// On the framework view is resized, wait all graphics commands for
	// synchronize and cleanup resource lock states.
	_primaryQueue->WaitLastSignal();

	_frameworkViewChain->ResizeBuffers(width, height);

	for (int32 i = 0; i < 3; ++i)
	{
		RHITexture2D* texture = _frameworkViewChain->GetBuffer(i);
		_rtv->CreateRenderTargetView(texture, i);
	}

	// Resize depth stencil buffer.
	if (_depthBuffer != nullptr)
	{
		DestroySubobject(_depthBuffer);
	}

	RHITexture2DClearValue clearValue =
	{
		.Format = ERHIPixelFormat::D24_UNORM_S8_UINT,
		.DepthStencil = { 1.0f, 0 }
	};

	_depthBuffer = _device->CreateTexture2D(ERHIResourceStates::DepthWrite, ERHIPixelFormat::D24_UNORM_S8_UINT, width, height, clearValue, ERHIResourceFlags::AllowDepthStencil);
	_dsv->CreateDepthStencilView(_depthBuffer, 0);

	_vpWidth = width;
	_vpHeight = height;

	LogSystem::Log(LogEngine, Info, L"Application resized to {}x{}.", width, height);
}

void GameEngine::GameTick(duration<float> elapsedTime)
{
	// Update input.
	InputComponent::StaticTick(elapsedTime);

	_scheduler.Tick(elapsedTime);
	_gameInstance->Tick(elapsedTime);
}

void GameEngine::RenderTick(duration<float> elapsedTime)
{
	int32 bufferIdx = _frameworkViewChain->GetCurrentBackBufferIndex();

	RHIViewport vp =
	{
		.TopLeftX = 0,
		.TopLeftY = 0,
		.Width = (float)_vpWidth,
		.Height = (float)_vpHeight,
		.MinDepth = 0,
		.MaxDepth = 1.0f
	};

	RHIScissorRect sc =
	{
		.Left = 0,
		.Top = 0,
		.Right = _vpWidth,
		.Bottom = _vpHeight
	};

	RHITransitionBarrier barrierBegin =
	{
		.Resource = _frameworkViewChain->GetBuffer(bufferIdx),
		.StateBefore = ERHIResourceStates::Present,
		.StateAfter = ERHIResourceStates::RenderTarget
	};
	RHITransitionBarrier barrierEnd =
	{
		.Resource = _frameworkViewChain->GetBuffer(bufferIdx),
		.StateBefore = ERHIResourceStates::RenderTarget,
		.StateAfter = ERHIResourceStates::Present
	};

	_deviceContext->Begin();
	_deviceContext->TransitionBarrier(1, &barrierBegin);
	_deviceContext->OMSetRenderTargets(_rtv, bufferIdx, 1, _dsv, 0);
	_deviceContext->ClearRenderTargetView(_rtv, bufferIdx, NamedColors::Transparent);
	_deviceContext->ClearDepthStencilView(_dsv, 0, 1.0f, nullopt);
	_deviceContext->RSSetScissorRects(1, &sc);
	_deviceContext->RSSetViewports(1, &vp);

	World* world = _gameInstance->GetWorld();
	APlayerCameraManager* playerCamera = world->GetPlayerCamera();

	Scene* scene = _gameInstance->GetWorld()->GetScene();
	MinimalViewInfo localPlayerView = playerCamera->GetCachedCameraView();
	localPlayerView.AspectRatio = (float)_vpWidth / (float)_vpHeight;
	scene->InitViews(localPlayerView);
	scene->RenderScene(_deviceContext);

	_deviceContext->TransitionBarrier(1, &barrierEnd);
	_deviceContext->End();

	_primaryQueue->ExecuteDeviceContext(_deviceContext);

	_frameworkViewChain->Present();
	_primaryQueue->Signal();
	_primaryQueue->WaitLastSignal();
}