// Fill out your copyright notice in the Description page of Project Settings.


#include "SeanUtils.h"

#if WITH_EDITOR
#include "Kismet2/KismetEditorUtilities.h"
#include "KismetCompilerModule.h" 
#include "AssetRegistry/AssetRegistryModule.h"
#include <SSCSEditor.h>


void USeanUtils::AddComponentsToBlueprint(UBlueprint* Blueprint, const TArray<UActorComponent*>& Components
) {
    FKismetEditorUtilities::FAddComponentsToBlueprintParams Params;
    return FKismetEditorUtilities::AddComponentsToBlueprint(Blueprint, Components, Params);
}

int32 USeanUtils::ApplyInstanceChangesToBlueprint(AActor* Actor)
{
    return FKismetEditorUtilities::ApplyInstanceChangesToBlueprint(Actor);
}


USCS_Node* GetNodeByClass(UBlueprint* Actor, FString cName) {
    UBlueprintGeneratedClass* bp = Cast<UBlueprintGeneratedClass>(Actor);

    USCS_Node* parentNode = nullptr;
    FName nameParent = FName(*cName);
    if (cName.IsEmpty())
    {
        parentNode = Actor->SimpleConstructionScript->GetDefaultSceneRootNode();
    }
    else
    {
        for (USCS_Node* SCSNode : Actor->SimpleConstructionScript->GetAllNodes())
        {
            if (SCSNode && (SCSNode->GetVariableName() == nameParent || (SCSNode->ComponentTemplate && SCSNode->ComponentTemplate->GetFName() == nameParent)))
            {
                parentNode = SCSNode;
            }
        }
    }

    // backup if it is not found in the above
    if (parentNode == nullptr)
    {
        parentNode = Actor->SimpleConstructionScript->GetDefaultSceneRootNode();
    }
    return parentNode;
}

UActorComponent* USeanUtils::GetComponentByNameOrDefault(UBlueprint* Actor, FString cName) {
    UBlueprintGeneratedClass* bp = Cast<UBlueprintGeneratedClass>(Actor);

    USCS_Node* parentNode = nullptr;
    FName nameParent = FName(*cName);
    if (cName.IsEmpty())
    {
        parentNode = Actor->SimpleConstructionScript->GetDefaultSceneRootNode();
    }
    else
    {
        for (USCS_Node* SCSNode : Actor->SimpleConstructionScript->GetAllNodes())
        {
            if (SCSNode && (SCSNode->GetVariableName() == nameParent || (SCSNode->ComponentTemplate && SCSNode->ComponentTemplate->GetFName() == nameParent)))
            {
                parentNode = SCSNode;
            }
        }
    }

    // backup if it is not found in the above
    if (parentNode == nullptr)
    {
        parentNode = Actor->SimpleConstructionScript->GetDefaultSceneRootNode();
    }
    return parentNode->ComponentTemplate;
}

UActorComponent* USeanUtils::AddComponentToActor(UBlueprint* Actor, UClass* Component, FName name, FString parentName)
{
    UBlueprintGeneratedClass* bp = Cast<UBlueprintGeneratedClass>(Actor);

    UClass* component_class = Cast<UClass>(Component);

    Actor->Modify();
    USCS_Node* node = Actor->SimpleConstructionScript->CreateNode(Component, name);
    if (!node)
    {
        return nullptr;
    }

    USCS_Node* parentNode = nullptr;

    if (Component->IsChildOf<USceneComponent>())
    {
        FName nameParent = FName(*parentName);
        if (parentName.IsEmpty())
        {
            parentNode = Actor->SimpleConstructionScript->GetDefaultSceneRootNode();
        }
        else
        {
            parentNode = Actor->SimpleConstructionScript->FindSCSNode(nameParent);
        }
    }

    // backup if it is not found in the above
    if (parentNode == nullptr)
    {
        parentNode = Actor->SimpleConstructionScript->GetDefaultSceneRootNode();
    }

    parentNode->AddChildNode(node);


    return (node->ComponentTemplate);
}

void USeanUtils::CompileBlueprint(UBlueprint* Blueprint)
{
    if (Blueprint)
    {
        EBlueprintCompileOptions Flags = EBlueprintCompileOptions::SkipSave;
        FKismetEditorUtilities::CompileBlueprint(Blueprint, Flags);
    }
}


void USeanUtils::AttachToComp(UBlueprint* Actor, UStaticMeshComponent* comp, FName socket)
{
    auto node = GetNodeByClass(Actor, comp->GetName());
    node->AttachToName = socket;
}

#endif

