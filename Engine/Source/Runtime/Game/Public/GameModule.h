// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

class SGameInstance;
class SGameEngine;

class GAME_API SGameModule : virtual public SObject
{
	GENERATED_BODY(SGameModule)

public:
	SGameModule();
	virtual ~SGameModule() override;

	virtual SGameInstance* CreatePrimaryGameModule();
	virtual SGameEngine* CreateGameEngine();
};

#define DEFINE_GAME_MODULE(GameModuleClass)		\
extern "C"										\
{												\
	__declspec(dllexport)						\
	SGameModule* LoadGameModule()				\
	{											\
		return new GameModuleClass();			\
	}											\
}