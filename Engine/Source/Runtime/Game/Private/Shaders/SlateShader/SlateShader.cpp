// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "pch.h"
#include "Shaders/SlateShader/SlateShader.h"
#include "Materials/Material.h"
#include "SlateMinimal.h"
#include "Draw/SlateWindowElementList.h"

#ifndef BYTE
#define BYTE uint8
#endif

#include "SlateShaderVS.hlsl.h"
#include "SlateShaderPS.hlsl.h"

SlateShader::SlateShader(RHIDevice* device) : Super(device)
{
	class SlateShaderMaterial : public Material
	{
	public:
		using Super = Material;

	public:
		SlateShaderMaterial(SlateShader* shader) : Super(shader)
		{
		}

		virtual int32 GetRootParameterMappingIndex(std::wstring_view parameterName) const override
		{
			if (parameterName == L"Color")
			{
				return 1;
			}
			else
			{
				return Super::GetRootParameterMappingIndex(parameterName);
			}
		}
	};

	_material = CreateSubobject<SlateShaderMaterial>(this);
}

std::span<uint8 const> SlateShader::CompileVS()
{
	return pSlateShaderVS;
}

std::span<uint8 const> SlateShader::CompilePS()
{
	return pSlateShaderPS;
}

Material* SlateShader::GetDefaultMaterial() const
{
	return _material;
}

auto SlateShader::MakeElement(const SlateRenderTransform& geometry, const Vector2& localSize, float depth) const -> DrawElement
{
	DrawElement drawElement;
	drawElement.M = geometry.GetMatrix();
	drawElement.AbsolutePosition = geometry.GetTranslation();
	drawElement.AbsoluteSize = localSize;
	drawElement.Depth = depth;
	return drawElement;
}

void SlateShader::RenderElements(RHIDeviceContext* deviceContext, const Vector2& screenSize, SlateWindowElementList* elements)
{
	uint64 gpuAddr = elements->ApplyAndCreateBuffer(deviceContext, this);
	deviceContext->SetGraphicsRoot32BitConstants(0, 2, &screenSize, 0);
	deviceContext->SetGraphicsRootShaderResourceView(1, gpuAddr);
	deviceContext->DrawInstanced(4, 1);
}

std::vector<RHIShaderParameterElement> SlateShader::GetShaderParameterDeclaration() const
{
	std::vector<RHIShaderParameterElement> elements;

	// [0] SlateConstants
	elements.emplace_back() =
	{
		.Type = ERHIShaderParameterType::ScalarParameterConstants,
		.ScalarConstantsParameter =
		{
			.ShaderRegister = 0,
			.Num32Bits = 2,
		}
	};

	// [1] SlateElements
	elements.emplace_back() =
	{
		.Type = ERHIShaderParameterType::StructuredBuffer,
		.StructuredBuffer =
		{
			.ShaderRegister = 0,
		}
	};

	return elements;
}