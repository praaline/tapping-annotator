#include <QTextStream>
#include <QtMultimedia>
#include <QElapsedTimer>
#include "QSleep.h"
#include "TappingTestForm.h"
#include "ui_TappingTestForm.h"

struct TappingTestFormData {
    TappingTestFormData() : results(0), timer(0), mediaPlayer(0), recording(false), stimulusID(-1) {}
    QTextStream *results;
    QElapsedTimer *timer;
    // Audio stimuli player
    QMediaPlayer *mediaPlayer;
    bool recording;
    // List of stimuli
    QList<QString> stimuli;
    // Current stimulus
    int stimulusID;
};

TappingTestForm::TappingTestForm(QElapsedTimer *timer, QTextStream *resultsStream, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TappingTestForm),
    d(new TappingTestFormData)
{
    ui->setupUi(this);
    // Results text stream and global timer
    d->results = resultsStream;
    d->timer = timer;
    // Stimuli
    d->stimulusID = -1;
    // Set up media player
    d->mediaPlayer = new QMediaPlayer();
    d->mediaPlayer->setVolume(90);
    d->mediaPlayer->setNotifyInterval(50);
    connect(d->mediaPlayer, SIGNAL(positionChanged(qint64)),
            this, SLOT(mediaPlayerPositionChanged(qint64)));
    connect(d->mediaPlayer, SIGNAL(stateChanged(QMediaPlayer::State)),
            this, SLOT(mediaPlayerStateChanged(QMediaPlayer::State)));
    d->recording = false;
    ui->progressBar->setMaximum(1000);
    // Set strong focus to enable keyboard events to be received
    setFocusPolicy(Qt::StrongFocus);
    // Signals
    connect(ui->buttonInstructionsNext, SIGNAL(clicked(bool)), this, SLOT(buttonInstructionsNextClicked()));
    connect(ui->buttonNext, SIGNAL(clicked(bool)), this, SLOT(buttonNextStimulusClicked()));
    connect(ui->buttonRepeat, SIGNAL(clicked(bool)), this, SLOT(buttonRepeatStimulusClicked()));

    ui->stackedWidget->setCurrentIndex(0);
}

TappingTestForm::~TappingTestForm()
{
    delete ui;
    delete d;
}

void TappingTestForm::setStimuli(const QList<QString> &stimuli)
{
    d->stimuli.clear();
    d->stimuli.append(stimuli);
    d->stimulusID = 0;
}

void TappingTestForm::shuffleStimuli()
{
    int nbIter = 100 * d->stimuli.size();
    for(int i = 0; i < nbIter; i++) {
        int begin = qrand() % d->stimuli.count();
        int end = qrand() % d->stimuli.count();
        while(begin == end) {
            end = qrand() % d->stimuli.count();
        }
        d->stimuli.swap(begin, end);
    }
    d->stimulusID = 0;
    qDebug() << d->stimuli;
}

void TappingTestForm::setTitle(const QString &title)
{
    ui->labelTitle->setText(title);
}

void TappingTestForm::setInstructions(const QString &instructions)
{
    ui->labelInstructions->setText(instructions);
}

void TappingTestForm::buttonInstructionsNextClicked()
{
    ui->stackedWidget->setCurrentIndex(1);
    playCurrentStimulus();
}

void TappingTestForm::buttonRepeatStimulusClicked()
{
    playCurrentStimulus();
}

void TappingTestForm::buttonNextStimulusClicked()
{
    d->stimulusID++;
    if (d->stimulusID == d->stimuli.count()) {
        emit finished();
        return;
    }
    playCurrentStimulus();
}

void TappingTestForm::playCurrentStimulus()
{
    if ((d->stimulusID < 0) || (d->stimulusID >= d->stimuli.count())) {
        (*d->results) << QString("<error message=\"wrong index\" stimulusID=\"%1\" />\n").arg(d->stimulusID);
        if (d->stimulusID < 0) emit finished(); else buttonNextStimulusClicked();
        return;
    }
    QString filenameSound = d->stimuli.at(d->stimulusID);
    if (!QFile::exists(filenameSound)) {
        (*d->results) << QString("<error message=\"file not found\" file=\"%1\" />\n").arg(filenameSound);
        buttonNextStimulusClicked();
        return;
    }
    // Write out "play" event
    QString xml = QString("<tapping file=\"%1\" timer=\"%2\" title=\"%3\">\n")
            .arg(filenameSound).arg(d->timer->elapsed()).arg(ui->labelTitle->text());
    (*d->results) << xml;
    d->results->flush();
    // Short delay before playing sound
    ui->groupBox->setTitle(QString("Fichier %1/%2").arg(d->stimulusID + 1).arg(d->stimuli.count()));
    ui->progressBar->setValue(0);
    QApplication::processEvents();
    qSleep(1000);
    // Start playing sound and recording events
    d->recording = true;
    qDebug() << filenameSound;
    d->mediaPlayer->setMedia(QUrl::fromLocalFile(filenameSound));
    d->mediaPlayer->play();
}

bool TappingTestForm::event(QEvent* event)
{
    if (!d->recording) return QWidget::event(event);
    if ((event->type() == QEvent::KeyPress) || (event->type() == QEvent::KeyRelease)) {
        QKeyEvent* key = static_cast<QKeyEvent*>(event);

        if (key->modifiers().testFlag(Qt::ShiftModifier) && key->modifiers().testFlag(Qt::ControlModifier) && key->key() == Qt::Key_D) {
            (*d->results) << QString("<event type=\"break\" timer=\"%1\" />\n").arg(d->timer->elapsed());
            d->results->flush();
            d->mediaPlayer->stop();
            emit finished();
        } else {
            QString eventType = (event->type() == QEvent::KeyPress) ? "KEYPRS" : "KEYREL";
            (*d->results) << "<event type=\"" << eventType << "\" timer=\"" << d->timer->elapsed()
                          << "\" mediaPos=\"" << d->mediaPlayer->position()
                          << "\" keycode=\"" << key->nativeScanCode()
                          << "\" isAutoRepeat=\"" << key->isAutoRepeat()
                          << "\" keyTimestamp=\"" << key->timestamp() << "\" />\n";
            d->results->flush();
        }
        return QWidget::event(event);
    }
    return QWidget::event(event);
}

void TappingTestForm::mediaPlayerPositionChanged(qint64 position)
{
    if (d->mediaPlayer->duration() <= 0) return;
    qint64 pos = position * 1000 / d->mediaPlayer->duration();
    ui->progressBar->setValue(pos);
}

void TappingTestForm::mediaPlayerStateChanged(QMediaPlayer::State state)
{
    if (state == QMediaPlayer::PlayingState) {
        ui->buttonNext->setEnabled(false);
        ui->buttonRepeat->setEnabled(false);
    } else {
        ui->buttonNext->setEnabled(true);
        ui->buttonRepeat->setEnabled(true);
        if (d->recording) {
            (*d->results) << QString("<event type=\"NEXT\" timer=\"%1\" />\n").arg(d->timer->elapsed());
            (*d->results) << "</tapping>\n";
            d->results->flush();
            d->recording = false;
        }
    }
}
