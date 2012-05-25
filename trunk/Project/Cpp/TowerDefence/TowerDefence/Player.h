// (C) 2009 christian.schladetsch@gmail.com

#ifndef TD_PLAYER_H
#	define TD_PLAYER_H

/// the player of the game; deals with the cursor and tower placement/upgrades
struct Player : GameObject<Player>
{
	void Update(GameTime);
	void Draw(Matrix const &);
};

TD_GAME_OBJECT(Player);

#endif // TD_PLAYER_H

//EOF
