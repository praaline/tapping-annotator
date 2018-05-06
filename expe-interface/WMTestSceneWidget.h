#ifndef WMTESTSCENEWIDGET_H
#define WMTESTSCENEWIDGET_H

#include <QObject>
#include <QWidget>

struct WMTestSceneWidgetData;

class WMTestSceneWidget : public QWidget
{
    Q_OBJECT
public:
    explicit WMTestSceneWidget(QWidget *parent = 0);
    ~WMTestSceneWidget();

    QSize minimumSizeHint() const Q_DECL_OVERRIDE;
    QSize sizeHint() const Q_DECL_OVERRIDE;

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

signals:

public slots:
    void setNumberOfBalls(int N);

private:
    WMTestSceneWidgetData *d;
};

#endif // WMTESTSCENEWIDGET_H
