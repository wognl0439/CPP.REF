// Copyright 2020-2021 Aumoa.lib. All right reserved.

import SC.Runtime.Game;

PrimitiveSceneProxy::PrimitiveSceneProxy(PrimitiveComponent* inComponent) : Super()
	, _MyComponent(inComponent)
{
}

void PrimitiveSceneProxy::UpdateTransform_GameThread(const Transform& value)
{
	ComponentTransform = value;
}