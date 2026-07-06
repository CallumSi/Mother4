// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MyBlueprintFunctionLibrary.generated.h"

USTRUCT(BlueprintType)
struct FAverages
{
	GENERATED_BODY()

	FAverages() :
		Mean(0.0f),
		Modal(0.0f),
		Median(0.0f) {};

	FAverages(float InMean, float InMode, float InMedian) :
		Mean(InMean),
		Modal(InMode),
		Median(InMedian) {};

	UPROPERTY(BlueprintReadOnly)
		float Mean = 0.0f;
	UPROPERTY(BlueprintReadOnly)
		float Modal = 0.0f;
	UPROPERTY(BlueprintReadOnly)
		float Median = 0.0f;
};

UCLASS()
class MYPROJECT_API UMyBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	public:
		UFUNCTION(BlueprintCallable)
		static FString GetPointlessMessage();

		UFUNCTION(BlueprintCallable,
			BlueprintPure,
			meta = (ToolTip = "3.14....",
			CompactNodeTitle = "pie"))
			static double GetMyPi();

		UFUNCTION(BlueprintCallable,
				meta = (ToolTip = "Calculate the mean mode and median for an array of interger values."))
		static FAverages CalculateAverages(const TArray<int32>& InValues);


	protected:

	private:
		static constexpr double Pi = 3.141592;
};
