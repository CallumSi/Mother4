// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBlueprintFunctionLibrary.h"

FString UMyBlueprintFunctionLibrary::GetPointlessMessage()
{
	return FString(TEXT("This is a message, but its rather pointless"));
}



double UMyBlueprintFunctionLibrary::GetMyPi()
{
	return Pi;
}



FAverages UMyBlueprintFunctionLibrary::CalculateAverages(const TArray<int32>& InValues)
{
	return FAverages(1, 2, 3);
}
