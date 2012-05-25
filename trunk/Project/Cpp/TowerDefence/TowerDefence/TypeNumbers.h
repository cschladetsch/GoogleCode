// (C) 2009 christian.schladetsch@gmail.com

#ifndef TD_TYPE_NUMBERS_H
#	define TD_TYPE_NUMBERS_H

/// the first 100 type-numbers are reserved by the CGL library
#	define TD_TYPE_NUMBER(T, N) \
		T = CGL_TYPE_NUMBER_LAST + N

/// type-numbers used for game object types
struct TypeNumbers
{
	enum
	{
		TD_TYPE_NUMBER(SplashScreenPhase, 0),
		TD_TYPE_NUMBER(PlayPhase, 1),
		TD_TYPE_NUMBER(Runner, 2),
		TD_TYPE_NUMBER(Tower, 3),
		TD_TYPE_NUMBER(ToolBar, 4),
		TD_TYPE_NUMBER(World, 5),
		TD_TYPE_NUMBER(Player, 6),
		TD_TYPE_NUMBER(Bullet, 7),
		TD_TYPE_NUMBER(Empty, 8),
	};
};

/// define traits for a TowerDefence type
#	define TD_TRAITS(T) \
		CGL_TRAITS(T, TypeNumbers::T)

#	define TD_GAME_OBJECT(T) \
		CGL_TRAITS_DERIVED_REFLECTED(T, TypeNumbers::T, ObjectBase)

#endif // TD_TYPE_NUMBERS_H

//EOF
