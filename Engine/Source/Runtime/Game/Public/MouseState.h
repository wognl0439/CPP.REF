// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameEnums.h"

struct MouseState
{
    uint8 bLeftButton : 1;
    uint8 bMiddleButton : 1;
    uint8 bRightButton : 1;
    uint8 bXButton1 : 1;
    uint8 bXButton2 : 1;
    int32 X;
    int32 Y;
    int32 ScrollWheelValue;
    EMousePositionMode Mode;
};