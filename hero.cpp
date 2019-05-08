#include "hero.h"
#include <math.h>

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

MainHero::MainHero(QObject *parent) :
    QObject(parent), QGraphicsItem()
{
    health = 100;
    maxHealth = health;
    setRotation(0);
    //m_texture = new QOpenGLTexture(QImage(":/hero/mainHero.png"));
    boss = QPointF(0,0);
    shot = false;

    gameTimer = new QTimer();

    connect(gameTimer, &QTimer::timeout, this, &MainHero::slotGameTimer);
    gameTimer->start(5);
    bulletTimer = new QTimer();
    connect(bulletTimer, &QTimer::timeout, this, &MainHero::slotBulletTimer);
    bulletTimer->start(1000/6);
}

MainHero::~MainHero()
{

}

int MainHero::hit(int damage)
{
    health -= damage;
    this->update(QRectF(-20,-20,40,40));
    if(health <= 0) {
        //посылаем сигнал, что герою плохо
        slotSetHP();
    }
}

void MainHero::slotSetHP(){
    if(health <= 0){
        emit signalCheckGameOver();
    }
}

QRectF MainHero::boundingRect() const
{
    return QRectF(-12,-15,24,30);
}

QPainterPath MainHero::shape() const
{
    QPainterPath path;
    path.addEllipse(boundingRect());
    return path;
}

//отрисовка главного героя
void MainHero::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QPolygon polygon;
    polygon << QPoint(0,-15) << QPoint(12,15) << QPoint(0,8) << QPoint(-12,15);
    painter->setBrush(Qt::darkGray);
    painter->drawPolygon(polygon);

    //painter->setPen(Qt::NoPen);
    //painter->setBrush(Qt::red);
    //painter->drawRect(-20, -20, static_cast<int>(40*health/maxHealth), 3);

    Q_UNUSED(option);
    Q_UNUSED(widget);
}

void MainHero::slotTarget(QPointF point)
{
    boss = point;
    QLineF lineToTarget(QPointF(0, 0), mapFromScene(boss));
    qreal angleToTarget = ::acos(lineToTarget.dx() / lineToTarget.length());
    if (lineToTarget.dy() < 0)
        angleToTarget = TwoPi - angleToTarget;
    angleToTarget = normalizeAngle((Pi - angleToTarget) + Pi / 2);
    if (angleToTarget >= 0 && angleToTarget < Pi) {
        /// Rotate left
        setRotation(rotation() - angleToTarget * 180 /Pi);
    } else if (angleToTarget <= TwoPi && angleToTarget > Pi) {
        /// Rotate right
        setRotation(rotation() + (angleToTarget - TwoPi )* (-180) /Pi);
    }
}

void MainHero::slotGameTimer()
{

    if(GetAsyncKeyState('A')){
        this->setX(this->x() - 1);
        if(!scene()->collidingItems(this).isEmpty()){
            this->setX(this->x() + 1);
        }
    }
    if(GetAsyncKeyState('D')){
        this->setX(this->x() + 1);
        if(!scene()->collidingItems(this).isEmpty()){
            this->setX(this->x() - 1);
        }
    }
    if(GetAsyncKeyState('W')){
        this->setY(this->y() - 1);
        if(!scene()->collidingItems(this).isEmpty()){
            this->setY(this->y() + 1);
        }
    }
    if(GetAsyncKeyState('S')){
        this->setY(this->y() + 1);
        if(!scene()->collidingItems(this).isEmpty()){
            this->setY(this->y() - 1);
        }
    }

    if(this->x() - 30 < 0){
        this->setX(30);         /// слева
    }
    if(this->x() + 30 > 520){
        this->setX(520 - 30);   /// справа
    }

    if(this->y() - 30 < 0){
        this->setY(30);         /// сверху
    }
    if(this->y() + 30 > 520){
        this->setY(520 - 30);   /// снизу
    }

    QLineF lineToTarget(QPointF(0, 0), mapFromScene(boss));
    qreal angleToTarget = ::acos(lineToTarget.dx() / lineToTarget.length());
    if (lineToTarget.dy() < 0)
        angleToTarget = TwoPi - angleToTarget;
    angleToTarget = normalizeAngle((Pi - angleToTarget) + Pi / 2);

    if (angleToTarget >= 0 && angleToTarget < Pi) {

        setRotation(rotation() - angleToTarget * 180 /Pi);
    } else if (angleToTarget <= TwoPi && angleToTarget > Pi) {

        setRotation(rotation() + (angleToTarget - TwoPi )* (-180) /Pi);
    }
}

void MainHero::slotBulletTimer()
{
    if(shot) emit signalBullet(mapToScene(0,-25), boss);

}

void MainHero::slotShot(bool shot)
{
    this->shot = shot;
}
