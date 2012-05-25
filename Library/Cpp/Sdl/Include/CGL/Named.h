// (C) 2009 christian.schladetsch@gmail.com

#ifndef CGL_NAMED_H_INCLUDED
#	define CGL_NAMED_H_INCLUDED

/// stores a string used to nominate an object
class Named
{
	String name;

public:
	Named() { }
	Named(const char *P) : name(P) { }
	Named(String const &N) : name(N) { }

	String const &GetName() const { return name; }
	void SetName(const char *P) { if (P) name = P; else name = ""; }
};

#endif // CGL_NAMED_H_INCLUDED

//EOF
