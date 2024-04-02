## 요소
- GameInstance
- GameMode
- PlayerController
- LevelScriptActor
    - ```C++ 
      ALevelScriptActor* GetCurrentLevelScriptActor()
      {
          UWorld* World = GetWorld(); // 현재 월드를 가져옵니다.
          if (World != nullptr)
          {
              // 현재 월드의 레벨 스크립트 액터를 가져옵니다.
              return World->GetLevelScriptActor();
          }

          return nullptr; // 월드가 없는 경우 nullptr 반환
      }
      ```
