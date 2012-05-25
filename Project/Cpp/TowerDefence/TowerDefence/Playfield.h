// (C) 2009 christian.schladetsch@gmail.com

#ifndef TD_PLAYFIELD_H
#define TD_PLAYFIELD_H

/// a 2d map of the game-world
struct Playfield
{
	typedef std::vector<TypeNumber> Map;

private:
	int x_dim, y_dim;
	Map map;

public:
	void Initialise(int x_dim, int y_dim);

	void Clear();
	void Set(int x, int y, TypeNumber);
	TypeNumber Get(int x, int y) const;
	bool OutOfBounds(int x, int y) const;

	bool IsType(int x, int y, TypeNumber num) const { return Get(x,y) == num; }
};

#endif // TD_PLAYFIELD_H

//EOF
