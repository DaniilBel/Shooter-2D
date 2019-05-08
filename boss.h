#ifndef TARGET_H
#define TARGET_H

#include <QObject>
#include <QGraphicsItem>
#include <QPainter>
#include <QGraphicsScene>
#include <QTimer>
#include <QDebug>
//#include <hero.h>

class Boss : public QObject, public QGraphicsItem
{
    Q_OBJECT
public:
    //explicit Target( QObject *parent = 0, QGraphicsItem *hero = 0);
    explicit Boss( QGraphicsItem *hero, QObject *parent = nullptr);
    ~Boss();
    void hit(int damage);
    void pause();

signals:


public slots:
   void slotGameTimer();
   void setHP(int);

protected:
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private:
    qreal angle; //угол поворота
    //int steps;          // Номер положения ножек паука
    //int countForSteps;
    int health;
    int maxHealth;
    QTimer      *timer;
    QGraphicsItem  *hero; //цель босса
};

#endif // TARGET_H
