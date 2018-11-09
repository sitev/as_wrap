#include <stdio.h>
#include "as_wrap.h"

// AngelScriptModule

AngelScriptFunc* AngelScriptModule::getFunc(const char *decl) {
	AngelScriptFunc *func = (AngelScriptFunc*)this->GetFunctionByDecl(decl);
	return func;
}


// AngelScript

void AngelScriptMessageCallback(const asSMessageInfo *msg, void *param)
{
	const char *type = "ERR ";
	if (msg->type == asMSGTYPE_WARNING) type = "WARN";
	else if (msg->type == asMSGTYPE_INFORMATION) type = "INFO";

	printf("%s (%d, %d) : %s : %s\n", msg->section, msg->row, msg->col, type, msg->message);
}

AngelScript::AngelScript() {
	engine = asCreateScriptEngine();
	engine->SetMessageCallback(asFUNCTION(AngelScriptMessageCallback), 0, asCALL_CDECL);
	RegisterStdString(engine);
	RegisterScriptArray(engine, false);

	builder = new CScriptBuilder();
	ctx = engine->CreateContext();
}

bool AngelScript::regCppFunc(const char *declaration, const asSFuncPtr &funcPointer, asDWORD callConv, void *auxiliary) {
	int r = engine->RegisterGlobalFunction(declaration, funcPointer, callConv, auxiliary);
	return r >= 0;
}

AngelScriptModule* AngelScript::newModule(const char *module, const char *file) {
	int r = builder->StartNewModule(engine, module);
	if (r < 0) return NULL;

	r = builder->AddSectionFromFile(file);
	if (r < 0) return NULL;

	r = builder->BuildModule();
	if (r < 0) {
		char scriptFile[101];
		engine->WriteMessage(scriptFile, 0, 0, asMSGTYPE_ERROR, "Script failed to build");
		return NULL;
	}

	AngelScriptModule *mod = (AngelScriptModule*)engine->GetModule(module);
	return mod;
}

bool AngelScript::exec(AngelScriptFunc *func) {
	ctx->Prepare(func);
	r = ctx->Execute();
	//int *v = (int*)ctx->GetReturnAddress();

	return r >= 0;
}

int AngelScript::getLastResult() {
	return r;
}

AngelScript::~AngelScript() {
	// Clean up
	ctx->Release();
	engine->ShutDownAndRelease();
	delete builder;

}