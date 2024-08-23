#include "MiscUtil.h"

//#include "f4se/PluginAPI.h"
#include "f4se/GameAPI.h"
#include "f4se/GameData.h"
#include "f4se/GameTypes.h"
#include "f4se/GameForms.h"
#include "f4se/GameRTTI.h"
#include "f4se/GameCamera.h"
#include "f4se/GameReferences.h"
#include "f4se/GameObjects.h"
#include "f4se/ObScript.h"

#include "f4se_common/SafeWrite.h"

#include "f4se/NiNodes.h"
//#include "f4se/NiGeometry.h"

#include "f4se/PapyrusNativeFunctions.h"

#include "SafeRead.h"

#include <fstream>
#include <ctime>

#pragma warning(disable: 4996)
#pragma warning(disable: 4229)

namespace MiscUtil {

	// Status of FXCam, if active TFC won't work
	#define k_FXCamIsActive					0x059DA100	// 5A13180

	#define k_saveCurrentPlayerCameraState	0x036F1283	// 3729283
	#define k_CheckIfCameraStateIsSelected	0x0124E6D0	// 124E6D0
	#define k_ToggleFlyCam					0x0124AD40	// 124AD40

	#define k_ControlPlayerCamera			0x01B28480	// 1B28480
	#define k_ControlPlayer					0x01B28570	// 1B28570

	#define k_fFreeCameraTranslationSpeed_Camera	0x038048E8	// 383C8E8
	#define k_fFreeCameraRotationSpeed_Camera		0x03804900	// 383C900

	RelocPtr <byte> g_FXCamIsActive(k_FXCamIsActive);
	RelocPtr <byte> g_saveCurrentPlayerCameraState(k_saveCurrentPlayerCameraState);

	class TESCameraState_LL
	{
	public:
		virtual ~TESCameraState_LL();

		virtual void OnStateStart();	// pure
		virtual void OnStateEnd();	// pure
		virtual void OnUpdate(void * unk1);
		virtual void Unk_04();
		virtual void Unk_05();
		virtual void Unk_06();	// pure
		virtual void Unk_07();	// pure
		virtual void Unk_08();	// pure

		BSIntrusiveRefCounted	refCount;			// 08
		BSInputEventUser		inputEventUser;		// 10 - 
		TESCamera				* camera;			// 18 - verified
		UInt32					stateId;			// 20
		UInt32					pad0024;			// 24
	};	// 28

	class PlayerCamera_LL : public PlayerCamera
	{
	public:
		MEMBER_FN_PREFIX(PlayerCamera_LL);
		DEFINE_MEMBER_FN(CheckIfCameraStateIsSelected, bool, k_CheckIfCameraStateIsSelected, UInt32 cmCommand);
		DEFINE_MEMBER_FN(ToggleFlyCam, void, k_ToggleFlyCam, bool pause);
	};
	RelocPtr <PlayerCamera_LL *> g_playerCamera_LL(0x058CEB28);

	class InputManager_LL : public InputManager
	{
	public:
		MEMBER_FN_PREFIX(InputManager_LL);
		DEFINE_MEMBER_FN(ControlPlayerCamera, void, k_ControlPlayerCamera);
		DEFINE_MEMBER_FN(ControlPlayer, void, k_ControlPlayer);
	};
	RelocPtr <InputManager_LL *> g_inputMgr_LL(0x059DA1E0);


	class Main_LL
	{
	public:
		UInt8	unk00[0x029];	// 00
		UInt8	pause;			// 2A
		UInt8	unk2B[6];		// 2B
	};	// 268 allocated
	RelocPtr <Main_LL *> g_Main_LL(0x05ADD2C8);

	// This should be a template: SettingT<INISettingCollection>
	class SettingT_INISettingCollection_LL
	{
	public:

		virtual void Unk_00();

		float	value;	// 08
		char	*name;	// 0C

		MEMBER_FN_PREFIX(SettingT_INISettingCollection_LL);
		DEFINE_MEMBER_FN(Changed, void, 0x01C91CF0);
	};	// 10

	RelocPtr <SettingT_INISettingCollection_LL> g_fFreeCameraTranslationSpeed_Camera(k_fFreeCameraTranslationSpeed_Camera) ;
	RelocPtr <SettingT_INISettingCollection_LL> g_fFreeCameraRotationSpeed_Camera(k_fFreeCameraRotationSpeed_Camera) ;

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

	void ToggleFreeCamera(StaticFunctionTag* base, bool arg1) {
		PlayerCamera_LL* pc = (*g_playerCamera_LL);
		InputManager_LL* im = (*g_inputMgr_LL);

		bool current = CALL_MEMBER_FN(pc, CheckIfCameraStateIsSelected)(PlayerCamera::kCameraState_Free);
		if (!GetFXCam(base))
		{
			if (!current)
				(*g_saveCurrentPlayerCameraState) = pc->unk30;
			CALL_MEMBER_FN(pc, ToggleFlyCam)(arg1);
			current = CALL_MEMBER_FN(pc, CheckIfCameraStateIsSelected)(PlayerCamera::kCameraState_Free);
			pc->Unk_02(current  || (*g_saveCurrentPlayerCameraState));
			if (current)
				CALL_MEMBER_FN(im, ControlPlayerCamera)();
			else
				CALL_MEMBER_FN(im, ControlPlayer)();
		}
	}

	void SetFreeCameraSpeed(StaticFunctionTag* base, float speed) {
		SetFlyCamSpeedMult(base, speed);
	}

	void SetFreeCameraState(StaticFunctionTag* base, bool enable, float speed) {
		PlayerCamera_LL* pc = *g_playerCamera_LL;
		if (pc)
		{
			TESCameraState_LL* cs = (TESCameraState_LL*)(pc->cameraState);
			if (cs)
			{
				bool InFreeCamera = cs->stateId == pc->kCameraState_Free;
				// Leave free camera
				if (InFreeCamera && !enable){
					ToggleFreeCamera(base, false);
				}
				// Enter free camera
				else if (!InFreeCamera && enable){
					SetFlyCamSpeedMult(base, speed);
					ToggleFreeCamera(base, false);
					CALL_MEMBER_FN(pc, SetCameraState)(pc->cameraStates[pc->kCameraState_Free]);
				}
			}
		}
	}


	float GetNodeRotation(StaticFunctionTag* base, TESObjectREFR* obj, BSFixedString nodeName, bool firstPerson, UInt32 nth) {
		if (nth < 0 || nth > 8 || !obj)
			return -1.0f;

		NiAVObject* skeleton = obj->GetObjectRootNode();
		PlayerCharacter* player = DYNAMIC_CAST(obj, TESObjectREFR, PlayerCharacter);
		if (player && player->unkF0)
			skeleton = firstPerson ? player->firstPersonSkeleton : player->unkF0->rootNode;
		if (!skeleton)
			return -1.0f;

		NiAVObject* node;
		if (skeleton && nodeName.c_str()[0])
			node = skeleton->GetObjectByName(&nodeName);
		if (!node)
			return -1.0f;

		return *node->m_worldTransform.rot.data[nth];
	}

	void PrintConsole(StaticFunctionTag* base, BSFixedString text) {
		if (!text.c_str())
			return;
		else if (strlen(text.c_str()) < 1000)
			Console_Print(text.c_str());
		else { // Large strings printed to console crash the game - truncate it
			std::string msg = text.c_str();
			msg.resize(997);
			msg.append("...");
			Console_Print(msg.c_str());
		}
	}

	#define k_SetMenusPtr 0x058D0898	// 5909918	// Cmd_ToggleMenus_Execute first offset (g_ui :) )
	RelocPtr<UInt8*> g_SetMenusPtr(k_SetMenusPtr);

	void SetMenus(StaticFunctionTag* base, bool enabled){
		if (g_SetMenusPtr != 0) {
			uintptr_t offset = g_SetMenusPtr.GetUIntPtr() + 0x248; 
			if (enabled)
				SafeWrite8(offset, 1);
			else
				SafeWrite8(offset, 0);
		}
	}

	uintptr_t batAddr = 0;
	char textBat[256];
	typedef char(*__cdecl batCommand)(int, char*, int, int, int, int*, int, int);
	typedef bool (* ObScript_Execute_LL)(void * paramInfo, void * scriptData, TESObjectREFR * thisObj, void * containingObj, void * scriptObj, void * locals, double * result, void * opcodeOffsetPtr, ...);

	void ExecuteBat(StaticFunctionTag* base, BSFixedString fileName) {
		memset(textBat, 0, sizeof(char)* 256);
		memcpy(textBat, fileName.c_str(), strlen(fileName.c_str()));

		batAddr = (uintptr_t)textBat;

		ObScript_Execute_LL func = (ObScript_Execute_LL)0x00526C50;	// Cmd_RunConsoleBatch_Execute
		func(0, 0, 0, 0, 0, 0, 0, 0, batAddr);
	}

	BSFixedString ReadFromFile(StaticFunctionTag* base, BSFixedString fileName) {
		std::ifstream doc;
		try {
			doc.open(fileName.c_str(), std::ifstream::in | std::ifstream::binary);
		}
		catch (std::exception*){
			return "";
		}
		if (doc.fail())
			return "";

		std::string contents;
		doc.seekg(0, std::ios::end);
		contents.resize(doc.tellg());
		doc.seekg(0, std::ios::beg);
		doc.read(&contents[0], contents.size());
		doc.close();
		return BSFixedString(contents.c_str());
	}

	bool WriteToFile(StaticFunctionTag* base, BSFixedString fileName, BSFixedString text, bool append, bool timestamp) {
		std::ofstream doc;
		try {
			doc.open(fileName.c_str(), append ? (std::ofstream::out | std::ofstream::app) : std::ofstream::out);
		}
		catch (std::exception*) {
			return false;
		}
		if (doc.fail())
			return false;

		if (timestamp) {
			time_t curTime;
			time(&curTime);
			struct tm * timeinfo = localtime(&curTime);
			doc << "[" << asctime(timeinfo) << "] ";
		}

		doc << text.c_str();
		doc.close();
		return true;
	}

	BSFixedString GetRaceEditorID(StaticFunctionTag* base, TESRace* raceForm) {
		if (!raceForm)
			return "";
		return BSFixedString(raceForm->editorId.c_str());
	}

	BSFixedString GetActorRaceEditorID(StaticFunctionTag* base, Actor* actorRef) {
		if (!actorRef)
			return "";
		return BSFixedString(actorRef->race->editorId.c_str());
	}

	void RegisterFuncs(VirtualMachine* vm) {
		vm->RegisterFunction(new NativeFunction1<StaticFunctionTag, void, bool>("ToggleFreeCamera", "MiscUtil", ToggleFreeCamera, vm));
		vm->SetFunctionFlags("MiscUtil", "ToggleFreeCamera", IFunction::kFunctionFlag_NoWait);

		vm->RegisterFunction(new NativeFunction1<StaticFunctionTag, void, float>("SetFreeCameraSpeed", "MiscUtil", SetFreeCameraSpeed, vm));
		vm->SetFunctionFlags("MiscUtil", "SetFreeCameraSpeed", IFunction::kFunctionFlag_NoWait);

		vm->RegisterFunction(new NativeFunction2<StaticFunctionTag, void, bool, float>("SetFreeCameraState", "MiscUtil", SetFreeCameraState, vm));
		vm->SetFunctionFlags("MiscUtil", "SetFreeCameraState", IFunction::kFunctionFlag_NoWait);

		vm->RegisterFunction(new NativeFunction1<StaticFunctionTag, void, BSFixedString>("PrintConsole", "MiscUtil", PrintConsole, vm));
		vm->SetFunctionFlags("MiscUtil", "PrintConsole", IFunction::kFunctionFlag_NoWait);

		vm->RegisterFunction(new NativeFunction1<StaticFunctionTag, void, bool>("SetMenus", "MiscUtil", SetMenus, vm));
		vm->SetFunctionFlags("MiscUtil", "SetMenus", IFunction::kFunctionFlag_NoWait);

		vm->RegisterFunction(new NativeFunction1<StaticFunctionTag, void, BSFixedString>("ExecuteBat", "MiscUtil", ExecuteBat, vm));
		vm->SetFunctionFlags("MiscUtil", "ExecuteBat", IFunction::kFunctionFlag_NoWait);

		vm->RegisterFunction(new NativeFunction1<StaticFunctionTag, BSFixedString, BSFixedString>("ReadFromFile", "MiscUtil", ReadFromFile, vm));
		vm->SetFunctionFlags("MiscUtil", "ReadFromFile", IFunction::kFunctionFlag_NoWait);

		vm->RegisterFunction(new NativeFunction4<StaticFunctionTag, bool, BSFixedString, BSFixedString, bool, bool>("WriteToFile", "MiscUtil", WriteToFile, vm));
		vm->SetFunctionFlags("MiscUtil", "WriteToFile", IFunction::kFunctionFlag_NoWait);

		vm->RegisterFunction(new NativeFunction1<StaticFunctionTag, BSFixedString, TESRace*>("GetRaceEditorID", "MiscUtil", GetRaceEditorID, vm));
		vm->SetFunctionFlags("MiscUtil", "GetRaceEditorID", IFunction::kFunctionFlag_NoWait);

		vm->RegisterFunction(new NativeFunction1<StaticFunctionTag, BSFixedString, Actor*>("GetActorRaceEditorID", "MiscUtil", GetActorRaceEditorID, vm));
		vm->SetFunctionFlags("MiscUtil", "GetActorRaceEditorID", IFunction::kFunctionFlag_NoWait);
	}

	void InitPlugin(){
	}
}