#include <QTextStream>
#include <QElapsedTimer>
#include "ParticipantIdentifactionForm.h"
#include "ui_ParticipantIdentifactionForm.h"

struct ParticipantIdentifactionFormData
{
    QTextStream *results;
    QElapsedTimer *timer;
};

ParticipantIdentifactionForm::ParticipantIdentifactionForm(QElapsedTimer *timer, QTextStream *resultsStream, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ParticipantIdentifactionForm),
    d(new ParticipantIdentifactionFormData)
{
    ui->setupUi(this);
    d->results = resultsStream;
    d->timer = timer;
    connect(ui->buttonFinished, SIGNAL(clicked(bool)), this, SLOT(finishButtonClicked()));
}

ParticipantIdentifactionForm::~ParticipantIdentifactionForm()
{
    delete ui;
    delete d;
}

void ParticipantIdentifactionForm::finishButtonClicked()
{
    // Collect identification data from the UI into an XML-formated string
    QString xml;
    xml.append("<identification ");
    xml.append("age=\"").append(ui->lineEditAge->text()).append("\" ");
    xml.append("sex=\"");
    if (ui->radioButtonSexMale->isChecked())        xml.append("M");
    else if (ui->radioButtonSexFemale->isChecked()) xml.append("F");
    else                                            xml.append("?");
    xml.append("\" ");
    xml.append("email=\"").append(ui->lineEditEmail->text()).append("\" ");
    xml.append("L1=\"");
    if (ui->radioButtonL1French->isChecked())   xml.append("FR");
    else if (!ui->lineEditL1->text().isEmpty()) xml.append(ui->lineEditL1->text());
    else                                        xml.append("OTHER");
    xml.append("\" ");
    xml.append("hearingprob=\"");
    if (ui->radioButtonHearingProbYes->isChecked())     xml.append("1");
    else if (ui->radioButtonHearingProbNo->isChecked()) xml.append("0");
    else                                                xml.append("?");
    xml.append("\" ");
    xml.append("musicaltraining=\"");
    if (ui->radioButtonMusicalTrainingYes->isChecked())     xml.append("1");
    else if (ui->radioButtonMusicalTrainingNo->isChecked()) xml.append("0");
    else                                                    xml.append("?");
    xml.append("\" ");
    xml.append("annotexperience=\"");
    if (ui->radioButtonAnnotExperienceYes->isChecked())     xml.append("1");
    else if (ui->radioButtonAnnotExperienceNo->isChecked()) xml.append("0");
    else                                                    xml.append("?");
    xml.append("\" ");    
    if (ui->checkBoxAnnotAccentuation->isChecked())    xml.append("annot_accentuation=\"1\" ");
    if (ui->checkBoxAnnotProminence->isChecked())      xml.append("annot_prominence=\"1\" ");
    if (ui->checkBoxAnnotIntonation->isChecked())      xml.append("annot_intonation=\"1\" ");
    if (ui->checkBoxAnnotProsodicUnits->isChecked())   xml.append("annot_prosodicunits=\"1\" ");
    if (!ui->lineEditAnnotOther->text().isEmpty())
        xml.append(QString("annot_other=\"%1\" ").arg(ui->lineEditAnnotOther->text()));
    if (!ui->lineEditBackground->text().isEmpty())
        xml.append("annot_background=\"").append(ui->lineEditBackground->text()).append("\" ");
    xml.append("/>\n");
    // Write identification data to results file and flush stream
    (*d->results) << xml;
    d->results->flush();
    emit finished();
}



