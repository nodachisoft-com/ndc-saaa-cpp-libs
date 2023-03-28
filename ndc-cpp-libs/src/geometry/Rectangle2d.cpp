#include "Rectangle2d.hpp"

using namespace nl;

Rectangle2d::Rectangle2d() : Lx(0.0f), Rx(0.0f), Uy(0.0f), By(0.0f) {}

Rectangle2d::Rectangle2d(const Point2d LeftUpperPoint, const Point2d RightButtomPoint)
{
    set2PointAsRectangle(LeftUpperPoint, RightButtomPoint);
}

void Rectangle2d::set2PointAsRectangle(const Point2d p1, const Point2d p2)
{
    Lx = (p1.x < p2.x) ? p1.x : p2.x; // 左 x 座標を求める
    Rx = (p1.x < p2.x) ? p2.x : p1.x; // 右 x 座標を求める
    Uy = (p1.y < p2.y) ? p1.y : p2.y; // 上 y 座標を求める
    By = (p1.y < p2.y) ? p2.y : p1.y; // 下 y 座標を求める
}

Point2d Rectangle2d::getLeftUpperPoint()
{
    Vector2d result(Lx, Uy);
    return result;
}

Point2d Rectangle2d::getRightBottomPoint()
{
    Vector2d result(Rx, By);
    return result;
}

float Rectangle2d::getWidth()
{
    return Rx - Lx;
}

float Rectangle2d::getHeight()
{
    return By - Uy;
}
