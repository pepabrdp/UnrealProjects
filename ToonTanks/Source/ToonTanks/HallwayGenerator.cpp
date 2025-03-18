// Fill out your copyright notice in the Description page of Project Settings.


#include "HallwayGenerator.h"
#include "Grid.h"

// Sets default values
AHallwayGenerator::AHallwayGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AHallwayGenerator::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHallwayGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AHallwayGenerator::GenerateHallways()
{
	AGrid* grid = Cast<AGrid>(GetOwner());
	if(  grid == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Grid is not set"));
		return;
	}

	//Randomly Generate a room within the indexes.
	FVector2D GridSize = grid->GetGridRowCol();
	int GridX = GridSize.X;
	int GridY = GridSize.Y;

	TArray<TTuple<AActor*, AActor*>> MSTFinalEdges = grid->GetMSTFinalEdges();
	float CellSize = grid->GetCellSize();
	for(TTuple<AActor*, AActor*> Elem : MSTFinalEdges)
	{
		if(Elem.Get<0>())
		{
			FVector Room1Location = Elem.Get<0>()->GetActorLocation();
			FVector2D Room1Coordinates = FVector2D(Room1Location.X / CellSize, Room1Location.Y / CellSize);
			UE_LOG(LogTemp,Log,TEXT("Room %s coordinates : (%f, %f)"),*(Elem.Get<0>()->GetFName().ToString())  ,Room1Coordinates.X,Room1Coordinates.Y);
		}
		
		if(Elem.Get<1>())
		{
			FVector Room2Location = Elem.Get<1>()->GetActorLocation();
			FVector2D Room2Coordinates = FVector2D(Room2Location.X / CellSize, Room2Location.Y / CellSize);
			UE_LOG(LogTemp,Log,TEXT("Room %s coordinates : (%f, %f)"),*(Elem.Get<1>()->GetFName().ToString()) ,Room2Coordinates.X,Room2Coordinates.Y);
		}
		

		
		
	}
}
