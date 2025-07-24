//
// Created by gerw on 8/21/24.
//

#ifndef QT_PROGRAMMING_2024_MAP_H
#define QT_PROGRAMMING_2024_MAP_H

#include <QGraphicsScene>
#include <QPointF>
#include <QVector>
#include <QRectF>
#include "../Item.h"

// 平台结构体定义
struct Platform {
    QRectF rect;        // 平台的矩形区域 (x, y, width, height)
    qreal height;       // 平台表面的Y坐标
    int priority;       // 优先级，数值越大优先级越高
    enum class Type {
        Normal, // 普通平台
        Grass, // 草地平台
        Ice, // 冰面平台
        Poison // 毒平台，持续扣血
    } type = Type::Normal; // 默认类型为普通平台
    
    Platform(QRectF r, qreal h, int p = 0,Type t = Type::Normal) 
        : rect(r), height(h), priority(p), type(t) {}
    
    // 检查X坐标是否在平台范围内
    bool containsX(qreal x) const {
        return x >= rect.left() && x <= rect.right();
    }
    
    // 检查点是否在平台上
    bool containsPoint(QPointF point) const {
        return rect.contains(point);
    }
};

// 墙结构体定义
struct Wall {
    QRectF rect;        // 墙的矩形区域 (x, y, width, height)
    QString name;       // 墙名称（可选，用于调试）
    
    Wall(QRectF r, QString n = "") : rect(r), name(n) {}
    
    // 检查点是否在墙上
    bool containsPoint(QPointF point) const {
        return rect.contains(point);
    }
};

class Map : public Item {
public:
    explicit Map(QGraphicsItem *parent = nullptr, const QString &pixmapPath = "");

    void scaleToFitScene(QGraphicsScene *scene);

    // 原有的简单地面高度方法（向后兼容）
    virtual qreal getFloorHeight();

    // 新的多平台系统方法
    virtual qreal getFloorHeightAt(qreal x); // 获取指定X坐标的地面高度
    
    virtual QVector<qreal> getAllFloorHeightsAt(qreal x); // 获取指定X坐标的所有平台高度
    
    virtual Platform* getNearestPlatform(QPointF position, qreal maxDistance = 50.0); // 获取最近的平台
    virtual Platform* getNearestPlatform(QPointF position1,QPointF position2, qreal maxDistance = 50.0);
    
    virtual Platform* getPlatformAt(QPointF position); // 获取指定位置的平台
    
    virtual QVector<Platform*> getPlatformsInRange(qreal xMin, qreal xMax); // 获取范围内的所有平台

    virtual QPointF getSpawnPos();

    // 墙的一些方法
    virtual QVector<Wall> getWalls();

protected:
    // 墙数据
    QVector<Wall> walls; // 墙数据，用于碰撞检测

    //初始化墙数据
    virtual void initializeWalls() = 0;
    
    // 平台数据
    QVector<Platform> platforms;
    
    // 初始化平台数据（子类实现）
    virtual void initializePlatforms() = 0;
    
    // 添加平台的辅助方法
    void addPlatform(QRectF rect, qreal height, int priority = 0, QString name = "");
    void addPlatform(qreal x, qreal y, qreal width, qreal height, int priority = 0, QString name = "");

    void initialize(); // 初始化地图数据
};


#endif //QT_PROGRAMMING_2024_MAP_H
