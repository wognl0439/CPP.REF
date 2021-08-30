// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "pch.h"
#include "Shaders/ColorShader/ColorShader.h"
#include "Materials/Material.h"

#ifndef BYTE
#define BYTE uint8
#endif

#include "ColorShaderVS.hlsl.h"
#include "ColorShaderPS.hlsl.h"

ColorShader::ColorShader(RHIDevice* device) : Super(device)
{
	class ColorShaderMaterial : public Material
	{
	public:
		using Super = Material;

	public:
		ColorShaderMaterial(ColorShader* shader) : Super(shader)
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

	_material = NewObject<ColorShaderMaterial>(this);
}

std::span<uint8 const> ColorShader::CompileVS()
{
	return pColorShaderVS;
}

std::span<uint8 const> ColorShader::CompilePS()
{
	return pColorShaderPS;
}

Material* ColorShader::GetDefaultMaterial() const
{
	return _material;
}

std::vector<RHIShaderParameterElement> ColorShader::GetShaderParameterDeclaration() const
{
	std::vector<RHIShaderParameterElement> elements;

	// [0] Camera constants.
	elements.emplace_back() =
	{
		.Type = ERHIShaderParameterType::ParameterCollection_CameraConstants,
		.ParameterCollection =
		{
			.ShaderRegister = 0
		}
	};

	// [1] gColor
	elements.emplace_back() =
	{
		.Type = ERHIShaderParameterType::ScalarParameterConstants,
		.ScalarConstantsParameter =
		{
			.ShaderRegister = 1,
			.Num32Bits = 3
		}
	};

	return elements;
}