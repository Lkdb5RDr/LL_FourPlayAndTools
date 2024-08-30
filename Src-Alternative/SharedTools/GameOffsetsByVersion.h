#pragma once

#include "f4se_common/f4se_version.h"

#ifndef RUNTIME_VERSION_1_10_980
#define RUNTIME_VERSION_1_10_980	MAKE_EXE_VERSION(1, 10, 980)
#endif

// defines game engine offsets as relative hexadecimal integer values.
// Note: same offsets for 98 106, 111 and 114 and 120 but no longer 130. 138 is same as 130 and different from 162 which is the same as 163.

#if   (CURRENT_RELEASE_RUNTIME <  RUNTIME_VERSION_1_10_162) 
	
#error This version is no longer usable at this time (I would need to compile here the old offsets)

#elif (CURRENT_RELEASE_RUNTIME == RUNTIME_VERSION_1_10_163 || CURRENT_RELEASE_RUNTIME == RUNTIME_VERSION_1_10_162)	// last was v46

#define k_hackArrayAdd							0x02775A34	// 2775914	// 279A9E4
#define k_hackArrayInsert						0x02775FD9	// 2775EB9	// 279AF89

#define k_FXCamIsActive							0x059DA130	// 59DA100	// 5A13180

#define k_saveCurrentPlayerCameraState			0x036F1283	// 3729283
#define k_CheckIfCameraStateIsSelected			0x0124E7F0	// 124E6D0	// 124E6D0
#define k_ToggleFlyCam							0x0124AE60	// 124AD40	// 124AD40

#define k_ControlPlayerCamera					0x01B285A0	// 1B28480	// 1B28480
#define k_ControlPlayer							0x01B28690	// 1B28570	// 1B28570

#define k_fFreeCameraTranslationSpeed_Camera	0x038048E8	// 383C8E8
#define k_fFreeCameraRotationSpeed_Camera		0x03804900	// 383C900

#define k_playerCamera_ST						0x058CEB28 
#define k_inputMgr_ST							0x059DA210 
#define k_Main_ST								0x05ADD2C8 

#define k_AddScriptAddedForm					0x004B0AC0 

// k_EventSource :
//	BSTGlobalEvent::EventSource_ViewCasterUpdateEvent_::Func0000+22
//	g_BSTGlobalEvent::EventSource_ViewCasterUpdateEvent_ or __ICI__BSTValueEventSource_ViewCasterUpdateEvent___Func0000_to0053_to0051_to0160+24
#define	k_EventSource							0x05A5F758	// 5A5F728	// 5A987A8	

#define k_collisionState						0x058D08B0	// 5909930	// Cmd_ToggleCollision_Execute + 0x055

// ??_7ExtraTextDisplayData@@6B@
#define k_ExtraTextDisplayDataVtbl				0x02C52788	// 2C52778	// 2C7C7C8 

#define k_uFGBlinkLeftEyeMorphIndex				0x037333F8
#define k_uFGBlinkRightEyeMorphIndex			0x03733410
#define k_fFGBlinkClosedValue					0x037333E0

// if I ever get back to this :)
#define k_CallFunction							0x00000000	//			//			// 0533C00

// And those are class member functions offsets for SharedTools.h

#define k_TESObjectREFR_SetAngleOnReference			0x0040BD70
#define k_TESObjectREFR_SetNoCollision				0x003EDEE0
#define k_TESObjectREFR_GetRefPersists				0x003F4DA0
#define k_TESObjectREFR_SetRefPersists				0x003F4E00
#define k_TESObjectREFR_GetDistanceFromPoint		0x0040F560
#define k_TESObjectREFR_GetDistanceFromReference	0x0040F620
#define k_TESObjectREFR_GetGoldAmount				0x003FA8D0
#define k_TESObjectREFR_GetMarkerUser				0x0047BD90
#define k_TESObjectREFR_WornHasKeyword				0x00400050
#define k_TESObjectREFR_GetScale					0x003F8540
#define k_TESObjectREFR_SetScale					0x003F85B0

#define k_Actor_HasPerkInRank						0x00DA6600	// 0DA64E0	// 0DA64E0
#define k_Actor_SetPosition							0x00D77690							// Actor::Func00CA
#define k_Actor_PutCreatedPackage					0x00D96C70							// Actor::Func00FA
#define k_Actor_DisableCollision					0x00D73AB0
#define k_Actor_EnableCollision						0x00D73A20
#define k_Actor_StopInteractingQuick				0x00E0EAA0							// It is probably a TESObjectREFR member, rather than an Actor member.
#define k_Actor_GetSex								0x00D74610
#define k_Actor_GetLevel							0x00D79E90
#define k_Actor_IsDead								0x00D89D40
#define k_Actor_EvaluatePackage						0x00D7F980

#define k_PROCESSTYPE_UpdateEquipment				0x00E60860 
	// action should be 0x35 (DefaultObject kActionIdle), checkConditions should be false, targetObj should be null
#define k_PROCESSTYPE_PlayIdle						0x00E35510
#define k_PROCESSTYPE_StopCurrentIdle				0x00E358C0
#define k_PROCESSTYPE_ClearCurrentDataforProcess	0x00E77520
#define k_PROCESSTYPE_ClearCurrentPackage			0x00E2A670 
#define k_PROCESSTYPE_GetFadeAlpha					0x00E450F0 
#define k_PROCESSTYPE_GetFadeState					0x00E44AD0 
#define k_PROCESSTYPE_ForceFadeToNormal				0x00E45170		// works on player and NPCs , actor cannot be NULL
#define k_PROCESSTYPE_SetIgnoringCombat				0x00E29F80
#define k_PROCESSTYPE_GetIgnoringCombat				0x00E29F70

#define k_TESObjectCELL_ST_GetDataXandY				0x003B1080
#define k_TESObjectCELL_ST_GetDataX					0x003B1040 
#define k_TESObjectCELL_ST_GetDataY					0x003B1060 
#define k_TESObjectCELL_ST_GetWorldSpace			0x00008880

#define k_PlayerCharacter_ST_UpdatePlayer3d			0x00EA0630

#define k_TESActorBaseData_ST_ModifyFlag			0x002B5EE0 

#define k_SettingT_INISettingCollection_ST_Changed	0x01C91E10	// 1C91CF0	// 1C91CF0 

#define k_Cmd_Default_Parse							0x004E7E00
#define k_ExtractArgs								0x004E37D0

#elif (CURRENT_RELEASE_RUNTIME == RUNTIME_VERSION_1_10_980)	// last was v48

#error This version is no longer supported

#define k_hackArrayAdd							0x01FEB41D	// 2775A34	// 2775914	// 279A9E4
#define k_hackArrayInsert						0x01FEBA85	// 2775FD9	// 2775EB9	// 279AF89

#define k_FXCamIsActive							0x02F6D578	// 59DA130	// 59DA100	// 5A13180

#define k_saveCurrentPlayerCameraState			0x02C763E0	// 36F1283	// 3729283
#define k_CheckIfCameraStateIsSelected			0x00FA52B0	// 124E7F0	// 124E6D0	// 124E6D0
#define k_ToggleFlyCam							0x00FA1610	// 124AE60	// 124AD40	// 124AD40

#define k_ControlPlayerCamera					0x01554DF0	// 1B285A0	// 1B28480	// 1B28480
#define k_ControlPlayer							0x01554EC0	// 1B28690	// 1B28570	// 1B28570

#define k_fFreeCameraTranslationSpeed_Camera	0x02CC6918	// 38048E8	// 383C8E8
#define k_fFreeCameraRotationSpeed_Camera		0x02CC6930	// 3804900	// 383C900

#define k_playerCamera_ST						0x02E639D8	// 58CEB28 
#define k_inputMgr_ST							0x02F6D628	// 59DA210 
#define k_Main_ST								0x03039BA8	// 5ADD2C8 

#define k_AddScriptAddedForm					0x00535200	// 04B0AC0 

// k_EventSource :
//	BSTGlobalEvent::EventSource_ViewCasterUpdateEvent_::Func0000+22
//	g_BSTGlobalEvent::EventSource_ViewCasterUpdateEvent_ or __ICI__BSTValueEventSource_ViewCasterUpdateEvent___Func0000_to0053_to0051_to0160+24
#define	k_EventSource							0x02FF02F0	// 5A5F758	// 5A5F728	// 5A987A8	

#define k_collisionState						0x02E65414	// 58D08B0	// 5909930	// Cmd_ToggleCollision_Execute + 0x055

// ??_7ExtraTextDisplayData@@6B@
#define k_ExtraTextDisplayDataVtbl				0x0227CD70	// 2C52788	// 2C52778	// 2C7C7C8 

#define k_uFGBlinkLeftEyeMorphIndex				0x00000000	// 037333F8
#define k_uFGBlinkRightEyeMorphIndex			0x00000000	// 03733410
#define k_fFGBlinkClosedValue					0x00000000	// 037333E0

// if I ever get back to this :)
#define k_CallFunction							0x00000000	//			//			//			// 0533C00

// And those are class member functions offsets for SharedTools.h

#define k_TESObjectREFR_SetAngleOnReference			0x00000000	// 040BD70
#define k_TESObjectREFR_SetNoCollision				0x00000000	// 03EDEE0
#define k_TESObjectREFR_GetRefPersists				0x00000000	// 03F4DA0
#define k_TESObjectREFR_SetRefPersists				0x00000000	// 03F4E00
#define k_TESObjectREFR_GetDistanceFromPoint		0x00000000	// 040F560
#define k_TESObjectREFR_GetDistanceFromReference	0x00000000	// 040F620
#define k_TESObjectREFR_GetGoldAmount				0x00000000	// 03FA8D0
#define k_TESObjectREFR_GetMarkerUser				0x00000000	// 047BD90
#define k_TESObjectREFR_WornHasKeyword				0x00000000	// 0400050
#define k_TESObjectREFR_GetScale					0x00000000	// 03F8540
#define k_TESObjectREFR_SetScale					0x00000000	// 03F85B0

#define k_Actor_HasPerkInRank						0x00C0C060	// 0DA6600	// 0DA64E0	// 0DA64E0
#define k_Actor_SetPosition							0x00000000	// 0D77690							// Actor::Func00CA
#define k_Actor_PutCreatedPackage					0x00000000	// 0D96C70							// Actor::Func00FA
#define k_Actor_DisableCollision					0x00000000	// 0D73AB0
#define k_Actor_EnableCollision						0x00000000	// 0D73A20
#define k_Actor_StopInteractingQuick				0x00000000	// 0E0EAA0							// It is probably a TESObjectREFR member, rather than an Actor member.
#define k_Actor_GetSex								0x00000000	// 0D74610
#define k_Actor_GetLevel							0x00000000	// 0D79E90
#define k_Actor_IsDead								0x00000000	// 0D89D40
#define k_Actor_EvaluatePackage						0x00000000	// 0D7F980

#define k_PROCESSTYPE_UpdateEquipment				0x00CA0F30	// 0E60860 
	// action should be 0x35 (DefaultObject kActionIdle), checkConditions should be false, targetObj should be null
#define k_PROCESSTYPE_PlayIdle						0x00C74D80	// 0E35510
#define k_PROCESSTYPE_StopCurrentIdle				0x00000000	// 0E358C0
#define k_PROCESSTYPE_ClearCurrentDataforProcess	0x00000000	// 0E77520
#define k_PROCESSTYPE_ClearCurrentPackage			0x00000000	// 0E2A670 
#define k_PROCESSTYPE_GetFadeAlpha					0x00000000	// 0E450F0 
#define k_PROCESSTYPE_GetFadeState					0x00000000	// 0E44AD0 
#define k_PROCESSTYPE_ForceFadeToNormal				0x00000000	// 0E45170		// works on player and NPCs , actor cannot be NULL
#define k_PROCESSTYPE_SetIgnoringCombat				0x00000000	// 0E29F80
#define k_PROCESSTYPE_GetIgnoringCombat				0x00000000	// 0E29F70

#define k_TESObjectCELL_ST_GetDataXandY				0x00000000	// 03B1080
#define k_TESObjectCELL_ST_GetDataX					0x00000000	// 03B1040 
#define k_TESObjectCELL_ST_GetDataY					0x00000000	// 03B1060 
#define k_TESObjectCELL_ST_GetWorldSpace			0x00000000	// 0008880

#define k_PlayerCharacter_ST_UpdatePlayer3d			0x00000000	// 0EA0630

#define k_TESActorBaseData_ST_ModifyFlag			0x002B5EE0	// 02B5EE0 

#define k_SettingT_INISettingCollection_ST_Changed	0x016849E0	// 1C91E10	// 1C91CF0	// 1C91CF0 

#define k_Cmd_Default_Parse							0x005621A0
#define k_ExtractArgs								0x00545200

#elif (CURRENT_RELEASE_RUNTIME == RUNTIME_VERSION_1_10_984)	// currently v53

#define k_hackArrayAdd 							0x01FEBB3D	// 1FEB41D	// 2775A34	// 2775914	// 279A9E4
#define k_hackArrayInsert						0x01FEC1A5	// 1FEBA85	// 2775FD9	// 2775EB9	// 279AF89

#define k_FXCamIsActive							0x02F6E578	//	2F6D578	// 59DA130	// 59DA100	// 5A13180

#define k_saveCurrentPlayerCameraState			0x02C773E0	// 2C763E0	// 36F1283	// 3729283
#define k_CheckIfCameraStateIsSelected			0x00FA5640	// 0FA52B0	// 124E7F0	// 124E6D0	// 124E6D0
#define k_ToggleFlyCam							0x00FA19A0	// 0FA1610	// 124AE60	// 124AD40	// 124AD40

#define k_ControlPlayerCamera					0x015551C0	// 1554DF0	// 1B285A0	// 1B28480	// 1B28480
#define k_ControlPlayer							0x01555290	// 1554EC0	// 1B28690	// 1B28570	// 1B28570

#define k_fFreeCameraTranslationSpeed_Camera	0x02CC7940	// 2CC6918	// 38048E8	// 383C8E8
#define k_fFreeCameraRotationSpeed_Camera		0x02CC7958	// 2CC6930	// 3804900	// 383C900

#define k_playerCamera_ST						0x02E649D8	// 2E639D8	// 58CEB28 
#define k_inputMgr_ST							0x02F6E628	// 2F6D628	// 59DA210 
#define k_Main_ST								0x0303ABA8	// 3039BA8	// 5ADD2C8 

#define k_AddScriptAddedForm					0x00535230	// 0535200	// 04B0AC0 

// k_EventSource :
//	BSTGlobalEvent::EventSource_ViewCasterUpdateEvent_::Func0000+22
//	g_BSTGlobalEvent::EventSource_ViewCasterUpdateEvent_ or __ICI__BSTValueEventSource_ViewCasterUpdateEvent___Func0000_to0053_to0051_to0160+24
#define	k_EventSource							0x02FF12F0	// 2FF02F0	// 5A5F758	// 5A5F728	// 5A987A8	

#define k_collisionState						0x02E66414	// 2E65414	// 58D08B0	// 5909930	// Cmd_ToggleCollision_Execute + 0x055

// ??_7ExtraTextDisplayData@@6B@
#define k_ExtraTextDisplayDataVtbl				0x0227CD70	// 227CD70	// 2C52788	// 2C52778	// 2C7C7C8 

#define k_uFGBlinkLeftEyeMorphIndex				0x02CB4B00	//			// 037333F8
#define k_uFGBlinkRightEyeMorphIndex			0x02CB4B18	//			// 03733410
#define k_fFGBlinkClosedValue					0x02CB4AE8	//			// 037333E0

// if I ever get back to this :)
#define k_CallFunction							0x00000000	//			//			//			//			// 0533C00

// And those are class member functions offsets for SharedTools.h

#define k_TESObjectREFR_SetAngleOnReference			0x004BF810	//			// 040BD70
#define k_TESObjectREFR_SetNoCollision				0x004A00A0	//			// 03EDEE0
#define k_TESObjectREFR_GetRefPersists				0x004A7990	//			// 03F4DA0
#define k_TESObjectREFR_SetRefPersists				0x004A79F0	//			// 03F4E00
#define k_TESObjectREFR_GetDistanceFromPoint		0x004C32F0	//			// 040F560
#define k_TESObjectREFR_GetDistanceFromReference	0x004C33B0	//			// 040F620
#define k_TESObjectREFR_GetGoldAmount				0x004ADAF0	//			// 03FA8D0
#define k_TESObjectREFR_GetMarkerUser				0x0050A790	//			// 047BD90
#define k_TESObjectREFR_WornHasKeyword				0x004B36F0	//			// 0400050
#define k_TESObjectREFR_GetScale					0x004AB5F0	//			// 03F8540
#define k_TESObjectREFR_SetScale					0x004AB660	//			// 03F85B0

#define k_Actor_HasPerkInRank						0x00C0C3F0	// 0C0C060	// 0DA6600	// 0DA64E0	// 0DA64E0
#define k_Actor_SetPosition							0x00BDAB80	//			// 0D77690							// Actor::Func00CA
#define k_Actor_PutCreatedPackage					0x00BFBC30	//			// 0D96C70							// Actor::Func00FA
#define k_Actor_DisableCollision					0x00BD6E30	//			// 0D73AB0
#define k_Actor_EnableCollision						0x00BD6DA0	//			// 0D73A20
#define k_Actor_StopInteractingQuick				0x00C55FD0	//			// 0E0EAA0							// It is probably a TESObjectREFR member, rather than an Actor member.
#define k_Actor_GetSex								0x00BD7960	//			// 0D74610
#define k_Actor_GetLevel							0x00BDD41B	//			// 0D79E90
#define k_Actor_IsDead								0x00BEDE20	//			// 0D89D40
#define k_Actor_EvaluatePackage						0x00BE3600	//			// 0D7F980

#define k_PROCESSTYPE_UpdateEquipment				0x00CA12C0	// 0CA0F30	// 0E60860 
	// action should be 0x35 (DefaultObject kActionIdle), checkConditions should be false, targetObj should be null
#define k_PROCESSTYPE_PlayIdle						0x00C75110	// 0C74D80	// 0E35510
#define k_PROCESSTYPE_StopCurrentIdle				0x00C754B0	//			// 0E358C0
#define k_PROCESSTYPE_ClearCurrentDataforProcess	0x00CB38C0	//			// 0E77520
#define k_PROCESSTYPE_ClearCurrentPackage			0x00C6B710	//			// 0E2A670 
#define k_PROCESSTYPE_GetFadeAlpha					0x00C853D0	//			// 0E450F0 
#define k_PROCESSTYPE_GetFadeState					0x00C84DC0	//			// 0E44AD0 
#define k_PROCESSTYPE_ForceFadeToNormal				0x00C85450	//			// 0E45170		// works on player and NPCs , actor cannot be NULL
#define k_PROCESSTYPE_SetIgnoringCombat				0x00C6AEE0	//			// 0E29F80
#define k_PROCESSTYPE_GetIgnoringCombat				0x00C6AED0	//			// 0E29F70

#define k_TESObjectCELL_ST_GetDataXandY				0x00472C70	//			// 03B1080
#define k_TESObjectCELL_ST_GetDataX					0x00472C30	//			// 03B1040 
#define k_TESObjectCELL_ST_GetDataY					0x00472C50	//			// 03B1060 
#define k_TESObjectCELL_ST_GetWorldSpace			0x00443DC0	//			// 0008880

#define k_PlayerCharacter_ST_UpdatePlayer3d			0x00CD68B0	//			// 0EA0630

#define k_TESActorBaseData_ST_ModifyFlag			0x002B5F10	//			// 02B5EE0 

#define k_SettingT_INISettingCollection_ST_Changed	0x01684DB0	// 16849E0	// 1C91E10	// 1C91CF0	// 1C91CF0 

#define k_Cmd_Default_Parse							0x005621D0
#define k_ExtractArgs								0x0055F350

#else
#error This version is not yet supported
#endif

// MiddleProcess was renamed to AIProcess in RUNTIME_VERSION_1_10_980

#if (CURRENT_RELEASE_RUNTIME < RUNTIME_VERSION_1_10_980)
#define PROCESSTYPE		MiddleProcess
#define PROCESSTYPEST	MiddleProcess_ST
#else
#define PROCESSTYPE		AIProcess
#define PROCESSTYPE_ST	AIProcess_ST
#endif

