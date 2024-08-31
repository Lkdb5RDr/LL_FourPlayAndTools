Scriptname LL_FourPlay Native Hidden

;
;	Shared community library of utility function from LoverLab distributed with FourPlay resources as a F4SE plugin with sources included
;

;	Version 53 for runtime 1.10.984	2024 09 01 by jaam and Chosen Clue and EgoBallistic and Fedim

;	Runtime version: This file should be runtime neutral. The accompanying F4SE plugin (ll_fourplay_1_10_984.dll) is NOT!
;		You need to always use a plugin corresponding with the game version you play.
;		Plugin should be available just after F4SE has been updated for the modified runtime.
;		Runtime versions lower than 1.10.984 will no longer be supported.
;		Written and tested against F4SE 0.7.2. You should not use an older version of F4SE.
;
;
;

; Returns the version of this script (when someone has not forgotten to update it :) )
Float Function GetLLFPScriptVersion() global
	return 53.0
endFunction

; Returns the version of the plugin and servers to verify it is properly installed.
Float Function GetLLFPPluginVersion() native global

; Custom profile: written into "Data\F4SE\Plugins"
; ===============

; Returns the full path for custom profile name. IE "WhereverYourGameIs\Data\F4SE\Plugins\name". Don't forget to provide the .ini extension.
string Function GetCustomConfigPath(string name) native global

; Get the value of custom config string option
string Function GetCustomConfigOption(string name, string section, string key) native global

; Get the value of custom config integer option (Use 0/1 for boolean)
int Function GetCustomConfigOption_UInt32(string name, string section, string key) native global

; Get the value of custom config float option
Float Function GetCustomConfigOption_float(string name, string section, string key) native global

; Sets the value of custom config string option (at most 66535 characters per option). The directories and path will be created as needed. If the result is false, the set did not happen.
bool Function SetCustomConfigOption(string name, string section, string key, string value) native global

; Sets the value of custom config integer option. The directories and path will be created as needed. If the result is false, the set did not happen.
bool Function SetCustomConfigOption_UInt32(string name, string section, string key, int data) native global

; Sets the value of custom config float option. The directories and path will be created as needed. If the result is false, the set did not happen.
bool Function SetCustomConfigOption_float(string name, string section, string key, float data) native global

; Get all the keys and values contained in a section. Both at once to avoid discrepancies in the order.
;	The keys are in VarToVarArray(Var[0]) as String[] and the values in VarToVarArray(Var[1]) as String[]
Var[] Function GetCustomConfigOptions(string fileName, string section) native global

; Set a list of keys and values in a section. Any other existing key will be left alone.
bool Function SetCustomConfigOptions(string fileName, string section, string[] keys, string[] values) native global

; Reset all the keys and values contained in a section.
;	Any exiting key value pair will be suppressed first, so providing none arrays will effectivly removes all keys from the section.
bool Function ResetCustomConfigOptions(string fileName, string section, string[] keys, string[] values) native global

; Get all the sections in a file.
string[] Function GetCustomConfigSections(string fileName) native global

; For all array functions:
;	The implementation as an arbitrary limitation of 32767 bytes buffer for all Keys, Values or sections involved.
;		If needed because the limitation becomes a problem, another implementation can be done using memory allocation, though there will remain a limit imposed by Windows.
;	When arrays for keys and values are provided, the count of elements in both arrays must be identical or the function fails on purpose.
;	An empty value should be provided as a zero length string.	TO BE TESTED

;
;	Camera functions
;	================
;

; Forces the FlyCam state.
;	if activate is true and the FlyCam is not active AND the FXCam is not active either, the FlyCam will be activated.
;	if activate is false and the FlyCam is active, the FlyCam will be deactivated.
;	if the requested state is identical to the current state nothing is done.
;		Returns whether the FlyCam was active or not before the call so mods can restore the previous state if needed.
bool Function SetFlyCam(bool activate) native global

;	TO BE TESTED
; Forces the FlyCam state. Alternative version that allows to pause/unpause the game when entering FlyCam
;	if activate is true and the FlyCam is not active AND the FXCam is not active either, the FlyCam will be activated.
;		if pause then the game will be paused
;	if activate is false and the FlyCam is active, the FlyCam will be deactivated.
;	otherwise if the requested activate is identical to the current state nothing is done.
;		Returns whether the FlyCam was active or not before the call so mods can restore the previous state if needed.
;TBT; bool Function SetFlyCam2(bool activate, bool pause) native global
;	TO BE TESTED - So far this is useless as scripts seem to be stopped while in pause mode :(

; Get the current state of the FlyCam
bool Function GetFlyCam() native global

; Get the current pause state of the game
bool Function GetPaused() native global

; Get the current state of the FXCam
bool Function GetFXCam() native global

; Select the speed at which the FlyCam moves (identical to SetUFOCamSpeedMult/sucsm console command)
;	The console command supports an optional second parameter to control rotation speed.
;	The way it handles default value is not compatible so I use an explicit bool to select which speed to change
;	Returns the previous value of the selected speed.
float Function SetFlyCamSpeedMult(float speed, bool rotation=False) native global

;
;	Power Armor/Race/Skeleton functions
;	===================================
;

;	Returns the actor's race when in PowerArmor
Race Function GetActorPowerArmorRace(Actor akActor) native global

;	Returns the actor's skeleton when in PowerArmor
string Function GetActorPowerArmorSkeleton(Actor akActor) native global

;	Returns the actor's current skeleton, not affected by PowerArmor
string Function GetActorSkeleton(Actor akActor) native global

;Chosen Clue Edit

;
;	String functions
;	================
;

;	Returns the first index of the position the toFind string starts. You can use this to check if an animation has a tag on it. Is not case sensitive.
Int Function StringFind(string theString, string toFind, int startIndex = 0) native global

;	Returns the selected substring from theString. If no length is set, the entire string past the startIndex number is returned.
string Function StringSubstring(string theString, int startIndex, int len = 0) native global

;	Splits the string into a string array based on the delimiter given in the second parameter. 
;	As this function does ignore whitespace, putting a space as the delimiter will only result in a string being returned without spaces.
string[] Function StringSplit(string theString, string delimiter = ",") native global

;	Opposite of StringSplit.
string Function StringJoin(string[] theStrings, string delimiter = ",") native global

;	Converts an integer of any base to a hexadecimal string representation
string Function IntToHexString(Int num) native global

;	Converts a hexadecimal string to an integer
Int Function HexStringToInt(String theString) native global

;
;	Array functions
;	===============
;

;Just a precursor: This does not mean we can use Alias based scripts to store animations like sexlab does, as the F4SE team has yet to include a typedef of them in the F4SE CPP files. I am guessing that they haven't reverse engineered it yet.

Form[] Function ResizeFormArray(Form[] theArray, int theSize, Form theFill = NONE) native global

String[] Function ResizeStringArray(String[] theArray, int theSize, String theFill = "") native global

Int[] Function ResizeIntArray(Int[] theArray, int theSize, Int theFill = 0) native global

Float[] Function ResizeFloatArray(Float[] theArray, int theSize, Float theFill = 0.0) native global

Bool[] Function ResizeBoolArray(Bool[] theArray, int theSize, Bool theFill = False) native global

Var[] Function ResizeVarArray2(Var[] theArray, int theSize) native global

Var[] Function ResizeVarArray(Var[] theArray, int theSize) global
	; Because filling with invalid values will CTD
	; Bugged for any version prior to 14
	; theFill will be ignored, but kept for compatibility. Anyway nobody used it ever as it would have CTD. Please use ResizeVarArray2
	Int theFill = 0
	return ResizeVarArrayInternal(theArray, theSize, theFill)
endFunction

;	if the int theSize is negative, the resulting array is a copy of the original array unchanged.

; Sets the minimum array size required by a mod. Returns false if the current value was greater.
Bool Function SetMinimalMaxArraySize(int theMaxArraySize) native global
; This patches ArrayAdd and ArrayInsert so they respect that maximum. The value is memorised in ToolLib.ini
;[Custom Arrays]
;uMaxArraySize=nnnnnn
;
; !! Creating arrays that are too large will adversaly affect your game !!

;
;	Keyword functions
;	=================
;

; Return the first keyword whose editorID is akEditorID
Keyword Function GetKeywordByName(string akEditorID) native global

; Adds a keyword to a form (not only a reference). Does not persists.
bool Function AddKeywordToForm(Form akForm, Keyword akKeyword) native global

; Delete a keyword from a form (not only a reference). Does not persists.
bool Function DelKeywordFromForm(Form akForm, Keyword akKeyword) native global

; Return an array of all keywords loaded in game.
Keyword[] Function GetAllKeywords() native global

;
;	CrossHair functions
;	====================
;

;	Returns the Reference that is currently under the CrossHair. Returns None if there isn't one currently.
ObjectReference Function LastCrossHairRef() native global

;	Returns the last Actor that is or was under the CrossHair. Returns None until there is one since the game was (re)started.
Actor Function LastCrossHairActor() native global

;
;	ObjectReference functions
;	=========================
;

;	Set the reference display name as a string without token. TO BE TESTED
bool Function ObjectReferenceSetSimpleDisplayName(ObjectReference akObject, string displayName) native global

; Sets reference angle in degrees. Will not cause load screen if called on Player
Function SetRefAngle(ObjectReference akRef, float x, float y, float z) native global

; Sets no-collision state on reference
Function SetRefNoCollision(ObjectReference akRef, bool abState) native global

; Get the scale of a reference
Float Function GetScale(ObjectReference akRef) native global

; Set the scale of a reference
Function SetScale(ObjectReference akRef, Float afScale) native global

;
;	Actor functions
;	===============
;

;	Check if the ActorBase has a specific skin. TO BE TESTED
bool Function ActorBaseIsClean(Actor akActor) native global

;	Return the WNAM ARMO of either the actor base or the actor's race
Form Function GetActorBaseSkinForm(Actor akActor) native global

;	Copy the base skin of one actor onto another as a skin override, similar to what LooksMenu does
bool Function CopyActorBaseskinForm(Actor akSourceActor, Actor akDestActor) native global

;	MFG morph function provided by EgoBallistic

;   Apply a MFG Morph to the actor
bool Function MfgMorph(Actor akActor, int morphID, int intensity) native global

;	Set all MFG Morph values to zero on an actor
bool Function MfgResetMorphs(Actor akActor) native global

;	Save an actor's MFG Morph values to an array of float
Float[] Function MfgSaveMorphs(Actor akActor) native global

;	Restore an array of saved MFG morph values to an actor
bool Function MfgRestoreMorphs(Actor akActor, Float[] values) native global

;	Copy the MFG morph values from one actor to another
bool Function MfgCopyMorphs(Actor a0, Actor a1) native global

;	Apply a set of MFG morphs to an actor.  Morph ID morphIDs[x] will be set to values[x]
bool Function MfgApplyMorphSet(Actor akActor, int[] morphIDs, int[] values) native global

; Return an Actor's actor flags (not the same as the Form flags)
Int Function GetActorFlags(Actor akActor) native global

;   Reset an actor's persistent Essential / Protected data
;	Calling Actor.SetEssential() overrides the actor base value. Changing the state on the actor base will no longer have any effect, and
;	neither will the Essential flag on Aliases.  This function resets the override so ActorBase and alias flags will work again.
bool Function ResetActorEssential(Actor akActor) native global

; Sets an actor's position. Directly moves the ref so does not cause load screen if called on player
Function SetActorPosition(Actor akActor, float x, float y, float z) native global

; Enable collisions for specific actor
Function EnableActorCollision(Actor akActor) native global

; Disable collisions for specific actor
Function DisableActorCollision(Actor akActor) native global

; Snap actor out of whatever interaction (furniture, etc) they are in
Function ActorStopInteractingQuick(Actor akActor) native global

; Force AI package
Function ActorSetPackageOverride(Actor akActor, Form pkg) native global

; Clear forced AI Package
Function ActorClearPackageOverride(Actor akActor) native global

; Returns whether the actor is currently ignoring combat (e.g. due to package flags)
Bool Function ActorGetIgnoringCombat(Actor akActor) native global

; Force the actor to ignore combat or not, overriding any package flags
Function ActorSetIgnoringCombat(Actor akActor, Bool abIgnoreCombat) native global

; Update actor 3D equipment only (less expensive than QueueUpdate 0xC)
Function ActorUpdateEquipment(Actor akActor) native global

; Update Player Character 3D
Function PlayerUpdateEquipment() native global

;
;	Collision functions
;	===================
;

;	Set the collision state of a reference. Returns the previous state.	TO BE TESTED _ currently fails.
;TBT;	bool Function ObjectReferenceSetCollision(ObjectReference akObject, bool enable=True) native global

;	Get the collision state of a reference. If akObject is None, return the global collision state (controlled by TCL).	TO BE TESTED
;TBT;	bool Function ObjectReferenceGetCollision(ObjectReference akObject) native global

;
;	Cell Functions
;   ==============
;

;	Returns the number of references of type formType in cell. Use GetFormType() below for formType values. If formType is 0 this returns the count of all refs in the cell.
Int Function GetNumRefsInCell(Cell akCell, Int formType) native global

;   Returns nth reference of type formType in cell.  If formType is 0 this returns the nth reference of any type.
ObjectReference Function GetNthRefInCell(Cell akCell, Int index, Int formType) native global

;
;	Misc. Form functions
;	====================
;

;	Returns the Editor ID of a Race. Originally GetFormEditorID, but passing in a form and using the F4SE function GetEditorID() has only worked on Quest and Race forms. So I've just made it for race forms only.
String Function GetRaceEditorID(Race akForm) native global

; Returns the name of the plugin that created a form
String Function OriginalPluginName(Form akForm) native global

; Returns the persistent ID of a form (excluding the load index) Should be compatible with esl files. (Fixed as of v18)
Int Function OriginalPluginID(Form akForm) native global

; Returns whether a form is in a given leveled item list
bool Function GetInLeveledItem(Leveleditem akList, Form akForm) native global

; Return a form's record flags
Int Function GetRecordFlags(Form akForm) native global

; Return a form's formType
Int Function GetFormType(Form akForm) native global

; Return whether form is persistent or not - updated in v51
Bool Function IsPersistent(Form akForm) native global

; Set a form persistent or not. Returns false if form does not exist - updated in v51
Bool Function SetPersistent(Form akForm, bool akFlag) native global

; Return an array of all the forms in a FormList
Form[] Function FormListToArray(FormList akFormList) native global

; Return the form with the given formID from any type of plugin (esm, esl, esp)
Form Function GetFormFromPlugin(String modName, Int formID) native global

; Return an array of forms given an array of plugin names and an array of form IDs
Form[] Function GetFormArray(String[] modNames, Int[] formIDs) native global

; Given an array of FormID Ints that was packed as Var via VarArrayToVar(), return the array of corresponding forms
Form[] Function GetFormArrayFromVar(Var akVar) native global

; Populate a formlist with the forms given an array of plugin names and a corresponding array of form IDs
Function PopulateFormlist(FormList akFormList, String[] modNames, Int[] formIDs) native global

;
;	AAF functions
;	==============
;

; Return an array of actors near akRef who qualify for AAF animations
Actor[] Function AAF_PerformActorScan(ObjectReference akRef, float radius) native global

; Return the actor gender as seen by AAF
Bool Function AAF_GetGender(Actor targetActor) native global

; Enable or disable overriding GetIsSex, SameSex and SameSexAsPC to report AAF_OverrideGender status on NPC.
Bool Function AAF_OverrideGetIsSex(Bool doActivate) native global

; Collects statistics on an actor for use by the Scaleform
Var[] Function AAF_MakeActorData(Actor targetActor, Bool includeDistance, Keyword[] conditionKeywords) native global

; Collects statistics on location objects for use by the Scaleform
Var[] Function AAF_MakeLocationData(ObjectReference target, Bool includeDistance = False) native global

; Returns whether an object is suitable for use as an animation location
Bool Function AAF_IsValidLocation(ObjectReference targetLocation, Var[] actorGroup) native global

; Converts the internal representation of AAF's furniture list to an array of Forms as Var
Var[] Function AAF_ProcessFurnitureList(Var[] akArgs) native global

; Returns an array of LocationData, packed as a Var, of all the valid locations for animations within the given radius from the scan location
Var Function AAF_GetLocationData(int scanLocation, Var[] furnitureForms, Var[] furnitureSources, Var[] actorGroup, Bool quickScan, Float scanRadius) native global

; Returns whether an actor has any keywords in the blocked keywords list
Bool Function AAF_GetBlockedStatus(Actor akActor) native global

; Returns whether an actor is valid for AAF use (not dead, not disabled/deleted, etc.)
Bool Function AAF_IsValidActor(Actor akActor) native global

; Returns whether an actor is valid for use and not currently in an animation
Bool Function AAF_IsAvailableActor(Actor akActor) native global

;
;	Misc functions
;	==============
;

;	Prints a message to the debug console. Exposes the internal F4SE function Console_Print using the code found in Papyrutil in Skyrim.
bool Function PrintConsole(String text) native global

;	Enable or disable blinking. Disabling allows MFG morphs to work on eyelids (morph ID 18 and 41)
Function SetAllowBlinking(Bool allowed) native global

;
;	HIDDEN Functions. Never call directly.
;

; hidden function, use ResizeVarArray instead
Var[] Function ResizeVarArrayInternal(Var[] theArray, int theSize, Var theFill) native global

;
;	Wav function (by Fedim)
;

;	Plays a WAV file. FileName relative to "Data\Sound\Voice\"
;	Option is possible "Data\Sound\Voice\MyPlugin\sound001.wav"
Function PlaySoundWav(string FileName) native global

;	Returns the current volume level of the player (-1 = 0xFFFFFFFF -> max)
;	LowWord	  - left channel volume
;	HeighWorg - right channel volume
;	0 <= volume <= 65535 (0xFFFF)
int Function GetVolumeWav() native global

;	Sets the volume of the player channels
;	0 <= volume <=65535 (0xFFFF)
int Function SetVolumeWav(int volumeA, int volumeB) native global

;	Randomly selects a line from the SectionText section of the "Data\F4SE\Plugins\PluginName.ini" file, plays the WAV and returns the text
;	ini file must be UTF-8 encoded
;	Sounds should be in the "Data\Sound\Voice\PluginName.esp\"
String Function VoiceMessage(String PluginName, String SectionText) native global

;
;	INI setting functions
;	=====================
;
;	Functions to retrieve INI settings, e.g. Fallout4.ini, Fallout4Prefs.ini, Fallout4Custom.ini, mod .ini files in Data folder
;	These functions retrieve the settings from the game engine, not from the INI files themselves.
;	Parameter is the .ini setting name and section, fSettingName:SectionName.  For example, fMinCurrentZoom:Camera
;	Note that typing is strict, e.g. you cannot call GetINIFloat on an Int setting
;
; Return Float setting value or 0.0 if not found
float Function GetINIFloat(string ini) native global

; Return Int setting value or 0 if not found
int Function GetINIInt(string ini) native global

; Return Bool setting value or false if not found
Bool Function GetINIBool(string ini) native global

; Return String setting value or NONE if not found
String Function GetINIString(string ini) native global

;
;	Animation functions
;	===================
;
;	Functions to play animations on actors. They do basic sanity checking on the inputs so that None idles and actors,
;	actors who are deleted or disabled, and actors with no AI middle process will not attempt to play idles.
;
; Make one actor play an idle
Function PlayIdle(Actor akActor, Form akIdle) native global

; Make an array of actors play a corresponding array of idles, in synch
Function PlayMultipleIdles(Actor[] akActor, Form[] akIdle) native global

; Stop the actor's current idle
Function StopCurrentIdle(Actor akActor) native global
