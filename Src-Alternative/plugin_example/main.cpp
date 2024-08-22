#include <shlobj.h>
#include "f4se/PluginAPI.h"
#include "f4se_common/f4se_version.h"
#include "f4se_common/Relocation.h"
#include "f4se_common/SafeWrite.h"
#include "f4se/ScaleformValue.h"
#include "f4se/ScaleformCallbacks.h"
#include "f4se/ScaleformMovie.h"
#include "f4se/PapyrusVM.h"
#include "f4se/PapyrusNativeFunctions.h"
#include "f4se/GameAPI.h"

#include "plugin_example.h"

// Store plugin log in My Games, differentiating between various builds
UInt32 pluginVersion = PLUGIN_VERSION;
char pluginName[] = { PLUGIN_NAME };
char pluginAuthor[] = { PLUGIN_AUTHOR };

char pluginExt[] = {".log"};
char pluginLogPath[] = {"\\My Games\\Fallout4\\F4SE\\"};
#ifdef _DEBUG
	char pluginEdition[] = {"-debug"};
#else
	char pluginEdition[] = {""};
#endif
#ifdef _NOGORE
	char pluginVariant[] = {"-nogore"};
#else
	char pluginVariant[] = {""};
#endif
static char pluginLog[sizeof(pluginLogPath)+sizeof(pluginName)+sizeof(pluginEdition)+sizeof(pluginVariant)+sizeof(pluginExt)];
char pluginCustomIni[sizeof(pluginName)+sizeof(".ini")];

PluginHandle	g_pluginHandle = kPluginHandle_Invalid;

#define REQUIRED_RUNTIME RUNTIME_VERSION_1_10_980

F4SEScaleformInterface		* g_scaleform = NULL;
F4SEPapyrusInterface		* g_papyrus = NULL;
F4SESerializationInterface	* g_serialization = NULL;
F4SEMessagingInterface		* g_messaging = NULL;

// Build the pluginLog name and open it
void OpenPluginLog()
{
	strcpy_s(pluginLog, pluginLogPath);
	strcat_s(pluginLog, pluginName);
	strcat_s(pluginLog, pluginVariant);
	strcat_s(pluginLog, pluginEdition);
	strcat_s(pluginLog, pluginExt);

	gLog.OpenRelative(CSIDL_MYDOCUMENTS, pluginLog);
#ifdef _DEBUG
	gLog.SetLogLevel(IDebugLog::kLevel_DebugMessage);
#endif
}

/**** simple gameplay patches ****/

void ApplyPatch(uintptr_t base, UInt8 * buf, UInt32 len)
{
	for(UInt32 i = 0; i < len; i++)
		SafeWrite8(RelocAddr <UInt8 *> (base + i).GetUIntPtr(), buf[i]);
}

void GameplayPatches(void)
{
	SafeWrite8(RelocAddr <UInt8 *> (0x013A25D0).GetUIntPtr(), 0xC3);	// disable achievements
}

/**** scaleform functions ****/

class F4SEScaleform_ExampleFunction : public GFxFunctionHandler
{
public:
	virtual void	Invoke(Args * args)
	{
		Console_Print("hello world from example plugin");
	}
};

bool RegisterScaleform(GFxMovieView * view, GFxValue * root)
{
	GFxMovieRoot	* movieRoot = view->movieRoot;
	RegisterFunction <F4SEScaleform_ExampleFunction>(root, movieRoot, "ExampleFunction");

	return true;
}

/**** papyrus function ****/

void HelloWorld(StaticFunctionTag *base)
{
	_DMESSAGE("HelloWorld");
	Console_Print("hello world from example plugin");
};

bool RegisterFuncs(VirtualMachine* vm)
{
	_MESSAGE("RegisterFuncs");
	vm->RegisterFunction(
		new NativeFunction0<StaticFunctionTag, void>("HelloWorld", "plugin_example", HelloWorld, vm));

	return true;
}

/**** serialization ****/

void Serialization_Revert(const F4SESerializationInterface * intfc)
{
	_MESSAGE("revert");
}

const UInt32 kSerializationDataVersion = 1;

void Serialization_Save(const F4SESerializationInterface * intfc)
{
	_MESSAGE("save");

	if(intfc->OpenRecord('DATA', kSerializationDataVersion))
	{
		char	kData[] = "hello world";

		intfc->WriteRecordData(kData, sizeof(kData));
	}
}

void Serialization_Load(const F4SESerializationInterface * intfc)
{
	_MESSAGE("load");

	UInt32	type;
	UInt32	version;
	UInt32	length;
	bool	error = false;

	while(!error && intfc->GetNextRecordInfo(&type, &version, &length))
	{
		switch(type)
		{
			case 'DATA':
			{
				if(version == kSerializationDataVersion)
				{
					if(length)
					{
						char	* buf = new char[length];

						intfc->ReadRecordData(buf, length);
						buf[length - 1] = 0;

						_MESSAGE("read data: %s", buf);
					}
					else
					{
						_MESSAGE("empty data?");
					}
				}
				else
				{
					error = true;
				}
			}
			break;

			default:
				_MESSAGE("unhandled type %08X", type);
				error = true;
				break;
		}
	}
}

void MessageCallback(F4SEMessagingInterface::Message * msg)
{
	if (msg->type == F4SEMessagingInterface::kMessage_NewGame)
	{
		_MESSAGE("%s received message new game as #%d", pluginName, msg->type);
	}
	else
		_MESSAGE("%s received message #%d", pluginName, msg->type);
}

extern "C"
{

bool F4SEPlugin_Query(const F4SEInterface * f4se, PluginInfo * info)
{

	OpenPluginLog();

	_MESSAGE("%s querying...", pluginName);

	// populate info structure
	if (info) {
		info->infoVersion =	PluginInfo::kInfoVersion;
		info->name =		pluginName;
		info->version =		pluginVersion;
	}

	// store plugin handle so we can identify ourselves later
	g_pluginHandle = f4se->GetPluginHandle();

	if(f4se->isEditor)
	{
		_MESSAGE("loaded in editor, marking as incompatible");

		return false;
	}
	else if(f4se->runtimeVersion != REQUIRED_RUNTIME)
	{
		_MESSAGE("unsupported runtime version %08X (expected %08X)", f4se->runtimeVersion, REQUIRED_RUNTIME);

		return false;
	}

	// get the scaleform interface and query its version
	g_scaleform = (F4SEScaleformInterface *)f4se->QueryInterface(kInterface_Scaleform);
	if(!g_scaleform)
	{
		_MESSAGE("couldn't get scaleform interface");

		return false;
	}

	if(g_scaleform->interfaceVersion < F4SEScaleformInterface::kInterfaceVersion)
	{
		_MESSAGE("scaleform interface too old (%d expected %d)", g_scaleform->interfaceVersion, F4SEScaleformInterface::kInterfaceVersion);

		return false;
	}

	// get the papyrus interface and query its version
	g_papyrus = (F4SEPapyrusInterface *)f4se->QueryInterface(kInterface_Papyrus);
	if(!g_papyrus)
	{
		_MESSAGE("couldn't get papyrus interface");

		return false;
	}

	if(g_papyrus->interfaceVersion < F4SEPapyrusInterface::kInterfaceVersion)
	{
		_MESSAGE("papyrus interface too old (%d expected %d)", g_papyrus->interfaceVersion, F4SEPapyrusInterface::kInterfaceVersion);

		return false;
	}

	// get the serialization interface and query its version
	g_serialization = (F4SESerializationInterface *)f4se->QueryInterface(kInterface_Serialization);
	if(!g_serialization)
	{
		_MESSAGE("couldn't get serialization interface");

		return false;
	}

	if(g_serialization->version < F4SESerializationInterface::kInterfaceVersion)
	{
		_MESSAGE("serialization interface too old (%d expected %d)", g_serialization->version, F4SESerializationInterface::kInterfaceVersion);

		return false;
	}

	// get the messaging interface and query its version
	g_messaging = (F4SEMessagingInterface *)f4se->QueryInterface(kInterface_Messaging);
	if(!g_messaging)
	{
		_MESSAGE("couldn't get papyrus interface");

		return false;
	}

	if(g_messaging->interfaceVersion < F4SEMessagingInterface::kInterfaceVersion)
	{
		_MESSAGE("messaging interface too old (%d expected %d)", g_messaging->interfaceVersion, F4SEMessagingInterface::kInterfaceVersion);

		return false;
	}

	// ### do not do anything else in this callback
	// ### only fill out PluginInfo and return true/false

	// supported runtime version
	_MESSAGE("%s query successful.", pluginName);
	return true;
}

bool F4SEPlugin_Load(const F4SEInterface * f4se)
{
	if ((g_pluginHandle != kPluginHandle_Invalid) || !F4SEPlugin_Query(f4se, NULL))
		return false;

	_MESSAGE("%s loading...", pluginName);

	// apply patches to the game here
	strcpy_s(pluginCustomIni, pluginName);
	strcat_s(pluginCustomIni, ".ini");
	GameplayPatches();

	// register scaleform callbacks
	g_scaleform->Register(pluginName, RegisterScaleform);

	// register papyrus functions
	g_papyrus->Register(RegisterFuncs);

	// register callbacks and unique ID for serialization

	// ### this must be a UNIQUE ID, change this and email me the ID (team [at] f4se [dot] silverlock [dot] org) so I can let you know if someone else has already taken it
	g_serialization->SetUniqueID(g_pluginHandle, 'PLEX');

	g_serialization->SetRevertCallback(g_pluginHandle, Serialization_Revert);
	g_serialization->SetSaveCallback(g_pluginHandle, Serialization_Save);
	g_serialization->SetLoadCallback(g_pluginHandle, Serialization_Load);

	if (g_messaging != nullptr)
		g_messaging->RegisterListener(g_pluginHandle, "F4SE", MessageCallback);

	_MESSAGE("%s load successful.", pluginName);
	return true;
}

};
