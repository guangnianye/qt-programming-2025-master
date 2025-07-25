//
// Created by user on 7/24/25
//

#include "MedicineManager.h"
#include "../Characters/Character.h"
#include "../../Physics/PhysicsConstants.h"
#include "HealthPotion.h"
#include "EnergyBoost.h"
#include "MagicPotion.h"
#include "GemBlue.h"
#include "GemGreen.h"

MedicineManager::MedicineManager(QGraphicsScene* scene, QObject* parent)
    : QObject(parent), gameScene(scene), dropTimer(new QTimer(this)),
      dropInterval(DEFAULT_DROP_INTERVAL), dropProbability(DEFAULT_DROP_PROBABILITY) {
    
    // 设置默认掉落区域（场景宽度范围）
    if (gameScene) {
        QRectF sceneRect = gameScene->sceneRect();
        dropArea = QRectF(sceneRect.left(), sceneRect.top(), sceneRect.width(), sceneRect.height());
    }
    
    // 连接定时器
    connect(dropTimer, &QTimer::timeout, this, &MedicineManager::dropRandomMedicine);
}

MedicineManager::~MedicineManager() {
    stopMedicineDrops();
}

void MedicineManager::startMedicineDrops() {
    if (!dropTimer->isActive()) {
        dropTimer->start(dropInterval);
    }
}

void MedicineManager::stopMedicineDrops() {
    if (dropTimer->isActive()) {
        dropTimer->stop();
    }
}

void MedicineManager::dropMedicine(MedicineDropType medicineType, const QPointF& position) {
    if (!gameScene) return;
    
    Medicine* medicine = createMedicine(medicineType);
    if (medicine) {
        medicine->setPos(position);
        gameScene->addItem(medicine);
        
        // 添加物理效果（掉落）
        medicine->setFlag(QGraphicsItem::ItemIsMovable, false);
        medicine->setFlag(QGraphicsItem::ItemIsSelectable, false);
    }
}

Medicine* MedicineManager::createMedicine(MedicineDropType medicineType, QGraphicsItem* parent) {
    switch (medicineType) {
        case MedicineDropType::HealthPotion:
            return new HealthPotion(parent);
        case MedicineDropType::EnergyBoost:
            return new EnergyBoost(parent);
        case MedicineDropType::MagicPotion:
            return new MagicPotion(parent);
        case MedicineDropType::GemBlue:
            return new GemBlue(parent);
        case MedicineDropType::GemGreen:
            return new GemGreen(parent);
        default:
            return nullptr;
    }
}

MedicineDropType MedicineManager::getRandomMedicineType() {
    // 使用权重分配系统
    QVector<MedicineDropType> weightedMedicines;
    
    // 健康药水：权重10（最常见）
    for (int i = 0; i < 11; ++i) {
        weightedMedicines.append(MedicineDropType::HealthPotion);
    }

    // 能量提升：权重8（较常见）
    for (int i = 0; i < 10; ++i) {
        weightedMedicines.append(MedicineDropType::EnergyBoost);
    }

    // 魔法药水：权重2（罕见）
    for (int i = 0; i < 2; ++i) {
        weightedMedicines.append(MedicineDropType::MagicPotion);
    }
    
    // 蓝宝石：权重1（极稀有）
   for (int i = 0; i < 1; ++i) {
        weightedMedicines.append(MedicineDropType::GemBlue);
    }
    
    // 绿宝石：权重1（极稀有）
    for (int i = 0; i < 1; ++i) {
        weightedMedicines.append(MedicineDropType::GemGreen);
    }

    if (weightedMedicines.isEmpty()) {
        return MedicineDropType::HealthPotion; // 默认返回健康药水
    }
    
    int randomIndex = QRandomGenerator::global()->bounded(weightedMedicines.size());
    return weightedMedicines[randomIndex];
}

bool MedicineManager::handleMedicinePickup(Character* character, Medicine* medicine) {
    if (!character || !medicine) return false;
    
    // 检查拾取距离
    QPointF characterPos = character->pos();
    QPointF medicinePos = medicine->pos();
    qreal distance = QLineF(characterPos, medicinePos).length();
    
    if (distance <= PhysicsConstants::PICKUP_DISTANCE) {
        // 应用药物效果
        medicine->applyEffect(character);
        
        // 从场景中移除药物
        if (medicine->scene()) {
            medicine->scene()->removeItem(medicine);
        }
        
        // 删除药物对象
        delete medicine;
        
        return true;
    }
    
    return false;
}

void MedicineManager::dropRandomMedicine() {
    if (!gameScene) return;
    
    // 根据概率决定是否掉落
    qreal randomValue = QRandomGenerator::global()->generateDouble();
    if (randomValue > dropProbability) {
        return; // 这次不掉落
    }
    
    // 获取随机药物类型并掉落
    MedicineDropType medicineType = getRandomMedicineType();
    QPointF dropPosition = getRandomDropPosition();
    dropMedicine(medicineType, dropPosition);
}

QPointF MedicineManager::getRandomDropPosition() const {
    if (dropArea.isEmpty()) {
        return QPointF(0, DROP_HEIGHT_OFFSET);
    }
    
    qreal randomX = dropArea.left() + 
                   QRandomGenerator::global()->generateDouble() * dropArea.width();
    // 让药物从屏幕上方开始掉落，实现自然掉落效果
    qreal dropY = DROP_HEIGHT_OFFSET;  // 从屏幕上方开始掉落
    
    return QPointF(randomX, dropY);
}
