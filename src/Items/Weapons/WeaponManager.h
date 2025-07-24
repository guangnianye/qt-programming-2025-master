//
// Created by user on 7/23/25
//

#ifndef QT_PROGRAMMING_2024_WEAPONMANAGER_H
#define QT_PROGRAMMING_2024_WEAPONMANAGER_H

#include <QObject>
#include <QTimer>
#include <QGraphicsScene>
#include <QPointF>
#include <QVector>
#include <QRandomGenerator>
#include "Weapon.h"

class Character;
class RangedWeapon;

// 武器类型枚举，用于随机生成
enum class WeaponDropType {
    ThrowingStone,
    Sword,
    Raygun,
    RaygunBig
};

// 武器管理器类
class WeaponManager : public QObject {
    Q_OBJECT

public:
    explicit WeaponManager(QGraphicsScene* scene, QObject* parent = nullptr);
    virtual ~WeaponManager();
    
    // 开始武器掉落
    void startWeaponDrops();
    
    // 停止武器掉落
    void stopWeaponDrops();
    
    // 手动投放武器
    void dropWeapon(WeaponDropType weaponType, const QPointF& position);
    
    // 创建指定类型的武器
    static Weapon* createWeapon(WeaponDropType weaponType, QGraphicsItem* parent = nullptr);
    
    // 获取随机武器类型
    static WeaponDropType getRandomWeaponType();
    
    // 处理角色拾取武器
    static Weapon* handleWeaponPickup(Character* character, Weapon* newWeapon);
    
    // 移除用完的远程武器
    static void removeDepletedRangedWeapon(Character* character);
    
    // 设置掉落参数
    void setDropInterval(int milliseconds) { dropInterval = milliseconds; }
    void setDropArea(const QRectF& area) { dropArea = area; }

private slots:
    // 定期掉落武器
    void dropRandomWeapon();
    
private:
    QGraphicsScene* gameScene;
    QTimer* dropTimer;
    
    // 掉落参数
    int dropInterval;           // 掉落间隔（毫秒）
    QRectF dropArea;           // 掉落区域
    
    // 默认掉落参数
    static constexpr int DEFAULT_DROP_INTERVAL = 5000;  // 5秒
    static constexpr int DROP_HEIGHT_OFFSET = -200;     // 从屏幕上方掉落
    
    // 获取随机掉落位置
    QPointF getRandomDropPosition() const;
};

#endif //QT_PROGRAMMING_2024_WEAPONMANAGER_H
