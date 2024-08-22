#pragma once

#include "f4se/GameTypes.h"
#include "f4se/GameData.h"
#include "f4se/GameForms.h"
#include "f4se/GameReferences.h"

#include <sstream>

namespace Forms {

	// Load Order
	void LoadCurrentMods();
	void ClearPreviousMods();

	void SaveCurrentMods(std::stringstream &ss);
	void LoadPreviousMods(std::stringstream &ss);

	// Forms
	inline UInt32 GetBaseID(TESForm* obj);
	inline UInt32 GetBaseID(UInt32 formID);
	inline std::string GetFormOwner(TESForm* obj);
	inline std::string GetFormOwner(UInt32 obj);

	UInt32 GetNewId(UInt32 oldId);
	UInt32 GetNewId(UInt32 oldId, std::string modName);
	UInt32 GetNewId(UInt32 oldId);

	// Form Keys
	UInt64 GetFormKey(TESForm* form);
	TESForm* GetFormKey(UInt64 key);
	UInt64 GetNewKey(UInt64 key);
	UInt64 GetNewKey(UInt64 key, std::string modName);


	// Form Strings
	std::string GetFormString(TESForm* obj);
	TESForm* ParseFormString(std::string &objString);

	// Misc
	TESForm* GameGetForm(int formId);
	bool IsValidObject(TESForm* obj, UInt64 formId);


	// TODO
	/*struct FormPair {
		UInt32 id;
		const char* mod;
		TESForm* asForm() {
			if (id == 0 || !mod)
				return NULL;
			UInt8 index = DataHandler::GetSingleton()->GetModIndex(mod);
			UInt32 obj = (((UInt32)index) << 24) | id;
			return obj == 0 ? NULL : LookupFormByID(obj);
		}
	};*/

}