# Cropout with Cpp

[![en](https://img.shields.io/badge/lang-en-red)](https://github.com/ChoiJK/Cropout_Clone_Cpp/blob/develop/README-en.md)
[![kr](https://img.shields.io/badge/lang-kr-blue)](https://github.com/ChoiJK/Cropout_Clone_Cpp/blob/develop/README.md)

---

## 1. Introduction

Unreal Engine 5의 대표적인 게임 샘플인 [Cropout Sample](https://www.unrealengine.com/en-US/blog/cropout-casual-rts-game-sample-project?sessionInvalidated=true)을 CPP로 복제하면서 UE5의 기능을 학습하기위한 프로젝트입니다.
- 참고 : [Cropout 소개](https://forums.unrealengine.com/t/explore-cropout-a-top-down-casual-rts-sample-project-general-announcements/1239909/65)

본 프로젝트의 목표는 학습입니다. 그러하기에 프로젝트의 진척은 조금 느릴 수 있습니다.   
(Behavior Tree를 통해 AI를 개발한다면 저는 Unreal Engine의 BT를 처음부터 학습하며 개발을 진행합니다.)   
저는 Unreal Engine을 처음 접하며 그로인해 기능을 이상하게 사용하거나, 구조, 문법상의 문제가 있을 수 있습니다.   
(이는 점진적으로 리펙토링하며 해결될 예정입니다.)

만약 오류나 첨언하실 내용이 있다면 언제라도 편하게 말씀주시기바랍니다.

- Unreal Version : 5.3.2
- IDE : Rider

## 2. What is Cropout sample?
Cropout은 Top-down방식의 Casual RTS 게임 샘플입니다. 크로스 플랫폼을 염두에 두고 프로젝트를 빌드하는 사례를 보여주며, 블루프린트로 제작된 유용한 기능도 소개합니다. (모두 다 블루프린트로 개발되어있습니다.)

아래의 기능들을 포함합니다.
- 일반 UI
- Enhanced Input
- Geometry Script
- Behavior Tree
    - EQS
- Animation Mongate
- Sound
- Key and Curves - Timeline play
- Save-Load
- Blueprint Interfaces
- Mobile & PC Packing

## 3. Goals
1. Cropout Sample의 모든 내용을 CPP로 개발합니다.
    - 리소스는 기존 Sample을 계승합니다.
    - 모든 컨텐츠/기능을 CPP로 개발할때까지는 리펙토링하지 않습니다.
2. CPP로 개발이 완료된다면 리펙토링, 성능프로파일링, 크로스 플랫폼 CI/CD까지 테스트해볼 예정입니다.
3. 위 과정을 진행하며 Plugin으로 뽑아낼 수 있는 기능이 있다면, OpenSource할 예정입니다. 

## 4. 테스트 방법
1. 본 저장소의 develop 브랜치를 로컬PC에 복제합니다.
2. Unreal engine을 통해 Cropout_Clone_Cpp/CropOut_Clone_Cpp.uproject을 엽니다.
3. Content Browser -> Conten/Level/InGame level을 열고 Play합니다.   
> 주의 - 모든 리소스에 대한 경로가 하드코딩되어있습니다. 주의바랍니다.

## 5. Progress
현재 작업은 약 60%정도 진행되었습니다.   

<작업 된 내용>
- Geometry Script를 통한 지형생성
- 모든 Entity의 Spawn
- Enhanced Input을 이용한 Interaction
- Behavior Tree를 통한 Villager AI
- Villager의 행위에 따른 오브젝트 Attach & Dettach
- Sound Play

<작업 할 내용>
- UI System 추가
    - UI를 이용한 게임로직 개발
- Level간 전환
- Game의 승리/패배 조건 설정
- Save/Load 기능
- 크로스 플랫폼 테스트

## 6. 정리, 아이디어
- Unreal
    - GamePlay Framework
    - Actor, ActorComponent
    - Resource Load
    - 갖가지 Pointer
    - Tick, Time & FlowControl
    - Lambda, Delegate
    - Curve, Timeline
    - Input
        - Legacy Input
        - Enhanced Input
        - UMG Input - link
        - CommonUI Input - link
    - Behaviour Tree
    - UI
        - UMG
        - CommonUI
        - UMG Input - link
        - CommonUI Input - link
    - Sound
- 아이디어
    - UI With MVP
    - Resource Loader
        - Resource를 Table로 빼서 관리
        - 모든 리소스의 Load를 일원화
        - Async Load/ Load이후의 처리까지 Lambda로 받아서 처리
    - Localization 처리
        - 외부 엑셀 테이블을 Localization Dashboard 데이터로 Import

https://github.com/ChoiJK/Cropout_Clone_Cpp/assets/14192551/dd1c1670-f1d3-467f-9a97-75af6d196a46
