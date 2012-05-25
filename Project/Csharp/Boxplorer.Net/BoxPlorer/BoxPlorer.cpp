#include "Precompiled.h"

#include <msclr\marshal_cppstd.h>

using namespace System;
using namespace System::Collections::Generic;

int woxmain(int, const char **);

//
// wrapper for boxplorer C++ library
//
public ref class BoxPlorer
{

public:

	void Start(String ^file)
	{
		msclr::interop::marshal_context context;
		const char *args[2];
		args[1] = context.marshal_as<const char *>(file);
		
		woxmain(2, args);
	}

	void LoadConfig(String ^file)
	{
	}

	Dictionary<System::String ^, Object ^> ^GetUniforms()
	{
		Dictionary<System::String ^, Object ^> ^uniforms = gcnew Dictionary<System::String ^, Object ^>();
		uniforms->Add("foo", 1);
		return uniforms;
	}
};

