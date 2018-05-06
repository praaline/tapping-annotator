#ifndef WMTESTDATAENTRYWIDGET_H
#define WMTESTDATAENTRYWIDGET_H

#include <QWidget>
class QElapsedTimer;
class QTextStream;

namespace Ui {
class WMTestDataEntryWidget;
}

struct WMTestDataEntryWidgetData;

class WMTestDataEntryWidget : public QWidget
{
    Q_OBJECT

public:
    explicit WMTestDataEntryWidget(QWidget *parent = 0);
    ~WMTestDataEntryWidget();

    void setParameters(QElapsedTimer *timer, QTextStream *resultsStream);
    void setMessage(const QString &message);
    void setCorrect(const QString &correct);
    QString response() const;
    QString correct() const;
    bool isResponseCorrect() const;
    void reset();

signals:
    void dataEntryCompleted();

private slots:
    void buttonPressed();

private:
    Ui::WMTestDataEntryWidget *ui;
    WMTestDataEntryWidgetData *d;
};

#endif // WMTESTDATAENTRYWIDGET_H
