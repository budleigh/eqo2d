#include <Windows.h>

#include "util.h"

char* createUUID() {
	UUID uuid;
	UuidCreate(&uuid);

	char* id;
	UuidToString(&uuid, (RPC_CSTR*)&id);

	return id;
}
