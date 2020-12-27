﻿// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "CoreAPI.h"
#include "CoreConcepts.h"

#include <compare>
#include <vector>
#include "TRefPtr.h"
#include "Vector3.h"
#include "Quaternion.h"

namespace SC::Runtime::Core::Numerics
{
#pragma pack(push, 8)
	struct CORE_API ObjectOrientedCube
	{
		using This = ObjectOrientedCube;

		Vector3 Center;
		Vector3 Extent;
		Quaternion Rotation;

		ObjectOrientedCube();
		ObjectOrientedCube(const Vector3& center, const Vector3& extent, const Quaternion& rotation);
		ObjectOrientedCube(const ObjectOrientedCube& copy);

		bool Equals(const ObjectOrientedCube& rh) const;
		bool NearlyEquals(const ObjectOrientedCube& rh, double epsilon) const;
		size_t GetHashCode() const;
		TRefPtr<String> ToString() const;

		bool IsOverlap(const ObjectOrientedCube& cube) const;
		std::vector<Vector3> CalcVertices() const;

		vs_property_get(Vector3, AxisX);
		Vector3 AxisX_get() const;
		vs_property_get(Vector3, AxisY);
		Vector3 AxisY_get() const;
		vs_property_get(Vector3, AxisZ);
		Vector3 AxisZ_get() const;

		bool operator ==(const ObjectOrientedCube& right) const;
		bool operator !=(const ObjectOrientedCube& right) const;
	};
#pragma pack(pop)
}