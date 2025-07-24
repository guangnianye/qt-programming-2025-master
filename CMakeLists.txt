cmake_minimum_required(VERSION 3.28)
project(qt_programming_2024)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

add_executable(qt_programming_2024 src/main.cpp
        src/MyGame.cpp
        src/MyGame.h
        src/Items/Item.cpp
        src/Items/Item.h
        src/Items/Characters/Character.cpp
        src/Items/Characters/Character.h
        src/Items/Characters/BuffSystem.h
        src/Items/Characters/Green.cpp
        src/Items/Characters/Green.h
        src/Scenes/Scene.cpp
        src/Scenes/Scene.h
        src/Scenes/BattleScene.cpp
        src/Scenes/BattleScene.h
        src/Scenes/ChooseBattlefieldScene.cpp
        src/Scenes/ChooseBattlefieldScene.h
        assets/assets.qrc
        src/Items/Maps/Map.cpp
        src/Items/Maps/Map.h
        src/Items/Maps/Battlefield.cpp
        src/Items/Maps/Battlefield.h
        src/Items/Mountable.cpp
        src/Items/Mountable.h
        src/Items/Weapons/Weapon.cpp
        src/Items/Weapons/Weapon.h
        src/Items/Weapons/Fist.cpp
        src/Items/Weapons/Fist.h
        src/Items/Weapons/MeleeWeapon.cpp
        src/Items/Weapons/MeleeWeapon.h
        src/Items/Weapons/RangedWeapon.cpp
        src/Items/Weapons/RangedWeapon.h
        src/Items/Weapons/Sword.cpp
        src/Items/Weapons/Sword.h
        src/Items/Weapons/Projectile.cpp
        src/Items/Weapons/Projectile.h
        src/Items/Weapons/ThrowingStone.cpp
        src/Items/Weapons/ThrowingStone.h
        src/Items/Weapons/Raygun.cpp
        src/Items/Weapons/Raygun.h
        src/Items/Weapons/RaygunBig.cpp
        src/Items/Weapons/RaygunBig.h
        src/Items/Weapons/WeaponManager.cpp
        src/Items/Weapons/WeaponManager.h
        src/Items/Pickable.cpp
        src/Items/Pickable.h
        src/Items/Medicine/Medicine.cpp
        src/Items/Medicine/Medicine.h
        src/Items/Medicine/MedicineManager.cpp
        src/Items/Medicine/MedicineManager.h
        src/Items/Medicine/HealthPotion.cpp
        src/Items/Medicine/HealthPotion.h
        src/Items/Medicine/EnergyBoost.cpp
        src/Items/Medicine/EnergyBoost.h
        src/Items/Medicine/MagicPotion.cpp
        src/Items/Medicine/MagicPotion.h
        src/Items/PickupManager.cpp
        src/Items/PickupManager.h
        src/Physics/PhysicsConstants.h)
target_link_libraries(qt_programming_2024
        Qt::Core
        Qt::Gui
        Qt::Widgets
)

