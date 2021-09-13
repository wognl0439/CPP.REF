// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include <vector>
#include "GameStructures.h"

class SPrimitiveComponent;

class GAME_API SPrimitiveSceneProxy : virtual public SObject
{
	GENERATED_BODY(SPrimitiveSceneProxy)

private:
	SPrimitiveComponent* _MyComponent = nullptr;

public:
	SPrimitiveSceneProxy(SPrimitiveComponent* inComponent);

	SPrimitiveComponent* GetComponent() const { return _MyComponent; }
	void UpdateTransform_GameThread(const Transform& value);

	std::vector<MeshBatch> MeshBatches;
	int64 PrimitiveId = -1;
	Transform ComponentTransform;
	uint8 bRenderStateDirty : 1 = false;

private:
	uint8 _bHiddenInGame : 1 = false;

public:
	virtual void MarkRenderStateDirty_GameThread();
	virtual void SetHiddenInGame_GameThread(bool bHiddenInGame);
	bool IsHiddenInGame() const { return _bHiddenInGame; }
};