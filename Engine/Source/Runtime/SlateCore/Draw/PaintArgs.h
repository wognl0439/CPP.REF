// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "RenderMinimal.h"

class SWidget;

class PaintArgs : virtual public Object
{
public:
	using Super = Object;

private:
	const SWidget* _parent = nullptr;
	std::chrono::duration<float> _deltaTime;

public:
	PaintArgs(const SWidget* parentWidget, std::chrono::duration<float> deltaTime);

	PaintArgs* WithNewParent(const SWidget* parentWidget) const;

	const SWidget* GetParentWidget() const { return _parent; }
	std::chrono::duration<float> GetDeltaTime() const { return _deltaTime; }
};