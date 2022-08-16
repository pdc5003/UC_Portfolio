using UnrealBuildTool;

public class UC_Portfolio : ModuleRules
{
	public UC_Portfolio(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] 
		{ 
			"Core", 
			"CoreUObject", 
			"Engine", 
			"InputCore",
			"AIModule",
			"GameplayTasks",
			"Niagara",
			"MovieScene",
			"LevelSequence",
			"CinematicCamera",
			"UMG"
		});

		PublicIncludePaths.Add(ModuleDirectory);
	}
}
