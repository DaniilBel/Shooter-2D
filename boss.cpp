#include "boss.h"
#include "hero.h"
#include "math.h"

static const double Pi = 3.14159265358979323846264338327950288419717;
static double TwoPi = 2.0 * Pi;

static qreal normalizeAngle(qreal angle)
{
    while (angle < 0)
        angle += TwoPi;
    while (angle > TwoPi)
        angle -= TwoPi;
    return angle;
}

Boss::Boss(QGraphicsItem *hero, QObject *parent) :
    QObject(parent), QGraphicsItem()
{
    health = 100;
    maxHealth = health;
    angle = 0;
    //steps = 0;
    //countForSteps = 0;
    setRotation(angle);
    this->hero = hero;
    timer = new QTimer();
    connect(timer, &QTimer::timeout, this, &Boss::slotGameTimer);
    timer->start(15);
}

Boss::~Boss()
{

}

QRectF Boss::boundingRect() const
{
    return QRectF(-20,-20,40,40);
}

//Отрисовывем нашего босса
void Boss::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    //painter->setPen(Qt::black);
    //painter->setBrush(Qt::red);
    //painter->drawRect(-20,-10,40,30);

    QPolygon polygon;
    polygon << QPoint(0,20)<< QPoint(14,10)<< QPoint(12,-2)<< QPoint(16,-10)<< QPoint(11,-14)
               << QPoint(12,-8)<< QPoint(8,-6)<< QPoint(4,-10)<< QPoint(-4,-10)<< QPoint(-8,-6)
                  << QPoint(-12,-8)<< QPoint(-11,-14)<< QPoint(-16,-10)<< QPoint(-12,-2)<< QPoint(-14,10);
    painter->setBrush(Qt::darkRed);
    painter->drawPolygon(polygon);

    //отрисовываем HP босса
    painter->setPen(Qt::NoPen);
    painter->setBrush(Qt::red);
    painter->drawRect(-20,-20, static_cast<int>(40*health/maxHealth), 3);

    Q_UNUSED(option);
    Q_UNUSED(widget);
}

void Boss::hit(int damage)
{
    health -= damage;
    this->update(QRectF(-20,-20,40,40));

    if(health <= 0) this->deleteLater();
}


void Boss::slotGameTimer()
{

  QLineF lineToTarget(QPointF(0, 0), mapFromItem(hero, 0, 0));

  qreal angleToTarget = ::acos(lineToTarget.dx() / lineToTarget.length());
     if (lineToTarget.dy() < 0)
         angleToTarget = TwoPi - angleToTarget;
     angleToTarget = normalizeAngle((Pi - angleToTarget) + Pi / 2);

     /* В Зависимости от того, слева или справа находится герой от босса,
      * устанавливаем направление поворота босса в данном тике таймера
      * Скорость разворота зависит от угла, на который необходимо повернуться боссу
      * */
     if (angleToTarget > 0 && angleToTarget < Pi) {
         // Rotate left
         if(angleToTarget > Pi / 5){
             angle = -15;
         } else if(angleToTarget > Pi / 10){
             angle = -5;
         } else {
             angle = -1;
         }
     } else if (angleToTarget <= TwoPi && angleToTarget > (TwoPi - Pi)) {
         // Rotate right
         if(angleToTarget < (TwoPi - Pi / 5)){
             angle = 15;
         } else if(angleToTarget < (TwoPi - Pi / 10)){
             angle = 5;
         } else {
             angle = 1;
         }
     } else if(angleToTarget == 0.0) {
         angle = 0;
     }

     setRotation(rotation() + angle); // Разворачиваемся

     // Бежим в сторону героя
     if(lineToTarget.length() >= 40){
         setPos(mapToParent(0, -3 /*-(qrand() % ((4 + 1) - 1) + 1))*/));

         // Двигаем ножками, Dance, dance, Spidy !!!
         //countForSteps++;
         //if(countForSteps == 6){
         //    steps = 1;
         //    update(QRectF(-40,-50,80,100));
         //} else if (countForSteps == 12){
         //    steps = 0;
         //    update(QRectF(-40,-50,80,100));
         //} else if (countForSteps == 18){
         //    steps = 2;
         //    update(QRectF(-40,-50,80,100));
         //} else if (countForSteps == 24) {
         //    steps = 0;
         //    update(QRectF(-40,-50,80,100));
         //    countForSteps = 0;
         //}
     }

     /* Производим проверку на то, наткнулся ли босс на какой-нибудь
      * элемент на графической сцене.
      * Для этого определяем небольшую область перед боссом,
      * в которой будем искать элементы
      * */
     QList<QGraphicsItem *> foundItems = scene()->items(QPolygonF()
                                                            << mapToScene(0, 0)
                                                            << mapToScene(-24, -24)
                                                            << mapToScene(24, -24));
     /* После чего проверяем все элементы.
      * Один из них будет сам герой - с ним ничего не делаем.
      * А с остальными высылаем сигнал в ядро игры
      * */
     foreach (QGraphicsItem *item, foundItems) {
         if (item == this)
             continue;
         if(item == hero){
             MainHero *h = qgraphicsitem_cast <MainHero *> (hero);
             h->hit(5);
         }
     }

     /* Проверка выхода за границы поля
      * Если объект выходит за заданные границы, то возвращаем его назад
      * */
     if(this->x() - 10 < 0){
         this->setX(-240);       // слева
     }
     if(this->x() + 10 > 600){
         this->setX(240);        // справа
     }

     if(this->y() - 10 < 0){
         this->setY(-240);       // сверху
     }
     if(this->y() + 10 > 600){
         this->setY(240);        // снизу
     }
 }

 /* Функция паузы, отвечает за включение и отключение паузы
  * */
 void Boss::pause()
 {
     if(timer->isActive()){
         timer->stop();
     } else {
         timer->start(15);
     }
 }
