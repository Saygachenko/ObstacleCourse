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
	
	StartLocation = GetActorLocation(); // Пример куба(одна из ловушек).Его начальные координаты X = -15700.0, Y = -2720.0, Z = 4690.0
}

// Called every frame
void AMovingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	MovePlatform(DeltaTime);
	RotatePlatform(DeltaTime);

}

void AMovingPlatform::MovePlatform(float DeltaTime)
{
	if (ShouldPlatforReturn()) // Наше ограничения движения куба по оси X. Если наш Куб ушёл дальше от стартовой точки чем мы ограничили в MovedDistance (100) тогда...
	{
		//float OverShot = DistanceMoved - MovedDistance; // узнаём на сколько наш куб привысил дистанцию
		//UE_LOG(LogTemp, Display, TEXT("%s Platform overshot by: %f"), *GetBane(), OverShot); // выводим значение в логи
		FVector MoveDirection = PlatformVelocity.GetSafeNormal(); // Вычисляем сумму квадратов скорости вектора X = 100.0, Y = 0, Z = 0. (100.0)^2 + (0)^2 + (0)^2 = 10000.0 оно не равно 1 (значит он не нормализован) и не меньше 1.e-8f(не 0), значит вычисляем Scale - 1/10000.0 = 1/100.0 = 0.01 и создаём новый вектор X * Scale, Y * Scale, Z * Scale. X = 100.0 * 0.01 = 1.0, Y = 0.0 * 0.01 = 0.0, Z = 0.0 * 0.01 = 0.0
		StartLocation = StartLocation + (MoveDirection * MovedDistance); // Перезаписываем стартовую локацию. Стартовая локация + (X1.0, Y0, Z0 * 100) = (X = 100.0, Y = 0, Z = 0) + (X = -15700.0, Y = -2720.0, Z = 4690.0) = (X = -15600.0, Y = -2720.0, Z = 4690.0)
		PlatformVelocity = -PlatformVelocity; // Инвертируем длину платформы, чтобы она двигалась в противоположном направлении
		SetActorLocation(StartLocation); // Устанавливаем новую стартовую локацию
	}
	else
	{
		FVector CurrentLocation = GetActorLocation(); // Пример куба(одна из ловушек). Его начальные координаты X = -15700.0, Y = -2720.0, Z = 4690.0
		CurrentLocation = CurrentLocation + (PlatformVelocity * DeltaTime); // Текущая локация = -15700.0, Y = -2720.0, Z = 4690.0 + (100 * 0.0166 = 1.66 юнитов за тик). 0.0166 - это моя задержка между кадрами (1000/16.6 = 60.2 кадров в секунду). Получается чтобы нам пройти 100 юнитов за 1 секунду, нам нужно 60.2 тика. По умолчанию я делаю это всё по оси X и ФПС я ограничил 60
		SetActorLocation(CurrentLocation); // Будет двигаться *бесконечно* (будет ограничено размерами виртуального мира или игрового пространства)
	}
}

void AMovingPlatform::RotatePlatform(float DeltaTime)
{
	// FRotator CurrentRotation = GetActorRotation();
	// CurrentRotation = CurrentRotation + (RotationVelocity * DeltaTime);
	// SetActorRotation(CurrentRotation);
	AddActorLocalRotation(RotationVelocity * DeltaTime);
}

bool AMovingPlatform::ShouldPlatforReturn() const
{
	return GetDistanceMoved() > MovedDistance;
}

float AMovingPlatform::GetDistanceMoved() const
{
	return FVector::Dist(StartLocation, GetActorLocation()); // V1 = (X = -15700.0, Y = -2720.0, Z = 4690.0) V2 = (X = -15698.34, Y = -2720.0, Z = 4690.0). (-15700.0 - -15698.34)^2 + (-2720.0 - -2720.0)^2 + (4690.0 - 4690.0)^2 = дистанция между векторами(формула длины вектора) (изначально мы отнимаем внутри скобок, потом умножаем саму на себя ). Вектор CurrentLocation движется бесконечно по оси Х у дистанции нет ограничения;
}

