// Fill out your copyright notice in the Description page of Project Settings.


#include "RoomGenerator.h"
#include "Grid.h"


// Sets default values
ARoomGenerator::ARoomGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ARoomGenerator::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ARoomGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARoomGenerator::SetGrid(AGrid* grid)
{
	Grid = grid;
}

void ARoomGenerator::Generate()
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
	
	//While not all rooms are generated keep generating
	while(NumRooms > 0)
	{
		int X = FMath::RandRange(0, GridX);
		int Y = FMath::RandRange(0, GridY);
		//Check if the room is already generated
		if(grid->GetOccupiedCell(X,Y) == 0)
		{
			grid->PlaceRoomAtCoordinate(X,Y);
			//Decrement the number of rooms
			NumRooms--;
		}
	}
	

}

