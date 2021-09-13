// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameMinimal.h"
#include "IFrameworkView.h"

/// <summary>
/// Represents the single window with input events and basic user interface behaviors.
/// </summary>
class SCoreWindow : virtual public SObject, virtual public IFrameworkView
{
	GENERATED_BODY(SCoreWindow)

public:
	static constexpr wchar_t ApplicationTitle[] = L"GameApp";

private:
	void* _hwnd = nullptr;
	uint8 _bMainLoop : 1 = false;
	int32 _lastError = 0;

public:
	/// <summary>
	/// Initialize new <see cref="SCoreWindow"/> instance.
	/// </summary>
	SCoreWindow();

	/// <inheritdoc/>
	virtual void Start() override;

	/// <inheritdoc/>
	virtual void* GetWindowHandle() const override;

	/// <inheritdoc/>
	virtual int32 GetLastError() const override;

	/// <inheritdoc/>
	virtual void SetLastError(int32 code) override;

	/// <inheritdoc/>
	virtual int32 GetFrameworkWidth() const override;

	/// <inheritdoc/>
	virtual int32 GetFrameworkHeight() const override;

	/// <inheritdoc/>
	virtual void SetFrameworkTitle(const std::wstring& frameworkTitle) override;
};