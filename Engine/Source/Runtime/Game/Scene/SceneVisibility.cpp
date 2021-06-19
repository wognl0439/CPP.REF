// Copyright 2020-2021 Aumoa.lib. All right reserved.

import std.core;
import SC.Runtime.Core;
import SC.Runtime.Game;
import SC.Runtime.RenderCore;

using namespace std;
using namespace std::chrono;

SceneVisibility::SceneVisibility(Scene* owner) : Super()
	, _scene(owner)
{
}

void SceneVisibility::CalcVisibility(const MinimalViewInfo& view)
{
	_visibilityCnt = 0;
	FrustumCull(_visibilityBits);

	ForEachVisibleItem([this](size_t idx)
	{
		_visibilityCnt += 1;
	});

	ReadyBuffer(_visibilityCnt, false);

	constexpr Vector3 up = Vector3(0, 1, 0);
	constexpr Vector3 forward = Vector3(0, 0, 1);
	Matrix4x4 viewMatrix = Matrix4x4::LookToLH(view.Location, view.Rotation.RotateVector(forward), view.Rotation.RotateVector(up));
	constexpr Radians angle = Degrees(45.0f).ToRadians();
	Matrix4x4 projMatrix = Matrix4x4::PerspectiveFovLH(angle, view.AspectRatio, 0.1f, 1000.0f);
	Matrix4x4 vp = Matrix4x4::Multiply(viewMatrix, projMatrix);

	// Push camera constants.
	auto* ptr = (RHIViewConstants*)_viewBuffer->GetMappingPointer();
	ForEachVisibleItem([this, &ptr, &vp](size_t idx)
	{
		Transform transform = _scene->_primitives[idx]->ComponentTransform;
		ptr->World = transform.GetMatrix();
		ptr->WorldViewProj = Matrix4x4::Multiply(ptr->World, vp);
		++ptr;
	});
}

void SceneVisibility::SetupView(RHIDeviceContext* dc, RHIShader* shader, size_t idx)
{
	vector<RHIShaderParameterElement> elements = shader->GetShaderParameterDeclaration();
	uint64 bufferLocation = _viewBuffer->GetGPUVirtualAddress() + sizeof(RHIViewConstants) * idx;
	
	for (size_t i = 0; i < elements.size(); ++i)
	{
		switch (elements[i].Type)
		{
		case ERHIShaderParameterType::ParameterCollection_CameraConstants:
			dc->SetGraphicsRootConstantBufferView((uint32)i, bufferLocation);
			break;
		}
	}
}

void SceneVisibility::FrustumCull(vector<int32>& bits)
{
	size_t cnt = _scene->_primitives.size();
	bits.resize((cnt / 8) + 1);

	for (size_t i = 0; i < bits.size(); ++i)
	{
		int32& n = bits[i];
		int32 f = 1;
		for (size_t j = 0; j < 32; ++j)
		{
			size_t primitiveIndex = i * 32 + j;
			if (primitiveIndex >= _scene->_primitives.size())
			{
				break;
			}

			PrimitiveSceneProxy* proxy = _scene->_primitives[primitiveIndex];

			// Simply implementation. If proxy is not null, the primitive is visible.
			if (proxy != nullptr)
			{
				n |= f;
			}

			f <<= 1;
		}
	}
}

void SceneVisibility::ReadyBuffer(size_t capa, bool bAllowShrink)
{
	RHIDevice* dev = _scene->GetDevice();
	size_t prev = sizeof(RHIViewConstants) * _viewBufCapa;
	size_t next = sizeof(RHIViewConstants) * capa;

	if (prev >= next)
	{
		// The view buffer not be allowed to shrink. Return immediately.
		return;
	}

	if (_viewBuffer != nullptr)
	{
		DestroySubobject(_viewBuffer);
	}

	_viewBuffer = dev->CreateDynamicBuffer(next);
}