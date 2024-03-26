# Cropout_Clone_Cpp

[![en](https://img.shields.io/badge/lang-en-red)](README-en.md)
[![kr](https://img.shields.io/badge/lang-kr-blue)](README.md)

## 1. Introduction

This project is designed to learn the features of UE5 by replicating the [Cropout Sample](https://www.unrealengine.com/en-US/blog/cropout-casual-rts-game-sample-project?sessionInvalidated=true), a representative game sample of Unreal Engine 5, in CPP.
- See: [Introduction to Cropout](https://forums.unrealengine.com/t/explore-cropout-a-top-down-casual-rts-sample-project-general-announcements/1239909/65)

The goal of this project is to learn, so progress may be a bit slow.   
(If I were developing AI with Behavior Trees, I would be learning BT in Unreal Engine from scratch).   
I'm new to Unreal Engine, and as a result, there may be some weird functionality, structure, or grammar issues.   
(These will be addressed as I gradually refactor).

If you find any errors or have any suggestions, please feel free to let me know.

- Unreal Version : 5.3.2
- IDE : Rider

## 2. What is Cropout sample?
Cropout is a sample top-down casual RTS game. It demonstrates how to build a project with cross-platform in mind, and also showcases some useful features built with Blueprints (all of which are developed with Blueprints).

It includes the following features
- General UI
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
1. develop everything in the Cropout Sample into a CPP.
    - Resources will inherit from the existing Sample.
    - Do not refactor until all content/features are developed with CPP.
2. Once the development is complete with CPP, we will test it for refactoring, performance profiling, and cross-platform CI/CD.
3. If there are features that can be extracted as plugins during the above process, we will OpenSource them

## 4. How to test
1. clone the develop branch of this repository to your local PC.
2. open Cropout_Clone_Cpp/CropOut_Clone_Cpp.uproject through Unreal engine.
3. Open Content Browser -> Conten/Level/InGame level and play.   
> Caution - Paths to all resources are hardcoded. Caution.

## 5. Progress
We are currently about 60% of the way through the task.   

[ What has been worked on ]   

- Geometry Script to generate terrain
- Spawning of all Entities
- Interaction using Enhanced Input
- Villager AI via Behavior Tree
- Attach & Dettach Objects based on Villager's behavior
- Sound Play

[ What to work on ]   

- Adding UI System
    - Developing Game Logic with UI
- Switching between Levels
- Setting win/loss conditions for the game
- Save/Load functionality
- Cross-Platform Testing

https://github.com/ChoiJK/Cropout_Clone_Cpp/assets/14192551/dd1c1670-f1d3-467f-9a97-75af6d196a46

    
--- 
Translated with www.DeepL.com/Translator (free version)
