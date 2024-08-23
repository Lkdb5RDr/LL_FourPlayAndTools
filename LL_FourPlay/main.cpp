#include <shlobj.h>
#include "f4se/PluginAPI.h"
#include "f4se_common/f4se_version.h"
#include "f4se_common/Relocation.h"
#include "f4se_common/SafeWrite.h"
#include "f4se/PapyrusVM.h"
#include "f4se/PapyrusNativeFunctions.h"
#include "f4se/GameAPI.h"
#include "f4se/GameData.h"
#include "f4se/GameForms.h"
#include "f4se/GameReferences.h"
#include "f4se/GameObjects.h"
#include "f4se/GameExtraData.h"
#include "f4se/GameRTTI.h"
#include "f4se/GameCamera.h"
#include "f4se/GameInput.h"
#include "f4se/GameSettings.h"
#include "f4se/ScaleformValue.h"
#include "f4se/ScaleformMovie.h"
#include "f4se/Hooks_Scaleform.h"
#include "f4se/ScaleformCallbacks.h"
#include <algorithm>
#include <locale>
#define _USE_MATH_DEFINES
#include <math.h>
#include <random>
#include "LL_FourPlay.h"

//by fedim
#include <fstream>
using namespace std;
#pragma comment(lib,"Winmm.lib")

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

F4SEScaleformInterface		* g_scaleform = NULL;
F4SEPapyrusInterface		* g_papyrus = NULL;
F4SESerializationInterface	* g_serialization = NULL;
F4SEMessagingInterface		* g_messaging = NULL;

#define REQUIRED_RUNTIME CURRENT_RELEASE_RUNTIME

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

/**** runtime code ****/

/**** helpers ****/

const std::string & GetCustomConfigPath(const char * name);
std::string GetCustomConfigOption(const char * name, const char * section, const char * key);
bool GetCustomConfigOption_UInt32(const char * name, const char * section, const char * key, UInt32 * dataOut);
bool SetCustomConfigOption(const char * name, const char * section, const char * key, const char * value);
bool SetCustomConfigOption_UInt32(const char * name, const char * section, const char * key, UInt32 data);

#define maxResultBuf 32768

/* modified from Utilities.h */

const std::string & GetCustomConfigPath(const char * name)
{
	static std::string s_configPath;

	std::string	runtimePath = GetRuntimeDirectory();
	if(!runtimePath.empty())
	{
		s_configPath  = runtimePath;
		s_configPath += "Data\\F4SE\\plugins\\";
		s_configPath += name;

		_DMESSAGE("config path = %s", s_configPath.c_str());
	}

	return s_configPath;
}

std::string GetCustomConfigOption(const char * name, const char * section, const char * key)
{
	std::string	result;

	const std::string & configPath = GetCustomConfigPath(name);
	if(!configPath.empty())
	{
		char	resultBuf[maxResultBuf];
		resultBuf[0] = 0;

		UInt32	resultLen = GetPrivateProfileString(section, key, NULL, resultBuf, sizeof(resultBuf), configPath.c_str());

		result = resultBuf;
	}

	return result;
}

bool SetCustomConfigOptions(const char * name, const char * section, VMArray<BSFixedString> keys, VMArray<BSFixedString> values)
{
	bool result = false;

	const std::string & configPath = GetCustomConfigPath(name);
	if(!configPath.empty() && (keys.Length() == values.Length()))
	{
		for (int i = 0; i < keys.Length(); ++i)
		{
			BSFixedString k;
			BSFixedString v;
			keys.Get(&k, i);
			values.Get(&v, i);
			result = WritePrivateProfileString(section, k.c_str(), v.c_str(), configPath.c_str()) ? true : false;
			if (!result)
				break;
		}
	}

	return result;
}

bool SplitKeyValue(const char * keyValue, BSFixedString & key, BSFixedString & value)
{
		std::string t = keyValue;
		std::string::size_type pos = t.find_first_of('=', 0);
		t.erase(pos, t.length());
		if (t.length() > 0)
		{
			key = t.c_str();

			t = keyValue;
			pos = t.find_first_of('=', 0);
			t.erase(0, ++pos);
			value = t.c_str();

			return true;
		}
		else
			return false;
}

bool GetCustomConfigOptions(const char * name, const char * section, VMArray<BSFixedString> & keys, VMArray<BSFixedString> & values)
{
	bool result = false;

	const std::string & configPath = GetCustomConfigPath(name);
	if(!configPath.empty() && section && !keys.IsNone())
	{
		char resultBuf[maxResultBuf];
		resultBuf[0] = 0;

		UInt32	resultLen = GetPrivateProfileSection(section, resultBuf, sizeof(resultBuf), configPath.c_str());
		char * begin = resultBuf;
		VMArray<BSFixedString> tk, tv;
		for (;*begin;++begin)
		{
			BSFixedString k, v;
			if (begin && (result = SplitKeyValue(begin, k, v)))
			{
				tk.Push(&k);
				tv.Push(&v);
				begin += strlen(begin);
			}
			else
				break;
		}
		keys = tk;
		if (!values.IsNone())
			values = tv;
	}

	return result;
}

VMArray<VMVariable> StringArrayToVarArray(VMArray<BSFixedString> str)
{
	VMVariable v;
	VMArray<VMVariable> va;
	for (int i = 0; i < str.Length(); i++)
	{
		BSFixedString s;
		str.Get(&s, i);
		v.Set(&s);
		va.Push(&v);
	}
	return va;
}

VMVariable StringArrayToVar(VMArray<BSFixedString> str)
{
	VMVariable v;
	VMArray<VMVariable> va = StringArrayToVarArray(str);
	v.Set<VMArray<VMVariable>>(&va);
	return v;
}

VMArray<VMVariable> GetCustomConfigOptions(const char * name, const char * section)
{
	VMArray<VMVariable> a;
	VMArray<BSFixedString> tk, tv;
	bool result = GetCustomConfigOptions(name, section, tk, tv);
	if (result)
	{
		VMVariable v;
		VMArray<VMVariable> va;
		v = StringArrayToVar(tk);
		a.Push(&v);
		v = StringArrayToVar(tv);
		a.Push(&v);
	}
	return a;
}

VMArray<BSFixedString> GetCustomConfigSections(const char * name)
{
	VMArray<BSFixedString>	result;

	const std::string & configPath = GetCustomConfigPath(name);
	if(!configPath.empty())
	{
		char	resultBuf[maxResultBuf];
		resultBuf[0] = 0;

		UInt32	resultLen = GetPrivateProfileSectionNames(resultBuf, sizeof(resultBuf), configPath.c_str());
		char * begin = resultBuf;
		for (;*begin;++begin)
		{
			BSFixedString l = begin;
			result.Push(&l);
			begin += strlen(begin);
		}
	}

	return result;
}

bool ResetCustomConfigOptions(const char * name, const char * section, VMArray<BSFixedString> keys, VMArray<BSFixedString> values)
{
	bool result = false;

	const std::string & configPath = GetCustomConfigPath(name);
	if(!configPath.empty() && (keys.Length() == values.Length()))
	{
		char	sendBuf[maxResultBuf];
		sendBuf[0] = 0;

		char * begin = sendBuf;
		int used = 1;	// at least the final terminator
		for (int i = 0; i < keys.Length(); ++i)
		{
			BSFixedString v;
			keys.Get(&v, i);
			const char * d = v.c_str();
			int l = strlen(d);
			used += l + 1;
			if (maxResultBuf>used)
			{
				strcat_s(begin, maxResultBuf-used, d);
				begin += strlen(d);
				strcat_s(begin, maxResultBuf-used-1, "=");
				begin += 1;
			}
			else
				return false;
			values.Get(&v, i);
			d = v.c_str();
			l = strlen(d);
			used += l + 1;
			if (maxResultBuf>used)
			{
				strcat_s(begin, maxResultBuf-used, d);
				begin += strlen(d)+1;
				*begin = 0;
			}
			else
				return false;
		}
		sendBuf[used] = 0;
		result = WritePrivateProfileSection(section, sendBuf, configPath.c_str()) ? true : false;
	}

	return result;
}

bool GetCustomConfigOption_UInt32(const char * name, const char * section, const char * key, UInt32 * dataOut)
{
	std::string	data = GetCustomConfigOption(name, section, key);
	if(data.empty())
		return false;

	return (sscanf_s(data.c_str(), "%u", dataOut) == 1);
}

bool GetCustomConfigOption_float(const char * name, const char * section, const char * key, float * dataOut)
{
	std::string	data = GetCustomConfigOption(name, section, key);
	if(data.empty())
		return false;

	return (sscanf_s(data.c_str(), "%g", dataOut) == 1);
}

bool SetCustomConfigOption(const char * name, const char * section, const char * key, const char * value)
{
	bool result = false;

	const std::string & configPath = GetCustomConfigPath(name);
	if(!configPath.empty())
		result = WritePrivateProfileString(section, key, value, configPath.c_str()) ? true : false;

	return result;
}

bool SetCustomConfigOption_UInt32(const char * name, const char * section, const char * key, UInt32 data)
{
	char value[65536] = "";
	if (!_itoa_s(data, value, 65535, 10))
		return SetCustomConfigOption(name, section, key, value);
	else
		return false;
}

bool SetCustomConfigOption_float(const char * name, const char * section, const char * key, float data)
{
	char value[65536] = "";
	sprintf_s(value, "%g", data);
	return SetCustomConfigOption(name, section, key, value);
}

UInt32 GetMaxArraySize()
{
	UInt32 maxArraySize = 0; // greater than 128 and up to $FFFFFFFF in theory (but don't, seriously). 0 means no change from vanilla
	GetCustomConfigOption_UInt32(pluginCustomIni, "Custom Arrays", "uMaxArraySize", &maxArraySize);
	return maxArraySize;
}

void SetMaxArraySize(UInt32 newMaxArraySize)
{
	#define s_hackArrayAdd 0x01FEBB3D	//	1FEB41D	// 2775A34	// 2775914	// 279A9E4
	#define s_hackArrayInsert 0x01FEC1A5	// 1FEBA85	// 2775FD9	// 2775EB9	// 279AF89
	SafeWrite32(RelocAddr <UInt32 *> (s_hackArrayAdd+2).GetUIntPtr(), newMaxArraySize);		// change maximum size reachable using the .add function of arrays.
	SafeWrite32(RelocAddr <UInt32 *> (s_hackArrayInsert+1).GetUIntPtr(), newMaxArraySize);	// change maximum size reachable using the .insert function of arrays.
}

void GameplayPatches(void)
{
	/**** bypass child test on kill ****/

	#define k_HookChild 0x00DA759A
	UInt32 kk = 0;					// Defaults to not active

	if (GetCustomConfigOption_UInt32(pluginCustomIni, "general", "KK", &kk) && kk)
	{
		// definitivly NOT working. I need more digging :(
		//for (UInt8 i = 0; i < 6; i+=2)
		//	SafeWrite16(RelocAddr <UInt16> (k_HookChild+i).GetUIntPtr(), 0xC032);	// xor al, al
		//_MESSAGE("Killable Kids is active");
	}

	UInt32 maxArraySize = 0; // greater than 128 and up to $FFFFFFFF in theory (but don't, seriously). 0 means no change from vanilla
	if(maxArraySize = GetMaxArraySize())
		SetMaxArraySize(maxArraySize);
}

bool SetMinimalMaxArraySize_internal(UInt32 newMaxArrraySize)
{
	bool result = true;
	UInt32 maxArraySize = 0; // greater than 128 and up to $FFFFFFFF in theory (but don't, seriously). 0 means no change from vanilla
	if(maxArraySize = GetMaxArraySize())
		if (maxArraySize > newMaxArrraySize)
		{
			newMaxArrraySize = maxArraySize;
			result = false;
		}
	if (result)
	{
		SetCustomConfigOption_UInt32(pluginCustomIni, "Custom Arrays", "uMaxArraySize", newMaxArrraySize);
		SetMaxArraySize(newMaxArrraySize);
	}
	return result;
}

/**** papyrus function ****/

class TESForm_LL: public BaseFormComponent
{
public:
	struct Mods
	{
		// Array of mods that contain this form (wether it is an original, a copy of an overwrite).
		// The builtin forms (formID < 0x0800) not overwritten in the game esm have a null mods pointer.
		// Those arrays are shared, as there is only one per combination of overwriting mods.
		// It is the same in Skyrim and Skyrim SE
		ModInfo ** entries;		// 00
		UInt32	size;			// 08
		UInt32	pad0C;			// 0C
	};

	Mods	* mods;		// 08
	UInt32	flags;		// 10	CK also
	UInt32	formID;		// 14	CK also
};

class ExtraPowerArmor_LL: public BSExtraData
{
public:

	UInt32		unk0018;	// 0018
	UInt32		unk001C;	// 001C
	TESRace		*race;		// 0020
	UInt16		unk0028;	// 0028
};

class ExtraLeveledCreature_LL : public BSExtraData
{
public:
	TESForm	* baseForm;		//	018
	UInt8	unk0020[104];	//	020
};

class TESObjectREFR_LL : public TESObjectREFR
{
public:
	static TESObjectREFR_LL* FromRef(TESObjectREFR* ref) { return reinterpret_cast<TESObjectREFR_LL*>(ref); };

	MEMBER_FN_PREFIX(TESObjectREFR_LL);
	DEFINE_MEMBER_FN(SetAngleOnReference, void, 0x004BF810	/* 040BD70 */, NiPoint3);
	DEFINE_MEMBER_FN(SetNoCollision, void, 0x004A00A0	/* 03EDEE0 */, bool);
	DEFINE_MEMBER_FN(GetRefPersists, bool, 0x004A7990	/* 03F4DA0 */);
	DEFINE_MEMBER_FN(SetRefPersists, void, 0x004A79F0	/* 03F4E00 */, bool);
	DEFINE_MEMBER_FN(GetDistanceFromPoint, float, 0x004C32F0	/* 040F560 */)
	DEFINE_MEMBER_FN(GetDistanceFromReference, float, 0x004C33B0	/* 040F620 */, TESObjectREFR const*, bool, bool)
	DEFINE_MEMBER_FN(GetGoldAmount, UInt32, 0x004ADAF0	/* 03FA8D0 */);
	DEFINE_MEMBER_FN(GetMarkerUser, UInt32 *, 0x0050A790	/* 047BD90 */, UInt32 *refHandlePtr, UInt32 MarkerNum, bool abIgnoreReserved); //returns refHandlePtr
	DEFINE_MEMBER_FN(WornHasKeyword, bool, 0x004B36F0	/* 0400050 */, BGSKeyword *kw);
	DEFINE_MEMBER_FN(GetScale, float, 0x004AB5F0 /* 0x03F8540 */);
	DEFINE_MEMBER_FN(SetScale, void, 0x004AB660 /* 0x03F85B0 */, float scale);
};

class Actor_LL : public Actor
{
public:
	static Actor_LL * FromActor(Actor * actor) { return reinterpret_cast<Actor_LL *>(actor);};

	MEMBER_FN_PREFIX(Actor_LL);
	DEFINE_MEMBER_FN(HasPerkInRank, byte, 0x00C0C3F0	/*	0C0C060 0DA6600 0DA64E0, 0DA64E0 */, BGSPerk *);
	DEFINE_MEMBER_FN(SetPosition, void, 0x00BDAB80	/* 0D77690 */, const NiPoint3& a_pos, bool a_updateCharController);		// Actor::Func00CA
	DEFINE_MEMBER_FN(PutCreatedPackage, void, 0x00BFBC30	/* 0D96C70 */, TESForm* a_pack, bool a_tempPack, bool a_isACreatedPackage, bool a_allowFromFurniture);		// Actor::Func00FA
	DEFINE_MEMBER_FN(DisableCollision, void, 0x00BD6E30	/* 0D73AB0 */, void);
	DEFINE_MEMBER_FN(EnableCollision, void, 0x00BD6DA0	/* 0D73A20 */, void);
	DEFINE_MEMBER_FN(StopInteractingQuick, void, 0x00C55FD0	/* 0E0EAA0 */, bool flag1, bool flag2, bool a_initializeNodes);	// It is probably a TESObjectREFR member, rather than an Actor member.
	DEFINE_MEMBER_FN(GetSex, unsigned char, 0x00BD7960	/* 0D74610 */, void);
	DEFINE_MEMBER_FN(GetLevel, UInt32, 0x00BDD41B	/* 0D79E90 */, void);
	DEFINE_MEMBER_FN(IsDead, bool, 0x00BEDE20	/* 0D89D40 */, void);		// Actor::Func00C0
	DEFINE_MEMBER_FN(EvaluatePackage, void, 0x00BE3600 /* 0D7F980 */, bool a_immediate, bool a_resetAI);

	struct AIProcess_LL : AIProcess
	{
		static AIProcess_LL * FromAIProcess(AIProcess * mp) { return reinterpret_cast<AIProcess_LL *>(mp); };
		void * unk00;	// 00
		Data08 * unk08;	// 08

		MEMBER_FN_PREFIX(AIProcess);
		DEFINE_MEMBER_FN(UpdateEquipment, void, 0x00CA12C0	/*	0CA0F30 0E60860 */, Actor * actor, UInt32 flags);
		// action should be 0x35 (DefaultObject kActionIdle), checkConditions should be false, targetObj should be null for AAF idles
		DEFINE_MEMBER_FN(PlayIdle, void, 0x00C75110	/* 0C74D80 0E35510 */, Actor* actor, int action, TESForm* idle, bool checkConditions, TESObjectREFR* targetObj);
		DEFINE_MEMBER_FN(StopCurrentIdle, void, 0x00C754B0	/* 0E358C0 */, Actor* actor, bool a1, bool a2);
		DEFINE_MEMBER_FN(ClearCurrentDataforProcess, void, 0x00CB38C0 /* 0E77520 */, Actor *actor); 
		DEFINE_MEMBER_FN(ClearCurrentPackage, void, 0x00C6B710 /* 0E2A670 */, Actor *actor);
	};
};

// from CommonLibF4
class TESWorldSpace : public TESForm
{
public:
	enum { kTypeID = kFormType_WRLD };
	TESFullName			fullName;
	TESTexture			texture;
	BSTHashMap<int32_t, TESObjectCELL*>	CellMap;
};
STATIC_ASSERT(offsetof(TESWorldSpace, CellMap) == 0x40);

class TESObjectCELL_LL : public TESObjectCELL
{
public:
	static TESObjectCELL_LL* FromCell(TESObjectCELL* cell) { return reinterpret_cast<TESObjectCELL_LL*>(cell); };
	
	MEMBER_FN_PREFIX(TESObjectCELL_LL);
	DEFINE_MEMBER_FN(GetDataXandY, void, 0x00472C70	/* 03B1080 */, int& iX, int& iY);
	DEFINE_MEMBER_FN(GetDataX, int, 0x00472C30	/* 03B1040 */, void);
	DEFINE_MEMBER_FN(GetDataY, int, 0x00472C50	/* 03B1060 */, void);
	DEFINE_MEMBER_FN(GetWorldSpace, TESWorldSpace*, 0x00443DC0	/* 0008880 */, void);
	};

class PlayerCharacter_LL : public PlayerCharacter
{
public:
	static PlayerCharacter_LL* FromPlayerCharacter(PlayerCharacter* plchr) { return reinterpret_cast<PlayerCharacter_LL*>(plchr); };

	MEMBER_FN_PREFIX(PlayerCharacter_LL);
	DEFINE_MEMBER_FN(UpdatePlayer3d, void, 0x00CD68B0	/* 0EA0630 */, void);
};

class TESActorBaseData_LL : public BaseFormComponent
{
public:
	virtual ~TESActorBaseData_LL();

	virtual void	Unk_07(void);
	virtual void	Unk_08(void);
	virtual void	Unk_09(void);

	enum
	{
		kFlagFemale = 0x01,
		kFlagEssential = 0x02,
		kFlagIsPreset = 0x04,
		kFlagRespawn = 0x08,
		kFlagAutoCalcStats = 0x10,
		kFlagUnique = 0x20,
		kFlagDoesntAffectStealthMeter = 0x40,
		kFlagPCLevelMult = 0x80,
		kFlagProtected = 0x800
	};

	enum
	{
		kTemplateTraits = 0,
		kTemplateStats,
		kTemplateFactions,
		kTemplateSpellList,
		kTemplateAIData,
		kTemplateAIPackages,
		kTemplateModelAnimation,
		kTemplateBaseData,
		kTemplateInventory,
		kTemplateScript,
		kTemplateDefPackList,
		kTemplateAttackData,
		kTemplateKeywords,
	};
	typedef TESForm * LPTESForm;
	typedef LPTESForm TemplateArray[13];

	UInt16			flags;				// 08 - 70
	UInt16			unk0A;				// 0A - 72
	UInt16			calcMinLevel;		// 0C - 74
	SInt16			calcMaxLevel;		// 0E - 76
	UInt16			dispositionBase;	// 10 - 78
	UInt16			useTemplateActors;	// 12 - 7A
	UInt16			unk14;				// 14 - 7C
	UInt16			unk16;				// 16 - 7E
	UInt64			unk18;				// 18 - 80
	UInt64			unk20;				// 20 - 88
	BGSVoiceType	* voiceType;		// 28 - 90
	UInt64			unk30;				// 30 - 98
	TemplateArray	* templates;		// 38 - A0
	UInt64			unk40;				// 40 - A8
	UInt64			unk48;				// 48 - B0
	UInt64			unk50;				// 50 - B8
	UInt64			unk58;				// 58 - C0
	UInt64			unk60;				// 60 - C8

	MEMBER_FN_PREFIX(TESActorBaseData_LL);
	DEFINE_MEMBER_FN(ModifyFlag, void, 0x002B5F10	/* 02B5EE0 */, UInt32 flag, bool doSet, bool unk1); // unk1 = 1 (appears to select which actor flags to use)
};
STATIC_ASSERT(sizeof(TESActorBaseData_LL) == 0x68);

// Status of FXCam, if active TFC won't work
	#define k_FXCamIsActive					0x02F6E578	//	2F6D578	// 59DA130	// 59DA100	// 5A13180

	#define k_saveCurrentPlayerCameraState	0x02C773E0	// 2C763E0	// 36F1283	// 3729283
	#define k_CheckIfCameraStateIsSelected	0x00FA5640	// 0FA52B0	// 124E7F0	// 124E6D0	// 124E6D0
	#define k_ToggleFlyCam					0x00FA19A0	// 0FA1610	// 124AE60	// 124AD40	// 124AD40

	#define k_ControlPlayerCamera			0x015551C0	// 1554DF0	// 1B285A0	// 1B28480	// 1B28480
	#define k_ControlPlayer					0x01555290	// 1554EC0	// 1B28690	// 1B28570	// 1B28570

	#define k_fFreeCameraTranslationSpeed_Camera	0x02CC7940	// 2CC6918	// 38048E8	// 383C8E8
	#define k_fFreeCameraRotationSpeed_Camera		0x02CC7958	// 2CC6930	// 3804900	// 383C900

RelocPtr <byte> g_FXCamIsActive(k_FXCamIsActive);
RelocPtr <byte> g_saveCurrentPlayerCameraState(k_saveCurrentPlayerCameraState);

class PlayerCamera_LL : public PlayerCamera
{
public:
	MEMBER_FN_PREFIX(PlayerCamera_LL);
	DEFINE_MEMBER_FN(CheckIfCameraStateIsSelected, bool, k_CheckIfCameraStateIsSelected, UInt32 cmCommand);
	DEFINE_MEMBER_FN(ToggleFlyCam, void, k_ToggleFlyCam, bool pause);
};
RelocPtr <PlayerCamera_LL *> g_playerCamera_LL(0x02E649D8	/* 2E639D8	58CEB28 */);

class InputManager_LL : public InputManager
{
public:
	MEMBER_FN_PREFIX(InputManager_LL);
	DEFINE_MEMBER_FN(ControlPlayerCamera, void, k_ControlPlayerCamera);
	DEFINE_MEMBER_FN(ControlPlayer, void, k_ControlPlayer);
};
RelocPtr <InputManager_LL *> g_inputMgr_LL(0x02F6E628	/* 2F6D628 59DA210 */);


class Main_LL
{
public:
	UInt8	unk00[0x029];	// 00
	UInt8	pause;			// 2A
	UInt8	unk2B[6];		// 2B
};	// 268 allocated
RelocPtr <Main_LL *> g_Main_LL(0x0303ABA8	/* 3039BA8 5ADD2C8 */);

// This should be a template: SettingT<INISettingCollection>
class SettingT_INISettingCollection_LL
{
public:

	virtual void Unk_00();

	float	value;	// 08
	char	*name;	// 0C

	MEMBER_FN_PREFIX(SettingT_INISettingCollection_LL);
	DEFINE_MEMBER_FN(Changed, void, 0x01684DB0	/* 16849E0, 1C91E10 1C91CF0, 1C91CF0 */ );
};	// 10

RelocPtr <SettingT_INISettingCollection_LL> g_fFreeCameraTranslationSpeed_Camera(k_fFreeCameraTranslationSpeed_Camera-8) ;
RelocPtr <SettingT_INISettingCollection_LL> g_fFreeCameraRotationSpeed_Camera(k_fFreeCameraRotationSpeed_Camera-8) ;

bool GetFXCam(StaticFunctionTag *base)
{
	return (*g_FXCamIsActive) ? true : false ;
};

bool GetFlyCam(StaticFunctionTag *base)
{
	PlayerCamera_LL* pc = (*g_playerCamera_LL);
	return CALL_MEMBER_FN(pc, CheckIfCameraStateIsSelected)(PlayerCamera::kCameraState_Free);
};

bool GetPaused(StaticFunctionTag *base)
{
	Main_LL* m = (*g_Main_LL);
	return m->pause ? true : false;
};

bool SetFlyCam(StaticFunctionTag *base, bool activate)
{
	PlayerCamera_LL* pc = (*g_playerCamera_LL);
	InputManager_LL* im = (*g_inputMgr_LL);

	bool current = CALL_MEMBER_FN(pc, CheckIfCameraStateIsSelected)(PlayerCamera::kCameraState_Free);
	if (current != activate && !GetFXCam(base))
	{
		if (!current)
			(*g_saveCurrentPlayerCameraState) = pc->unk30;
		CALL_MEMBER_FN(pc, ToggleFlyCam)(false);
		current = CALL_MEMBER_FN(pc, CheckIfCameraStateIsSelected)(PlayerCamera::kCameraState_Free);
		pc->Unk_02(current  || (*g_saveCurrentPlayerCameraState));
		if (current)
			CALL_MEMBER_FN(im, ControlPlayerCamera)();
		else
			CALL_MEMBER_FN(im, ControlPlayer)();
	}
	return current;
}

bool SetFlyCam2(StaticFunctionTag *base, bool activate, bool pause)
{
	PlayerCamera_LL* pc = (*g_playerCamera_LL);
	InputManager_LL* im = (*g_inputMgr_LL);
	Main_LL* m = (*g_Main_LL);

	bool current = CALL_MEMBER_FN(pc, CheckIfCameraStateIsSelected)(PlayerCamera::kCameraState_Free);
	if (current != activate && !GetFXCam(base))
	{
		if (!current)
			(*g_saveCurrentPlayerCameraState) = pc->unk30;
		CALL_MEMBER_FN(pc, ToggleFlyCam)(pause);
		current = CALL_MEMBER_FN(pc, CheckIfCameraStateIsSelected)(PlayerCamera::kCameraState_Free);
		pc->Unk_02(current  || (*g_saveCurrentPlayerCameraState));
		if (current)
			CALL_MEMBER_FN(im, ControlPlayerCamera)();
		else
			CALL_MEMBER_FN(im, ControlPlayer)();
	}
	else if (current)
	{
		m->pause = pause;
		CALL_MEMBER_FN(im, ControlPlayerCamera)();
	}
	return current;
}

float SetFlyCamSpeedMult(StaticFunctionTag *base, float speed, bool rotation=false)
{
	float result = 0.0;
	if (rotation)
	{
		SettingT_INISettingCollection_LL * rotation = g_fFreeCameraRotationSpeed_Camera;
		result = rotation->value;
		rotation->value = speed;
		CALL_MEMBER_FN(rotation, Changed)();
	}
	else
	{
		SettingT_INISettingCollection_LL * translation = g_fFreeCameraTranslationSpeed_Camera;
		result = translation->value;
		translation->value = speed;
		CALL_MEMBER_FN(translation, Changed)();
	}
	return result;
}

TESRace* GetActorPowerArmorRace(StaticFunctionTag *base, Actor* actor)	// returns the race of an actor when in power armor
{
	if (actor)
	{
		Actor_LL * actor_LL = Actor_LL::FromActor(actor);

		BGSPerk * paPerk = (BGSPerk *)LookupFormByID(0x0001F8A9);
		//if (CALL_MEMBER_FN(actor_LL, HasPerkInRank)(paPerk))
			if (actor->extraDataList)
			{
					ExtraPowerArmor_LL* epa = (ExtraPowerArmor_LL*) actor->extraDataList->GetByType(kExtraData_PowerArmor);
					if (epa && epa->race)
						return epa->race;
			}

		//if (actor->race)
		//	return actor->race;

		//if (actor->baseForm)
		//{
		//	TESNPC* npc = (TESNPC*)Runtime_DynamicCast(actor->baseForm, RTTI_TESForm, RTTI_TESNPC);
		//	if (npc)
		//		return npc->race.race;
		//}
	}
	// if stil nothing then...
	return NULL;
}

BSFixedString GetActorPowerArmorSkeleton(StaticFunctionTag *base, Actor* actor)	// returns the skeleton of an actor when in power armor
{
	if (!actor)
		return "";
	TESNPC * npc = NULL;
	BSFixedString result = "";
	if (actor->baseForm)
		npc = (TESNPC *)Runtime_DynamicCast(actor->baseForm, RTTI_TESForm, RTTI_TESNPC);
	TESRace* race = GetActorPowerArmorRace(base, actor);
	if (race && npc)
		result = race->models[CALL_MEMBER_FN(npc, GetSex)()].name;

	return result;
}

BSFixedString GetActorSkeleton(StaticFunctionTag *base, Actor* actor)
{
	if (!actor)
		return "";
	BSFixedString result = "";
	TESRace* race = NULL;;
	TESNPC * npc = NULL;

	if (actor->baseForm)
		npc = (TESNPC *)Runtime_DynamicCast(actor->baseForm, RTTI_TESForm, RTTI_TESNPC);

	if (actor->race)
		race = actor->race;

	if (!race && npc)
		race = npc->race.race;

	if (race && npc)
		result = race->models[CALL_MEMBER_FN(npc, GetSex)()].name;

	return result;
}

BSFixedString GetActorActualSkeleton(StaticFunctionTag *base, Actor* actor)
{
	if (!actor)
		return "";
	BSFixedString result = "";
	TESRace* race = NULL;;
	TESNPC * npc = NULL;

	if (actor->baseForm)
		npc = (TESNPC *)Runtime_DynamicCast(actor->baseForm, RTTI_TESForm, RTTI_TESNPC);

	if (npc)
		race = npc->race.race;

	if (race)
		result = race->models[CALL_MEMBER_FN(npc, GetSex)()].name;

	return result;
}


UInt32 GetActorFlags(StaticFunctionTag *base, Actor* akActor)
{
	return (akActor) ? akActor->actorFlags : 0;
}

bool ResetActorEssential(StaticFunctionTag * base, Actor* akActor)
{
	bool ret = false;

	if (akActor)
	{
		ExtraDataList *edl = akActor->extraDataList;

		if (edl->HasType(kExtraData_EssentialProtected))
		{
			BSExtraData* edp = akActor->extraDataList->GetByType(kExtraData_EssentialProtected);
			ret = edl->Remove(kExtraData_EssentialProtected, edp);
		}
	}
	return ret;
}


bool SetMinimalMaxArraySize(StaticFunctionTag *base, UInt32 newMaxArrraySize)
{
	return SetMinimalMaxArraySize_internal(newMaxArrraySize);
}

#define MAXDEPTH 4000

static int currDepth = 0;

bool GetInLeveledItem(TESLevItem *lev, TESForm *item)
{
	for (UInt16 i = 0; i < lev->leveledList.length; i++)
	{
		TESForm * member = lev->leveledList.entries[i].form;

		if (member && member->formID == item->formID)
			return true;
			
		TESLevItem *sublist = (TESLevItem*)Runtime_DynamicCast(member, RTTI_TESForm, RTTI_TESLevItem);

		if (sublist && currDepth < MAXDEPTH)
		try
		{
			++currDepth;
			bool r = GetInLeveledItem(sublist, item);
			--currDepth;
			if (r) 
				return r;
		}
		catch(...)
		{
			--currDepth;
			throw;
		}
	}
	return false;
}

bool GetInLeveledItem_(StaticFunctionTag *base, TESLevItem *lev, TESForm *item)
{
	if (item && lev)
		return GetInLeveledItem(lev, item);
	else
		return false;
}

#define MAXLEVEL 1234

SInt32 CheckOneBase(TESNPC * base, SInt32 sex, UInt32 * level);
SInt32 CheckOneLeveled(TESLevCharacter * lev, SInt32 sex, UInt32 * level)
{
	*level++;
	if (*level > MAXLEVEL)
		return -1;
	for (UInt16 i = 0; i < lev->leveledList.length; i++)
	{
		TESForm * member = lev->leveledList.entries[i].form;
		TESNPC * npc = (TESNPC*)Runtime_DynamicCast(member, RTTI_TESForm, RTTI_TESNPC);
		if (npc)
		{
			if (CheckOneBase(npc, sex, level) < 0)
				return -1;
			continue;
		}
		TESLevCharacter * lev = (TESLevCharacter*)Runtime_DynamicCast(member, RTTI_TESForm, RTTI_TESNPC);
		if (lev)
		{
			if (CheckOneLeveled(lev, sex, level) < 0)
				return -1;
			continue;
		}
		_AssertionFailed(NULL, 0, "Logic says it should be an NPC or a leveled character!!!");
	}
	return sex;
}

SInt32 CheckOneBase(TESNPC * base, SInt32 sex, UInt32 * level)
{
	*level++;
	if (*level > MAXLEVEL)
		return -1;
	if (CALL_MEMBER_FN(base, GetSex)() != sex)
		return -1;
	TESActorBaseData_LL * ab = (TESActorBaseData_LL *) (& base->actorData);
	if (((ab->useTemplateActors >> TESActorBaseData_LL::kTemplateTraits) & 1) == 1)
	{
		// TPLT Default Template is not loaded by runtime, so only check the template for traits
		if ((ab->templates) && *(ab->templates))
		{
			TESForm * templ = *(ab->templates)[0];
			TESNPC * npc = (TESNPC*)Runtime_DynamicCast(templ, RTTI_TESForm, RTTI_TESNPC);
			if (npc)
				return CheckOneBase(npc, sex, level);
			TESLevCharacter * lev = (TESLevCharacter*)Runtime_DynamicCast(templ, RTTI_TESForm, RTTI_TESNPC);
			if (lev)
				return CheckOneLeveled(lev, sex, level);
			_AssertionFailed(NULL, 0, "Logic says it should be an NPC or a leveled character!!!");
		}
	}
	return sex;
}

SInt32 GetActorPersistentSex(StaticFunctionTag *base, Actor* actor)	// return value: -1 if sex can change, 0 if always Male, 1 if always female
{
	SInt32 result = -1;
	TESNPC * npc = NULL;
	ExtraLeveledCreature_LL * elc = NULL;
	if (actor && actor->extraDataList)
		elc = (ExtraLeveledCreature_LL *)actor->extraDataList->GetByType(kExtraData_LeveledCreature);	// This is the editor baseForm
	if (elc)
		npc = (TESNPC*)Runtime_DynamicCast(elc->baseForm, RTTI_TESForm, RTTI_TESNPC);
	else
		npc = (TESNPC*)Runtime_DynamicCast(actor->baseForm, RTTI_TESForm, RTTI_TESNPC);
	if (npc)
	{
		SInt32 currentSex = npc->actorData.flags && TESActorBaseData::kFlagFemale ? 1 : 0;
		UInt32 level = 0;
		return CheckOneBase(npc, currentSex, &level);
	}
	return result;
}

// The next three functions are made with code from SKSE's PapyrusStringUtil.cpp file, and modified to work with F4SE. So credits to the extender team!

SInt32 StringFind(StaticFunctionTag* thisInput, BSFixedString theString, BSFixedString toFind, UInt32 startIndex)
{
	if (startIndex < 0)
		startIndex = 0;

	std::string s(theString);
	if (startIndex >= s.length())
		return -1;

	std::transform(s.begin(), s.end(), s.begin(), toupper);

	std::string f(toFind);
	std::transform(f.begin(), f.end(), f.begin(), toupper);

	std::string::size_type pos = s.find(f, startIndex);
	return (pos == std::string::npos) ? -1 : pos;
}

BSFixedString StringSubstring(StaticFunctionTag* thisInput, BSFixedString theString, UInt32 startIndex, UInt32 len)
{

	std::string nullStr;

	if (startIndex < 0)
		return nullStr.c_str();

	std::string tmpStr(theString);
	if (startIndex >= tmpStr.length())
		return nullStr.c_str();

	std::string sub = (len) ? tmpStr.substr(startIndex, len) : tmpStr.substr(startIndex);
	return sub.c_str();
}

VMArray<BSFixedString> StringSplit(StaticFunctionTag* thisInput, BSFixedString theString, BSFixedString theDelimiter)
{
	VMArray<BSFixedString> result;

	std::string str(theString);
	std::string delimiters(theDelimiter);

	std::string::size_type whitespacePos = str.find_first_of(" ");

	while (whitespacePos != std::string::npos) //eliminate whitespace. Taggers should tag with tags that have word starting with a capital letter. EG: "InsideOut", "TheQuickBrownFox"
	{
		str.erase(whitespacePos, 1);
		whitespacePos = str.find_first_of(" ");
	}

	std::string f(str); // Eliminate case sensitivity during find
	std::transform(f.begin(), f.end(), f.begin(), toupper);
	std::transform(delimiters.begin(), delimiters.end(), delimiters.begin(), toupper);

	std::string::size_type lastPos = f.find_first_not_of(delimiters, 0);
	std::string::size_type pos = f.find_first_of(delimiters, lastPos);

	while (std::string::npos != pos || std::string::npos != lastPos)
	{
		std::string token = str.substr(lastPos, pos - lastPos); // Pull from original string
		BSFixedString ConvToken = token.c_str(); //convert token into c string for BSFixedString
		result.Push(&ConvToken);
		lastPos = f.find_first_not_of(delimiters, pos);
		pos = f.find_first_of(delimiters, lastPos);
	}

	return result;
}

BSFixedString StringJoin(StaticFunctionTag* thisInput, VMArray<BSFixedString> theSplitString, BSFixedString theDelimiter) // This joins strings together, delimited by "," by default.
{
	BSFixedString result;
	std::string delimiter(theDelimiter);
	std::string preResult;
	BSFixedString bufferBS;
	std::string nullStr;

	if (theSplitString.Length() == 0)
	{
		return nullStr.c_str();
	}

	if (theSplitString.Length() == 1)
	{
		theSplitString.Get(&bufferBS, 0);
		return bufferBS;
	}

	for (UInt32 i = 0; i < theSplitString.Length(); i++)
	{
		if (i > 0)
		{
			theSplitString.Get(&bufferBS, i);
			std::string buffer(bufferBS);
			preResult = preResult + delimiter + buffer;
		}
		else
		{
			theSplitString.Get(&bufferBS, i);
			std::string buffer(bufferBS);
			preResult = buffer;
		}
	}

	result = preResult.c_str();

	return result;
}

BSFixedString IntToHexString(StaticFunctionTag* base, SInt32 iNum)
{
	char tmpstr[20];
	_itoa_s(iNum, tmpstr, 20, 16);
	return BSFixedString(tmpstr);
}

UInt32 HexStringToInt(StaticFunctionTag* base, BSFixedString theString)
{
	return(strtoul(theString.c_str(), nullptr, 16));
}

BSFixedString GetRaceEditorID(StaticFunctionTag* thisInput, TESRace* theForm)
{

	return BSFixedString(theForm->GetEditorID());

}

template<typename T>
VMArray<T> ResizeArray(StaticFunctionTag* thisInput, VMArray<T> theArray, SInt32 theSize, T fill)
//Most of this code also is from the SKSE cpps, so credits to the SKSE team once again!
//Honestly, this function and the papyrus functions based on it will probably be included in a F4SE release, so expect this to become deprecated.
{
	if (theSize < 0)
		return theArray;

	VMArray<T> result;

	result.Resize(theSize);

	UInt32 arrayLength = min(theArray.Length(), theSize); // Take the lower of the two, so if the size is decreasing, theSize becomes the terminator value for the loop. Otherwise, fill the new array with the old one's data.
	for (UInt32 i = 0; i < arrayLength; i++)
	{
		T value;
		theArray.Get(&value, i);
		result.Set(&value, i);
	}

	arrayLength = max(theArray.Length(), theSize); // Because VMArray doesn't have a Resize() function that takes two vars, we have to manually set the rest of an increased array based on the fill value.
	for (UInt32 i = theArray.Length(); i < arrayLength; i++) // For loop stops if arrayLength is the same as i, which will happen if the array size was decreased.
	{
		result.Set(&fill, i);
	}

	return result;
}

VMArray<VMVariable> ResizeVarArray2(StaticFunctionTag* thisInput, VMArray<VMVariable> theArray, SInt32 theSize/*, VMVariable fill*/)
{
	if (theSize < 0)
		return theArray;

	VMArray<VMVariable> result;

	result.Resize(theSize);

	// Var and None do not mix well, so let the standard lib initialize for us

	//UInt32 arrayLength = min(theArray.Length(), theSize); // Take the lower of the two, so if the size is decreasing, theSize becomes the terminator value for the loop. Otherwise, fill the new array with the old one's data.

	//arrayLength = max(theArray.Length(), theSize); // Because VMArray doesn't have a Resize() function that takes two vars, we have to manually set the rest of an increased array based on the fill value.
	//for (UInt32 i = theArray.Length(); i < arrayLength; i++) // For loop stops if arrayLength is the same as i, which will happen if the array size was decreased.
	//{
	//	if (!fill.IsNone())
	//		result.Set(&fill, i);
	//}

	return result;
}

VMArray<VMVariable> ResizeVarArray(StaticFunctionTag* thisInput, VMArray<VMVariable> theArray, SInt32 theSize, VMVariable fill)
{
	return ResizeVarArray2(thisInput, theArray, theSize);
}

BSFixedString GetCustomConfigPath_(StaticFunctionTag *base, BSFixedString name)
{
	return BSFixedString(GetCustomConfigPath(name).c_str());
};

BSFixedString GetCustomConfigOption_(StaticFunctionTag *base, BSFixedString name, BSFixedString section, BSFixedString key)
{
	return BSFixedString(GetCustomConfigOption(name, section, key).c_str());
};

bool SetCustomConfigOptions_(StaticFunctionTag *base, BSFixedString name, BSFixedString section, VMArray<BSFixedString> keys, VMArray<BSFixedString> values)
{
	return SetCustomConfigOptions(name, section, keys, values);
};

bool GetCustomConfigOptions_(StaticFunctionTag *base, BSFixedString name, BSFixedString section, VMArray<BSFixedString> keys, VMArray<BSFixedString> values)
{
	return GetCustomConfigOptions(name, section, keys, values);
};

VMArray<VMVariable> GetCustomConfigOptions_(StaticFunctionTag *base, BSFixedString name, BSFixedString section)
{
	return GetCustomConfigOptions(name, section);
};

VMArray<BSFixedString> GetCustomConfigSections_(StaticFunctionTag *base, BSFixedString name)
{
	return GetCustomConfigSections(name);
};

bool ResetCustomConfigOptions_(StaticFunctionTag *base, BSFixedString name, BSFixedString section, VMArray<BSFixedString> keys, VMArray<BSFixedString> values)
{
	return ResetCustomConfigOptions(name, section, keys, values);
};

UInt32 GetCustomConfigOption_UInt32_(StaticFunctionTag *base, BSFixedString name, BSFixedString section, BSFixedString key)
{
	UInt32 dataOut = 0;
	return GetCustomConfigOption_UInt32(name, section, key, &dataOut) ? dataOut : 0;
};

float GetCustomConfigOption_float_(StaticFunctionTag *base, BSFixedString name, BSFixedString section, BSFixedString key)
{
	float dataOut = 0;
	return GetCustomConfigOption_float(name, section, key, &dataOut) ? dataOut : 0;
};

bool SetCustomConfigOption_(StaticFunctionTag *base, BSFixedString name, BSFixedString section, BSFixedString key, BSFixedString value)
{
	return SetCustomConfigOption(name, section, key, value);
};

bool SetCustomConfigOption_UInt32_(StaticFunctionTag *base, BSFixedString name, BSFixedString section, BSFixedString key, UInt32 data)
{
	return SetCustomConfigOption_UInt32(name, section, key, data);
};

bool SetCustomConfigOption_float_(StaticFunctionTag *base, BSFixedString name, BSFixedString section, BSFixedString key, float data)
{
	return SetCustomConfigOption_float(name, section, key, data);
};

float GetLLFPPluginVersion(StaticFunctionTag *base)
{
	return pluginVersion;
};

BGSKeyword * GetKeywordByName(BSFixedString editorID)
{
	DataHandler* theDH = *g_dataHandler.GetPtr();
	for (UInt32 i = 0; i < theDH->arrKYWD.count; i++)
	{
		if (0 == _stricmp(theDH->arrKYWD[i]->keyword, editorID))
			return theDH->arrKYWD[i];
	}
	return NULL;
}

BGSKeyword * GetKeywordByName_(StaticFunctionTag *base, BSFixedString editorID)
{
	return GetKeywordByName(editorID);
}

VMArray<BGSKeyword*> GetAllKeywords()
{
	VMArray<BGSKeyword*> result;
	DataHandler* theDH = *g_dataHandler.GetPtr();
	for (UInt32 i = 0; i < theDH->arrKYWD.count; i++)
	{
		//BGSKeyword * k = theDH->arrKYWD[i];
		result.Push(&(theDH->arrKYWD[i]));
	}
	return result;
}

VMArray<BGSKeyword*> GetAllKeywords_(StaticFunctionTag *base)
{
	return GetAllKeywords();
}

// Does not to persist.
bool AddKeywordToForm(TESForm * thisForm, BGSKeyword * keyword)
{
	bool result = false;
	if (!thisForm || !keyword)
		return result;

	BGSKeywordForm* pKeywords = DYNAMIC_CAST(thisForm, TESForm, BGSKeywordForm);
	if (pKeywords) {
		for(UInt32 i = 0; i < pKeywords->numKeywords; i++)
		{
			if(pKeywords->keywords[i] && keyword == pKeywords->keywords[i])
				return false;
		}
		// This keyword is not yet applied to this form, let's add it.
		BGSKeyword	** newKeywords = (BGSKeyword **)Heap_Allocate(sizeof(BGSKeyword*)*(pKeywords->numKeywords+1));
		if (newKeywords)
		{
			for(UInt32 i = 0; i < pKeywords->numKeywords; i++)
				newKeywords[i] = pKeywords->keywords[i];
			newKeywords[pKeywords->numKeywords] = keyword;
			pKeywords->numKeywords++;
			Heap_Free(pKeywords->keywords);
			pKeywords->keywords = newKeywords;
			result = true;
		}
	}
	return result;
}

bool AddKeywordToForm_(StaticFunctionTag *base, TESForm * thisForm, BGSKeyword * keyword)
{
	return AddKeywordToForm(thisForm, keyword);
}

 
bool DelKeywordFromForm(TESForm * thisForm, BGSKeyword * keyword)
{
	bool result = false;
	if (!thisForm || !keyword)
		return result;

	BGSKeywordForm* pKeywords = DYNAMIC_CAST(thisForm, TESForm, BGSKeywordForm);
	if (pKeywords) {
		for(UInt32 i = 0; i < pKeywords->numKeywords; i++)
		{
			if(pKeywords->keywords[i] && keyword == pKeywords->keywords[i])
			{
				// This keyword is applied to this form, let's remove it.
				BGSKeyword	** newKeywords = NULL;
				if (1 < pKeywords->numKeywords)
				{
					// There will at least be one remaining keyword
					newKeywords = (BGSKeyword **)Heap_Allocate(sizeof(BGSKeyword*)*(pKeywords->numKeywords-1));
					if (newKeywords)
					{
						for(UInt32 j = 0; j < pKeywords->numKeywords; j++)
							if (j < i)
								newKeywords[j] = pKeywords->keywords[j];
							else
								if (j > i)
									newKeywords[j-1] = pKeywords->keywords[j];
					}
					else
						return result;
				}
				pKeywords->numKeywords--;
				Heap_Free(pKeywords->keywords);
				pKeywords->keywords = newKeywords;
				result = true;
			}
		}
	}
	return result;
}

bool DelKeywordFromForm_(StaticFunctionTag *base, TESForm * thisForm, BGSKeyword * keyword)
{
	return DelKeywordFromForm(thisForm, keyword);
}
 
// credits to shavkacagarikia for figuring out HasKeyword
typedef bool(*_IKeywordFormBase_HasKeyword)(IKeywordFormBase* keywordFormBase, BGSKeyword* keyword, UInt32 unk3);

template <typename T>
T GetVirtualFunction(void* baseObject, int vtblIndex) 
{
	uintptr_t* vtbl = reinterpret_cast<uintptr_t**>(baseObject)[0];
	return reinterpret_cast<T>(vtbl[vtblIndex]);
}

bool HasKeyword(TESForm* form, BGSKeyword* keyword) 
{
	if (form && keyword)
	{
		IKeywordFormBase* keywordFormBase = DYNAMIC_CAST(form, TESForm, IKeywordFormBase);
		if (keywordFormBase)
		{
			auto HasKeyword_Internal = GetVirtualFunction<_IKeywordFormBase_HasKeyword>(keywordFormBase, 1);
			if (HasKeyword_Internal(keywordFormBase, keyword, 0))
			{
				return true;
			}
		}
	}
	return false;
}

bool HasKeywordInFormList(TESForm *form, BGSListForm* formList)
{
	if (form && formList)
	{
		BGSKeyword *pKW;
		TESForm *pF;
		for (UInt32 i = 0; i < formList->forms.count; i++)
		{
			formList->forms.GetNthItem(i, pF);
			pKW = DYNAMIC_CAST(pF, TESForm, BGSKeyword);
			if (HasKeyword(form, pKW))
				return true;
		}
	}
	return false;
}
 
BSFixedString OriginalPluginName(TESForm* akForm)
{
	TESForm_LL * form = (TESForm_LL *)akForm;
	if (form)
	{
		if (form->formID > 0x01000000 && form->formID < 0xFF000000 && form->mods && form->mods->entries && form->mods->entries[0])
			return form->mods->entries[0]->name;
		else
			return "Fallout4.esm";
	}
	else
		return "";
}

UInt32 OriginalPluginID(TESForm* akForm)
{
	char name[0x0104];
	strcpy_s(name, OriginalPluginName(akForm));
	char * dot = strrchr(name, '.');
	//if (dot)
	//	for (char * c = dot; c ; c++)
	//		*c = toupper(*c);
	if (dot && 0==_stricmp(dot, ".ESL"))
		return akForm->formID & 0x0000FFFF;
	else
		return akForm->formID & 0x00FFFFFF;
}

BSFixedString OriginalPluginName_(StaticFunctionTag *base, TESForm* akForm)
{
	return OriginalPluginName(akForm);
}

UInt32 OriginalPluginID_(StaticFunctionTag *base, TESForm* akForm)
{
	return OriginalPluginID(akForm);
}

UInt32 GetRecordFlags(StaticFunctionTag *base, TESForm* akForm)
{
	return (akForm) ? akForm->flags : 0;
}

UInt32 GetFormType(StaticFunctionTag *base, TESForm* akForm)
{
	return (akForm) ? akForm->GetFormType() : 0;
}

bool IsPersistent(StaticFunctionTag *base, TESForm* akForm)
{
	bool result = false;

	if (akForm)
	{
		TESObjectREFR *ref = DYNAMIC_CAST(akForm, TESForm, TESObjectREFR);
		if (ref)
			result = CALL_MEMBER_FN(TESObjectREFR_LL::FromRef(ref), GetRefPersists)();
	}
	return result;
}

bool SetPersistent(StaticFunctionTag *base, TESForm* akForm, bool bValue)
{
	bool result = false;

	if (akForm)
	{
		TESObjectREFR *ref = DYNAMIC_CAST(akForm, TESForm, TESObjectREFR);
		if (ref)
		{
			CALL_MEMBER_FN(TESObjectREFR_LL::FromRef(ref), SetRefPersists)(bValue);
			result = true;
		}
	}
	return result;
}

float GetScale(StaticFunctionTag* base, TESObjectREFR* akRef)
{
	float result = 0.0;

	if (akRef)
	{
		result = CALL_MEMBER_FN(TESObjectREFR_LL::FromRef(akRef), GetScale)();
	}
	return result;
}

void SetScale(StaticFunctionTag *base, TESObjectREFR* akRef, float fScale)
{
	if (akRef)
	{
		CALL_MEMBER_FN(TESObjectREFR_LL::FromRef(akRef), SetScale)(fScale);
	}
}

VMArray<TESForm*> FormListToArray(StaticFunctionTag* base, BGSListForm* formList)
{
	VMArray<TESForm*> result;
	if (formList)
	{
		for (UInt32 i = 0; i < formList->forms.count; i++)
		{
			TESForm* form = nullptr;
			formList->forms.GetNthItem(i, form);
			if (form)
				result.Push(&form);
		}
	}
	return result;
}

UInt32 GetFullFormID(StaticFunctionTag *base, BSFixedString modName, UInt32 formID)
{
	if (!modName)
		return 0;

	const ModInfo *mod = (*g_dataHandler)->LookupModByName(modName.c_str());

	if (mod && mod->modIndex != -1) {
		if (mod->recordFlags & (1 << 9)) { // ESL
			formID &= 0xFFF;
			formID |= 0xFE << 24;
			formID |= mod->lightIndex << 12;
		}
		else {
			formID &= 0xFFFFFF;
			formID |= (mod->modIndex) << 24;
		}
		return formID;
	}
	return 0;
}

TESForm * GetFormFromPlugin(StaticFunctionTag *base, BSFixedString modName, UInt32 formID)
{
	UInt32 fullFormID = GetFullFormID(base, modName, formID);

	return LookupFormByID(fullFormID);
}

VMArray<TESForm *> GetFormArray(StaticFunctionTag *base, VMArray<BSFixedString> modNames, VMArray<UInt32> formIDs)
{
	VMArray<TESForm *> result;
	BSFixedString modName;
	UInt32 formID;
	TESForm *formptr;

	if (modNames.Length() != formIDs.Length())
		return result;

	UInt32 mln = modNames.Length();
	for (UInt32 i = 0; i < mln; i++)
	{
		modNames.Get(&modName, i);
		formIDs.Get(&formID, i);
		formptr = GetFormFromPlugin(base, modName, formID);
		if (formptr)
			result.Push(&formptr);
	}
	return result;
}

// Given an array of FormID Ints that was packed as Var via VarArrayToVar(), return the array of corresponding forms
VMArray<TESForm *> GetFormArrayFromVar(StaticFunctionTag *base, VMVariable var)
{
	VMArray<TESForm *> result;

	VMArray<VMVariable> vars;
	var.Get<VMArray<VMVariable>>(&vars);
	
	UInt32 formID;
	VMVariable tmpVar;
	TESForm *formPtr;
	UInt32 len = vars.Length();

	for (UInt32 i = 0; i < len; i++)
	{
		vars.Get(&tmpVar, i);
		tmpVar.Get(&formID);
		formPtr = LookupFormByID(formID);
		if (formPtr)
			result.Push(&formPtr);
	}
	return result;
}

// 04B0AC0 void __fastcall BGSListForm::AddScriptAddedForm(BGSListForm *this, struct TESForm *akForm)
typedef bool(*_AddScriptAddedForm)(BGSListForm *FormList, TESForm *akForm);
RelocAddr<_AddScriptAddedForm> AddScriptAddedForm(0x00535230	/* 0535200 04B0AC0 */);

void PopulateFormlist(StaticFunctionTag *base, BGSListForm *FormList, VMArray<BSFixedString> modNames, VMArray<UInt32> formIDs)
{
	BSFixedString modName;
	UInt32 formID;
	TESForm *formPtr;

	if (modNames.Length() != formIDs.Length() || !FormList)
		return;

	UInt32 mln = modNames.Length();
	for (UInt32 i = 0; i < mln; i++)
	{
		modNames.Get(&modName, i);
		formIDs.Get(&formID, i);
		formPtr = GetFormFromPlugin(base, modName, formID);
		if (formPtr)
		{
			AddScriptAddedForm(FormList, formPtr);
		}
	}
	return;
}


void PlayIdle(StaticFunctionTag *base, Actor* actor, TESForm* idle)
{
	if(!actor || !idle)
		return;

	// There is probably a better way to do this
	Actor_LL::AIProcess_LL *middleProcess = static_cast<Actor_LL::AIProcess_LL *>(actor->middleProcess);

	TESForm *tF = (TESForm*)DYNAMIC_CAST(actor, Actor, TESForm);
	if (middleProcess && !(tF->flags & (TESForm::kFlag_IsDeleted | TESForm::kFlag_IsDisabled)))
		CALL_MEMBER_FN(middleProcess, PlayIdle)(actor, 0x35, idle, false, nullptr);
}


void PlayMultipleIdles(StaticFunctionTag *base, VMArray<Actor*> actors, VMArray<TESForm*> idles)
{
	int numActors = actors.Length();
	if (numActors == idles.Length())
	{
		Actor **tActors = new Actor*[numActors];
		TESForm **tIdles = new TESForm *[numActors];
		Actor_LL::AIProcess_LL**tMiddles = new Actor_LL::AIProcess_LL *[numActors];

		int j = 0;
		// Unpack everything to reduce latency. Skip deleted and disabled Actors.
		for (int i = 0; i < numActors; i++)
		{
			actors.Get(&tActors[j], i);
			idles.Get(&tIdles[j], i);
			if (tActors[j] && tIdles[j])
			{
				tMiddles[j] = static_cast<Actor_LL::AIProcess_LL*>(tActors[j]->middleProcess);
				if (tMiddles[j])
				{
					TESForm *tF = (TESForm*)DYNAMIC_CAST(tActors[j], Actor, TESForm);
					if ((tF->flags & (TESForm::kFlag_IsDeleted | TESForm::kFlag_IsDisabled)) == 0)
						++j;
				}
			}
		}

		for (int i = 0; i < j; i++)
			CALL_MEMBER_FN(tMiddles[i], PlayIdle)(tActors[i], 0x35, tIdles[i], false, nullptr);

		delete(tActors);
		delete(tIdles);
		delete(tMiddles);
	}
}

void StopCurrentIdle(StaticFunctionTag* base, Actor* actor)
{
	if (actor)
	{
		Actor_LL::AIProcess_LL* middleProcess = static_cast<Actor_LL::AIProcess_LL*>(actor->middleProcess);

		if (middleProcess)
			CALL_MEMBER_FN(middleProcess, StopCurrentIdle)(actor, true, true);
	}
}

// code originally from HUD++ plugin by kassent (on GitHub)

struct ViewCasterUpdateEvent_LL
{
    struct ViewCasterValueEvent_LL
    {
      UInt32			crosshairHandler;	// 00	RefHandler
      UInt32			unk04;				// 04
      UInt32			unk08;				// 08
      UInt32			unk0C;				// 0C
      UInt64			unk10;				// 10
      UInt64			unk18;				// 18
      UInt32			unk20;				// 20
      UInt32			unk24;				// 24
    };
	ViewCasterValueEvent_LL value;			// 00
	bool					unk28;			// 28
	UInt32					unk2C;			// 2C
	bool					isNPC;			// 30 ?
	float					unk34;			// 34
	bool					canInteractive;	// 38
};
STATIC_ASSERT(sizeof(ViewCasterUpdateEvent_LL) == 0x40);

template <typename EventT>
class BSTEventDispatcher_LL	// BSTGlobalEvent::EventSource<typename EventT> : public struct BSTSingletonSDM<EventT,BSTSingletonSDMOpStaticBuffer>, BSTEventSink<BSTGlobalEvent::KillSDMEvent>, BSTEventSource<ViewCasterUpdateEvent>
{
public:
	typedef BSTEventSink<EventT> SinkT;

	bool AddEventSink(SinkT * sink)
	{
		SimpleLocker locker(&lock);

		// Check for duplicate first
		for (int i = 0; i < eventSinks.count; i++)
		{
			if(eventSinks[i] == sink)
				return false;
		}

		if(!eventSinks.Insert(0, sink))
			eventSinks.Push(sink);
		return true;
	}

	void RemoveEventSink(SinkT * sink)
	{
		SimpleLocker locker(&lock);

		for (int i = 0; i < eventSinks.count; i++)
		{
			if(eventSinks[i] == sink) {
				eventSinks.Remove(i);
				break;
			}
		}
	}

	SimpleLock			lock;				// 000
	tArray<SinkT*>		eventSinks;			// 008
	tArray<SinkT*>		addBuffer;			// 020
	tArray<SinkT*>		removeBuffer;		// 038
	bool				stateFlag;			// 050
	char				pad[3];
};

#define kEventSource 0x02FF12F0	// 2FF02F0	// 5A5F758	// 5A5F728	// 5A987A8	
	// BSTGlobalEvent::EventSource_ViewCasterUpdateEvent_::Func0000+22
	// g_BSTGlobalEvent::EventSource_ViewCasterUpdateEvent_ or __ICI__BSTValueEventSource_ViewCasterUpdateEvent___Func0000_to0053_to0051_to0160+24
typedef BSTEventDispatcher<ViewCasterUpdateEvent_LL> EventDispatcher;
typedef BSTGlobalEvent::EventSource<ViewCasterUpdateEvent_LL> EventSource;
RelocPtr<EventSource *> g_Source(kEventSource);

static UInt32 CrosshairRefHandle = 0;		// Zero if the crosshair is no longer on a reference
static UInt32 CrosshairActorHandle = 0;		// Memorize the last one until a new one is pointed at

class CrosshairChangeHandler_LL : public BSTEventSink<ViewCasterUpdateEvent_LL>
{
public:
	virtual ~CrosshairChangeHandler_LL() { };
	virtual	EventResult	ReceiveEvent(ViewCasterUpdateEvent_LL * evn, void * dispatcher) override
	{
		NiPointer<TESObjectREFR> pRef = nullptr;
		CrosshairRefHandle = evn->value.crosshairHandler;

		LookupREFRByHandle(CrosshairRefHandle, pRef);
		if (pRef != nullptr)
		{
			Actor* a = (Actor*)DYNAMIC_CAST(pRef, TESObjectREFR, Actor);
			if (a)
				CrosshairActorHandle = CrosshairRefHandle;
		}
		return kEvent_Continue;
	}

	static void Register()
	{
		static CrosshairChangeHandler_LL * pCrosshairHandler = new CrosshairChangeHandler_LL();
		EventSource* e = (*g_Source);

		if (e != nullptr)
		{
			if (e->eventDispatcher.AddEventSink(pCrosshairHandler))
				_MESSAGE("Registered crosshair update handler.");

			CrosshairRefHandle = 0;
			CrosshairActorHandle = 0;
		}
		else
			_MESSAGE("Can't register crosshair update handler...");
	}
};

void MessageCallback(F4SEMessagingInterface::Message * msg)
{
	if (msg->type == F4SEMessagingInterface::kMessage_PostLoadGame || msg->type == F4SEMessagingInterface::kMessage_NewGame)
	{
		CrosshairChangeHandler_LL::Register();
	}
}

TESObjectREFR * LastCrossHairRef(StaticFunctionTag *base)
{
	NiPointer<TESObjectREFR> pRef = nullptr;
	LookupREFRByHandle(CrosshairRefHandle, pRef);
	return pRef;
}

Actor * LastCrossHairActor(StaticFunctionTag *base)
{
	NiPointer<TESObjectREFR> pRef = nullptr;
	Actor* a = nullptr;
	LookupREFRByHandle(CrosshairActorHandle, pRef);
	if (pRef != nullptr)
	{
		TESForm *tF = (TESForm*)DYNAMIC_CAST(pRef, TESObjectREFR, TESForm);
		if ((tF->flags & (TESForm::kFlag_IsDeleted | TESForm::kFlag_IsDisabled)) == 0)
			a = (Actor*)DYNAMIC_CAST(pRef, TESObjectREFR, Actor);
	}
	return a;
}

#define kFlag_CollisionState 11 // TESObjectREFR::LoadedData::kFlag_CollisionState

#define k_collisionState 0x02E66414	// 2E65414	// 58D08B0	// 5909930	// Cmd_ToggleCollision_Execute offset 0x055
RelocPtr <byte> g_collisionState(k_collisionState);

bool ObjectReferenceSetCollision(StaticFunctionTag * base, TESObjectREFR* object, bool state = true)
{
	if (!object)
	{
		bool current = *g_collisionState ? true : false;
		if (state!=current)
			*g_collisionState = state;
		return current;
	}
	else
		if (object->unkF0)
		{
			bool current = object->unkF0->flags & kFlag_CollisionState ? true : false;
			if (state)
				object->unkF0->flags |= kFlag_CollisionState;
			else
				object->unkF0->flags &= !kFlag_CollisionState;
			return current;
		}
		else
			return true;
}

bool ObjectReferenceGetCollision(StaticFunctionTag * base, TESObjectREFR* object)
{
	if (!object)
		return *g_collisionState ? true : false;
	else
		if(object->unkF0)
			return object->unkF0->flags & kFlag_CollisionState ? true : false;
		else
			return true;
}

// ??_7ExtraTextDisplayData@@6B@
RelocAddr <uintptr_t> s_ExtraTextDisplayDataVtbl(0x0227CD70	/* 227CD70	2C52788 2C52778, 2C7C7C8 */ );

bool ObjectReferenceSetSimpleDisplayName(StaticFunctionTag * base, TESObjectREFR* object, BSFixedString text)
{

	if (!object)
		return false;
	ExtraTextDisplayData * dd = NULL;
	if (text.data)
	{
		StringCache::Entry * e = text.data;
		char * c = e->Get<char>() ;
		if (c && *c)
		{
			dd = (ExtraTextDisplayData*)BSExtraData::Create(sizeof(ExtraTextDisplayData), s_ExtraTextDisplayDataVtbl.GetUIntPtr());
			if (dd)
			{
				((BSExtraData*)dd)->type = kExtraData_TextDisplayData;
				dd->name = text;
				dd->message = nullptr;
				dd->quest = nullptr;
				dd->type = -2;
				dd->textReplaceData = NULL;
				dd->nameLength = strlen(c);
			}
		}
	}
	return object->extraDataList->Add(kExtraData_TextDisplayData, dd);
}

void SetRefAngle(StaticFunctionTag *base, TESObjectREFR *ref, float degX, float degY, float degZ)
{
	if (ref)
	{
		float radX = degX * M_PI / 180.0;
		float radY = degY * M_PI / 180.0;
		float radZ = degZ * M_PI / 180.0;
		NiPoint3 newPos(radX, radY, radZ);
		CALL_MEMBER_FN((TESObjectREFR_LL *)ref, SetAngleOnReference)(newPos);
	}
}

void SetRefNoCollision(StaticFunctionTag *base, TESObjectREFR *ref, bool state)
{
	if (ref)
	{
		CALL_MEMBER_FN((TESObjectREFR_LL *)ref, SetNoCollision)(state);
	}
}

// The next two functions are made with code from SKSE's PapyrusCell.cpp. Credits to the SKSE team.

UInt32 GetNumRefsInCell(StaticFunctionTag *base, TESObjectCELL* thisCell, UInt32 formType)
{
	if (!thisCell)
		return 0;

	if (formType == 0)
		return thisCell->objectList.count;

	// match the formType instead
	UInt32 numMatching = 0;
	UInt32 numRefs = thisCell->objectList.count;
	for (UInt32 n = 0; n < numRefs; ++n)
	{
		TESObjectREFR* pRef = NULL;
		thisCell->objectList.GetNthItem(n, pRef);
		if (pRef && (pRef->formType == formType || pRef->baseForm->formType == formType))
			numMatching++;
	}
	return numMatching;
}

TESObjectREFR * GetNthRefInCell(StaticFunctionTag *base, TESObjectCELL * thisCell, UInt32 index, UInt32 formType)
{
	if (!thisCell)
		return NULL;

	TESObjectREFR* pRef = NULL;


	if (formType == 0) {
		thisCell->objectList.GetNthItem(index, pRef);
		return pRef;
	}

	UInt32 numMatching = 0;
	UInt32 numRefs = thisCell->objectList.count;
	for (UInt32 n = 0; n < numRefs; ++n) {
		thisCell->objectList.GetNthItem(n, pRef);
		if (pRef && (pRef->formType == formType || pRef->baseForm->formType == formType)) {
			if (numMatching++ == index) {
				return pRef;
			}
		}
	}
	return NULL;
}


bool ActorBaseIsClean(StaticFunctionTag * base, Actor* actor)
{

	if (!actor)
		return false;
	TESNPC * npc = DYNAMIC_CAST(actor->baseForm, TESForm, TESNPC);
	return npc ? npc->skinForm.skin == nullptr : false;
}


TESForm * GetActorBaseSkinForm(StaticFunctionTag * base, Actor* actor)
{
	if (!actor)
		return nullptr;

	TESNPC * npc = DYNAMIC_CAST(actor->baseForm, TESForm, TESNPC);

	if (npc && npc->skinForm.skin)
		return DYNAMIC_CAST(npc->skinForm.skin, TESObjectARMO, TESForm);

	if (npc && npc->race.race)
			if (npc->race.race->skinForm.skin)
					return DYNAMIC_CAST(npc->race.race->skinForm.skin, TESObjectARMO, TESForm);

	return nullptr;
}

bool CopyActorBaseskinForm(StaticFunctionTag * base, Actor* srcActor, Actor* dstActor)
{
	if (!srcActor || !dstActor)
		return false;

	TESNPC * srcnpc = DYNAMIC_CAST(srcActor->baseForm, TESForm, TESNPC);
	TESNPC * dstnpc = DYNAMIC_CAST(dstActor->baseForm, TESForm, TESNPC);

	if (!srcnpc || !dstnpc)
		return false;

	// if the skins are the same, nothing to do
	if (srcnpc->skinForm.skin != dstnpc->skinForm.skin) {

		// Actorbase skins are different, assign the new skin
		dstnpc->skinForm.skin = srcnpc->skinForm.skin;
	
		// Update the actor ref - skin only so this is less expensive than QueueUpdate()
		auto aiProcess = dstActor->middleProcess;
		if(aiProcess) 
			aiProcess->DoUpdate3dModel(dstActor, 0x11);

		return true;
	}

	return false;
}

// MFG morph data is stored in a float array at offset 0x3C8 in Actor::MiddleProcess::Data08
// morph values are stored starting at index 0x3C
// there are 0x36 morph values
float *GetMorphData(Actor *actor)
{
	if (!actor)
		return nullptr;

	auto aiProcess = actor->middleProcess;
	if (!aiProcess)
		return nullptr;

	auto data08 = aiProcess->unk08;
	if (!data08)
		return nullptr;

	float *unk3C8 = (float *)data08->unk00[0x3C8 >> 3];
	if (!unk3C8)
		return nullptr;

	return &unk3C8[0x3C];
}

//v24 = (float)v26 * 0.0099999998;
//if (v25 < 0x36 && v24 >= 0.0 && v24 <= 1.0)
//	*(float *)&v16[v25 + 0x3C] = v24;
bool MfgMorph_internal(Actor* actor, SInt32 morphID, SInt32 intensity)
{
	float *morphData = GetMorphData(actor);
	if (morphData)
	{
		float  fIntensity = (float)intensity * 0.0099999998;
		if (morphID > 0 && morphID < 0x36 && fIntensity >= 0.0 && fIntensity <= 1.0)
			morphData[morphID] = fIntensity;		
		return true;
	}
	return false;
}

bool MfgMorph(StaticFunctionTag* base, Actor* actor, SInt32 morphID, SInt32 intensity)
{
	return MfgMorph_internal(actor, morphID, intensity);
}

bool MfgResetMorphs(StaticFunctionTag* base, Actor* actor)
{
	float *morphData = GetMorphData(actor);
	if (morphData)
	{
		for (int i = 1; i < 0x36; i++)
			morphData[i] = 0.0;	
		return true;
	}
	return false;
}

VMArray<float> MfgSaveMorphs(StaticFunctionTag* base, Actor* actor)
{
	VMArray<float> result;
	float *morphData = GetMorphData(actor);
	if (morphData)
	{
		for (int i = 0; i < 0x36; i++)
			result.Push(&morphData[i]);
		return result;
	}
	result.SetNone(true);
	return result;
}

bool MfgRestoreMorphs(StaticFunctionTag* base, Actor* actor, VMArray<float> values)
{
	float *morphData = GetMorphData(actor);
	if ((morphData) && (values.Length() == 0x36))
	{
		for (int i = 1; i < 0x36; i++)
			values.Get(&morphData[i], i);
		return true;
	}
	return false;
}

bool MfgCopyMorphs(StaticFunctionTag* base, Actor* actor0, Actor* actor1)
{
	float *src = GetMorphData(actor0);
	float *dst = GetMorphData(actor1);
	if (src && dst)
	{
		for (int i = 1; i < 0x36; i++)
			dst[i] = src[i];
	}
	return true;
}

bool MfgApplyMorphSet(StaticFunctionTag* base, Actor* actor, VMArray<SInt32> morphIDs, VMArray<SInt32> values)
{
	float *morphData = GetMorphData(actor);
	if (!morphData)
		return false;

	if (morphIDs.Length() != values.Length())
		return false;

	SInt32 id;
	SInt32 val;
	for (int i = 0; i < values.Length(); i++)
	{
		morphIDs.Get(&id, i);
		values.Get(&val, i);
		float  fIntensity = (float)val * 0.0099999998;
		if (id > 0 && id < 0x36 && fIntensity >= 0.0 && fIntensity <= 1.0)
			morphData[id] = fIntensity;
		else
			return false;
	}
	return true;
}

// From Skyrim's PapyrusUtil
bool PrintConsole(StaticFunctionTag* base, BSFixedString text) {
	if (!text.c_str())
		return false;
	else if (strlen(text.c_str()) < 1000)
		Console_Print(text.c_str());
	else { // Large strings printed to console crash the game - truncate it
		std::string msg = text.c_str();
		msg.resize(997);
		msg.append("...");
		Console_Print(msg.c_str());
	}
	return true;
}

// ---------------------------------------------------------------
//	fedim's опусы

//	from https://audiocoding.ru/articles/2008-05-22-wav-file-structure/

struct WAVHEADER
{
	// WAV-формат начинается с RIFF-заголовка:

	// Содержит символы "RIFF" в ASCII кодировке
	// (0x52494646 в big-endian представлении)
	char chunkId[4];

	// 36 + subchunk2Size, или более точно:
	// 4 + (8 + subchunk1Size) + (8 + subchunk2Size)
	// Это оставшийся размер цепочки, начиная с этой позиции.
	// Иначе говоря, это размер файла - 8, то есть,
	// исключены поля chunkId и chunkSize.
	unsigned long chunkSize;

	// Содержит символы "WAVE"
	// (0x57415645 в big-endian представлении)
	char format[4];

	// Формат "WAVE" состоит из двух подцепочек: "fmt " и "data":
	// Подцепочка "fmt " описывает формат звуковых данных:

	// Содержит символы "fmt "
	// (0x666d7420 в big-endian представлении)
	char subchunk1Id[4];

	// 16 для формата PCM.
	// Это оставшийся размер подцепочки, начиная с этой позиции.
	unsigned long subchunk1Size;

	// Аудио формат, полный список можно получить здесь http://audiocoding.ru/wav_formats.txt
	// Для PCM = 1 (то есть, Линейное квантование).
	// Значения, отличающиеся от 1, обозначают некоторый формат сжатия.
	unsigned short audioFormat;

	// Количество каналов. Моно = 1, Стерео = 2 и т.д.
	unsigned short numChannels;

	// Частота дискретизации. 8000 Гц, 44100 Гц и т.д.
	unsigned long sampleRate;

	// sampleRate * numChannels * bitsPerSample/8
	// байт в секунду
	unsigned long byteRate;

	// numChannels * bitsPerSample/8
	// Количество байт для одного сэмпла, включая все каналы.
	unsigned short blockAlign;

	// Так называемая "глубиная" или точность звучания. 8 бит, 16 бит и т.д.
	unsigned short bitsPerSample;

	// Подцепочка "data" содержит аудио-данные и их размер.

	// Содержит символы "data"
	// (0x64617461 в big-endian представлении)
	char subchunk2Id[4];

	// numSamples * numChannels * bitsPerSample/8
	// Количество байт в области данных.
	unsigned long subchunk2Size;

	// Далее следуют непосредственно Wav данные.
};

float GetWaveDuration(std::string fn)
{
	WAVHEADER header;
	FILE* file;
	errno_t err;
	err = fopen_s(&file, fn.c_str(), "rb");
	if (err)
	{
		return 0;
	}
	fread_s(&header, sizeof(WAVHEADER), sizeof(WAVHEADER), 1, file);
	fclose(file);
	
	return float( header.subchunk2Size / header.byteRate * 100);
}

void Exec(StaticFunctionTag* base, BSFixedString str)
{
	UINT res = WinExec((const char*)str.data, SW_SHOWMINIMIZED);
}

bool FileExist(std::string fName) {
	fstream f(fName);
	if(f.bad())
		return false;
	if (f.is_open())
	{
		f.close();
		Sleep(100);
	}
	return true;
}

void PlaySoundWav(StaticFunctionTag* base, BSFixedString str) {
	if (str == "")
		PlaySound(NULL, NULL, SND_PURGE);
	else
	{
		std::string	runtimePath = GetRuntimeDirectory() + "Data\\Sound\\Voice\\";

		std::string fn = runtimePath + (const char*)str.data;
		_DMESSAGE(fn.c_str());
		if (FileExist(fn))
		{
			PlaySound(fn.c_str(), NULL, SND_FILENAME | SND_ASYNC);  //| SND_LOOP);
		}
	}
}

UInt32 GetVolumeWav(StaticFunctionTag* base)
{
	UINT uRetVal;
	DWORD volume;
	WAVEOUTCAPS waveCaps;

	if (!waveOutGetDevCaps(0, (LPWAVEOUTCAPS)&waveCaps, sizeof(WAVEOUTCAPS)))
	{
		// Verify the device supports volume changes
		if (waveCaps.dwSupport & WAVECAPS_VOLUME)
		{
			uRetVal = waveOutGetVolume(0, (LPDWORD)&volume);
		}
	}
	return volume;
}

UInt32 SetVolumeWav(StaticFunctionTag* base, UInt32 volumeA, UInt32 volumeB)
{
	UINT uRetVal;
	//DWORD volume;
	WAVEOUTCAPS waveCaps;

	if (!waveOutGetDevCaps(0, (LPWAVEOUTCAPS)&waveCaps, sizeof(WAVEOUTCAPS)))

	{
		// Verify the device supports volume changes
		if (waveCaps.dwSupport & WAVECAPS_VOLUME)
		{
			// The low word is the left volume, the high word is the right.
			// Set left channel: 2000h is one-eighth volume (8192 base ten).
			// Set right channel: 4000h is quarter volume (16384 base ten).
			uRetVal = waveOutSetVolume(0, MAKELONG(volumeA, volumeB));

		}
	}
	return uRetVal;
}

BSFixedString VoiceMessage(StaticFunctionTag* base, BSFixedString plugin, BSFixedString text)
{
	BSFixedString section = text;

	std::string	s_runtimePath = GetRuntimeDirectory();
	if (!s_runtimePath.empty())
	{
		std::string fname = plugin.c_str();
		fname += ".ini";

		_DMESSAGE(fname.c_str());

		VMArray<BSFixedString> keys, values;
		if (GetCustomConfigOptions(fname.c_str(), section, keys, values))
		{
			UInt32 sc = keys.Length();
			if (sc > 1)
			{
				srand(time(NULL));
				int r = rand() % (sc);
				sc = r + 1;
			}
			if (sc > 0)
			{
				sc--;
				BSFixedString fnm;
				keys.Get(&fnm, sc);
				values.Get(&section, sc);

				std::string s_voicePath = s_runtimePath + "Data\\Sound\\Voice\\" + plugin.c_str();
				if(GetFileAttributes(s_voicePath.c_str()) == INVALID_FILE_ATTRIBUTES)
					s_voicePath += ".esp";

				fname = s_voicePath + "\\" + fnm.c_str() + ".wav";

				if (FileExist(fname))
					while (!PlaySound(fname.c_str(), NULL, SND_FILENAME | SND_ASYNC | SND_NOSTOP))  //| SND_LOOP);
					{
						Sleep(100);
					}
			}
		}
	}
	return BSFixedString(section);
}

// GetINI functions modified from SKSE

float GetINIFloat(StaticFunctionTag* base, BSFixedString ini)
{
	Setting * setting = GetINISetting(ini.c_str());

	if (!setting || setting->GetType() != Setting::kType_Float)
		return 0.0;

	return setting->data.f32;
}

UInt32 GetINIInt(StaticFunctionTag* base, BSFixedString ini)
{
	Setting * setting = GetINISetting(ini.c_str());

	if (!setting || setting->GetType() != Setting::kType_Integer)
		return 0;

	return setting->data.s32;
}

bool GetINIBool(StaticFunctionTag* base, BSFixedString ini)
{
	Setting * setting = GetINISetting(ini.c_str());

	if (!setting || setting->GetType() != Setting::kType_Bool)
		return false;

	return setting->data.u8 >= 1 ? true : false;
}

BSFixedString GetINIString(StaticFunctionTag* base, BSFixedString ini)
{
	BSFixedString result;
	Setting * setting = GetINISetting(ini.c_str());

	if (setting && setting->GetType() == Setting::kType_String)
		result = setting->data.s;

	return result;
}

// Actor functions

void SetActorPosition(StaticFunctionTag *base, Actor *akActor, float X, float Y, float Z)
{
	if (akActor)
	{
		NiPoint3 newPos(X, Y, Z);
		Actor_LL* actor_LL = Actor_LL::FromActor(akActor);
		CALL_MEMBER_FN(actor_LL, SetPosition)(newPos, true);
	}
}

void EnableActorCollision(StaticFunctionTag* base, Actor* akActor)
{
	if (akActor)
	{
		Actor_LL* actor_LL = Actor_LL::FromActor(akActor);
		CALL_MEMBER_FN(actor_LL, EnableCollision)();
	}
}

void DisableActorCollision(StaticFunctionTag* base, Actor* akActor)
{
	if (akActor)
	{
		Actor_LL* actor_LL = Actor_LL::FromActor(akActor);
		CALL_MEMBER_FN(actor_LL, DisableCollision)();
	}
}

void ActorStopInteractingQuick(StaticFunctionTag* base, Actor* akActor)
{
	if (akActor)
	{
		Actor_LL* actor_LL = Actor_LL::FromActor(akActor);
		CALL_MEMBER_FN(actor_LL, StopInteractingQuick)(true, true, true);
	}
}

// Update actor 3D equipment only(less expensive than QueueUpdate 0xC)
void ActorUpdateEquipment(StaticFunctionTag* base, Actor* actor)
{
	if (actor)
	{
		auto aiProcess = actor->middleProcess;
		if (aiProcess)
			aiProcess->DoUpdate3dModel(actor, 0x11);
	}
}

// Force AI package
void ActorSetPackageOverride(StaticFunctionTag *base, Actor *actor, TESForm *pkg)
{
	if (actor && pkg && pkg->formType == kFormType_PACK)
	{
		Actor_LL *a = Actor_LL::FromActor(actor);
		auto middleProcess = a->middleProcess;
		if (middleProcess)
		{
			Actor_LL::AIProcess_LL*mp = Actor_LL::AIProcess_LL::FromAIProcess(middleProcess);
			CALL_MEMBER_FN(mp, ClearCurrentPackage)(actor);
			CALL_MEMBER_FN(mp, ClearCurrentDataforProcess)(actor);
			CALL_MEMBER_FN(a, PutCreatedPackage)(pkg, false, false, true);
			CALL_MEMBER_FN(a, EvaluatePackage)(false, false);
		}
	}
}

// Clear AI package override
void ActorClearPackageOverride(StaticFunctionTag *base, Actor *actor)
{
	if (actor)
	{
		Actor_LL *a = Actor_LL::FromActor(actor);
		auto middleProcess = a->middleProcess;
		if (middleProcess)
		{
			Actor_LL::AIProcess_LL*mp = Actor_LL::AIProcess_LL::FromAIProcess(middleProcess);
			CALL_MEMBER_FN(mp, ClearCurrentPackage)(actor);
			CALL_MEMBER_FN(mp, ClearCurrentDataforProcess)(actor);
			CALL_MEMBER_FN(a, EvaluatePackage)(false, false);
		}
	}
}

// Update Player Character 3D
void PlayerUpdateEquipment(StaticFunctionTag* base)
{
	PlayerCharacter_LL *playerRef = PlayerCharacter_LL::FromPlayerCharacter(*g_player);
	CALL_MEMBER_FN(playerRef, UpdatePlayer3d)();
}

// From F4SE PapyrusActor.c
TESObjectREFR * GetFurnitureReference(Actor * actor)
{
	NiPointer<TESObjectREFR> refr;
	if (!actor)
		return nullptr;

	auto aiProcess = actor->middleProcess;
	if (!aiProcess)
		return nullptr;

	UInt32 furnitureHandle = 0;
	auto data08 = aiProcess->unk08;
	if (!data08)
		return nullptr;

	if (actor->actorState.flags & (ActorState::Flags::kUnk1 | ActorState::Flags::kUnk2))
		furnitureHandle = data08->furnitureHandle2;
	else
		furnitureHandle = data08->furnitureHandle1;

	LookupREFRByHandle(furnitureHandle, refr);
	return refr;
}

TESObjectREFR * GetActorInFurniture(TESObjectREFR *furnRef)
{
	if (!furnRef)
		return nullptr;

	if (furnRef->baseForm->formType != kFormType_FURN)
		return nullptr;

	NiPointer<TESObjectREFR> refr;
	UInt32 refHandle;
	int numMarkers = *((unsigned int *)furnRef->baseForm + 102);  // TESFurniture::QNumMarkers(TESFurniture *this) { return *((unsigned int *)this + 102); }

	for (int i = 0; i < numMarkers; i++)
	{
		CALL_MEMBER_FN((TESObjectREFR_LL *)furnRef, GetMarkerUser)(&refHandle, i, false);
		if (refHandle)
		{
			LookupREFRByHandle(refHandle, refr);
			return refr;
		}
	}
	return nullptr;
}

// ---------------------------------------------------------------
// AAF specific functions
// ---------------------------------------------------------------

#define AGILITY			(0x2C7)
#define CHARISMA		(0x2C5)
#define ENDURANCE		(0x2C4)
#define INTELLIGENCE	(0x2C6)
#define LUCK			(0x2C8)
#define PERCEPTION		(0x2C3)
#define STRENGTH		(0x2C2)

#define AAF_ATTRACTION_AGILITY		(0x03007F)
#define AAF_ATTRACTION_CHARISMA		(0x030080)
#define AAF_ATTRACTION_EXPERIENCE	(0x03007E)
#define AAF_ATTRACTION_INTELLIGENCE	(0x030081)
#define AAF_ATTRACTION_LUCK			(0x030084)
#define AAF_ATTRACTION_PERCEPTION	(0x030082)
#define AAF_ATTRACTION_STRENGTH		(0x030083)
#define AAF_ATTRACTION_WEALTH		(0x03007D)
#define AAF_ISATTRACTIONGENERATED	(0x03081F)

#define GETAV(actor, av) actor->actorValueOwner.GetValue((ActorValueInfo *)LookupFormByID(av))
#define GETAAFAV(actor, av) actor->actorValueOwner.GetValue((ActorValueInfo *)GetFormFromPlugin(NULL, "AAF.esm", av))
#define MODAAFAV(actor, av, value) actor->actorValueOwner.Mod(0,(ActorValueInfo *)GetFormFromPlugin(NULL, "AAF.esm", av), value)

template <typename T>
void AddToVarArray(VMArray<VMVariable> *array, T val) 
{
	VMVariable tmpAV;
	tmpAV.Set(&val, false);
	array->Push(&tmpAV, false);
}

VMVariable VarArrayToVar(VMArray<VMVariable> vars)
{
	VMVariable result;
	result.Set<VMArray<VMVariable>>(&vars);
	return result;
}

float get_random()
{
	static std::default_random_engine e;
	static std::uniform_real_distribution<> dis(0, 1);
	return dis(e);
}

void generateAttractionValues(Actor *targetActor)
{
	MODAAFAV(targetActor, AAF_ATTRACTION_AGILITY, get_random() * 10.0);
	MODAAFAV(targetActor, AAF_ATTRACTION_CHARISMA, get_random() * 10.0);
	MODAAFAV(targetActor, AAF_ATTRACTION_EXPERIENCE, get_random() * 10.0);
	MODAAFAV(targetActor, AAF_ATTRACTION_INTELLIGENCE, get_random() * 10.0);
	MODAAFAV(targetActor, AAF_ATTRACTION_LUCK, get_random() * 10.0);
	MODAAFAV(targetActor, AAF_ATTRACTION_PERCEPTION, get_random() * 10.0);
	MODAAFAV(targetActor, AAF_ATTRACTION_STRENGTH, get_random() * 10.0);
	MODAAFAV(targetActor, AAF_ATTRACTION_WEALTH, get_random() * 10.0);
	MODAAFAV(targetActor, AAF_ISATTRACTIONGENERATED, 1.0);
}

VMArray<VMVariable>getAttractionProfile(Actor *targetActor)
{
	if (GETAAFAV(targetActor, AAF_ISATTRACTIONGENERATED) == 0.0)
		generateAttractionValues(targetActor);

	VMArray<VMVariable> attractionProfile;
	AddToVarArray<float>(&attractionProfile, GETAAFAV(targetActor, AAF_ATTRACTION_AGILITY));
	AddToVarArray<float>(&attractionProfile, GETAAFAV(targetActor, AAF_ATTRACTION_CHARISMA));
	AddToVarArray<float>(&attractionProfile, GETAAFAV(targetActor, AAF_ATTRACTION_EXPERIENCE));
	AddToVarArray<float>(&attractionProfile, GETAAFAV(targetActor, AAF_ATTRACTION_INTELLIGENCE));
	AddToVarArray<float>(&attractionProfile, GETAAFAV(targetActor, AAF_ATTRACTION_LUCK));
	AddToVarArray<float>(&attractionProfile, GETAAFAV(targetActor, AAF_ATTRACTION_PERCEPTION));
	AddToVarArray<float>(&attractionProfile, GETAAFAV(targetActor, AAF_ATTRACTION_STRENGTH));
	AddToVarArray<float>(&attractionProfile, GETAAFAV(targetActor, AAF_ATTRACTION_WEALTH));

	return attractionProfile;
}

VMArray<VMVariable> getAttractionQualities(Actor *targetActor)
{
	VMArray<VMVariable>  attractionQualities;
	AddToVarArray<float>(&attractionQualities, GETAV(targetActor, AGILITY));
	AddToVarArray<float>(&attractionQualities, GETAV(targetActor, CHARISMA));
	AddToVarArray<SInt32>(&attractionQualities, CALL_MEMBER_FN((Actor_LL *)targetActor, GetLevel)());
	AddToVarArray<float>(&attractionQualities, GETAV(targetActor, INTELLIGENCE));
	AddToVarArray<float>(&attractionQualities, GETAV(targetActor, LUCK));
	AddToVarArray<float>(&attractionQualities, GETAV(targetActor, PERCEPTION));
	AddToVarArray<float>(&attractionQualities, GETAV(targetActor, STRENGTH));
	AddToVarArray<SInt32>(&attractionQualities, CALL_MEMBER_FN((TESObjectREFR_LL *)targetActor, GetGoldAmount)());
	return attractionQualities;
}

BSFixedString GetName(TESForm* thisForm)
{
	if (!thisForm)
		return BSFixedString();

	TESFullName* pFullName = DYNAMIC_CAST(thisForm, TESForm, TESFullName);
	if (pFullName)
		return pFullName->name;

	return BSFixedString();
}

BSFixedString GetDisplayName(TESObjectREFR *ref)
{
	TESForm * baseForm = ref->baseForm;
	ExtraDataList * extraDataList = ref->extraDataList;

	if (baseForm)
	{
		if (extraDataList)
		{
			BSExtraData * extraData = extraDataList->GetByType(ExtraDataType::kExtraData_TextDisplayData);
			if (extraData)
			{
				ExtraTextDisplayData * displayText = DYNAMIC_CAST(extraData, BSExtraData, ExtraTextDisplayData);
				if (displayText)
				{
					return *CALL_MEMBER_FN(displayText, GetReferenceName)(baseForm);
				}
			}
		}

		TESFullName* pFullName = DYNAMIC_CAST(baseForm, TESForm, TESFullName);
		if (pFullName)
			return pFullName->name;
	}

	return BSFixedString();
}

VMArray<VMVariable> AAF_MakeActorData(StaticFunctionTag *base, Actor *targetActor, bool includeDistance, VMArray<BGSKeyword *>conditionKeywords)
{
	VMArray<VMVariable> resultData;
	SInt32 gender;
	TESNPC *targetBase;
	float distance = 0.0;
	VMArray<VMVariable> keywordResults;
	VMArray<VMVariable> attractionProfile;
	VMArray<VMVariable> attractionQualities;

	if (targetActor)
	{
		targetBase = (TESNPC *)Runtime_DynamicCast(targetActor->baseForm, RTTI_TESForm, RTTI_TESNPC);

		attractionProfile = getAttractionProfile(targetActor);
		attractionQualities = getAttractionQualities(targetActor);

		if (HasKeyword(targetActor, (BGSKeyword *)GetFormFromPlugin(NULL, "AAF.esm", 0x0121BB))) // AAF_GenderOverride_Male
			gender = 0;
		else
		{
			if (HasKeyword(targetActor, (BGSKeyword *)GetFormFromPlugin(NULL, "AAF.esm", 0x0121BC))) // AAF_GenderOverride_Female
				gender = 1;
			else
				gender = CALL_MEMBER_FN(targetBase, GetSex)();
		}

		if (includeDistance)
			distance = CALL_MEMBER_FN((TESObjectREFR_LL *)targetActor, GetDistanceFromReference)((TESObjectREFR *)LookupFormByID(0x14), false, false);

		for (int i = 0; i < conditionKeywords.Length(); i++)
		{
			BGSKeyword *kw;
			conditionKeywords.Get(&kw, i);
			if (HasKeyword(targetActor, kw))
				AddToVarArray<bool>(&keywordResults, true);
			else
				AddToVarArray<bool>(&keywordResults, false);
		}

		AddToVarArray<SInt32>(&resultData, targetActor->formID);
		AddToVarArray<BSFixedString>(&resultData, GetDisplayName(targetActor));
		AddToVarArray<float>(&resultData, distance);
		AddToVarArray<SInt32>(&resultData, gender);
		AddToVarArray<BSFixedString>(&resultData, GetName(targetActor->race));
		AddToVarArray<SInt32>(&resultData, targetActor->race->formID);
		AddToVarArray<BSFixedString>(&resultData, GetDisplayName(targetActor));
		AddToVarArray<bool>(&resultData, HasKeyword(targetActor, (BGSKeyword *)GetFormFromPlugin(NULL, "AAF.esm", 0x00915A))); // AAF_ActorBusy

		TESObjectREFR *furnRef = GetFurnitureReference(targetActor);
		if (furnRef)
			AddToVarArray<SInt32>(&resultData, furnRef->formID);
		else
			AddToVarArray<SInt32>(&resultData, 0);

		VMVariable tmp = VarArrayToVar(keywordResults);
		resultData.Push(&tmp, false);

		tmp = VarArrayToVar(attractionProfile);
		resultData.Push(&tmp, false);

		tmp = VarArrayToVar(attractionQualities);
		resultData.Push(&tmp, false);
	}

	return resultData;
}

bool AAF_IsValidLocation(StaticFunctionTag *base, TESObjectREFR *targetLocation, VMArray<VMVariable>actorGroup)
{
	TESForm *tF = (TESForm*)DYNAMIC_CAST(targetLocation, TESObjectREFR, TESForm);
	if (tF->flags & (TESForm::kFlag_IsDeleted | TESForm::kFlag_IsDisabled))
		return false;

	float xAngle = targetLocation->rot.x * 180.0 / M_PI;
	float yAngle = targetLocation->rot.y * 180.0 / M_PI;
	if (xAngle < -5.0 || xAngle > 5.0 || yAngle < -5.0 || yAngle > 5.0)
		return false;

	TESObjectREFR *pActor = GetActorInFurniture(targetLocation);
	if (pActor)
	{
		for (int i = 0; i < actorGroup.Length(); i++)
		{
			VMVariable tmpVar;
			SInt32 fID;
			actorGroup.Get(&tmpVar, i);
			tmpVar.Get(&fID);
			if (fID == pActor->formID)
			{
				return true;
			}
		}
		return false;
	}
	return true;
}

VMArray<VMVariable> AAF_MakeLocationData(StaticFunctionTag *base, TESObjectREFR *target, bool includeDistance)
{
	float distance = 0;

	if (includeDistance)
		distance = CALL_MEMBER_FN((TESObjectREFR_LL *)target, GetDistanceFromReference)((TESObjectREFR *)LookupFormByID(0x14), false, false);

	VMArray<VMVariable> result;
	AddToVarArray<SInt32>(&result, target->formID);
	AddToVarArray<BSFixedString>(&result, GetDisplayName(target));
	AddToVarArray<float>(&result, distance);
	AddToVarArray<SInt32>(&result, target->baseForm->formID);
	return result;
}

bool AAF_GetBlockedStatus(StaticFunctionTag *base, Actor *targetActor)
{
	if (targetActor)
	{
		TESForm* form = GetFormFromPlugin(NULL, "AAF.esm", 0x022BB1);	// AAF_BlockedKeywords
		BGSListForm *AAF_BlockedKeywords = DYNAMIC_CAST(form, TESForm, BGSListForm);
		if (AAF_BlockedKeywords)
		{
			for (UInt32 i = 0; i < AAF_BlockedKeywords->forms.count; i++)
			{
				AAF_BlockedKeywords->forms.GetNthItem(i, form);
				if (form)
				{
					BGSKeyword *pKW = DYNAMIC_CAST(form, TESForm, BGSKeyword);
					if (pKW)
					{
						TESForm *pF = DYNAMIC_CAST(targetActor, Actor, TESForm);
						if (HasKeyword(pF, pKW))
						{
							return true;
						}
					}
				}
			}
		}
	}
	return false;
}

bool AAF_IsValidActor(StaticFunctionTag *base, Actor *targetActor)
{
	if (targetActor)
	{
		TESForm *tF = DYNAMIC_CAST(targetActor, Actor, TESForm);
		return (((tF->flags & (TESForm::kFlag_IsDeleted | TESForm::kFlag_IsDisabled)) == 0) &&
			(CALL_MEMBER_FN((Actor_LL *)targetActor, IsDead)() == false) &&
			(AAF_GetBlockedStatus(base, targetActor) == false));
	}
	return false;
}

bool AAF_IsAvailableActor(StaticFunctionTag *base, Actor *targetActor)
{
	if (targetActor)
	{
		TESForm *pF = GetFormFromPlugin(base, "AAF.esm", 0x00915A);
		BGSKeyword *AAF_ActorBusy = DYNAMIC_CAST(pF, TESForm, BGSKeyword);

		if (AAF_ActorBusy)
			return (AAF_IsValidActor(base, targetActor) && (HasKeyword(targetActor, AAF_ActorBusy) == false));
	}
	return false;
}

void GetActorsInCell(TESObjectCELL* thisCell, VMArray<Actor*>*result, TESObjectREFR *targetRef, float radius)
{
	TESForm *tF = GetFormFromPlugin(NULL, "AAF.esm", 0x000F9B); // AAF_ActorTypeList
	BGSListForm *AAF_ActorTypeList = DYNAMIC_CAST(tF, TESForm, BGSListForm);
	TESObjectREFR *PlayerRef = (TESObjectREFR *)LookupFormByID(0x14);

	if (thisCell)
	{
		TESObjectREFR* pRef;
		for (UInt64 i = 0; i < thisCell->objectList.count; i++)
		{
			thisCell->objectList.GetNthItem(i, pRef);

			if (pRef && pRef->baseForm)
			{
				if (pRef->baseForm->formType == kFormType_NPC_)
				{
					tF = DYNAMIC_CAST(pRef, TESObjectREFR, TESForm);
					Actor *tActor = DYNAMIC_CAST(pRef, TESObjectREFR, Actor);
					float distance = CALL_MEMBER_FN((TESObjectREFR_LL *)tF, GetDistanceFromReference)(PlayerRef, false, false);

					if ((distance <= radius) && HasKeywordInFormList(tF, AAF_ActorTypeList) && AAF_IsAvailableActor(NULL, tActor))
					{
						result->Push(&tActor);
					}
				}
			}
		}
	}
}

TESObjectCELL* GetCellAtCoords(TESWorldSpace* world, SInt16 cellX, SInt16 cellY)
{
	UInt32 cellKey = (cellX << 16) | (cellY & 0x0000FFFF);
	auto it = world->CellMap.find(cellKey);
	if (it != world->CellMap.end())
		return it->second;
	else
		return NULL;
}

VMArray<Actor*> AAF_PerformActorScan(StaticFunctionTag* base, TESObjectREFR *ref, float radius)
{
	VMArray<Actor*> result;

	if (ref)
	{
		TESObjectCELL_LL* cell = TESObjectCELL_LL::FromCell(ref->parentCell);
		TESWorldSpace* world = CALL_MEMBER_FN(cell, GetWorldSpace)();

		if (world)
		{
			SInt16 curX = CALL_MEMBER_FN(cell, GetDataX)();
			SInt16 curY = CALL_MEMBER_FN(cell, GetDataY)();
			SInt16 minX = curX - 1;
			SInt16 maxX = curX + 1;
			SInt16 minY = curY - 1;
			SInt16 maxY = curY + 1;

			for (SInt16 tmpX = minX; tmpX <= maxX; tmpX++)
			{
				for (SInt16 tmpY = minY; tmpY <= maxY; tmpY++)
				{
					TESObjectCELL* tmpCell = GetCellAtCoords(world, tmpX, tmpY);
					if (tmpCell)
						GetActorsInCell(tmpCell, &result, ref, radius);
				}
			}
		}
		else
			GetActorsInCell(cell, &result, ref, radius);
	}
	return result;
}

bool RegisterFuncs(VirtualMachine* vm)
{
	_MESSAGE("RegisterFuncs");
	vm->RegisterFunction(
		new NativeFunction0<StaticFunctionTag, float>("GetLLFPPluginVersion", pluginName, GetLLFPPluginVersion, vm));
	vm->RegisterFunction(
		new NativeFunction1<StaticFunctionTag, TESRace*, Actor*>("GetActorPowerArmorRace", pluginName, GetActorPowerArmorRace, vm));
	vm->RegisterFunction(
		new NativeFunction1<StaticFunctionTag, BSFixedString, Actor*>("GetActorPowerArmorSkeleton", pluginName, GetActorPowerArmorSkeleton, vm));
	vm->RegisterFunction(
		new NativeFunction1<StaticFunctionTag, BSFixedString, Actor*>("GetActorSkeleton", pluginName, GetActorSkeleton, vm));
	//vm->RegisterFunction(
	//	new NativeFunction1<StaticFunctionTag, BSFixedString, Actor*>("GetActorActualSkeleton", pluginName, GetActorActualSkeleton, vm));
	vm->RegisterFunction(
		new NativeFunction1<StaticFunctionTag, bool, Actor*>("ResetActorEssential", pluginName, ResetActorEssential, vm));
	vm->RegisterFunction(
		new NativeFunction1<StaticFunctionTag, bool, bool>("SetFlyCam", pluginName, SetFlyCam, vm));
	//vm->RegisterFunction(
	//	new NativeFunction2<StaticFunctionTag, bool, bool, bool>("SetFlyCam2", pluginName, SetFlyCam2, vm));
	vm->RegisterFunction(
		new NativeFunction0<StaticFunctionTag, bool>("GetFlyCam", pluginName, GetFlyCam, vm));
	vm->RegisterFunction(
		new NativeFunction0<StaticFunctionTag, bool>("GetFXCam", pluginName, GetFXCam, vm));
	vm->RegisterFunction(
		new NativeFunction2<StaticFunctionTag, float, float, bool>("SetFlyCamSpeedMult", pluginName, SetFlyCamSpeedMult, vm));
	vm->RegisterFunction(
		new NativeFunction1<StaticFunctionTag, BSFixedString, BSFixedString>("GetCustomConfigPath", pluginName, GetCustomConfigPath_, vm));
	vm->RegisterFunction(
		new NativeFunction3<StaticFunctionTag, BSFixedString, BSFixedString, BSFixedString, BSFixedString>("GetCustomConfigOption", pluginName, GetCustomConfigOption_, vm));
	// vm->RegisterFunction(
		// new NativeFunction4<StaticFunctionTag, bool, BSFixedString, BSFixedString, VMArray<BSFixedString>, VMArray<BSFixedString>>("GetCustomConfigOptions", pluginName, GetCustomConfigOptions_, vm));
	vm->RegisterFunction(
		new NativeFunction2<StaticFunctionTag, VMArray<VMVariable>, BSFixedString, BSFixedString>("GetCustomConfigOptions", pluginName, GetCustomConfigOptions_, vm));
	vm->RegisterFunction(
		new NativeFunction1<StaticFunctionTag, VMArray<BSFixedString>, BSFixedString>("GetCustomConfigSections", pluginName, GetCustomConfigSections_, vm));
	vm->RegisterFunction(
		new NativeFunction3<StaticFunctionTag, UInt32, BSFixedString, BSFixedString, BSFixedString>("GetCustomConfigOption_UInt32", pluginName, GetCustomConfigOption_UInt32_, vm));
	vm->RegisterFunction(
		new NativeFunction4<StaticFunctionTag, bool, BSFixedString, BSFixedString, BSFixedString, BSFixedString>("SetCustomConfigOption", pluginName, SetCustomConfigOption_, vm));
	vm->RegisterFunction(
		new NativeFunction4<StaticFunctionTag, bool, BSFixedString, BSFixedString, VMArray<BSFixedString>, VMArray<BSFixedString>>("SetCustomConfigOptions", pluginName, SetCustomConfigOptions_, vm));
	vm->RegisterFunction(
		new NativeFunction4<StaticFunctionTag, bool, BSFixedString, BSFixedString, VMArray<BSFixedString>, VMArray<BSFixedString>>("ResetCustomConfigOptions", pluginName, ResetCustomConfigOptions_, vm));
	vm->RegisterFunction(
		new NativeFunction4<StaticFunctionTag, bool, BSFixedString, BSFixedString, BSFixedString, UInt32>("SetCustomConfigOption_UInt32", pluginName, SetCustomConfigOption_UInt32_, vm));
	vm->RegisterFunction(
		new NativeFunction3<StaticFunctionTag, SInt32, BSFixedString, BSFixedString, UInt32>("StringFind", pluginName, StringFind, vm));
	vm->RegisterFunction(
		new NativeFunction3<StaticFunctionTag, BSFixedString, BSFixedString, UInt32, UInt32>("StringSubstring", pluginName, StringSubstring, vm));
	vm->RegisterFunction(
		new NativeFunction2<StaticFunctionTag, VMArray<BSFixedString>, BSFixedString, BSFixedString>("StringSplit", pluginName, StringSplit, vm));
	vm->RegisterFunction(
		new NativeFunction2<StaticFunctionTag, BSFixedString, VMArray<BSFixedString>, BSFixedString>("StringJoin", pluginName, StringJoin, vm));
	vm->RegisterFunction(
		new NativeFunction1<StaticFunctionTag, BSFixedString, SInt32>("IntToHexString", pluginName, IntToHexString, vm));
	vm->RegisterFunction(
		new NativeFunction1<StaticFunctionTag, UInt32, BSFixedString>("HexStringToInt", pluginName, HexStringToInt, vm));
	vm->RegisterFunction(
		new NativeFunction1<StaticFunctionTag, BSFixedString, TESRace*>("GetRaceEditorID", pluginName, GetRaceEditorID, vm));
	// The next functions are for if you need to go past the 128 limit for forms, strings, ints, floats, and bools.
	//Right now, if we want to implement an alias system like SexLab did for its animations, we'd have to wait for the F4SE team to figure out aliases and make a typedef for them before ResizeArray can work with it.
	vm->RegisterFunction(
		new NativeFunction3<StaticFunctionTag, VMArray<TESForm*>, VMArray<TESForm*>, SInt32, TESForm*>("ResizeFormArray", pluginName, ResizeArray<TESForm*>, vm));
	vm->RegisterFunction(
		new NativeFunction3<StaticFunctionTag, VMArray<BSFixedString>, VMArray<BSFixedString>, SInt32, BSFixedString>("ResizeStringArray", pluginName, ResizeArray<BSFixedString>, vm));
	vm->RegisterFunction(
		new NativeFunction3<StaticFunctionTag, VMArray<SInt32>, VMArray<SInt32>, SInt32, SInt32>("ResizeIntArray", pluginName, ResizeArray<SInt32>, vm));
	vm->RegisterFunction(
		new NativeFunction3<StaticFunctionTag, VMArray<float>, VMArray<float>, SInt32, float>("ResizeFloatArray", pluginName, ResizeArray<float>, vm));
	vm->RegisterFunction(
		new NativeFunction3<StaticFunctionTag, VMArray<bool>, VMArray<bool>, SInt32, bool>("ResizeBoolArray", pluginName, ResizeArray<bool>, vm));
	vm->RegisterFunction(
		new NativeFunction3<StaticFunctionTag, VMArray<VMVariable>, VMArray<VMVariable>, SInt32, VMVariable>("ResizeVarArrayInternal", pluginName, ResizeVarArray, vm));
	vm->RegisterFunction(
		new NativeFunction2<StaticFunctionTag, VMArray<VMVariable>, VMArray<VMVariable>, SInt32>("ResizeVarArray2", pluginName, ResizeVarArray2, vm));
	vm->RegisterFunction(
		new NativeFunction1<StaticFunctionTag, bool, UInt32>("SetMinimalMaxArraySize", pluginName, SetMinimalMaxArraySize, vm));
	vm->RegisterFunction(
		new NativeFunction2<StaticFunctionTag, bool, TESLevItem*, TESForm*>("GetInLeveledItem", pluginName, GetInLeveledItem_, vm));
	vm->RegisterFunction(
		new NativeFunction1<StaticFunctionTag, BGSKeyword*, BSFixedString>("GetKeywordByName", pluginName, GetKeywordByName_, vm));
	vm->RegisterFunction(
		new NativeFunction0 <StaticFunctionTag, VMArray<BGSKeyword*>> ("GetAllKeywords", pluginName, GetAllKeywords_, vm));
	vm->RegisterFunction(
		new NativeFunction2 <StaticFunctionTag, bool, TESForm *, BGSKeyword *> ("AddKeywordToForm", pluginName, AddKeywordToForm_, vm));
	vm->RegisterFunction(
		new NativeFunction2 <StaticFunctionTag, bool, TESForm *, BGSKeyword *> ("DelKeywordFromForm", pluginName, DelKeywordFromForm_, vm));
	vm->RegisterFunction(
		new NativeFunction1 <StaticFunctionTag, VMArray<TESForm*>, BGSListForm*>("FormListToArray", pluginName, FormListToArray, vm));
	vm->RegisterFunction(
		new NativeFunction2<StaticFunctionTag, TESForm *, BSFixedString, UInt32>("GetFormFromPlugin", pluginName, GetFormFromPlugin, vm));
	vm->RegisterFunction(
		new NativeFunction2 <StaticFunctionTag, VMArray<TESForm *>, VMArray<BSFixedString>, VMArray<UInt32>>("GetFormArray", pluginName, GetFormArray, vm));
	vm->RegisterFunction(
		new NativeFunction1 <StaticFunctionTag, VMArray<TESForm *>, VMVariable>("GetFormArrayFromVar", pluginName, GetFormArrayFromVar, vm));
	vm->RegisterFunction(
		new NativeFunction3 <StaticFunctionTag, void, BGSListForm *, VMArray<BSFixedString>, VMArray<UInt32>>("PopulateFormlist", pluginName, PopulateFormlist, vm));
	vm->RegisterFunction(
		new NativeFunction1 <StaticFunctionTag, UInt32, TESForm *>("GetRecordFlags", pluginName, GetRecordFlags, vm));
	vm->RegisterFunction(
		new NativeFunction1 <StaticFunctionTag, UInt32, Actor *>("GetActorFlags", pluginName, GetActorFlags, vm));
	vm->RegisterFunction(
		new NativeFunction1 <StaticFunctionTag, UInt32, TESForm *>("GetFormType", pluginName, GetFormType, vm));
	vm->RegisterFunction(
		new NativeFunction1 <StaticFunctionTag, bool, TESForm *>("IsPersistent", pluginName, IsPersistent, vm));
	vm->RegisterFunction(
		new NativeFunction2 <StaticFunctionTag, bool, TESForm *, bool>("SetPersistent", pluginName, SetPersistent, vm));
	vm->RegisterFunction(
		new NativeFunction1 <StaticFunctionTag, float, TESObjectREFR*>("GetScale", pluginName, GetScale, vm));
	vm->RegisterFunction(
		new NativeFunction2 <StaticFunctionTag, void, TESObjectREFR *, float>("SetScale", pluginName, SetScale, vm));
	vm->RegisterFunction(
		new NativeFunction1 <StaticFunctionTag, BSFixedString, TESForm *> ("OriginalPluginName", pluginName, OriginalPluginName_, vm));
	vm->RegisterFunction(
		new NativeFunction1 <StaticFunctionTag, UInt32, TESForm *> ("OriginalPluginID", pluginName, OriginalPluginID_, vm));
	vm->RegisterFunction(
		new NativeFunction0 <StaticFunctionTag, TESObjectREFR*> ("LastCrossHairRef", pluginName, LastCrossHairRef, vm));
	vm->RegisterFunction(
		new NativeFunction0 <StaticFunctionTag, Actor*> ("LastCrossHairActor", pluginName, LastCrossHairActor, vm));
	//vm->RegisterFunction(
	//	new NativeFunction1<StaticFunctionTag, bool, TESObjectREFR*>("ObjectReferenceGetCollision", pluginName, ObjectReferenceGetCollision, vm));
	//vm->RegisterFunction(
	//	new NativeFunction2<StaticFunctionTag, bool, TESObjectREFR*, bool>("ObjectReferenceSetCollision", pluginName, ObjectReferenceSetCollision, vm));
	vm->RegisterFunction(
		new NativeFunction2<StaticFunctionTag, bool, TESObjectREFR*, BSFixedString>("ObjectReferenceSetSimpleDisplayName", pluginName, ObjectReferenceSetSimpleDisplayName, vm));
	vm->RegisterFunction(
		new NativeFunction4<StaticFunctionTag, void, TESObjectREFR*, float, float, float>("SetRefAngle", pluginName, SetRefAngle, vm));
	vm->RegisterFunction(
		new NativeFunction2<StaticFunctionTag, void, TESObjectREFR*, bool>("SetRefNoCollision", pluginName, SetRefNoCollision, vm));
	vm->RegisterFunction(
		new NativeFunction2<StaticFunctionTag, UInt32, TESObjectCELL*, UInt32>("GetNumRefsInCell", pluginName, GetNumRefsInCell, vm));
	vm->RegisterFunction(
		new NativeFunction3<StaticFunctionTag, TESObjectREFR *, TESObjectCELL*, UInt32, UInt32>("GetNthRefInCell", pluginName, GetNthRefInCell, vm));
	vm->RegisterFunction(
		new NativeFunction1<StaticFunctionTag, bool, Actor*>("ActorBaseIsClean", pluginName, ActorBaseIsClean, vm));
	vm->RegisterFunction(
		new NativeFunction1<StaticFunctionTag, TESForm*, Actor*>("GetActorBaseSkinForm", pluginName, GetActorBaseSkinForm, vm));
	vm->RegisterFunction(
		new NativeFunction2<StaticFunctionTag, bool, Actor*, Actor*>("CopyActorBaseskinForm", pluginName, CopyActorBaseskinForm, vm));
	vm->RegisterFunction(
		new NativeFunction3<StaticFunctionTag, bool, Actor*, SInt32, SInt32>("MfgMorph", pluginName, MfgMorph, vm));
	vm->RegisterFunction(
		new NativeFunction1<StaticFunctionTag, bool, Actor*>("MfgResetMorphs", pluginName, MfgResetMorphs, vm));
	vm->RegisterFunction(
		new NativeFunction1<StaticFunctionTag, VMArray<float>, Actor*>("MfgSaveMorphs", pluginName, MfgSaveMorphs, vm));
	vm->RegisterFunction(
		new NativeFunction2<StaticFunctionTag, bool, Actor*, VMArray<float>>("MfgRestoreMorphs", pluginName, MfgRestoreMorphs, vm));
	vm->RegisterFunction(
		new NativeFunction2<StaticFunctionTag, bool, Actor*, Actor*>("MfgCopyMorphs", pluginName, MfgCopyMorphs, vm));
	vm->RegisterFunction(
		new NativeFunction3<StaticFunctionTag, bool, Actor*, VMArray<SInt32>, VMArray<SInt32>>("MfgApplyMorphSet", pluginName, MfgApplyMorphSet, vm));
	vm->RegisterFunction(
		new NativeFunction1<StaticFunctionTag, bool, BSFixedString>("PrintConsole", pluginName, PrintConsole, vm));

	vm->RegisterFunction(
		new NativeFunction1 <StaticFunctionTag, void, BSFixedString>("PlaySoundWav", "SkyOut", PlaySoundWav, vm));
	vm->RegisterFunction(
		new NativeFunction2 <StaticFunctionTag, UInt32, UInt32, UInt32>("SetVolumeWav", pluginName, SetVolumeWav, vm));
	vm->RegisterFunction(
		new NativeFunction0 <StaticFunctionTag, UInt32>("GetVolumeWav", pluginName, GetVolumeWav, vm));
	vm->RegisterFunction(
		new NativeFunction2<StaticFunctionTag, BSFixedString, BSFixedString, BSFixedString>("VoiceMessage", pluginName, VoiceMessage, vm));

	vm->RegisterFunction(
		new NativeFunction1<StaticFunctionTag, float, BSFixedString>("GetINIFloat", pluginName, GetINIFloat, vm));
	vm->RegisterFunction(
		new NativeFunction1<StaticFunctionTag, UInt32, BSFixedString>("GetINIInt", pluginName, GetINIInt, vm));
	vm->RegisterFunction(
		new NativeFunction1<StaticFunctionTag, bool, BSFixedString>("GetINIBool", pluginName, GetINIBool, vm));
	vm->RegisterFunction(
		new NativeFunction1<StaticFunctionTag, BSFixedString, BSFixedString>("GetINIString", pluginName, GetINIString, vm));

	vm->RegisterFunction(
		new NativeFunction2<StaticFunctionTag, void, Actor*, TESForm*>("PlayIdle", pluginName, PlayIdle, vm));
	vm->RegisterFunction(
		new NativeFunction2<StaticFunctionTag, void, VMArray<Actor*>, VMArray<TESForm*>>("PlayMultipleIdles", pluginName, PlayMultipleIdles, vm));
	vm->RegisterFunction(
		new NativeFunction1 <StaticFunctionTag, void, Actor*>("StopCurrentIdle", pluginName, StopCurrentIdle, vm));

	vm->RegisterFunction(
		new NativeFunction4<StaticFunctionTag, void, Actor*, float, float, float>("SetActorPosition", pluginName, SetActorPosition, vm));
	vm->RegisterFunction(
		new NativeFunction1<StaticFunctionTag, void, Actor*>("EnableActorCollision", pluginName, EnableActorCollision, vm));
	vm->RegisterFunction(
		new NativeFunction1<StaticFunctionTag, void, Actor*>("DisableActorCollision", pluginName, DisableActorCollision, vm));
	vm->RegisterFunction(
		new NativeFunction1<StaticFunctionTag, void, Actor*>("ActorStopInteractingQuick", pluginName, ActorStopInteractingQuick, vm));
	vm->RegisterFunction(
		new NativeFunction1<StaticFunctionTag, void, Actor*>("ActorUpdateEquipment", pluginName, ActorUpdateEquipment, vm));
	vm->RegisterFunction(
		new NativeFunction2<StaticFunctionTag, void, Actor*, TESForm*>("ActorSetPackageOverride", pluginName, ActorSetPackageOverride, vm));
	vm->RegisterFunction(
		new NativeFunction1<StaticFunctionTag, void, Actor*>("ActorClearPackageOverride", pluginName, ActorClearPackageOverride, vm));
	vm->RegisterFunction(
		new NativeFunction0<StaticFunctionTag, void>("PlayerUpdateEquipment", pluginName, PlayerUpdateEquipment, vm));

	vm->RegisterFunction(
		new NativeFunction2<StaticFunctionTag, VMArray<Actor *>, TESObjectREFR *, float>("AAF_PerformActorScan", pluginName, AAF_PerformActorScan, vm));
	vm->RegisterFunction(
		new NativeFunction3<StaticFunctionTag, VMArray<VMVariable>, Actor*, bool, VMArray<BGSKeyword*> >("AAF_MakeActorData", pluginName, AAF_MakeActorData, vm));
	vm->RegisterFunction(
		new NativeFunction2<StaticFunctionTag, bool, TESObjectREFR*, VMArray<VMVariable>>("AAF_IsValidLocation", pluginName, AAF_IsValidLocation, vm));
	vm->RegisterFunction(
		new NativeFunction2<StaticFunctionTag, VMArray<VMVariable>, TESObjectREFR*, bool>("AAF_MakeLocationData", pluginName, AAF_MakeLocationData, vm));
	vm->RegisterFunction(
		new NativeFunction1<StaticFunctionTag, bool, Actor *>("AAF_IsValidActor", pluginName, AAF_IsValidActor, vm));
	vm->RegisterFunction(
		new NativeFunction1<StaticFunctionTag, bool, Actor *>("AAF_IsAvailableActor", pluginName, AAF_IsAvailableActor, vm));
	vm->RegisterFunction(
		new NativeFunction1<StaticFunctionTag, bool, Actor *>("AAF_GetBlockedStatus", pluginName, AAF_GetBlockedStatus, vm));

	return true;
}

class Scaleform_SetCustomConfigOption : public GFxFunctionHandler
{
public:
	virtual void Invoke(Args * args)
	{
		BSFixedString name(args->args[0].GetString());
		BSFixedString section(args->args[1].GetString());
		BSFixedString key(args->args[2].GetString());
		BSFixedString value(args->args[3].GetString());

		SetCustomConfigOption(name, section, key, value);
	}
};

class Scaleform_GetCustomConfigOption : public GFxFunctionHandler
{
public:
	virtual void Invoke(Args * args)
	{
		BSFixedString name(args->args[0].GetString());
		BSFixedString section(args->args[1].GetString());
		BSFixedString key(args->args[2].GetString());

		args->result->SetString(BSFixedString(GetCustomConfigOption(name, section, key).c_str()));
	}
};

class Scaleform_MfgMorph : public GFxFunctionHandler
{
public:
	virtual void Invoke(Args * args)
	{
		UInt32 FormID = args->args[0].GetInt();
		UInt32 MorphID = args->args[1].GetInt();
		UInt32 Intensity = args->args[2].GetInt();
		TESForm * form = LookupFormByID(FormID);
		if (form)
		{
			Actor* actor = (Actor*)DYNAMIC_CAST(form, TESForm, Actor);
			MfgMorph_internal(actor, MorphID, Intensity);
		}
	}
};

class Scaleform_HasKeyword : public GFxFunctionHandler
{
public:
	virtual void Invoke(Args * args)
	{
		bool result = false;
		UInt32 FormID = args->args[0].GetInt();
		BSFixedString KeywordName(args->args[1].GetString());
		TESForm * form = LookupFormByID(FormID);
		if (form)
		{
			BGSKeyword *kw = GetKeywordByName(KeywordName);
			if (kw)
				result = HasKeyword(form, kw);
		}
		args->result->SetBool(result);
	}
};

class Scaleform_WornHasKeyword : public GFxFunctionHandler
{
public:
	virtual void Invoke(Args * args)
	{
		bool result = false;
		UInt32 FormID = args->args[0].GetInt();
		BSFixedString KeywordName(args->args[1].GetString());
		TESForm * form = LookupFormByID(FormID);
		if (form)
		{
			TESObjectREFR *ref = DYNAMIC_CAST(form, TESForm, TESObjectREFR);
			if (ref)
			{
				BGSKeyword *kw = GetKeywordByName(KeywordName);
				if (kw)
					result = CALL_MEMBER_FN(TESObjectREFR_LL::FromRef(ref), WornHasKeyword)(kw);
			}
		}
		args->result->SetBool(result);
	}
};

class Scaleform_GetScale : public GFxFunctionHandler
{
public:
	virtual void Invoke(Args* args)
	{
		UInt32 FormID = args->args[0].GetInt();
		float result = 0.0;

		TESForm* form = LookupFormByID(FormID);
		if (form)
		{
			TESObjectREFR* ref = DYNAMIC_CAST(form, TESForm, TESObjectREFR);
			if (ref)
			{
				result = CALL_MEMBER_FN(TESObjectREFR_LL::FromRef(ref), GetScale)();
			}
		}
		args->result->SetNumber(result);
	}
};


class Scaleform_SetScale : public GFxFunctionHandler
{
public:
	virtual void Invoke(Args * args)
	{
		UInt32 FormID = args->args[0].GetInt();
		float Scale = args->args[1].GetNumber();

		TESForm * form = LookupFormByID(FormID);
		if (form)
		{
			TESObjectREFR *ref = DYNAMIC_CAST(form, TESForm, TESObjectREFR);
			if (ref)
			{
				CALL_MEMBER_FN(TESObjectREFR_LL::FromRef(ref), SetScale)(Scale);
			}
		}
	}
};


bool RegisterScaleformFuncs(GFxValue * llfp, GFxMovieRoot *movieRoot)
{
	RegisterFunction<Scaleform_GetCustomConfigOption>(llfp, movieRoot, "GetCustomConfigOption");
	RegisterFunction<Scaleform_SetCustomConfigOption>(llfp, movieRoot, "SetCustomConfigOption");
	RegisterFunction<Scaleform_MfgMorph>(llfp, movieRoot, "MfgMorph");
	RegisterFunction<Scaleform_HasKeyword>(llfp, movieRoot, "HasKeyword");
	RegisterFunction<Scaleform_WornHasKeyword>(llfp, movieRoot, "WornHasKeyword");
	RegisterFunction<Scaleform_SetScale>(llfp, movieRoot, "GetScale");
	RegisterFunction<Scaleform_SetScale>(llfp, movieRoot, "SetScale");

	return true;
}

bool RegisterScaleform(GFxMovieView * view, GFxValue * plugin)
{
	GFxMovieRoot	* movieRoot = view->movieRoot;

	GFxValue currentSWFPath;
	const char* currentSWFPathString = nullptr;

	if (movieRoot->GetVariable(&currentSWFPath, "root.loaderInfo.url")) {
		currentSWFPathString = currentSWFPath.GetString();
	}
	else {
		_MESSAGE("WARNING: Scaleform registration failed.");
	}

	// Look for the menu that we want to inject into.
	if (strcmp(currentSWFPathString, "Interface/HUDMenu.swf") == 0) {
		GFxValue root; movieRoot->GetVariable(&root, "root");

		// Register native code object
		GFxValue obj; movieRoot->CreateObject(&obj);
		root.SetMember(PLUGIN_MENU, &obj);
		RegisterScaleformFuncs(&obj, movieRoot);
		
		_MESSAGE("Injected %s into Interface/HUDMenu.swf", PLUGIN_MENU);
	}
	return true;
}

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
	{ REQUIRED_RUNTIME, 0 },	// compatible with 1.10.980

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

	// get the scaleform interface and query its version
	g_scaleform = (F4SEScaleformInterface *)f4se->QueryInterface(kInterface_Scaleform);
	if (!g_scaleform)
	{
		_MESSAGE("couldn't get scaleform interface");

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

	// register papyrus functions
	g_papyrus->Register(RegisterFuncs);

	// register plugin with scaleform
	g_scaleform->Register(PLUGIN_MENU, RegisterScaleform);

	if (g_messaging != nullptr)
		g_messaging->RegisterListener(g_pluginHandle, "F4SE", MessageCallback);

	_MESSAGE("%s load successful.", pluginName);
	return true;
}

};
