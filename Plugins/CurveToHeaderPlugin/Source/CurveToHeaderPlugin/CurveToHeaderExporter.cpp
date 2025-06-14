#include "CurveToHeaderExporter.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "HAL/PlatformFilemanager.h"
#include "Interfaces/IPluginManager.h"

void ACurveToHeaderExporter::ExportCurveToHeader()
{
    if (!Curve || MaxLevel <= 0)
    {
        UE_LOG(LogTemp, Error, TEXT("Invalid curve or MaxLevel."));
        return;
    }

    FString ModuleName = FApp::GetProjectName();
    FString HeaderName = TEXT("PrecomputedXP.h");

    FString PrecomputedFolderPath = FPaths::ProjectDir() / TEXT("Source") / ModuleName / TEXT("Public") / TEXT("PrecomputedValues");
    FString OutputPath = PrecomputedFolderPath / HeaderName;

    IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
    if (!PlatformFile.DirectoryExists(*PrecomputedFolderPath))
    {
        PlatformFile.CreateDirectoryTree(*PrecomputedFolderPath);
    }

    FString Header;
    Header += TEXT("// Auto-generated from curve\n");
    Header += TEXT("#pragma once\n\n");
    Header += TEXT("constexpr float PrecomputedXP[] = {\n");

    for (int32 Level = 1; Level <= MaxLevel; ++Level)
    {
        float XP = Curve->GetFloatValue(Level);
        Header += FString::Printf(TEXT("    %.2ff,\n"), XP);
    }

    Header += TEXT("};\n");
    Header += FString::Printf(TEXT("constexpr int32 XPLevelCount = %d;\n"), MaxLevel);

    if (FFileHelper::SaveStringToFile(Header, *OutputPath))
    {
        UE_LOG(LogTemp, Display, TEXT("Exported XP header to: %s"), *OutputPath);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to write header to: %s"), *OutputPath);
    }
}
