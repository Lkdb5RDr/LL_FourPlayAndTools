Scriptname thirtySomething:thirtySomething_MCM_Script extends Quest

;; MCM Script and configuration adapted from Animated Fannies

Actor Property PlayerRef Auto Const

GlobalVariable property thirtySomethingActiveStatus auto
GlobalVariable Property thirtySomethingMCMHasLoaded auto
GlobalVariable Property thirtySomethingTargetLevel Auto
GlobalVariable Property thirtySomethingAddExperience Auto

String ThisMod = "ThirtySomething"

Event OnQuestInit()
	thirtySomethingMCMHasLoaded.SetValue(0)

	If CheckForMCM(true)
		UpdateMCMSettings(true)
	EndIf
	
	RegisterForRemoteEvent(PlayerRef, "OnPlayerLoadGame")

	RegisterForExternalEvent("OnMCMSettingChange|"+ThisMod, "OnMCMSettingChange")
	
	thirtySomethingMCMHasLoaded.SetValue(1)
	
	debug.trace("TS report: MCM is ready to use")
EndEvent

Function OnMCMSettingChange(string modName, string id)

	If modName == ThisMod
		UpdateMCMSettings()
	endIF

endFunction

Function UpdateMCMSettings(bool firstLoad = false)
	debug.trace("TS report: updating MCM")

	;; LoadSetting(thirtySomethingActiveStatus,  "thirtySomethingActiveStatus:Global",  0.0,   1.0)
	;; LoadSetting(thirtySomethingTargetLevel,   "thirtySomethingTargetLevel:Global",   1.0, 250.0)
	;; LoadSetting(thirtySomethingAddExperience, "thirtySomethingAddExperience:Global", 0.0, 250.0)
endFunction

Function LoadSetting(GlobalVariable akGlobal, string asSetting, float afMin, float afMax)
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
	        debug.trace("TS report: no MCM found")
		If FirstLoad			
			Utility.Wait(1.0)
			DEBUG.Notification("TS ThirtySomething: Please install Mod Configuration Menu.")
			
		Else
			DEBUG.MessageBox("TS ThirtySomething: Please re-install Mod Configuration Menu.")		
		EndIf
		
		Return False
	
	EndIf
        debug.trace("TS report: MCM found")
	Return True

EndFunction


