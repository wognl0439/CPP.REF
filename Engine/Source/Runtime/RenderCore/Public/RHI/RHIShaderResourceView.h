// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "RHIView.h"
#include "ComPtr.h"

struct ID3D12DescriptorHeap;
struct D3D12_CPU_DESCRIPTOR_HANDLE;
class SRHITexture2D;

/// <summary>
/// Represents shader resource view for binding to gpu input.
/// </summary>
class RENDERCORE_API SRHIShaderResourceView : public SRHIView
{
	GENERATED_BODY(SRHIShaderResourceView)

private:
	ComPtr<ID3D12DescriptorHeap> _descriptor;
	const uint32 _descriptorCount = 0;
	uint32 _increment = 0;

public:
	SRHIShaderResourceView(SRHIDevice* device, uint32 descriptorCount);
	~SRHIShaderResourceView() override;

	/// <summary>
	/// Create shader resource view.
	/// </summary>
	void CreateShaderResourceView(SRHITexture2D* texture, int32 index);

	/// <summary>
	/// Get initialized descriptor count.
	/// </summary>
	int32 GetDescriptorCount() const { return _descriptorCount; }

public /*internal*/:
	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandle(int32 index = 0) const;
};