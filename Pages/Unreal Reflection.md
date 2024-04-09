## Unreal Reflection

### UClass
- UClass는 UObject의 상속 클래스로, 클래스 자체의 메타데이터를 포함합니다.
- Unreal Engine에서는 이를 사용하여 런타임에 타입 정보를 얻고, 동적으로 객체를 생성하는 리플렉션 시스템을 구현합니다.


### 예제
``` c++
// CropoutLevelScriptBase.cpp
// 현재 UE에서 사용중인 모든 Plugin, Module에서 ULevelManager를 상속한 모든 Class 찾기
for(TObjectIterator<UClass> It; It; ++It)
{
    UClass* Class = *It;
    if(Class->IsChildOf(ULevelManager::StaticClass()))
    {
        UE_LOG(LogTemp, Warning, TEXT("Found class: %s"), *Class->GetName());
        LevelManagerMap.Add(Class, NewObject<ULevelManager>(this, Class));
    }
}
```