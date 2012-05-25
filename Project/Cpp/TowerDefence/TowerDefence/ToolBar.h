// (C) 2009 christian.schladetsch@gmail.com

#ifndef TD_TOOLBAR_H
#	define TD_TOOLBAR_H

struct ToolBar : GameObject<ToolBar>
{
	void Update(GameTime);
	void Draw(Matrix const &);
};

TD_GAME_OBJECT(ToolBar);

#endif // TD_TOOLBAR_H

//EOF
