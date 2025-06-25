#include "CurveToHeaderExporter.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "HAL/PlatformFilemanager.h"

ACurveToHeaderExporter::ACurveToHeaderExporter()
{
#if WITH_EDITOR
    bRunConstructionScriptOnDrag = false;   // we don’t need Tick or a construction script
#endif
}

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

// Helper: make a row name safe for a C++ identifier (“Fire-XP” → “Fire_XP”)
FString ACurveToHeaderExporter::SanitizeIdentifier(const FString& In)
{
    FString Out;
    Out.Reserve(In.Len());
    for (TCHAR C : In)
    {
        Out += (FChar::IsAlnum(C) || C == TEXT('_')) ? C : TEXT('_');
    }
    return Out;
}

void ACurveToHeaderExporter::ExportCurveTableToHeader()
{
    if (!CurveTable || MaxLevel <= 0)
    {
        UE_LOG(LogTemp, Error, TEXT("Invalid curve table or MaxLevel."));
        return;
    }

    // Paths
    const FString ModuleName      = FApp::GetProjectName();
    const FString HeaderName      = TEXT("PrecomputedXPTable.h");
    const FString PrecomputedPath = FPaths::ProjectDir() / TEXT("Source") / ModuleName /
                                    TEXT("Public") / TEXT("PrecomputedValues");
    const FString OutputPath      = PrecomputedPath / HeaderName;

    IPlatformFile& PF = FPlatformFileManager::Get().GetPlatformFile();
    if (!PF.DirectoryExists(*PrecomputedPath))
    {
        PF.CreateDirectoryTree(*PrecomputedPath);
    }

    // Header
    FString Header;
    Header += TEXT("// Auto-generated from curve table – DO NOT EDIT\n");
    Header += TEXT("#pragma once\n\n");
    Header += FString::Printf(TEXT("constexpr int32 XPLevelCount = %d;\n\n"), MaxLevel);

    // Iterate rows
    int32 RowIndex = 0;
    for (const auto& Pair : CurveTable->GetRowMap())           
    {
        const FName          RowName   = Pair.Key;
        const FRealCurve*    RealCurve = Pair.Value;          
        if (!RealCurve) { continue; }

        const FString SafeName = SanitizeIdentifier(RowName.ToString());

        Header += FString::Printf(TEXT("// Row \"%s\"\n"), *RowName.ToString());
        Header += FString::Printf(TEXT("constexpr float PrecomputedXP_%s[%d] = {\n"),
                                  *SafeName, MaxLevel);

        for (int32 Level = 1; Level <= MaxLevel; ++Level)
        {
            const float XP = RealCurve->Eval(static_cast<float>(Level));
            Header += FString::Printf(TEXT("    %.2ff%s\n"),
                                      XP, Level < MaxLevel ? TEXT(",") : TEXT(""));
        }
        Header += TEXT("};\n\n");

        ++RowIndex;
    }

    Header += FString::Printf(TEXT("constexpr int32 XPRowCount = %d;\n"), RowIndex);

    // Write file 
    if (FFileHelper::SaveStringToFile(Header, *OutputPath))
    {
        UE_LOG(LogTemp, Display, TEXT("Exported XP table header to: %s"), *OutputPath);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to write header to: %s"), *OutputPath);
    }
}
