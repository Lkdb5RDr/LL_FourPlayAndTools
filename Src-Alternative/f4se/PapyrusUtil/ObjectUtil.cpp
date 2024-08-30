#include "ObjectUtil.h"

#include "f4se/PluginAPI.h"
#include "f4se/GameAPI.h"
#include "f4se/GameData.h"
#include "f4se/GameRTTI.h"
#include "f4se_common/SafeWrite.h"
#include "f4se/PapyrusNativeFunctions.h"
#include "f4se_common/BranchTrampoline.h"
#include "xbyak/xbyak.h"

#include "SafeRead.h"
#include "Data.h"
#include "Forms.h"

#include <boost/algorithm/string.hpp>

class TESIdleForm : public TESForm
{
public:
	enum { kTypeID = kFormType_IDLE };

	void				* unk0020;		// 20 - something, was linked list
	UInt8				loopMin;		// 28 - DATA for 6 bytes
	UInt8				loopMax;		// 29
	UInt8				flags;			// 2A
	UInt8				group;			// 2B
	UInt16				replayDelay;	// 2C
	UInt8				pad2E[2];		// 2E
	UInt64				unk30;			// 30 - NiFormArray in Skyrim, here a pointer ?
	TESIdleForm			* parent;		// 38 - ANAM
	TESIdleForm			* previous;		// 40
	BSFixedString		behaviorGraph;	// 48 - DNAM
	BSFixedString		animationEvent;	// 50 - ENAM
	BSFixedString		fileName;		// 58 - GNAM
	BSString			editorID;		// 60 - EDID, type assumed because of Skyrim
	UInt32				unk0068;		// 68
	UInt32				unk006C;		// 6C - most likely filler
};

namespace ObjectUtil {
	typedef Data::Values<TESForm*, UInt32> Idles;

	Idles* s_Idles = NULL;
	Idles* GetSingleton(){
		if (s_Idles == NULL)
			s_Idles = new Idles();
		return s_Idles;
	}


	/*
	*
	*  Idle override patching
	*
	*/

	const char* AnimEvent = NULL;
	void DecideAnimReplace(int a1){
		if (!s_Idles || a1 == 0)
			return;
		AnimEvent = NULL;
		TESForm* obj = (TESForm*)RelocAddr<TESForm>(a1+8).GetUIntPtr();
		if (obj == nullptr)
			return;
		UInt64 key = Forms::GetFormKey((TESForm*)obj);
		if (key == 0)
			return;

		// Look for a replacer
		std::string AnimEventName((const char*)RelocAddr<const char *>(a1 + 0x28));	// at least 28
		TESForm* form = GetSingleton()->GetValue(key, AnimEventName, NULL);
		if (!form)
			form = GetSingleton()->GetValue(0, AnimEventName, NULL);
		if (!form)
			return;

		// Get the Idle object to use
		TESIdleForm* idle = DYNAMIC_CAST(form, TESForm, TESIdleForm);
		if (!idle)
			return;

		// Override
		//_MESSAGE(IdleRef->animationEvent.c_str());
		SafeWrite32(a1 + 0x40, (UInt32)idle);
		AnimEvent = idle->animationEvent.c_str();
		SafeWrite32(a1 + 0x28, (UInt32)AnimEvent);
		/*
		Normal a1								Modified a1
		+0		vtable
		+8		actor
		+10		0
		+18		BGSAction (e.g. ActionIdle)				?BGSAction(Idle)
		+20		2
		+24
		+28		AnimationEventName*						AnimationEventName *
		+30		AnimationParams?
		+34		0
		+38		0
		+3C
		+40		TESIdleForm*
		+48		0
		+4C		1
		+50		4 bytes
		+54
		+58		flags 4 bytes
		// After that no clue...
		+... 	0
		*/
	}

#define hookAnimFunc					0x00E20E90	// second call in ActorMediator::Func0001
#define hookAnimStart					hookAnimFunc + 0x048

#define hookAnimNull			hookAnimFunc + 0x065
#define hookAnimNotNull			hookAnimFunc + 0x04E

	RelocAddr<uintptr_t> AnimStart(hookAnimStart);
	RelocAddr<uintptr_t> AnimNull(hookAnimNull);
	RelocAddr<uintptr_t> AnimNotNull(hookAnimNotNull);
	void InitPlugin() {
		struct AnimStart_Code : Xbyak::CodeGenerator {
			AnimStart_Code(void * buf) : Xbyak::CodeGenerator(4096, buf)
			{
				push(eax);
				push(ecx);
				push(edx);
				mov(edx, rbx);
				call(DecideAnimReplace);
				pop(edx);
				pop(ecx);
				pop(eax);
				test(byte[rbx + 0x58], 1);
				je("animJEJump");
				jmp((void*)AnimNull.GetUIntPtr());
				L("animJEJump");
				jmp((void*)AnimNotNull.GetUIntPtr());
				ret();
			}
		};

		void * codeBuf = g_localTrampoline.StartAlloc();
		AnimStart_Code code(codeBuf);
		g_localTrampoline.EndAlloc(code.getCurr());

		g_branchTrampoline.Write5Branch(AnimStart, uintptr_t(code.getCode()));
	}

	/*
	 *
	 *  Papyrus Interface: ObjectUtil.psc
	 *
	 */

	void SetReplaceAnimation(StaticFunctionTag* base, TESObjectREFR* ObjRef, BSFixedString Anim, TESIdleForm* IdleRef) {
		if (!IdleRef || !Anim.c_str())
			return;
		GetSingleton()->SetValue(Forms::GetFormKey(ObjRef), Anim.c_str(), IdleRef);
	}

	bool RemoveReplaceAnimation(StaticFunctionTag* base, TESObjectREFR* ObjRef, BSFixedString Anim) {
		if (!s_Idles || !Anim.c_str())
			return false;
		return GetSingleton()->UnsetValue(Forms::GetFormKey(ObjRef), Anim.c_str());
	}

	UInt32 CountReplaceAnimation(StaticFunctionTag* base, TESObjectREFR* ObjRef) {
		if (!s_Idles)
			return 0;
		int count = 0;
		Idles* Overrides = GetSingleton();
		Overrides->s_dataLock.Enter();

		Idles::Map::iterator itr = Overrides->Data.find(Forms::GetFormKey(ObjRef));
		if (itr != Overrides->Data.end())
			count = itr->second.size();

		Overrides->s_dataLock.Leave();
		return count;
	}

	UInt32 ClearReplaceAnimation(StaticFunctionTag* base, TESObjectREFR* ObjRef) {
		if (!s_Idles)
			return 0;
		int removed = 0;
		Idles* Overrides = GetSingleton();
		Overrides->s_dataLock.Enter();

		Idles::Map::iterator itr = Overrides->Data.find(Forms::GetFormKey(ObjRef));
		if (itr != Overrides->Data.end()){
			removed = itr->second.size();
			Overrides->Data.erase(itr);
		}

		Overrides->s_dataLock.Leave();
		return removed;
	}

	BSFixedString GetKeyReplaceAnimation(StaticFunctionTag* base, TESObjectREFR* ObjRef, UInt32 index) {
		if (!s_Idles)
			return "";
		std::string anim;
		Idles* Overrides = GetSingleton();
		Overrides->s_dataLock.Enter();

		Idles::Map::iterator itr = Overrides->Data.find(Forms::GetFormKey(ObjRef));
		if (itr != Overrides->Data.end() && index < itr->second.size()) {
			Idles::Obj::iterator itr2 = itr->second.begin();
			std::advance(itr2, index);
			anim = itr2->first;
		}

		Overrides->s_dataLock.Leave();
		return BSFixedString(anim.c_str());
	}

	TESIdleForm* GetValueReplaceAnimation(StaticFunctionTag* base, TESObjectREFR* ObjRef, BSFixedString Anim){
		if (!s_Idles || !Anim.c_str() || !*Anim.c_str())
			return NULL;
		TESIdleForm* IdleRef = NULL;
		Idles* Overrides = GetSingleton();
		Overrides->s_dataLock.Enter();

		UInt64 objKey = Forms::GetFormKey(ObjRef);
		if (!Overrides->Data[objKey].empty()) {
			std::string key(Anim.c_str());
			boost::to_lower(key);
			Idles::Obj::iterator itr = Overrides->Data[objKey].find(key);
			if (itr != Overrides->Data[objKey].end()){
				TESForm* FormRef = LookupFormByID(itr->second);
				if (FormRef && FormRef->formType == kFormType_IDLE)
					IdleRef = DYNAMIC_CAST(FormRef, TESForm, TESIdleForm);
			}
		}

		Overrides->s_dataLock.Leave();
		return IdleRef;
	}

	void RegisterFuncs(VirtualMachine* vm) {
#if 0
		vm->RegisterFunction(new NativeFunction3<StaticFunctionTag, void, TESObjectREFR*, BSFixedString, TESIdleForm*>("SetReplaceAnimation", "ObjectUtil", SetReplaceAnimation, vm));
		vm->SetFunctionFlags("ObjectUtil", "SetReplaceAnimation", IFunction::kFunctionFlag_NoWait);

		vm->RegisterFunction(new NativeFunction2<StaticFunctionTag, bool, TESObjectREFR*, BSFixedString>("RemoveReplaceAnimation", "ObjectUtil", RemoveReplaceAnimation, vm));
		vm->SetFunctionFlags("ObjectUtil", "RemoveReplaceAnimation", IFunction::kFunctionFlag_NoWait);

		vm->RegisterFunction(new NativeFunction1<StaticFunctionTag, UInt32, TESObjectREFR*>("CountReplaceAnimation", "ObjectUtil", CountReplaceAnimation, vm));
		vm->SetFunctionFlags("ObjectUtil", "CountReplaceAnimation", IFunction::kFunctionFlag_NoWait);

		vm->RegisterFunction(new NativeFunction1<StaticFunctionTag, UInt32, TESObjectREFR*>("ClearReplaceAnimation", "ObjectUtil", ClearReplaceAnimation, vm));
		vm->SetFunctionFlags("ObjectUtil", "ClearReplaceAnimation", IFunction::kFunctionFlag_NoWait);

		vm->RegisterFunction(new NativeFunction2<StaticFunctionTag, BSFixedString, TESObjectREFR*, UInt32>("GetKeyReplaceAnimation", "ObjectUtil", GetKeyReplaceAnimation, vm));
		vm->SetFunctionFlags("ObjectUtil", "GetKeyReplaceAnimation", IFunction::kFunctionFlag_NoWait);

		vm->RegisterFunction(new NativeFunction2<StaticFunctionTag, TESIdleForm*, TESObjectREFR*, BSFixedString>("GetValueReplaceAnimation", "ObjectUtil", GetValueReplaceAnimation, vm));
		vm->SetFunctionFlags("ObjectUtil", "GetValueReplaceAnimation", IFunction::kFunctionFlag_NoWait);
#endif
	}
}