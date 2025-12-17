#pragma once

#define PAPYRUSUTIL_VERSION 45

#include "f4se/PluginAPI.h"
#include "f4se/GameAPI.h"


struct StaticFunctionTag;


namespace Plugin {
	void InitPlugin();
	bool RegisterFuncs(VMClassRegistry* registry);
	void HandleF4SEMessage(F4SEMessagingInterface::Message * msg);
	UInt32 GetVersion(StaticFunctionTag* base);

}
