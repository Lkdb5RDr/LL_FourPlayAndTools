#pragma once

#include "..\SharedTools\GameOffsetsByVersion.h"

// Updated class definitions from the current F4SE with new members and functions.

class TESForm_ST : public BaseFormComponent
{
public:
	struct Mods
	{
		// Array of mods that contain this form (wether it is an original, a copy of an overwrite).
		// The builtin forms (formID < 0x0800) not overwritten in the game esm have a null mods pointer.
		// Those arrays are shared, as there is only one per combination of overwriting mods.
		// It is the same in Skyrim and Skyrim SE
		ModInfo** entries;		// 00
		UInt32	size;			// 08
		UInt32	pad0C;			// 0C
	};

	Mods* mods;		// 08
	UInt32	flags;		// 10	CK also
	UInt32	formID;		// 14	CK also
};

class ExtraPowerArmor_ST : public BSExtraData
{
public:

	UInt32		unk0018;	// 0018
	UInt32		unk001C;	// 001C
	TESRace* race;		// 0020
	UInt16		unk0028;	// 0028
};

class ExtraLeveledCreature_ST : public BSExtraData
{
public:
	TESForm* baseForm;		//	018
	UInt8	unk0020[104];	//	020
};

class TESObjectREFR_ST : public TESObjectREFR
{
public:
	static TESObjectREFR_ST* FromRef(TESObjectREFR* ref) { return reinterpret_cast<TESObjectREFR_ST*>(ref); };

	MEMBER_FN_PREFIX(TESObjectREFR_ST);
	DEFINE_MEMBER_FN(SetAngleOnReference, void, k_TESObjectREFR_SetAngleOnReference	/* 040BD70 */, NiPoint3);
	DEFINE_MEMBER_FN(SetNoCollision, void, k_TESObjectREFR_SetNoCollision, bool);
	DEFINE_MEMBER_FN(GetRefPersists, bool, k_TESObjectREFR_GetRefPersists);
	DEFINE_MEMBER_FN(SetRefPersists, void, k_TESObjectREFR_SetRefPersists, bool);
	DEFINE_MEMBER_FN(GetDistanceFromPoint, float, k_TESObjectREFR_GetDistanceFromPoint)
	DEFINE_MEMBER_FN(GetDistanceFromReference, float, k_TESObjectREFR_GetDistanceFromReference, TESObjectREFR const*, bool, bool)
	DEFINE_MEMBER_FN(GetGoldAmount, UInt32, k_TESObjectREFR_GetGoldAmount);
	DEFINE_MEMBER_FN(GetMarkerUser, UInt32*, k_TESObjectREFR_GetMarkerUser, UInt32* refHandlePtr, UInt32 MarkerNum, bool abIgnoreReserved); //returns refHandlePtr
	DEFINE_MEMBER_FN(WornHasKeyword, bool, k_TESObjectREFR_WornHasKeyword, BGSKeyword* kw);
	DEFINE_MEMBER_FN(GetScale, float, k_TESObjectREFR_GetScale);
	DEFINE_MEMBER_FN(SetScale, void, k_TESObjectREFR_SetScale, float scale);
};

class Actor_ST : public Actor
{
public:
	static Actor_ST* FromActor(Actor* actor) { return reinterpret_cast<Actor_ST*>(actor); };

	MEMBER_FN_PREFIX(Actor_ST);
	DEFINE_MEMBER_FN(HasPerkInRank, byte, k_Actor_HasPerkInRank);
	DEFINE_MEMBER_FN(SetPosition, void, k_Actor_SetPosition, const NiPoint3& a_pos, bool a_updateCharController);				// Actor::Func00CA
	DEFINE_MEMBER_FN(PutCreatedPackage, void, k_Actor_PutCreatedPackage, TESForm* a_pack, bool a_tempPack, bool a_isACreatedPackage, bool a_allowFromFurniture);		// Actor::Func00FA
	DEFINE_MEMBER_FN(DisableCollision, void, k_Actor_DisableCollision, void);
	DEFINE_MEMBER_FN(EnableCollision, void, k_Actor_EnableCollision, void);
	DEFINE_MEMBER_FN(StopInteractingQuick, void, k_Actor_StopInteractingQuick, bool flag1, bool flag2, bool a_initializeNodes);	// It is probably a TESObjectREFR member, rather than an Actor member.
	DEFINE_MEMBER_FN(GetSex, unsigned char, k_Actor_GetSex, void);
	DEFINE_MEMBER_FN(GetLevel, UInt32, k_Actor_GetLevel, void);																	// Actor::Func00CA
	DEFINE_MEMBER_FN(IsDead, bool, k_Actor_IsDead, void);																		// Actor::Func00C0
	DEFINE_MEMBER_FN(EvaluatePackage, void, k_Actor_EvaluatePackage, bool a_immediate, bool a_resetAI);

	struct PROCESSTYPE_ST : PROCESSTYPE
	{
		static PROCESSTYPE_ST* FromPROCESSTYPE(PROCESSTYPE* mp) { return reinterpret_cast<PROCESSTYPE_ST*>(mp); };

		void* unk10;	// 10 - struct with at least fadeAlpha information as a float at index 0x0224. If unk10 is null, fadeAlpha defaut value (1.0) is found at offset 0x0142273D00. 0x0220 if Sate (if not null, which implies fadeState is false/0)
		UInt64 skip18[(0x0E1 - 18) / 0x08];
		UInt8	unkE1;	// ignore combat

		MEMBER_FN_PREFIX(PROCESSTYPE);
		DEFINE_MEMBER_FN(UpdateEquipment, void, k_PROCESSTYPE_UpdateEquipment, Actor* actor, UInt32 flags);	// Called DoUpdate3dModel in F4SE 0.7 and newer
		// action should be 0x35 (DefaultObject kActionIdle), checkConditions should be false, targetObj should be null
		DEFINE_MEMBER_FN(PlayIdle, void, k_PROCESSTYPE_PlayIdle, Actor* actor, int action, TESForm* idle, bool checkConditions, TESObjectREFR* targetObj);
		DEFINE_MEMBER_FN(StopCurrentIdle, void, k_PROCESSTYPE_StopCurrentIdle, Actor* actor, bool a1, bool a2);
		DEFINE_MEMBER_FN(ClearCurrentDataforProcess, void, k_PROCESSTYPE_ClearCurrentDataforProcess, Actor* actor);
		DEFINE_MEMBER_FN(ClearCurrentPackage, void, k_PROCESSTYPE_ClearCurrentPackage, Actor* actor);
		DEFINE_MEMBER_FN(GetFadeAlpha, float, k_PROCESSTYPE_GetFadeAlpha, void);
		DEFINE_MEMBER_FN(GetFadeState, float, k_PROCESSTYPE_GetFadeState, void);
		DEFINE_MEMBER_FN(ForceFadeToNormal, void, k_PROCESSTYPE_ForceFadeToNormal, Actor* actor);  // works on player and NPCs , actor cannot be NULL
		DEFINE_MEMBER_FN(SetIgnoringCombat, void, k_PROCESSTYPE_SetIgnoringCombat, bool);
		DEFINE_MEMBER_FN(GetIgnoringCombat, bool, k_PROCESSTYPE_GetIgnoringCombat, void);
	};
};

// from CommonLibF4
class TESWorldSpace_ST : public TESForm
{
public:
	enum { kTypeID = kFormType_WRLD };
	TESFullName			fullName;
	TESTexture			texture;
	BSTHashMap<int32_t, TESObjectCELL*>	CellMap;
};
STATIC_ASSERT(offsetof(TESWorldSpace_ST, CellMap) == 0x40);

class TESObjectCELL_ST : public TESObjectCELL
{
public:
	static TESObjectCELL_ST* FromCell(TESObjectCELL* cell) { return reinterpret_cast<TESObjectCELL_ST*>(cell); };

	MEMBER_FN_PREFIX(TESObjectCELL_ST);
	DEFINE_MEMBER_FN(GetDataXandY, void, k_TESObjectCELL_ST_GetDataXandY, int& iX, int& iY);
	DEFINE_MEMBER_FN(GetDataX, int, k_TESObjectCELL_ST_GetDataX, void);
	DEFINE_MEMBER_FN(GetDataY, int, k_TESObjectCELL_ST_GetDataY, void);
	DEFINE_MEMBER_FN(GetWorldSpace, TESWorldSpace_ST *, k_TESObjectCELL_ST_GetWorldSpace, void);
};

class PlayerCharacter_ST : public PlayerCharacter
{
public:
	static PlayerCharacter_ST* FromPlayerCharacter(PlayerCharacter* plchr) { return reinterpret_cast<PlayerCharacter_ST*>(plchr); };

	MEMBER_FN_PREFIX(PlayerCharacter_ST);
	DEFINE_MEMBER_FN(UpdatePlayer3d, void, k_PlayerCharacter_ST_UpdatePlayer3d, void);
};

class TESActorBaseData_ST : public BaseFormComponent
{
public:
	virtual ~TESActorBaseData_ST();

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
	typedef TESForm* LPTESForm;
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
	BGSVoiceType* voiceType;		// 28 - 90
	UInt64			unk30;				// 30 - 98
	TemplateArray* templates;		// 38 - A0
	UInt64			unk40;				// 40 - A8
	UInt64			unk48;				// 48 - B0
	UInt64			unk50;				// 50 - B8
	UInt64			unk58;				// 58 - C0
	UInt64			unk60;				// 60 - C8

	MEMBER_FN_PREFIX(TESActorBaseData_ST);
	DEFINE_MEMBER_FN(ModifyFlag, void, k_TESActorBaseData_ST_ModifyFlag, UInt32 flag, bool doSet, bool unk1); // unk1 = 1 (appears to select which actor flags to use)
};
STATIC_ASSERT(sizeof(TESActorBaseData_ST) == 0x68);

// Status of FXCam, if active TFC won't work

class PlayerCamera_ST : public PlayerCamera
{
public:
	MEMBER_FN_PREFIX(PlayerCamera_ST);
	DEFINE_MEMBER_FN(CheckIfCameraStateIsSelected, bool, k_CheckIfCameraStateIsSelected, UInt32 cmCommand);
	DEFINE_MEMBER_FN(ToggleFlyCam, void, k_ToggleFlyCam, bool pause);
};

class InputManager_ST : public InputManager
{
public:
	MEMBER_FN_PREFIX(InputManager_ST);
	DEFINE_MEMBER_FN(ControlPlayerCamera, void, k_ControlPlayerCamera);
	DEFINE_MEMBER_FN(ControlPlayer, void, k_ControlPlayer);
};

class Main_ST
{
public:
	UInt8	unk00[0x029];	// 00
	UInt8	pause;			// 2A
	UInt8	unk2B[6];		// 2B
};	// 268 allocated

// This should be a template: SettingT<INISettingCollection>
class SettingT_INISettingCollection_ST
{
public:

	virtual void Unk_00();

	float	value;	// 08
	char* name;	// 0C

	MEMBER_FN_PREFIX(SettingT_INISettingCollection_ST);
	DEFINE_MEMBER_FN(Changed, void, k_SettingT_INISettingCollection_ST_Changed);
};	// 10

// code originally from HUD++ plugin by kassent (on GitHub)

struct ViewCasterUpdateEvent_ST
{
	struct ViewCasterValueEvent_ST
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
	ViewCasterValueEvent_ST value;			// 00
	bool					unk28;			// 28
	UInt32					unk2C;			// 2C
	bool					isNPC;			// 30 ?
	float					unk34;			// 34
	bool					canInteractive;	// 38
};
STATIC_ASSERT(sizeof(ViewCasterUpdateEvent_ST) == 0x40);

template <typename EventT>
class BSTEventDispatcher_ST	// BSTGlobalEvent::EventSource<typename EventT> : public struct BSTSingletonSDM<EventT,BSTSingletonSDMOpStaticBuffer>, BSTEventSink<BSTGlobalEvent::KillSDMEvent>, BSTEventSource<ViewCasterUpdateEvent>
{
public:
	typedef BSTEventSink<EventT> SinkT;

	bool AddEventSink(SinkT* sink)
	{
		SimpleLocker locker(&lock);

		// Check for duplicate first
		for (int i = 0; i < eventSinks.count; i++)
		{
			if (eventSinks[i] == sink)
				return false;
		}

		if (!eventSinks.Insert(0, sink))
			eventSinks.Push(sink);
		return true;
	}

	void RemoveEventSink(SinkT* sink)
	{
		SimpleLocker locker(&lock);

		for (int i = 0; i < eventSinks.count; i++)
		{
			if (eventSinks[i] == sink) {
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

class CrosshairChangeHandler_ST : public BSTEventSink<ViewCasterUpdateEvent_ST>
{
public:
	virtual ~CrosshairChangeHandler_ST() { };
	virtual	EventResult	ReceiveEvent(ViewCasterUpdateEvent_ST* evn, void* dispatcher) override;
	static void Register();
};

// From earlier Script Extenders

struct ObScriptParam_ST
{
	const char* typeStr;	// 00
	UInt32		typeID;		// 04
	UInt32		isOptional;	// 08
};

struct ScriptData_ST
{
	// members
	UInt16	opcode;		// 00
	UInt16	chunkSize;	// 02
	UInt16	numParams;	// 04
};

class Script_ST;
class ScriptLocals_ST;
class ScriptLineBuffer_ST;
class ScriptBuffer_ST;

struct	FirstParamOf_Eval {
	TESObjectREFR* thisObj;
	UInt64			unk010;
	UInt64			unk018;
	UInt64			unk020;
	UInt64			unk028;
	UInt64			unk030;
	UInt64			unk038;
	UInt64			unk040;
	UInt64			unk048;
	UInt64			unk050;
	UInt64			unk058;
	UInt32			unk060;
	UInt8			unk064;
};

#define COMMAND_ARGS_ST				ObScriptParam_ST * paramInfo, ScriptData_ST * scriptData, TESObjectREFR * thisObj, TESObjectREFR * containingObj, Script_ST * scriptObj, ScriptLocals_ST * locals, double& result, UInt32& opcodeOffsetPtr
#define COMMAND_ARGS_EVAL_ST		FirstParamOf_Eval& fpoe, void * arg1, void * arg2, double& result, void * arg3
#define COMMAND_ARGS_PARSE_ST		UInt32 numParams, const ObScriptParam_ST* paramInfo, ScriptLineBuffer_ST* lineBuf, ScriptBuffer_ST* scriptBuf
#define EXTRACT_ARGS_ARGS_ST		const ObScriptParam_ST* paramInfo, ScriptData_ST* scriptData, UInt32& opcodeOffsetPtr, TESObjectREFR* thisObj, TESObjectREFR* containingObj, Script_ST* scriptObj, ScriptLocals_ST* locals, void* args1, void* args2
#define PASS_COMMAND_ARGS_ST		paramInfo, scriptData, thisObj, containingObj, scriptObj, locals, result, opcodeOffsetPtr
#define PASS_COMMAND_EVAL_ST		fpoe, arg1, arg2, result, arg3
#define PASS_COMMAND_PARSE_ST		numParams, paramInfo, lineBuf, scriptBuf
#define PASS_EXTRACT_ARGS_ARGS_ST	paramInfo, scriptData, opcodeOffsetPtr, thisObj, containingObj, scriptObj, locals, args1, args2

typedef bool (*ObScript_Eval_ST)(COMMAND_ARGS_EVAL_ST);
typedef bool (*ObScript_Execute_ST)(COMMAND_ARGS_ST);
typedef bool (*ObScript_Parse_ST)();
RelocAddr <ObScript_Parse_ST> Cmd_Default_Parse(k_Cmd_Default_Parse);

typedef bool (*_ExtractArgs_ST)(EXTRACT_ARGS_ARGS_ST);
RelocAddr <_ExtractArgs_ST> ExtractArgs_ST(k_ExtractArgs);


// 50
struct ObScriptCommand_ST
{
	const char*			longName;		// 00
	const char*			shortName;		// 08
	UInt32				opcode;			// 10
	UInt32				pad14;			// 14
	const char*			helpText;		// 18
	UInt8				needsParent;	// 20
	UInt8				pad21;			// 21
	UInt16				numParams;		// 22
	UInt32				pad24;			// 24
	ObScriptParam_ST*	params;			// 28

	// handlers
	ObScript_Execute_ST	execute;		// 30
	ObScript_Parse_ST	parse;			// 38
	ObScript_Eval_ST	eval;			// 40

	UInt32				flags;			// 48
	UInt32				pad4C;			// 4C

	static ObScriptCommand_ST* FromObScriptCommand(ObScriptCommand* command) { return reinterpret_cast<ObScriptCommand_ST*>(command); };
};

