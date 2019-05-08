#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QTimer>
#include <QDebug>

#include <hero.h>
#include <customscene.h>
#include <bullet.h>
#include <boss.h>


namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();
    static void slotHitTarget(QGraphicsItem *item);
    void slotSetHPBar();
    Ui::Widget *ui;

private:

    CustomScene  *scene;
    MainHero *mainHero;
    QTimer *timerTarget;
    static QList<QGraphicsItem *> bosses;
    Boss *boss;


private slots:
    void slotBullet(QPointF start, QPointF end); /// Слот для создания пули
    void slotGameOver();

    //void slotCreateTarget();

public slots:

};

#endif // WIDGET_H
