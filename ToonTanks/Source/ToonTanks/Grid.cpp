// Fill out your copyright notice in the Description page of Project Settings.


#include "Grid.h"
#include "RoomGenerator.h"

// Sets default values
AGrid::AGrid()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	

}

// Called when the game starts or when spawned
void AGrid::BeginPlay()
{
	Super::BeginPlay();

	GridArray.Init(0,LenX * LenY);
	ARoomGenerator*  RoomGenerator = GetWorld()->SpawnActor<ARoomGenerator>(RoomGeneratorClass,GetActorLocation(),GetActorRotation());
	if(!RoomGenerator)
	{
		UE_LOG(LogTemp, Error, TEXT("Room Generator is not spawned"));
		return;
	}
	RoomGenerator->SetOwner(this);
	RoomGenerator->Generate();
	DisplayDebugGrid();

}

// Called every frame
void AGrid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


// Called every frame
void AGrid::DisplayDebugGrid()
{
	if (LenX <= 0 || LenY <= 0)
	{
		UE_LOG(LogTemp, Error, TEXT("Grid size is less than or equal to 0"));
		return;
	}

	//FVector2D StartLocation = FVector2D(GetActorLocation().X - (LenX * CellSize) / 2, GetActorLocation().Y - (LenY * CellSize) / 2);
	//FVector CurrentLocation = FVector(StartLocation.X, StartLocation.Y, GetActorLocation().Z);
	FVector StartLocation = GetActorLocation();
	FVector CurrentLocation = GetActorLocation();

	for (int i = 0; i < LenX; i++)
	{
		for (int j = 0; j < LenY; j++)
		{
			FColor DebugColor; 
			if(GridArray[i * LenY + j ] == 0)
			{
				DebugColor = FColor::Emerald;
			}
			else
			{
				DebugColor = FColor::Red;
			}
			DrawDebugBox(GetWorld(), CurrentLocation, FVector(CellSize/2, CellSize/2, CellSize/2), DebugColor, true, -1, 0, 5);			
			CurrentLocation.Y += CellSize;
		}
		CurrentLocation.Y = StartLocation.Y;
		CurrentLocation.X += CellSize;
	}
}

FVector2D AGrid::GetGridRowCol() const
{
	return FVector2D(LenX - 1, LenY - 1);	
}

void AGrid::PlaceRoomAtCoordinate(int X, int Y)
{
	GridArray[(X) * LenY + (Y)] = 1;
	FVector SpawnLocation = GetActorLocation() + FVector(X * CellSize, Y * CellSize, 0);
	AActor*  Room = GetWorld()->SpawnActor<AActor>(RoomClass,SpawnLocation,GetActorRotation());
	if(!Room)
	{
		UE_LOG(LogTemp, Error, TEXT("Room is not spawned"));
		return;
	}
}


int AGrid::GetOccupiedCell(int X, int Y) const
{
	return GridArray[(X) * LenY + (Y)];
}
