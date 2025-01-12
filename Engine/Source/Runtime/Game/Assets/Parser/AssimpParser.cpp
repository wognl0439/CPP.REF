// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "AssimpParser.h"
#include "scene.h"
#include "Importer.hpp"
#include "postprocess.h"
#include "LogGame.h"
#include "GameStructures.h"
#include "GameEngine.h"
#include "Assets/StaticMesh.h"
#include "Scene/StaticMeshRenderData.h"
#include "RHI/RHIVertexFactory.h"
#include "RHI/RHIResource.h"

using namespace std;
using namespace std::filesystem;

using enum ELogVerbosity;

class AssimpParser::Impl
{
public:
	unique_ptr<Assimp::Importer> _importer;
	const aiScene* _scene = nullptr;
};

AssimpParser::AssimpParser(GameEngine* engine, RHIVertexFactory* vfactory) : Super(engine, vfactory)
{
	_impl = make_unique<Impl>();
	_impl->_importer = make_unique<Assimp::Importer>();
}

AssimpParser::~AssimpParser()
{
}

bool AssimpParser::TryParse(const path& importPath)
{
	static constexpr int32 ReadOptions =
		aiProcess_JoinIdenticalVertices |
		aiProcess_ImproveCacheLocality |
		aiProcess_RemoveRedundantMaterials |
		aiProcess_GenUVCoords |
		aiProcess_CalcTangentSpace |
		aiProcess_LimitBoneWeights |
		aiProcess_GenNormals |
		aiProcess_Triangulate |
		aiProcess_ConvertToLeftHanded;

	string filepath_mb = importPath.string();
	_impl->_scene = _impl->_importer->ReadFile(filepath_mb.c_str(), ReadOptions);
	if (_impl->_scene == nullptr)
	{
		LogSystem::Log(LogAssets, Error, L"Cannot parsing scene from {0} file with assimp library.", importPath.wstring());
		return false;
	}

	// Save directory name.
	_name = importPath.stem();
	_parent = importPath.parent_path();

	// Check that mesh has any bone.
	bool bSkeletalMesh = false;
	for (uint32 i = 0; i < _impl->_scene->mNumMeshes; ++i)
	{
		if (_impl->_scene->mMeshes[i]->HasBones())
		{
			bSkeletalMesh = true;
			break;
		}
	}

	//if (!ProcessMaterials())
	//{
	//	LogSystem::Log(LogAssets, Error, L"Cannot process materials.");
	//	return false;
	//}

	if (bSkeletalMesh)
	{
		LogSystem::Log(LogAssets, Error, L"SkeletalMesh is not supported yet with assimp library.");
		//return false;
	}
	//else
	{
		if (!ProcessStaticMeshSubsets())
		{
			LogSystem::Log(LogAssets, Error, L"Cannot process static mesh subsets.");
			return false;
		}
	}

	return true;
}

bool AssimpParser::IsStaticMesh() const
{
	return _mesh != nullptr;
}

StaticMesh* AssimpParser::GetStaticMesh() const
{
	return _mesh;
}
//
//bool AssimpParser::ProcessMaterials()
//{
//	DirectXDeviceBundle* deviceBundle = engine->GetDeviceBundle();
//	COMDeviceBundle* comBundle = engine->GetCOMDevice();
//
//	materials.reserve(_impl->_scene->mNumMaterials);
//	for (uint32 i = 0; i < _impl->_scene->mNumMaterials; ++i)
//	{
//		aiMaterial* aiMat = _impl->_scene->mMaterials[i];
//
//		TRefPtr<Material>& mat = materials.emplace_back();
//		mat = NewObject<Material>(engine->GetDeviceBundle());
//
//		auto aiMat_GetColor3D = [aiMat](auto key, auto _1, auto _2, const Vector3& _default = Vector3::Zero) -> Vector3
//		{
//			aiColor3D value;
//			if (aiMat->Get(key, _1, _2, value) != AI_SUCCESS)
//			{
//				return _default;
//			}
//			else
//			{
//				return reinterpret_cast<Vector3&>(value);
//			}
//		};
//
//		auto aiMat_GetFloat = [aiMat](auto key, auto _1, auto _2, float _default = 0) -> float
//		{
//			ai_real value;
//			if (aiMat->Get(key, _1, _2, value) != AI_SUCCESS)
//			{
//				return _default;
//			}
//			else
//			{
//				return value;
//			}
//		};
//
//		auto aiMat_LoadTexture = [&](auto aiTextureType, int32 index) -> TComPtr<ID3D12Resource>
//		{
//			aiString diffuseId;
//			if (aiMat->Get(AI_MATKEY_TEXTURE(aiTextureType, index), diffuseId) != AI_SUCCESS)
//			{
//				return nullptr;
//			}
//
//			TRefPtr<String> fullpath = String::Format(L"{0}/{1}", directoryName, diffuseId.C_Str());
//			if (optional<size_t> index = fullpath->IndexOf(L'*'); index.has_value())
//			{
//				LogSystem::Log(LogAssets, Verbose, L"Filepath include asterlisk detected. Cut path. Original: {0}", fullpath);
//				fullpath = fullpath->Substring(0, index.value());
//			}
//
//			auto imageLoader = NewObject<COMImageLoader>(comBundle, fullpath);
//			if (!imageLoader->IsValid)
//			{
//				LogSystem::Log(LogAssets, Error, L"Imageloader cannot read image file: {0}", fullpath);
//				return nullptr;
//			}
//
//			auto imageFrame = imageLoader->GetFrame(0);
//			auto converter = imageFrame->ConvertFormat(DXGI_FORMAT_R8G8B8A8_UNORM);
//
//			return deviceBundle->CreateTexture2D(converter.Get(), engine->GetPrimaryCommandQueue(), DXGI_FORMAT_R8G8B8A8_UNORM, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
//		};
//
//		// Assign color keys.
//		mat->Ambient = aiMat_GetColor3D(AI_MATKEY_COLOR_AMBIENT);
//		mat->Diffuse = aiMat_GetColor3D(AI_MATKEY_COLOR_DIFFUSE);
//		mat->Specular = aiMat_GetColor3D(AI_MATKEY_COLOR_SPECULAR);
//		mat->SpecExp = aiMat_GetFloat(AI_MATKEY_SHININESS);
//
//		mat->DiffuseMap = aiMat_LoadTexture(aiTextureType_DIFFUSE, 0).Get();
//		mat->NormalMap = aiMat_LoadTexture(aiTextureType_NORMALS, 0).Get();
//	}
//
//	return true;
//}

bool AssimpParser::ProcessStaticMeshSubsets()
{
	// Ready render data.
	StaticMeshRenderData* renderData = CreateSubobject<StaticMeshRenderData>();
	MeshBatch& batch = renderData->MeshBatches.emplace_back();
	batch.VertexFactory = GetVertexFactory();

	vector<RHIVertex>& vertexBuffer = batch.VertexBuffer;
	vector<uint32>& indexBuffer = batch.IndexBuffer;

	// Query size for all containers preallocate.
	size_t vertexCount = 0;
	size_t indexCount = 0;
	size_t subsetCount = 0;
	for (uint32 i = 0; i < _impl->_scene->mNumMeshes; ++i)
	{
		const aiMesh* myMesh = _impl->_scene->mMeshes[i];
		vertexCount += myMesh->mNumVertices;
		indexCount += (size_t)myMesh->mNumFaces * 3;
		subsetCount += 1;
	}

	// Pre-allocate containers.
	vertexBuffer.reserve(vertexCount);
	indexBuffer.reserve(indexCount);
	batch.Elements.reserve((size_t)_impl->_scene->mNumMeshes);
	batch.MaterialSlots.resize((size_t)_impl->_scene->mNumMaterials);

	int32 lastVertexLocation = 0;
	int32 lastIndexLocation = 0;

	// Store all elements for this game engine.
	for (uint32 i = 0; i < _impl->_scene->mNumMeshes; ++i)
	{
		aiMesh* mesh = _impl->_scene->mMeshes[i];

		MeshBatchElement& element = batch.Elements.emplace_back() =
		{
			.IndexCount = (uint32)mesh->mNumFaces * 3,
			.InstanceCount = 1,
			.StartIndexLocation = lastIndexLocation,
			.BaseVertexLocation = lastVertexLocation,
			.VertexCount = (uint32)mesh->mNumVertices
		};

		// Store vertices.
		for (uint32 i = 0; i < mesh->mNumVertices; ++i)
		{
			RHIVertex& myVertex = vertexBuffer.emplace_back();
			memcpy(&myVertex.Position, &mesh->mVertices[i], sizeof(Vector3));
			memcpy(&myVertex.Normal, &mesh->mNormals[i], sizeof(Vector3));
			memcpy(&myVertex.TexCoord, &mesh->mTextureCoords[0][i], sizeof(Vector2));
			if (mesh->HasVertexColors(i))
			{
				memcpy(&myVertex.Color, &mesh->mColors[i], sizeof(Color));
			}
			else
			{
				myVertex.Color = NamedColors::White;
			}
		}

		// Store indices.
		for (uint32 i = 0; i < mesh->mNumFaces; ++i)
		{
			uint32& _1 = indexBuffer.emplace_back();
			indexBuffer.emplace_back();
			indexBuffer.emplace_back();
			memcpy(&_1, mesh->mFaces[i].mIndices, sizeof(uint32) * 3);
		}
		
		element.MaterialSlotIndex = mesh->mMaterialIndex;

		lastVertexLocation = (int32)vertexBuffer.size();
		lastIndexLocation = (int32)indexBuffer.size();
	}

	RHIResource* vb = batch.VertexFactory->CreateVertexBuffer(vertexBuffer.data(), vertexBuffer.size());
	vb->SetOuter(renderData);
	batch.VertexBufferLocation = vb->GetGPUVirtualAddress();

	RHIResource* ib = batch.VertexFactory->CreateIndexBuffer(indexBuffer.data(), indexBuffer.size());
	ib->SetOuter(renderData);
	batch.IndexBufferLocation = ib->GetGPUVirtualAddress();

	_mesh = CreateSubobject<StaticMesh>(_name.wstring(), renderData);
	return true;
}