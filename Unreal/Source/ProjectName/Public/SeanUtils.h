// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Core.h"

#if WITH_EDITOR
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Kismet2/KismetEditorUtilities.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Kismet2/CompilerResultsLog.h"
#include "SeanUtils.generated.h"
/**
 *
 */

UCLASS()
class ProjectName_API USeanUtils : public UBlueprintFunctionLibrary
{

    GENERATED_BODY()
public:
    USeanUtils();
    ~USeanUtils();

    UFUNCTION(BlueprintCallable, meta = (DisplayName = "Execute Sample function", Keywords = "MyScriptingLibrary sample test testing"), Category = "MyScriptingLibraryTesting")
    static float MyScriptingLibrarySampleFunction(float Param);

    UFUNCTION(BlueprintCallable, meta = (Keywords = "Python Editor"), Category = "PythonEditor")
    static int32 ApplyInstanceChangesToBlueprint(AActor* Actor);

    UFUNCTION(BlueprintCallable, meta = (Keywords = "Python Editor"), Category = "PythonEditor")
    void AddComponentsToBlueprint(UBlueprint* Blueprint, const TArray<UActorComponent*>& Components);


    UFUNCTION(BlueprintCallable, meta = (Keywords = "Python Editor"), Category = "PythonEditor")
    UActorComponent* AddComponentToActor(UBlueprint* Actor, UClass* Component, FName name, FString parentName);

    UFUNCTION(BlueprintCallable, meta = (Keywords = "Python Editor"), Category = "PythonEditor")
    UActorComponent* GetComponentByNameOrDefault(UBlueprint* Actor, FString cName);

    UFUNCTION(BlueprintCallable, meta = (Keywords = "Python Editor"), Category = "PythonEditor")
    void AttachToComp(UBlueprint* Actor, UStaticMeshComponent* comp, FName socket);

    UFUNCTION(BlueprintCallable, meta = (Keywords = "Python Editor"), Category = "PythonEditor")
    void CompileBlueprint(UBlueprint* Blueprint);
};

#endif