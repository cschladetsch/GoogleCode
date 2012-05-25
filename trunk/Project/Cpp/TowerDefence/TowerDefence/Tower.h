// (C) 2009 christian.schladetsch@gmail.com

#ifndef TD_TOWER_H
#	define TD_TOWER_H

/// a static tower that shoots or otherwise hinders runners
struct Tower : GameObject<Tower>
{
	float firing_rate;		///< maximum firing rate in seconds
	float next_fire_time;	///< time when the tower can fire again
	float range;			///< range of the tower
	float damage;			///< the damange dealt by the bullets that the tower fires

	Tower();

	void Update(GameTime);
	void Draw(Matrix const &);
};

TD_GAME_OBJECT(Tower);

#endif // TD_TOWER_H

//EOF
