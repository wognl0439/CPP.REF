// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "pch.h"
#include "RHI/RHITexture.h"

SRHITexture::SRHITexture(SRHIDevice* device, ID3D12Resource* resource) : Super(device, resource)
{
}

SRHITexture::~SRHITexture()
{
}