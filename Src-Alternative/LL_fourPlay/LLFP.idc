#include <idc.idc>

static main(void)
{
  // .\LL_FourPlay.h
  // .\main.cpp
  // MakeNameEx( 0x0140000000 + s_hackArrayAdd+2, "", SN_NOCHECK + SN_PUBLIC + SN_NOWARN); // 0x00000000 Line:367
  // MakeNameEx( 0x0140000000 + s_hackArrayInsert+1, "", SN_NOCHECK + SN_PUBLIC + SN_NOWARN); // 0x00000000 Line:368
  // MakeNameEx( 0x0140000000 + k_HookChild+i, "", SN_NOCHECK + SN_PUBLIC + SN_NOWARN); // 0x00000000 Line:382
  // MakeNameEx( 0x0140000000 + 0x004BF810	/* 040BD70 */, "TESObjectREFR_LL::SetAngleOnReference", SN_NOCHECK + SN_PUBLIC + SN_NOWARN); // 0x00000000 Line:453
  // MakeNameEx( 0x0140000000 + 0x004A00A0	/* 03EDEE0 */, "TESObjectREFR_LL::SetNoCollision", SN_NOCHECK + SN_PUBLIC + SN_NOWARN); // 0x00000000 Line:454
  // MakeNameEx( 0x0140000000 + 0x004A7990	/* 03F4DA0 */, "TESObjectREFR_LL::GetRefPersists", SN_NOCHECK + SN_PUBLIC + SN_NOWARN); // 0x00000000 Line:455
  // MakeNameEx( 0x0140000000 + 0x004A79F0	/* 03F4E00 */, "TESObjectREFR_LL::SetRefPersists", SN_NOCHECK + SN_PUBLIC + SN_NOWARN); // 0x00000000 Line:456
  // MakeNameEx( 0x0140000000 + 0x004C32F0	/* 040F560 */, "TESObjectREFR_LL::GetDistanceFromPoint", SN_NOCHECK + SN_PUBLIC + SN_NOWARN); // 0x00000000 Line:457
  // MakeNameEx( 0x0140000000 + 0x004C33B0	/* 040F620 */, "TESObjectREFR_LL::GetDistanceFromReference", SN_NOCHECK + SN_PUBLIC + SN_NOWARN); // 0x00000000 Line:458
  // MakeNameEx( 0x0140000000 + 0x004ADAF0	/* 03FA8D0 */, "TESObjectREFR_LL::GetGoldAmount", SN_NOCHECK + SN_PUBLIC + SN_NOWARN); // 0x00000000 Line:459
  // MakeNameEx( 0x0140000000 + 0x0050A790	/* 047BD90 */, "TESObjectREFR_LL::GetMarkerUser", SN_NOCHECK + SN_PUBLIC + SN_NOWARN); // 0x00000000 Line:460
  // MakeNameEx( 0x0140000000 + 0x004B36F0	/* 0400050 */, "TESObjectREFR_LL::WornHasKeyword", SN_NOCHECK + SN_PUBLIC + SN_NOWARN); // 0x00000000 Line:461
  // MakeNameEx( 0x0140000000 + 0x00C0C3F0	/*	0C0C060 0DA6600 0DA64E0, "Actor_LL::HasPerkInRank", SN_NOCHECK + SN_PUBLIC + SN_NOWARN); // 0x00000000 Line:470
  // MakeNameEx( 0x0140000000 + 0x00BDAB80	/* 0D77690 */, "Actor_LL::SetPosition", SN_NOCHECK + SN_PUBLIC + SN_NOWARN); // 0x00000000 Line:471
  // MakeNameEx( 0x0140000000 + 0x00BFBC30	/* 0D96C70 */, "Actor_LL::PutCreatedPackage", SN_NOCHECK + SN_PUBLIC + SN_NOWARN); // 0x00000000 Line:472
  // MakeNameEx( 0x0140000000 + 0x00BD6E30	/* 0D73AB0 */, "Actor_LL::DisableCollision", SN_NOCHECK + SN_PUBLIC + SN_NOWARN); // 0x00000000 Line:473
  // MakeNameEx( 0x0140000000 + 0x00BD6DA0	/* 0D73A20 */, "Actor_LL::EnableCollision", SN_NOCHECK + SN_PUBLIC + SN_NOWARN); // 0x00000000 Line:474
  // MakeNameEx( 0x0140000000 + 0x00C55FD0	/* 0E0EAA0 */, "Actor_LL::StopInteractingQuick", SN_NOCHECK + SN_PUBLIC + SN_NOWARN); // 0x00000000 Line:475
  // MakeNameEx( 0x0140000000 + 0x00BD7960	/* 0D74610 */, "Actor_LL::GetSex", SN_NOCHECK + SN_PUBLIC + SN_NOWARN); // 0x00000000 Line:476
  // MakeNameEx( 0x0140000000 + 0x00BDD41B	/* 0D79E90 */, "Actor_LL::GetLevel", SN_NOCHECK + SN_PUBLIC + SN_NOWARN); // 0x00000000 Line:477
  // MakeNameEx( 0x0140000000 + 0x00BEDE20	/* 0D89D40 */, "Actor_LL::IsDead", SN_NOCHECK + SN_PUBLIC + SN_NOWARN); // 0x00000000 Line:478
  // MakeNameEx( 0x0140000000 + 0x00BE3600 /* 0D7F980 */, "Actor_LL::EvaluatePackage", SN_NOCHECK + SN_PUBLIC + SN_NOWARN); // 0x00000000 Line:479
  // MakeNameEx( 0x0140000000 + 0x00CA12C0	/*	0CA0F30 0E60860 */, "AIProcess::UpdateEquipment", SN_NOCHECK + SN_PUBLIC + SN_NOWARN); // 0x00000000 Line:488
  // MakeNameEx( 0x0140000000 + 0x00C75110	/* 0C74D80 0E35510 */, "AIProcess::PlayIdle", SN_NOCHECK + SN_PUBLIC + SN_NOWARN); // 0x00000000 Line:490
  // MakeNameEx( 0x0140000000 + 0x00C754B0	/* 0E358C0 */, "AIProcess::StopCurrentIdle", SN_NOCHECK + SN_PUBLIC + SN_NOWARN); // 0x00000000 Line:491
  // MakeNameEx( 0x0140000000 + 0x00CB38C0 /* 0E77520 */, "AIProcess::ClearCurrentDataforProcess", SN_NOCHECK + SN_PUBLIC + SN_NOWARN); // 0x00000000 Line:492
  // MakeNameEx( 0x0140000000 + 0x00C6B710 /* 0E2A670 */, "AIProcess::ClearCurrentPackage", SN_NOCHECK + SN_PUBLIC + SN_NOWARN); // 0x00000000 Line:493
  // MakeNameEx( 0x0140000000 + 0x00472C70	/* 03B1080 */, "TESObjectCELL_LL::GetDataXandY", SN_NOCHECK + SN_PUBLIC + SN_NOWARN); // 0x00000000 Line:514
  // MakeNameEx( 0x0140000000 + 0x00472C30	/* 03B1040 */, "TESObjectCELL_LL::GetDataX", SN_NOCHECK + SN_PUBLIC + SN_NOWARN); // 0x00000000 Line:515
  // MakeNameEx( 0x0140000000 + 0x00472C50	/* 03B1060 */, "TESObjectCELL_LL::GetDataY", SN_NOCHECK + SN_PUBLIC + SN_NOWARN); // 0x00000000 Line:516
  // MakeNameEx( 0x0140000000 + 0x00443DC0	/* 0008880 */, "TESObjectCELL_LL::GetWorldSpace", SN_NOCHECK + SN_PUBLIC + SN_NOWARN); // 0x00000000 Line:517
  // MakeNameEx( 0x0140000000 + 0x00CD68B0	/* 0EA0630 */, "PlayerCharacter_LL::UpdatePlayer3d", SN_NOCHECK + SN_PUBLIC + SN_NOWARN); // 0x00000000 Line:526
  // MakeNameEx( 0x0140000000 + 0x002B5F10	/* 02B5EE0 */, "TESActorBaseData_LL::ModifyFlag", SN_NOCHECK + SN_PUBLIC + SN_NOWARN); // 0x00000000 Line:590
  // MakeNameEx( 0x0140000000 + k_FXCamIsActive, "g_FXCamIsActive", SN_NOCHECK + SN_PUBLIC + SN_NOWARN); // 0x00000000 Line:607
  // MakeNameEx( 0x0140000000 + k_saveCurrentPlayerCameraState, "g_saveCurrentPlayerCameraState", SN_NOCHECK + SN_PUBLIC + SN_NOWARN); // 0x00000000 Line:608
  // MakeNameEx( 0x0140000000 + k_CheckIfCameraStateIsSelected, "PlayerCamera_LL::CheckIfCameraStateIsSelected", SN_NOCHECK + SN_PUBLIC + SN_NOWARN); // 0x00000000 Line:614
  // MakeNameEx( 0x0140000000 + k_ToggleFlyCam, "PlayerCamera_LL::ToggleFlyCam", SN_NOCHECK + SN_PUBLIC + SN_NOWARN); // 0x00000000 Line:615
  // MakeNameEx( 0x0140000000 + 0x02E649D8	/* 2E639D8	58CEB28 */, "g_playerCamera_LL", SN_NOCHECK + SN_PUBLIC + SN_NOWARN); // 0x00000000 Line:617
  // MakeNameEx( 0x0140000000 + k_ControlPlayerCamera, "InputManager_LL::ControlPlayerCamera", SN_NOCHECK + SN_PUBLIC + SN_NOWARN); // 0x00000000 Line:623
  // MakeNameEx( 0x0140000000 + k_ControlPlayer, "InputManager_LL::ControlPlayer", SN_NOCHECK + SN_PUBLIC + SN_NOWARN); // 0x00000000 Line:624
  // MakeNameEx( 0x0140000000 + 0x02F6E628	/* 2F6D628 59DA210 */, "g_inputMgr_LL", SN_NOCHECK + SN_PUBLIC + SN_NOWARN); // 0x00000000 Line:626
  // MakeNameEx( 0x0140000000 + 0x0303ABA8	/* 3039BA8 5ADD2C8 */, "g_Main_LL", SN_NOCHECK + SN_PUBLIC + SN_NOWARN); // 0x00000000 Line:636
  // MakeNameEx( 0x0140000000 + 0x01684DB0	/* 16849E0, "SettingT_INISettingCollection_LL::Changed", SN_NOCHECK + SN_PUBLIC + SN_NOWARN); // 0x00000000 Line:649
  // MakeNameEx( 0x0140000000 + k_fFreeCameraTranslationSpeed_Camera-8, "g_fFreeCameraTranslationSpeed_Camera", SN_NOCHECK + SN_PUBLIC + SN_NOWARN); // 0x00000000 Line:652
  // MakeNameEx( 0x0140000000 + k_fFreeCameraRotationSpeed_Camera-8, "g_fFreeCameraRotationSpeed_Camera", SN_NOCHECK + SN_PUBLIC + SN_NOWARN); // 0x00000000 Line:653
  // MakeNameEx( 0x0140000000 + 0x00535230	/* 0535200 04B0AC0 */, "AddScriptAddedForm", SN_NOCHECK + SN_PUBLIC + SN_NOWARN); // 0x00000000 Line:1554
  // MakeNameEx( 0x0140000000 + kEventSource, "g_Source", SN_NOCHECK + SN_PUBLIC + SN_NOWARN); // 0x00000000 Line:1713
  // MakeNameEx( 0x0140000000 + k_collisionState, "g_collisionState", SN_NOCHECK + SN_PUBLIC + SN_NOWARN); // 0x00000000 Line:1787
  // MakeNameEx( 0x0140000000 + 0x0227CD70	/* 227CD70	2C52788 2C52778, 2C7C7C8 */, "s_ExtraTextDisplayDataVtbl", SN_NOCHECK + SN_PUBLIC + SN_NOWARN); // 0x00000000 Line:1824
}

