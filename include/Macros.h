#pragma once
#include <thread>
#include "Nebuleuse.h"

//Used for easily starting threads
#define STARTCOMTHREAD(x,...) thread thre(&Neb::Nebuleuse::Thread_ ## x, this, __VA_ARGS__);\
								thre.detach();

//Used to quickly verify document in parsing
#define PARSEANDCHECK(x)	if (doc.Parse(x.c_str()).HasParseError()){\
								return ThrowError(NEBULEUSE_ERROR_PARSEFAILED);\
																												}\
							if (!doc.IsObject()){\
								return ThrowError(NEBULEUSE_ERROR_PARSEFAILED);\
																					}\
							if (doc.HasMember("Code") && doc.HasMember("Message")){\
								if (doc["Code"].IsInt()){\
									return ThrowError(doc["Code"].GetInt(), doc["Message"].GetString());\
																								}\
																					}
#define STDTOJSONVAL(x) StringRef(x.c_str())