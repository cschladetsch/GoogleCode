// (C) 2009 christian.schladetsch@gmail.com

#include "CGL/Precompiled.h"

CGL_BEGIN

Object::Object(Object const *P) : klass(0), handle(0)
{
	if (!P)
		return;
	klass = P->klass;
	handle = P->handle;
}

void Object::Delete() const
{
	Factory *factory = GetFactory();
	if (!factory)
		return;
	factory->Delete(GetHandle());
}

Object *Object::Deref() const
{
	if (!Exists())
		return 0;
	return GetFactory()->GetObject(GetHandle());
}

Factory *Object::GetFactory() const
{
	return klass ? klass->GetFactory() : 0;
}

bool Exists(Object const *base)
{
	return base && Exists(*base);
}

bool Exists(Object const &base)
{
	return base.GetFactory() && base.GetFactory()->Exists(base.GetHandle());
}

bool Object::Exists() const
{
	return this && ::Exists(*this);
}

TypeNumber Object::GetTypeNumber() const
{
	if (!Exists())
		return 0;
	return GetClass()->GetTypeNumber();
}

TypeNumber Object::GetParentTypeNumber() const
{
	if (!Exists())
		return 0;
	return GetClass()->GetParentTypeNumber();
}

void Delete(Object const &base)
{
	base.Delete();
}

CGL_END

//EOF
