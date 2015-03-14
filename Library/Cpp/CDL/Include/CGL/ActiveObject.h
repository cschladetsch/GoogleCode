// (C) 2009 christian.schladetsch@gmail.com

#ifndef CGL_ACTIVE_OBJECT_H_INCLUDED
#define CGL_ACTIVE_OBJECT_H_INCLUDED

struct BaseBase
{
	virtual ~BaseBase() { }
};

CGL_TRAITS(BaseBase, CGL_TypeNumbers::Base);

/// An ActiveObject is a dynamic part of the system; it has an update and draw method
struct ActiveObject : BaseBase
{
protected:
	friend struct Game;
	Game *game;

public:
	ActiveObject() : game(0) { }

	Game *GetGame() const { return game; }

	/// invoked after the objects game pointer has been set
	virtual void Prepare() { }

	virtual void Update(GameTime) = 0;
	virtual void Draw(Matrix const &) = 0;

	/// make a new object, using the same factory that made this object
	template <class T>
	Pointer<T> NewGameObject()
	{
		Pointer<T> object = game->Self->template New<T>();
		object->game = game;
		object->Prepare();
		return object;
	}
};

#endif // CGL_ACTIVE_OBJECT_H_INCLUDED

//EOF
