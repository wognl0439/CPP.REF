// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameEngineSubsystem.h"

class PlatformModule;
class GameInstance;
class GameModule;

class GAME_API GameModuleSystem : public GameEngineSubsystem
{
	GENERATED_BODY(GameModuleSystem)

private:
	PlatformModule* _module = nullptr;
	GameModule* _gameModule = nullptr;

public:
	GameModuleSystem();
	virtual ~GameModuleSystem() override;
	
	void LoadGameModule(std::wstring_view gameModuleName);
	GameInstance* LoadGameInstance();
};