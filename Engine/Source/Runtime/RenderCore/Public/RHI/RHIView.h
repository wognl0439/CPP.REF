// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "RHIDeviceChild.h"

class SRHIShaderDescriptorView;

/// <summary>
/// Represents resource view for binding to gpu context.
/// </summary>
class RENDERCORE_API SRHIView : public SRHIDeviceChild
{
	GENERATED_BODY(SRHIView)

public:
	SRHIView(SRHIDevice* device);
};