#include "MasterGrid.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Runtime/Core/Public/Misc/AssertionMacros.h"
#include "Tile.h"

// Sets default values
AMasterGrid::AMasterGrid()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	InitializeGrid();
}

// Called when the game starts or when spawned
void AMasterGrid::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AMasterGrid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMasterGrid::InitializeGrid(){
	Tiles.Init(FGridTile2DArray(), Rows);
	for (int32 i = 0; i < Rows; i++) {
		Tiles[i].Ar.Init(nullptr,Columns);
	}
	UE_LOG(LogTemp, Warning, TEXT("Grid initialized"));
}

void AMasterGrid::GenerateGrid() {
	UE_LOG(LogTemp, Warning, TEXT("Placing all base tiles"));
	for (int32 i = 0; i < Rows; i++) {
		for (int32 j = 0; j < Columns; j++) {
			ATile* _tile = PlaceTile(i, j);
			Tiles[i].Ar[j] = _tile;
		}
	}
}

void AMasterGrid::CleanGrid() {
	UE_LOG(LogTemp, Warning, TEXT("Cleaning grid"));
	for (int32 i = 0; i < Tiles.Num(); i++) {
		for (int32 j = 0; j < Tiles[i].Ar.Num(); j++) {
			ATile* _tile = Tiles[i].Ar[j];
			if (_tile) {
				Tiles[i].Ar[j]->Destroy();
			}
		}
	}
}

ATile* AMasterGrid::PlaceTile(int32 i, int32 j) {
	ATile* _tile = nullptr;

	if (i == 0 && j == 0) { // Top Right Corner
		_tile = PlaceCornerTile(i, j,ETileOrientation::TopRight);
	}else if (i == 0 && j == (Tiles[i].Ar.Num() - 1)) { // Top Left Corner
		_tile = PlaceCornerTile(i, j, ETileOrientation::TopLeft);
	}else if (i == (Tiles.Num() - 1) && j == 0) { // Bottom Right Corner
		_tile = PlaceCornerTile(i, j, ETileOrientation::BottomRight);
	}else if (i == (Tiles.Num() - 1) && j == (Tiles[i].Ar.Num() - 1)) { // Bottom Left Corner
		_tile = PlaceCornerTile(i, j, ETileOrientation::BottomLeft);
	}else if (i == 0 && j < (Tiles[i].Ar.Num() - 1)) { // Top border
		_tile = PlaceBorderTile(i, j, ETileOrientation::Top);
	}else if (i == (Tiles.Num() - 1) && j > 0) { // Bottom border
		_tile = PlaceBorderTile(i, j, ETileOrientation::Bottom);
	}else if (i > 0 && j == 0) { // Right border
		_tile = PlaceBorderTile(i, j, ETileOrientation::Right);
	}else if (i > 0 && j == (Tiles[i].Ar.Num() - 1)) { // Left border
		_tile = PlaceBorderTile(i, j, ETileOrientation::Left);
	}else { // Inner
		_tile = PlaceInnerTile(i, j);
	}

	return _tile;
}

ATile* AMasterGrid::PlaceInnerTile(int32 i, int32 j) {
	FVector _gridBaseLocation = GetActorLocation();

	FVector _tileLocation(0.0f, 0.0f, 0.0f);
	_tileLocation.X = _gridBaseLocation.X + InnerTile.GetDefaultObject()->Width * i;
	_tileLocation.Y = _gridBaseLocation.Y + InnerTile.GetDefaultObject()->Height * j;
	UE_LOG(LogTemp, Warning, TEXT("Placing inner tile at [%d][%d] = [%f][%f]"), i, j, _tileLocation.X, _tileLocation.Y);
	FRotator _tileRotation(0.0f, 0.0f, 0.0f);
	FActorSpawnParameters SpawnInfo;

	ATile* _spawnedTile = GetWorld()->SpawnActor<ATile>(InnerTile.Get(), _tileLocation, _tileRotation, SpawnInfo);

	_spawnedTile->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);

	FString _tileName = FString::Printf(TEXT("InnerTile[%d][%d]"), i, j);
	_spawnedTile->SetActorLabel(_tileName);

	return _spawnedTile;
}
ATile* AMasterGrid::PlaceBorderTile(int32 i, int32 j, ETileOrientation orientation) {
	FVector _gridBaseLocation = GetActorLocation();

	FVector _tileLocation(0.0f, 0.0f, 0.0f);
	_tileLocation.X = _gridBaseLocation.X + BorderTile.GetDefaultObject()->Width * i;
	_tileLocation.Y = _gridBaseLocation.Y + BorderTile.GetDefaultObject()->Height * j;
	UE_LOG(LogTemp, Warning, TEXT("Placing border tile at [%d][%d] = [%f][%f]"), i, j, _tileLocation.X, _tileLocation.Y);
	FRotator _tileRotation(0.0f, 0.0f, 0.0f);
	FString _tileName;

	switch (orientation) {
	case ETileOrientation::Top:
		_tileRotation.Yaw = 90;
		_tileName = FString::Printf(TEXT("TopBorderTile[%d][%d]"), i, j);
		break;
	case ETileOrientation::Bottom:
		_tileRotation.Yaw = -90;
		_tileName = FString::Printf(TEXT("BottomBorderTile[%d][%d]"), i, j);
		break;
	case ETileOrientation::Left:
		_tileRotation.Yaw = 0;
		_tileName = FString::Printf(TEXT("LeftBorderTile[%d][%d]"), i, j);
		break;
	case ETileOrientation::Right:
		_tileRotation.Yaw = -180;
		_tileName = FString::Printf(TEXT("RightBorderTile[%d][%d]"), i, j);
		break;
	}

	FActorSpawnParameters SpawnInfo;

	ATile* _spawnedTile = GetWorld()->SpawnActor<ATile>(BorderTile.Get(), _tileLocation, _tileRotation, SpawnInfo);
	FVector _origin;
	FVector _tileBounds;
	_spawnedTile->GetActorBounds(false, _origin,_tileBounds);
	UE_LOG(LogTemp, Warning, TEXT("Border Bounds's [%f,%f,%f]"), _tileBounds.X, _tileBounds.Y, _tileBounds.Z);
	_spawnedTile->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
	_spawnedTile->SetActorLabel(_tileName);

	return _spawnedTile;
}
ATile* AMasterGrid::PlaceCornerTile(int32 i, int32 j, ETileOrientation orientation) {
	FVector _gridBaseLocation = GetActorLocation();

	FVector _tileLocation(0.0f, 0.0f, 0.0f);
	_tileLocation.X = _gridBaseLocation.X + CornerTile.GetDefaultObject()->Width * i;
	_tileLocation.Y = _gridBaseLocation.Y + CornerTile.GetDefaultObject()->Height * j;
	UE_LOG(LogTemp, Warning, TEXT("Placing corner tile at [%d][%d] = [%f][%f]"), i, j, _tileLocation.X, _tileLocation.Y);
	FRotator _tileRotation(0.0f, 0.0f, 0.0f);
	FString _tileName;

	switch (orientation) {
	case ETileOrientation::TopLeft:
		_tileRotation.Yaw = 0;
		_tileName = FString::Printf(TEXT("TopLeftCornerTile[%d][%d]"), i, j);
		break;
	case ETileOrientation::TopRight:
		_tileRotation.Yaw = 90;
		_tileName = FString::Printf(TEXT("TopRightCornerTile[%d][%d]"), i, j);
		break;
	case ETileOrientation::BottomLeft:
		_tileRotation.Yaw = -90;
		_tileName = FString::Printf(TEXT("BottomLeftCornerTile[%d][%d]"), i, j);
		break;
	case ETileOrientation::BottomRight:
		_tileRotation.Yaw = -180;
		_tileName = FString::Printf(TEXT("BottomRightCornerTile[%d][%d]"), i, j);
		break;
	}

	FActorSpawnParameters SpawnInfo;

	ATile* _spawnedTile = GetWorld()->SpawnActor<ATile>(CornerTile.Get(), _tileLocation, _tileRotation, SpawnInfo);
	_spawnedTile->SetActorLabel(_tileName);
	_spawnedTile->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);

	return _spawnedTile;
}

#if WITH_EDITOR  
void AMasterGrid::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	//Get all of our components  
	TArray<UActorComponent*> MyComponents;
	GetComponents(MyComponents);

	//Get the name of the property that was changed  
	FName PropertyName = (PropertyChangedEvent.Property != nullptr) ? PropertyChangedEvent.Property->GetFName() : NAME_None;

	// We test using GET_MEMBER_NAME_CHECKED so that if someone changes the property name  
	// in the future this will fail to compile and we can update it.  
	if ((PropertyName == TEXT("bGenerateGrid")))
	{
		CleanGrid();
		InitializeGrid();
		GenerateGrid();
	}

	// Call the base class version  
	Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif

