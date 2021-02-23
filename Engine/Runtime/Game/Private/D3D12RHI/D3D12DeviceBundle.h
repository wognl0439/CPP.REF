﻿// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"
#include "RHI/IRHIDeviceBundle.h"

#include <span>
#include "D3D12Minimal.h"
#include "RHI/RHIResourceFlags.h"

class D3D12SwapChain;
class D3D12ImmediateCommandList;
class D3D12OfflineDescriptorManager;
class D3D12OnlineDescriptorManager;
class D3D12DynamicBufferManager;

class D3D12DeviceBundle : virtual public Object, virtual public IRHIDeviceBundle
{
public:
	using Super = Object;
	using This = D3D12DeviceBundle;

private:
	static D3D12DeviceBundle* instance;

	ComPtr<IDXGIFactory2> dxgiFactory;

	ComPtr<IDXGIAdapter1> primaryAdapter;
	ComPtr<ID3D12Device5> d3d12Device;

	TRefPtr<D3D12SwapChain> swapChain;
	TRefPtr<D3D12ImmediateCommandList> immediateCommandList;
	TRefPtr<D3D12OfflineDescriptorManager> rtvManager;
	TRefPtr<D3D12OfflineDescriptorManager> dsvManager;
	TRefPtr<D3D12OfflineDescriptorManager> srvManager;

	TRefPtr<RHIShaderLibrary> shaderLibrary;
	TRefPtr<RHIResourceGC> resourceGC;

	std::map<size_t, TRefPtr<D3D12DynamicBufferManager>> bufferManager;

public:
	D3D12DeviceBundle();
	~D3D12DeviceBundle() override;

	virtual void InitializeBundle();
	virtual void ReleaseBundle();
	virtual void Commit(IRHICommandList* inCommandList);

	virtual IRHISwapChain* GetSwapChain() const;
	virtual IRHIImmediateCommandList* GetImmediateCommandList() const;
	virtual RHIShaderLibrary* GetShaderLibrary() const;
	virtual RHIResourceGC* GetResourceGC() const;

	virtual TRefPtr<IRHICommandFence> CreateCommandFence();
	virtual TRefPtr<IRHIRenderTargetView> CreateRenderTargetView(IRHIResource* resource);
	virtual TRefPtr<IRHIDepthStencilView> CreateDepthStencilView(IRHIResource* resource, ERHITextureFormat inViewFormat);
	virtual TRefPtr<IRHIShaderResourceView> CreateTextureView(IRHIResource* resource, ERHITextureFormat inViewFormat);
	virtual TRefPtr<IRHIShaderResourceView> CreateTextureGroupView(IRHIResource* const* inResources, size_t count);
	virtual TRefPtr<IRHIDeferredCommandList> CreateDeferredCommandList();
	virtual TRefPtr<IRHIFence> CreateFence();
	virtual TRefPtr<IRHIOnlineDescriptorPatch> CreateOnlineDescriptorPatch();
	virtual TRefPtr<IRHIShaderBindingTable> CreateShaderBindingTable(IRHIShader* inShader);

	virtual TRefPtr<IRHIRenderTarget> CreateGBufferRenderTarget();  // D3D12GBufferRenderTarget.cpp
	virtual TRefPtr<IRHIRenderTarget> CreateHDRRenderTarget();  // D3D12HDRRenderTarget.cpp
	virtual TRefPtr<IRHIResource> CreateVertexBuffer(const RHIVertex* vertices, size_t count);
	virtual TRefPtr<IRHIResource> CreateIndexBuffer(const uint32* indices, size_t count);
	virtual TRefPtr<IRHIResource> CreateDynamicBuffer(size_t sizeInBytes);
	virtual TRefPtr<IRHIResource> CreateImmutableBuffer(size_t sizeInBytes, ERHIResourceStates initialState);
	virtual TRefPtr<IRHIResource> CreateTexture2D(ERHITextureFormat format, int32 width, int32 height, ERHIResourceStates initialStates, ERHIResourceFlags flags, const RHITextureClearValue& inClearValue);
	virtual TRefPtr<IRHIResource> CreateTexture2D(ERHITextureFormat format, PlatformImage* platformImage);

	virtual void UpdateTextureGroupView(IRHIShaderResourceView* inView, IRHIResource* const* inResources, size_t count);

	vs_property_get_auto(ID3D12Device5*, Device, d3d12Device.Get());

private:
	void InitializeCOM();
	void InitializeDXGI();
	void InitializeD3D12();
	void InitializeShaders();

	bool IsAdapterSuitable(IDXGIAdapter1* adapter) const;
	bool IsDeviceSuitable(ID3D12Device* device) const;
	
	ComPtr<ID3D12Resource> CreateImmutableBuffer(D3D12_RESOURCE_STATES initialState, const uint8* initialBuffer, size_t sizeInBytes, ERHIResourceFlags flags = ERHIResourceFlags::None);

	// CALLBACK HANDLERS
	void Application_OnSizing(int32 width, int32 height);
};