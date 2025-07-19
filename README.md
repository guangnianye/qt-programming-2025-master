# Qt Programming 2025 游戏项目

## 项目简介

这是一个基于 Qt6 和 C++ 开发的2D游戏项目，使用了 Qt Graphics Framework 来构建游戏场景和渲染系统。

## 技术栈

- **编程语言**: C++17
- **GUI框架**: Qt6 (Core, Gui, Widgets)
- **构建系统**: CMake 3.28+
- **图形渲染**: QGraphicsView/QGraphicsScene

## 项目结构

```
├── src/                    # 源代码目录
│   ├── main.cpp           # 程序入口点
│   ├── MyGame.h/cpp       # 主游戏窗口类
│   ├── Items/             # 游戏物品系统
│   │   ├── Item.h/cpp     # 基础物品类
│   │   ├── Characters/    # 角色相关
│   │   ├── Armors/        # 护甲装备
│   │   ├── HeadEquipments/# 头部装备
│   │   ├── LegEquipments/ # 腿部装备
│   │   └── Maps/          # 地图相关
│   ├── Physics/           # 物理系统
│   └── Scenes/            # 游戏场景
│       ├── Scene.h/cpp    # 基础场景类
│       └── BattleScene.h/cpp # 战斗场景
├── assets/                # 游戏资源
│   ├── assets.qrc        # Qt资源文件
│   └── Items/            # 物品贴图资源
└── build/                # 构建输出目录
```

## 功能特性

### 已实现功能
- ✅ 基于Qt Graphics Framework的游戏引擎架构
- ✅ 场景管理系统 (Scene/BattleScene)
- ✅ 物品系统基础框架
- ✅ 角色、装备、地图等游戏元素的基础类结构
- ✅ 游戏主窗口和视图系统
- ✅ 资源管理系统 (QRC)

### 游戏素材
- 角色精灵图 (绿色角色：正面、蹲下、跳跃状态)
- 地图背景素材

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

## 开发计划

项目目前处于早期开发阶段，游戏具体玩法逻辑还在设计中。基础的引擎架构和资源系统已经搭建完成，后续将专注于：

- 🔄 游戏核心玩法设计
- 🔄 物理系统完善
- 🔄 战斗系统实现
- 🔄 用户输入处理
- 🔄 游戏逻辑优化

## 开发者信息

项目创建时间：2025年7月
开发框架：Qt6 + CMake
