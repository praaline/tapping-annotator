#include <QTextStream>
#include <QElapsedTimer>
#include "EndExperimentForm.h"
#include "ui_EndExperimentForm.h"

struct EndExperimentFormData {
    QTextStream *results;
    QElapsedTimer *timer;
};

EndExperimentForm::EndExperimentForm(QElapsedTimer *timer, QTextStream *resultsStream,
                                     QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EndExperimentForm),
    d(new EndExperimentFormData)
{
    ui->setupUi(this);
    d->results = resultsStream;
    d->timer = timer;

    connect(ui->buttonFinished, SIGNAL(clicked(bool)), this, SLOT(finishButtonClicked()));
}

EndExperimentForm::~EndExperimentForm()
{
    delete ui;
    delete d;
}

void EndExperimentForm::finishButtonClicked()
{
    emit finished();
}
