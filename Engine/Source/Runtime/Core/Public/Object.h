// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include <set>
#include <string>
#include <string_view>
#include <functional>
#include "PrimitiveTypes.h"
#include "Reflection/ReflectionMacros.h"

/// <summary>
/// Supports all classes in the smart component hierarchy and provides low-level services to derived classes.
/// Represents unit that subobjects are binding and managed.
/// </summary>
class CORE_API SObject : public std::enable_shared_from_this<SObject>
{
	GENERATED_BODY(SObject)

private:
	SObject* _outer = nullptr;
	std::set<std::shared_ptr<SObject>> _subobjects;

public:
	/// <summary>
	/// Initialize new <see cref="SObject"/> class instance.
	/// </summary>
	SObject();
	virtual ~SObject() noexcept;

private:
	SObject(const SObject&) = delete;
	SObject(SObject&&) = delete;

public:
	/// <summary>
	/// Get a simple string representing this object.
	/// </summary>
	/// <param name="formatArgs"> The formatting args that use to std::format. </param>
	/// <returns> The string view instance. </returns>
	virtual std::wstring ToString(std::wstring_view formatArgs = L"") const;
	SFUNCTION(ToString);

	/// <summary>
	/// Create object which linked outer to this.
	/// </summary>
	/// <typeparam name="T"> Type of subobject. </typeparam>
	/// <typeparam name="...TArgs"> The type sequence of constructor arguments. </typeparam>
	/// <param name="...args"> The constructor arguments. </param>
	/// <returns> The instantiated pointer. </returns>
	template<class T, class... TArgs> requires std::constructible_from<T, TArgs...>
	T* NewObject(TArgs&&... args)
	{
		std::shared_ptr shared = std::make_shared<T>(std::forward<TArgs>(args)...);
		auto ptr = shared.get();
		_subobjects.emplace(std::move(shared));
		ptr->_outer = this;
		return ptr;
	}

	/// <summary>
	/// Create object without outer.
	/// </summary>
	/// <typeparam name="T"> Type of subobject. </typeparam>
	/// <typeparam name="...TArgs"> The type sequence of constructor arguments. </typeparam>
	/// <param name="...args"> The constructor arguments. </param>
	/// <returns> The instantiated pointer. </returns>
	template<class T, class... TArgs> requires std::constructible_from<T, TArgs...>
	static T* NewStaticObject(TArgs&&... args)
	{
		T* ptr = new T(std::forward<TArgs>(args)...);
		return ptr;
	}

	/// <summary>
	/// Get outer that owner of this object.
	/// </summary>
	/// <returns> The outer object. </returns>
	SObject* GetOuter() const;
	SFUNCTION(GetOuter);

	/// <summary>
	/// Change outer. SObject will destroy when outer be destroyed.
	/// </summary>
	/// <param name="newOuter"> The new outer. </param>
	/// <returns> The object pointer if attachment is failed. </returns>
	std::shared_ptr<SObject> SetOuter(SObject* newOuter);
	SFUNCTION(SetOuter);

	/// <summary>
	/// Destroy subobject.
	/// </summary>
	/// <param name="subobject"> The target object. </param>
	static void DestroySubobject(SObject* subobject);

	template<std::derived_from<SObject> T>
	T* As() { return dynamic_cast<T*>(this); }
	template<std::derived_from<SObject> T>
	const T* As() const { return dynamic_cast<const T*>(this); }

private:
	std::shared_ptr<SObject> InternalDetachSubobject(std::shared_ptr<SObject> subobject);
	void InternalAttachSubobject(std::shared_ptr<SObject> subobject);
	void InternalDestroySubobject(SObject* subobject);
};

#define implements virtual public 