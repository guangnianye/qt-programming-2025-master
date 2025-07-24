//
// Created by user on 7/24/25
//

#include "MedicineManager.h"
#include "../Characters/Character.h"
#include "../../Physics/PhysicsConstants.h"

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
    // 目前没有具体药物类型，返回nullptr
    // 未来在这里实现具体药物的创建逻辑
    Q_UNUSED(medicineType)
    Q_UNUSED(parent)
    return nullptr;
}

MedicineDropType MedicineManager::getRandomMedicineType() {
    // 目前没有具体药物类型，需要等待具体药物实现
    // 未来在这里实现随机药物类型选择
    // 使用权重分配，类似武器系统
    
    // QVector<MedicineDropType> weightedMedicines;
    // ... 添加权重逻辑
    
    // 临时返回，等待实现
    return static_cast<MedicineDropType>(0);
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
    
    // 目前没有具体药物类型，所以暂时不执行掉落
    // 未来实现：
    // MedicineDropType medicineType = getRandomMedicineType();
    // QPointF dropPosition = getRandomDropPosition();
    // dropMedicine(medicineType, dropPosition);
}

QPointF MedicineManager::getRandomDropPosition() const {
    if (dropArea.isEmpty()) {
        return QPointF(0, DROP_HEIGHT_OFFSET);
    }
    
    qreal randomX = dropArea.left() + 
                   QRandomGenerator::global()->generateDouble() * dropArea.width();
    qreal dropY = dropArea.top() + DROP_HEIGHT_OFFSET;
    
    return QPointF(randomX, dropY);
}
