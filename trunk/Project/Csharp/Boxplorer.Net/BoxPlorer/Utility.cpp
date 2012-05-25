#include "Precompiled.h"
#include "Utility.h"
#include <direct.h>


// Allocate a char[] and read a text file into it. Return 0 on error.
char* readFile(char const* name) 
{
	FILE* f;
	int len;
	char* s = 0;

	// open file an get its length
	if (!(f = fopen(name, "r"))) goto readFileError1;
	fseek(f, 0, SEEK_END);
	len = ftell(f);

	// read the file in an allocated buffer
	if (!(s = (char*)malloc(len+1))) goto readFileError2;
	rewind(f);
	len = fread(s, 1, len, f);
	s[len] = '\0';

readFileError2: fclose(f);
readFileError1: return s;
}


void changeWorkingDirectory(const char* configFile) 
{
	char dirName[MAX_PATH];
	strncpy(dirName, configFile, sizeof dirName);
	dirName[sizeof dirName - 1] = 0;
	if (strstr(configFile, ".cfg.data") == NULL) 
	{
		strncat(dirName, ".data", sizeof dirName);
		_mkdir(dirName);
	} 
	else 
	{
		// Already have a .cfg.data in path, chdir to it.
		int i = strlen(dirName);
		while (i > 0 && strchr("/\\", dirName[--i]) == NULL);
		dirName[i] = 0;
	}
	if (!_chdir(dirName)) 
		printf(__FUNCTION__ " : chdir '%s'\n", dirName);
}


//EOF
