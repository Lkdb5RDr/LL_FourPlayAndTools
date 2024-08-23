#include "Data.h"
#include "External.h"
#include "Forms.h"

#include "f4se/GameForms.h"

#include "f4se/PapyrusVM.h"
#include "f4se/PapyrusArgs.h"
#include "f4se/PapyrusNativeFunctions.h"

#include <boost/algorithm/string.hpp>

namespace Data {
	// Flat obj=>key=>value storage
	intv* intValues = NULL;
	flov* floatValues = NULL;
	strv* stringValues = NULL;
	forv* formValues = NULL;

	// Vector obj=>key=>vector[i] storage
	intl* intLists = NULL;
	flol* floatLists = NULL;
	strl* stringLists = NULL;
	forl* formLists = NULL;

	template <> intv* GetValues<SInt32, SInt32>() { return intValues; }
	template <> flov* GetValues<float, float>() { return floatValues; }
	template <> strv* GetValues<BSFixedString, std::string>() { return stringValues; }
	template <> forv* GetValues<TESForm*, UInt32>() { return formValues; }

	template <> intl* GetLists<SInt32, SInt32>() { return intLists; }
	template <> flol* GetLists<float, float>() { return floatLists; }
	template <> strl* GetLists<BSFixedString, std::string>() { return stringLists; }
	template <> forl* GetLists<TESForm*, UInt32>() { return formLists; }

	void InitLists() {
		intValues = new intv();
		floatValues = new flov();
		stringValues = new strv();
		formValues = new forv();

		intLists = new intl();
		floatLists = new flol();
		stringLists = new strl();
		formLists = new forl();
	}

	/*
	*
	* Define methods
	*
	*/

	template <typename T, typename S>
	T Values<T, S>::SetValue(UInt64 obj, std::string key, T value){
		s_dataLock.Enter();
		boost::to_lower(key);
		Data[obj][key] = cast(value);
		s_dataLock.Leave();
		return value;
	}

	template <typename T, typename S>
	T Values<T, S>::GetValue(UInt64 obj, std::string key, T value){
		s_dataLock.Enter();
		Map::iterator itr = Data.find(obj);
		if (itr != Data.end()){
			boost::to_lower(key);
			Obj::iterator itr2 = itr->second.find(key);
			if (itr2 != itr->second.end())
				value = cast(itr2->second);
		}
		s_dataLock.Leave();
		return value;
	}

	template <typename T, typename S>
	bool Values<T, S>::UnsetValue(UInt64 obj, std::string key){
		bool unset = false;
		s_dataLock.Enter();
		Map::iterator itr = Data.find(obj);
		if (itr != Data.end()){
			boost::to_lower(key);
			Obj::iterator itr2 = itr->second.find(key);
			if (itr2 != itr->second.end()){
				unset = true;
				itr->second.erase(itr2);
				if (itr->second.size() == 0)
					Data.erase(itr);
			}
		}
		s_dataLock.Leave();
		return unset;
	}

	template <typename T, typename S>
	bool Values<T, S>::HasValue(UInt64 obj, std::string key){
		bool found = false;
		s_dataLock.Enter();

		Map::iterator itr = Data.find(obj);
		if (itr != Data.end()){
			boost::to_lower(key);
			found = itr->second.find(key) != itr->second.end();
		}

		s_dataLock.Leave();
		return found;
	}
	
	template <typename T, typename S>
	int Lists<T, S>::ListAdd(UInt64 obj, std::string key, T value, bool allowDuplicate){
		if (!allowDuplicate && ListHas(obj, key, value))
			return -1;
		s_dataLock.Enter();

		boost::to_lower(key);
		int index = Data[obj][key].size();
		Data[obj][key].push_back(cast(value));

		s_dataLock.Leave();
		return index;
	}

	template <typename T, typename S>
	T Lists<T, S>::ListGet(UInt64 obj, std::string key, UInt32 index) {
		S value = S();
		s_dataLock.Enter();

		boost::to_lower(key);
		List* vector = GetVector(obj, key);
		if (vector != NULL && index >= 0 && index < vector->size())
			value = vector->at(index);

		s_dataLock.Leave();
		return cast(value);
	}


	template <typename T, typename S>
	T Lists<T, S>::ListSet(UInt64 obj, std::string key, UInt32 index, T value){
		s_dataLock.Enter();

		boost::to_lower(key);
		List* vector = GetVector(obj, key);
		if (vector != NULL && index >= 0 && index < vector->size())
			vector->at(index) = cast(value);
		else if (index == 0 && (vector == NULL || vector->size() == 0))
			Data[obj][key].push_back(cast(value));
		else{
			S var = S();
			value = cast(var);
		}

		s_dataLock.Leave();
		return value;
	}

	template <typename T, typename S>
	int Lists<T, S>::ListRemove(UInt64 obj, std::string key, T value, bool allInstances){
		int removed = 0;
		s_dataLock.Enter();

		boost::to_lower(key);
		List* vector = GetVector(obj, key);
		if (vector != NULL){
			// Remove
			int count = vector->size();
			if (allInstances)
				vector->erase(std::remove(vector->begin(), vector->end(), cast(value)), vector->end());
			else {
				List::iterator itr = std::find(vector->begin(), vector->end(), cast(value));
				if (itr != vector->end())
					vector->erase(itr);
			}
			removed = count - vector->size();
			// Cleanup
			if (vector->size() == 0) {
				Data[obj].erase(key);
				if (Data[obj].size() == 0)
					Data.erase(obj);
			}
		}

		s_dataLock.Leave();
		return removed;
	}

	template <typename T, typename S>
	bool Lists<T, S>::ListInsertAt(UInt64 obj, std::string key, UInt32 index, T value) {
		bool inserted = false;
		s_dataLock.Enter();
		boost::to_lower(key);
		List* vector = GetVector(obj, key);
		if (vector != NULL && vector->size() > 0 && index < vector->size()) {
			try {
				List::iterator itr = vector->begin();
				std::advance(itr, index);
				vector->insert(itr, cast(value));
				//vector->at(index) = value;
				inserted = true;
			}
			catch (const std::out_of_range&){
				inserted = false;
			}
		}
		else if (index == 0 && (vector == NULL || vector->size() == 0)){
			Data[obj][key].push_back(cast(value));
			inserted = true;
		}
		s_dataLock.Leave();
		return inserted;
	}

	template <typename T, typename S>
	bool Lists<T, S>::ListRemoveAt(UInt64 obj, std::string key, UInt32 index) {
		bool removed = false;
		s_dataLock.Enter();
		boost::to_lower(key);
		List* vector = GetVector(obj, key);
		if (vector != NULL && vector->size() > 0 && index < vector->size()) {
			vector->erase(vector->begin() + index);
			removed = true;
			// Cleanup
			if (vector->size() == 0) {
				Data[obj].erase(key);
				if (Data[obj].size() == 0)
					Data.erase(obj);
			}
		}
		s_dataLock.Leave();
		return removed;
	}


	template <typename T, typename S>
	int Lists<T, S>::ListClear(UInt64 obj, std::string key){
		int removed = 0;
		s_dataLock.Enter();
		Map::iterator itr = Data.find(obj);
		if (itr != Data.end()){
			boost::to_lower(key);
			Obj::iterator itr2 = itr->second.find(key);
			if (itr2 != itr->second.end()){
				removed = itr2->second.size();
				itr->second.erase(itr2);
				// Cleanup
				if (itr->second.size() == 0)
					Data.erase(itr);
			}
		}
		s_dataLock.Leave();
		return removed;
	}

	template <typename T, typename S>
	int Lists<T, S>::ListCount(UInt64 obj, std::string key){
		int count = 0;
		s_dataLock.Enter();
		boost::to_lower(key);
		List* vector = GetVector(obj, key);
		if (vector != NULL)
			count = vector->size();
		s_dataLock.Leave();
		return count;
	}

	template <typename T, typename S>
	int Lists<T, S>::ListFind(UInt64 obj, std::string key, T value){
		int index = -1;
		s_dataLock.Enter();
		boost::to_lower(key);
		List* vector = GetVector(obj, key);
		if (vector != NULL){
			List::iterator itr = std::find(vector->begin(), vector->end(), cast(value));
			if (itr != vector->end())
				index = itr - vector->begin();
		}
		s_dataLock.Leave();
		return index;
	}

	template <typename T, typename S>
	bool Lists<T, S>::ListHas(UInt64 obj, std::string key, T value){
		bool found = false;
		s_dataLock.Enter();
		boost::to_lower(key);
		List* vector = GetVector(obj, key);
		found = vector != NULL && std::find(vector->begin(), vector->end(), cast(value)) != vector->end();
		s_dataLock.Leave();
		return found;
	}
	

	template <typename T, typename S>
	void Lists<T, S>::ListSort(UInt64 obj, std::string key) {
		s_dataLock.Enter();
		boost::to_lower(key);
		List* vector = GetVector(obj, key);
		if (vector != NULL)
			std::sort(vector->begin(), vector->end());
		s_dataLock.Leave();
	}
	
	template <typename T, typename S>
	void Lists<T, S>::ListSlice(UInt64 obj, std::string key, VMArray<T> Output, UInt32 startIndex){
		s_dataLock.Enter();

		boost::to_lower(key);
		List* vector = GetVector(obj, key);
		if (vector != NULL && startIndex < vector->size() && startIndex >= 0) {
			List::iterator itr = vector->begin();
			std::advance(itr, startIndex);
			for (UInt32 index = 0; index < Output.Length() && itr != vector->end(); ++itr, ++index){
				T val = cast(*itr);
				Output.Set(&val, index);
			}
		}
		s_dataLock.Leave();
	}

	template <typename T, typename S>
	int Lists<T, S>::ListResize(UInt64 obj, std::string key, UInt32 length, T filler){
		if (length == 0)
			return ListClear(obj, key) * -1;
		s_dataLock.Enter();

		int start = 0;
		S value = cast(filler);
		boost::to_lower(key);
		List* vector = GetVector(obj, key);
		if (vector != NULL) {
			start = vector->size();
			if (length < vector->size())
				vector->resize(length);
			else {
				for (UInt32 i = 0; length > vector->size() && i < 500; ++i)
					vector->push_back(value);
			}
		}
		else {
			for (UInt32 i = 0; length > Data[obj][key].size() && i < 500; ++i)
				Data[obj][key].push_back(value);				
		}

		s_dataLock.Leave();
		return Data[obj][key].size() - start;
	}

	template <typename T, typename S>
	bool Lists<T, S>::ListCopy(UInt64 obj, std::string key, VMArray<T> Input) {
		if (Input.Length() < 1)
			return false;
		s_dataLock.Enter();
		boost::to_lower(key);
		List* vector = GetVector(obj, key);
		if (vector != NULL)
			vector->clear();
		for (UInt32 i = 0; i < Input.Length(); ++i) {
			T var;
			Input.Get(&var, i);
			S value = cast(var);
			Data[obj][key].push_back(value);
		}
		s_dataLock.Leave();
		return true;
	}


	// Special case for strings
	template <> int Lists<BSFixedString, std::string>::ListFind(UInt64 obj, std::string key, BSFixedString value){
		int index = -1;
		s_dataLock.Enter();
		boost::to_lower(key);
		List* vector = GetVector(obj, key);
		if (vector != NULL){
			std::string var = value.c_str();
			for (List::iterator itr = vector->begin(); itr != vector->end(); ++itr){
				if (boost::iequals(var, *itr)) {
					index = itr - vector->begin();
					break;
				}
			}
		}
		s_dataLock.Leave();
		return index;
	}

	template <> bool Lists<BSFixedString, std::string>::ListHas(UInt64 obj, std::string key, BSFixedString value) {
		return ListFind(obj, key, value) != -1;
	}

	/*template <> int Lists<BSFixedString, std::string>::ListAdd(UInt64 obj, std::string key, BSFixedString value, bool allowDuplicate){
		if (!allowDuplicate && ListFind(obj, key, value) != -1)
			return -1;
		s_dataLock.Enter();

		boost::to_lower(key);
		int index = Data[obj][key].size();
		Data[obj][key].push_back(value.c_str());
		
		s_dataLock.Leave();
		return index;
	}*/

	template <> int Lists<BSFixedString, std::string>::ListRemove(UInt64 obj, std::string key, BSFixedString value, bool allInstances){
		int removed = 0;
		s_dataLock.Enter();

		boost::to_lower(key);
		List* vector = GetVector(obj, key);
		if (vector != NULL){
			// Remove
			int count = vector->size();
			std::string var = value.c_str();
			for (List::iterator itr = vector->begin(); itr != vector->end();){
				if (!boost::iequals(var, *itr)){
					vector->erase(itr);
					count++;
					if (allInstances)
						break;
				}
				else ++itr;
			}
			removed = count - vector->size();
			// Cleanup
			if (vector->size() == 0) {
				Data[obj].erase(key);
				if (Data[obj].size() == 0)
					Data.erase(obj);
			}
		}

		s_dataLock.Leave();
		return removed;
	}

	/*
	*
	* DEBUG methods
	*
	*/

	template <typename T, typename S>
	void Values<T, S>::RemoveForm(UInt64 &obj) {
		s_dataLock.Enter();
		Map::iterator itr = Data.find(obj);
		if (itr != Data.end())
			Data.erase(itr);
		s_dataLock.Leave();
	}

	template <typename T, typename S>
	void Lists<T, S>::RemoveForm(UInt64 &obj) {
		s_dataLock.Enter();
		Map::iterator itr = Data.find(obj);
		if (itr != Data.end())
			Data.erase(itr);
		s_dataLock.Leave();
	}

	template <typename T, typename S>
	int Values<T, S>::Cleanup() {
		int removed = 0;
		s_dataLock.Enter();
		for (Map::iterator itr = Data.begin(); itr != Data.end();){
			if (itr->first == 0){
				itr++;
				continue;
			}
			TESForm* ptr = (TESForm*)Forms::GameGetForm((UInt32)(itr->first & 0xFFFFFFFF));
			if (!ptr || !Forms::IsValidObject(ptr, itr->first)) {
				removed++;
				itr = Data.erase(itr);
			}
			else
				itr++;
		}
		s_dataLock.Leave();
		return removed;
	}
	
	template <typename T, typename S>
	int Lists<T, S>::Cleanup() {
		int removed = 0;
		s_dataLock.Enter();
		for (Map::iterator itr = Data.begin(); itr != Data.end();){
			if (itr->first == 0){
				itr++;
				continue;
			}
			TESForm* ptr = (TESForm*)Forms::GameGetForm((UInt32)(itr->first & 0xFFFFFFFF));
			if (!ptr || !Forms::IsValidObject(ptr, itr->first)) {
				removed++;
				itr = Data.erase(itr);
			}
			else
				itr++;
		}
		s_dataLock.Leave();
		return removed;
	}

	template <typename T, typename S>
	TESForm* Values<T, S>::GetNthObj(UInt32 i) {
		if (Data.size() <= i)
			return NULL;
		Map::iterator itr = Data.begin();
		std::advance(itr, i);
		return itr != Data.end() ? LookupFormByID((UInt32)(itr->first & 0xFFFFFFFF)) : NULL;
	}

	template <typename T, typename S>
	TESForm* Lists<T, S>::GetNthObj(UInt32 i) {
		if (Data.size() <= i)
			return NULL;
		Map::iterator itr = Data.begin();
		std::advance(itr, i);
		return itr != Data.end() ? LookupFormByID((UInt32)(itr->first & 0xFFFFFFFF)) : NULL;
	}

	template <typename T, typename S>
	std::string Values<T, S>::GetNthKey(UInt64 obj, UInt32 i){
		if (Data.find(obj) == Data.end() || Data[obj].size() <= i)
			return "";
		Obj::iterator itr = Data[obj].begin();
		std::advance(itr, i);
		return itr != Data[obj].end() ? itr->first : "";
	}

	template <typename T, typename S>
	std::string Lists<T, S>::GetNthKey(UInt64 obj, UInt32 i){
		if (Data.find(obj) == Data.end() || Data[obj].size() <= i)
			return "";
		Obj::iterator itr = Data[obj].begin();
		std::advance(itr, i);
		return itr != Data[obj].end() ? itr->first : "";
	}

	// Data Seralization
	const UInt32 kSerializationDataVersion = 1;

	template <typename S>
	void EncodeValue(S &v) {}
	void EncodeValue(std::string &v){
		if (v.empty())
			v += (char)0x1B;
		else
			std::replace(v.begin(), v.end(), ' ', (char)0x7);
	}

	template <typename S>
	void DecodeValue(S &v) {}
	void DecodeValue(std::string &v){
		if (v.size() == 1 && v[0] == (char)0x1B)
			v.clear();
		else
			std::replace(v.begin(), v.end(), (char)0x7, ' ');
	}
	void DecodeValue(UInt32 &v){ v = Forms::GetNewId(v); }

	template <typename T, typename S>
	void Values<T, S>::Revert(){
		s_dataLock.Enter();
		Data.clear();
		s_dataLock.Leave();
	}

	template <typename T, typename S>
	void Lists<T, S>::Revert(){
		s_dataLock.Enter();
		Data.clear();
		s_dataLock.Leave();
	}

	// LEGACY style LOADING in single stream
	template <typename T, typename S>
	void Values<T, S>::LoadStream(std::stringstream &ss){
		int count;
		ss >> count;
		if (count < 1)
			return;
		s_dataLock.Enter();
		for (int i = 0; i < count; i++) {
			UInt64 objKey;
			ss >> objKey;
			objKey = Forms::GetNewKey(objKey);
			int count2;
			ss >> count2;
			for (int n = 0; n < count2; n++) {
				// Key
				std::string key;
				ss >> key;
				DecodeValue(key);
				boost::to_lower(key);
				// Value
				S value;
				ss >> value;
				DecodeValue(value);
				// Save to data store
				Data[objKey][key] = value;
			}
		}
		s_dataLock.Leave();
	}

	template <typename T, typename S>
	void Lists<T, S>::LoadStream(std::stringstream &ss){
		int count;
		ss >> count;
		if (count < 1)
			return;
		s_dataLock.Enter();
		for (int i = 0; i < count; ++i) {
			UInt64 objKey;
			ss >> objKey;
			objKey = Forms::GetNewKey(objKey);
			int count2;
			ss >> count2;
			for (int n = 0; n < count2; ++n) {
				// Key
				std::string key;
				ss >> key;
				DecodeValue(key);
				boost::to_lower(key);
				// Vector
				int count3;
				ss >> count3;
				for (int k = 0; k < count3; ++k){
					S value;
					ss >> value;
					DecodeValue(value);
					Data[objKey][key].push_back(value);
				}

			}
		}
		s_dataLock.Leave();
	}


	// NEW style SAVING with individual streams
	template <typename T, typename S>
	void Values<T, S>::Save(const F4SESerializationInterface *intfc, UInt32 type){
		if (intfc->OpenRecord(type, kSerializationDataVersion)) {
			s_dataLock.Enter();
			std::stringstream ss;
			ss << (int)Data.size();
			for (Map::iterator i = Data.begin(); i != Data.end(); ++i) {
				ss << ' ' << i->first;
				ss << ' ' << (int)i->second.size();
				for (Obj::iterator n = i->second.begin(); n != i->second.end(); ++n) {
					// Key
					std::string key = n->first;
					EncodeValue(key);
					ss << ' ' << key;
					// Value
					S val = n->second;
					EncodeValue(val);
					ss << ' ' << val;
				}
			}
			std::string str = ss.str();
			const char *cstr = str.c_str();
			//_MESSAGE(cstr);
			intfc->WriteRecordData(cstr, strlen(cstr));
			s_dataLock.Leave();
		}
	}

	template <typename T, typename S>
	void Lists<T, S>::Save(const F4SESerializationInterface *intfc, UInt32 type){
		if (intfc->OpenRecord(type, kSerializationDataVersion)) {
			s_dataLock.Enter();
			std::stringstream ss;
			ss << (int)Data.size();
			for (Map::iterator i = Data.begin(); i != Data.end(); ++i) {
				ss << ' ' << i->first;
				ss << ' ' << (int)i->second.size();
				for (Obj::iterator n = i->second.begin(); n != i->second.end(); ++n) {
					// Key
					std::string key = n->first;
					EncodeValue(key);
					ss << ' ' << key;
					// Vector
					ss << ' ' << (int)n->second.size();
					for (List::iterator itr = n->second.begin(); itr != n->second.end(); ++itr) {
						S value = *itr;
						EncodeValue(value);
						ss << ' ' << value;
					}
				}
			}
			std::string str = ss.str();
			const char *cstr = str.c_str();
			//_MESSAGE(cstr);
			intfc->WriteRecordData(cstr, strlen(cstr));
			s_dataLock.Leave();
		}
	}
	// NEW style LOADING with individual streams
	template <typename T, typename S>
	void Values<T, S>::Load(const F4SESerializationInterface *intfc, UInt32 &version, UInt32 &length){
		if (version == kSerializationDataVersion) {
			if (length) {
				char *buf = new char[length + 1];
				intfc->ReadRecordData(buf, length);
				buf[length] = 0;
				_MESSAGE("length: %d", strlen(buf));
				std::stringstream ss(buf);
				LoadStream(ss);
				delete[] buf;
			}
		}
	}

	template <typename T, typename S>
	void Lists<T, S>::Load(const F4SESerializationInterface *intfc, UInt32 &version, UInt32 &length){
		if (version == kSerializationDataVersion) {
			if (length) {
				char *buf = new char[length + 1];
				intfc->ReadRecordData(buf, length);
				buf[length] = 0;
				_MESSAGE("length: %d", strlen(buf));
				std::stringstream ss(buf);
				LoadStream(ss);
				delete[] buf;
			}
		}
	}

	template class Values<SInt32, SInt32>;
	template class Values<float, float>;
	template class Values<BSFixedString, std::string>;
	template class Values<TESForm*, UInt32>;
	template class Lists<SInt32, SInt32>;
	template class Lists<float, float>;
	template class Lists<BSFixedString, std::string>;
	template class Lists<TESForm*, UInt32>;


	//ICriticalSection s_serialLock;
	
	void Serialization_Load(const F4SESerializationInterface* intfc) {
		_MESSAGE("Storage Loading...");
		UInt32	type;
		UInt32	version;
		UInt32	length;
		bool	error = false;

		if (!intValues)
			InitLists();
		Forms::LoadCurrentMods();

		while (!error && intfc->GetNextRecordInfo(&type, &version, &length)) {
			switch (type) {
				// Values
			case 'INTV': {
							 _MESSAGE("INTV Load");
							 intValues->Load(intfc, version, length);
			}
				break;
			case 'FLOV': {
							 _MESSAGE("FLOV Load");
							 floatValues->Load(intfc, version, length);
			}
				break;
			case 'STRV': {
							 _MESSAGE("STRV Load");
							 stringValues->Load(intfc, version, length);
			}
				break;
			case 'FORV': {
							 _MESSAGE("FORV Load");
							 formValues->Load(intfc, version, length);
			}
				break;
				// Lists
			case 'INTL': {
							 _MESSAGE("INTL Load");
							 intLists->Load(intfc, version, length);
			}
				break;
			case 'FLOL': {
							 _MESSAGE("FLOL Load");
							 floatLists->Load(intfc, version, length);
			}
				break;
			case 'STRL': {
							 _MESSAGE("STRL Load");
							 stringLists->Load(intfc, version, length);
			}
				break;
			case 'FORL': {
							 _MESSAGE("FORL Load");
							 formLists->Load(intfc, version, length);
			}
				// Overrides
			case 'PACO': {
							 //_MESSAGE("PACO Load");
							 //packageLists->Load(intfc, version, length);
			}
				break;
			case 'IDLO': {
							 //_MESSAGE("IDLO Load");
							 //idleLists->Load(intfc, version, length);
			}
				break;
			case 'DATA': {
							 _MESSAGE("DATA Load");
							 if (version == kSerializationDataVersion) {
								 if (length) {
									 char *buf = new char[length + 1];
									 intfc->ReadRecordData(buf, length);
									 buf[length] = 0;
									 std::stringstream ss(buf);
									 _MESSAGE("data: %d", strlen(ss.str().c_str()));
									 int ver;
									 ss >> ver;
									 Forms::LoadPreviousMods(ss);
									 if (ver == -1){
										 _MESSAGE("Loading legacy data...");
										 intValues->LoadStream(ss);
										 floatValues->LoadStream(ss);
										 stringValues->LoadStream(ss);
										 formValues->LoadStream(ss);

										 intLists->LoadStream(ss);
										 floatLists->LoadStream(ss);
										 stringLists->LoadStream(ss);
										 formLists->LoadStream(ss);
									 }

									 delete[] buf;
								 }
							 }

			}
				break;
			default:
				_MESSAGE("unhandled type %08X", type);
				//error = true;
				break;
			}
		}
		Forms::ClearPreviousMods();
	}

	void Serialization_Save(const F4SESerializationInterface* intfc) {
		_MESSAGE("Storage Saving...");

		if (!intValues)
			InitLists();

		Forms::LoadCurrentMods();
		//Forms::ClearPreviousMods();

		// Save load order
		if (intfc->OpenRecord('DATA', kSerializationDataVersion)) {
			std::stringstream ss;
			ss << (int)1;
			Forms::SaveCurrentMods(ss);
			std::string str = ss.str();
			const char *cstr = str.c_str();
			intfc->WriteRecordData(cstr, strlen(cstr));
		}

		// Cleanup removed forms
		int cleaned = 0;
		cleaned += intValues->Cleanup();
		cleaned += floatValues->Cleanup();
		cleaned += stringValues->Cleanup();
		cleaned += formValues->Cleanup();
		cleaned += intLists->Cleanup();
		cleaned += floatLists->Cleanup();
		cleaned += stringLists->Cleanup();
		cleaned += formLists->Cleanup();
		_MESSAGE("Removed Objects: %d", cleaned);

		// Save value storage
		intValues->Save(intfc, 'INTV');
		floatValues->Save(intfc, 'FLOV');
		stringValues->Save(intfc, 'STRV');
		formValues->Save(intfc, 'FORV');
		// Save list storage
		intLists->Save(intfc, 'INTL');
		floatLists->Save(intfc, 'FLOL');
		stringLists->Save(intfc, 'STRL');
		formLists->Save(intfc, 'FORL');

		// Save external files
		External::SaveFiles();

	}

	void Serialization_Revert(const F4SESerializationInterface* intfc) {
		_MESSAGE("Storage Reverting...");
		if (!intValues)
			InitLists();
		else {
			Forms::LoadCurrentMods();
			Forms::ClearPreviousMods();

			intValues->Revert();
			floatValues->Revert();
			stringValues->Revert();
			formValues->Revert();

			intLists->Revert();
			floatLists->Revert();
			stringLists->Revert();
			formLists->Revert();

			// Revert external files
			External::RevertFiles();
		}
	}

}



