### TUniquePtr

### TSharedPtr

### TWeakPtr

### TSoftClassPtr

### TSoftObjectPtr
- Load Synchronous
``` c++
BuildEntityImage->SetBrushFromTexture(TexturePtr.LoadSynchronous());
  ```

- Load Async
``` c++
TSoftObjectPtr<UTexture2D> TexturePtr = TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/Path/To/Texture.TextureName")));

FStreamableManager& StreamableManager = UAssetManager::GetStreamableManager();
// 주의 : 예제들 찾아보면 TSoftObjectPtr을 매개변수로 받는 것 같이 되어있는데 메소드를 직접 확인해보면 TSoftObjectPath를 받음.
StreamableManager.RequestAsyncLoad(TexturePtr.ToSoftObjectPath(), FStreamableDelegate::CreateLambda([TexturePtr]()
{
    UTexture2D* Texture = TexturePtr.Get();
    // 로드 완료 후 처리
}
  ```