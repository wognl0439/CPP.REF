﻿// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "RHI/IRHIDeviceBundle.h"

#include "RHI/IRHIShaderResourceView.h"
#include "RHI/IRHIResource.h"

TRefPtr<IRHIShaderResourceView> IRHIDeviceBundle::CreateTextureGroupView(std::span<IRHIResource* const> inResources)
{
	return CreateTextureGroupView(inResources.data(), inResources.size());
}

TRefPtr<IRHIResource> IRHIDeviceBundle::CreateVertexBuffer(std::span<const RHIVertex> vertices)
{
	return CreateVertexBuffer(vertices.data(), vertices.size());
}

TRefPtr<IRHIResource> IRHIDeviceBundle::CreateIndexBuffer(std::span<const uint32> indices)
{
	return CreateIndexBuffer(indices.data(), indices.size());
}

void IRHIDeviceBundle::UpdateTextureGroupView(IRHIShaderResourceView* inView, std::span<IRHIResource* const> inResources)
{
	UpdateTextureGroupView(inView, inResources.data(), inResources.size());
}