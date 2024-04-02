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
StreamableManager.RequestAsyncLoad(TexturePtr, FStreamableDelegate::CreateLambda([TexturePtr]()
{
    UTexture2D* Texture = TexturePtr.Get();
    // 로드 완료 후 처리
}
  ```