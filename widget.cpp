#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    // Устанавливаем параметры окна приложения
    this->resize(600,700);
    this->setFixedSize(600,600);

    ui->setupUi(this);
    scene   = new CustomScene();    // Инициализируем кастомизированную сцену

    //ui->label->setFixedSize(100, 20);
    //ui->label->setText("100");
    //ui->label->setStyleSheet("QLabel { background-color : red;"
    //                         "color : white;"
    //                         "text-align : left}");

    ui->progressBar->setMinimum(0);
    ui->progressBar->setMaximum(100);
    ui->progressBar->setValue(100);

    //connect(mainHero, &MainHero::hit, this, &Widget::slotSetHPBar);

    ui->graphicsView->setScene(scene);  // Устанавливаем графическую сцену в graphicsView
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);    // Устанавливаем сглаживание
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff); // Отключаем скроллбар по вертикали
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff); // Отключаем скроллбар по горизонтали

    scene->setSceneRect(0,0,520,520);   // Устанавливаем размеры графической сцены

    // Создаем кастомизированный курсор из ресурсного файла
    QCursor cursor = QCursor(QPixmap(":/cursor/cursorTarget.png"));
    ui->graphicsView->setCursor(cursor);    // Устанавливаем курсор в QGraphicsView
    mainHero = new MainHero();  // Инициализируем треугольник
    mainHero->setPos(60,60);  // Устанавливаем стартовую позицию треугольника
    mainHero->setZValue(2);
    scene->addItem(mainHero);   // Добавляем треугольник на графическую сцену

    //Target *target = new Target();
    boss = new Boss(mainHero);
    boss->setPos(300, 300);
    boss->setZValue(-1);
    scene->addItem(boss);
    bosses.append(boss);

    //подключаем сигнал на проверку получения урона
    //connect(mainHero, &MainHero::hit, this, &Widget::slotSetHPBar);
    /* Подключаем сигнал от босса на проверку состояния GameOver
     * */
    connect(mainHero, &MainHero::signalCheckGameOver, this, &Widget::slotGameOver);

    ui->graphicsView->setMouseTracking(true);

    connect(scene, &CustomScene::signalTargetCoordinate, mainHero, &MainHero::slotTarget);
    connect(scene, &CustomScene::signalShot, mainHero, &MainHero::slotShot);
    connect(mainHero, &MainHero::signalBullet, this, &Widget::slotBullet);

    scene->addRect(0,0,520,20,QPen(Qt::NoPen),QBrush(Qt::black));
    scene->addRect(0,0,20,520,QPen(Qt::NoPen),QBrush(Qt::black));
    scene->addRect(0,500,520,20,QPen(Qt::NoPen),QBrush(Qt::black));
    scene->addRect(500,0,20,520,QPen(Qt::NoPen),QBrush(Qt::black));

  //  timerTarget = new QTimer();
//   Widget::slotCreateTarget();
  //  connect(timerTarget, &QTimer::timeout, this, &Widget::slotCreateTarget);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::slotBullet(QPointF start, QPointF end)
{
    /// Добавляем на сцену пулю
    Bullet *bullet = new Bullet(start, end, mainHero);
    bullet->setCallbackFunc(slotHitTarget);
    scene->addItem(bullet);
    slotSetHPBar();
}

//void Widget::slotCreateTarget()
//{
    //Target *target = new Target();
    //scene->addItem(target);
    //target->setPos(400, 400);
    //target->setZValue(-1);
    //targets.append(target);
//}

void Widget::slotHitTarget(QGraphicsItem *item)
{
    foreach (QGraphicsItem *targ, bosses) {
        if(targ == item){
            /// Кастуем объект из списка в класс Target
           Boss *t = qgraphicsitem_cast <Boss *> (targ);
            t->hit(5);
        }
    }

}

void Widget::slotSetHPBar(){

}

//int MainHero::hit(int damage)
//{
//    health -= damage;
//    //Widget *w = new Widget;
//    //w->slotSetHPBar(health);
//    //w->ui->progressBar->setValue(health);
//    //delete w;
//    this->update(QRectF(-20,-20,40,40));
//    if(health <= 0) {
//        //посылаем сигнал, что герою плохо
//        slotSetHP();
//    }
//}

void Widget::slotGameOver(){
    QMessageBox::warning(this, "title", "VI PIDOR BUGAGAGAGAGA");

    boss->deleteLater();
    mainHero->deleteLater();

    this->close();
}

//void Widget::slotHitHero(QGraphicsItem *item)
//{
//    foreach (QGraphicsItem *targ, bosses) {
//        if(targ == item){
//            /// Кастуем объект из списка в класс Target
//           Boss *t = qgraphicsitem_cast <Boss *> (targ);
//            t->hit(5);
//        }
//    }
//
//}

QList<QGraphicsItem *> Widget::bosses;
//<QGraphicsItem *> Widget::heroes;
