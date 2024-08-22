#include <shlobj.h>
#include "f4se/PluginAPI.h"
#include "f4se_common/f4se_version.h"
#include "f4se/ScaleformCallbacks.h"

#include "Plugin.h"
#include "Offsets.h"
#include "Data.h"
#include "Serialize.h"

#include <shlobj.h>

#include "versionlibdb.h"

#include "thisplugin.h"

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
PluginHandle	g_pluginHandle = kPluginHandle_Invalid;

// F4SE Interfaces
F4SEInterface * SaveF4SE;
F4SESerializationInterface * g_serialization = NULL;
F4SEPapyrusInterface       * g_papyrus = NULL;

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

#define REQUIRED_RUNTIME CURRENT_RELEASE_RUNTIME

extern "C"
{

_declspec(dllexport) F4SEPluginVersionData F4SEPlugin_Version =
{
	F4SEPluginVersionData::kVersion,
	
	PLUGIN_VERSION,
	PLUGIN_NAME,
	PLUGIN_AUTHOR,

	0,	// not version independent
	0,	// not version independent (extended field)
	{ REQUIRED_RUNTIME, 0 },	// compatible with this version only

	0,	// works with any version of the script extender. you probably do not need to put anything here
};


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
	
	// version checks
	if(f4se->f4seVersion < F4SE_VERSION_INTEGER)
	{
		_ERROR("f4se version too old (got %08X expected at least %08X)", f4se->f4seVersion, F4SE_VERSION_INTEGER);
		return false;
	}

	if(!f4se->isEditor)
	{
		if(f4se->runtimeVersion != REQUIRED_RUNTIME)
		{
			_ERROR("incorrect runtime version (got %08X need %08X)", f4se->runtimeVersion, REQUIRED_RUNTIME);
			return false;
		}

	}
	else
	{
		return false;
	}

		// Initialize offsets with address library
		if (!Plugin::InitializeOffsets()) {
			_MESSAGE("InitializeOffsets failed!");
			return false;
		}

	// get the serialization interface and query its version
	g_serialization = (F4SESerializationInterface *)f4se->QueryInterface(kInterface_Serialization);
	if(!g_serialization) {
		_MESSAGE("couldn't get serialization interface");
		return false;
	}
	if(g_serialization->version < F4SESerializationInterface::kInterfaceVersion) {
		_MESSAGE("serialization interface too old (%d expected %d)", g_serialization->version, F4SESerializationInterface::kInterfaceVersion);
		return false;
	}

	// get papyrus vm interface
	g_papyrus = (F4SEPapyrusInterface *)f4se->QueryInterface(kInterface_Papyrus);
	if(!g_papyrus) {
		_MESSAGE("couldn't get papyrus interface");
		return false;
	}

	return true;
}

bool F4SEPlugin_Load(const F4SEInterface * f4se)
{
	if ((g_pluginHandle != kPluginHandle_Invalid) || !F4SEPlugin_Query(f4se, NULL))
		return false;

	_MESSAGE("%s loading...", pluginName);


	// save the F4SEinterface in case we need it later
	SaveF4SE = (F4SEInterface *)f4se;

	// Dev tmp
	//g_serialization->SetUniqueID(g_pluginHandle, 884792 + 15325);

	g_serialization->SetUniqueID(g_pluginHandle, 884715692 + 227106806);

	g_serialization->SetRevertCallback(g_pluginHandle, Data::Serialization_Revert);
	g_serialization->SetSaveCallback(g_pluginHandle, Data::Serialization_Save);
	g_serialization->SetLoadCallback(g_pluginHandle, Data::Serialization_Load);

		g_papyrus->Register(Plugin::RegisterFuncs);


		// Init storage
		Plugin::InitPlugin();
		Data::InitLists();


	return true;
}

}
