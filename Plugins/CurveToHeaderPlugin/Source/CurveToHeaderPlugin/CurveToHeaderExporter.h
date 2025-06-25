// CurveToHeaderExporter.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CurveToHeaderExporter.generated.h"

// Forward declarations instead of heavy header includes.
class UCurveFloat;
class UCurveTable;

/**
 * Utility Actor that converts Curve assets into constexpr C++ header files
 * so XP (or any numeric progression) can be fetched without loading assets
 * at runtime.  Place the actor in-editor or call the functions from a
 * Blutility / commandlet.
 */

UCLASS(Blueprintable)
class CURVETOHEADERPLUGIN_API ACurveToHeaderExporter : public AActor
{
    GENERATED_BODY()

public:
    ACurveToHeaderExporter();

    /** Export a single UCurveFloat into "PrecomputedXP.h" */
    UFUNCTION(CallInEditor, BlueprintCallable, Category = "Curve Export")
    void ExportCurveToHeader();

    /** Export every row in a UCurveTable into "PrecomputedXPTable.h" */
    UFUNCTION(CallInEditor, BlueprintCallable, Category = "Curve Export")
    void ExportCurveTableToHeader();

    /** Curve sampled when using ExportCurveToHeader() */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Curve Export")
    TObjectPtr<UCurveFloat> Curve = nullptr;

    /** CurveTable sampled when using ExportCurveTableToHeader() */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Curve Export")
    TObjectPtr<UCurveTable> CurveTable = nullptr;

    /** Highest level (inclusive) to sample from the curve(s) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Curve Export", meta = (ClampMin = "1"))
    int32 MaxLevel = 100;

private:
    /** Make a row name safe for use as a C++ identifier */
    static FString SanitizeIdentifier(const FString& In);
};