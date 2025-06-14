#pragma once

#include "CoreMinimal.h"
#include "EditorUtilityActor.h"
#include "UObject/ObjectMacros.h"
#include "UObject/Object.h"
#include "Curves/CurveFloat.h"
#include "CurveToHeaderExporter.generated.h"

UCLASS(Blueprintable)
class CURVETOHEADERPLUGIN_API ACurveToHeaderExporter : public AEditorUtilityActor
{
    GENERATED_BODY()

public:

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XP Export")
    UCurveFloat* Curve;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XP Export")
    int32 MaxLevel = 100;

    UFUNCTION(CallInEditor, Category="XP Export")
    void ExportCurveToHeader();
};