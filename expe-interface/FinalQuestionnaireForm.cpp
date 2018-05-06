#include <QTextStream>
#include <QElapsedTimer>
#include "FinalQuestionnaireForm.h"
#include "ui_FinalQuestionnaireForm.h"

struct FinalQuestionnaireFormData {
    QTextStream *results;
    QElapsedTimer *timer;
};

FinalQuestionnaireForm::FinalQuestionnaireForm(QElapsedTimer *timer, QTextStream *resultsStream, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FinalQuestionnaireForm),
    d(new FinalQuestionnaireFormData)
{
    ui->setupUi(this);
    d->results = resultsStream;
    d->timer = timer;

    connect(ui->buttonFinished, SIGNAL(clicked(bool)), this, SLOT(finishButtonClicked()));
}

FinalQuestionnaireForm::~FinalQuestionnaireForm()
{
    delete ui;
    delete d;
}

void FinalQuestionnaireForm::finishButtonClicked()
{
    QString xml;
    xml.append("<questionnaire>\n");
    xml.append("<question id=\"1\">\n").append(ui->textEditQ1->toPlainText()).append("\n</question>\n");
    xml.append("<question id=\"2\">\n").append(ui->textEditQ2->toPlainText()).append("\n</question>\n");
    xml.append("</questionnaire>\n");
    // Write data to results file and flush stream
    (*d->results) << xml;
    d->results->flush();
    emit finished();
}
