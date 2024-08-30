#include "JsonUtil.h"

#include "f4se/PluginAPI.h"
#include "f4se/GameAPI.h"
#include "f4se/GameTypes.h"
#include "f4se/PapyrusNativeFunctions.h"
#include "f4se/PapyrusArgs.h"

#include "External.h"

namespace JsonUtil {
	using namespace External;

	bool SaveJson(StaticFunctionTag* base, BSFixedString name, bool styled){
		ExternalFile* file = GetFile(name.c_str());
		return file->SaveFile(styled);
	}
	bool LoadJson(StaticFunctionTag* base, BSFixedString name){
		ExternalFile* file = GetFile(name.c_str());
		return file->LoadFile();
	}

	template <typename T>
	T SetValue(StaticFunctionTag* base, BSFixedString name, BSFixedString key, T value) {
		ExternalFile* file = GetFile(name.c_str());
		return file->SetValue<T>(key.c_str(), value);
	}

	template <typename T>
	T GetValue(StaticFunctionTag* base, BSFixedString name, BSFixedString key, T missing) {
		ExternalFile* file = GetFile(name.c_str());
		return file->GetValue<T>(key.c_str(), missing);
	}

	template <typename T>
	bool UnsetValue(StaticFunctionTag* base, BSFixedString name, BSFixedString key) {
		ExternalFile* file = GetFile(name.c_str());
		return file->UnsetValue<T>(key.c_str());
	}

	template <typename T>
	bool HasValue(StaticFunctionTag* base, BSFixedString name, BSFixedString key) {
		ExternalFile* file = GetFile(name.c_str());
		return file->HasValue<T>(key.c_str());
	}

	template <typename T>
	UInt32 ListAdd(StaticFunctionTag* base, BSFixedString name, BSFixedString key, T value, bool allowDuplicate) {
		ExternalFile* file = GetFile(name.c_str());
		return file->ListAdd<T>(key.c_str(), value, allowDuplicate);
	}

	template <typename T>
	T ListGet(StaticFunctionTag* base, BSFixedString name, BSFixedString key, UInt32 index) {
		ExternalFile* file = GetFile(name.c_str());
		return file->ListGet<T>(key.c_str(), index);
	}

	template <typename T>
	T ListSet(StaticFunctionTag* base, BSFixedString name, BSFixedString key, UInt32 index, T value) {
		ExternalFile* file = GetFile(name.c_str());
		return file->ListSet<T>(key.c_str(), index, value);
	}

	template <typename T>
	UInt32 ListRemove(StaticFunctionTag* base, BSFixedString name, BSFixedString key, T value, bool allInstances) {
		ExternalFile* file = GetFile(name.c_str());
		return file->ListRemove<T>(key.c_str(), value, allInstances);
	}

	template <typename T>
	bool ListRemoveAt(StaticFunctionTag* base, BSFixedString name, BSFixedString key, UInt32 index) {
		ExternalFile* file = GetFile(name.c_str());
		return file->ListRemoveAt<T>(key.c_str(), index);
	}

	template <typename T>
	bool ListInsertAt(StaticFunctionTag* base, BSFixedString name, BSFixedString key, UInt32 index, T value) {
		ExternalFile* file = GetFile(name.c_str());
		return file->ListInsertAt<T>(key.c_str(), index, value);
	}

	template <typename T>
	UInt32 ListClear(StaticFunctionTag* base, BSFixedString name, BSFixedString key) {
		ExternalFile* file = GetFile(name.c_str());
		return file->ListClear<T>(key.c_str());
	}

	template <typename T>
	UInt32 ListCount(StaticFunctionTag* base, BSFixedString name, BSFixedString key) {
		ExternalFile* file = GetFile(name.c_str());
		return file->ListCount<T>(key.c_str());
	}

	template <typename T>
	SInt32 ListFind(StaticFunctionTag* base, BSFixedString name, BSFixedString key, T value) {
		ExternalFile* file = GetFile(name.c_str());
		return file->ListFind<T>(key.c_str(), value);
	}

	template <typename T>
	bool ListHas(StaticFunctionTag* base, BSFixedString name, BSFixedString key, T value) {
		ExternalFile* file = GetFile(name.c_str());
		return file->ListHas<T>(key.c_str(), value);
	}

	template <typename T>
	void ListSlice(StaticFunctionTag* base, BSFixedString name, BSFixedString key, VMArray<T> Output, UInt32 startIndex){
		if (Output.Length() < 1)
			return;
		ExternalFile* file = GetFile(name.c_str());
		file->ListSlice<T>(key.c_str(), Output, startIndex);
	}

	template <typename T>
	SInt32 ListResize(StaticFunctionTag* base, BSFixedString name, BSFixedString key, UInt32 length, T filler) {
		if (length > 500)
			return 0;
		ExternalFile* file = GetFile(name.c_str());
		return file->ListResize(key.c_str(), length, filler);
	}

	template <typename T>
	bool ListCopy(StaticFunctionTag* base, BSFixedString name, BSFixedString key, VMArray<T> Input) {
		if (Input.Length() < 1)
			return false;
		ExternalFile* file = GetFile(name.c_str());
		return file->ListCopy(key.c_str(), Input);
	}


	void RegisterFuncs(VirtualMachine* vm){

		// File manipulation
		vm->RegisterFunction(new NativeFunction2<StaticFunctionTag, bool, BSFixedString, bool>("Save", "JsonUtil", SaveJson, vm));
		vm->SetFunctionFlags("JsonUtil", "Save", IFunction::kFunctionFlag_NoWait);

		vm->RegisterFunction(new NativeFunction1<StaticFunctionTag, bool, BSFixedString>("Load", "JsonUtil", LoadJson, vm));
		vm->SetFunctionFlags("JsonUtil", "Load", IFunction::kFunctionFlag_NoWait);

		// Global  values
		vm->RegisterFunction(new NativeFunction3<StaticFunctionTag, SInt32, BSFixedString, BSFixedString, SInt32>("SetInt", "JsonUtil", SetValue<SInt32>, vm));
		vm->RegisterFunction(new NativeFunction3<StaticFunctionTag, float, BSFixedString, BSFixedString, float>("SetFloat", "JsonUtil", SetValue<float>, vm));
		vm->RegisterFunction(new NativeFunction3<StaticFunctionTag, BSFixedString, BSFixedString, BSFixedString, BSFixedString>("SetString", "JsonUtil", SetValue<BSFixedString>, vm));
		vm->RegisterFunction(new NativeFunction3<StaticFunctionTag, TESForm*, BSFixedString, BSFixedString, TESForm*>("SetForm", "JsonUtil", SetValue<TESForm*>, vm));
		vm->SetFunctionFlags("JsonUtil", "SetInt", IFunction::kFunctionFlag_NoWait);
		vm->SetFunctionFlags("JsonUtil", "SetFloat", IFunction::kFunctionFlag_NoWait);
		vm->SetFunctionFlags("JsonUtil", "SetString", IFunction::kFunctionFlag_NoWait);
		vm->SetFunctionFlags("JsonUtil", "SetForm", IFunction::kFunctionFlag_NoWait);

		vm->RegisterFunction(new NativeFunction3<StaticFunctionTag, SInt32, BSFixedString, BSFixedString, SInt32>("GetInt", "JsonUtil", GetValue<SInt32>, vm));
		vm->RegisterFunction(new NativeFunction3<StaticFunctionTag, float, BSFixedString, BSFixedString, float>("GetFloat", "JsonUtil", GetValue<float>, vm));
		vm->RegisterFunction(new NativeFunction3<StaticFunctionTag, BSFixedString, BSFixedString, BSFixedString, BSFixedString>("GetString", "JsonUtil", GetValue<BSFixedString>, vm));
		vm->RegisterFunction(new NativeFunction3<StaticFunctionTag, TESForm*, BSFixedString, BSFixedString, TESForm*>("GetForm", "JsonUtil", GetValue<TESForm*>, vm));
		vm->SetFunctionFlags("JsonUtil", "GetInt", IFunction::kFunctionFlag_NoWait);
		vm->SetFunctionFlags("JsonUtil", "GetFloat", IFunction::kFunctionFlag_NoWait);
		vm->SetFunctionFlags("JsonUtil", "GetString", IFunction::kFunctionFlag_NoWait);
		vm->SetFunctionFlags("JsonUtil", "GetForm", IFunction::kFunctionFlag_NoWait);

		vm->RegisterFunction(new NativeFunction2<StaticFunctionTag, bool, BSFixedString, BSFixedString>("UnsetInt", "JsonUtil", UnsetValue<SInt32>, vm));
		vm->RegisterFunction(new NativeFunction2<StaticFunctionTag, bool, BSFixedString, BSFixedString>("UnsetFloat", "JsonUtil", UnsetValue<float>, vm));
		vm->RegisterFunction(new NativeFunction2<StaticFunctionTag, bool, BSFixedString, BSFixedString>("UnsetString", "JsonUtil", UnsetValue<BSFixedString>, vm));
		vm->RegisterFunction(new NativeFunction2<StaticFunctionTag, bool, BSFixedString, BSFixedString>("UnsetForm", "JsonUtil", UnsetValue<TESForm*>, vm));
		vm->SetFunctionFlags("JsonUtil", "UnsetInt", IFunction::kFunctionFlag_NoWait);
		vm->SetFunctionFlags("JsonUtil", "UnsetFloat", IFunction::kFunctionFlag_NoWait);
		vm->SetFunctionFlags("JsonUtil", "UnsetString", IFunction::kFunctionFlag_NoWait);
		vm->SetFunctionFlags("JsonUtil", "UnsetForm", IFunction::kFunctionFlag_NoWait);

		vm->RegisterFunction(new NativeFunction2<StaticFunctionTag, bool, BSFixedString, BSFixedString>("HasInt", "JsonUtil", HasValue<SInt32>, vm));
		vm->RegisterFunction(new NativeFunction2<StaticFunctionTag, bool, BSFixedString, BSFixedString>("HasFloat", "JsonUtil", HasValue<float>, vm));
		vm->RegisterFunction(new NativeFunction2<StaticFunctionTag, bool, BSFixedString, BSFixedString>("HasString", "JsonUtil", HasValue<BSFixedString>, vm));
		vm->RegisterFunction(new NativeFunction2<StaticFunctionTag, bool, BSFixedString, BSFixedString>("HasForm", "JsonUtil", HasValue<TESForm*>, vm));
		vm->SetFunctionFlags("JsonUtil", "HasInt", IFunction::kFunctionFlag_NoWait);
		vm->SetFunctionFlags("JsonUtil", "HasFloat", IFunction::kFunctionFlag_NoWait);
		vm->SetFunctionFlags("JsonUtil", "HasString", IFunction::kFunctionFlag_NoWait);
		vm->SetFunctionFlags("JsonUtil", "HasForm", IFunction::kFunctionFlag_NoWait);

		// Global  lists
		vm->RegisterFunction(new NativeFunction4<StaticFunctionTag, UInt32, BSFixedString, BSFixedString, SInt32, bool>("IntListAdd", "JsonUtil", ListAdd<SInt32>, vm));
		vm->RegisterFunction(new NativeFunction4<StaticFunctionTag, UInt32, BSFixedString, BSFixedString, float, bool>("FloatListAdd", "JsonUtil", ListAdd<float>, vm));
		vm->RegisterFunction(new NativeFunction4<StaticFunctionTag, UInt32, BSFixedString, BSFixedString, BSFixedString, bool>("StringListAdd", "JsonUtil", ListAdd<BSFixedString>, vm));
		vm->RegisterFunction(new NativeFunction4<StaticFunctionTag, UInt32, BSFixedString, BSFixedString, TESForm*, bool>("FormListAdd", "JsonUtil", ListAdd<TESForm*>, vm));
		vm->SetFunctionFlags("JsonUtil", "IntListAdd", IFunction::kFunctionFlag_NoWait);
		vm->SetFunctionFlags("JsonUtil", "FloatListAdd", IFunction::kFunctionFlag_NoWait);
		vm->SetFunctionFlags("JsonUtil", "StringListAdd", IFunction::kFunctionFlag_NoWait);
		vm->SetFunctionFlags("JsonUtil", "FormListAdd", IFunction::kFunctionFlag_NoWait);

		vm->RegisterFunction(new NativeFunction3<StaticFunctionTag, SInt32, BSFixedString, BSFixedString, UInt32>("IntListGet", "JsonUtil", ListGet<SInt32>, vm));
		vm->RegisterFunction(new NativeFunction3<StaticFunctionTag, float, BSFixedString, BSFixedString, UInt32>("FloatListGet", "JsonUtil", ListGet<float>, vm));
		vm->RegisterFunction(new NativeFunction3<StaticFunctionTag, BSFixedString, BSFixedString, BSFixedString, UInt32>("StringListGet", "JsonUtil", ListGet<BSFixedString>, vm));
		vm->RegisterFunction(new NativeFunction3<StaticFunctionTag, TESForm*, BSFixedString, BSFixedString, UInt32>("FormListGet", "JsonUtil", ListGet<TESForm*>, vm));
		vm->SetFunctionFlags("JsonUtil", "IntListGet", IFunction::kFunctionFlag_NoWait);
		vm->SetFunctionFlags("JsonUtil", "FloatListGet", IFunction::kFunctionFlag_NoWait);
		vm->SetFunctionFlags("JsonUtil", "StringListGet", IFunction::kFunctionFlag_NoWait);
		vm->SetFunctionFlags("JsonUtil", "FormListGet", IFunction::kFunctionFlag_NoWait);

		vm->RegisterFunction(new NativeFunction4<StaticFunctionTag, SInt32, BSFixedString, BSFixedString, UInt32, SInt32>("IntListSet", "JsonUtil", ListSet<SInt32>, vm));
		vm->RegisterFunction(new NativeFunction4<StaticFunctionTag, float, BSFixedString, BSFixedString, UInt32, float>("FloatListSet", "JsonUtil", ListSet<float>, vm));
		vm->RegisterFunction(new NativeFunction4<StaticFunctionTag, BSFixedString, BSFixedString, BSFixedString, UInt32, BSFixedString>("StringListSet", "JsonUtil", ListSet<BSFixedString>, vm));
		vm->RegisterFunction(new NativeFunction4<StaticFunctionTag, TESForm*, BSFixedString, BSFixedString, UInt32, TESForm*>("FormListSet", "JsonUtil", ListSet<TESForm*>, vm));
		vm->SetFunctionFlags("JsonUtil", "IntListSet", IFunction::kFunctionFlag_NoWait);
		vm->SetFunctionFlags("JsonUtil", "FloatListSet", IFunction::kFunctionFlag_NoWait);
		vm->SetFunctionFlags("JsonUtil", "StringListSet", IFunction::kFunctionFlag_NoWait);
		vm->SetFunctionFlags("JsonUtil", "FormListSet", IFunction::kFunctionFlag_NoWait);

		vm->RegisterFunction(new NativeFunction4<StaticFunctionTag, UInt32, BSFixedString, BSFixedString, SInt32, bool>("IntListRemove", "JsonUtil", ListRemove<SInt32>, vm));
		vm->RegisterFunction(new NativeFunction4<StaticFunctionTag, UInt32, BSFixedString, BSFixedString, float, bool>("FloatListRemove", "JsonUtil", ListRemove<float>, vm));
		vm->RegisterFunction(new NativeFunction4<StaticFunctionTag, UInt32, BSFixedString, BSFixedString, BSFixedString, bool>("StringListRemove", "JsonUtil", ListRemove<BSFixedString>, vm));
		vm->RegisterFunction(new NativeFunction4<StaticFunctionTag, UInt32, BSFixedString, BSFixedString, TESForm*, bool>("FormListRemove", "JsonUtil", ListRemove<TESForm*>, vm));
		vm->SetFunctionFlags("JsonUtil", "IntListRemove", IFunction::kFunctionFlag_NoWait);
		vm->SetFunctionFlags("JsonUtil", "FloatListRemove", IFunction::kFunctionFlag_NoWait);
		vm->SetFunctionFlags("JsonUtil", "StringListRemove", IFunction::kFunctionFlag_NoWait);
		vm->SetFunctionFlags("JsonUtil", "FormListRemove", IFunction::kFunctionFlag_NoWait);

		vm->RegisterFunction(new NativeFunction3<StaticFunctionTag, bool, BSFixedString, BSFixedString, UInt32>("IntListRemoveAt", "JsonUtil", ListRemoveAt<SInt32>, vm));
		vm->RegisterFunction(new NativeFunction3<StaticFunctionTag, bool, BSFixedString, BSFixedString, UInt32>("FloatListRemoveAt", "JsonUtil", ListRemoveAt<float>, vm));
		vm->RegisterFunction(new NativeFunction3<StaticFunctionTag, bool, BSFixedString, BSFixedString, UInt32>("StringListRemoveAt", "JsonUtil", ListRemoveAt<BSFixedString>, vm));
		vm->RegisterFunction(new NativeFunction3<StaticFunctionTag, bool, BSFixedString, BSFixedString, UInt32>("FormListRemoveAt", "JsonUtil", ListRemoveAt<TESForm*>, vm));
		vm->SetFunctionFlags("JsonUtil", "IntListRemoveAt", IFunction::kFunctionFlag_NoWait);
		vm->SetFunctionFlags("JsonUtil", "FloatListRemoveAt", IFunction::kFunctionFlag_NoWait);
		vm->SetFunctionFlags("JsonUtil", "StringListRemoveAt", IFunction::kFunctionFlag_NoWait);
		vm->SetFunctionFlags("JsonUtil", "FormListRemoveAt", IFunction::kFunctionFlag_NoWait);

		vm->RegisterFunction(new NativeFunction4<StaticFunctionTag, bool, BSFixedString, BSFixedString, UInt32, SInt32>("IntListInsertAt", "JsonUtil", ListInsertAt<SInt32>, vm));
		vm->RegisterFunction(new NativeFunction4<StaticFunctionTag, bool, BSFixedString, BSFixedString, UInt32, float>("FloatListInsertAt", "JsonUtil", ListInsertAt<float>, vm));
		vm->RegisterFunction(new NativeFunction4<StaticFunctionTag, bool, BSFixedString, BSFixedString, UInt32, BSFixedString>("StringListInsertAt", "JsonUtil", ListInsertAt<BSFixedString>, vm));
		vm->RegisterFunction(new NativeFunction4<StaticFunctionTag, bool, BSFixedString, BSFixedString, UInt32, TESForm*>("FormListInsertAt", "JsonUtil", ListInsertAt<TESForm*>, vm));
		vm->SetFunctionFlags("JsonUtil", "IntListInsertAt", IFunction::kFunctionFlag_NoWait);
		vm->SetFunctionFlags("JsonUtil", "FloatListInsertAt", IFunction::kFunctionFlag_NoWait);
		vm->SetFunctionFlags("JsonUtil", "StringListInsertAt", IFunction::kFunctionFlag_NoWait);
		vm->SetFunctionFlags("JsonUtil", "FormListInsertAt", IFunction::kFunctionFlag_NoWait);

		vm->RegisterFunction(new NativeFunction2<StaticFunctionTag, UInt32, BSFixedString, BSFixedString>("IntListClear", "JsonUtil", ListClear<SInt32>, vm));
		vm->RegisterFunction(new NativeFunction2<StaticFunctionTag, UInt32, BSFixedString, BSFixedString>("FloatListClear", "JsonUtil", ListClear<float>, vm));
		vm->RegisterFunction(new NativeFunction2<StaticFunctionTag, UInt32, BSFixedString, BSFixedString>("StringListClear", "JsonUtil", ListClear<BSFixedString>, vm));
		vm->RegisterFunction(new NativeFunction2<StaticFunctionTag, UInt32, BSFixedString, BSFixedString>("FormListClear", "JsonUtil", ListClear<TESForm*>, vm));
		vm->SetFunctionFlags("JsonUtil", "IntListClear", IFunction::kFunctionFlag_NoWait);
		vm->SetFunctionFlags("JsonUtil", "FloatListClear", IFunction::kFunctionFlag_NoWait);
		vm->SetFunctionFlags("JsonUtil", "StringListClear", IFunction::kFunctionFlag_NoWait);
		vm->SetFunctionFlags("JsonUtil", "FormListClear", IFunction::kFunctionFlag_NoWait);

		vm->RegisterFunction(new NativeFunction2<StaticFunctionTag, UInt32, BSFixedString, BSFixedString>("IntListCount", "JsonUtil", ListCount<SInt32>, vm));
		vm->RegisterFunction(new NativeFunction2<StaticFunctionTag, UInt32, BSFixedString, BSFixedString>("FloatListCount", "JsonUtil", ListCount<float>, vm));
		vm->RegisterFunction(new NativeFunction2<StaticFunctionTag, UInt32, BSFixedString, BSFixedString>("StringListCount", "JsonUtil", ListCount<BSFixedString>, vm));
		vm->RegisterFunction(new NativeFunction2<StaticFunctionTag, UInt32, BSFixedString, BSFixedString>("FormListCount", "JsonUtil", ListCount<TESForm*>, vm));
		vm->SetFunctionFlags("JsonUtil", "IntListCount", IFunction::kFunctionFlag_NoWait);
		vm->SetFunctionFlags("JsonUtil", "FloatListCount", IFunction::kFunctionFlag_NoWait);
		vm->SetFunctionFlags("JsonUtil", "StringListCount", IFunction::kFunctionFlag_NoWait);
		vm->SetFunctionFlags("JsonUtil", "FormListCount", IFunction::kFunctionFlag_NoWait);

		vm->RegisterFunction(new NativeFunction3<StaticFunctionTag, SInt32, BSFixedString, BSFixedString, SInt32>("IntListFind", "JsonUtil", ListFind<SInt32>, vm));
		vm->RegisterFunction(new NativeFunction3<StaticFunctionTag, SInt32, BSFixedString, BSFixedString, float>("FloatListFind", "JsonUtil", ListFind<float>, vm));
		vm->RegisterFunction(new NativeFunction3<StaticFunctionTag, SInt32, BSFixedString, BSFixedString, BSFixedString>("StringListFind", "JsonUtil", ListFind<BSFixedString>, vm));
		vm->RegisterFunction(new NativeFunction3<StaticFunctionTag, SInt32, BSFixedString, BSFixedString, TESForm*>("FormListFind", "JsonUtil", ListFind<TESForm*>, vm));
		vm->SetFunctionFlags("JsonUtil", "IntListFind", IFunction::kFunctionFlag_NoWait);
		vm->SetFunctionFlags("JsonUtil", "FloatListFind", IFunction::kFunctionFlag_NoWait);
		vm->SetFunctionFlags("JsonUtil", "StringListFind", IFunction::kFunctionFlag_NoWait);
		vm->SetFunctionFlags("JsonUtil", "FormListFind", IFunction::kFunctionFlag_NoWait);

		vm->RegisterFunction(new NativeFunction3<StaticFunctionTag, bool, BSFixedString, BSFixedString, SInt32>("IntListHas", "JsonUtil", ListHas<SInt32>, vm));
		vm->RegisterFunction(new NativeFunction3<StaticFunctionTag, bool, BSFixedString, BSFixedString, float>("FloatListHas", "JsonUtil", ListHas<float>, vm));
		vm->RegisterFunction(new NativeFunction3<StaticFunctionTag, bool, BSFixedString, BSFixedString, BSFixedString>("StringListHas", "JsonUtil", ListHas<BSFixedString>, vm));
		vm->RegisterFunction(new NativeFunction3<StaticFunctionTag, bool, BSFixedString, BSFixedString, TESForm*>("FormListHas", "JsonUtil", ListHas<TESForm*>, vm));
		vm->SetFunctionFlags("JsonUtil", "IntListHas", IFunction::kFunctionFlag_NoWait);
		vm->SetFunctionFlags("JsonUtil", "FloatListHas", IFunction::kFunctionFlag_NoWait);
		vm->SetFunctionFlags("JsonUtil", "StringListHas", IFunction::kFunctionFlag_NoWait);
		vm->SetFunctionFlags("JsonUtil", "FormListHas", IFunction::kFunctionFlag_NoWait);

		vm->RegisterFunction(new NativeFunction4<StaticFunctionTag, void, BSFixedString, BSFixedString, VMArray<SInt32>, UInt32>("IntListSlice", "JsonUtil", ListSlice<SInt32>, vm));
		vm->RegisterFunction(new NativeFunction4<StaticFunctionTag, void, BSFixedString, BSFixedString, VMArray<float>, UInt32>("FloatListSlice", "JsonUtil", ListSlice<float>, vm));
		vm->RegisterFunction(new NativeFunction4<StaticFunctionTag, void, BSFixedString, BSFixedString, VMArray<BSFixedString>, UInt32>("StringListSlice", "JsonUtil", ListSlice<BSFixedString>, vm));
		vm->RegisterFunction(new NativeFunction4<StaticFunctionTag, void, BSFixedString, BSFixedString, VMArray<TESForm*>, UInt32>("FormListSlice", "JsonUtil", ListSlice<TESForm*>, vm));
		vm->SetFunctionFlags("JsonUtil", "IntListSlice", IFunction::kFunctionFlag_NoWait);
		vm->SetFunctionFlags("JsonUtil", "FloatListSlice", IFunction::kFunctionFlag_NoWait);
		vm->SetFunctionFlags("JsonUtil", "StringListSlice", IFunction::kFunctionFlag_NoWait);
		vm->SetFunctionFlags("JsonUtil", "FormListSlice", IFunction::kFunctionFlag_NoWait);

		vm->RegisterFunction(new NativeFunction4 <StaticFunctionTag, SInt32, BSFixedString, BSFixedString, UInt32, SInt32>("IntListResize", "JsonUtil", ListResize<SInt32>, vm));
		vm->RegisterFunction(new NativeFunction4 <StaticFunctionTag, SInt32, BSFixedString, BSFixedString, UInt32, float>("FloatListResize", "JsonUtil", ListResize<float>, vm));
		vm->RegisterFunction(new NativeFunction4 <StaticFunctionTag, SInt32, BSFixedString, BSFixedString, UInt32, BSFixedString>("StringListResize", "JsonUtil", ListResize<BSFixedString>, vm));
		vm->RegisterFunction(new NativeFunction4 <StaticFunctionTag, SInt32, BSFixedString, BSFixedString, UInt32, TESForm*>("FormListResize", "JsonUtil", ListResize<TESForm*>, vm));
		vm->SetFunctionFlags("JsonUtil", "IntListResize", IFunction::kFunctionFlag_NoWait);
		vm->SetFunctionFlags("JsonUtil", "FloatListResize", IFunction::kFunctionFlag_NoWait);
		vm->SetFunctionFlags("JsonUtil", "StringListResize", IFunction::kFunctionFlag_NoWait);
		vm->SetFunctionFlags("JsonUtil", "FormListResize", IFunction::kFunctionFlag_NoWait);

		vm->RegisterFunction(new NativeFunction3 <StaticFunctionTag, bool, BSFixedString, BSFixedString, VMArray<SInt32>>("IntListCopy", "JsonUtil", ListCopy<SInt32>, vm));
		vm->RegisterFunction(new NativeFunction3 <StaticFunctionTag, bool, BSFixedString, BSFixedString, VMArray<float>>("FloatListCopy", "JsonUtil", ListCopy<float>, vm));
		vm->RegisterFunction(new NativeFunction3 <StaticFunctionTag, bool, BSFixedString, BSFixedString, VMArray<BSFixedString>>("StringListCopy", "JsonUtil", ListCopy<BSFixedString>, vm));
		vm->RegisterFunction(new NativeFunction3 <StaticFunctionTag, bool, BSFixedString, BSFixedString, VMArray<TESForm*>>("FormListCopy", "JsonUtil", ListCopy<TESForm*>, vm));
		vm->SetFunctionFlags("JsonUtil", "IntListCopy", IFunction::kFunctionFlag_NoWait);
		vm->SetFunctionFlags("JsonUtil", "FloatListCopy", IFunction::kFunctionFlag_NoWait);
		vm->SetFunctionFlags("JsonUtil", "StringListCopy", IFunction::kFunctionFlag_NoWait);
		vm->SetFunctionFlags("JsonUtil", "FormListCopy", IFunction::kFunctionFlag_NoWait);
	}

}