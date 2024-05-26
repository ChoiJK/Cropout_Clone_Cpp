## 코드 흐름 소개

> 대표적인 S/W의 흐름에 대해 설명합니다.
> 각 코드의 자세한 사항에 대해서는 설명하지 않습니다.

### Folder 구조 및 역할
- Cropout_Clone_Cpp     
    <details>
    <summary> Core </summary>

    - UE GamePlay Framework를 구현합니다.
        - GameInstance
        - PlayerController
        - GameMode
    - Manager에 대한 관리또한 포함됩니다. (GameInstance)
    
    </details>
    
    <details>
    <summary> ApplicationState </summary>

    - Application의 상태를 관리합니다.
        - Application level의 제어
        - level 전환
        - State 전환시 Process들을 추가하여 여러 연출, 효과 등 안정적인 S/W 플로우 관리 가능
    
    </details>

    <details>
    <summary> Data </summary>

    - 여러파일에서 사용되어질 수 있는 Struct들의 모음입니다.
    - TableRow도 모두 이곳에 있습니다.
    
    </details>
    
    <details>
    <summary> Entity </summary>

    - Level에 생성될 수 있는 모든 Entity입니다.
    - Villager은 BT에 대한 개발도 포함됩니다.
    - 모든 Interactable은 DataTable을 통해 정의됩니다.
    - InteractableInfo를 통해 실제 객체들을 생성합니다.
    
    </details>

    <details>
    <summary> Enum </summary>

    - 여러파일에서 사용되어질 수 있는 Enum들의 모음입니다.
    
    </details>
    
    <details>
    <summary> Global </summary>

    - 초기 개발단계에서 프로젝트의 모든 영역에 활용하기 위해 개발한 Global객체들입니다.
    - GameInstance를 통해 관리됩니다.
    - 추후 리펙토링시 사라질 예정입니다.
    
    </details>
    
    <details>
    <summary> Input </summary>

    - Input Modifier를 구현합니다.
    
    </details>
    
    <details>
    <summary> Manager </summary>

    - LevelManager, SystemManager를 정의합니다.
    - Sound, Table Manager가 구현되어있습니다.
    
    </details>
    
    <details>
    <summary> Player </summary>

    - Player가 실제로 컨트롤할 Pawn가 구현되어있습니다.
    - CropoutPlayer와 그를 구성하는 ActorComponent들로 구성되어있습니다.
    - 이를 이용하여 플레이어의 인풋을 통해 카메라 처리, 객체 배치, Villager의 액션등을 처리 할 수 있습니다.
    
    </details>
    
    <details>
    <summary> SimpleRx </summary>

    - 데이터와 데이터의 변화에 반응하는 시스템을 구축합니다.
    - 가장 심플한 형태의 Rx를 구현합니다.
    - ReactiveProperty, SubscribeHandle, CompositeDisposable로 구현됩니다.
    
    </details>
    
    <details>
    <summary> Spawn </summary>

    - 지형과 객체들을 Level에 배치하는 역할을 담당합니다.
    - IslandGen : 난수를 통해 매번 특별한 지형을 생성합니다.
    - Spawner : 지형이 생성된 후 지형 위에 Resource들을 생성합니다.
    
    </details>
    
    <details>
    <summary> UI </summary>

    - 화면에 표시될 수 있는 모든 UI에 대한 처리를 담당합니다.
    - UiManager와 UiBase를 통해 UI의 Activate, Deactivate을 처리합니다.
    
    </details>
    
    <details>
    <summary> Util </summary>

    - 필요한 Util셋을 구성하고있습니다.
    
    </details>

### Application LifeCycle
- ApplicationStateManager를 통해 관리합니다.
    - SystemManager로서 Application이 시작하고 종료될 때까지 유지됩니다.
    - State와 Transition을 통해 S/W의 상태를 전환하고, 전환에 따른 로직이 처리됩니다.
    - 각 State전환에 대한 처리를 자세히 살펴보려면 ApplicationStateManager, TransitionProcessor를 확인하시면 됩니다.
    - 각 TransitionProcess간 의존관계 처리작업이 추가되었습니다. (24/05/26)
![alt text](Images/Code%20Overview_1.png)

### Manager 관리
- Manager에 대한 관리는 CropoutGameInstance에서 처리합니다.
- Manager는 SystemManager, LevelManager로 구분됩니다.
- SystemManager : S/W의 전체 라이프사이클동안 유지되는 Manager입니다.
    - Level전환시 OnLevelLoaded, OnLevelDestroy 이벤트가 발생하여 필요한 로직을 처리할 수 있습니다.
    - 대표적으로 ApplicationState, Table, SimpleReactive에 대한 처리를 관리합니다.
- LevelManager : Level이 열릴 때 생성되었다가 파괴될 때 제거되는 Manager입니다.
    - 대표적으로 Entity, Sound, Ui에 대한 처리를 관리합니다.
    - MainMenu 이후에만 생성합니다.

### Table소개
- 모든 Table은 TableManager를 통해 Load되고 활용할 수 있습니다.
- Blueprint/Villager/DT_VillagerJobs
    - Villager의 Job 처리에 대한 데이터
- Table/BuildableInfoTable
    - Build Panel에서 BuildEntityCardWidget을 생성할 때 사용하는 데이터
- Table/InteractableInfoTable
    - Interactable 객체에 대한 모든 데이터
- Table/SpawnerInfoTable
    - Level Load 후 지형이 생성되면 초기에 객체들을 생성할 때 사용하는 데이터 
- Table/WidgetDataTable
    - S/W에서 동적으로 Load하는 모든 UI에 대한 데이터


### Entity 소개
- Villager
    - 자원을 채취하고, 건축물을 건축하고, 농사를 지을 수 있는 NPC입니다.
    - 마우스 드래그를 통해 Villager와 Interactable간 연결을 지을 수 있습니다.
    - 하나의 Action을 시작하면 Level에 존재하는 모든 동일한 Interactable의 Action을 종료해야 휴식상태로 전환합니다.
        - ex - Villager를 건축에 할당하면 Level의 모든 건축물의 건축을 완료해야 휴식상태로 전환됩니다.
- Interactable
    - Building
        - 건축물입니다.
        - Build Panel을 통해 배치하고, Villager를 할당하여 건축할 수 있습니다.
        - Villager가 여러명 배치되면 더 빠르게 건축됩니다.
        - 건축물을 지으면 건축물에 해당하는 Action이 발생합니다.
            - Vilager 추가
            - Game 승리
    - Crop
        - 농경지입니다.
        - Build Panel을 통해 배치하고, Villager를 할당하여 농사를 지을 수 있습니다.
        - Villager는 여러 농경지를 관리할 수 있습니다. (BT에 의해 제어)
        - 하나의 농경지는 한번에 한명의 Villager의 관리만 받을 수 있습니다.
    - Resource
        - 자원입니다.
        - 초기에 배치되어있으며, BuildPanel을 통해 추가배치 할 수 있습니다.
        - Villager를 할당하여 채취할 수 있습니다.
        - 모든 자원을 채취하면 객체가 사라집니다.