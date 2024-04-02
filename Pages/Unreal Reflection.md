## Unreal Reflection

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