#include <QString>
#include <QDateTime>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QElapsedTimer>
#include <QSysInfo>
#include <QDir>
#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "ParticipantIdentifactionForm.h"
#include "WorkingMemoryTestForm.h"
#include "TappingTestForm.h"
#include "FinalQuestionnaireForm.h"
#include "EndExperimentForm.h"

struct MainWindowData {
    QString username;
    QString filenameResults;
    QFile *fileResults;
    QTextStream *results;
    QElapsedTimer *timer;
};

QList<QString> loadCorpus(const QString &filename, const QString &subdirectory)
{
    QString applicationDirPath = QApplication::applicationDirPath();
    applicationDirPath = applicationDirPath.replace("/EXPE.app/Contents/MacOS", "");

    QList<QString> ret;
    // Try to load the list of samples
    QFile fileList(filename);
    if ( !fileList.open( QIODevice::ReadOnly | QIODevice::Text ) ) {
        // list file not found...
        return ret;
    }
    QTextStream streamList(&fileList);
    streamList.setCodec("UTF-8");

    while (!streamList.atEnd()) {
        QString line = streamList.readLine().trimmed();
        if (line.isEmpty()) continue;
        ret.append(applicationDirPath + "/" + subdirectory + "/" + line);
    }
    return ret;
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    d(new MainWindowData)
{
    ui->setupUi(this);
    setWindowState(Qt::WindowMaximized);

    QString applicationDirPath = QApplication::applicationDirPath();
    applicationDirPath = applicationDirPath.replace("/EXPE.app/Contents/MacOS", "");
    qDebug() << applicationDirPath;

    // Create global timer
    d->timer = new QElapsedTimer();

    // Try to get username from the operating system
    QString username = qgetenv("USER");
    if (username.isEmpty()) username = qgetenv("USERNAME");
    d->username = username;

    // Create results file and save the file handler and the associated text stream in the PImpl.
    updateFilenameResults();
    QFile *fileResults = new QFile(QDir::homePath() + "/" + d->filenameResults);
    if ( !fileResults->open( QIODevice::ReadWrite | QIODevice::Text ) ) {
        delete fileResults;
        QMessageBox::warning(this, "File not found",
                             QString("Cannot write to results file %1. "
                                     "Is the program in a directory where you can write?")
                             .arg(applicationDirPath + "/" +  d->filenameResults));
        return;
    }
    QTextStream *stream = new QTextStream(fileResults);
    stream->setCodec("UTF-8");
    d->results = stream;
    d->fileResults = fileResults;
    // Write initial data into results file
    QString tapperVersion = "1.1";
    QString experimentName = "boundaries_experts";
    QString experimentPhase = "2017B";
    QString xml = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
    xml.append(QString("<gamepadannotator version=\"%1\" experiment=\"%2\" phase=\"%3\" ")
               .arg(tapperVersion).arg(experimentName).arg(experimentPhase));
    xml.append(QString("datetime=\"%1\" ").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss t")));
    xml.append(QString("username=\"%1\" ").arg(d->username));
    xml.append(QString("filename=\"%1\" ").arg(d->filenameResults));
    xml.append(">\n");
    (*d->results) << xml;
    // Write system information to file
    (*d->results) << QString("<sysinfo type=\"%1\" timer=\"%2\" timermonotonic=\"%3\" />\n")
                     .arg(QSysInfo::prettyProductName())
                     .arg(QElapsedTimer::clockType()).arg(QElapsedTimer::isMonotonic());
    // Begin recording experiment data
    (*d->results) << "<experiment>\n";
    d->results->flush();

    // Read list of stimuli for NS and MS
    QList<QString> stimuliNS, stimuliMS, trainingNS, trainingMS;
    QList<QString> smalleyDemo, smalleyTest;
    trainingNS = loadCorpus(applicationDirPath + "/stimuli/trainingNS.txt", "stimuli");
    trainingMS = loadCorpus(applicationDirPath + "/stimuli/trainingMS.txt", "stimuli");
    stimuliNS = loadCorpus(applicationDirPath + "/stimuli/stimNS.txt", "stimuli");
    stimuliMS = loadCorpus(applicationDirPath + "/stimuli/stimMS.txt", "stimuli");
    smalleyDemo = loadCorpus(applicationDirPath + "/smalley/smalleyTraining.txt", "smalley");
    smalleyTest = loadCorpus(applicationDirPath + "/smalley/smalleyTest.txt", "smalley");
    // Lists of stimuli for other parts of the experiment
    QList<QString> beeps01; beeps01 << applicationDirPath + "/beeps01.wav";
    QList<QString> beeps02; beeps02 << applicationDirPath + "/beeps02.wav";

    // Create experiment pages
    // PARTICIPANT IDENTIFICATION
    ParticipantIdentifactionForm *pageIndentification = new ParticipantIdentifactionForm(d->timer, d->results, this);
    connect(pageIndentification, SIGNAL(finished()), this, SLOT(moveToNextPage()));
    ui->stackedWidgetPages->addWidget(pageIndentification);
    // TONAL PERCEPTION TEST
    TonePerceptionTestForm *pageTonal = new TonePerceptionTestForm(d->timer, d->results, this);
    connect(pageTonal, SIGNAL(finished()), this, SLOT(moveToNextPage()));
    pageTonal->setDemoStimuli(smalleyDemo);
    pageTonal->setDemoResponses(QList<QString>() << "descendante" << "plate" << "montante");
    pageTonal->setStimuli(smalleyTest);
    pageTonal->shuffleStimuli();
    pageTonal->setTitle("Perception de l'intonation");
    pageTonal->setInstructions(
                "Vous allez entendre un mot avec une intonation montane, plate ou descendante. \u00C0 chaque fois, "
                "choisissez la r\u00E9ponse en fonction de ce que vous avez entendu.\n\n"
                "Il y aura 3 exemples avec la bonne r\u00E9ponse. Ensuite c'est \u00E0 vous!");
    ui->stackedWidgetPages->addWidget(pageTonal);
    // WORKING MEMORY TEST
    WorkingMemoryTestForm *pageWM = new WorkingMemoryTestForm(d->timer, d->results, this);
    connect(pageWM, SIGNAL(finished()), this, SLOT(moveToNextPage()));
    ui->stackedWidgetPages->addWidget(pageWM);
    // RT BEFORE
    TappingTestForm *pageRTbefore = new TappingTestForm(d->timer, d->results, this);
    connect(pageRTbefore, SIGNAL(finished()), this, SLOT(moveToNextPage()));
    pageRTbefore->setStimuli(beeps01);
    pageRTbefore->setTitle("V\u00E9rification de la pression sur la barre d'espace");
    pageRTbefore->setInstructions(
                "Nous allons d'abord v\u00E9rifier que vous pressez correctement sur la barre d'espace en vous "
                "demandant de presser apr\u00E8s chaque bip que vous entendrez.\n\n"
                "Pour cette t\u00E2che comme pour les suivantes, vous avez toujours la possibilit\u00E9 de recommencer "
                "enti\u00E8rement le traitement d'un fichier sonore (si vous avez \u00E9t\u00E9 interrompu, par exemple). "
                "Dans ce cas, au lieu de passer au fichier suivant, appuyez sur la touche \"Je souhaite recommencer ce fichier\".\n\n"
                "Munissez-vous d'un casque audio pour une plus grande qualit\u00E9 d'\u00E9coute. ");
    ui->stackedWidgetPages->addWidget(pageRTbefore);
    // TRAINING NS
    TappingTestForm *pageTrainingNS = new TappingTestForm(d->timer, d->results, this);
    connect(pageTrainingNS, SIGNAL(finished()), this, SLOT(moveToNextPage()));
    pageTrainingNS->setStimuli(trainingNS);
    pageTrainingNS->setTitle("Perception des groupements de mots en parole naturelle");
    pageTrainingNS->setInstructions("Vous allez d'abord vous exercer sur deux extraits de parole naturelle.\n\n"
                                    "Il faut simplement presser la barre d'espace (et la rel\u00E2cher) quand vous percevez "
                                    "la fin d'un groupe de mots.");
    ui->stackedWidgetPages->addWidget(pageTrainingNS);
    // TAPPING NS
    TappingTestForm *pageTappingNS = new TappingTestForm(d->timer, d->results, this);
    connect(pageTappingNS, SIGNAL(finished()), this, SLOT(moveToNextPage()));
    pageTappingNS->setStimuli(stimuliNS);
    pageTappingNS->shuffleStimuli();
    pageTappingNS->setTitle("Perception des groupements de mots en parole naturelle");
    pageTappingNS->setInstructions("Vous allez maintenant entendre 24 extraits naturels (12 minutes). "
                                   "Pour chacun d'eux, une seule \u00E9coute en continu vous sera propos\u00E9e. Vous avez la "
                                   "possibilit\u00E9 de recommencer l'annotation compl\u00E8te d'un extrait si vous le souhaitez.");
    ui->stackedWidgetPages->addWidget(pageTappingNS);
    // TRAINING MS
    TappingTestForm *pageTrainingMS = new TappingTestForm(d->timer, d->results, this);
    connect(pageTrainingMS, SIGNAL(finished()), this, SLOT(moveToNextPage()));
    pageTrainingMS->setStimuli(trainingMS);
    pageTrainingMS->setTitle("Perception des groupements de mots en parole manipul\u00E9e");
    pageTrainingMS->setInstructions("Vous allez maintenant vous exercer sur deux extraits de parole manipul\u00E9e.\n\n"
                                    "Il faut simplement presser la barre d'espace (et la rel\u00E2cher) quand vous percevez "
                                    "la fin d'un groupe de mots.");
    ui->stackedWidgetPages->addWidget(pageTrainingMS);
    // TAPPING MS
    TappingTestForm *pageTappingMS = new TappingTestForm(d->timer, d->results, this);
    connect(pageTappingMS, SIGNAL(finished()), this, SLOT(moveToNextPage()));
    pageTappingMS->setStimuli(stimuliMS);
    pageTappingMS->shuffleStimuli();
    pageTappingMS->setTitle("Perception des groupements de mots en parole manipul\u00E9e");
    pageTappingMS->setInstructions("Vous allez continuer avec les 24 extraits manipul\u00E9s."
                                   "Pour chacun d'eux, une seule \u00E9coute en continu vous sera propos\u00E9e. Vous avez la "
                                   "possibilit\u00E9 de recommencer l'annotation compl\u00E8te d'un extrait si vous le souhaitez.");
    ui->stackedWidgetPages->addWidget(pageTappingMS);
    // RT AFTER
    TappingTestForm *pageRTafter = new TappingTestForm(d->timer, d->results, this);
    connect(pageRTafter, SIGNAL(finished()), this, SLOT(moveToNextPage()));
    pageRTafter->setStimuli(beeps02);
    pageRTafter->setTitle("V\u00E9rification de la pression sur la barre d'espace");
    pageRTafter->setInstructions("Nous allons v\u00E9rifier \u00E0 nouveau que la pression sur la barre d'espace fonctionne "
                                 "(5 bips : on presse d\u00E8s qu'on a entendu le bip).");
    ui->stackedWidgetPages->addWidget(pageRTafter);
    // QUESTIONNAIRE
    FinalQuestionnaireForm *pageFinal = new FinalQuestionnaireForm(d->timer, d->results, this);
    connect(pageFinal, SIGNAL(finished()), this, SLOT(moveToNextPage()));
    ui->stackedWidgetPages->addWidget(pageFinal);
    // END OF EXPERIMENT
    EndExperimentForm *pageEnd = new EndExperimentForm(d->timer, d->results, this);
    connect(pageEnd, SIGNAL(finished()), this, SLOT(endExperiment()));
    ui->stackedWidgetPages->addWidget(pageEnd);

    // Go to welcome screen
    ui->stackedWidgetPages->setCurrentIndex(0);

    // Connect the button to start the experiment
    connect(ui->buttonStart, SIGNAL(clicked(bool)), this, SLOT(startExperimentClicked()));

    d->timer->start();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete d;
}

void MainWindow::updateFilenameResults()
{
    // QString applicationDirPath = QApplication::applicationDirPath();
    // applicationDirPath = applicationDirPath.replace("/EXPE.app/Contents/MacOS", "");
    // Try to create a unique filename for the results
    QString filenameResults = QString("results_%1_0.txt").arg(d->username);
    int i = 0;
    while (QFile::exists(QDir::homePath() + "/" + filenameResults) && i < 20) {
        i++;
        filenameResults = filenameResults.replace(QString("_%1.txt").arg(i - 1), QString("_%1.txt").arg(i));
    }
    d->filenameResults = filenameResults;
}

void MainWindow::startExperimentClicked()
{
    ui->stackedWidgetPages->setCurrentIndex(1);
}

void MainWindow::moveToNextPage()
{
    ui->stackedWidgetPages->setCurrentIndex(ui->stackedWidgetPages->currentIndex() + 1);
}

void MainWindow::endExperiment()
{
    (*d->results) << "</experiment>\n"
                     "</gamepadannotator>\n";
    if (d->fileResults)
        d->fileResults->close();
    QApplication::quit();
}


