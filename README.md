# Qt Programming 2025 游戏项目

![Game Logo](assets/Backgrounds/ChooseBattlefieldSceneBackground.png)

## 项目简介

这是一个基于 Qt6 和 C++ 开发的2D游戏项目，使用了 Qt Graphics Framework 来构建游戏场景和渲染系统。

## 🖼️ 游戏截图

### 游戏流程展示

<table>
<tr>
<td align="center">
<img src="assets/Screenshots/StartMenu.png" width="400px" alt="开始菜单"/>
<br>
<b>开始菜单</b>
<br>
游戏启动界面，简洁的设计和Play按钮
</td>
<td align="center">
<img src="assets/Screenshots/GameModeSelection.png" width="400px" alt="游戏模式选择"/>
<br>
<b>游戏模式选择</b>
<br>
选择PvP、PvE或单人练习模式
</td>
</tr>
<tr>
<td align="center">
<img src="assets/Screenshots/ChooseBattlefield.png" width="400px" alt="地图选择"/>
<br>
<b>地图选择</b>
<br>
选择不同的战斗地图进行游戏
</td>
<td align="center">
<img src="assets/Screenshots/Battle_example_field_4.png" width="400px" alt="战斗场景"/>
<br>
<b>战斗场景</b>
<br>
实时对战，武器掉落，药物系统
</td>
</tr>
<tr>
<td colspan="2" align="center">
<img src="assets/Screenshots/GameOver.png" width="400px" alt="游戏结束"/>
<br>
<b>游戏结束</b>
<br>
显示胜负结果，提供重玩和返回选项
</td>
</tr>
</table>

### 🎮 游戏特色
- 🔥 **实时双人对战**: 支持本地双人对战模式
- 🤖 **AI控制系统**: 基于AIController的角色控制，支持基本操作
- ⚔️ **丰富武器系统**: 5种不同类型武器，各具特色
- 🎯 **动态武器掉落**: 武器从天空随机掉落，增加游戏策略性
- 💊 **多样化药物系统**: 5种药物类型，包含治疗、增益、防护效果
- 🛡️ **武器防护机制**: 蓝宝石提供武器特定防护，绿宝石提供护盾系统
- 🏃‍♂️ **流畅物理引擎**: 重力、碰撞检测、平台跳跃
- 🎮 **直观用户界面**: 开始菜单、模式选择、地图选择完整的游戏流程

## 技术栈

- **编程语言**: C++17
- **GUI框架**: Qt6 (Core, Gui, Widgets)
- **构建系统**: CMake 3.28+
- **图形渲染**: QGraphicsView/QGraphicsScene
- **开发工具**: Qt Creator, MinGW 64-bit
- **版本控制**: Git
- **资源管理**: Qt Resource System (.qrc)

## 核心依赖库

### Qt6 框架模块
- **Qt6::Core** - 核心功能(QObject, QTimer, QGraphicsScene等)
- **Qt6::Gui** - GUI基础组件(QPixmap, QFont, 事件处理等)  
- **Qt6::Widgets** - 窗口部件(QMainWindow, QGraphicsView等)


## 项目结构

```
├── src/                    # 源代码目录
│   ├── main.cpp           # 程序入口点
│   ├── MyGame.h/cpp       # 主游戏窗口类 (QMainWindow子类)
│   ├── GameMode.h/cpp     # 游戏模式系统 (PvP/PvE/Single模式定义)
│   ├── AI/                # AI控制系统
│   │   └── AIController.h/cpp    # AI控制器 (角色控制接口)
│   │   │                         # - 提供程序化角色操作接口
│   │   │                         # - 支持移动、攻击、拾取等基本操作
│   │   │                         # - 可实时启用/禁用AI控制
│   │   │                         # - 为AI策略实现预留扩展接口
│   ├── Items/             # 游戏物品系统
│   │   ├── Item.h/cpp     # 基础物品类 (QGraphicsItem子类)
│   │   ├── Pickable.h/cpp # 可拾取物品基类 (Item子类)
│   │   ├── PickupManager.h/cpp # 统一拾取管理器 (QObject子类)
│   │   ├── Characters/    # 角色相关
│   │   │   ├── Character.h/cpp # 基础角色类 (Item子类)
│   │   │   ├── BuffSystem.h    # 增益效果系统 (武器防护+伤害护盾)
│   │   │   └── Green.h/cpp     # Green角色 (Character子类)
│   │   ├── Weapons/       # 武器系统
│   │   │   ├── Weapon.h/cpp          # 武器基类 (Item子类)
│   │   │   ├── WeaponManager.h/cpp   # 武器管理器 (QObject子类)
│   │   │   ├── MeleeWeapon.h/cpp     # 近战武器基类 (Weapon子类)
│   │   │   ├── RangedWeapon.h/cpp    # 远程武器基类 (Weapon子类)
│   │   │   ├── Projectile.h/cpp      # 投射物基类 (QObject+QGraphicsPixmapItem)
│   │   │   ├── Fist.h/cpp            # 拳头(默认武器) (MeleeWeapon子类)
│   │   │   ├── Sword.h/cpp           # 剑 (MeleeWeapon子类)
│   │   │   ├── ThrowingStone.h/cpp   # 投掷石头 (RangedWeapon子类)
│   │   │   ├── Raygun.h/cpp          # 激光枪 (RangedWeapon子类)
│   │   │   └── RaygunBig.h/cpp       # 大型激光枪 (RangedWeapon子类)
│   │   ├── Medicine/      # 药物系统
│   │   │   ├── Medicine.h/cpp        # 药物基类 (Pickable子类)
│   │   │   ├── MedicineManager.h/cpp # 药物管理器 (QObject子类)
│   │   │   ├── HealthPotion.h/cpp    # 健康药水 (Medicine子类)
│   │   │   ├── MagicPotion.h/cpp     # 魔法药水 (Medicine子类)
│   │   │   ├── EnergyBoost.h/cpp     # 能量增益药水 (Medicine子类)
│   │   │   ├── GemBlue.h/cpp         # 蓝宝石 (武器防护药物)
│   │   │   └── GemGreen.h/cpp        # 绿宝石 (护盾药物)
│   │   └── Maps/          # 地图相关
│   │       ├── Map.h/cpp             # 地图基类 (Item子类)
│   │       └── Battlefield.h/cpp     # 战场地图 (Map子类)
│   ├── Physics/           # 物理系统
│   │   └── PhysicsConstants.h        # 物理常量 (静态常量类)
│   └── Scenes/            # 游戏场景
│       ├── Scene.h/cpp    # 基础场景类 (QGraphicsScene子类)
│       ├── StartMenuScene.h/cpp      # 开始菜单场景 (Scene子类)
│       ├── BattleScene.h/cpp         # 战斗场景 (Scene子类)
│       ├── ChooseBattlefieldScene.h/cpp # 地图选择场景 (Scene子类)
│       ├── GameModeSelectionScene.h/cpp # 游戏模式选择场景 (Scene子类)
│       └── GameOverScene.h/cpp       # 游戏结束场景 (Scene子类)
├── assets/                # 游戏资源
│   ├── assets.qrc        # Qt资源文件
│   ├── Screenshots/      # 游戏截图
│   │   ├── StartMenu.png         # 开始菜单截图
│   │   ├── GameModeSelection.png # 模式选择截图
│   │   ├── ChooseBattlefield.png # 地图选择截图
│   │   ├── Battle_example_field_4.png # 战斗场景截图
│   │   └── GameOver.png          # 游戏结束截图
│   ├── Backgrounds/      # 背景图片
│   │   ├── ChooseBattlefieldSceneBackground.png
│   │   └── GameOverBackground.png
│   └── Items/            # 物品贴图资源
│       ├── Characters/   # 角色贴图
│       │   └── green/    # Green角色各状态贴图
│       ├── Maps/         # 地图资源
│       │   └── Battlefield/ # 战场地图资源
│       ├── Medicine/     # 药物贴图
│       │   ├── full_heart.png   # 魔法药水图标
│       │   ├── half_heart.png   # 健康药水图标
│       │   └── mushroom.png     # 能量药水图标
│       └── Weapons/      # 武器贴图
│           ├── Sword/    # 剑相关资源
│           ├── ThrowingStone/ # 投掷石头资源
│           └── Raygun/   # 激光枪系列资源
└── build/                # 构建输出目录
    ├── qt_programming_2024.exe # 可执行文件
    ├── CMakeFiles/       # CMake构建文件
    └── qt_programming_2024_autogen/ # Qt MOC自动生成文件
```

## 🏗️ 系统架构设计

### 核心架构模式

#### 1. 场景管理系统 (Scene Manager Pattern)
```cpp
MyGame (主控制器)
├── Scene* currentScene (当前活动场景)
├── StartMenuScene* (开始菜单场景)
├── GameModeSelectionScene* (游戏模式选择场景)
├── ChooseBattlefieldScene* (地图选择场景)
├── BattleScene* (战斗场景) 
├── GameOverScene* (游戏结束场景)
└── QGraphicsView* view (统一视图容器)
```

#### 2. 物品继承体系 (Item Hierarchy)
```cpp
QGraphicsItem (Qt基类)
└── Item (项目基础物品类)
    ├── Character (角色类)
    │   └── Green (具体角色实现)
    ├── Map (地图类)
    │   └── Battlefield (战场地图)
    ├── Weapon (武器类)
    │   ├── MeleeWeapon (近战武器)
    │   │   ├── Fist (拳头)
    │   │   └── Sword (剑)
    │   └── RangedWeapon (远程武器)
    │       ├── ThrowingStone (投掷石头)
    │       ├── Raygun (激光枪)
    │       └── RaygunBig (大型激光枪)
    └── Pickable (可拾取物品)
        └── Medicine (药物类)
            ├── HealthPotion (健康药水)
            ├── MagicPotion (魔法药水)
            ├── EnergyBoost (能量药水)
            ├── GemBlue (蓝宝石防护药物)
            └── GemGreen (绿宝石护盾药物)
```

#### 3. 管理器模式 (Manager Pattern)
```cpp
QObject (Qt基类)
├── WeaponManager (武器管理器)
│   ├── QTimer* dropTimer (掉落定时器)
│   ├── 武器创建工厂方法
│   ├── 掉落逻辑控制
│   └── 拾取处理逻辑
├── MedicineManager (药物管理器)
│   ├── QTimer* dropTimer (掉落定时器)
│   ├── 药物创建工厂方法
│   ├── 概率控制系统
│   └── 效果应用逻辑
└── PickupManager (统一拾取管理器)
    ├── WeaponManager* weaponManager (武器管理器引用)
    ├── MedicineManager* medicineManager (药物管理器引用)
    ├── 距离检测算法
    └── 类型识别分发
```

## 功能特性

### 已实现功能
- ✅ **游戏引擎架构**: 基于Qt Graphics Framework的完整游戏引擎
- ✅ **场景管理系统**: 完整的Scene/StartMenuScene/GameModeSelectionScene/ChooseBattlefieldScene/BattleScene/GameOverScene架构
- ✅ **用户界面系统**: 开始菜单、游戏模式选择、地图选择、游戏结束界面的完整流程
- ✅ **游戏模式系统**: PvP/PvE/Single模式选择，支持不同游戏体验
- ✅ **物品系统框架**: Item/Pickable完整继承体系
- ✅ **角色系统**: Character基类+增益效果(BuffSystem)+血量管理+武器防护系统
- ✅ **AI控制系统**: AIController提供角色控制接口，支持基本操作
  - 🤖 **程序化控制**: 通过代码控制角色的移动、攻击、拾取等操作
  - 🤖 **实时切换**: 可以在游戏运行时启用或禁用AI控制
  - 🤖 **操作完整性**: 支持角色的所有基本操作，包括组合操作
  - 🤖 **扩展接口**: 为未来AI策略实现提供了基础框架
- ✅ **完整的武器系统**
  - 🎯 **武器分类体系**: 近战武器(MeleeWeapon)/远程武器(RangedWeapon)双分支
  - 🎯 **武器掉落机制**: WeaponManager定时掉落+权重分配系统
  - 🎯 **武器拾取系统**: 距离检测+按键拾取+武器替换逻辑
  - 🎯 **投射物系统**: Projectile基类+物理轨迹+碰撞检测
  - 🎯 **弹药消耗机制**: 远程武器弹药管理+自动回收+默认武器切换
  - 🎯 **射击间隔控制**: 基于QElapsedTimer的武器冷却时间管理
- ✅ **完整的药物系统**
  - 💊 **药物分类体系**: 治疗类/增益类/减益类/特殊类四大类型
  - 💊 **药物掉落机制**: MedicineManager概率掉落+权重控制
  - 💊 **药物拾取系统**: 统一拾取接口+即时效果应用
  - 💊 **即时效果药物**: HealthPotion(+25HP)/MagicPotion(回满血)
  - 💊 **持续性药物**: EnergyBoost(速度+50%+持续回血25秒)
  - 💊 **防护性药物**: GemBlue(武器防护)/GemGreen(伤害护盾)
  - 💊 **增益效果系统**: BuffEffect结构+定时器管理+效果叠加+武器防护
- ✅ **统一拾取管理系统**
  - 🔄 **PickupManager核心**: 统一处理武器/药物/其他可拾取物品
  - 🔄 **距离检测系统**: 基于PhysicsConstants::PICKUP_DISTANCE的精确检测
  - 🔄 **最近物品选择**: 欧几里得距离算法+自动筛选最优目标
  - 🔄 **多类型物品支持**: dynamic_cast类型识别+分发处理
  - 🔄 **自动内存管理**: Qt父子对象系统+RAII资源管理
- ✅ **物理系统**: 重力加速度+碰撞检测+平台跳跃+边界处理
- ✅ **双人对战系统**: 本地PvP+独立控制(WASD+JK vs 方向键+NM)
- ✅ **完整的用户界面流程**: 开始菜单→模式选择→地图选择→战斗→结束界面的完整游戏体验
- ✅ **游戏主窗口系统**: QMainWindow+QGraphicsView+场景切换
- ✅ **资源管理系统**: Qt Resource System(.qrc)+自动资源加载

### 🎮 场景管理系统详解

#### 场景生命周期管理
```cpp
// 场景切换流程
1. currentScene->stopLoop()    // 停止当前场景循环
2. delete oldScene            // 清理旧场景资源
3. newScene = new Scene()     // 创建新场景
4. view->setScene(newScene)   // 设置到视图
5. newScene->startLoop()      // 启动新场景循环
```

#### 场景间通信机制
- **信号槽连接**: Qt信号槽机制实现场景间数据传递
- **MyGame控制器**: 作为中央控制器协调场景切换
- **状态保持**: currentMapId等关键状态在MyGame中维护

#### 各场景职责分工
| 场景名称 | 主要职责 | 关键功能 | 输入处理 |
|---------|---------|---------|---------|
| **StartMenuScene** | 游戏启动界面 | 游戏标题显示+开始按钮+动画效果 | 鼠标点击/Enter键/空格键 |
| **GameModeSelectionScene** | 游戏模式选择 | PvP/PvE/Single模式选择 | 方向键/鼠标选择+Enter确认 |
| **ChooseBattlefieldScene** | 地图选择 | UI交互+地图预览+选择确认 | 方向键/鼠标选择+Enter确认 |
| **BattleScene** | 核心战斗 | 双人对战+物理系统+物品管理+AI控制 | 双玩家独立控制+AI接口+ESC返回 |
| **GameOverScene** | 结算显示 | 胜负结果+重玩选项+返回菜单 | R重玩+M地图选择+ESC模式选择+Q开始菜单 |

#### 场景切换流程图

```mermaid
graph LR
    A[开始菜单] --> B[模式选择]
    B --> C[地图选择]
    C --> D[战斗场景]
    D --> E[游戏结束]
    E --> A
    E --> B
    E --> C
    E --> D
```

#### 场景截图展示

| 开始菜单 | 模式选择 |
|---------|---------|
| ![开始菜单](assets/Screenshots/StartMenu.png) | ![模式选择](assets/Screenshots/GameModeSelection.png) |

| 地图选择 | 战斗场景 |
|---------|---------|
| ![地图选择](assets/Screenshots/ChooseBattlefield.png) | ![战斗场景](assets/Screenshots/Battle_example_field_4.png) |

| 游戏结束 |
|---------|
| ![游戏结束](assets/Screenshots/GameOver.png) |

### 🎨 开始菜单系统详解

![开始菜单](assets/Screenshots/StartMenu.png)

#### StartMenuScene功能特性
- **视觉设计**: 深色背景配合游戏标题，营造专业游戏氛围
- **交互按钮**: 大型Play按钮，支持鼠标点击和键盘操作
- **动画效果**: 标题文字呼吸效果，增强视觉吸引力
- **多种输入**: 支持鼠标点击、Enter键、空格键开始游戏
- **响应式布局**: 所有UI元素自动居中，适配960x640分辨率

#### 开始菜单UI组件
```cpp
StartMenuScene UI结构
├── 背景层 (Z值: -100 到 -25)
│   ├── 深色背景矩形 (QGraphicsRectItem)
│   ├── 背景图片 (可选, QGraphicsPixmapItem) 
│   └── 半透明遮罩 (QGraphicsRectItem)
├── 文本层 (Z值: 10)
│   ├── 主标题 "Qt Programming 2025" (48pt Arial Bold)
│   ├── 副标题 "2D Battle Game" (24pt Arial)
│   └── 操作说明文字 (16pt Arial)
└── 交互层 (Z值: 5-10)
    ├── Play按钮背景 (QGraphicsRectItem)
    └── Play按钮文字 "PLAY" (28pt Arial Bold)
```

#### 交互逻辑实现
- **信号机制**: 通过 `playButtonClicked()` 信号通知主控制器
- **按钮检测**: 基于矩形区域的点击检测算法
- **动画系统**: 基于QTimer的平滑动画效果 (20 FPS)
- **状态管理**: 标题透明度动态变化 (0.6-1.0 淡入淡出)

### 🏗️ 物理系统深度解析

#### 物理常量系统 (PhysicsConstants)
```cpp
// 重力相关
static constexpr qreal GRAVITY_ACCELERATION = 980.0;    // 像素/秒²
static constexpr qreal MAX_FALL_SPEED = 500.0;          // 最大下降速度
static constexpr qreal JUMP_SPEED = -500.0;             // 跳跃初始速度

// 移动相关  
static constexpr qreal HORIZONTAL_MOVE_SPEED = 300.0;   // 水平移动速度
static constexpr qreal ITEM_FALL_SPEED = 200.0;         // 物品下落速度

// 交互相关
static constexpr qreal PICKUP_DISTANCE = 100.0;         // 拾取距离
static constexpr qreal ATTACK_RANGE = 80.0;             // 攻击范围
static constexpr qreal ATTACK_DAMAGE = 10.0;            // 基础攻击伤害
```

#### 重力系统实现
- **重力应用**: Character::applyGravity()基于deltaTime精确计算
- **地面检测**: Map::getFloorHeightAt()提供地形高度查询
- **碰撞处理**: Character::handleGroundCollision()处理着陆逻辑
- **物品掉落**: 武器/药物掉落时自动应用重力效果

#### 碰撞检测系统
- **角色碰撞**: AABB包围盒检测+地形查询
- **攻击判定**: getAttackRange()范围检测+伤害应用
- **投射物碰撞**: Projectile自动检测+目标筛选

### 🎮 角色系统深度解析

#### 角色系统架构
```cpp
Character (基类) ← Item ← QGraphicsItem
├── 基础属性系统
│   ├── 血量管理 (currentHealth/maxHealth)
│   ├── 移动系统 (velocity/速度控制)
│   ├── 方向控制 (directionRight)
│   └── 状态管理 (跳跃/蹲下/攻击/拾取)
├── 武器装备系统
│   ├── Weapon* weapon (当前装备武器)
│   ├── equipWeapon() (装备武器方法)
│   └── unequipWeapon() (卸下武器方法)
├── 增益效果系统
│   ├── QMap<QString, BuffEffect> activeBuffs
│   ├── QMap<QString, QTimer*> buffTimers
│   └── BuffEffect应用/移除逻辑
└── 血量条UI系统
    ├── QGraphicsRectItem* healthBarBackground
    ├── QGraphicsRectItem* healthBarForeground
    └── QGraphicsTextItem* healthText
```

#### 增益效果系统 (BuffSystem)
```cpp
struct BuffEffect {
    QString name;             // 增益名称
    qreal speedMultiplier;    // 速度倍数 (1.5 = 150%速度)
    qreal healthRegenRate;    // 血量回复速度 (1.0 HP/秒)
    int duration;             // 持续时间 (25000毫秒)
    int tickInterval;         // 触发间隔 (1000毫秒)
    QMap<QString, qreal> weaponProtections; // 武器防护映射
    qreal damageShield;       // 伤害护盾值 (能吸收的总伤害量)
    qreal maxDamageShield;    // 最大护盾值 (用于显示护盾百分比)
};

struct WeaponProtection {
    QString weaponName;       // 武器名称 ("fist", "sword", "raygun"等)
    qreal damageMultiplier;   // 伤害倍数 (0.0=免疫, 0.5=50%减免, 1.0=正常)
};
```

#### 角色控制系统
- **输入处理**: processInput()处理按键状态更新
- **移动逻辑**: processMovementAndBounds()处理移动+边界检测
- **攻击系统**: processAttack()处理攻击逻辑+武器调用
- **物理应用**: applyGravity()应用重力+地面碰撞

#### 血量管理系统
- **血量条显示**: 实时更新的可视化血量条
- **伤害计算**: takeDamage()方法处理伤害应用
- **治疗机制**: heal()方法处理血量恢复
- **死亡检测**: isAlive()状态检查+游戏结束触发

### 🗺️ 地图系统深度解析

#### 地图系统架构
```cpp
Map (基类) ← Item ← QGraphicsItem
├── 地形数据管理
│   ├── Wall结构体数组 (墙体碰撞)
│   ├── Platform结构体数组 (平台系统)
│   └── Floor高度查询系统
├── 碰撞检测接口
│   ├── getFloorHeightAt(qreal x) 
│   ├── getWalls() 返回墙体数据
│   └── getPlatforms() 返回平台数据
└── 视觉渲染系统
    ├── 背景图片缩放适配
    ├── scaleToFitScene() 场景适配
    └── QGraphicsPixmapItem显示层
```

#### Battlefield地图实现
- **多地图支持**: 构造函数接受mapId参数选择不同地图
- **地形定义**: 硬编码的墙体和平台位置数据
- **碰撞几何**: Wall和Platform结构体定义可碰撞区域
- **视觉资源**: 对应的背景图片资源加载

#### 地图与角色交互
- **重力应用**: Character通过Map引用查询地面高度
- **碰撞检测**: Character调用Map的墙体数据进行碰撞检查
- **平台系统**: 支持可跳跃通过的平台机制

## 🔧 模块间逻辑关系

### 核心交互流程图

#### 游戏启动流程
```
main.cpp 创建 QApplication
    ↓
MyGame(QMainWindow) 创建主窗口
    ↓
switchToStartMenuScene() 切换到开始菜单
    ↓
StartMenuScene 显示游戏标题和Play按钮
    ↓
playButtonClicked() 信号发送开始游戏请求
    ↓
MyGame::onStartGame() 处理开始游戏
    ↓
switchToGameModeSelectionScene() 切换到模式选择
    ↓
GameModeSelectionScene 显示模式选择界面
    ↓
gameModeSelected(mode) 信号发送选择结果
    ↓
MyGame::onGameModeSelected() 处理模式选择
    ↓
switchToChooseMapScene() 切换到地图选择
    ↓
ChooseBattlefieldScene 显示地图选择界面
    ↓
mapSelected(mapId) 信号发送选择结果
    ↓
MyGame::onMapSelected() 处理地图选择
    ↓
switchToBattleScene(mapId) 切换到战斗场景
    ↓
BattleScene(mapId) 创建战斗场景并初始化所有管理器
```

#### 战斗场景初始化流程
```cpp
BattleScene::BattleScene(mapId) {
    // 1. 基础场景设置
    setSceneRect(0, 0, 960, 640);
    
    // 2. 地图和角色创建
    map = new Battlefield(mapId);
    character = new Green();
    enemy = new Green();
    
    // 3. 建立角色与地图关系
    character->setMap(map);
    enemy->setMap(map);
    
    // 4. 初始化武器(默认拳头)
    character->equipWeapon(new Fist(character));
    enemy->equipWeapon(new Fist(enemy));
    
    // 5. 创建管理器系统
    weaponManager = new WeaponManager(this);
    medicineManager = new MedicineManager(this);
    pickupManager = new PickupManager(this);
    
    // 6. 建立管理器间关系
    pickupManager->setWeaponManager(weaponManager);
    pickupManager->setMedicineManager(medicineManager);
    
    // 7. 启动掉落系统
    weaponManager->startWeaponDrops();
    medicineManager->startMedicineDrops();
}
```

#### 游戏主循环 (BattleScene::update())
```cpp
void BattleScene::update() {
    Scene::update();                // 基础场景更新
    processPhysics();              // 物理系统(重力+碰撞)
    processAttacks();              // 攻击系统处理
    processItemPickup();           // 物品拾取系统
    checkGameOver();               // 游戏结束检测
}
```

### 管理器系统协作关系

#### WeaponManager ↔ Character 交互
```cpp
// 武器掉落 → 场景
WeaponManager::dropRandomWeapon() {
    WeaponDropType type = getRandomWeaponType();  // 权重随机
    QPointF position = getRandomDropPosition();   // 位置计算
    Weapon* weapon = createWeapon(type);          // 工厂创建
    gameScene->addItem(weapon);                   // 添加到场景
}

// 角色拾取 → 武器管理器
WeaponManager::handleWeaponPickup(character, weapon) {
    Weapon* oldWeapon = character->getWeapon();   // 获取当前武器
    character->equipWeapon(weapon);               // 装备新武器
    return oldWeapon;                             // 返回旧武器供删除
}
```

#### MedicineManager ↔ Character 交互  
```cpp
// 药物效果应用
Medicine::applyEffect(Character* character) {
    applyMedicineEffect(character);               // 调用具体效果
    // 即时效果: character->heal(amount)
    // 持续效果: character->applyBuff(buffEffect)
}

// 增益效果管理
Character::applyBuff(BuffEffect& buff) {
    activeBuffs[buff.name] = buff;                // 记录增益
    QTimer* timer = new QTimer();                 // 创建持续时间定时器
    QTimer* tickTimer = new QTimer();             // 创建触发定时器
    // 设置定时器逻辑...
}
```

#### PickupManager 统一拾取协调
```cpp
PickupManager::handlePickup(Character* character) {
    // 1. 查找最近可拾取物品
    QGraphicsItem* item = getClosestPickableItem(character);
    
    // 2. 类型识别和分发
    if (auto weapon = dynamic_cast<Weapon*>(item)) {
        return handleWeaponPickup(character, weapon);
    } else if (auto medicine = dynamic_cast<Medicine*>(item)) {
        return handleMedicinePickup(character, medicine);
    } else if (auto pickable = dynamic_cast<Pickable*>(item)) {
        return handlePickableItemPickup(character, pickable);
    }
    
    return false;
}
```

### 内存管理和对象生命周期

#### Qt父子对象系统
- **场景管理**: QGraphicsScene自动管理添加的QGraphicsItem
- **武器装备**: 武器通过setParentItem(character)建立父子关系
- **管理器清理**: QObject父子关系确保正确的析构顺序

#### 资源回收策略
```cpp
// 武器替换时的内存管理
Weapon* oldWeapon = character->equipWeapon(newWeapon);
if (oldWeapon && oldWeapon->getWeaponname() != "fist") {
    delete oldWeapon;  // 手动删除被替换的武器
}

// 药物使用后的清理
medicine->applyEffect(character);
if (medicine->scene()) {
    medicine->scene()->removeItem(medicine);  // 从场景移除
}
delete medicine;  // 删除药物对象
```

### 事件驱动架构

#### 输入事件流
```
用户输入 → Qt事件系统 → Scene::keyPressEvent()
    ↓
BattleScene::keyPressEvent() → Character状态更新
    ↓  
Character::processInput() → 移动/攻击/拾取逻辑
    ↓
武器/药物/移动系统响应
```

#### 信号槽通信网络
```cpp
// 场景切换通信
ChooseBattlefieldScene::mapSelected(mapId) 
    → MyGame::onMapSelected(mapId)
    → MyGame::switchToBattleScene(mapId)

// 游戏结束通信  
BattleScene::gameOver(winner)
    → MyGame::onGameOver(winner)
    → MyGame::switchToGameOverScene(winner)

// 重新开始通信
GameOverScene::restartBattle()
    → MyGame::onRestartBattle()
    → MyGame::switchToBattleScene(currentMapId)
```

## AI控制系统设计

### 🤖 AI系统架构

#### 设计目标
- **简单控制**: AIController提供角色操作的程序化接口
- **基础功能**: 基于QObject的控制器，实现基本角色操作
- **完整接口**: 支持角色的移动、跳跃、攻击、拾取等操作
- **开关控制**: 可以启用或禁用AI控制

#### AI控制器结构
```cpp
AIController ← QObject
├── Character* character      // 被控制的角色引用
├── bool aiEnabled           // AI启用状态
├── 操作接口
│   ├── moveLeft/moveRight() // 移动控制
│   ├── jump()              // 跳跃控制
│   ├── attack()            // 攻击控制
│   ├── pickup()            // 拾取控制
│   ├── squat()             // 蹲下控制
│   └── stopMovement()      // 停止移动
└── universalAction()       // 组合操作接口
```

### 🎮 AI控制功能

#### 基本控制接口
- **setEnabled(bool)**: 启用/禁用AI控制
- **moveLeft/Right()**: 水平移动控制
- **jump()**: 跳跃操作
- **attack()**: 攻击操作  
- **pickup()**: 拾取物品
- **squat()**: 蹲下操作
- **stopMovement()**: 停止所有移动

#### 组合操作接口
```cpp
void universalAction(bool left, bool right, bool up, bool down, bool attack, bool pickup);
```
- 允许同时执行多个操作
- 布尔参数对应不同的操作状态
- 可以组合实现复杂动作

### 🔧 实现特点

#### 设计优势
- **简单实现**: 直接调用角色方法，无复杂算法
- **易于扩展**: 可以方便地添加新的控制逻辑
- **便于调试**: 简单的控制流程，容易定位问题
- **实时切换**: 游戏运行时可以开关AI控制

#### 系统集成
- **统一接口**: AI控制和玩家控制使用相同的角色方法
- **状态同步**: AI操作直接影响角色状态
- **权限隔离**: AI启用时可以接管角色控制

### 🔍 AI模块技术实现

#### 核心文件结构
```
src/AI/
├── AIController.h          # AI控制器头文件
└── AIController.cpp        # AI控制器实现文件
```

#### AIController类详解
```cpp
class AIController : public QObject {
    Q_OBJECT
    
public:
    explicit AIController(Character* character, QObject* parent = nullptr);
    
    // 基本控制方法
    void setEnabled(bool enabled);
    bool isEnabled() const;
    
    // 移动控制
    void moveLeft();
    void moveRight();
    void stopMovement();
    void jump();
    void squat();
    
    // 战斗控制
    void attack();
    void pickup();
    
    // 组合操作
    void universalAction(bool left, bool right, bool up, bool down, 
                        bool attack, bool pickup);
    
private:
    Character* m_character;     // 被控制的角色
    bool m_aiEnabled;          // AI启用状态
};
```

#### 控制方法实现原理
```cpp
// 移动控制实现
void AIController::moveLeft() {
    if (m_aiEnabled && m_character) {
        m_character->setLeftPressed(true);
        m_character->setRightPressed(false);
    }
}

// 攻击控制实现
void AIController::attack() {
    if (m_aiEnabled && m_character) {
        m_character->setAttackPressed(true);
    }
}

// 组合操作实现
void AIController::universalAction(bool left, bool right, bool up, 
                                  bool down, bool attack, bool pickup) {
    if (!m_aiEnabled || !m_character) return;
    
    m_character->setLeftPressed(left);
    m_character->setRightPressed(right);
    m_character->setUpPressed(up);
    m_character->setDownPressed(down);
    m_character->setAttackPressed(attack);
    m_character->setPickupPressed(pickup);
}
```

#### 使用示例
```cpp
// 在BattleScene中使用AI控制器
BattleScene::BattleScene() {
    // 创建AI控制器
    enemyAI = new AIController(enemy, this);
    
    // 启用AI控制
    enemyAI->setEnabled(true);
    
    // 简单的AI行为示例
    connect(gameTimer, &QTimer::timeout, this, [this]() {
        // 让AI角色随机移动
        if (qrand() % 100 < 30) {
            enemyAI->moveLeft();
        } else if (qrand() % 100 < 60) {
            enemyAI->moveRight();
        } else {
            enemyAI->stopMovement();
        }
        
        // 让AI角色在接近时攻击
        qreal distance = calculateDistance(enemy, character);
        if (distance < 100) {
            enemyAI->attack();
        }
    });
}
```

### 🎯 AI应用场景

#### 当前实现的AI功能
- **基础移动**: 程序化控制角色的左右移动
- **跳跃控制**: 可以触发角色跳跃动作
- **攻击操作**: 控制角色使用当前装备的武器
- **物品拾取**: 控制角色拾取附近的武器或药物
- **状态切换**: 可以实时启用或禁用AI控制

#### AI控制的局限性
- **无决策逻辑**: AI控制器只提供操作接口，不包含决策算法
- **需要外部逻辑**: 需要在游戏逻辑中编写AI行为策略
- **简单状态机**: 当前实现适合简单的行为模式
- **无学习能力**: AI不会根据游戏情况自动调整策略

#### 集成方式
- **独立模块**: AI控制器作为独立模块，不影响现有游戏逻辑
- **可选功能**: 可以选择性地为角色启用AI控制
- **实时切换**: 游戏运行时可以在人工控制和AI控制间切换
- **扩展性**: 为未来的AI策略实现预留了接口

### 🔧 AI扩展可能性

#### 短期扩展方向
- **基础策略**: 实现简单的追击、逃跑、拾取物品策略
- **状态机**: 构建基于状态的AI行为系统
- **目标选择**: 实现最近敌人、最近物品的自动选择逻辑

#### 中期扩展方向
- **行为树**: 实现更复杂的AI决策结构
- **路径规划**: 添加简单的路径寻找算法
- **策略评估**: 根据当前游戏状态选择合适的行为

#### 技术限制
- **性能考虑**: 复杂AI算法可能影响游戏流畅度
- **实现复杂度**: 高级AI功能需要大量额外开发工作
- **平衡性**: AI过强或过弱都会影响游戏体验

## 药物系统深度解析

### 🏗️ 武器系统架构

#### 核心类结构
```
Weapon (基类) ← Item
├── MeleeWeapon (近战武器基类)
│   ├── Fist (拳头)
│   └── Sword (剑)
└── RangedWeapon (远程武器基类)
    ├── ThrowingStone (投掷石头)
    ├── Raygun (激光枪)
    └── RaygunBig (大型激光枪)

WeaponManager (武器管理器) ← QObject
Projectile (投射物基类) ← QObject + QGraphicsPixmapItem
├── StoneProjectile (石头投射物)
├── LaserBullet (激光子弹)
└── BigLaserBullet (大型激光子弹)
```

#### 关键系统组件
- **WeaponManager**: 负责武器掉落、拾取逻辑、弹药管理
- **Projectile**: 处理远程武器的子弹物理和碰撞检测
- **Character.equipWeapon()**: 角色武器装备/卸载系统

### 🎯 具体武器详细参数

#### 武器图鉴展示

| 武器预览 | 武器名称 | 类型 | 伤害值 | 射程 | 弹药数 | 射击间隔 | 特殊属性 |
|---------|---------|------|--------|------|--------|----------|----------|
| 🥊 | **Fist** | 近战 | 5.0 | 30.0 | ∞ | - | 默认武器，无贴图 |
| ![Sword](assets/Items/Weapons/Sword/Sword.png) | **Sword** | 近战 | 25.0 | 60.0 | ∞ | - | 高伤害近战 |
| ![ThrowingStone](assets/Items/Weapons/ThrowingStone/ThrowingStone_Icon.png) | **ThrowingStone** | 远程 | 15.0 | 400.0 | 10发 | - | 抛物线轨迹，受重力影响 |
| ![Raygun](assets/Items/Weapons/Raygun/raygun.png) | **Raygun** | 远程 | 15.0 | 1000.0 | 10发 | 200ms | 直线射击，不受重力影响 |
| ![RaygunBig](assets/Items/Weapons/Raygun/raygunBig.png) | **RaygunBig** | 远程 | 50.0 | 1200.0 | 3发 | 800ms | 高伤害，低射速 |

### 🎮 武器系统机制详解

#### 武器掉落系统
- **掉落间隔**: 每5秒从天空掉落一件武器
- **掉落位置**: 场景上方随机X坐标，固定Y坐标(-100像素)
- **掉落权重**: 
  - 投掷石头: 20% (2/10)
  - 剑: 30% (3/10)
  - 激光枪: 30% (3/10)
  - 大型激光枪: 20% (2/10)

#### 武器拾取逻辑
```cpp
// 拾取条件检查
1. 角色按住拾取键 (J键/N键)
2. 武器在拾取范围内 (PhysicsConstants::PICKUP_DISTANCE)
3. 武器未被其他角色装备

// 拾取结果处理
- 如果角色无武器或仅有拳头 → 直接装备新武器
- 如果角色已有其他武器 → 旧武器消失，装备新武器
```

#### 弹药消耗机制
- **远程武器**: 每次攻击消耗1发弹药
- **弹药耗尽**: 武器自动消失，角色重新装备拳头
- **近战武器**: 无弹药限制，可无限使用

#### 投射物物理系统
- **激光子弹**: 匀速直线运动，速度800像素/秒，不受重力
- **大型激光子弹**: 匀速直线运动，速度600像素/秒，不受重力  
- **投掷石头**: 抛物线运动，受重力影响，初始速度400像素/秒

### 📁 武器资源文件路径

#### 武器贴图资源展示

**武器本体贴图:**
- ![Sword](assets/Items/Weapons/Sword/Sword.png) `:/Items/Weapons/Sword/Sword.png`
- ![ThrowingStone](assets/Items/Weapons/ThrowingStone/ThrowingStone_Icon.png) `:/Items/Weapons/ThrowingStone/ThrowingStone_Icon.png`  
- ![Raygun](assets/Items/Weapons/Raygun/raygun.png) `:/Items/Weapons/Raygun/raygun.png`
- ![RaygunBig](assets/Items/Weapons/Raygun/raygunBig.png) `:/Items/Weapons/RaygunBig/raygunbig.png`

**投射物贴图:**
- ![Bullet](assets/Items/Weapons/Raygun/bullet.png) `:/Items/Weapons/Raygun/bullet.png`

> 💡 **注意**: 拳头无贴图，投掷石头复用武器贴图作为投射物,大激光枪子弹和普通的使用的一样

#### 源代码文件结构
```
src/Items/Weapons/
├── Weapon.h/cpp          # 武器基类 (Item子类)
├── WeaponManager.h/cpp   # 武器管理器 (QObject子类)
├── MeleeWeapon.h/cpp     # 近战武器基类
├── RangedWeapon.h/cpp    # 远程武器基类  
├── Projectile.h/cpp      # 投射物基类
├── Fist.h/cpp           # 拳头实现
├── Sword.h/cpp          # 剑实现
├── ThrowingStone.h/cpp  # 投掷石头实现 (含StoneProjectile)
├── Raygun.h/cpp         # 激光枪实现 (含LaserBullet)
└── RaygunBig.h/cpp      # 大型激光枪实现 (含BigLaserBullet)
```

### 🔧 技术实现要点

#### 武器显示与装备
- 武器作为角色子对象显示 (`setParentItem(character)`)
- 相对位置: (0, 0) - 角色中心
- Z值层级: 1 (显示在角色前面)
- 自动跟随角色移动和旋转

#### 碰撞检测系统
- **近战武器**: 基于攻击范围矩形检测 (`getAttackRange()`)
- **远程武器**: 投射物自动检测与角色的碰撞
- **伤害应用**: 调用 `Character::takeDamage()` 方法

#### 内存管理
- 武器拾取时自动从场景移除
- 武器替换时旧武器自动删除
- 投射物碰撞后自动清理 (`cleanup()`)
- 弹药耗尽时武器自动删除

## 药物系统深度解析

### 🧪 药物系统架构

#### 核心类结构
```
Medicine (基类) ← Pickable ← Item
├── HealthPotion (健康药水)
├── MagicPotion (魔法药水)
└── EnergyBoost (能量增益药水)

MedicineManager (药物管理器) ← QObject
BuffEffect (增益效果结构)
```

#### 关键系统组件
- **MedicineManager**: 负责药物掉落、拾取逻辑、概率控制
- **BuffEffect**: 处理持续性增益效果 (速度提升、持续回血)
- **Character.applyBuff()**: 角色增益效果应用系统
- **Medicine.applyEffect()**: 药物效果应用接口

### 💊 具体药物详细参数

#### 药物图鉴展示

| 药物预览 | 药物名称 | 类型 | 效果值 | 持续时间 | 特殊属性 | 掉落权重 |
|---------|---------|------|--------|----------|----------|----------|
| ![HealthPotion](assets/Items/Medicine/half_heart.png) | **HealthPotion** | 治疗 | +25 HP | 即时 | 快速回血 | 44% (11/25) |
| ![MagicPotion](assets/Items/Medicine/full_heart.png) | **MagicPotion** | 治疗 | 回满血 | 即时 | 完全恢复 | 8% (2/25) |
| ![EnergyBoost](assets/Items/Medicine/mushroom.png) | **EnergyBoost** | 增益 | +50%速度 +1HP/秒 | 25秒 | 持续性Buff | 40% (10/25) |
| ![GemBlue](assets/Items/Medicine/gem_blue.png) | **GemBlue** | 防护 | 拳头免疫+剑50%减免 | 永久 | 武器防护 | 4% (1/25) |
| ![GemGreen](assets/Items/Medicine/gem_green.png) | **GemGreen** | 护盾 | 200点伤害护盾 | 直到耗尽 | 对远程武器75%减免 | 4% (1/25) |

### 🎮 药物系统机制详解

#### 药物掉落系统
- **掉落间隔**: 每8秒从天空掉落一瓶药物 (比武器稍慢)
- **掉落概率**: 70%概率触发掉落 (避免过于频繁)
- **掉落位置**: 场景上方随机X坐标，固定Y坐标(-100像素)
- **掉落权重**: 
  - 健康药水: 44% (11/25) - 最常见
  - 能量增益: 40% (10/25) - 常见
  - 魔法药水: 8% (2/25) - 稀有
  - 蓝宝石: 4% (1/25) - 极稀有
  - 绿宝石: 4% (1/25) - 极稀有

#### 药物效果机制
```cpp
// 即时效果药物 (健康药水、魔法药水)
1. character->heal(healAmount) - 立即恢复血量
2. 药物消失 - 从场景移除并删除

// 持续性药物 (能量增益药水)
1. character->applyBuff(energyBuff) - 应用增益效果
2. 速度倍数: 1.5倍 (50%速度提升)
3. 持续回血: 1HP/秒，持续25秒
4. 自动管理: 效果到期自动移除

// 防护性药物 (蓝宝石)
1. character->applyBuff(protectionBuff) - 应用武器防护
2. 拳头攻击: 完全免疫 (0%伤害)
3. 剑攻击: 50%伤害减免
4. 永久效果: 直到角色死亡

// 护盾药物 (绿宝石)
1. character->applyBuff(shieldBuff) - 应用伤害护盾
2. 护盾容量: 200点伤害
3. 远程武器防护: 75%伤害减免
4. 护盾耗尽: 效果自动消失
```

#### 药物分类系统
- **MedicineEffectType::Healing**: 治疗类药物 (直接回复血量)
- **MedicineEffectType::Buff**: 增益类药物 (提供临时增强效果)
- **MedicineEffectType::Special**: 特殊效果类药物 (武器防护、伤害护盾)
- **MedicineEffectType::Debuff**: 减益类药物 (预留，未来可用于毒药)

### 📁 药物资源文件路径

#### 药物贴图资源展示

**药物本体贴图:**
- ![HealthPotion](assets/Items/Medicine/half_heart.png) `:/Items/Medicine/half_heart.png`
- ![MagicPotion](assets/Items/Medicine/full_heart.png) `:/Items/Medicine/full_heart.png`  
- ![EnergyBoost](assets/Items/Medicine/mushroom.png) `:/Items/Medicine/mushroom.png`
- ![GemBlue](assets/Items/Medicine/gem_blue.png) `:/Items/Medicine/gem_blue.png`
- ![GemGreen](assets/Items/Medicine/gem_green.png) `:/Items/Medicine/gem_green.png`

#### 源代码文件结构
```
src/Items/Medicine/
├── Medicine.h/cpp        # 药物基类 (Pickable子类)
├── MedicineManager.h/cpp # 药物管理器 (QObject子类)
├── HealthPotion.h/cpp    # 健康药水实现 (+25HP即时回复)
├── MagicPotion.h/cpp     # 魔法药水实现 (回满血即时回复)
├── EnergyBoost.h/cpp     # 能量药水实现 (含BuffEffect系统)
├── GemBlue.h/cpp         # 蓝宝石实现 (武器防护系统)
└── GemGreen.h/cpp        # 绿宝石实现 (伤害护盾系统)
```

### 🔧 药物系统技术实现

#### 增益效果系统 (BuffSystem)
```cpp
struct BuffEffect {
    QString name;             // 增益名称
    qreal speedMultiplier;    // 速度倍数 (1.5 = 150%速度)
    qreal healthRegenRate;    // 血量回复速度 (1.0 HP/秒)
    int duration;             // 持续时间 (25000毫秒 = 25秒)
    int tickInterval;         // 触发间隔 (1000毫秒 = 1秒)
};
```

#### 药物效果应用流程
1. **距离检测**: 角色在拾取范围内 (100像素)
2. **效果应用**: 调用 `medicine->applyEffect(character)`
3. **类型判断**: 根据药物类型执行不同逻辑
4. **场景清理**: 自动从场景移除药物对象
5. **内存管理**: 自动删除药物实例

#### 持续性效果管理
- **Timer系统**: 基于QTimer实现周期性回血
- **效果叠加**: 支持多个Buff同时生效
- **自动清理**: 效果到期自动移除
- **状态同步**: 实时更新角色属性

## 统一拾取管理系统解析

### 🔄 PickupManager系统架构

#### 核心设计理念
- **统一接口**: 所有可拾取物品通过同一套机制处理
- **自动检测**: 自动识别不同类型物品并调用对应处理逻辑
- **距离优化**: 优先拾取距离最近的物品
- **内存安全**: 确保拾取后的对象正确清理

#### 类层次结构
```
PickupManager ← QObject
├── WeaponManager* (武器管理器引用)
├── MedicineManager* (药物管理器引用)
└── 统一拾取逻辑处理

支持的可拾取物品:
- Weapon (武器) → WeaponManager::handleWeaponPickup()
- Medicine (药物) → MedicineManager::handleMedicinePickup()  
- Pickable (其他) → 通用拾取处理
```

### 🎯 拾取系统核心机制

#### 拾取范围检测
- **拾取距离**: 100像素 (PhysicsConstants::PICKUP_DISTANCE)
- **检测算法**: 基于欧几里得距离计算
- **范围优化**: 使用矩形区域预筛选，减少计算量

#### 物品选择算法
```cpp
// 拾取优先级算法
1. 扫描角色周围100像素范围内所有物品
2. 过滤出可拾取物品 (Weapon/Medicine/Pickable)
3. 计算每个物品到角色的精确距离
4. 选择距离最近的物品进行拾取
5. 应用对应的拾取逻辑
```

#### 多类型物品处理
- **类型识别**: 使用 `dynamic_cast` 安全转换对象类型
- **分发处理**: 根据物品类型调用专门的管理器
- **统一接口**: 所有处理函数返回bool表示是否成功拾取

### 🔧 PickupManager技术细节

#### 核心方法解析
```cpp
// 主要拾取处理方法
bool handlePickup(Character* character) {
    1. 获取最近的可拾取物品 → getClosestPickableItem()
    2. 检查拾取距离 → calculateDistance()
    3. 识别物品类型 → dynamic_cast<Type*>()
    4. 分发到对应处理器 → handleXxxPickup()
    5. 返回拾取结果
}

// 物品搜索算法
QGraphicsItem* getClosestPickableItem() {
    1. 创建角色周围的搜索矩形区域
    2. 获取区域内所有图形项 → scene->items(searchArea)
    3. 过滤可拾取物品 → isPickableItem()
    4. 计算距离并找到最近物品
    5. 返回最近的可拾取物品
}
```

#### 内存管理策略
- **武器拾取**: 旧武器自动删除，新武器设置父对象
- **药物拾取**: 使用后立即从场景移除并删除
- **通用物品**: 应用效果后自动清理
- **异常处理**: 空指针检查，防止崩溃

#### 性能优化特性
- **预筛选机制**: 使用矩形区域避免遍历整个场景
- **距离缓存**: 避免重复计算相同物品的距离  
- **类型检查**: 使用快速的dynamic_cast避免无效转换
- **范围限制**: 固定的拾取距离避免无限范围搜索

### 游戏素材
- 角色精灵图 (绿色角色：正面、蹲下、跳跃状态)
- 地图背景素材 (战场地图、地图选择背景、游戏模式选择背景)
- 武器贴图资源 (剑、投掷石头、激光枪、大型激光枪)
- 药物贴图资源 (健康药水、魔法药水、能量增益药水、蓝宝石、绿宝石)
- 装备贴图资源 (各类护甲、头部装备、腿部装备)

## 🎮 游戏玩法

![战斗演示](assets/Screenshots/Battle_example_field_4.png)
*战斗场景：双角色对战，武器和药物从天空掉落*

### 🎯 控制方式

#### 玩家1控制
| 动作 | 按键 | 说明 |
|------|------|------|
| 🏃‍♂️ 移动 | **WASD** | 左右移动控制 |
| 🦘 跳跃 | **W** | 向上跳跃 |
| 🐸 蹲下 | **S** | 蹲下动作 |
| 🤏 拾取 | **J** | 拾取武器 |
| ⚔️ 攻击 | **K** | 武器攻击 |

#### 玩家2控制
| 动作 | 按键 | 说明 |
|------|------|------|
| 🏃‍♀️ 移动 | **方向键** | 左右移动控制 |
| 🦘 跳跃 | **↑** | 向上跳跃 |
| 🐸 蹲下 | **↓** | 蹲下动作 |
| 🤏 拾取 | **N** | 拾取武器 |
| ⚔️ 攻击 | **M** | 武器攻击 |

#### 通用控制
- **ESC**: 返回上一级界面（在不同场景中有不同行为）
- **Enter/空格**: 在开始菜单确认开始游戏

### 🎲 游戏流程

#### 完整游戏流程图

```
🎮 启动游戏
    ↓
📱 开始菜单 (StartMenu.png)
    ↓ [点击Play]
🎯 模式选择 (GameModeSelection.png)
    ↓ [选择PvP/PvE/Single]
🗺️ 地图选择 (ChooseBattlefield.png)
    ↓ [选择战斗地图]
⚔️ 开始战斗 (Battle_example_field_4.png)
    ↓ [战斗结束]
🏆 结果显示 (GameOver.png)
    ↓
┌─────────────────────────────┐
│ 可选择的后续操作：            │
│ • R - 重新战斗               │
│ • M - 返回地图选择           │
│ • ESC - 返回模式选择         │
│ • Q - 返回开始菜单           │
└─────────────────────────────┘
```

1. **启动游戏**: 显示开始菜单，点击Play按钮开始
2. **模式选择**: 选择PvP、PvE或单人练习模式
3. **地图选择**: 选择战斗地图
4. **开始战斗**: 进入实时对战
5. **结果显示**: 游戏结束后显示胜负结果
6. **重新游戏**: 可选择重玩、更换地图或返回开始菜单

### � 游戏机制
- 🥊 **双人对战模式**: 在战场地图中进行实时PvP战斗
- 🤖 **AI控制功能**: AI角色可以执行基本的角色操作
- 🎁 **武器掉落系统**: 武器从天空定期掉落，增加战术变数
- 💊 **药物掉落系统**: 药物从天空随机掉落，提供治疗和增益效果
- ⚡ **多样化武器**: 不同武器有独特的攻击方式、伤害和射程
- 🔋 **弹药系统**: 远程武器有弹药限制，用完后自动消失
- 💪 **增益效果**: 能量药水提供速度提升和持续回血
- 🔄 **统一拾取**: 拾取系统自动选择最近的可用物品
- ❤️ **血量系统**: 角色有血量，受到攻击时会减少血量

## 🔧 依赖库详细清单

### Qt6 框架模块
```cmake
# CMakeLists.txt中的Qt6依赖声明
find_package(Qt6 6.5.3 EXACT REQUIRED COMPONENTS Core Gui Widgets)
target_link_libraries(qt_programming_2024 Qt6::Core Qt6::Gui Qt6::Widgets)
```

| Qt6 模块 | 版本要求 | 使用功能 | 在项目中的应用 |
|---------|---------|---------|---------------|
| **Qt6::Core** | 6.5.3 | QObject基础对象系统 | ✅ 所有管理器类的基类(QObject) |
| | | QTimer定时器系统 | ✅ 武器/药物掉落定时器、增益效果计时 |
| | | QString字符串处理 | ✅ 武器名称、增益效果名称管理 |
| | | QMap容器系统 | ✅ 增益效果存储(activeBuffs) |
| | | 信号槽机制 | ✅ 场景切换、游戏事件通信 |
| | | QPointF几何计算 | ✅ 位置坐标、碰撞检测计算 |
| **Qt6::Gui** | 6.5.3 | QPixmap图像处理 | ✅ 纹理加载、角色/武器/地图显示 |
| | | QBrush/QPen绘图 | ✅ 血量条、UI元素绘制 |
| | | QKeyEvent事件系统 | ✅ 键盘输入处理、游戏控制 |
| | | QFont字体系统 | ✅ 血量文本、UI文字显示 |
| **Qt6::Widgets** | 6.5.3 | QApplication应用框架 | ✅ 主程序入口(main.cpp) |
| | | QMainWindow主窗口 | ✅ MyGame主窗口类 |
| | | QGraphicsView视图系统 | ✅ 游戏渲染引擎 |
| | | QGraphicsScene场景管理 | ✅ 所有Scene类的基础 |
| | | QGraphicsItem对象系统 | ✅ 所有游戏对象的基类 |
| | | QGraphicsRectItem几何图形 | ✅ 血量条背景/前景 |
| | | QGraphicsTextItem文本显示 | ✅ 血量数值、UI文本 |
| | | QGraphicsPixmapItem图像显示 | ✅ 角色、武器、地图背景 |

### CMake 构建系统
```cmake
cmake_minimum_required(VERSION 3.28)  # CMake最低版本要求
set(CMAKE_CXX_STANDARD 17)            # C++17标准
set(CMAKE_CXX_STANDARD_REQUIRED ON)   # 强制C++17标准

# Qt6特定配置
set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} $ENV{QTDIR})
set(CMAKE_AUTOMOC ON)                  # 自动MOC编译
set(CMAKE_AUTORCC ON)                  # 自动RCC资源编译
set(CMAKE_AUTOUIC ON)                  # 自动UIC界面编译
```

### Windows 平台依赖
- **MinGW 64位编译器**: 用于Windows平台C++编译
- **Windows SDK**: 提供Windows平台API支持
- **Visual C++ 运行库**: Qt6 Windows版本运行时依赖

### 开发环境要求
```bash
# 推荐开发环境
Qt6.5.3 + MinGW 64位
CMake 3.28+
Windows 10/11
Visual Studio Code / Qt Creator
Git版本控制
```

## 编译与运行

### 环境要求
- CMake 3.28 或更高版本
- Qt6 开发环境
- C++17 兼容的编译器

### 构建步骤

```bash
# 创建构建目录
mkdir build
cd build

# 配置项目
cmake ..

# 编译项目
cmake --build .

# 运行游戏
./qt_programming_2024.exe  # Windows
./qt_programming_2024      # Linux/macOS
```

## 🚀 开发计划

### ✅ 已完成的核心系统
- 🏗️ **游戏引擎架构**: Qt Graphics Framework + Scene管理
- 🎮 **游戏模式系统**: PvP/PvE/Single模式选择和场景管理
- 🤖 **AI控制系统**: AIController提供角色控制接口，支持基本操作
- ⚔️ **完整武器系统**: 5种武器，掉落/拾取/攻击机制完备
- 💊 **完整药物系统**: 5种药物，治疗/增益/防护效果，武器防护+护盾系统
- 🔄 **统一拾取系统**: PickupManager统一管理所有可拾取物品
- 🌍 **物理系统**: 重力、碰撞检测、平台跳跃
- 👥 **双人对战系统**: 本地PvP支持

### 🔄 正在开发/计划/想象中的功能

#### 🎯 短期计划
- 🆕 **更多武器类型**: 弓箭、魔法法杖、爆炸性武器
- 💊 **更多药物类型**: 毒药系统、护盾药水、隐身药水
- 🤖 **AI行为优化**: 
  - 基础战斗策略 (追击、躲避、攻击时机选择)
  - 物品拾取逻辑 (优先级判断、距离计算)
  - 简单状态机 (巡逻、战斗、拾取状态切换)
- ✨ **特殊效果**: 武器特效、爆炸动画、粒子系统
- 🎮 **UI优化**: 
  - 血量条、弹药显示、Buff状态显示改进
  - 武器切换动画
  - 开始菜单背景音乐和音效
  - 按钮悬停效果和点击反馈
  - 场景切换动画效果

#### 🎭 中期计划
- 🦸‍♂️ **角色技能系统**: 独特技能、升级系统
- 🗺️ **更多地图**: 不同主题战场、环境交互元素
- 🔊 **音效系统**: 武器音效、背景音乐、环境音
- 🤖 **AI策略系统**:
  - 行为树实现 (复杂决策逻辑)
  - 路径寻找算法 (A*或简化版本)
  - 多角色AI协作

#### 🌟 长期目标
- 🤖 **AI策略增强**: 
  - 更复杂的战斗策略 (武器选择、地形利用)
  - 决策树系统 (基于游戏状态的智能决策)
  - 难度等级 (不同AI智能水平)
- ⚖️ **游戏平衡**: 武器数值调优、战斗节奏、药物平衡
- 🏆 **胜负系统**: 计分机制、胜利条件、排行榜

## 开发者信息

项目创建时间：2025年7月
开发框架：Qt6 + CMake
