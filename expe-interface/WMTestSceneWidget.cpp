#include <math.h>
#include <QList>
#include <QPoint>
#include <QPainter>
#include <QDebug>
#include "WMTestSceneWidget.h"

struct WMTestSceneWidgetData {
    WMTestSceneWidgetData() :
        circleradius(50), circlecolor(QColor("yellow")) {}

    QList<QPoint> centres;
    int circleradius;
    QColor circlecolor;
};

WMTestSceneWidget::WMTestSceneWidget(QWidget *parent) :
    QWidget(parent),
    d(new WMTestSceneWidgetData)
{
    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);
}

WMTestSceneWidget::~WMTestSceneWidget()
{
    delete d;
}

QSize WMTestSceneWidget::minimumSizeHint() const
{
    return QSize(100, 100);
}

QSize WMTestSceneWidget::sizeHint() const
{
    return QSize(400, 200);
}

double dist(QPoint p, QPoint q)
{
    return sqrt(pow((p.x() - q.x()), 2.0) + pow((p.y() - q.y()), 2.0));
}

int randrange(int min, int max)
{
    return min + (rand() % (int)(max - min + 1));
}

void WMTestSceneWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QPainter painter(this);
    painter.setBrush(QBrush(d->circlecolor));
    foreach (QPoint p, d->centres) {
        painter.drawEllipse(p, d->circleradius, d->circleradius);
    }
}

void WMTestSceneWidget::setNumberOfBalls(int N)
{
    int width = size().width();
    int height = size().height();
    int padding = d->circleradius;
    // Centre of the widget
    int xc = width / 2;
    int yc = height / 2;
    // max distance from center
    int mdfc_x = xc - d->circleradius - padding;
    int mdfc_y = yc - d->circleradius - padding;

    d->centres.clear();
    while (d->centres.count() < N) {
        int randomdistance_x = randrange(-mdfc_x, mdfc_x);
        int randomdistance_y = randrange(-mdfc_y, mdfc_y);
        int newx = xc + randomdistance_x;
        int newy = yc + randomdistance_y;
        bool ok = true;
        if (d->centres.count() > 0) {
            // check if balls overlap
            foreach (QPoint p, d->centres) {
                QPoint q(newx, newy);
                if (dist(p, q) < 2 * d->circleradius + padding) {
                    qDebug() << 2 * d->circleradius + padding << dist(p, q) << p << q;
                    // overlap!
                    ok = false;
                    break;
                }
            }
            if (ok) d->centres << QPoint(newx, newy);
        }
        else {
            // First ball
            d->centres << QPoint(newx, newy);
        }
    }
    update();
}

