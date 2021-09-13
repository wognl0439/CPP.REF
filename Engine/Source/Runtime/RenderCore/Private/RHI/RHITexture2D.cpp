// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "pch.h"
#include "RHI/RHITexture2D.h"

SRHITexture2D::SRHITexture2D(SRHIDevice* device, ID3D12Resource* resource) : Super(device, resource)
{
}

SRHITexture2D::~SRHITexture2D()
{
}

void SRHITexture2D::GetPixelSize(int32* pWidth, int32* pHeight)
{
	D3D12_RESOURCE_DESC desc = GetResource()->GetDesc();
	if (pWidth != nullptr)
	{
		*pWidth = (int32)desc.Width;
	}
	if (pHeight != nullptr)
	{
		*pHeight = (int32)desc.Height;
	}
}