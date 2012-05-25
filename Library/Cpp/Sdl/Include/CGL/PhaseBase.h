// (C) 2009 christian.schladetsch@gmail.com

#ifndef CGL_PHASE_BASE_H
#	define CGL_PHASE_BASE_H

CGL_BEGIN

struct PhaseBase;

CGL_TRAITS_DERIVED_ABSTRACT(PhaseBase, CGL_TypeNumbers::PhaseBase, ActiveObject);

/// stores a set of classes and instances
struct PhaseBase : ActiveObject
{
	/// optionally override to handle input during the phase
	virtual bool InputEvent(SDL_Event const &) { return false; }

	/// optionally override to enter this phaes, given last phase
	virtual void Enter(Pointer<PhaseBase> /*previous*/) {  }

	/// optionally override to leave this phaes, given next phase
	virtual void Leave(Pointer<PhaseBase> /*next*/) { }
};

CGL_END

#endif // CGL_PHASE_BASE_H

//EOF
