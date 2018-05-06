#include <QElapsedTimer>
#include <QTextStream>
#include "WMTestDataEntryWidget.h"
#include "ui_WMTestDataEntryWidget.h"

struct WMTestDataEntryWidgetData {
    WMTestDataEntryWidgetData() :
        timer(0), results(0) {}

    QElapsedTimer *timer;
    QTextStream *results;
    QString response;
    QString correct;

};

WMTestDataEntryWidget::WMTestDataEntryWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WMTestDataEntryWidget),
    d(new WMTestDataEntryWidgetData)
{
    ui->setupUi(this);
    connect(ui->button_1, SIGNAL(clicked(bool)), this, SLOT(buttonPressed()));
    connect(ui->button_2, SIGNAL(clicked(bool)), this, SLOT(buttonPressed()));
    connect(ui->button_3, SIGNAL(clicked(bool)), this, SLOT(buttonPressed()));
    connect(ui->button_4, SIGNAL(clicked(bool)), this, SLOT(buttonPressed()));
    connect(ui->button_5, SIGNAL(clicked(bool)), this, SLOT(buttonPressed()));
    connect(ui->button_6, SIGNAL(clicked(bool)), this, SLOT(buttonPressed()));
    connect(ui->button_7, SIGNAL(clicked(bool)), this, SLOT(buttonPressed()));
    connect(ui->button_8, SIGNAL(clicked(bool)), this, SLOT(buttonPressed()));
    connect(ui->button_9, SIGNAL(clicked(bool)), this, SLOT(buttonPressed()));
    connect(ui->button_BS, SIGNAL(clicked(bool)), this, SLOT(buttonPressed()));
    connect(ui->button_OK, SIGNAL(clicked(bool)), this, SLOT(buttonPressed()));
}

WMTestDataEntryWidget::~WMTestDataEntryWidget()
{
    delete ui;
    delete d;
}

void WMTestDataEntryWidget::setParameters(QElapsedTimer *timer, QTextStream *resultsStream)
{
    d->results = resultsStream;
    d->timer = timer;
}

void WMTestDataEntryWidget::setMessage(const QString &message)
{
    ui->labelMessage->setText(message);
}

void WMTestDataEntryWidget::setCorrect(const QString &correct)
{
    d->correct = correct;
}

QString WMTestDataEntryWidget::response() const
{
    return d->response;
}

QString WMTestDataEntryWidget::correct() const
{
    return d->correct;
}

bool WMTestDataEntryWidget::isResponseCorrect() const
{
    return (d->response == d->correct);
}

void WMTestDataEntryWidget::reset()
{
    d->response.clear();
    d->correct.clear();
    ui->labelResponse->setText("");
}

void WMTestDataEntryWidget::buttonPressed()
{
    if (!d->results) return;
    QPushButton *button = (QPushButton *)sender();
    QString value = button->text();
    if (value == "OK") {
        (*d->results) << QString("<wmresponse response=\"%1\" timer=\"%2\" correct=\"%3\" final=\"1\" />\n")
                         .arg(d->response).arg(d->timer->elapsed()).arg(d->correct);
        emit dataEntryCompleted();
    }
    else if (value == "C") {
        if (d->response.length() >= 1) d->response.chop(1);
        (*d->results) << QString("<wmresponse response=\"%1\" timer=\"%2\" correct=\"%3\" final=\"0\" />\n")
                         .arg(d->response).arg(d->timer->elapsed()).arg(d->correct);
        ui->labelResponse->setText(d->response);
    }
    else {
        // a number
        d->response = d->response.append(value);
        (*d->results) << QString("<wmresponse response=\"%1\" timer=\"%2\" correct=\"%3\" final=\"0\" />\n")
                         .arg(d->response).arg(d->timer->elapsed()).arg(d->correct);
        ui->labelResponse->setText(d->response);
    }
}
