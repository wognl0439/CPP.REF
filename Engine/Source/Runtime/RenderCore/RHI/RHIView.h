// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "RHIDeviceChild.h"

/// <summary>
/// Represents resource view for binding to gpu context.
/// </summary>
class RHIView : public RHIDeviceChild
{
public:
	using Super = RHIDeviceChild;

public:
	RHIView(RHIDevice* device);
};