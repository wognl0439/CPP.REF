// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "Object.h"
#include <filesystem>

class SGameInstance;

class CORE_API SPlatformModule : virtual public SObject
{
public:
	using Super = SObject;

private:
	void* _NativeHandle = nullptr;

public:
	SPlatformModule(const std::filesystem::path& modulePath);
	virtual ~SPlatformModule() override;

	bool IsValid() const;

	template<class TFunction>
	TFunction* GetFunctionPointer(std::string_view functionName) const
	{
		return reinterpret_cast<TFunction*>(InternalGetFunctionPointer(functionName));
	}

	template<class TRet, class... TArgs>
	TRet(*GetFunctionPointer(std::string_view functionName) const)(TArgs...)
	{
		return reinterpret_cast<TRet(*)(TArgs...)>(InternalGetFunctionPointer(functionName));
	}

private:
	void(*InternalGetFunctionPointer(std::string_view functionName) const)();
};