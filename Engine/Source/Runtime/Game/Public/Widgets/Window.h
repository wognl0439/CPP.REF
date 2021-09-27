// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "RenderMinimal.h"
#include "CompoundWidget.h"
#include "Layout/Geometry.h"

class GAME_API SWindow : public SCompoundWidget
{
	GENERATED_BODY(SWindow)

private:
	Vector2 _localSize;
	std::chrono::duration<float> _lastDeltaTime;
	std::optional<Geometry> _cachedRootGeometry;

	std::vector<SWidget*> _screenWidgets;

public:
	SWindow(const std::wstring& name);

	void ExecuteTick(std::chrono::duration<float> deltaTime);
	void ExecutePaint(SSlateWindowElementList* drawElements) const;
	Geometry MakeRootGeometry() const;

	void SetWindowSize(const Vector2& localSize);

	void AddWidgetToScreen(SWidget* widget);
	SWidget* GetWidgetInScreen(const std::wstring& name) const;
	void RemoveWidgetFromScreen(const std::wstring& name);

	virtual Vector2 GetDesiredSize() const;

protected:
	std::span<SWidget* const> GetWidgets() const { return _screenWidgets; }

protected:
	virtual void OnArrangeChildren(SArrangedChildrens* arrangedChildrens, const Geometry& allottedGeometry) const override;
};