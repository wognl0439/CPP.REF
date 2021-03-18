﻿// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

#pragma pack(push, 1)

struct HomogeneousLight
{
	enum Lights : uint32
	{
		INDEX_NONE,
		INDEX_DIRECTIONAL
	};

	Lights LightIndex;
	float Ambient;
	float Diffuse;
	float Specular;
	Vector3 Color;
	float pad;

	union
	{
		struct
		{
			Vector3 Direction;
			float pad;
		}
		DirectionalLightAtt;
	};

	HomogeneousLight()
		: LightIndex(INDEX_NONE)
		, Ambient(0)
		, Diffuse(0)
		, Specular(0)
		, Color(Vector3::Zero)
	{
	}

	HomogeneousLight(const HomogeneousLight& rhs)
	{
		memcpy(this, &rhs, sizeof(HomogeneousLight));
	}
};

#pragma pack(pop)