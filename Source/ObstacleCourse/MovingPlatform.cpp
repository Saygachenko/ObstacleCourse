// Fill out your copyright notice in the Description page of Project Settings.


#include "MovingPlatform.h"

// Sets default values
AMovingPlatform::AMovingPlatform()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMovingPlatform::BeginPlay()
{
	Super::BeginPlay();
	
	StartLocation = GetActorLocation(); // ������ ����(���� �� �������).��� ��������� ���������� X = -15700.0, Y = -2720.0, Z = 4690.0
}

// Called every frame
void AMovingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Move platform forwards
		// Get current location
	FVector CurrentLocation = GetActorLocation(); // ������ ����(���� �� �������). ��� ��������� ���������� X = -15700.0, Y = -2720.0, Z = 4690.0
		// Add vector to that location
	CurrentLocation = CurrentLocation + (PlatformVelocity * DeltaTime); // ������� ������� = -15700.0, Y = -2720.0, Z = 4690.0 + (100 * 0.0166 = 1.66 ������ �� ���). 0.0166 - ��� ��� �������� ����� ������� (1000/16.6 = 60.2 ������ � �������). ���������� ����� ��� ������ 100 ������ �� 1 �������, ��� ����� 60.2 ����. �� ��������� � ����� ��� �� �� ��� X � ��� � ��������� 60
	SetActorLocation(CurrentLocation); // ����� ��������� *����������* (����� ���������� ��������� ������������ ���� ��� �������� ������������)

	// Send platform back if gone too far
		// Check how far we've moved
	const float DistanceMoved = FVector::Dist(StartLocation, CurrentLocation); // V1 = (X = -15700.0, Y = -2720.0, Z = 4690.0) V2 = (X = -15698.34, Y = -2720.0, Z = 4690.0). (-15700.0 - -15698.34)^2 + (-2720.0 - -2720.0)^2 + (4690.0 - 4690.0)^2 = ��������� ����� ��������� (���������� �� �������� ������ ������, ����� �������� ���� �� ���� ). ������ CurrentLocation �������� ���������� �� ��� � � ��������� ��� �����������
		// Reverse direction of motion of gone too far
	if (DistanceMoved > MovedDistance) // ���� ����������� �������� ���� �� ��� X. ���� ��� ��� ���� ������ �� ��������� ����� ��� �� ���������� � MovedDistance (100) �����...
	{
		FString NameTraps = GetName();
		float OverShot = DistanceMoved - MovedDistance; // ����� �� ������� ��� ��� �������� ���������
		UE_LOG(LogTemp, Display, TEXT("%s Platform overshot by: %f"), *NameTraps, OverShot); // ������� �������� � ����
		FVector MoveDirection = PlatformVelocity.GetSafeNormal(); // ��������� ����� ��������� �������� ������� X = 100.0, Y = 0, Z = 0. (100.0)^2 + (0)^2 + (0)^2 = 10000.0 ��� �� ����� 1 (������ �� �� ������������) � �� ������ 1.e-8f(�� 0), ������ ��������� Scale - 1/10000.0 = 1/100.0 = 0.01 � ������ ����� ������ X * Scale, Y * Scale, Z * Scale. X = 100.0 * 0.01 = 1.0, Y = 0.0 * 0.01 = 0.0, Z = 0.0 * 0.01 = 0.0
		StartLocation = StartLocation + (MoveDirection * MovedDistance); // �������������� ��������� �������. ��������� ������� + (X1.0, Y0, Z0 * 100) = (X = 100.0, Y = 0, Z = 0) + (X = -15700.0, Y = -2720.0, Z = 4690.0) = (X = -15600.0, Y = -2720.0, Z = 4690.0)
		PlatformVelocity = -PlatformVelocity; // ����������� ����� ���������, ����� ��� ��������� � ��������������� �����������
		SetActorLocation(StartLocation); // ������������� ����� ��������� �������
	}
}

