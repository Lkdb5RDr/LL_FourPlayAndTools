Tool to manipulate Hard links between the source directory and the destination directory.

The only useful option is /SPACE to 
	keep history of every version of a game published while using as little space as possible.
	keep in synch multiple copy of the same game: I usually have
		- an unmodified version identical to the steam download at the time of release
		- a version for playing managed by Vortex
		- multiple versions managed by Mod Organizer 2 for different case of testing and or development (or to organize groups of mods).
		- optionally, a copy of the Game Pass version for study.
		- in case of update, and until necessary mods are updated, 
			an additional version lightly modded of the new game version managed by MO2 to test the new version.
			once the necessary mods are available
				this version should be deleted 
				the primary unmodified version synchronized with the steam download.
				all other versions synchronized from the previous one and "SynchroLinked" again.

All other options where for specific cases except maybe /FULL to initialize a new copy of an existing reference.

SynchroLink Source Destination /SPACE 
	will recursively replace any identical file present in both source and destination by a hard link, 
	saving the space needed for the second copy of the file.

SynchroLink Source Destination /FULL
	will
		recursively delete any file in destination not in source
		recursively create hard links in destination of every file in source.


In any case only files of the same name are considered.

