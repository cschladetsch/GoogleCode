// (C) 2009 christian.schladetsch@gmail.com

#ifndef TD_BULLET_H
#	define TD_BULLET_H

/// moves at very fast speed, causing damage to its target
struct Bullet : GameObject<Bullet>
{
	float damage;			///< how much damage this bullet does
	Vector trajectory;		///< the path the bullet follows
	Time time_to_die;				///< when the bullet will hit
	Pointer<ObjectBase> target;		///< what the bullet will hit

	void Update(GameTime);
	void Draw(Matrix const &);

	void SetTrajectory(Vector v) { trajectory = v; }
	void SetTimeToDie(Time T) { time_to_die = T; }
	void SetDamage(float D) { damage = D; }
	void SetTarget(Object P) { target = P; }
};

TD_GAME_OBJECT(Bullet);

#endif // TD_BULLET_H

//EOF

