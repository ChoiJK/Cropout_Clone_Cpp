## UObject, UClass

### UObject
- UObject는 Unreal Engine의 모든 클래스의 기본 클래스입니다.
- 모든 게임 엔진의 객체는 'UObject'를 상속받아야합니다. 이를 통해 엔진이 메모리 관리, 직렬화, 가비지 컬렉션 등의 기능을 제공할 수 있습니다.

### UClass
- UClass는 UObject의 상속 클래스로, 클래스 자체의 메타데이터를 포함합니다.
- Unreal Engine에서는 이를 사용하여 런타임에 타입 정보를 얻고, 동적으로 객체를 생성하는 리플렉션 시스템을 구현합니다.

### UClass는 어떨때 사용하는가?
- 만약 Blueprint 객체를 C++에서 Spawn하려면 Blueprint의 UClass를 알아야합니다.
- 그리고 이 UClass를 이용하여 Blueprint Actor를 Spawn할수있습니다.
``` c++
// Blueprint 객체를 Load할때 해당 객체의 레퍼런스
UClass* BPActorClass = LoadClass<AActor>("/Game/Blueprint/BP_Actor.BP_Actor_C");
if(BPActorClass != nullptr)
{
    GetWorld()->SpawnActor(BPActorClass);
}
```

- 위 예시는 UClass를 통해 Actor를 Spawn합니다.
- [Unreal Reflection 페이지](./Unreal%20Reflection.md)를 통해 UClass를 이용한 Reflection활용방법을 확인하실 수 있습니다.

#### 순수가상함수
``` c++
UCLASS(Abstract)
class MYPROJECT_API UAnimal : public UObject
{
    GENERATED_BODY()

public:
    // 순수가상함수 선언
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Animal")
    void MakeSound();
    
    virtual void MakeSound_Implementation() PURE_VIRTUAL(UAnimal::MakeSound, );

    // 일반 함수 선언 및 구현
    UFUNCTION(BlueprintCallable, Category = "Animal")
    void Sleep()
    {
        UE_LOG(LogTemp, Warning, TEXT("Animal is sleeping"));
    }
}
```