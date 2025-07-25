//
// Created by user on 7/24/25
//

#ifndef QT_PROGRAMMING_2024_MEDICINEMANAGER_H
#define QT_PROGRAMMING_2024_MEDICINEMANAGER_H

#include <QObject>
#include <QTimer>
#include <QGraphicsScene>
#include <QPointF>
#include <QVector>
#include <QRandomGenerator>
#include "Medicine.h"

class Character;

// 药物类型枚举，用于随机生成
enum class MedicineDropType {
    HealthPotion,    // 健康药水
    EnergyBoost,     // 能量提升
    MagicPotion,     // 魔法药水
    GemBlue,         // 蓝宝石（武器防护）
    GemGreen,        // 绿宝石（远程武器护盾）
    // 未来可以添加更多药物类型
};

// 药物管理器类
class MedicineManager : public QObject {
    Q_OBJECT

public:
    explicit MedicineManager(QGraphicsScene* scene, QObject* parent = nullptr);
    virtual ~MedicineManager();
    
    // 开始药物掉落
    void startMedicineDrops();
    
    // 停止药物掉落
    void stopMedicineDrops();
    
    // 手动投放药物
    void dropMedicine(MedicineDropType medicineType, const QPointF& position);
    
    // 创建指定类型的药物
    static Medicine* createMedicine(MedicineDropType medicineType, QGraphicsItem* parent = nullptr);
    
    // 获取随机药物类型
    static MedicineDropType getRandomMedicineType();
    
    // 处理角色拾取药物
    static bool handleMedicinePickup(Character* character, Medicine* medicine);
    
    // 设置掉落参数
    void setDropInterval(int milliseconds) { dropInterval = milliseconds; }
    void setDropArea(const QRectF& area) { dropArea = area; }
    
    // 设置药物掉落概率（相对于武器）
    void setDropProbability(qreal probability) { dropProbability = probability; }

private slots:
    // 定期掉落药物
    void dropRandomMedicine();
    
private:
    QGraphicsScene* gameScene;
    QTimer* dropTimer;
    
    // 掉落参数
    int dropInterval;           // 掉落间隔（毫秒）
    QRectF dropArea;           // 掉落区域
    qreal dropProbability;     // 掉落概率（0.0-1.0）
    
    // 默认掉落参数
    static constexpr int DEFAULT_DROP_INTERVAL = 8000;  // 8秒（比武器稍慢）
    static constexpr int DROP_HEIGHT_OFFSET = -200;     // 从屏幕上方掉落
    static constexpr qreal DEFAULT_DROP_PROBABILITY = 0.7; // 70%概率掉落
    
    // 获取随机掉落位置
    QPointF getRandomDropPosition() const;
};

#endif //QT_PROGRAMMING_2024_MEDICINEMANAGER_H
