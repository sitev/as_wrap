#pragma once

#include <angelscript.h>
#include <scriptstdstring/scriptstdstring.h>
#include <scriptbuilder/scriptbuilder.h>
#include <scriptarray/scriptarray.h>


class AngelScriptFunc : public asIScriptFunction {

};

class AngelScriptModule : public asIScriptModule {
public:
	AngelScriptFunc * getFunc(const char *decl);
};

class AngelScript {
public:
	asIScriptEngine *engine;
	CScriptBuilder *builder;
	asIScriptContext *ctx;
	int r;
public:
	AngelScript();
	bool regCppFunc(const char *declaration, const asSFuncPtr &funcPointer, asDWORD callConv, void *auxiliary = 0);
	AngelScriptModule* newModule(const char *module, const char *file);
	bool exec(AngelScriptFunc *func);
	int getLastResult();
	~AngelScript();
};