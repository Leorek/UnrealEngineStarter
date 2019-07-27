#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MasterGrid.generated.h"

UENUM(BlueprintType)
enum class ETileOrientation : uint8
{
	Top 	 UMETA(DisplayName = "Top"),
	TopLeft UMETA(DisplayName = "Top"),
	TopRight UMETA(DisplayName = "Top"),
	Bottom 	 UMETA(DisplayName = "Bottom"),
	BottomLeft 	 UMETA(DisplayName = "Bottom"),
	BottomRight 	 UMETA(DisplayName = "Bottom"),
	Left	 UMETA(DisplayName = "Left"),
	Right	 UMETA(DisplayName = "Right")
};

USTRUCT()
struct FGridTile2DArray {
	GENERATED_BODY()
public:

	TArray<class ATile*> Ar;

	ATile* operator[] (int32 i) {
		return Ar[i];
	}

	void Add(ATile* rune) {
		Ar.Add(rune);
	}
};

UCLASS()
class GAMEJAM_API AMasterGrid : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMasterGrid();
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings")
	bool bGenerateGrid = false;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings")
	int32 Rows = 3;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings")
	int32 Columns = 3;

	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category="Tiles")
	TSubclassOf<class ATile> CornerTile;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Tiles")
	TSubclassOf<class ATile> BorderTile;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Tiles")
	TSubclassOf<class ATile> InnerTile;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
#if WITH_EDITOR  
	void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
	TArray<FGridTile2DArray> Tiles;
	void InitializeGrid();
	void CleanGrid();
	class ATile* PlaceTile(int32 i, int32 j);
	class ATile* PlaceInnerTile(int32 i, int32 j);
	class ATile* PlaceBorderTile(int32 i, int32 j, ETileOrientation orientation);
	class ATile* PlaceCornerTile(int32 i, int32 j, ETileOrientation orientation);


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void GenerateGrid();

};
