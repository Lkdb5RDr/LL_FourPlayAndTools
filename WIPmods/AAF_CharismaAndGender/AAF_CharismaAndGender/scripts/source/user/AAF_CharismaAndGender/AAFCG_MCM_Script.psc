Scriptname AAF_CharismaAndGender:AAFCG_MCM_Script extends Quest

;; MCM Script and configuration adapted from Animated Fannies

Actor Property PlayerRef Auto Const

GlobalVariable property AAF_Charisma_ActivateOverrideGetIsSex auto
GlobalVariable Property AAF_Charisma_MCMHasLoaded auto

String ThisMod = "AAF_CharismaAndGender"

Event OnQuestInit()
	AAF_Charisma_MCMHasLoaded.SetValue(0)

	If CheckForMCM(true)
		UpdateMCMSettings(true)
	EndIf
	
	RegisterForRemoteEvent(PlayerRef, "OnPlayerLoadGame")

	RegisterForExternalEvent("OnMCMSettingChange|"+ThisMod, "OnMCMSettingChange")
	
	AAF_Charisma_MCMHasLoaded.SetValue(1)
	
	debug.trace("AAFCG report: MCM is ready to use")
EndEvent

Function OnMCMSettingChange(string modName, string id)

	If modName == ThisMod
		UpdateMCMSettings()
	endIF

endFunction

Function UpdateMCMSettings(bool firstLoad = false)
	debug.trace("AAFCG report: updating MCM")

	If LoadSetting(AAF_Charisma_ActivateOverrideGetIsSex,  "fAAFCG_ActivateOverrideGetIsSex:Global",  0.0,   1.0)
		LL_FourPlay.AAF_OverrideGetIsSex(AAF_Charisma_ActivateOverrideGetIsSex.GetValue() as bool)
	EndIf
endFunction

bool Function LoadSetting(GlobalVariable akGlobal, string asSetting, float afMin, float afMax)
	if akGlobal
		float settingBuffer = MCM.GetModSettingFloat(ThisMod, asSetting)
	
		If settingBuffer != akGlobal.GetValue() ;1.15 - If the value changed

			If settingBuffer <= afMax && settingBuffer >= afMin ; The setting is within the range of values 
	                   
				akGlobal.SetValue(settingBuffer)
	
			ElseIf settingBuffer < afMin ; The setting is improperly less than the minimum
	
				akGlobal.SetValue(afMin)
		
				MCM.SetModSettingFloat(ThisMod, asSetting, afMin)
	
			ElseIf settingBuffer > afMax ; The setting is improperly greater than the max
	
				akGlobal.SetValue(afMax)
		
				MCM.SetModSettingFloat(ThisMod, asSetting, afMax)
	
			EndIf
			Return True
		Else
			Return False
		EndIf
	endIF
EndFunction

Event Actor.OnPlayerLoadGame(Actor akSender)
	If CheckForMCM()
		UpdateMCMSettings()
	EndIf
EndEvent

bool Function CheckForMCM(bool FirstLoad = false)

	If !MCM.IsInstalled()
	        debug.trace("AAFCG report: no MCM found")
		If FirstLoad			
			Utility.Wait(1.0)
			DEBUG.Notification("AAF_CharismaAndGender: Please install Mod Configuration Menu.")
			
		Else
			DEBUG.MessageBox("AAF_CharismaAndGender: Please re-install Mod Configuration Menu.")		
		EndIf
		
		Return False
	
	EndIf
        debug.trace("AAFCG report: MCM found")
	Return True

EndFunction


