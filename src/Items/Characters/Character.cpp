//
// Created by gerw on 8/20/24.
//

#include <QTransform>
#include <QTimer>
#include "Character.h"
#include "../Maps/Map.h"
#include "../Medicine/Medicine.h"

Character::Character(QGraphicsItem *parent):Item(parent,"") {
//    ellipseItem = new QGraphicsEllipseItem(-5, -5, 10, 10, this);
//    // Optionally, set some properties of the ellipse
//    ellipseItem->setBrush(Qt::green);          // Fill color
//    ellipseItem->setZValue(1);
    
    // 初始化血量系统
    maxHealth = PhysicsConstants::DEFAULT_MAX_HEALTH;
    currentHealth = maxHealth;
    
    // 初始化默认武器（拳头）
    weapon = new Fist(this);
    
    // 初始化血量条UI
    setupHealthBar();
}

Character::~Character() {
    // 清理宝石
    if (currentGem) {
        delete currentGem;
        currentGem = nullptr;
    }
    
    // 清理所有buff和相关定时器
    clearAllBuffs();
}


bool Character::isLeftDown() const {
    return leftDown;
}

void Character::setLeftDown(bool leftDown) {
    Character::leftDown = leftDown;
}

bool Character::isRightDown() const {
    return rightDown;
}

void Character::setRightDown(bool rightDown) {
    Character::rightDown = rightDown;
}

bool Character::isUpDown() const {
    return upDown;
}

void Character::setUpDown(bool upDown) {
    Character::upDown = upDown;
}

bool Character::isPickDown() const {
    return pickDown;
}

void Character::setPickDown(bool pickDown) {
    Character::pickDown = pickDown;
}

void Character::setAttacking(bool attacking) {
    Character::attackDown = attacking;
}

void Character::processAttack() {
    const qreal frameTime = 11.11; // 90FPS，每帧约11.11毫秒
    
    // 更新攻击后摇计时器
    if (attackRecoveryTimer > 0) {
        attackRecoveryTimer -= frameTime;
        // 后摇结束，恢复正常显示
        if (attackRecoveryTimer <= 0) {
            updatePixmapItem("normal");
            // 恢复正常状态时保持当前方向
            setDirection(directionRight);
        }
    }
    
    // 更新攻击间隔计时器（用于长按连击）
    if (attackIntervalTimer > 0) {
        attackIntervalTimer -= frameTime;
    }
    
    // 瞬间攻击逻辑：isPerformingAttack只用作标记，立即处理后立即重置
    if (isPerformingAttack) {
        // 攻击逻辑在这一帧内立即完成
        
        // 统一使用武器系统进行攻击
        if (weapon) {
            weapon->attack();
        } 
        isPerformingAttack = false; // 立即重置
        attackTimer = 0;
        // 开始后摇显示
    }
    
    // 处理攻击输入
    if (attackDown) {
        // 检查是否可以攻击：不在攻击状态、不在后摇状态、不在间隔状态
        if (!isPerformingAttack && attackRecoveryTimer <= 0 && attackIntervalTimer <= 0) {
            // 触发瞬间攻击
            isPerformingAttack = true; // 设置标记，将在下一帧立即处理
            updatePixmapItem("attack");
            // 攻击时保持当前方向
            setDirection(directionRight);
            attackRecoveryTimer = PhysicsConstants::ATTACK_RECOVERY;
            // 设置间隔计时器，防止连击过快
            attackIntervalTimer = PhysicsConstants::ATTACK_INTERVAL;
        }
    }
    
    lastAttackDown = attackDown;
}

void Character::processHitState() {
    const qreal frameTime = 11.11; // 90FPS，每帧约11.11毫秒
    
    // 更新受击状态计时器
    if (hitRecoveryTimer > 0) {
        hitRecoveryTimer -= frameTime;
        // 受击状态结束，恢复正常显示
        if (hitRecoveryTimer <= 0) {
            isBeingHit = false;
            updatePixmapItem("normal");
            // 恢复正常状态时保持当前方向
            setDirection(directionRight);
        }
    }
}

bool Character::isCurrentlyAttacking() const {
    return isPerformingAttack;
}

QRectF Character::getAttackRange() const {
    // 根据角色朝向和武器攻击范围计算攻击范围
    QRectF characterRect = boundingRect();
    qreal range = weapon ? weapon->getAttackRange() : PhysicsConstants::ATTACK_RANGE; // 使用武器的攻击范围
    qreal attacktolerance = 40; // 边界容忍度
    if (directionRight) {
        // 向右攻击
        return QRectF(pos().x() - attacktolerance, 
                      pos().y(), 
                      range, 
                      characterRect.height());
    } else {
        // 向左攻击
        return QRectF(pos().x() + attacktolerance, 
                      pos().y(), 
                      range, 
                      characterRect.height());
    }
}

bool Character::isSquatting() const {
    return squatting;
}

void Character::setSquatting(bool squatting) {
    Character::squatting = squatting;
}

const QPointF &Character::getVelocity() const {
    return velocity;
}

void Character::setVelocity(const QPointF &velocity) {
    Character::velocity = velocity;
}

void Character::processInput() {
    // 如果角色死亡，不处理输入
    if (!isAlive()) {
        return;
    }
    
    auto newVelocity = velocity; // 保持当前的Y方向速度
    const auto baseMoveSpeed = PhysicsConstants::HORIZONTAL_MOVE_SPEED; // 基础移动速度
    const auto moveSpeed = baseMoveSpeed * getCurrentSpeedMultiplier(); // 应用增益后的移动速度
    
    // 水平移动 (只影响X方向速度)
    newVelocity.setX(0); // 重置水平速度
    
    // 处理水平移动和方向
    if (isLeftDown()) {
        newVelocity.setX(-moveSpeed);
        setDirection(false); // 设置方向为左
    }
    if (isRightDown()) {
        newVelocity.setX(moveSpeed);
        setDirection(true); // 设置方向为右
    }
    
    // 跳跃 (只在地面上时才能跳跃)
    if (isUpDown() && onGround && !isSquatting()) {
        newVelocity.setY(jumpSpeed);
        setOnGround(false);
    }

    // 处理蹲下状态
    if (isSquatting() && onGround && (hitRecoveryTimer <= 0)) {
        // 如果蹲下，设置Y方向速度为0（不在受击状态时）
        newVelocity.setY(0);
        newVelocity.setX(0); // 蹲下时不移动
        updatePixmapItem("duck");
        // 蹲下时也要保持正确的方向
        setDirection(directionRight);
    }

    // 处理正常状态的显示（只有在不处于特殊状态时）
    if (!isSquatting() && (attackRecoveryTimer <= 0) && (hitRecoveryTimer <= 0)) {
        // 保持当前方向的变换
        setDirection(directionRight);
        // 只有在不处于攻击后摇期间和受击状态期间才设置为normal
        updatePixmapItem("normal");
    }
    
    setVelocity(newVelocity);

    if (!lastPickDown && pickDown) { // first time pickDown
        picking = true;
    } else {
        picking = false;
    }
    lastPickDown = pickDown;

    // 处理攻击
    processAttack();
    
    // 处理受击状态
    processHitState();
    
    // 处理不同种类平台的逻辑
    processPlatformstypes();
}

bool Character::isPicking() const {
    return picking;
}

#pragma region Gravity System
// 重力系统方法实现
bool Character::isOnGround() const {
    return onGround;
}

void Character::setOnGround(bool onGround) {
    Character::onGround = onGround;
}

qreal Character::getGravityAcceleration() const {
    return gravityAcceleration;
}

void Character::applyGravity(qreal deltaTime) {
    if (!onGround) {
        // 应用重力加速度 (deltaTime 以毫秒为单位，需要转换为秒)
        qreal deltaTimeInSeconds = deltaTime / 1000.0;
        velocity.setY(velocity.y() + gravityAcceleration * deltaTimeInSeconds);
        
        // 限制最大下降速度
        // if (velocity.y() > maxFallSpeed) {
        //     velocity.setY(maxFallSpeed);
        // }
    }
}

void Character::handleGroundCollision(qreal groundHeight) {
    // 获取角色当前位置
    qreal characterX1 = pos().x();
    qreal characterX2 = pos().x();
    if(directionRight) {
        characterX1 = pos().x() - boundingRect().width() + edgeTolerance;
        characterX2 = pos().x() - edgeTolerance;
    } else {
        characterX1 = pos().x() + boundingRect().width() - edgeTolerance;
        characterX2 = pos().x() + edgeTolerance;
    }
    qreal characterBottom = pos().y();
    
    // 检查当前位置是否有平台
    if (map) {
        // 查找最接近的平台
        const Platform* nearestPlatform = map->getNearestPlatform(QPointF(characterX1, characterBottom + boundingRect().height()),QPointF(characterX2, characterBottom + boundingRect().height()));
        
        if (nearestPlatform && characterBottom + boundingRect().height() >= nearestPlatform->height && velocity.y() >= 0) {
            // 检查角色是否真的在平台上（X坐标范围内）
            if ((nearestPlatform->containsX(characterX1))||(nearestPlatform->containsX(characterX2))) {
                // 角色着地在平台上
                setOnGround(true);
                velocity.setY(0);
                // 调整位置确保不穿透平台
                setPos(pos().x(), nearestPlatform->height - boundingRect().height());
                return;
            }
        }
        
        // 检查是否需要离开地面
        if (onGround) {
            // 获取当前位置的所有平台高度
            QVector<qreal> heights = map->getAllFloorHeightsAt(characterX1);
            bool foundSupportingPlatform = false;
            
            for (qreal height : heights) {
                if (qAbs(characterBottom + boundingRect().height() - height) <= PhysicsConstants::GROUND_TOLERANCE) {
                    foundSupportingPlatform = true;
                    break;
                }
            }
            
            if (!foundSupportingPlatform) {
                setOnGround(false);
            }
        }
    } 
}

#pragma endregion

#pragma region Health System
// 血量系统方法实现
qreal Character::getCurrentHealth() const {
    return currentHealth;
}

qreal Character::getMaxHealth() const {
    return maxHealth;
}

void Character::setMaxHealth(qreal maxHealth) {
    Character::maxHealth = maxHealth;
    // 如果当前血量超过新的最大血量，调整到最大血量
    if (currentHealth > maxHealth) {
        currentHealth = maxHealth;
    }
}

void Character::takeDamage(qreal damage, const QString& weaponName) {
    if (damage > 0 && isAlive()) {
        // 首先应用护盾吸收
        qreal shieldedDamage = applyShieldDamage(damage, weaponName);
        
        // 然后计算武器防护减免（对剩余伤害）
        qreal protectionMultiplier = getWeaponProtectionMultiplier(weaponName);
        qreal actualDamage = shieldedDamage * protectionMultiplier;
        
        // 如果完全免疫（倍数为0），则不造成伤害
        if (protectionMultiplier == 0.0 && shieldedDamage > 0) {
            return;
        }
        
        currentHealth -= actualDamage;
        if (currentHealth < 0) {
            currentHealth = 0;
        }
        
        // 触发受击状态
        isBeingHit = true;
        hitRecoveryTimer = PhysicsConstants::HIT_RECOVERY;
        updatePixmapItem("hit");
        // 受击时保持当前方向
        setDirection(directionRight);
        
        // 更新血量条显示
        updateHealthBar();
        
        // 如果血量归零，角色死亡
        if (!isAlive()) {
            // 立即停止角色的所有活动
            handleCharacterDeath();
        }
    }
}

void Character::heal(qreal amount) {
    if (amount > 0 && isAlive()) {
        currentHealth += amount;
        if (currentHealth > maxHealth) {
            currentHealth = maxHealth;
        }
        
        // 更新血量条显示
        updateHealthBar();
    }
}

bool Character::isAlive() const {
    return currentHealth > 0;
}

qreal Character::getHealthPercentage() const {
    if (maxHealth <= 0) return 0;
    return (currentHealth / maxHealth) * 100.0;
}

#pragma endregion

#pragma region Map System

void Character::setMap(Map* map) {
    this->map = map;
}

Map* Character::getMap() const {
    return map;
}

void Character::checkWallCollisions(const QVector<Wall>& walls) {
    if (!isAlive()) {
        return;
    }

    if (walls.isEmpty()) {
        return; // 静默处理没有墙的情况
    }
    qreal CharacterX = pos().x();
    if(directionRight) {
        CharacterX -= edgeTolerance; // 角色右侧位置
    } else {
        CharacterX += edgeTolerance; // 角色左侧位置
    }
    QPointF characterPos = QPointF(CharacterX, pos().y()+ boundingRect().height());
    // 利用角色位置而不是矩形检查与每个墙的碰撞
    for (const Wall& wall : walls) {
        if (wall.containsPoint(characterPos)) {
            // 如果角色与墙碰撞，处理碰撞
            if (directionRight) {
                setPos(wall.rect.left(), pos().y());
            } else {
                setPos(wall.rect.right(), pos().y());
            }
            velocity.setX(0); // 停止水平移动
            return; // 只处理第一个碰撞
        }
    }
}
void Character::processMovementAndBounds(qreal deltaTime, const QRectF& sceneRect) {
    if (!isAlive()) {
        return;
    }
    
    // 应用重力
    applyGravity(deltaTime);
    
    // 检查地面碰撞
    if (map) {
        handleGroundCollision(map->getFloorHeightAt(pos().x()));
    }
    
    // 更新位置 (deltaTime 以毫秒为单位，需要转换为秒)
    qreal deltaTimeInSeconds = deltaTime / 1000.0;
    setPos(pos() + velocity * deltaTimeInSeconds);

    // 检查墙碰撞
    if (map) {
        checkWallCollisions(map->getWalls());
    }
    // 确保角色不会离开场景边界
    QPointF newPos = pos();
    
    // 水平边界检查
    if (newPos.x() < sceneRect.left()) {
        newPos.setX(sceneRect.left());
    } else if (newPos.x()> sceneRect.right()) {
        newPos.setX(sceneRect.right());
    }
    
    setPos(newPos);
}

void Character::processPlatformstypes() {
    if (!isAlive()) {
        return;
    }
    
    // 处理不同种类平台的逻辑
    if (map) {
        QPointF characterPos = QPointF(pos().x(), pos().y() + boundingRect().height());
        const Platform* platform = map->getPlatformAt(characterPos);
        
        // 首先检查是否离开了毒平台
        if (hasBuff("Poison Damage") && (!platform || platform->type != Platform::Type::Poison)) {
            // 角色离开了毒平台，移除毒素debuff
            removeBuff("Poison Damage");
        }
        
        if (platform) {
            switch (platform->type) {
                case Platform::Type::Normal:
                    // 普通平台逻辑
                    pixmapItem->setOpacity(1.0); // 恢复不透明度
                    jumpSpeed = PhysicsConstants::JUMP_SPEED; // 恢复正常跳跃速度
                    break;
                case Platform::Type::Grass:
                    // 草地平台逻辑
                    if (isSquatting()){
                        // 如果蹲下，设置半透明效果
                        pixmapItem->setOpacity(0.5); // 半透明
                    }else {
                        pixmapItem->setOpacity(1.0); // 恢复不透明度
                    }
                    jumpSpeed = PhysicsConstants::JUMP_SPEED; // 恢复正常跳跃速度
                    break;
                case Platform::Type::Ice:
                    // 冰面平台逻辑
                    // 速度加快
                    pixmapItem->setOpacity(1.0); // 恢复不透明度
                    velocity.setX(velocity.x() * PhysicsConstants::ICE_FRICTION);
                    jumpSpeed =1.2* PhysicsConstants::JUMP_SPEED; // 冰面跳跃速度加倍
                    break;
                case Platform::Type::Poison:
                    // 毒平台逻辑
                    pixmapItem->setOpacity(1.0); // 恢复不透明度
                    jumpSpeed = PhysicsConstants::JUMP_SPEED; // 恢复正常跳跃速度
                    
                    // 应用毒素debuff（只有在没有该debuff时才应用）
                    if (!hasBuff("Poison Damage")) {
                        BuffEffect poisonDebuff(
                            "Poison Damage",    // 名称
                            1.0,                // 速度倍数（不变）
                            -2.0,               // 血量回复速度（负值表示扣血，每秒扣2血）
                            0,                  // 持续时间（0表示无限持续，直到离开平台）
                            1000                // 触发间隔（1000ms = 1秒）
                        );
                        applyBuff(poisonDebuff);
                    }
                    break;
                default:
                    break;
            }
        }
    }
}

#pragma endregion

#pragma region Direction System

void Character::setDirection(bool right) {
    if (directionRight != right) {
        // 只有在方向真正改变时才调整位置
        if (right) {
            setPos(pos().x() + boundingRect().width(), pos().y());
        } else {
            setPos(pos().x() - boundingRect().width(), pos().y());
        }
        directionRight = right;
    }
    
    // 设置变换
    if (directionRight) {
        setTransform(QTransform().scale(-1, 1));
    } else {
        setTransform(QTransform().scale(1, 1));
    }
}

bool Character::isDirectionRight() const {
    return directionRight;
}

#pragma endregion

#pragma region Pixmap System

void Character::updatePixmapItem(const QString& condition) {
    if(characterPixmapPaths.contains(condition)) {
        QString pixmapPath = characterPixmapPaths.value(condition);
        if (!pixmapPath.isEmpty()) {
            QPixmap pixmap(pixmapPath);
            if (!pixmap.isNull()) {
                if (pixmapItem) {
                    pixmapItem->setPixmap(pixmap);
                } else {
                    pixmapItem = new QGraphicsPixmapItem(pixmap, this);
                }
            } 
        } 
    }
}

#pragma endregion

#pragma region Weapon System

Weapon* Character::equipWeapon(Weapon* newWeapon) {
    Weapon* oldWeapon = weapon;
    
    // 如果新武器为空，装备默认拳头
    if (newWeapon == nullptr) {
        weapon = new Fist(this);
    } else {
        weapon = newWeapon;
        weapon->setParentItem(this);
        // 可以在这里设置武器的相对位置
        weapon->setPos(0, 0);  // 武器相对于角色的位置
        
        // 确保武器可见并设置合适的显示层级
        weapon->setVisible(true);
        weapon->setZValue(1);  // 设置Z值，确保武器显示在角色前面
    }
    
    // 如果旧武器不是拳头，返回它；如果是拳头，删除它
    if (oldWeapon && oldWeapon->getWeaponname() != "fist") {
        oldWeapon->setParentItem(nullptr);
        return oldWeapon;
    } else if (oldWeapon) {
        delete oldWeapon;  // 删除旧的拳头
        return nullptr;
    }
    
    return nullptr;
}

Weapon* Character::unequipWeapon() {
    Weapon* oldWeapon = weapon;
    
    // 卸下武器后，自动装备拳头
    weapon = new Fist(this);
    
    // 如果卸下的武器不是拳头，返回它；如果是拳头，删除它
    if (oldWeapon && oldWeapon->getWeaponname() != "fist") {
        oldWeapon->setParentItem(nullptr);
        return oldWeapon;
    } else if (oldWeapon) {
        delete oldWeapon;  // 删除旧的拳头
        return nullptr;
    }
    
    return nullptr;
}

#pragma endregion

#pragma region Gem System

void Character::equipGem(Medicine* gem) {
    if (!gem) return;
    
    // 如果已经有宝石，先卸下并删除旧宝石
    if (currentGem) {
        Medicine* oldGem = unequipGem();
        if (oldGem) {
            delete oldGem;  // 删除旧宝石
        }
    }
    
    // 装备新宝石
    currentGem = gem;
    currentGem->setParentItem(this);
    
    // 设置宝石的显示位置（在角色右上角）
    QRectF characterBounds = boundingRect();
    qreal gemX = characterBounds.width() * 0.8;  // 右侧位置
    qreal gemY = characterBounds.top() - 10;     // 稍微在角色上方
    currentGem->setPos(gemX, gemY);
    
    // 设置宝石的显示属性
    currentGem->setVisible(true);
    currentGem->setZValue(10);  // 确保宝石显示在前面
    currentGem->setScale(0.5);  // 缩小显示尺寸
}

Medicine* Character::unequipGem() {
    Medicine* oldGem = currentGem;
    currentGem = nullptr;
    
    if (oldGem) {
        oldGem->setParentItem(nullptr);
        oldGem->setVisible(false);
    }
    
    return oldGem;
}

#pragma endregion

#pragma region Health Bar UI

void Character::setupHealthBar() {
    // 血量条的大小和相对位置
    qreal barWidth = 60;
    qreal barHeight = 8;
    qreal barX = -barWidth / 2;  // 相对于角色中心
    
    // 动态计算血量条位置，确保在角色上方
    QRectF characterBounds = boundingRect();
    qreal barY = characterBounds.top() - 20;  // 在角色顶部上方20像素
    
    // 创建血量条背景（深灰色）
    healthBarBackground = new QGraphicsRectItem(barX, barY, barWidth, barHeight, this);
    healthBarBackground->setBrush(QBrush(QColor(60, 60, 60))); // 深灰色背景
    healthBarBackground->setPen(QPen(QColor(0, 0, 0), 1)); // 黑色边框
    healthBarBackground->setZValue(100); // 确保在角色之上显示
    
    // 创建血量条前景（绿色）
    healthBarForeground = new QGraphicsRectItem(barX, barY, barWidth, barHeight, this);
    healthBarForeground->setBrush(QBrush(QColor(0, 255, 0))); // 绿色前景
    healthBarForeground->setPen(QPen(Qt::NoPen)); // 无边框
    healthBarForeground->setZValue(101); // 在背景之上
    
    // 创建血量数值文本（可选，设置为较小字体）
    healthText = new QGraphicsTextItem(this);
    healthText->setPos(barX, barY - 12); // 位于血量条上方
    healthText->setDefaultTextColor(QColor(255, 255, 255)); // 白色文字
    healthText->setFont(QFont("Arial", 7)); // 小字体
    healthText->setZValue(102); // 在最上层
    
    // 初始更新血量显示
    updateHealthBar();
}

void Character::updateHealthBar() {
    if (!healthBarBackground || !healthBarForeground || !healthText) {
        return;
    }
    
    // 如果血量条被隐藏，不更新
    if (!healthBarVisible) {
        return;
    }
    
    qreal healthPercentage = (maxHealth > 0) ? (currentHealth / maxHealth) : 0;
    
    // 更新血量条宽度
    qreal barWidth = 60;
    qreal currentWidth = barWidth * healthPercentage;
    
    // 更新前景血量条的宽度
    QRectF rect = healthBarForeground->rect();
    rect.setWidth(currentWidth);
    healthBarForeground->setRect(rect);
    
    // 根据血量百分比改变颜色
    QColor barColor;
    if (healthPercentage > 0.6) {
        barColor = QColor(0, 255, 0); // 绿色 (健康)
    } else if (healthPercentage > 0.3) {
        barColor = QColor(255, 255, 0); // 黄色 (警告)
    } else {
        barColor = QColor(255, 0, 0); // 红色 (危险)
    }
    healthBarForeground->setBrush(QBrush(barColor));
    
    // 更新血量数值文本
    QString healthString = QString("%1/%2").arg(static_cast<int>(currentHealth)).arg(static_cast<int>(maxHealth));
    healthText->setPlainText(healthString);
    
    // 如果角色死亡，显示特殊效果
    if (!isAlive()) {
        healthText->setPlainText("DEAD");
        healthText->setDefaultTextColor(QColor(255, 0, 0)); // 红色死亡文字
        // 可以添加死亡时的血量条特效
    } else {
        healthText->setDefaultTextColor(QColor(0,255,0)); // 绿色正常文字
    }
}

void Character::setHealthBarVisible(bool visible) {
    healthBarVisible = visible;
    
    if (healthBarBackground) {
        healthBarBackground->setVisible(visible);
    }
    if (healthBarForeground) {
        healthBarForeground->setVisible(visible);
    }
    if (healthText) {
        healthText->setVisible(visible);
    }
}

#pragma endregion

#pragma region Buff System

void Character::applyBuff(const BuffEffect& buff) {
    // 如果已有同名增益，先移除
    if (hasBuff(buff.name)) {
        removeBuff(buff.name);
    }
    
    // 添加新增益
    activeBuffs[buff.name] = buff;
    
    // 设置持续时间定时器
    if (buff.duration > 0) {
        QTimer* durationTimer = new QTimer(); // 不设置父对象，手动管理
        durationTimer->setSingleShot(true);
        durationTimer->setInterval(buff.duration);
        
        // 将增益名称存储在定时器属性中
        durationTimer->setProperty("buffName", buff.name);
        
        // 使用更安全的连接方式
        QObject::connect(durationTimer, &QTimer::timeout, [this, buff, durationTimer]() {
            // 检查角色对象是否仍然有效
            if (this && activeBuffs.contains(buff.name)) {
                removeBuff(buff.name);
            }
            // 自动删除定时器
            durationTimer->deleteLater();
        });
        buffTimers[buff.name] = durationTimer;
        durationTimer->start();
    }
    
    // 设置持续性效果定时器（如血量回复或持续伤害）
    if (buff.healthRegenRate != 0 && buff.tickInterval > 0) {
        QTimer* tickTimer = new QTimer(); // 不设置父对象，手动管理
        tickTimer->setInterval(buff.tickInterval);
        
        // 将增益名称存储在定时器属性中
        tickTimer->setProperty("buffName", buff.name);
        
        // 使用更安全的连接方式
        QObject::connect(tickTimer, &QTimer::timeout, [this, buff]() {
            // 检查角色对象是否仍然有效且buff仍然存在
            if (this && activeBuffs.contains(buff.name)) {
                const BuffEffect& currentBuff = activeBuffs[buff.name];
                if (currentBuff.healthRegenRate > 0) {
                    // 正值：治疗
                    heal(currentBuff.healthRegenRate);
                } else if (currentBuff.healthRegenRate < 0) {
                    // 负值：伤害
                    takeDamage(-currentBuff.healthRegenRate);
                }
            }
        });
        buffTickTimers[buff.name] = tickTimer;
        tickTimer->start();
    }
}

void Character::removeBuff(const QString& buffName) {
    // 移除增益效果
    activeBuffs.remove(buffName);
    
    // 清理持续时间定时器
    if (buffTimers.contains(buffName)) {
        QTimer* timer = buffTimers[buffName];
        if (timer) {
            timer->stop();
            timer->deleteLater();
        }
        buffTimers.remove(buffName);
    }
    
    // 清理触发定时器
    if (buffTickTimers.contains(buffName)) {
        QTimer* timer = buffTickTimers[buffName];
        if (timer) {
            timer->stop();
            timer->deleteLater();
        }
        buffTickTimers.remove(buffName);
    }
}

void Character::clearAllBuffs() { 
    // 停止并删除所有持续时间定时器
    for (auto it = buffTimers.begin(); it != buffTimers.end(); ++it) {
        if (it.value()) {
            it.value()->stop();
            it.value()->deleteLater();
        }
    }
    buffTimers.clear();
    
    // 停止并删除所有触发定时器
    for (auto it = buffTickTimers.begin(); it != buffTickTimers.end(); ++it) {
        if (it.value()) {
            it.value()->stop();
            it.value()->deleteLater();
        }
    }
    buffTickTimers.clear();
    
    // 清理增益效果
    activeBuffs.clear();
}

qreal Character::getCurrentSpeedMultiplier() const {
    qreal totalMultiplier = 1.0;
    
    for (const BuffEffect& buff : activeBuffs.values()) {
        totalMultiplier *= buff.speedMultiplier;
    }
    
    return totalMultiplier;
}

qreal Character::getWeaponProtectionMultiplier(const QString& weaponName) const {
    if (weaponName.isEmpty()) {
        return 1.0; // 没有指定武器时，正常伤害
    }
    
    qreal totalProtection = 1.0;
    
    // 遍历所有激活的增益效果，查找武器防护
    for (const BuffEffect& buff : activeBuffs.values()) {
        qreal protection = buff.getWeaponProtection(weaponName);
        if (protection < totalProtection) {
            totalProtection = protection; // 取最强的防护效果
        }
    }
    
    return totalProtection;
}

qreal Character::applyShieldDamage(qreal damage, const QString& weaponName) {
    if (weaponName.isEmpty() || damage <= 0) {
        return damage;
    }
    
    qreal remainingDamage = damage;
    QStringList buffNamesToRemove;
    
    // 遍历所有激活的增益效果，查找有护盾且对该武器有效的buff
    for (auto it = activeBuffs.begin(); it != activeBuffs.end(); ++it) {
        BuffEffect& buff = it.value();
        
        // 检查这个buff是否对当前武器有防护效果
        if (buff.weaponProtections.contains(weaponName) && buff.damageShield > 0) {
            qreal protectionMultiplier = buff.getWeaponProtection(weaponName);
            
            // 对于绿宝石护盾，使用特殊逻辑：护盾承受75%伤害，角色承受25%伤害
            if (buff.name == "绿宝石护盾") {
                qreal shieldDamage = remainingDamage * 0.75;  // 护盾承受75%伤害
                qreal characterDamage = remainingDamage * 0.25; // 角色承受25%伤害
                
                // 护盾尝试承受75%的伤害
                qreal unabsorbedShieldDamage = buff.consumeShield(shieldDamage);
                
                // 最终角色承受的伤害 = 25%基础伤害 + 护盾无法承受的部分
                remainingDamage = characterDamage + unabsorbedShieldDamage;
            } else {
                // 原有逻辑：用于蓝宝石等其他防护
                qreal reducedDamage = remainingDamage * protectionMultiplier;
                qreal damageToAbsorb = remainingDamage - reducedDamage;
                qreal unabsorbedDamage = buff.consumeShield(damageToAbsorb);
                remainingDamage = reducedDamage + unabsorbedDamage;
            }
            
            // 如果护盾耗尽，标记为需要移除
            if (buff.isShieldDepleted()) {
                buffNamesToRemove.append(buff.name);
            }
            
            break; // 一次只使用一个护盾
        }
    }
    
    // 移除耗尽的护盾buff
    for (const QString& buffName : buffNamesToRemove) {
        removeBuff(buffName);
        
        // 如果是绿宝石护盾耗尽，也要移除装备的绿宝石
        if (buffName == "绿宝石护盾" && hasGem()) {
            Medicine* gem = getCurrentGem();
            if (gem && gem->getTypeName() == "GemGreen") {
                Medicine* removedGem = unequipGem();
                if (removedGem) {
                    delete removedGem;
                }
            }
        }
    }
    
    return remainingDamage;
}

bool Character::hasBuff(const QString& buffName) const {
    return activeBuffs.contains(buffName);
}

void Character::updateBuffs(qreal deltaTime) {
    // 目前定时器系统自动处理增益更新
    // 这个方法保留用于未来可能的手动更新需求
    Q_UNUSED(deltaTime)
}

void Character::handleCharacterDeath() {  
    // 1. 停止所有输入动作
    setLeftDown(false);
    setRightDown(false);
    setUpDown(false);
    setSquatting(false);
    setAttacking(false);
    setPickDown(false);
    
    // 2. 清理所有buff和定时器
    clearAllBuffs();
    
    // 3. 停止所有攻击动作
    isPerformingAttack = false;
    attackTimer = 0;
    attackIntervalTimer = 0;
    attackRecoveryTimer = 0;
    
    // 4. 停止受击状态
    isBeingHit = false;
    hitRecoveryTimer = 0;
    
    // 5. 停止velocity，让角色不再移动
    velocity = QPointF(0, 0);
}

#pragma endregion
