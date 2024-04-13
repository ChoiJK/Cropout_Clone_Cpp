# Actor & ActorComponent


## 주의
### Nullptr 이슈
- UObject를 상속받아 처리하는 중에 모든 로직이 잘 동작하다가 멤버변수가 갑자기 Nullptr로 설정되는경우
    - Unreal의 가비지 컬렉션(Garbage Collection, GC)시스템은 UObject 파생 클래스의 인스턴스가 더 이상 참조되지 않거나 적절하게 처리되지 않는 경우, 이를 메모리에서 해제할 수 있습니다.
    - 인스턴스에 대한 참조를 유지하기위해 `UPROPERTY()` 매크로를 사용하세요.
    - `UPROPERTY()`는 UObject 파생 객체를 가비지 컬렉터가 추적하는데도 사용합니다.