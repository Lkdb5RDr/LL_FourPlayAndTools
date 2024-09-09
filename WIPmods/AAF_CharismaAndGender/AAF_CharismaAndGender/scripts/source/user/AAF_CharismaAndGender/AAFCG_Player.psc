Scriptname AAF_CharismaAndGender:AAFCG_Player extends ReferenceALias 

CustomEvent OnLoadedGame

Event OnPlayerLoadGame()
	SendCustomEvent("OnLoadedGame", None)
endEvent
