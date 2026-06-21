# FPS_Demo

这是一个基于 Unreal Engine 5 的 C++ 双人合作 FPS Demo。

项目实现了基础的第一人称射击、AI 敌人追击与攻击、敌人受击死亡、团队共享得分、胜利与失败判定，以及基于 UE 网络框架的双人合作玩法。

## 项目简介

本项目是一个双人合作 FPS 小 Demo。两名玩家可以进入同一局游戏，共同击杀 AI 敌人。敌人会自动追踪并攻击距离最近的存活玩家。玩家击杀敌人后会增加团队共享得分，当团队总分达到目标分数时游戏胜利；如果两名玩家全部死亡，则游戏失败。

项目主要使用 C++ 实现核心玩法逻辑，并结合蓝图完成角色资源、动画、UI 和关卡配置。

## 功能特性

* 第一人称玩家控制
* C++ 玩家角色逻辑
* 基于 Line Trace 的射击命中检测
* AI 敌人移动与攻击
* 敌人血量、受击、死亡与加分逻辑
* 团队共享得分系统
* 基于目标分数的胜利机制
* 基于全员死亡的失败机制
* 双人合作网络玩法
* 客户端开火请求通过 Server RPC 发送到服务器
* 敌人受击、死亡、加分由服务器判定
* 使用 GameState 同步团队得分和游戏结果
* HUD 显示玩家血量、团队得分、Victory 和 Game Over

## 玩法说明

游戏支持双人合作模式。

两名玩家共同击杀敌人。每击杀一个敌人，团队共享分数增加。当团队分数达到目标值时，游戏进入 Victory 状态。

敌人会自动寻找距离自己最近的存活玩家进行追击和攻击。如果某名玩家死亡，敌人会忽略该玩家并继续攻击另一名存活玩家。当两名玩家全部死亡后，游戏进入 Game Over 状态。

## 多人网络设计

本项目采用 Unreal Engine 的服务器权威网络模型。

核心设计如下：

### AFPSGameMode

`AFPSGameMode` 只存在于服务器，用于处理游戏规则。

主要职责包括：

* 敌人死亡后增加团队分数
* 判断是否达到胜利条件
* 统计死亡玩家数量
* 判断是否进入 Game Over
* 修改 GameState 中的共享游戏状态

### AFPSGameState

`AFPSGameState` 存在于服务器和所有客户端，用于保存并同步全局游戏状态。

主要同步内容包括：

* 当前团队分数
* 目标分数
* 当前游戏状态
* 死亡玩家数量

HUD 通过 GameState 读取分数和游戏结果，从而保证服务器和客户端显示一致。

### AFPSCharacter

`AFPSCharacter` 负责玩家输入、移动、开火、血量和死亡逻辑。

玩家开火时，客户端会调用 Server RPC，将开火请求发送到服务器。服务器负责执行 Line Trace 命中检测，并决定是否对敌人造成伤害。

玩家死亡后会被禁止移动和开火，避免死亡玩家继续影响游戏结果。

### AEnemyCharacter

`AEnemyCharacter` 负责敌人的血量、受击、死亡和分数奖励。

敌人的受击和死亡逻辑只在服务器执行。敌人死亡后，服务器会调用 GameMode 增加团队分数，并同步敌人销毁结果到客户端。

### AEnemyAIController

`AEnemyAIController` 负责敌人的 AI 行为。

敌人会周期性寻找距离最近的存活玩家作为目标。如果当前目标死亡，敌人会重新选择其他存活玩家。如果所有玩家都死亡，敌人停止行动。

## 网络流程

### 玩家开火流程

```text
客户端玩家按下开火键
↓
本地播放开火表现
↓
调用 ServerFire RPC
↓
服务器执行 Line Trace
↓
服务器判断是否命中敌人
↓
服务器调用敌人 TakeHit
↓
敌人死亡后服务器加分
↓
GameState 将分数和游戏结果同步给所有客户端
```

### 胜利流程

```text
敌人被击杀
↓
服务器 GameMode 增加团队分数
↓
判断团队分数是否达到目标分数
↓
如果达到目标分数，设置 GameState 为 Victory
↓
所有客户端 HUD 显示 Victory
```

### 失败流程

```text
敌人攻击玩家
↓
服务器扣除玩家血量
↓
玩家血量归零后死亡
↓
GameMode 统计死亡玩家数量
↓
如果所有玩家都死亡，设置 GameState 为 GameOver
↓
所有客户端 HUD 显示 Game Over
```

## 操作方式

| 操作   | 按键    |
| ---- | ----- |
| 移动   | WASD  |
| 视角控制 | 鼠标    |
| 跳跃   | Space |
| 开火   | 鼠标左键  |

## 多人测试方式

在 Unreal Editor 中进行 PIE 双人测试：

1. 打开项目。
2. 点击 Play 按钮旁边的小箭头。
3. 将 `Number of Players` 设置为 `2`。
4. 将 Net Mode 设置为 `Play As Listen Server`。
5. 将 Play Mode 设置为 `New Editor Window`。
6. 点击 Play 开始测试。

预期效果：

* 两名玩家可以进入同一地图。
* 两名玩家都可以移动和开火。
* Server 玩家和 Client 玩家都可以击杀敌人。
* 两名玩家共享同一个团队分数。
* 团队分数达到目标后，两个窗口都会显示 Victory。
* 两名玩家全部死亡后，两个窗口都会显示 Game Over。
* 死亡玩家不能继续移动和开火。
* 敌人会攻击最近的存活玩家。

## 主要代码结构

```text
Source/FPS_Demo/
├── FPSCharacter.h / FPSCharacter.cpp
├── FPSGameMode.h / FPSGameMode.cpp
├── FPSGameState.h / FPSGameState.cpp
├── FPSGameTypes.h
├── EnemyCharacter.h / EnemyCharacter.cpp
└── EnemyAIController.h / EnemyAIController.cpp
```

## 技术要点

* 使用 UE C++ 实现核心玩法逻辑。
* 使用 Server RPC 实现客户端开火请求。
* 服务器负责命中检测、伤害计算、敌人死亡和加分。
* 使用 GameState Replication 同步团队分数和游戏结果。
* 将服务器规则逻辑和客户端可见状态分离。
* 使用 AIController 实现敌人追击与攻击逻辑。
* 敌人会动态选择最近的存活玩家作为攻击目标。
* 玩家死亡后不能继续移动、开火或击杀敌人。
* HUD 通过 GameState 读取共享游戏状态，避免客户端访问 GameMode。

## 当前限制

* 当前主要使用 PIE Listen Server 进行双人测试。
* 尚未实现在线大厅、匹配或房间系统。
* 尚未实现 Dedicated Server 部署。
* 尚未实现玩家复活、死亡动画和观战系统。
* 尚未实现复杂武器系统，例如换弹、弹药和多武器切换。
* 尚未实现 FPS 高级网络功能，例如延迟补偿和命中回溯。

## 后续改进方向

* 添加敌人波次生成系统。
* 使用 PlayerState 记录每个玩家的击杀数和死亡状态。
* 添加更完整的死亡动画和死亡 UI。
* 添加武器弹药和换弹机制。
* 使用 NetMulticast 同步第三人称开火动画、枪口火焰和音效。
* 添加主菜单、重新开始和关卡结算界面。
* 支持打包后进行局域网联机测试。


## 引擎版本

Unreal Engine 5.7


