#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <QObject>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QPainter>
#include <QPolygon>
#include <QTimer>
#include <QDebug>
#include <QCursor>
//#include <QOpenGLTexture>
#include <QMessageBox>
#include <windows.h>
//#include <widget.h>

class MainHero : public QObject, public QGraphicsItem
{
    Q_OBJECT
public:
    explicit MainHero(QObject *parent = nullptr);
    ~MainHero();
    int hit(int damage);

signals:
    void signalBullet(QPointF start, QPointF end);
    void signalCheckGameOver();


public slots:

    //void slotdownHP();
    void slotTarget(QPointF point);
    void slotShot(bool shot);
    void slotGameTimer();
    void slotSetHP();

private:
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QPainterPath shape() const;
    //QOpenGLTexture *m_texture;
private slots:

    void slotBulletTimer();

private:
    bool shot;
    //static QList<QGraphicsItem *> heroes;
    QTimer *bulletTimer;
    QTimer *gameTimer;
    QPointF boss;
    int health;
    int maxHealth;
};

#endif // TRIANGLE_H
