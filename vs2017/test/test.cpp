#include <iostream>
#include <string>

using namespace std;

#include "as_wrap.h"

void print(const string &s) {
	cout << s;
}

int main()
{
	AngelScript as;

	as.regCppFunc("void print(const string &in)", asFUNCTION(print), asCALL_CDECL);
	AngelScriptModule *mod = as.newModule("test_module", "test.as");
	AngelScriptFunc *func = mod->getFunc("void main()");
	as.exec(func);

	getchar();

	return 0;
}

