#include "ActorUtil.h"
#include "f4se_common/Relocation.h"
#include "f4se_common/BranchTrampoline.h"

#include "f4se/PluginAPI.h"
#include "f4se/GameAPI.h"
#include "f4se/GameData.h"
#include "f4se/PapyrusNativeFunctions.h"

#include "f4se/GameRTTI.h"
#include "f4se_common/SafeWrite.h"

#include "SafeRead.h"
#include "Data.h"
#include "Forms.h"

#include "xbyak/xbyak.h"

#define hookPackageStart				0x00D7F6D0 + 0x039	// inside Actor::PickValidPackage
#define BaseExtraList__PickValidPackage	0x00098C90

#define hookEndPackageCodeStart			0x01307A70 + 0x0E0	// FragmentEventHandlerGameScript::_$BSTEventSinkUTESPackageEvent::Func0001
#define hookEndPackageCodeCall			0x01B41DB0	// Ref::SetValue I think...

class TESPackage : public TESForm
{
public:
	enum { kTypeID = kFormType_PACK };	// special-case

	MEMBER_FN_PREFIX(TESPackage);
	DEFINE_MEMBER_FN(IsValidForActor, bool, 0x0071DB80, Actor*);
};

namespace ActorUtil {
	typedef Data::Lists<TESForm*, UInt32> Packages;

	Packages* s_Packages = NULL;
	Packages* GetSingleton(){
		if (s_Packages == NULL)
			s_Packages = new Packages();
		return s_Packages;
	}

	/*
	 *
	 *  Package override handling
	 *
	 */

	void PackageEnded(UInt32 actorID, UInt32 packageID){
		if (!s_Packages || actorID == 0 || packageID == 0)
			return;
		TESForm* ActorRef = LookupFormByID(actorID);
		TESForm* FormRef = LookupFormByID(packageID);
		TESPackage* PackageRef = DYNAMIC_CAST(FormRef, TESForm, TESPackage);
		if (!PackageRef)
			return;
		Packages* Data = GetSingleton();
		UInt64 obj = Forms::GetFormKey(ActorRef);
		if (Data->ListHas(obj, "package", PackageRef)){
			Data->ListRemove(obj, "package", PackageRef, true);
			Data->ListRemove(obj, "flagged", PackageRef, true);
		}
	}

	bool IsValidPackage(TESPackage* package, Actor* actor) { 
		return CALL_MEMBER_FN(package, IsValidForActor)(actor) != 0; 
	}

	UInt32 DecidePackage(Actor* actor, UInt32 packageID){
		if (!s_Packages || actor == 0)
			return packageID;
		Packages* Data = GetSingleton();
		UInt64 obj = Forms::GetFormKey(actor);
		Packages::List* package = Data->GetVector(obj, std::string("package"));
		if (package != NULL){
			for (Packages::List::reverse_iterator itr = package->rbegin(); itr != package->rend(); itr++){
				TESForm* form = (TESForm*)Forms::GameGetForm(*itr);
				if (form != NULL && form->formType == kFormType_PACK){
					_MESSAGE("Checking Package: %x", form->formID);
					TESPackage* package = (TESPackage*)form;
					if (Data->ListHas(obj, "flagged", form) || IsValidPackage(package, actor)) {
						_MESSAGE("Picked package override...");
						packageID = package->formID;
						break;
					}
				}
			}
		}
		return packageID;
	}

	RelocAddr<uintptr_t> PackageStart(hookPackageStart);
	RelocAddr<uintptr_t> PackageStartCall(BaseExtraList__PickValidPackage);
	RelocAddr<uintptr_t> PackageEnd(hookEndPackageCodeStart);
	RelocAddr<uintptr_t> PackageEndCall(hookEndPackageCodeCall);
	RelocAddr<uintptr_t> PackageEndReturn(hookEndPackageCodeStart + 5);

	void InitPlugin()
	{
		struct PackageStart_Code : Xbyak::CodeGenerator {
			PackageStart_Code(void * buf) : Xbyak::CodeGenerator(4096, buf)
			{
/*
.text:006A9E90 hookPackageCodeStart:                   ; DATA XREF: 0F718C7Eo
.text:006A9E90                                         ; StorageUtil.dll:5124519Do
.text:006A9E90 jmp     sub_5124517F                    ; Jump
.text:006A9E95
.text:006A9E95 hookJumpPackage:                        ; CODE XREF: 0F718C75j

StorageUtil.dll:5124517F sub_5124517F proc near
StorageUtil.dll:5124517F call    BaseExtraList__PickValidPackageOffset ; Indirect Call Near Procedure
StorageUtil.dll:51245185 cmp     esi, 0                          ; Compare Two Operands
StorageUtil.dll:51245188 jz      short exit                      ; Jump if Zero (ZF=1)
StorageUtil.dll:5124518A push    eax
StorageUtil.dll:5124518B push    esi
StorageUtil.dll:5124518C call    DecidePackage                    ; Call Procedure
StorageUtil.dll:51245191 add     esp, 8                          ; Add
StorageUtil.dll:51245194
StorageUtil.dll:51245194 exit:                                   ; CODE XREF: sub_5124517F+9j
StorageUtil.dll:51245194 jmp     hookPackageReturn                  ; Indirect Near Jump
StorageUtil.dll:51245194 sub_5124517F endp
*/
			
				call((void*)PackageStartCall.GetUIntPtr());
				cmp(rsi, 0);		// this - Actor
				je("packageOrig");
				mov(rdx, rax);		// package or package form ID
				mov(rcx, rsi);
				call(DecidePackage);
				L("packageOrig");
				ret();
			}
		};

		void * codeBuf = g_localTrampoline.StartAlloc();
		PackageStart_Code code(codeBuf);
		g_localTrampoline.EndAlloc(code.getCurr());

		g_branchTrampoline.Write5Branch(PackageStart, uintptr_t(code.getCode()));

		// save registration handles hook
		{
			struct PackageEnd_Code : Xbyak::CodeGenerator {
				PackageEnd_Code(void * buf) : Xbyak::CodeGenerator(4096, buf)
				{
/*
.text:008CAB54 hookEndPackageCodeStart:                ; DATA XREF: 0F718CBAo
.text:008CAB54 jmp     sub_512451B0                    ; Jump
.text:008CAB59
.text:008CAB59 hookEndPackageCodeReturn:               ; CODE XREF: 0F718CB1j

StorageUtil.dll:512451B0 sub_512451B0 proc near
StorageUtil.dll:512451B0 push    eax
StorageUtil.dll:512451B1 push    ecx
StorageUtil.dll:512451B2 push    edx
StorageUtil.dll:512451B3 mov     eax, [edi]
StorageUtil.dll:512451B5 mov     dword_512E7838, eax
StorageUtil.dll:512451BA mov     eax, [edi+4]
StorageUtil.dll:512451BD mov     dword_512E783C, eax
StorageUtil.dll:512451C2 call    sub_51245070                    ; Call Procedure
StorageUtil.dll:512451C7 pop     edx
StorageUtil.dll:512451C8 pop     ecx
StorageUtil.dll:512451C9 pop     eax
StorageUtil.dll:512451CA call    hookEndPackageCodeCallOffset    ; Indirect Call Near Procedure
StorageUtil.dll:512451D0 jmp     hookEndPackageCodeReturnOffset  ; Indirect Near Jump
StorageUtil.dll:512451D0 sub_512451B0 endp
*/
					push(rax);
					push(rcx);
					push(rdx);
					push(r8);
					push(r9);
					mov(rcx, ptr[rsi]);
					mov(rdx, ptr[rsi + 4]);
					call(PackageEnded);
					pop(r9);
					pop(r8);
					pop(edx);
					pop(ecx);
					pop(eax);
					call((void*)PackageEndCall.GetUIntPtr());
					jmp((void*)PackageEndReturn.GetUIntPtr());

				}
			};

			void * codeBuf = g_localTrampoline.StartAlloc();
			PackageEnd_Code code(codeBuf);
			g_localTrampoline.EndAlloc(code.getCurr());

			g_branchTrampoline.Write5Branch(PackageEnd, uintptr_t(code.getCode()));

		}

	}

	/*
	 *
	 *  Papyrus Interface: ActorUtil.psc
	 *
	 */

	void AddPackage(StaticFunctionTag* base, Actor* ActorRef, TESPackage* PackageRef, UInt32 priority, UInt32 flags){
		if (!ActorRef || !PackageRef)
			return;
		UInt64 obj = Forms::GetFormKey(ActorRef);
		Packages* Overrides = GetSingleton();
		Overrides->ListAdd(obj, "package", PackageRef, false);
		if (flags == 1)
			Overrides->ListAdd(obj, "flagged", PackageRef, false);
	}

	bool RemovePackage(StaticFunctionTag* base, Actor* ActorRef, TESPackage* PackageRef){
		if (!ActorRef || !PackageRef)
			return false;
		UInt64 obj = Forms::GetFormKey(ActorRef);
		Packages* Overrides = GetSingleton();
		Overrides->ListRemove(obj, "flagged", PackageRef, true);
		return Overrides->ListRemove(obj, "package", PackageRef, true) != 0;
	}

	UInt32 CountPackages(StaticFunctionTag* base, Actor* ActorRef){
		if (!s_Packages || !ActorRef)
			return 0;
		return GetSingleton()->ListCount(Forms::GetFormKey(ActorRef), "package");
	}

	UInt32 ClearActor(StaticFunctionTag* base, Actor* ActorRef){
		if (!s_Packages || !ActorRef)
			return 0;

		int removed = 0;
		Packages* Overrides = GetSingleton();
		Overrides->s_dataLock.Enter();

		Packages::Map::iterator itr = Overrides->Data.find(Forms::GetFormKey(ActorRef));
		if (itr != Overrides->Data.end()){
			removed = itr->second["packages"].size();
			Overrides->Data.erase(itr);
		}

		Overrides->s_dataLock.Leave();
		return removed;
	}

	UInt32 ClearPackage(StaticFunctionTag* base, TESPackage* PackageRef){
		if (!s_Packages || !PackageRef)
			return 0;
		int removed = 0;
		Packages* Overrides = GetSingleton();
		for (Packages::Map::iterator itr = Overrides->Data.begin(); itr != Overrides->Data.end(); ++itr){
			removed += Overrides->ListRemove(itr->first, "package", PackageRef, true);
			Overrides->ListRemove(itr->first, "flagged", PackageRef, true);
		}
		return removed;
	}

	void RegisterFuncs(VirtualMachine* vm) {
		vm->RegisterFunction(new NativeFunction4<StaticFunctionTag, void, Actor*, TESPackage*, UInt32, UInt32>("AddPackageOverride", "ActorUtil", AddPackage, vm));
		vm->SetFunctionFlags("ActorUtil", "AddPackageOverride", IFunction::kFunctionFlag_NoWait);

		vm->RegisterFunction(new NativeFunction2<StaticFunctionTag, bool, Actor*, TESPackage*>("RemovePackageOverride", "ActorUtil", RemovePackage, vm));
		vm->SetFunctionFlags("ActorUtil", "RemovePackageOverride", IFunction::kFunctionFlag_NoWait);

		vm->RegisterFunction(new NativeFunction1<StaticFunctionTag, UInt32, Actor*>("CountPackageOverride", "ActorUtil", CountPackages, vm));
		vm->SetFunctionFlags("ActorUtil", "CountPackageOverride", IFunction::kFunctionFlag_NoWait);

		vm->RegisterFunction(new NativeFunction1<StaticFunctionTag, UInt32, Actor*>("ClearPackageOverride", "ActorUtil", ClearActor, vm));
		vm->SetFunctionFlags("ActorUtil", "ClearPackageOverride", IFunction::kFunctionFlag_NoWait);

		vm->RegisterFunction(new NativeFunction1<StaticFunctionTag, UInt32, TESPackage*>("RemoveAllPackageOverride", "ActorUtil", ClearPackage, vm));
		vm->SetFunctionFlags("ActorUtil", "RemoveAllPackageOverride", IFunction::kFunctionFlag_NoWait);
	}
}