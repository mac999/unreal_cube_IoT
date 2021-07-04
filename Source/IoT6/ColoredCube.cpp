// Fill out your copyright notice in the Description page of Project Settings.


#include "ColoredCube.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInterface.h"

#pragma optimize("", off)

class SerialPortInstance
{
public:
	SerialPortInstance();
	~SerialPortInstance();

	SerialPort _port;
};

SerialPortInstance::SerialPortInstance()
{
	_port.open(_T("COM3"));
}

SerialPortInstance::~SerialPortInstance()
{
	_port.close();
}


SerialPortInstance _serial;

// Sets default values
AColoredCube::AColoredCube()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AColoredCube::BeginPlay()
{
	Super::BeginPlay();

	auto Cube = FindComponentByClass<UStaticMeshComponent>();
	auto Material = Cube->GetMaterial(0);

	_DynamicMaterial = UMaterialInstanceDynamic::Create(Material, NULL);
	Cube->SetMaterial(0, _DynamicMaterial);

	_randomColor = FMath::Rand() % 64;
}

// Called every frame
void AColoredCube::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TArray<uint8> Buffer;
	Buffer.Add(0);
	int len = _serial._port.read(Buffer);
	if (len)
	{
		FString string = FString::Printf(TEXT("Data = %d"), Buffer[0]);
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, string);

		float blend = ((float)Buffer[0] + _randomColor) / (255.0);
		// float blend = 0.5f + FMath::Cos(GetWorld()->TimeSeconds) / 2;
		_DynamicMaterial->SetScalarParameterValue(TEXT("Blend"), blend);
	}
}

#pragma optimize("", on)