// (C) 2009 christian.schladetsch@gmail.com

#ifndef TD_WORLD_H
#	define TD_WORLD_H

/// the game world, storing all the game objects
struct World : GameObject<World>
{
	typedef std::list<Pointer<ObjectBase> > Objects;

private:
	Objects objects;

public:
	~World();

	void Update(GameTime);
	void Draw(Matrix const &);

	void AddObject(Object);

	template <class T>
	std::vector<Pointer<T> > GetObjectsOfType() const
	{
		std::vector<Pointer<T> > result;
		foreach (Object object, objects)
		{
			if (object.IsType<T>() || object.IsConvertible<T>())
				result.push_back(object);
		}
		return result;
	}

	template <class T>
	void ForEach(void (T::*method)())
	{
		foreach (Object object, objects)
		{
			if (object.IsConvertible<T>())
			{
				Pointer<T> typed = object;
				((*typed).*method)();
			}
		}
	}

	template <class T, class A0>
	void ForEach(void (T::*method)(A0 a0))
	{
		foreach (Object object, objects)
		{
			if (object.IsConvertible<T>())
			{
				Pointer<T> typed = object;
				((*typed).*method)(a0);
			}
		}
	}
};

CGL_TRAITS_DERIVED_REFLECTED(World, CGL_TypeNumbers::World, ObjectBase);

#endif // TD_WORLD_H

//EOF
