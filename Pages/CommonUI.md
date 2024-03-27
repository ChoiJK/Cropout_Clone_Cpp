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

## 생각 생각
- Input처리에 대한 작업이 생각보다 많다
- Viewport Input은 InGame Input과 분리되어있다


## 작업 순서
1. CommonUI Plugin 활성화
2. 뷰포트 입력 라우팅 구성
    - ProjectSetting - Game Viewport Client Class -> CommonGameViewportClient 설정
    - 기본적으로 입력 라우팅 시스템은 이 Viewport에 내장되어있음
    - 입력은 뷰포트 수준에서 캡쳐되고 계층구조의 최상위 위젯으로 전송됨
3. 입력 액션 데이터 테이블 생성
    - CommonInputActionDataBase 생성
    - DataTable 생성 -> Target : CommonInputActionDataBase
    - 본 예제에서의 파일이름 : InputActionTable
    - InputActionTable에 필요한 작업 추가
4. 컨트롤러 데이터 바인딩(플랫폼별 UI요소)
    - Blueprint 생성 -> Parent : CommonInputBaseControllerData
    - ControllerData_PC_GamePad
    - ControllerData_PC_Keyboard
    - Input Brush Data Map에 대한 설정 진행
5. 디폴트 내비게이션 액션 구성
    - Blueprint 생성 -> Parent : CommonUIInputData
    - DemoGameInputData
    - Default Click Action, Default Back Action의 데이터 테이블(InputActionTable), 행 이름을 설정(Confirm, Cancel)
    - ProjectSetting - Game/Common Input Settings - Input Data 설정
        - DemoGameInputData
6. Style 파일들을 생성&설정하고 Default 설정
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
    - ProjectSettings - Plugins -> Common UI Framework
        - Default Throbber Material
            - 로딩 화면에 사용
        - Default Image Resource Object
            - 로드되지 않은 UI에셋의 자리표시자
7. 이제 UI들을 배치하고 로직을 작성해보자.

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
- @ TODO
#### CommonUI와 Enhanced Input이 동시에 처리되지 않음. 해결 방법은?
- [Enhanced Input으로 Common UI 사용](https://dev.epicgames.com/documentation/ko-kr/unreal-engine/using-commonui-with-enhnaced-input-in-unreal-engine?application_version=5.2)   
    - 이건 CommonAction Widget에 관련된 사항으로 보임
- PlayerController에서 **SetInputMode(FInputModeGameAndUI());**