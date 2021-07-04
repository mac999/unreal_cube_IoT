// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SerialPort.h"
#include "ColoredCube.generated.h"

class UMaterialInstanceDynamic;

UCLASS()
class IOT6_API AColoredCube : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AColoredCube();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UMaterialInstanceDynamic* _DynamicMaterial;
	int _randomColor = 0;
};
