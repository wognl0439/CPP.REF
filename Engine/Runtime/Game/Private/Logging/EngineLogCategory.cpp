﻿// Copyright 2020 Aumoa.lib. All right reserved.

#include "EngineLogCategory.h"

#include "Logging/LogVerbosity.h"

LogCategoryBase LogCamera(ELogVerbosity::Verbose, nameof(LogCamera));
LogCategoryBase LogEngine(ELogVerbosity::Verbose, nameof(LogEngine));
LogCategoryBase LogActor(ELogVerbosity::Verbose, nameof(LogActor));
LogCategoryBase LogDiagnostics(ELogVerbosity::Verbose, nameof(LogDiagnostics));
LogCategoryBase LogRendering(ELogVerbosity::Verbose, nameof(LogRendering));
LogCategoryBase LogPlatform(ELogVerbosity::Verbose, nameof(LogPlatform));
LogCategoryBase LogInput(ELogVerbosity::Verbose, nameof(LogInput));
LogCategoryBase LogPawn(ELogVerbosity::Verbose, nameof(LogPawn));