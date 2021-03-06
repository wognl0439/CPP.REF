﻿// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreAPI.h"
#include "CoreConcepts.h"
#include "TNumericsBasicOperator.h"

#include "TRefPtr.h"
#include "Mathematics/TAngleDef.h"

struct Vector3;
struct Vector4;
struct Quaternion;

#pragma pack(push, 4)
struct CORE_API Matrix4x4 : public TNumericsBasicOperator<Matrix4x4, float, float, float, float, float, float, float, float, float, float, float, float, float, float, float, float>
{
    using This = Matrix4x4;

    float _11;
    float _12;
    float _13;
    float _14;
    float _21;
    float _22;
    float _23;
    float _24;
    float _31;
    float _32;
    float _33;
    float _34;
    float _41;
    float _42;
    float _43;
    float _44;

    Matrix4x4();
    Matrix4x4(float _11, float _12, float _13, float _14, float _21, float _22, float _23, float _24, float _31, float _32, float _33, float _34, float _41, float _42, float _43, float _44);
    Matrix4x4(const Vector4& v1, const Vector4& v2, const Vector4& v3, const Vector4& v4);
    Matrix4x4(const Matrix4x4& copy);

    bool Equals(const Matrix4x4& rh) const;
    bool NearlyEquals(const Matrix4x4& rh, float epsilon) const;
    size_t GetHashCode() const;
    TRefPtr<String> ToString() const;

    Vector4 GetComponentOrDefault(size_t index) const;
    template<TIsMatrixType T>
    inline void Construct(const T& matrix);
    template<TIsMatrixType T>
    inline T Cast() const;
    bool Contains(size_t index) const;
    bool Contains(size_t row, size_t column) const;

    vs_property_get(size_t, Count);
    vs_property_get(size_t, Rows);
    vs_property_get(size_t, Columns);

    Matrix4x4& Transpose();
    Matrix4x4& Invert();
    Vector3 TransformVector(const Vector3& vec) const;
    Vector4 TransformVector(const Vector4& vec) const;

    vs_property_get(bool, IsIdentity);
    vs_property_get(float, Determinant);
    vs_property_get(Matrix4x4, Inverse);
    vs_property_get(Matrix4x4, Transposed);

    void BreakTransform(Vector3& outTranslation, Vector3& outScale, Quaternion& outRotation) const;
    vs_property_get(Vector3, Translation);
    vs_property_get(Vector3, Scale);
    vs_property_get(Quaternion, Rotation);

    const Vector4& operator [](size_t index) const;
    Vector4& operator [](size_t index);

    Matrix4x4 operator -() const;
    Matrix4x4 operator +(const Matrix4x4& right) const;
    Matrix4x4 operator -(const Matrix4x4& right) const;
    Matrix4x4 operator *(const Matrix4x4& right) const;
    Matrix4x4 operator /(const Matrix4x4& right) const;

    Matrix4x4& operator +=(const Matrix4x4& right);
    Matrix4x4& operator -=(const Matrix4x4& right);
    Matrix4x4& operator *=(const Matrix4x4& right);
    Matrix4x4& operator /=(const Matrix4x4& right);

    static Matrix4x4 Multiply(const Matrix4x4& a, const Matrix4x4& b);
    static Matrix4x4 LookTo(const Vector3& location, const Vector3& dir, const Vector3& up);
    static Matrix4x4 AffineTransformation(const Vector3& t, const Vector3& s, const Quaternion& q);
    static Matrix4x4 PerspectiveFovLH(float fov, float aspectRatio, float nearZ, float farZ);
    static Matrix4x4 OrthographicLH(float inWidth, float inHeight, float nearZ, float farZ);
    static Matrix4x4 RotationX(TRadians<float> angle);
    static Matrix4x4 RotationY(TRadians<float> angle);
    static Matrix4x4 RotationZ(TRadians<float> angle);
    static Matrix4x4 RotationNormal(const Vector3& inAxis, TRadians<float> angle);

    static Matrix4x4 Identity;
};
#pragma pack(pop)

#include "Matrix4x4.inl"