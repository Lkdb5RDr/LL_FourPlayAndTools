#pragma once

#include "f4se_common/f4se_version.h"

#ifndef RUNTIME_VERSION_1_10_980
#define RUNTIME_VERSION_1_10_980	MAKE_EXE_VERSION(1, 10, 980)
#endif
#ifndef RUNTIME_VERSION_1_11_159
#define RUNTIME_VERSION_1_11_159	MAKE_EXE_VERSION(1, 11, 159)
#endif
#ifndef RUNTIME_VERSION_1_11_169
#define RUNTIME_VERSION_1_11_169	MAKE_EXE_VERSION(1, 11, 169)
#endif

#ifndef RUNTIME_VERSION_1_11_191
#define RUNTIME_VERSION_1_11_191	MAKE_EXE_VERSION(1, 11, 191)
#endif

#ifndef RUNTIME_VERSION_1_11_221
#define RUNTIME_VERSION_1_11_221	MAKE_EXE_VERSION(1, 11, 221)
#endif

#ifndef RUNTIME_VERSION_1_11_240
#define RUNTIME_VERSION_1_11_240	MAKE_EXE_VERSION(1, 11, 240)
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

#define k_TESActorBaseData_ST_ModifyFlag			0x00149720	// bug found in v55, wrong offset (0x002B5EE0)

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

#elif (CURRENT_RELEASE_RUNTIME == RUNTIME_VERSION_1_10_984)	// currently v55

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

#elif (CURRENT_RELEASE_RUNTIME == RUNTIME_VERSION_1_11_137)	// currently v56

#define k_hackArrayAdd 							0x0213B3CD	// 1FEBB3D	// 1FEB41D	// 2775A34	// 2775914	// 279A9E4
#define k_hackArrayInsert						0x0213BA35	// 1FEC1A5	// 1FEBA85	// 2775FD9	// 2775EB9	// 279AF89

#define k_FXCamIsActive							0x031D4758	// 2F6E578	//	2F6D578	// 59DA130	// 59DA100	// 5A13180

#define k_saveCurrentPlayerCameraState			0x02ECC6C0	// 2C773E0	// 2C763E0	// 36F1283	// 3729283
#define k_CheckIfCameraStateIsSelected			0x01026090	// 0FA52B0	// 0FA5640	// 0FA52B0	// 124E7F0	// 124E6D0	// 124E6D0
#define k_ToggleFlyCam							0x010223F0	// 0FA19A0	// 0FA1610	// 124AE60	// 124AD40	// 124AD40

#define k_ControlPlayerCamera					0x01669DC0	// 15551C0	// 1554DF0	// 1B285A0	// 1B28480	// 1B28480
#define k_ControlPlayer							0x01669E90	// 1555290	// 1554EC0	// 1B28690	// 1B28570	// 1B28570

#define k_fFreeCameraTranslationSpeed_Camera	0x02F1CEB8	// 2CC7940	// 2CC6918	// 38048E8	// 383C8E8
#define k_fFreeCameraRotationSpeed_Camera		0x02F1CED0	// 2CC7958	// 2CC6930	// 3804900	// 383C900

#define k_playerCamera_ST						0x030CAAD8	// 2E649D8	// 2E639D8	// 58CEB28 
#define k_inputMgr_ST							0x031D4808	// 2F6E628	// 2F6D628	// 59DA210 
#define k_Main_ST								0x032BC010	// 303ABA8	// 3039BA8	// 5ADD2C8 

#define k_AddScriptAddedForm					0x00586250	// 0535230	// 0535200	// 04B0AC0 

// k_EventSource :
//	BSTGlobalEvent::EventSource_ViewCasterUpdateEvent_::Func0000+22
//	g_BSTGlobalEvent::EventSource_ViewCasterUpdateEvent_ or __ICI__BSTValueEventSource_ViewCasterUpdateEvent___Func0000_to0053_to0051_to0160+24
#define	k_EventSource							0x032574D8	// 2FF12F0	// 2FF02F0	// 5A5F758	// 5A5F728	// 5A987A8	

#define k_collisionState						0x030CC5C4	// 2E66414	// 2E65414	// 58D08B0	// 5909930	// Cmd_ToggleCollision_Execute + 0x055

// ??_7ExtraTextDisplayData@@6B@
#define k_ExtraTextDisplayDataVtbl				0x024606F0	// 227CD70	// 227CD70	// 2C52788	// 2C52778	// 2C7C7C8 

#define k_uFGBlinkLeftEyeMorphIndex				0x02F09D40	// 2CB4B00	//			// 037333F8
#define k_uFGBlinkRightEyeMorphIndex			0x02F09D58	// 2CB4B18	//			// 03733410
#define k_fFGBlinkClosedValue					0x02F09D28	// 2CB4AE8	//			// 037333E0

// if I ever get back to this :)
#define k_CallFunction							0x00000000	//			//			//			//			// 0533C00

// And those are class member functions offsets for SharedTools.h

#define k_TESObjectREFR_SetAngleOnReference			0x00510830	// 04BF810	//			// 040BD70	//	TESObjectREFR::SetAngleOnReference
#define k_TESObjectREFR_SetNoCollision				0x004F1070	// 04A00A0	//			// 03EDEE0	//	TESObjectREFR::SetNoCollision
#define k_TESObjectREFR_GetRefPersists				0x004F8960	// 04A7990	//			// 03F4DA0	//	TESObjectREFR::GetRefPersists
#define k_TESObjectREFR_SetRefPersists				0x004F89C0	// 04A79F0	//			// 03F4E00	//	TESObjectREFR::SetRefPersists
#define k_TESObjectREFR_GetDistanceFromPoint		0x00514310	// 04C32F0	//			// 040F560	//	TESObjectREFR::GetDistanceFromPoint
#define k_TESObjectREFR_GetDistanceFromReference	0x005143D0	// 04C33B0	//			// 040F620	//	TESObjectREFR::GetDistanceFromReference
#define k_TESObjectREFR_GetGoldAmount				0x004FEAB0	// 04ADAF0	//			// 03FA8D0	//	TESObjectREFR::GetGoldAmount
#define k_TESObjectREFR_GetMarkerUser				0x0055B7B0	// 050A790	//			// 047BD90	//	TESObjectREFR::GetMarkerUser
#define k_TESObjectREFR_WornHasKeyword				0x005046B0	// 04B36F0	//			// 0400050	//	TESObjectREFR::WornHasKeyword
#define k_TESObjectREFR_GetScale					0x004FC5C0	// 04AB5F0	//			// 03F8540	//	TESObjectREFR::GetScale
#define k_TESObjectREFR_SetScale					0x004FC630	// 04AB660	//			// 03F85B0	//	TESObjectREFR::SetScale

#define k_Actor_HasPerkInRank						0x00C8CEA0	// 0C0C3F0	// 0C0C060	// 0DA6600	// 0DA64E0	// 0DA64E0	//	Actor::HasPerkInRank
#define k_Actor_SetPosition							0x00C5B630	// 0BDAB80	//			// 0D77690							// Actor::Func00CA
#define k_Actor_PutCreatedPackage					0x00C7C6E0	// 0BFBC30	//			// 0D96C70							// Actor::Func00FA
#define k_Actor_DisableCollision					0x00C578E0	// 0BD6E30	//			// 0D73AB0	//	Actor::DisableCollision in DisableCollisionHandler::DoDisable = Func0001
#define k_Actor_EnableCollision						0x00C57850	// 0BD6DA0	//			// 0D73A20	//	Actor::EnableCollision in EnableCollisionHandler::DoEnable = Func0001
#define k_Actor_StopInteractingQuick				0x00CD6A80	// 0C55FD0	//			// 0E0EAA0							// It is probably a TESObjectREFR member, rather than an Actor member.
#define k_Actor_GetSex								0x00C58410	// 0BD7960	//			// 0D74610
#define k_Actor_GetLevel							0x00C5DEC0	// 0BDD41B	//			// 0D79E90
#define k_Actor_IsDead								0x00C6E8D0	// 0BEDE20	//			// 0D89D40	//	Actor::Func00C0
#define k_Actor_EvaluatePackage						0x00C640B0	// 0BE3600	//			// 0D7F980

#define k_PROCESSTYPE_UpdateEquipment				0x00D21D60	// 0CA12C0	// 0CA0F30	// 0E60860 
	// action should be 0x35 (DefaultObject kActionIdle), checkConditions should be false, targetObj should be null
#define k_PROCESSTYPE_PlayIdle						0x00CF5BB0	// 0C75110	// 0C74D80	// 0E35510
#define k_PROCESSTYPE_StopCurrentIdle				0x00CF5F50	// 0C754B0	//			// 0E358C0
#define k_PROCESSTYPE_ClearCurrentDataforProcess	0x00D34360	// 0CB38C0	//			// 0E77520
#define k_PROCESSTYPE_ClearCurrentPackage			0x00CEC1B0	// 0C6B710	//			// 0E2A670 
#define k_PROCESSTYPE_GetFadeAlpha					0x00D05E70	// 00C853D0	//			// 0E450F0 
#define k_PROCESSTYPE_GetFadeState					0x00D05860	// 0C84DC0	//			// 0E44AD0 
#define k_PROCESSTYPE_ForceFadeToNormal				0x00D05EF0	// 0C85450	//			// 0E45170	// Second func after GetFadeAlpha		// works on player and NPCs , actor cannot be NULL
#define k_PROCESSTYPE_SetIgnoringCombat				0x00CEB980	// 0C6AEE0	//			// 0E29F80	// currently PlayerCharacter_18 with faulty plugin
#define k_PROCESSTYPE_GetIgnoringCombat				0x00CEB970	// 0C6AED0	//			// 0E29F70

#define k_TESObjectCELL_ST_GetDataXandY				0x004C3B80	// 0472C70	//			// 03B1080	//	TESObjectCELL::GetDataXandY
#define k_TESObjectCELL_ST_GetDataX					0x004C3B40	// 0472C30	//			// 03B1040	//	TESObjectCELL::GetDataX
#define k_TESObjectCELL_ST_GetDataY					0x004C3B60	// 0472C50	//			// 03B1060	//	TESObjectCELL::GetDataY
#define k_TESObjectCELL_ST_GetWorldSpace			0x00494C80	// 0443DC0	//			// 0008880	//	TESObjectCELL::GetWorldSpace

#define k_PlayerCharacter_ST_UpdatePlayer3d			0x00D573B0	//	0CD68B0	//			// 0EA0630	//	PlayerCharacter::UpdatePlayer3d

#define k_TESActorBaseData_ST_ModifyFlag			0x00307460	// 02B5F10	//			// 0149720 

#define k_SettingT_INISettingCollection_ST_Changed	0x0179A190	// 1684DB0	// 16849E0	// 1C91E10	// 1C91CF0	// 1C91CF0 

#define k_Cmd_Default_Parse							0x005B31F0	// 05621D0	// 04E7E00
#define k_ExtractArgs								0x005B0370	// 055F350	// 04E37D0

#elif (CURRENT_RELEASE_RUNTIME == RUNTIME_VERSION_1_11_159)	// currently v57

#define k_hackArrayAdd 							0x0213BADD	// 213B3CD	// 1FEBB3D	// 1FEB41D	// 2775A34	// 2775914	// 279A9E4
#define k_hackArrayInsert						0x0213C145	// 213BA35	// 1FEC1A5	// 1FEBA85	// 2775FD9	// 2775EB9	// 279AF89

#define k_FXCamIsActive							0x031D56D8	// 31D4758	// 2F6E578	//	2F6D578	// 59DA130	// 59DA100	// 5A13180

#define k_saveCurrentPlayerCameraState			0x02ECD6C0	// 2ECC6C0	// 2C773E0	// 2C763E0	// 36F1283	// 3729283
#define k_CheckIfCameraStateIsSelected			0x01026650	// 1026090	// 0FA52B0	// 0FA5640	// 0FA52B0	// 124E7F0	// 124E6D0	// 124E6D0
#define k_ToggleFlyCam							0x010229B0	// 10223F0	// 0FA19A0	// 0FA1610	// 124AE60	// 124AD40	// 124AD40

#define k_ControlPlayerCamera					0x0166A490	// 1669DC0	// 15551C0	// 1554DF0	// 1B285A0	// 1B28480	// 1B28480
#define k_ControlPlayer							0x0166A560	// 1669E90	// 1555290	// 1554EC0	// 1B28690	// 1B28570	// 1B28570

#define k_fFreeCameraTranslationSpeed_Camera	0x02F1DEB8	// 2F1CEB8	// 2CC7940	// 2CC6918	// 38048E8	// 383C8E8
#define k_fFreeCameraRotationSpeed_Camera		0x02F1DED0	// 2F1CED0	// 2CC7958	// 2CC6930	// 3804900	// 383C900

#define k_playerCamera_ST						0x030CBA58	// 30CAAD8	// 2E649D8	// 2E639D8	// 58CEB28 
#define k_inputMgr_ST							0x031D5788	// 31D4808	// 2F6E628	// 2F6D628	// 59DA210 
#define k_Main_ST								0x032BCFB0	// 32BC010	// 303ABA8	// 3039BA8	// 5ADD2C8 

#define k_AddScriptAddedForm					0x00586390	// 0586250	// 0535230	// 0535200	// 04B0AC0 

// k_EventSource :
//	BSTGlobalEvent::EventSource_ViewCasterUpdateEvent_::Func0000+22
//	g_BSTGlobalEvent::EventSource_ViewCasterUpdateEvent_ or __ICI__BSTValueEventSource_ViewCasterUpdateEvent___Func0000_to0053_to0051_to0160+24
#define	k_EventSource							0x03258458	// 32574D8	// 2FF12F0	// 2FF02F0	// 5A5F758	// 5A5F728	// 5A987A8	

#define k_collisionState						0x030CD544	// 30CC5C4	// 2E66414	// 2E65414	// 58D08B0	// 5909930	// Cmd_ToggleCollision_Execute + 0x055

// ??_7ExtraTextDisplayData@@6B@
#define k_ExtraTextDisplayDataVtbl				0x024616F0	// 24606F0	// 227CD70	// 227CD70	// 2C52788	// 2C52778	// 2C7C7C8 

#define k_uFGBlinkLeftEyeMorphIndex				0x02F0AD40	// 2F09D40	// 2CB4B00	//			// 037333F8
#define k_uFGBlinkRightEyeMorphIndex			0x02F0AD58	// 2F09D58	// 2CB4B18	//			// 03733410
#define k_fFGBlinkClosedValue					0x02F0AD28	// 2F09D28	// 2CB4AE8	//			// 037333E0

// if I ever get back to this :)
#define k_CallFunction							0x00000000	//			//			//			//			// 0533C00

// And those are class member functions offsets for SharedTools.h

#define k_TESObjectREFR_SetAngleOnReference			0x00510970	// 0510830	// 04BF810	//			// 040BD70	//	TESObjectREFR::SetAngleOnReference
#define k_TESObjectREFR_SetNoCollision				0x004F11B0	// 04F1070	// 04A00A0	//			// 03EDEE0	//	TESObjectREFR::SetNoCollision
#define k_TESObjectREFR_GetRefPersists				0x004F8AA0	// 04F8960	// 04A7990	//			// 03F4DA0	//	TESObjectREFR::GetRefPersists
#define k_TESObjectREFR_SetRefPersists				0x004F8B00	// 04F89C0	// 04A79F0	//			// 03F4E00	//	TESObjectREFR::SetRefPersists
#define k_TESObjectREFR_GetDistanceFromPoint		0x00514450	// 0514310	// 04C32F0	//			// 040F560	//	TESObjectREFR::GetDistanceFromPoint
#define k_TESObjectREFR_GetDistanceFromReference	0x00514510	// 05143D0	// 04C33B0	//			// 040F620	//	TESObjectREFR::GetDistanceFromReference
#define k_TESObjectREFR_GetGoldAmount				0x004FEBF0	// 04FEAB0	// 04ADAF0	//			// 03FA8D0	//	TESObjectREFR::GetGoldAmount
#define k_TESObjectREFR_GetMarkerUser				0x0055B8F0	// 055B7B0	// 050A790	//			// 047BD90	//	TESObjectREFR::GetMarkerUser
#define k_TESObjectREFR_WornHasKeyword				0x005047F0	// 05046B0	// 04B36F0	//			// 0400050	//	TESObjectREFR::WornHasKeyword
#define k_TESObjectREFR_GetScale					0x004FC700	// 04FC5C0	// 04AB5F0	//			// 03F8540	//	TESObjectREFR::GetScale
#define k_TESObjectREFR_SetScale					0x004FC770	// 04FC630	// 04AB660	//			// 03F85B0	//	TESObjectREFR::SetScale

#define k_Actor_HasPerkInRank						0x00C8D460	// 0C8CEA0	// 0C0C3F0	// 0C0C060	// 0DA6600	// 0DA64E0	// 0DA64E0	//	Actor::HasPerkInRank
#define k_Actor_SetPosition							0x00C5BBF0	// 0C5B630	// 0BDAB80	//			// 0D77690	// Actor::Func00CA
#define k_Actor_PutCreatedPackage					0x00C7CCA0	// 0C7C6E0	// 0BFBC30	//			// 0D96C70	// Actor::Func00FA
#define k_Actor_DisableCollision					0x00C57EA0	// 0C578E0	// 0BD6E30	//			// 0D73AB0	//	Actor::DisableCollision in DisableCollisionHandler::DoDisable = Func0001
#define k_Actor_EnableCollision						0x00C57E10	// 0C57850	// 0BD6DA0	//			// 0D73A20	//	Actor::EnableCollision in EnableCollisionHandler::DoEnable = Func0001
#define k_Actor_StopInteractingQuick				0x00CD7040	// 0CD6A80	// 0C55FD0	//			// 0E0EAA0	// It is probably a TESObjectREFR member, rather than an Actor member. TESObjectREFR::StopInteractingQuick
#define k_Actor_GetSex								0x00C589D0	// 0C58410	// 0BD7960	//			// 0D74610	// Actor.GetSex
#define k_Actor_GetLevel							0x00C5E480	// 0C5DEC0	// 0BDD41B	//			// 0D79E90	// Actor::GetLevel
#define k_Actor_IsDead								0x00C6EE90	// 0C6E8D0	// 0BEDE20	//			// 0D89D40	// Actor::Func00C0 = Actor::IsDead
#define k_Actor_EvaluatePackage						0x00C64670	// 0C640B0	// 0BE3600	//			// 0D7F980	// Actor::EvaluatePackage

#define k_PROCESSTYPE_UpdateEquipment				0x00D22320	// 0D21D60	// 0CA12C0	// 0CA0F30	// 0E60860 // MiddleProcess::UpdateEquipment
	// action should be 0x35 (DefaultObject kActionIdle), checkConditions should be false, targetObj should be null
#define k_PROCESSTYPE_PlayIdle						0x00CF6170	// 0CF5BB0	// 0C75110	// 0C74D80	// 0E35510 // PROCESSTYPE::PlayIdle
#define k_PROCESSTYPE_StopCurrentIdle				0x00CF6510	// 0CF5F50	// 0C754B0	//			// 0E358C0 // PROCESSTYPE::StopCurrentIdle
#define k_PROCESSTYPE_ClearCurrentDataforProcess	0x00D34920	// 0D34360	// 0CB38C0	//			// 0E77520 // PROCESSTYPE::ClearCurrentDataforProcess
#define k_PROCESSTYPE_ClearCurrentPackage			0x00CEC770	// 0CEC1B0	// 0C6B710	//			// 0E2A670 // PROCESSTYPE::ClearCurrentPackage
#define k_PROCESSTYPE_GetFadeAlpha					0x00D06430	// 0D05E70	// 00C853D0	//			// 0E450F0 // PROCESSTYPE::GetFadeAlpha
#define k_PROCESSTYPE_GetFadeState					0x00D05E20	// 0D05860	// 0C84DC0	//			// 0E44AD0 // PROCESSTYPE::GetFadeState
#define k_PROCESSTYPE_ForceFadeToNormal				0x00D064B0	// 0D05EF0	// 0C85450	//			// 0E45170 // Actor::AIProcess::ForceFadeToNormal 	// Second func after GetFadeAlpha		// works on player and NPCs , actor cannot be NULL
#define k_PROCESSTYPE_SetIgnoringCombat				0x00CEBF40	// 0CEB980	// 0C6AEE0	//			// 0E29F80 // Actor::AIProcess::SetIgnoringCombat
#define k_PROCESSTYPE_GetIgnoringCombat				0x00CEBF30	// 0CEB970	// 0C6AED0	//			// 0E29F70 // Actor::AIProcess::GetIgnoringCombat

#define k_TESObjectCELL_ST_GetDataXandY				0x004C3CC0	// 04C3B80	// 0472C70	//			// 03B1080 // TESObjectCELL::GetDataXandY
#define k_TESObjectCELL_ST_GetDataX					0x004C3C80	// 04C3B40	// 0472C30	//			// 03B1040 // TESObjectCELL::GetDataX
#define k_TESObjectCELL_ST_GetDataY					0x004C3CA0	// 04C3B60	// 0472C50	//			// 03B1060 // TESObjectCELL::GetDataY
#define k_TESObjectCELL_ST_GetWorldSpace			0x00494DC0	// 0494C80	// 0443DC0	//			// 0008880 // TESObjectCELL::GetWorldSpace

#define k_PlayerCharacter_ST_UpdatePlayer3d			0x00D57970	// 0D573B0	//	0CD68B0	//			// 0EA0630 // PlayerCharacter::UpdatePlayer3d

#define k_TESActorBaseData_ST_ModifyFlag			0x003075A0	// 0307460	// 02B5F10	//			// 0149720 // TESActorBaseData::ModifyFlag

#define k_SettingT_INISettingCollection_ST_Changed	0x0179A860	// 179A190	// 1684DB0	// 16849E0	// 1C91E10	// 1C91CF0	// 1C91CF0 // SettingT_INISettingCollection::Changed

#define k_Cmd_Default_Parse							0x005B3330	// 05B31F0	// 05621D0	// 04E7E00	//			// Cmd_Default_Parse
#define k_ExtractArgs								0x005B04B0	// 05B0370	// 055F350	// 04E37D0	//			// ExtractArgs

#elif (CURRENT_RELEASE_RUNTIME == RUNTIME_VERSION_1_11_169)	// currently v58

#define k_hackArrayAdd 							0x0213C72D	// 213BADD	// 213B3CD	// 1FEBB3D	// 1FEB41D	// 2775A34	// 2775914	// 279A9E4
#define k_hackArrayInsert						0x0213CD95	// 213C145	// 213BA35	// 1FEC1A5	// 1FEBA85	// 2775FD9	// 2775EB9	// 279AF89

#define k_FXCamIsActive							0x031DA8D8	// 31D56D8	// 31D4758	// 2F6E578	//	2F6D578	// 59DA130	// 59DA100	// 5A13180

#define k_saveCurrentPlayerCameraState			0x02ED26C0	// 2ECD6C0	// 2ECC6C0	// 2C773E0	// 2C763E0	// 36F1283	// 3729283
#define k_CheckIfCameraStateIsSelected			0x01026B00	// 1026650	// 1026090	// 0FA52B0	// 0FA5640	// 0FA52B0	// 124E7F0	// 124E6D0	// 124E6D0
#define k_ToggleFlyCam							0x01022E60	// 10229B0	// 10223F0	// 0FA19A0	// 0FA1610	// 124AE60	// 124AD40	// 124AD40	// PlayerCamera::ToggleFlyCam

#define k_ControlPlayerCamera					0x0166AE10	// 166A490	// 1669DC0	// 15551C0	// 1554DF0	// 1B285A0	// 1B28480	// 1B28480	// InputManager::ControlPlayerCamera
#define k_ControlPlayer							0x0166AEE0	// 166A560	// 1669E90	// 1555290	// 1554EC0	// 1B28690	// 1B28570	// 1B28570	// InputManager::ControlPlayer

#define k_fFreeCameraTranslationSpeed_Camera	0x02F22EB8	// 2F1DEB8	// 2F1CEB8	// 2CC7940	// 2CC6918	// 38048E8	// 383C8E8	// aFfreecameratra
#define k_fFreeCameraRotationSpeed_Camera		0x02F22ED0	// 2F1DED0	// 2F1CED0	// 2CC7958	// 2CC6930	// 3804900	// 383C900

#define k_playerCamera_ST						0x030D0C58	// 30CBA58	// 30CAAD8	// 2E649D8	// 2E639D8	// 58CEB28 
#define k_inputMgr_ST							0x031DA988	// 31D5788	// 31D4808	// 2F6E628	// 2F6D628	// 59DA210 
#define k_Main_ST								0x032C21B0	// 32BCFB0	// 32BC010	// 303ABA8	// 3039BA8	// 5ADD2C8 

#define k_AddScriptAddedForm					0x00586780	// 0586390	// 0586250	// 0535230	// 0535200	// 04B0AC0	// AddScriptAddedForm

// k_EventSource :
//	BSTGlobalEvent::EventSource_ViewCasterUpdateEvent_::Func0000+22
//	g_BSTGlobalEvent::EventSource_ViewCasterUpdateEvent_ or __ICI__BSTValueEventSource_ViewCasterUpdateEvent___Func0000_to0053_to0051_to0160+24
#define	k_EventSource							0x0325D658	// 3258458	// 32574D8	// 2FF12F0	// 2FF02F0	// 5A5F758	// 5A5F728	// 5A987A8	

#define k_collisionState						0x030D2744	// 30CD544	// 30CC5C4	// 2E66414	// 2E65414	// 58D08B0	// 5909930	// Cmd_ToggleCollision_Execute + 0x055

// ??_7ExtraTextDisplayData@@6B@
#define k_ExtraTextDisplayDataVtbl				0x024636F0	// 24616F0	// 24606F0	// 227CD70	// 227CD70	// 2C52788	// 2C52778	// 2C7C7C8 // ??_7ExtraTextDisplayData@@6B@

#define k_uFGBlinkLeftEyeMorphIndex				0x02F0FD40	// 2F0AD40	// 2F09D40	// 2CB4B00	//			// 037333F8
#define k_uFGBlinkRightEyeMorphIndex			0x02F0FD58	// 2F0AD58	// 2F09D58	// 2CB4B18	//			// 03733410
#define k_fFGBlinkClosedValue					0x02F0FD28	// 2F0AD28	// 2F09D28	// 2CB4AE8	//			// 037333E0

// if I ever get back to this :)
#define k_CallFunction							0x00000000	//			//			//			//			// 0533C00

// And those are class member functions offsets for SharedTools.h

#define k_TESObjectREFR_SetAngleOnReference			0x00510D60	// 0510970	// 0510830	// 04BF810	//			// 040BD70	//	TESObjectREFR::SetAngleOnReference
#define k_TESObjectREFR_SetNoCollision				0x004F15A0	// 04F11B0	// 04F1070	// 04A00A0	//			// 03EDEE0	//	TESObjectREFR::SetNoCollision
#define k_TESObjectREFR_GetRefPersists				0x004F8E90	// 04F8AA0	// 04F8960	// 04A7990	//			// 03F4DA0	//	TESObjectREFR::GetRefPersists
#define k_TESObjectREFR_SetRefPersists				0x004F8EF0	// 04F8B00	// 04F89C0	// 04A79F0	//			// 03F4E00	//	TESObjectREFR::SetRefPersists
#define k_TESObjectREFR_GetDistanceFromPoint		0x00514840	// 0514450	// 0514310	// 04C32F0	//			// 040F560	//	TESObjectREFR::GetDistanceFromPoint
#define k_TESObjectREFR_GetDistanceFromReference	0x00514900	// 0514510	// 05143D0	// 04C33B0	//			// 040F620	//	TESObjectREFR::GetDistanceFromReference
#define k_TESObjectREFR_GetGoldAmount				0x004FEFE0	// 04FEBF0	// 04FEAB0	// 04ADAF0	//			// 03FA8D0	//	TESObjectREFR::GetGoldAmount
#define k_TESObjectREFR_GetMarkerUser				0x0055BCE0	// 055B8F0	// 055B7B0	// 050A790	//			// 047BD90	//	TESObjectREFR::GetMarkerUser
#define k_TESObjectREFR_WornHasKeyword				0x00504BE0	// 05047F0	// 05046B0	// 04B36F0	//			// 0400050	//	TESObjectREFR::WornHasKeyword
#define k_TESObjectREFR_GetScale					0x004FCAF0	// 04FC700	// 04FC5C0	// 04AB5F0	//			// 03F8540	//	TESObjectREFR::GetScale
#define k_TESObjectREFR_SetScale					0x004FCB60	// 04FC770	// 04FC630	// 04AB660	//			// 03F85B0	//	TESObjectREFR::SetScale

#define k_Actor_HasPerkInRank						0x00C8D910	// 0C8D460	// 0C8CEA0	// 0C0C3F0	// 0C0C060	// 0DA6600	// 0DA64E0	// 0DA64E0	//	Actor::HasPerkInRank
#define k_Actor_SetPosition							0x00C5C0A0	// 0C5BBF0	// 0C5B630	// 0BDAB80	//			// 0D77690	// Actor::Func00CA
#define k_Actor_PutCreatedPackage					0x00C7D150	// 0C7CCA0	// 0C7C6E0	// 0BFBC30	//			// 0D96C70	// Actor::Func00FA
#define k_Actor_DisableCollision					0x00C58350	// 0C57EA0	// 0C578E0	// 0BD6E30	//			// 0D73AB0	//	Actor::DisableCollision in DisableCollisionHandler::DoDisable = Func0001
#define k_Actor_EnableCollision						0x00C582C0	// 0C57E10	// 0C57850	// 0BD6DA0	//			// 0D73A20	//	Actor::EnableCollision in EnableCollisionHandler::DoEnable = Func0001
#define k_Actor_StopInteractingQuick				0x00CD74F0	// 0CD7040	// 0CD6A80	// 0C55FD0	//			// 0E0EAA0	// It is probably a TESObjectREFR member, rather than an Actor member. TESObjectREFR::StopInteractingQuick
#define k_Actor_GetSex								0x00C58E80	// 0C589D0	// 0C58410	// 0BD7960	//			// 0D74610	// Actor.GetSex
#define k_Actor_GetLevel							0x00C5E930	// 0C5E480	// 0C5DEC0	// 0BDD41B	//			// 0D79E90	// Actor::GetLevel
#define k_Actor_IsDead								0x00C6F340	// 0C6EE90	// 0C6E8D0	// 0BEDE20	//			// 0D89D40	// Actor::Func00C0 = Actor::IsDead
#define k_Actor_EvaluatePackage						0x00C64B20	// 0C64670	// 0C640B0	// 0BE3600	//			// 0D7F980	// Actor::EvaluatePackage

#define k_PROCESSTYPE_UpdateEquipment				0x00D227D0	// 0D22320	// 0D21D60	// 0CA12C0	// 0CA0F30	// 0E60860	// MiddleProcess::UpdateEquipment or Actor::AIProcess::DoUpdate3dModel
	// action should be 0x35 (DefaultObject kActionIdle), checkConditions should be false, targetObj should be null
#define k_PROCESSTYPE_PlayIdle						0x00CF6620	// 0CF6170	// 0CF5BB0	// 0C75110	// 0C74D80	// 0E35510 // Actor::AIProcess::PlayIdle
#define k_PROCESSTYPE_StopCurrentIdle				0x00CF69C0	// 0CF6510	// 0CF5F50	// 0C754B0	//			// 0E358C0 // Actor::AIProcess::StopCurrentIdle
#define k_PROCESSTYPE_ClearCurrentDataforProcess	0x00D34DD0	// 0D34920	// 0D34360	// 0CB38C0	//			// 0E77520 // Actor::AIProcess::ClearCurrentDataforProcess
#define k_PROCESSTYPE_ClearCurrentPackage			0x00CECC20	// 0CEC770	// 0CEC1B0	// 0C6B710	//			// 0E2A670 // Actor::AIProcess::ClearCurrentPackage
#define k_PROCESSTYPE_GetFadeAlpha					0x00D068E0	// 0D06430	// 0D05E70	// 00C853D0	//			// 0E450F0 // Actor::AIProcess::GetFadeAlpha
#define k_PROCESSTYPE_GetFadeState					0x00D062D0	// 0D05E20	// 0D05860	// 0C84DC0	//			// 0E44AD0 // Actor::AIProcess::GetFadeState
#define k_PROCESSTYPE_ForceFadeToNormal				0x00D06960	// 0D064B0	// 0D05EF0	// 0C85450	//			// 0E45170 // Actor::AIProcess::ForceFadeToNormal 	// Second func after GetFadeAlpha		// works on player and NPCs , actor cannot be NULL
#define k_PROCESSTYPE_SetIgnoringCombat				0x00CEC3F0	// 0CEBF40	// 0CEB980	// 0C6AEE0	//			// 0E29F80 // Actor::AIProcess::SetIgnoringCombat
#define k_PROCESSTYPE_GetIgnoringCombat				0x00CEC3E0	// 0CEBF30	// 0CEB970	// 0C6AED0	//			// 0E29F70 // Actor::AIProcess::GetIgnoringCombat

#define k_TESObjectCELL_ST_GetDataXandY				0x004C40B0	// 04C3CC0	// 04C3B80	// 0472C70	//			// 03B1080 // TESObjectCELL::GetDataXandY
#define k_TESObjectCELL_ST_GetDataX					0x004C4070	// 04C3C80	// 04C3B40	// 0472C30	//			// 03B1040 // TESObjectCELL::GetDataX
#define k_TESObjectCELL_ST_GetDataY					0x004C4090	// 04C3CA0	// 04C3B60	// 0472C50	//			// 03B1060 // TESObjectCELL::GetDataY
#define k_TESObjectCELL_ST_GetWorldSpace			0x004951B0	// 0494DC0	// 0494C80	// 0443DC0	//			// 0008880 // TESObjectCELL::GetWorldSpace

#define k_PlayerCharacter_ST_UpdatePlayer3d			0x00D57E20	// 0D57970	// 0D573B0	//	0CD68B0	//			// 0EA0630 // PlayerCharacter::UpdatePlayer3d

#define k_TESActorBaseData_ST_ModifyFlag			0x00307990	// 03075A0	// 0307460	// 02B5F10	//			// 0149720 // TESActorBaseData::ModifyFlag

#define k_SettingT_INISettingCollection_ST_Changed	0x0179B1E0	// 179A860	// 179A190	// 1684DB0	// 16849E0	// 1C91E10	// 1C91CF0	// 1C91CF0 // SettingT_INISettingCollection::Changed

#define k_Cmd_Default_Parse							0x005B3720	// 05B3330	// 05B31F0	// 05621D0	// 04E7E00	//			// Cmd_Default_Parse
#define k_ExtractArgs								0x005B08A0	// 05B04B0	// 05B0370	// 055F350	// 04E37D0	//			// ExtractArgs

#elif (CURRENT_RELEASE_RUNTIME == RUNTIME_VERSION_1_11_191)	// currently v59

#define k_hackArrayAdd 							0x0214121D	// 213C72D	// 213BADD	// 213B3CD	// 1FEBB3D	// 1FEB41D	// 2775A34	// 2775914	// 279A9E4
#define k_hackArrayInsert						0x02141885	// 213CD95	// 213C145	// 213BA35	// 1FEC1A5	// 1FEBA85	// 2775FD9	// 2775EB9	// 279AF89

#define k_FXCamIsActive							0x031E59D8	// 31DA8D8	// 31D56D8	// 31D4758	// 2F6E578	//	2F6D578	// 59DA130	// 59DA100	// 5A13180

#define k_saveCurrentPlayerCameraState			0x02EDD6C0	// 2ED26C0	// 2ECD6C0	// 2ECC6C0	// 2C773E0	// 2C763E0	// 36F1283	// 3729283
#define k_CheckIfCameraStateIsSelected			0x0102B090	// 1026B00	// 1026650	// 1026090	// 0FA52B0	// 0FA5640	// 0FA52B0	// 124E7F0	// 124E6D0	// 124E6D0
#define k_ToggleFlyCam							0x010273F0	// 1022E60	// 10229B0	// 10223F0	// 0FA19A0	// 0FA1610	// 124AE60	// 124AD40	// 124AD40	// PlayerCamera::ToggleFlyCam

#define k_ControlPlayerCamera					0x0166F330	// 166AE10	// 166A490	// 1669DC0	// 15551C0	// 1554DF0	// 1B285A0	// 1B28480	// 1B28480	// InputManager::ControlPlayerCamera
#define k_ControlPlayer							0x0166F400	// 166AEE0	// 166A560	// 1669E90	// 1555290	// 1554EC0	// 1B28690	// 1B28570	// 1B28570	// InputManager::ControlPlayer

// since 984 used as offset - 8, so was in error for all  previous versions except 984. Reverted to directoffset i the source.
#define k_fFreeCameraTranslationSpeed_Camera	0x02F2DEC8	// 2F22EB8	// 2F1DEB8	// 2F1CEB8	// 2CC7940	// 2CC6918	// 38048E8	// 383C8E8	// aFfreecameratra
#define k_fFreeCameraRotationSpeed_Camera		0x02F2DEE0	// 2F22ED0	// 2F1DED0	// 2F1CED0	// 2CC7958	// 2CC6930	// 3804900	// 383C900

#define k_playerCamera_ST						0x030DBD58	// 30D0C58	// 30CBA58	// 30CAAD8	// 2E649D8	// 2E639D8	// 58CEB28 
#define k_inputMgr_ST							0x031E5A88	// 31DA988	// 31D5788	// 31D4808	// 2F6E628	// 2F6D628	// 59DA210 
#define k_Main_ST								0x032D2020	// 32C21B0	// 32BCFB0	// 32BC010	// 303ABA8	// 3039BA8	// 5ADD2C8 

#define k_AddScriptAddedForm					0x005891B0	// 0586780	// 0586390	// 0586250	// 0535230	// 0535200	// 04B0AC0	// AddScriptAddedForm	RE::ID::BGSListForm::AddScriptAddedForm

// k_EventSource :
//	BSTGlobalEvent::EventSource_ViewCasterUpdateEvent_::Func0000+22
//	g_BSTGlobalEvent::EventSource_ViewCasterUpdateEvent_ or __ICI__BSTValueEventSource_ViewCasterUpdateEvent___Func0000_to0053_to0051_to0160+24
#define	k_EventSource							0x03268758	// 325D658	// 3258458	// 32574D8	// 2FF12F0	// 2FF02F0	// 5A5F758	// 5A5F728	// 5A987A8	// Also from ADF4::ID2219564

#define k_collisionState						0x030DD844	// 30D2744	// 30CD544	// 30CC5C4	// 2E66414	// 2E65414	// 58D08B0	// 5909930	// Cmd_ToggleCollision_Execute + 0x055

// ??_7ExtraTextDisplayData@@6B@
#define k_ExtraTextDisplayDataVtbl				0x02468700	// 24636F0	// 24616F0	// 24606F0	// 227CD70	// 227CD70	// 2C52788	// 2C52778	// 2C7C7C8 // ??_7ExtraTextDisplayData@@6B@

#define k_uFGBlinkLeftEyeMorphIndex				0x02F1AD40	// 2F0FD40	// 2F0AD40	// 2F09D40	// 2CB4B00	//			// 037333F8
#define k_uFGBlinkRightEyeMorphIndex			0x02F1AD58	// 2F0FD58	// 2F0AD58	// 2F09D58	// 2CB4B18	//			// 03733410
#define k_fFGBlinkClosedValue					0x02F1AD28	// 2F0FD28	// 2F0AD28	// 2F09D28	// 2CB4AE8	//			// 037333E0

// if I ever get back to this :)
#define k_CallFunction							0x00000000	//			//			//			//			// 0533C00

// And those are class member functions offsets for SharedTools.h

#define k_TESObjectREFR_SetAngleOnReference			0x00513790	// 0510D60	// 0510970	// 0510830	// 04BF810	//			// 040BD70	//	TESObjectREFR::SetAngleOnReference
#define k_TESObjectREFR_SetNoCollision				0x004F3FD0	// 04F15A0	// 04F11B0	// 04F1070	// 04A00A0	//			// 03EDEE0	//	TESObjectREFR::SetNoCollision also ADF4::ID2200734
#define k_TESObjectREFR_GetRefPersists				0x004FB8C0	// 04F8E90	// 04F8AA0	// 04F8960	// 04A7990	//			// 03F4DA0	//	TESObjectREFR::GetRefPersists also ADF4::ID2200812
#define k_TESObjectREFR_SetRefPersists				0x004FB920	// 04F8EF0	// 04F8B00	// 04F89C0	// 04A79F0	//			// 03F4E00	//	TESObjectREFR::SetRefPersists also ADF4::ID2200816
#define k_TESObjectREFR_GetDistanceFromPoint		0x00517270	// 0514840	// 0514450	// 0514310	// 04C32F0	//			// 040F560	//	TESObjectREFR::GetDistanceFromPoint also ADF4::ID2201172
#define k_TESObjectREFR_GetDistanceFromReference	0x00517330	// 0514900	// 0514510	// 05143D0	// 04C33B0	//			// 040F620	//	TESObjectREFR::GetDistanceFromReference also ADF4::ID2201174
#define k_TESObjectREFR_GetGoldAmount				0x00501A10	// 04FEFE0	// 04FEBF0	// 04FEAB0	// 04ADAF0	//			// 03FA8D0	//	TESObjectREFR::GetGoldAmount also ADF4::ID2200923 ADF4::RE::ID::TESObjectREFR::GetGoldAmount
#define k_TESObjectREFR_GetMarkerUser				0x0055E710	// 055BCE0	// 055B8F0	// 055B7B0	// 050A790	//			// 047BD90	//	TESObjectREFR::GetMarkerUser also ADF4::ID2202566
#define k_TESObjectREFR_WornHasKeyword				0x00507610	// 0504BE0	// 05047F0	// 05046B0	// 04B36F0	//			// 0400050	//	TESObjectREFR::WornHasKeyword also ADF4::ID2200995
#define k_TESObjectREFR_GetScale					0x004FF520	// 04FCAF0	// 04FC700	// 04FC5C0	// 04AB5F0	//			// 03F8540	//	TESObjectREFR::GetScale also ADF4::ID2200892
#define k_TESObjectREFR_SetScale					0x004FF590	// 04FCB60	// 04FC770	// 04FC630	// 04AB660	//			// 03F85B0	//	TESObjectREFR::SetScale also ADF4::ID2200893 ADF4::RE::ID::TESObjectREFR::SetScale

#define k_Actor_HasPerkInRank						0x00C91EA0	// 0C8D910	// 0C8D460	// 0C8CEA0	// 0C0C3F0	// 0C0C060	// 0DA6600	// 0DA64E0	// 0DA64E0	//	Actor::HasPerkInRank also ADF4::ID2230125 ADF4::RE::ID::Actor::GetPerkRank
#define k_Actor_SetPosition							0x00C60630	// 0C5C0A0	// 0C5BBF0	// 0C5B630	// 0BDAB80	//			// 0D77690	// Actor::Func00CA also Actor::SetPosition
#define k_Actor_PutCreatedPackage					0x00C816E0	// 0C7D150	// 0C7CCA0	// 0C7C6E0	// 0BFBC30	//			// 0D96C70	// Actor::Func00FA also Actor::PutCreatedPackage
#define k_Actor_DisableCollision					0x00C5C8E0	// 0C58350	// 0C57EA0	// 0C578E0	// 0BD6E30	//			// 0D73AB0	//	Actor::DisableCollision in DisableCollisionHandler::DoDisable = Func0001 also ADF4::ID2229653
#define k_Actor_EnableCollision						0x00C5C850	// 0C582C0	// 0C57E10	// 0C57850	// 0BD6DA0	//			// 0D73A20	//	Actor::EnableCollision in EnableCollisionHandler::DoEnable = Func0001 also ADF4::ID2229652
#define k_Actor_StopInteractingQuick				0x00CDBA80	// 0CD74F0	// 0CD7040	// 0CD6A80	// 0C55FD0	//			// 0E0EAA0	// It is probably a TESObjectREFR member, rather than an Actor member. TESObjectREFR::StopInteractingQuick also ADF4::ID2231227 ADF4::RE::ID::Actor::StopInteractingQuick
#define k_Actor_GetSex								0x00C5D410	// 0C58E80	// 0C589D0	// 0C58410	// 0BD7960	//			// 0D74610	// Actor::GetSex also ADF4::ID2229674 ADF4::RE::ID::Actor::GetSex
#define k_Actor_GetLevel							0x00C62EC0	// 0C5E930	// 0C5E480	// 0C5DEC0	// 0BDD41B	//			// 0D79E90	// Actor::GetLevel also ADF4::ID2229734 ADF4::RE::ID::Actor::GetLevel
#define k_Actor_IsDead								0x00C738D0	// 0C6F340	// 0C6EE90	// 0C6E8D0	// 0BEDE20	//			// 0D89D40	// Actor::Func00C0 = Actor::IsDead also ADF4::ID2229903
#define k_Actor_EvaluatePackage						0x00C690B0	// 0C64B20	// 0C64670	// 0C640B0	// 0BE3600	//			// 0D7F980	// Actor::EvaluatePackage also ADF4::ID2229805

#define k_PROCESSTYPE_UpdateEquipment				0x00D26D60	// 0D227D0	// 0D22320	// 0D21D60	// 0CA12C0	// 0CA0F30	// 0E60860	// MiddleProcess::UpdateEquipment or Actor::AIProcess::DoUpdate3dModel also ADF4::ID2232144
	// action should be 0x35 (DefaultObject kActionIdle), checkConditions should be false, targetObj should be null
#define k_PROCESSTYPE_PlayIdle						0x00CFABB0	// 0CF6620	// 0CF6170	// 0CF5BB0	// 0C75110	// 0C74D80	// 0E35510	// Actor::AIProcess::PlayIdle also ADF4::ID2231704 ADF4::RE::ID::AIProcess::SetupSpecialIdle
#define k_PROCESSTYPE_StopCurrentIdle				0x00CFAF50	// 0CF69C0	// 0CF6510	// 0CF5F50	// 0C754B0	//			// 0E358C0	// Actor::AIProcess::StopCurrentIdle also ADF4::ID2231705 ADF4::RE::ID::AIProcess::StopCurrentIdle
#define k_PROCESSTYPE_ClearCurrentDataforProcess	0x00D39360	// 0D34DD0	// 0D34920	// 0D34360	// 0CB38C0	//			// 0E77520	// Actor::AIProcess::ClearCurrentDataforProcess also ADF4::ID2232435
#define k_PROCESSTYPE_ClearCurrentPackage			0x00CF11B0	// 0CECC20	// 0CEC770	// 0CEC1B0	// 0C6B710	//			// 0E2A670	// Actor::AIProcess::ClearCurrentPackage also ADF4::ID2231582
#define k_PROCESSTYPE_GetFadeAlpha					0x00D0AE70	// 0D068E0	// 0D06430	// 0D05E70	// 00C853D0	//			// 0E450F0	// Actor::AIProcess::GetFadeAlpha also ADF4__ID2231942
#define k_PROCESSTYPE_GetFadeState					0x00D0A860	// 0D062D0	// 0D05E20	// 0D05860	// 0C84DC0	//			// 0E44AD0	// Actor::AIProcess::GetFadeState also ADF4::ID2231935
#define k_PROCESSTYPE_ForceFadeToNormal				0x00D0AEF0	// 0D06960	// 0D064B0	// 0D05EF0	// 0C85450	//			// 0E45170	// Actor::AIProcess::ForceFadeToNormal also ADF4__ID2231944	// Second func after GetFadeAlpha		// works on player and NPCs , actor cannot be NULL
#define k_PROCESSTYPE_SetIgnoringCombat				0x00CF0986	// 0CEC3F0	// 0CEBF40	// 0CEB980	// 0C6AEE0	//			// 0E29F80	// Actor::AIProcess::SetIgnoringCombat also ADF4::ID2231554
#define k_PROCESSTYPE_GetIgnoringCombat				0x00CF0970	// 0CEC3E0	// 0CEBF30	// 0CEB970	// 0C6AED0	//			// 0E29F70	// Actor::AIProcess::GetIgnoringCombat also ADF4::ID2231553

#define k_TESObjectCELL_ST_GetDataXandY				0x004C6AE0	// 04C40B0	// 04C3CC0	// 04C3B80	// 0472C70	//			// 03B1080	// TESObjectCELL::GetDataXandY also ADF4::ID2200215
#define k_TESObjectCELL_ST_GetDataX					0x004C6AA0	// 04C4070	// 04C3C80	// 04C3B40	// 0472C30	//			// 03B1040	// TESObjectCELL::GetDataX also ADF4::ID2200213 ADF4::RE::ID::TESObjectCELL::GetDataX
#define k_TESObjectCELL_ST_GetDataY					0x004C6AC0	// 04C4090	// 04C3CA0	// 04C3B60	// 0472C50	//			// 03B1060	// TESObjectCELL::GetDataY also ADF4::ID2200214 ADF4::RE::ID::TESObjectCELL::GetDataY
#define k_TESObjectCELL_ST_GetWorldSpace			0x00497BE0	// 04951B0	// 0494DC0	// 0494C80	// 0443DC0	//			// 0008880	// TESObjectCELL::GetWorldSpace also ADF4::ID2199522

#define k_PlayerCharacter_ST_UpdatePlayer3d			0x00D5C3B0	// 0D57E20	// 0D57970	// 0D573B0	//	0CD68B0	//			// 0EA0630	// PlayerCharacter::UpdatePlayer3d also ADF4::ID2232941

#define k_TESActorBaseData_ST_ModifyFlag			0x0030A3C0	// 0307990	// 03075A0	// 0307460	// 02B5F10	//			// 0149720	// TESActorBaseData::ModifyFlag also ADF4::ID2192890

#define k_SettingT_INISettingCollection_ST_Changed	0x0179F920	// 179B1E0	// 179A860	// 179A190	// 1684DB0	// 16849E0	// 1C91E10	// 1C91CF0	// 1C91CF0 // SettingT_INISettingCollection::Changed also ADF4::ID2274827

#define k_Cmd_Default_Parse							0x005B6150	// 05B3720	// 05B3330	// 05B31F0	// 05621D0	// 04E7E00	//			// Cmd_GetWantBlocking_Parse also Cmd_Default_Parse also ADF4::ID2204343 ADF4::RE::ID::SCRIPT_FUNCTION::CompileFunction
#define k_ExtractArgs								0x005B32D0	// 05B08A0	// 05B04B0	// 05B0370	// 055F350	// 04E37D0	//			// ExtractArgs also ADF4::RE::ID::RTDynamicCast ADF4::ID4818455

#define k_GetDisplayModeList						0x01824350	                                                                        // Buffout4::CreateD3DAndSwapChainFix::GetDisplayModeList also ADF4::ID4492363

#elif (CURRENT_RELEASE_RUNTIME == RUNTIME_VERSION_1_11_221)	// currently v60

#define k_hackArrayAdd 							0x021412FD	// 214121D + 0xE0	// 213C72D	// 213BADD	// 213B3CD	// 1FEBB3D	// 1FEB41D	// 2775A34	// 2775914	// 279A9E4
#define k_hackArrayInsert						0x02141965	// 2141885 + 0xE0	// 213CD95	// 213C145	// 213BA35	// 1FEC1A5	// 1FEBA85	// 2775FD9	// 2775EB9	// 279AF89

#define k_FXCamIsActive							0x031E5A58	// 31E59D8	// 31DA8D8	// 31D56D8	// 31D4758	// 2F6E578	//	2F6D578	// 59DA130	// 59DA100	// 5A13180

#define k_saveCurrentPlayerCameraState			0x02EDD6C0	// 2EDD6C0	// 2ED26C0	// 2ECD6C0	// 2ECC6C0	// 2C773E0	// 2C763E0	// 36F1283	// 3729283
#define k_CheckIfCameraStateIsSelected			0x0102B230	// 102B090	// 1026B00	// 1026650	// 1026090	// 0FA52B0	// 0FA5640	// 0FA52B0	// 124E7F0	// 124E6D0	// 124E6D0
#define k_ToggleFlyCam							0x01027590	// 10273F0	// 1022E60	// 10229B0	// 10223F0	// 0FA19A0	// 0FA1610	// 124AE60	// 124AD40	// 124AD40	// PlayerCamera::ToggleFlyCam

#define k_ControlPlayerCamera					0x0166F430	// 166F330	// 166AE10	// 166A490	// 1669DC0	// 15551C0	// 1554DF0	// 1B285A0	// 1B28480	// 1B28480	// InputManager::ControlPlayerCamera
#define k_ControlPlayer							0x0166F500	// 166F400	// 166AEE0	// 166A560	// 1669E90	// 1555290	// 1554EC0	// 1B28690	// 1B28570	// 1B28570	// InputManager::ControlPlayer

#define k_fFreeCameraTranslationSpeed_Camera	0x02F2DF18	// 2F2DEC8	// 2F22EB8	// 2F1DEB8	// 2F1CEB8	// 2CC7940	// 2CC6918	// 38048E8	// 383C8E8	// aFfreecameratra
#define k_fFreeCameraRotationSpeed_Camera		0x02F2DF30	// 2F2DEE0	// 2F22ED0	// 2F1DED0	// 2F1CED0	// 2CC7958	// 2CC6930	// 3804900	// 383C900

#define k_playerCamera_ST						0x030DBDD8	// 30DBD58	// 30D0C58	// 30CBA58	// 30CAAD8	// 2E649D8	// 2E639D8	// 58CEB28 
#define k_inputMgr_ST							0x031E5B08	// 31E5A88	// 31DA988	// 31D5788	// 31D4808	// 2F6E628	// 2F6D628	// 59DA210 
#define k_Main_ST								0x032D20A0	// 32D2020	// 32C21B0	// 32BCFB0	// 32BC010	// 303ABA8	// 3039BA8	// 5ADD2C8 

#define k_AddScriptAddedForm					0x005891C0	// 05891B0	// 0586780	// 0586390	// 0586250	// 0535230	// 0535200	// 04B0AC0	// AddScriptAddedForm	RE::ID::BGSListForm::AddScriptAddedForm

// k_EventSource :
//	BSTGlobalEvent::EventSource_ViewCasterUpdateEvent_::Func0000+22
//	g_BSTGlobalEvent::EventSource_ViewCasterUpdateEvent_ or __ICI__BSTValueEventSource_ViewCasterUpdateEvent___Func0000_to0053_to0051_to0160+24
#define	k_EventSource							0x032687D8	// 3268758	// 325D658	// 3258458	// 32574D8	// 2FF12F0	// 2FF02F0	// 5A5F758	// 5A5F728	// 5A987A8	// Also from ADF4::ID2219564

#define k_collisionState						0x030DD8C4	// 30DD844	// 30D2744	// 30CD544	// 30CC5C4	// 2E66414	// 2E65414	// 58D08B0	// 5909930	// Cmd_ToggleCollision_Execute + 0x055

// ??_7ExtraTextDisplayData@@6B@
#define k_ExtraTextDisplayDataVtbl				0x02468700	// 2468700	// 24636F0	// 24616F0	// 24606F0	// 227CD70	// 227CD70	// 2C52788	// 2C52778	// 2C7C7C8 // ??_7ExtraTextDisplayData@@6B@

#define k_uFGBlinkLeftEyeMorphIndex				0x02F1AD90	// 2F1AD40	// 2F0FD40	// 2F0AD40	// 2F09D40	// 2CB4B00	//			// 037333F8
#define k_uFGBlinkRightEyeMorphIndex			0x02F1ADA8	// 2F1AD58	// 2F0FD58	// 2F0AD58	// 2F09D58	// 2CB4B18	//			// 03733410
#define k_fFGBlinkClosedValue					0x02F1AD78	// 2F1AD28	// 2F0FD28	// 2F0AD28	// 2F09D28	// 2CB4AE8	//			// 037333E0

// if I ever get back to this :)
#define k_CallFunction							0x00000000	//			//			//			//			// 0533C00

// And those are class member functions offsets for SharedTools.h

#define k_TESObjectREFR_SetAngleOnReference			0x005137A0	// 0513790	// 0510D60	// 0510970	// 0510830	// 04BF810	//			// 040BD70	//	TESObjectREFR::SetAngleOnReference
#define k_TESObjectREFR_SetNoCollision				0x004F3FE0	// 04F3FD0	// 04F15A0	// 04F11B0	// 04F1070	// 04A00A0	//			// 03EDEE0	//	TESObjectREFR::SetNoCollision also ADF4::ID2200734
#define k_TESObjectREFR_GetRefPersists				0x004FB8D0	// 04FB8C0	// 04F8E90	// 04F8AA0	// 04F8960	// 04A7990	//			// 03F4DA0	//	TESObjectREFR::GetRefPersists also ADF4::ID2200812
#define k_TESObjectREFR_SetRefPersists				0x004FB930	// 04FB920	// 04F8EF0	// 04F8B00	// 04F89C0	// 04A79F0	//			// 03F4E00	//	TESObjectREFR::SetRefPersists also ADF4::ID2200816
#define k_TESObjectREFR_GetDistanceFromPoint		0x00517280	// 0517270	// 0514840	// 0514450	// 0514310	// 04C32F0	//			// 040F560	//	TESObjectREFR::GetDistanceFromPoint also ADF4::ID2201172
#define k_TESObjectREFR_GetDistanceFromReference	0x00517340	// 0517330	// 0514900	// 0514510	// 05143D0	// 04C33B0	//			// 040F620	//	TESObjectREFR::GetDistanceFromReference also ADF4::ID2201174
#define k_TESObjectREFR_GetGoldAmount				0x00501A20	// 0501A10	// 04FEFE0	// 04FEBF0	// 04FEAB0	// 04ADAF0	//			// 03FA8D0	//	TESObjectREFR::GetGoldAmount also ADF4::ID2200923 ADF4::RE::ID::TESObjectREFR::GetGoldAmount
#define k_TESObjectREFR_GetMarkerUser				0x0055E720	// 055E710	// 055BCE0	// 055B8F0	// 055B7B0	// 050A790	//			// 047BD90	//	TESObjectREFR::GetMarkerUser also ADF4::ID2202566
#define k_TESObjectREFR_WornHasKeyword				0x00507620	// 0507610	// 0504BE0	// 05047F0	// 05046B0	// 04B36F0	//			// 0400050	//	TESObjectREFR::WornHasKeyword also ADF4::ID2200995
#define k_TESObjectREFR_GetScale					0x004FF530	// 04FF520	// 04FCAF0	// 04FC700	// 04FC5C0	// 04AB5F0	//			// 03F8540	//	TESObjectREFR::GetScale also ADF4::ID2200892
#define k_TESObjectREFR_SetScale					0x004FF5A0	// 04FF590	// 04FCB60	// 04FC770	// 04FC630	// 04AB660	//			// 03F85B0	//	TESObjectREFR::SetScale also ADF4::ID2200893 ADF4::RE::ID::TESObjectREFR::SetScale

#define k_Actor_HasPerkInRank						0x00C92030	// 0C91EA0	// 0C8D910	// 0C8D460	// 0C8CEA0	// 0C0C3F0	// 0C0C060	// 0DA6600	// 0DA64E0	// 0DA64E0	//	Actor::HasPerkInRank also ADF4::ID2230125 ADF4::RE::ID::Actor::GetPerkRank
#define k_Actor_SetPosition							0x00C607C0	// 0C60630	// 0C5C0A0	// 0C5BBF0	// 0C5B630	// 0BDAB80	//			// 0D77690	// Actor::Func00CA also Actor::SetPosition
#define k_Actor_PutCreatedPackage					0x00C81870	// 0C816E0	// 0C7D150	// 0C7CCA0	// 0C7C6E0	// 0BFBC30	//			// 0D96C70	// Actor::Func00FA also Actor::PutCreatedPackage
#define k_Actor_DisableCollision					0x00C5CA70	// 0C5C8E0	// 0C58350	// 0C57EA0	// 0C578E0	// 0BD6E30	//			// 0D73AB0	//	Actor::DisableCollision in DisableCollisionHandler::DoDisable = Func0001 also ADF4::ID2229653
#define k_Actor_EnableCollision						0x00C5C9E0	// 0C5C850	// 0C582C0	// 0C57E10	// 0C57850	// 0BD6DA0	//			// 0D73A20	//	Actor::EnableCollision in EnableCollisionHandler::DoEnable = Func0001 also ADF4::ID2229652
#define k_Actor_StopInteractingQuick				0x00CDBC10	// 0CDBA80	// 0CD74F0	// 0CD7040	// 0CD6A80	// 0C55FD0	//			// 0E0EAA0	// It is probably a TESObjectREFR member, rather than an Actor member. TESObjectREFR::StopInteractingQuick also ADF4::ID2231227 ADF4::RE::ID::Actor::StopInteractingQuick
#define k_Actor_GetSex								0x00C5D5A0	// 0C5D410	// 0C58E80	// 0C589D0	// 0C58410	// 0BD7960	//			// 0D74610	// Actor::GetSex also ADF4::ID2229674 ADF4::RE::ID::Actor::GetSex
#define k_Actor_GetLevel							0x00C63050	// 0C62EC0	// 0C5E930	// 0C5E480	// 0C5DEC0	// 0BDD41B	//			// 0D79E90	// Actor::GetLevel also ADF4::ID2229734 ADF4::RE::ID::Actor::GetLevel
#define k_Actor_IsDead								0x00C73A60	// 0C738D0	// 0C6F340	// 0C6EE90	// 0C6E8D0	// 0BEDE20	//			// 0D89D40	// Actor::Func00C0 = Actor::IsDead also ADF4::ID2229903
#define k_Actor_EvaluatePackage						0x00C69240	// 0C690B0	// 0C64B20	// 0C64670	// 0C640B0	// 0BE3600	//			// 0D7F980	// Actor::EvaluatePackage also ADF4::ID2229805

#define k_PROCESSTYPE_UpdateEquipment				0x00D26EF0	// 0D26D60	// 0D227D0	// 0D22320	// 0D21D60	// 0CA12C0	// 0CA0F30	// 0E60860	// MiddleProcess::UpdateEquipment or Actor::AIProcess::DoUpdate3dModel also ADF4::ID2232144
	// action should be 0x35 (DefaultObject kActionIdle), checkConditions should be false, targetObj should be null
#define k_PROCESSTYPE_PlayIdle						0x00CFAD40	// 0CFABB0	// 0CF6620	// 0CF6170	// 0CF5BB0	// 0C75110	// 0C74D80	// 0E35510	// Actor::AIProcess::PlayIdle also ADF4::ID2231704 ADF4::RE::ID::AIProcess::SetupSpecialIdle
#define k_PROCESSTYPE_StopCurrentIdle				0x00CFB0E0	// 0CFAF50	// 0CF69C0	// 0CF6510	// 0CF5F50	// 0C754B0	//			// 0E358C0	// Actor::AIProcess::StopCurrentIdle also ADF4::ID2231705 ADF4::RE::ID::AIProcess::StopCurrentIdle (is next after PlayIdle)
#define k_PROCESSTYPE_ClearCurrentDataforProcess	0x00D394F0	// 0D39360	// 0D34DD0	// 0D34920	// 0D34360	// 0CB38C0	//			// 0E77520	// Actor::AIProcess::ClearCurrentDataforProcess also ADF4::ID2232435
#define k_PROCESSTYPE_ClearCurrentPackage			0x00CF1340	// 0CF11B0	// 0CECC20	// 0CEC770	// 0CEC1B0	// 0C6B710	//			// 0E2A670	// Actor::AIProcess::ClearCurrentPackage also ADF4::ID2231582
#define k_PROCESSTYPE_GetFadeAlpha					0x00D0B000	// 0D0AE70	// 0D068E0	// 0D06430	// 0D05E70	// 00C853D0	//			// 0E450F0	// Actor::AIProcess::GetFadeAlpha also ADF4__ID2231942
#define k_PROCESSTYPE_GetFadeState					0x00D0A9F0	// 0D0A860	// 0D062D0	// 0D05E20	// 0D05860	// 0C84DC0	//			// 0E44AD0	// Actor::AIProcess::GetFadeState also ADF4::ID2231935
#define k_PROCESSTYPE_ForceFadeToNormal				0x00D0B080	// 0D0AEF0	// 0D06960	// 0D064B0	// 0D05EF0	// 0C85450	//			// 0E45170	// Actor::AIProcess::ForceFadeToNormal also ADF4__ID2231944	// Second func after GetFadeAlpha		// works on player and NPCs , actor cannot be NULL
#define k_PROCESSTYPE_SetIgnoringCombat				0x00CF0B10	// 0CF0986	// 0CEC3F0	// 0CEBF40	// 0CEB980	// 0C6AEE0	//			// 0E29F80	// Actor::AIProcess::SetIgnoringCombat also ADF4::ID2231554
#define k_PROCESSTYPE_GetIgnoringCombat				0x00CF0B00	// 0CF0970	// 0CEC3E0	// 0CEBF30	// 0CEB970	// 0C6AED0	//			// 0E29F70	// Actor::AIProcess::GetIgnoringCombat also ADF4::ID2231553

#define k_TESObjectCELL_ST_GetDataXandY				0x004C6AF0	//	04C6AE0	// 04C40B0	// 04C3CC0	// 04C3B80	// 0472C70	//			// 03B1080	// TESObjectCELL::GetDataXandY also ADF4::ID2200215
#define k_TESObjectCELL_ST_GetDataX					0x004C6AB0	// 04C6AA0	// 04C4070	// 04C3C80	// 04C3B40	// 0472C30	//			// 03B1040	// TESObjectCELL::GetDataX also ADF4::ID2200213 ADF4::RE::ID::TESObjectCELL::GetDataX
#define k_TESObjectCELL_ST_GetDataY					0x004C6AD0	// 04C6AC0	// 04C4090	// 04C3CA0	// 04C3B60	// 0472C50	//			// 03B1060	// TESObjectCELL::GetDataY also ADF4::ID2200214 ADF4::RE::ID::TESObjectCELL::GetDataY
#define k_TESObjectCELL_ST_GetWorldSpace			0x00497BF0	// 0497BE0	// 04951B0	// 0494DC0	// 0494C80	// 0443DC0	//			// 0008880	// TESObjectCELL::GetWorldSpace also ADF4::ID2199522

#define k_PlayerCharacter_ST_UpdatePlayer3d			0x00D5C540	// 0D5C3B0	// 0D57E20	// 0D57970	// 0D573B0	//	0CD68B0	//			// 0EA0630	// PlayerCharacter::UpdatePlayer3d also ADF4::ID2232941

#define k_TESActorBaseData_ST_ModifyFlag			0x0030A3D0	// 030A3C0	// 0307990	// 03075A0	// 0307460	// 02B5F10	//			// 0149720	// TESActorBaseData::ModifyFlag also ADF4::ID2192890

#define k_SettingT_INISettingCollection_ST_Changed	0x0179FA40	// 179F920	// 179B1E0	// 179A860	// 179A190	// 1684DB0	// 16849E0	// 1C91E10	// 1C91CF0	// 1C91CF0 // SettingT_INISettingCollection::Changed also ADF4::ID2274827

#define k_Cmd_Default_Parse							0x005B6160	// 05B6150	// 05B3720	// 05B3330	// 05B31F0	// 05621D0	// 04E7E00	//			// Cmd_GetWantBlocking_Parse also Cmd_Default_Parse also ADF4::ID2204343 ADF4::RE::ID::SCRIPT_FUNCTION::CompileFunction
#define k_ExtractArgs								0x005B32E0	// 05B32D0	// 05B08A0	// 05B04B0	// 05B0370	// 055F350	// 04E37D0	//			// ExtractArgs also ADF4::RE::ID::RTDynamicCast ADF4::ID4818455

#define k_GetDisplayModeList						0x01824470	// 1824350	                                                                        // Buffout4::CreateD3DAndSwapChainFix::GetDisplayModeList also ADF4::ID4492363	(Start from WinMain + 12 )

#elif (CURRENT_RELEASE_RUNTIME == RUNTIME_VERSION_1_11_240)	// currently v61

#define k_hackArrayAdd 							0x02148D2D	// 21412FD	// 214121D + 0xE0	// 213C72D	// 213BADD	// 213B3CD	// 1FEBB3D	// 1FEB41D	// 2775A34	// 2775914	// 279A9E4
#define k_hackArrayInsert						0x02149395	// 2141965	// 2141885 + 0xE0	// 213CD95	// 213C145	// 213BA35	// 1FEC1A5	// 1FEBA85	// 2775FD9	// 2775EB9	// 279AF89

#define k_FXCamIsActive							0x031F0AD8	// 31E5A58	// 31E59D8	// 31DA8D8	// 31D56D8	// 31D4758	// 2F6E578	//	2F6D578	// 59DA130	// 59DA100	// 5A13180

#define k_saveCurrentPlayerCameraState			0x02EE86C0	// 2EDD6C0	// 2EDD6C0	// 2ED26C0	// 2ECD6C0	// 2ECC6C0	// 2C773E0	// 2C763E0	// 36F1283	// 3729283
#define k_CheckIfCameraStateIsSelected			0x0102B5C0	// 102B230	// 102B090	// 1026B00	// 1026650	// 1026090	// 0FA52B0	// 0FA5640	// 0FA52B0	// 124E7F0	// 124E6D0	// 124E6D0
#define k_ToggleFlyCam							0x01027920	// 1027590	// 10273F0	// 1022E60	// 10229B0	// 10223F0	// 0FA19A0	// 0FA1610	// 124AE60	// 124AD40	// 124AD40	// PlayerCamera::ToggleFlyCam

#define k_ControlPlayerCamera					0x0166F750	// 166F430	// 166F330	// 166AE10	// 166A490	// 1669DC0	// 15551C0	// 1554DF0	// 1B285A0	// 1B28480	// 1B28480	// InputManager::ControlPlayerCamera
#define k_ControlPlayer							0x0166F820	// 166F500	// 166F400	// 166AEE0	// 166A560	// 1669E90	// 1555290	// 1554EC0	// 1B28690	// 1B28570	// 1B28570	// InputManager::ControlPlayer

#define k_fFreeCameraTranslationSpeed_Camera	0x02F38F08	// 2F2DF18	// 2F2DEC8	// 2F22EB8	// 2F1DEB8	// 2F1CEB8	// 2CC7940	// 2CC6918	// 38048E8	// 383C8E8	// aFfreecameratra
#define k_fFreeCameraRotationSpeed_Camera		0x02F38F20	// 2F2DF30	// 2F2DEE0	// 2F22ED0	// 2F1DED0	// 2F1CED0	// 2CC7958	// 2CC6930	// 3804900	// 383C900

#define k_playerCamera_ST						0x030E6E58	// 30DBDD8	// 30DBD58	// 30D0C58	// 30CBA58	// 30CAAD8	// 2E649D8	// 2E639D8	// 58CEB28 
#define k_inputMgr_ST							0x031F0B88	// 31E5B08	// 31E5A88	// 31DA988	// 31D5788	// 31D4808	// 2F6E628	// 2F6D628	// 59DA210 
#define k_Main_ST								0x032DD130	// 32D20A0	// 32D2020	// 32C21B0	// 32BCFB0	// 32BC010	// 303ABA8	// 3039BA8	// 5ADD2C8 

#define k_AddScriptAddedForm					0x005894E0	// 05891C0	// 05891B0	// 0586780	// 0586390	// 0586250	// 0535230	// 0535200	// 04B0AC0	// AddScriptAddedForm	RE::ID::BGSListForm::AddScriptAddedForm

// k_EventSource :
//	BSTGlobalEvent::EventSource_ViewCasterUpdateEvent_::Func0000+22
//	g_BSTGlobalEvent::EventSource_ViewCasterUpdateEvent_ or __ICI__BSTValueEventSource_ViewCasterUpdateEvent___Func0000_to0053_to0051_to0160+24
#define	k_EventSource							0x03273868	// 32687D8	// 3268758	// 325D658	// 3258458	// 32574D8	// 2FF12F0	// 2FF02F0	// 5A5F758	// 5A5F728	// 5A987A8	// Also from ADF4::ID2219564

#define k_collisionState						0x030E8944	// 30DD8C4	// 30DD844	// 30D2744	// 30CD544	// 30CC5C4	// 2E66414	// 2E65414	// 58D08B0	// 5909930	// Cmd_ToggleCollision_Execute + 0x055

// ??_7ExtraTextDisplayData@@6B@
#define k_ExtraTextDisplayDataVtbl				0x02470760	// 2468700	// 2468700	// 24636F0	// 24616F0	// 24606F0	// 227CD70	// 227CD70	// 2C52788	// 2C52778	// 2C7C7C8 // ??_7ExtraTextDisplayData@@6B@

#define k_uFGBlinkLeftEyeMorphIndex				0x02F25D90	// 2F1AD90	// 2F1AD40	// 2F0FD40	// 2F0AD40	// 2F09D40	// 2CB4B00	//			// 037333F8
#define k_uFGBlinkRightEyeMorphIndex			0x02F25DA8	// 2F1ADA8	// 2F1AD58	// 2F0FD58	// 2F0AD58	// 2F09D58	// 2CB4B18	//			// 03733410
#define k_fFGBlinkClosedValue					0x02F1AD78	// 2F1AD78	// 2F1AD28	// 2F0FD28	// 2F0AD28	// 2F09D28	// 2CB4AE8	//			// 037333E0

// if I ever get back to this :)
#define k_CallFunction							0x00000000	//			//			//			//			// 0533C00

// And those are class member functions offsets for SharedTools.h

#define k_TESObjectREFR_SetAngleOnReference			0x00513AC0	// 05137A0	// 0513790	// 0510D60	// 0510970	// 0510830	// 04BF810	//			// 040BD70	//	TESObjectREFR::SetAngleOnReference
#define k_TESObjectREFR_SetNoCollision				0x004F4300	// 04F3FE0	// 04F3FD0	// 04F15A0	// 04F11B0	// 04F1070	// 04A00A0	//			// 03EDEE0	//	TESObjectREFR::SetNoCollision also ADF4::ID2200734
#define k_TESObjectREFR_GetRefPersists				0x004FBBF0	// 04FB8D0	// 04FB8C0	// 04F8E90	// 04F8AA0	// 04F8960	// 04A7990	//			// 03F4DA0	//	TESObjectREFR::GetRefPersists also ADF4::ID2200812
#define k_TESObjectREFR_SetRefPersists				0x004FBC50	// 04FB930	// 04FB920	// 04F8EF0	// 04F8B00	// 04F89C0	// 04A79F0	//			// 03F4E00	//	TESObjectREFR::SetRefPersists also ADF4::ID2200816
#define k_TESObjectREFR_GetDistanceFromPoint		0x005175A0	// 0517280	// 0517270	// 0514840	// 0514450	// 0514310	// 04C32F0	//			// 040F560	//	TESObjectREFR::GetDistanceFromPoint also ADF4::ID2201172
#define k_TESObjectREFR_GetDistanceFromReference	0x00517660	// 0517340	// 0517330	// 0514900	// 0514510	// 05143D0	// 04C33B0	//			// 040F620	//	TESObjectREFR::GetDistanceFromReference also ADF4::ID2201174
#define k_TESObjectREFR_GetGoldAmount				0x00501D40	// 0501A20	// 0501A10	// 04FEFE0	// 04FEBF0	// 04FEAB0	// 04ADAF0	//			// 03FA8D0	//	TESObjectREFR::GetGoldAmount also ADF4::ID2200923 ADF4::RE::ID::TESObjectREFR::GetGoldAmount
#define k_TESObjectREFR_GetMarkerUser				0x0055EA40	// 055E720	// 055E710	// 055BCE0	// 055B8F0	// 055B7B0	// 050A790	//			// 047BD90	//	TESObjectREFR::GetMarkerUser also ADF4::ID2202566
#define k_TESObjectREFR_WornHasKeyword				0x00507940	// 0507620	// 0507610	// 0504BE0	// 05047F0	// 05046B0	// 04B36F0	//			// 0400050	//	TESObjectREFR::WornHasKeyword also ADF4::ID2200995
#define k_TESObjectREFR_GetScale					0x004FF850	// 04FF530	// 04FF520	// 04FCAF0	// 04FC700	// 04FC5C0	// 04AB5F0	//			// 03F8540	//	TESObjectREFR::GetScale also ADF4::ID2200892
#define k_TESObjectREFR_SetScale					0x004FF8C0	// 04FF5A0	// 04FF590	// 04FCB60	// 04FC770	// 04FC630	// 04AB660	//			// 03F85B0	//	TESObjectREFR::SetScale also ADF4::ID2200893 ADF4::RE::ID::TESObjectREFR::SetScale

#define k_Actor_HasPerkInRank						0x00C923C0	// 0C92030	// 0C91EA0	// 0C8D910	// 0C8D460	// 0C8CEA0	// 0C0C3F0	// 0C0C060	// 0DA6600	// 0DA64E0	// 0DA64E0	//	Actor::HasPerkInRank also ADF4::ID2230125 ADF4::RE::ID::Actor::GetPerkRank
#define k_Actor_SetPosition							0x00C60B50	// 0C607C0	// 0C60630	// 0C5C0A0	// 0C5BBF0	// 0C5B630	// 0BDAB80	//			// 0D77690	// Actor::Func00CA also Actor::SetPosition
#define k_Actor_PutCreatedPackage					0x00C81C00	// 0C81870	// 0C816E0	// 0C7D150	// 0C7CCA0	// 0C7C6E0	// 0BFBC30	//			// 0D96C70	// Actor::Func00FA also Actor::PutCreatedPackage
#define k_Actor_DisableCollision					0x00C5CE00	// 0C5CA70	// 0C5C8E0	// 0C58350	// 0C57EA0	// 0C578E0	// 0BD6E30	//			// 0D73AB0	//	Actor::DisableCollision in DisableCollisionHandler::DoDisable = Func0001 also ADF4::ID2229653
#define k_Actor_EnableCollision						0x00C5CD70	// 0C5C9E0	// 0C5C850	// 0C582C0	// 0C57E10	// 0C57850	// 0BD6DA0	//			// 0D73A20	//	Actor::EnableCollision in EnableCollisionHandler::DoEnable = Func0001 also ADF4::ID2229652
#define k_Actor_StopInteractingQuick				0x00CDBC10	// 0CDBC10	// 0CDBA80	// 0CD74F0	// 0CD7040	// 0CD6A80	// 0C55FD0	//			// 0E0EAA0	// It is probably a TESObjectREFR member, rather than an Actor member. TESObjectREFR::StopInteractingQuick also ADF4::ID2231227 ADF4::RE::ID::Actor::StopInteractingQuick
#define k_Actor_GetSex								0x00C5D930	// 0C5D5A0	// 0C5D410	// 0C58E80	// 0C589D0	// 0C58410	// 0BD7960	//			// 0D74610	// Actor::GetSex also ADF4::ID2229674 ADF4::RE::ID::Actor::GetSex
#define k_Actor_GetLevel							0x00C633E0	// 0C63050	// 0C62EC0	// 0C5E930	// 0C5E480	// 0C5DEC0	// 0BDD41B	//			// 0D79E90	// Actor::GetLevel also ADF4::ID2229734 ADF4::RE::ID::Actor::GetLevel
#define k_Actor_IsDead								0x00C73DF0	// 0C73A60	// 0C738D0	// 0C6F340	// 0C6EE90	// 0C6E8D0	// 0BEDE20	//			// 0D89D40	// Actor::Func00C0 = Actor::IsDead also ADF4::ID2229903
#define k_Actor_EvaluatePackage						0x00C695D0	// 0C69240	// 0C690B0	// 0C64B20	// 0C64670	// 0C640B0	// 0BE3600	//			// 0D7F980	// Actor::EvaluatePackage also ADF4::ID2229805

#define k_PROCESSTYPE_UpdateEquipment				0x00D27280	// 0D26EF0	// 0D26D60	// 0D227D0	// 0D22320	// 0D21D60	// 0CA12C0	// 0CA0F30	// 0E60860	// MiddleProcess::UpdateEquipment or Actor::AIProcess::DoUpdate3dModel also ADF4::ID2232144
	// action should be 0x35 (DefaultObject kActionIdle), checkConditions should be false, targetObj should be null
#define k_PROCESSTYPE_PlayIdle						0x00CFB0D0	// 0CFAD40	// 0CFABB0	// 0CF6620	// 0CF6170	// 0CF5BB0	// 0C75110	// 0C74D80	// 0E35510	// Actor::AIProcess::PlayIdle also ADF4::ID2231704 ADF4::RE::ID::AIProcess::SetupSpecialIdle
#define k_PROCESSTYPE_StopCurrentIdle				0x00CFB470	// 0CFB0E0	// 0CFAF50	// 0CF69C0	// 0CF6510	// 0CF5F50	// 0C754B0	//			// 0E358C0	// Actor::AIProcess::StopCurrentIdle also ADF4::ID2231705 ADF4::RE::ID::AIProcess::StopCurrentIdle (is next after PlayIdle)
#define k_PROCESSTYPE_ClearCurrentDataforProcess	0x00D39880	// 0D394F0	// 0D39360	// 0D34DD0	// 0D34920	// 0D34360	// 0CB38C0	//			// 0E77520	// Actor::AIProcess::ClearCurrentDataforProcess also ADF4::ID2232435
#define k_PROCESSTYPE_ClearCurrentPackage			0x00CF16D0	// 0CF1340	// 0CF11B0	// 0CECC20	// 0CEC770	// 0CEC1B0	// 0C6B710	//			// 0E2A670	// Actor::AIProcess::ClearCurrentPackage also ADF4::ID2231582
#define k_PROCESSTYPE_GetFadeAlpha					0x00D0B390	// 0D0B000	// 0D0AE70	// 0D068E0	// 0D06430	// 0D05E70	// 00C853D0	//			// 0E450F0	// Actor::AIProcess::GetFadeAlpha also ADF4__ID2231942
#define k_PROCESSTYPE_GetFadeState					0x00D0AD80	// 0D0A9F0	// 0D0A860	// 0D062D0	// 0D05E20	// 0D05860	// 0C84DC0	//			// 0E44AD0	// Actor::AIProcess::GetFadeState also ADF4::ID2231935
#define k_PROCESSTYPE_ForceFadeToNormal				0x00D0B410	// 0D0B080	// 0D0AEF0	// 0D06960	// 0D064B0	// 0D05EF0	// 0C85450	//			// 0E45170	// Actor::AIProcess::ForceFadeToNormal also ADF4__ID2231944	// Second func after GetFadeAlpha		// works on player and NPCs , actor cannot be NULL
#define k_PROCESSTYPE_SetIgnoringCombat				0x00CF0EA0	// 0CF0B10	// 0CF0986	// 0CEC3F0	// 0CEBF40	// 0CEB980	// 0C6AEE0	//			// 0E29F80	// Actor::AIProcess::SetIgnoringCombat also ADF4::ID2231554
#define k_PROCESSTYPE_GetIgnoringCombat				0x00CF0E90	// 0CF0B00	// 0CF0970	// 0CEC3E0	// 0CEBF30	// 0CEB970	// 0C6AED0	//			// 0E29F70	// Actor::AIProcess::GetIgnoringCombat also ADF4::ID2231553

#define k_TESObjectCELL_ST_GetDataXandY				0x004C6E10	// 04C6AF0	//	04C6AE0	// 04C40B0	// 04C3CC0	// 04C3B80	// 0472C70	//			// 03B1080	// TESObjectCELL::GetDataXandY also ADF4::ID2200215
#define k_TESObjectCELL_ST_GetDataX					0x004C6DD0	// 04C6AB0	// 04C6AA0	// 04C4070	// 04C3C80	// 04C3B40	// 0472C30	//			// 03B1040	// TESObjectCELL::GetDataX also ADF4::ID2200213 ADF4::RE::ID::TESObjectCELL::GetDataX
#define k_TESObjectCELL_ST_GetDataY					0x004C6DF0	// 04C6AD0	// 04C6AC0	// 04C4090	// 04C3CA0	// 04C3B60	// 0472C50	//			// 03B1060	// TESObjectCELL::GetDataY also ADF4::ID2200214 ADF4::RE::ID::TESObjectCELL::GetDataY
#define k_TESObjectCELL_ST_GetWorldSpace			0x00497F10	// 0497BF0	// 0497BE0	// 04951B0	// 0494DC0	// 0494C80	// 0443DC0	//			// 0008880	// TESObjectCELL::GetWorldSpace also ADF4::ID2199522

#define k_PlayerCharacter_ST_UpdatePlayer3d			0x00D5C8D0	// 0D5C540	// 0D5C3B0	// 0D57E20	// 0D57970	// 0D573B0	//	0CD68B0	//			// 0EA0630	// PlayerCharacter::UpdatePlayer3d also ADF4::ID2232941

#define k_TESActorBaseData_ST_ModifyFlag			0x0030A6F0	// 030A3D0	// 030A3C0	// 0307990	// 03075A0	// 0307460	// 02B5F10	//			// 0149720	// TESActorBaseData::ModifyFlag also ADF4::ID2192890

#define k_SettingT_INISettingCollection_ST_Changed	0x0179FE40	// 179FA40	// 179F920	// 179B1E0	// 179A860	// 179A190	// 1684DB0	// 16849E0	// 1C91E10	// 1C91CF0	// 1C91CF0 // SettingT_INISettingCollection::Changed also ADF4::ID2274827

#define k_Cmd_Default_Parse							0x005B6480	// 05B6160	// 05B6150	// 05B3720	// 05B3330	// 05B31F0	// 05621D0	// 04E7E00	//			// Cmd_GetWantBlocking_Parse also Cmd_Default_Parse also ADF4::ID2204343 ADF4::RE::ID::SCRIPT_FUNCTION::CompileFunction
#define k_ExtractArgs								0x005B3600	// 05B32E0	// 05B32D0	// 05B08A0	// 05B04B0	// 05B0370	// 055F350	// 04E37D0	//			// ExtractArgs also ADF4::RE::ID::RTDynamicCast ADF4::ID4818455

#define k_GetDisplayModeList						0x01824930	// 1824470	// 1824350	                                                                        // Buffout4::CreateD3DAndSwapChainFix::GetDisplayModeList also ADF4::ID4492363	(Start from WinMain + 12 )

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

