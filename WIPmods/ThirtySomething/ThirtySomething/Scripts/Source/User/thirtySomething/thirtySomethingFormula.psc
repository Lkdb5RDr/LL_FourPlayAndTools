ScriptName thirtySomething:thirtySomethingFormula extends Quest

GlobalVariable property thirtySomethingActiveStatus auto
GlobalVariable Property thirtySomethingMCMHasLoaded auto
GlobalVariable Property thirtySomethingTargetLevel Auto
GlobalVariable Property thirtySomethingAddExperience Auto
Quest property MQ102 auto const
thirtySomething:thirtySomething_MCM_Script property thirtySomething_MCM_Script Auto

ActorValue property experience auto const
Actor player = None
Int	lastLevel = 1

Function LevelToInternal(int targetLevel = 30)
	if player && (targetLevel < 250)
		int exp = player.GetBaseValue(experience) as int
		int expReq = game.GetXPForLevel(targetLevel)
		if exp < expReq
			exp = expReq - exp;
			game.RewardPlayerXP(exp, true)
		endIf
	endIf
endFunction

Function DoRebuild(Int level)
	player = Game.GetPlayer()
	if lastLevel < 2
		; Count how many special points the player has or has spent
		int currentSpecialPoints = -7 + \
			player.GetBaseValue(Game.GetStrengthAV()) as int + \
			player.GetBaseValue(Game.GetPerceptionAV()) as int + \
			player.GetBaseValue(Game.GetEnduranceAV()) as int + \
			player.GetBaseValue(Game.GetCharismaAV()) as int + \
			player.GetBaseValue(Game.GetIntelligenceAV()) as int + \
			player.GetBaseValue(Game.GetAgilityAV()) as int + \
			player.GetBaseValue(Game.GetLuckAV()) as int
		; revert all special to the minimum = 1
		player.SetValue(Game.GetStrengthAV(), 1)
		player.SetValue(Game.GetPerceptionAV(), 1)
		player.SetValue(Game.GetEnduranceAV(), 1)
		player.SetValue(Game.GetCharismaAV(), 1)
		player.SetValue(Game.GetIntelligenceAV(), 1)
		player.SetValue(Game.GetAgilityAV(), 1)
		player.SetValue(Game.GetLuckAV(), 1)
		   
		; Convert special points to perkPoints (It's easier to distribute them in the perk menu )
		Game.AddPerkPoints( currentSpecialPoints )
	endIf
	
	if level > 0
		Game.AddPerkPoints( level )
	endIf
	Debug.Trace(Self + ": Leveled up to " + player.GetLevel() + " with " + level + " extra perk points to spend.")
	lastLevel = player.GetLevel()
endFunction

Int Function LevelTo()
	int target = 30
	if thirtySomethingTargetLevel
		target = thirtySomethingTargetLevel.GetValue() as int
	endIf
	int levels = 0
	if thirtySomethingAddExperience
		levels = thirtySomethingAddExperience.GetValueInt()
	endIf
	if player.GetLevel() < target
		LevelToInternal(target)
		DoRebuild(levels)
	endIf
	return target
endFunction

Function DoSomething()
	if player == None
		player = game.GetPlayer()
	endIf
	lastLevel = player.GetLevel()
	LevelTo()
endFunction

thirtySomething:thirtySomething_player Property apiPlayer auto

Event OnInit()
	RegisterForCustomEvent(apiPlayer, "OnLoadedGame")
	StartTimer(10, 999)
endEvent

Event thirtySomething:thirtySomething_player.OnLoadedGame(thirtySomething:thirtySomething_player akSender, Var[] akArgs)
	StartTimer(10, 999)
endEvent

GlobalVariable StartMeUp = None

Event OnTimer(int TID)
	if TID == 999
		thirtySomething_MCM_Script.UpdateMCMSettings()
		StartMeUp = Game.GetFormFromFile(0x0202B6D6, "StartMeUp.esp") as GlobalVariable
		if ((StartMeUp && StartMeUp.GetValueInt() > 0) || (MQ102.GetStage()>=10)) && !(thirtySomethingActiveStatus.GetValue() as int == 0)
			DoSomething()
		endIf
		StartTimer(10, 999)
		return
	endIf
endEvent
