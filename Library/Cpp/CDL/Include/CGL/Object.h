// (C) 2009 christian.schladetsch@gmail.com

#ifndef CGL_OBJECT_BASE_H_INCLUDED
#define CGL_OBJECT_BASE_H_INCLUDED

CGL_BEGIN

/// base for all objects in the game
struct ObjectBase : ActiveObject
{
private:
	Point location;
	float rotation;
	float scale;
	ColorRGB color_rgb;
	Color color;
	float speed;
	float radius;

public:
	ObjectBase() : location(0,00), rotation(0), scale(1), color_rgb(1,1,1), speed(1), radius(10) { }

	// accessors with virtual mutators
	Point GetLocation() const { return location; }
	virtual void SetLocation(Point const &pos) { location = pos; }
	float GetRotation() const { return rotation; }
	virtual void SetRotation(float rot) { rotation = rot; }
	Color GetColor() const { return color; }
	virtual void SetColor(Color col) { color = col; }
	float GetScale() const { return scale; }
	virtual void SetScale(float S) { scale = S; }
	float GetRadius() const { return radius; }
	virtual void SetRadius(float S) { radius = S; }
	float GetSpeed() const { return speed; }
	virtual void SetSpeed(float S) { speed = S; }
};

CGL_TRAITS_ABSTRACT(ObjectBase, CGL_TypeNumbers::ObjectBase);

/// a game object is reflected and has a common base structure
template <class T>
struct GameObject : Reflected<T>, ObjectBase
{
};

CGL_END

#endif // CGL_OBJECT_BASE_H_INCLUDED

//EOF
