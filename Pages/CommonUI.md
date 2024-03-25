# CommonUI

> 본 페이지는 [Introduction to Common UI](https://www.youtube.com/live/TTB5y-03SnE?si=jD3cqkcm8hYgOYEw), [Common UI Quickstart Guid](https://dev.epicgames.com/documentation/en-us/unreal-engine/common-ui-quickstart-guide-for-unreal-engine?application_version=5.0)를 정리한 내용입니다.

## CommonUI의 특징
- Styling System
- Input System
    - 게임 내 입력 시스템과 별도로 입력 동작 인식
    - 이를 특정 컨트롤러 데이터 자산에 매핑할 수 있는 입력 매핑 시스템
    - 자동으로 플랫폼을 선택하고 설정한 아이콘을 사용함
    - 게임 플레이 도중에도 동일함
        - 게임패드로 게임을 하다가 키보드,마우스로 전환하면 아이콘도 동적으로 전환됨
- Input Routing
- 활성화 가능한 위젯 시스템
- 여러가지 유용한 위젯

## 고비를 처리하자
- 


## 작업 순서
- CommonUI Plugin 활성화
- ProjectSetting - Game Viewport Client Class -> CommonGameViewportClient 설정
    - 기본적으로 입력 라우팅 시스템은 이 Viewport에 내장되어있음
    - 입력은 뷰포트 수준에서 캡쳐되고 계층구조의 최상위 위젯으로 전송됨
- CommonInputActionDataBase 생성
    - DataTable 생성 -> Target : CommonInputActionDataBase
    - 본 예제에서의 파일이름 : InputActionTable
    - InputActionTable에 필요한 작업 추가
- Blueprint 생성 -> Parent : CommonInputBaseControllerData
    - ControllerData_PC_GamePad
    - ControllerData_PC_Keyboard
    - Input Brush Data Map에 대한 설정 진행
- Blueprint 생성 -> Parent : CommonUIInputData
    - DemoGameInputData
    - Default Click Action, Default Back Action의 데이터 테이블(InputActionTable), 행 이름을 설정(Confirm, Cancel)
- ProjectSetting - Game/Common Input Settings - Input Data 설정
    - DemoGameInputData
- Style 파일들을 생성&설정하고 Default 설정
    - ProjectSetting - Plugins - Common UI Editor에서 Text, Button, Border Style의 Default Style을 설정할 수 있음
    - Deault Style이 아닌 여러 스타일을 만들어서 여러 곳에 사용 가능
        - Text style의 경우
            - Title
            - Header
            - Body
            - Menu
            - Etc...
        - 테마에 대한 설정도 Style로 가능할 듯
            - White
            - Dark
            - Etc...
- CommonActivatableWidget
    - 

- UI 배치
    - Overlay
        - Overlay 안에는 여러 위젯을 배치할 수 있음.
        - 서로 겹쳐서 렌더링 됨
    -Common Activatable Widget Stack
        - 기본적으로 메뉴 스택을 구축할 수 있게 해주는 위젯

## 팁
- 텍스쳐를 UI용으로 설정 일괄 변경
    - 일괄적으로 설정을 변경할 텍스쳐들을 선택
    - 마우스 우클릭 -> 에셋 액션 -> 프로퍼티 매트릭스에서 선택 편집
    - 텍스쳐 그룹 : UI
- Canvas Widget은 가능한 한 최소한으로 사용해야함
    - Canvas가 많으면 CPU 연산량이 많아짐
- UnrealEngine의 String
    - Name
        - Const String
    - String
        - String
    - Text
        - Localized String
        - Tools - Localization Dashboard

## 기타
#### CommonUI가 활성화될 때마다 기본적인 이동입력이 비활성화 되는 것 같음. 해결 방법은?
- 

