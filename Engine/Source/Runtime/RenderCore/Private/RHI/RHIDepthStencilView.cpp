// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "pch.h"
#include "RHI/RHIDepthStencilView.h"
#include "InternalComPtr.h"
#include "RHI/LogRHI.h"
#include "RHI/RHIDevice.h"
#include "RHI/RHITexture2D.h"

SRHIDepthStencilView::SRHIDepthStencilView(SRHIDevice* device, uint32 descriptorCount) : Super(device)
	, _descriptorCount(descriptorCount)
{
	ID3D12Device* dev = device->GetDevice();
	D3D12_DESCRIPTOR_HEAP_DESC heapd =
	{
		.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV,
		.NumDescriptors = descriptorCount,
		.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE,
	};
	HR(LogRHI, dev->CreateDescriptorHeap(&heapd, IID_PPV_ARGS(&_descriptor)));
	_increment = dev->GetDescriptorHandleIncrementSize(heapd.Type);
}

SRHIDepthStencilView::~SRHIDepthStencilView()
{
}

void SRHIDepthStencilView::CreateDepthStencilView(SRHITexture2D* texture, int32 index)
{
	ID3D12Resource* resource = texture->GetResource();
	ID3D12Device* dev = GetDevice()->GetDevice();

	D3D12_CPU_DESCRIPTOR_HANDLE handle = _descriptor->GetCPUDescriptorHandleForHeapStart();
	handle.ptr += _increment * index;

	dev->CreateDepthStencilView(resource, nullptr, handle);
}

D3D12_CPU_DESCRIPTOR_HANDLE SRHIDepthStencilView::GetCPUDescriptorHandle(int32 index) const
{
	D3D12_CPU_DESCRIPTOR_HANDLE handle = _descriptor->GetCPUDescriptorHandleForHeapStart();
	handle.ptr += _increment * index;
	return handle;
}