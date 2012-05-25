// (C) 2009 christian.schladetsch.net

#include "CGL/Precompiled.h"

CGL_BEGIN

Factory::Factory() : next_handle(0)
{
}

Factory::~Factory()
{
	Purge();
}

Object *Factory::NewFromClass(ClassBase *klass)
{
	assert(klass != 0);
	if (!klass)
		return 0;
	Object *base = klass->Create(++next_handle);
	base->Construct();
	instances[base->handle] = base;
	return base;
}

/// make a new object given its type number
void Factory::New(TypeNumber N)
{
	Classes::const_iterator klass = classes.find(N);
	assert(klass != classes.end());
	if (klass != classes.end())
		NewFromClass(klass->second);
}

/// delete an existing object
void Factory::Delete(Object *base)
{
	assert(base != 0);
	if (base)
		Delete(base->GetHandle());
}

void Factory::Delete(Handle handle)
{
	Instances::const_iterator obj = instances.find(handle);
	if (obj != instances.end())
	{
		instances.erase(obj);
		deathrow.insert(handle);
	}
}

Object *Factory::GetObject(Handle handle) const
{
	Instances::const_iterator instance = instances.find(handle);
	return instance == instances.end() ? 0 : instance->second;
}

bool Factory::Exists(Object const &base) const
{
	return Exists(base.GetHandle());
}

bool Factory::Exists(Handle handle) const
{
	if (deathrow.find(handle) != deathrow.end())
		return false;
	return instances.find(handle) != instances.end();
}

void Factory::Purge()
{
	// make a copy of deathrow, since as we delete objects they may
	// in turn delete other objects, which would change deathrow
	std::vector<Handle> cp(deathrow.begin(), deathrow.end());
	deathrow.clear();

	foreach (Handle handle, cp)
	{
		Object *base = GetObject(handle);
		if (base)
		{
			std::cout << "deleting object at " << base << ", type " << base->GetTypeNumber() << "\n";
			delete base;
		}
	}
}

CGL_END

//EOF
