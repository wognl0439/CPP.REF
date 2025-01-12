// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Rook.h"
#include "GameEngine.h"
#include "Assets/AssetImporter.h"
#include "Queries/DirectionalMovablePiece.h"

ARook::ARook() : Super()
{
}

bool ARook::QueryMovable(MovablePointsQuery& query, const ChessBoardBuilt& built) const
{
	return DirectionalMovablePiece::QueryMovable(this, query, built, true, false, 8);
}

StaticMesh* ARook::GetStaticMesh() const
{
	static StaticMesh* globalMesh = GameEngine::GetEngine()->GetAssetImporter()->ImportStaticMesh(AssetPath);
	return globalMesh;
}