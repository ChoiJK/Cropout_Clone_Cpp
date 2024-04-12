## C++에서의 Resource Load

Unreal에서 C++로 개발할 때 Resource가 필요하다면 다양한 경로를 통해 취득할 수 있습니다.
크게 3가지 범주로 볼수 있습니다.   

1. 빌드시간에 로드
2. 런타임에 로드 (Sync)
3. 런타임에 로드 (Async)

### 빌드시간에 로드
- UObject를 상속받은 Class의 생성자에서 활용할 수 있습니다.
- 컴파일타임에 리소스의 존재와 유효성을 검사하여, 리소스 경로 오류를 빠르게 식별할 수 있습니다.

```C++
static ConstructorHelpers::FObjectFinder<ResourceType> resource(TEXT("ResourcePath"));
if(resource.Succeeded())
{
    // Resource 활용
}

static ConstructorHelpers::FClassFinder<ResourceType> resource(Text("ResourcePath"));
if(resource.Succeeded())
{
    UClass* resourceClass = resource.Class;
    //resourceClass 활용
}
```

### 런타임에 로드 (Sync)
- 런타임 언제든지 활용할 수 있습니다.
- 다양한 유형의 리소스와 객체를 동적으로 로드하는 데 유용합니다.
- 실행 중에 리소스를 로드하므로, 호출될 때마다 성능에 영향을 줄 수 있습니다.

```C++
// StaticLoadObject를 활용한 Sync Object Load
UObject* loadObj = StaticLoadObject(ResourceType::StaticClass(), nullptr, Text("ResourcePath"));
if(IsValid(loadObj))
{
    ResourceType* resource = Cast<ResourceType>(loadObj);
    // resource 활용
}

// StaticLoadClass를 활용한 Sync Class Load
UClass* loadClass = StaticLoadClass(BaseResourceType::StaticClass(), nullptr, Text("ResourcePath"));
if(IsValid(loadClass))
{
    // loadClass 활용
}

// FStreamableManager를 활용한 Sync Object Load
FStreamableManager streamableManager;
streamableManager.LoadSynchronous(FSoftObjectPath)
```

### 런타임에 로드 (Async)
- 게임플레이에 방해를 주지 않고 Background에서 Resource를 Load할 수 있습니다.
- AsyncLoad는 FSoftObjectPtr, FSoftObjectPath를 통해 활용할 수 있습니다.
- AsyncLoad는 조금은 주의하여 사용해야합니다.
    - 매우 큰 리소스에 대해 AsyncLoad를 요청하였으나 Load가 완료되기 전 Obj가 사라지는 이슈
    - 리소스를 AsyncLoad했으나 Tick에서 아직 Load되지 않은 Resource를 활용하는 이슈

```C++
// Input TArray<FSoftObjectPtr<UStsticMesh>> MeshList
TArray<FSoftObjectPath> MeshPathList;
for(const TSoftObjectPtr<UStaticMesh>& mesh : MeshList)
{
    MeshPathList.Add(mesh.ToSoftObjectPath());
}

// MeshPathList의 Load가 완료되면 Callback을 통해 알려줍니다.
// Callback은 다양한 방법을 사용할 수 있습니다. (ex. Lambda, Delegate, TFunction, etc..)
// 아래 예시는 Lambda를 활용합니다.
FStreamableManager streamableManager;
streamableManager.RequestAsyncLoad(MeshPathList, RequestAsyncLoad(MeshPathList,
		FStreamableDelegate::CreateLambda([MeshList, MeshPathList, this]()
		{
            // MeshPathList(FSoftObjectPath)의 Load가 완료되면 
            // MeshList(TSoftObjectPtr)를 통해 resoruce를 활용할 수 있습니다.
            for(const TSoftObjectPtr<UStaticMesh>& mesh : MeshList)
			{
                // 현재 Class의 MeshList에 Load한 Resource를 보관합니다.
				this->MeshList.Add(mesh.Get());
			}

            // MeshPathList(FSoftObjectPath)의 Load가 완료되면 
            // MeshPathList(FSoftObjectPath)를 통해 resoruce를 활용할 수 있습니다.
            for(FSoftObjectPath meshPath : MeshPathList)
			{
                // 현재 Class의 MeshList에 Load한 Resource를 보관합니다.
				this->MeshList.Add(Cast<UStaticMesh>(meshPath.TryLoad()));
			}
        }));

```

> 이외에도 UAssetManager, Blueprints에서의 Resource Load 등 여러가지 Resource Load 방법이 있습니다.
