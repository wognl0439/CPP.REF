// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

interface IRHIBuffer;
interface IRHIDeviceContext;
class SScene;
struct SceneViewScope;

class SCENERENDER_API SceneView
{
	SScene* _MyScene = nullptr;
	IRHIBuffer* _ViewBuffer = nullptr;
	size_t _CurrentViewSize = 0;

private:
	struct ViewInfo
	{
		Matrix4x4 World;
		Matrix4x4 WorldViewProj;
	};

	std::vector<ViewInfo> Views;

public:
	struct PrimitiveViewInfo
	{
		int64 PrimitiveId;
	};

	std::vector<PrimitiveViewInfo> ViewIndexes;

public:
	SceneView(SScene* InScene);

	void InitViews(IRHIDeviceContext* Context, const SceneViewScope& InViewScope);
	SScene* GetScene() const;
	uint64 GetStructuredBufferViewAddress() const;

	void AdvanceViewAddress(uint64& AddrBefore, size_t InAdvance) const
	{
		AddrBefore += sizeof(ViewInfo) * (uint64)InAdvance;
	}
};