// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class BreakoutMPServerTarget : TargetRules
{
	public BreakoutMPServerTarget( TargetInfo Target) : base(Target)
	{
		Type = TargetType.Server;
		ExtraModuleNames.AddRange( new string[] { "BreakoutMP" } );
	}
}
