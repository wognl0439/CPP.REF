// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Bishop.h"
#include "GameEngine.h"
#include "Assets/AssetImporter.h"
#include "Queries/DirectionalMovablePiece.h"

ABishop::ABishop() : Super()
{
}

bool ABishop::QueryMovable(MovablePointsQuery& query, const ChessBoardBuilt& built) const
{
	return DirectionalMovablePiece::QueryMovable(this, query, built, false, true, 8);
}

SStaticMesh* ABishop::GetStaticMesh() const
{
	static SStaticMesh* globalMesh = SGameEngine::GetEngine()->GetAssetImporter()->ImportStaticMesh(AssetPath);
	return globalMesh;
}