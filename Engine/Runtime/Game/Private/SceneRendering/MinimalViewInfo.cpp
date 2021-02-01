﻿// Copyright 2020 Aumoa.lib. All right reserved.

#include "SceneRendering/MinimalViewInfo.h"

MinimalViewInfo::MinimalViewInfo()
	: FOV(0.25f * 3.14f)
	, AspectRatio(1.0f)
	, NearPlane(0.1f)
	, FarPlane(1000.0f)
	, Location(Vector3::Zero)
	, Rotation(Quaternion::Identity)
	, ViewProj(Matrix4x4::Identity)
{

}

void MinimalViewInfo::Apply()
{
	Vector3 forward = Rotation.RotateVector(Vector3::Forward);
	Vector3 up = Rotation.RotateVector(Vector3::Up);
	
	Matrix4x4 lookTo = Matrix4x4::LookTo(Location, forward, up);
	Matrix4x4 proj = Matrix4x4::PerspectiveFovLH(FOV, AspectRatio, NearPlane, FarPlane);
	Matrix4x4 vp = Matrix4x4::Multiply(lookTo, proj);

	ViewProj = vp;
}