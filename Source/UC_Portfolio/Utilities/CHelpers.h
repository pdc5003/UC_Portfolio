#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"

class UC_PORTFOLIO_API CHelpers
{
public:
	template<typename T>
	static void GetAsset(T** OutObject, FString InPath)
	{
		ConstructorHelpers::FObjectFinder<T> asset(*InPath);
		verifyf(asset.Succeeded(), L"asset.Succeeded()");

		*OutObject = asset.Object;
	}

	template<typename T>
	static void GetAssetDynamic(T** OutObject, FString InPath)
	{
		T* obj = Cast<T>(StaticLoadObject(T::StaticClass(), NULL, *InPath));
		verifyf(!!obj, L"!!obj");

		*OutObject = obj;
	}


	template<typename T>
	static void GetClass(TSubclassOf<T>* OutObject, FString InPath)
	{
		ConstructorHelpers::FClassFinder<T> asset(*InPath);
		verifyf(asset.Succeeded(), L"asset.Succeeded()");

		*OutObject = asset.Class;
	}



	/*template <typename ObjClass>
	static FORCEINLINE ObjClass* LoadBlueprintFromPath(const FName& Path, const FName& BlueprintName)
	{
		if (Path == NAME_None) return NULL;
		FString cName = BlueprintName.ToString().Append(FString("_C"));
		TArray<UObject*> tempArray;
		if (EngineUtils::FindOrLoadAssetsByPath(*Path.ToString(), tempArray, EngineUtils::ATL_Class))
		{
			for (int i = 0; i < tempArray.Num(); ++i)
			{
				UObject* temp = tempArray[i];
				if (temp == NULL || (!Cast<ObjClass>(temp)) || (temp->GetName().Compare(cName) != 0))
				{
					continue;
				}

				return Cast<ObjClass>(temp);
			}
		}

		return NULL;
	}*/



	template<typename T>
	static void CreateComponent(AActor* InActor, T** InComponent, FName InName, USceneComponent* InParent = NULL)
	{
		*InComponent = InActor->CreateDefaultSubobject<T>(InName);

		if (!!InParent)
		{
			(*InComponent)->SetupAttachment(InParent);

			return;
		}
			
		InActor->SetRootComponent((*InComponent));
	}

	template<typename T>
	static void CreateActorComponent(AActor* InActor, T** InComponent, FName InName)
	{
		*InComponent = InActor->CreateDefaultSubobject<T>(InName);
	}

	template<typename T>
	static T* GetComponent(AActor* InActor)
	{
		return Cast<T>(InActor->GetComponentByClass(T::StaticClass()));
	}
};