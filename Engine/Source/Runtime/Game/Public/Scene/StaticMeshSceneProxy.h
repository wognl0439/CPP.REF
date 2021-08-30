// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "PrimitiveSceneProxy.h"

class StaticMeshComponent;

class GAME_API StaticMeshSceneProxy : public PrimitiveSceneProxy
{
	CLASS_BODY(StaticMeshSceneProxy)

public:
	StaticMeshSceneProxy(StaticMeshComponent* inComponent, const std::vector<MeshBatch>& batches);
};