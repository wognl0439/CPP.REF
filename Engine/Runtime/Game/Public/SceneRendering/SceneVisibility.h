﻿// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

#include "Diagnostics/ScopedCycleCounter.h"
#include "SceneRendering/MinimalViewInfo.h"

class PrimitiveSceneProxy;
class Scene;
class ShaderCameraConstantVector;

class GAME_API SceneVisibility : virtual public Object
{
public:
	using Super = Object;
	using This = SceneVisibility;

private:
	Scene* myScene;
	MinimalViewInfo myView;
	std::vector<bool> visibilities;
	TRefPtr<ShaderCameraConstantVector> shaderCameraConstants;
	bool bDirty : 1;

public:
	SceneVisibility(Scene* inScene);
	~SceneVisibility() override;

	void CalcVisibility();
	void UpdateView(const MinimalViewInfo& inView);

	vs_property_get(const std::vector<bool>&, PrimitiveVisibility);
	const std::vector<bool>& PrimitiveVisibility_get() const;
	vs_property_get(ShaderCameraConstantVector*, ShaderCameraConstants);
	ShaderCameraConstantVector* ShaderCameraConstants_get() const;
};