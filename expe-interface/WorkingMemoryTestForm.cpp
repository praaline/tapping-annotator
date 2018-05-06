#include <QDebug>
#include <QKeyEvent>
#include <QTextStream>
#include <QElapsedTimer>
#include "QSleep.h"
#include "WorkingMemoryTestForm.h"
#include "ui_WorkingMemoryTestForm.h"

enum WMTestState {
    Welcome,
    Training1_Show3,
    Training1_Show5,
    Training1_Explain,
    Training1_EnterResponse,
    Training1_CorrectResponse,
    Training1_WrongResponse,
    Training2_Show7,
    Training2_Show2,
    Training2_Explain,
    Training2_EnterResponse,
    Training2_CorrectResponse,
    Training2_WrongResponse,
    Trials
};

struct WorkingMemoryTestFormData {
    WorkingMemoryTestFormData() :
        results(0), timer(0), state(WMTestState::Welcome), cspanLevel(2), wrongAnswers(0), repetition(1) {}
    QTextStream *results;
    QElapsedTimer *timer;
    WMTestState state;
    int cspanLevel;
    int wrongAnswers;
    int repetition;
};

WorkingMemoryTestForm::WorkingMemoryTestForm(QElapsedTimer *timer, QTextStream *resultsStream, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WorkingMemoryTestForm),
    d(new WorkingMemoryTestFormData)
{
    ui->setupUi(this);
    d->results = resultsStream;
    d->timer = timer;
    // Data entry form
    ui->wmDataEntry->setParameters(d->timer, d->results);
    ui->wmDataEntry->setMessage("Nombre de points");
    // Title
    ui->labelTitle->setText("Test de mémoire");
    // Signals
    connect(ui->buttonNext, SIGNAL(clicked(bool)), this, SLOT(continueToNextState()));
    connect(ui->wmDataEntry, SIGNAL(dataEntryCompleted()), this, SLOT(dataEntryFormCompleted()));
    // Start
    stateEngine(WMTestState::Welcome);
}

WorkingMemoryTestForm::~WorkingMemoryTestForm()
{
    delete ui;
    delete d;
}

void WorkingMemoryTestForm::showBalls(int numberOfBalls)
{
    ui->buttonNext->blockSignals(true);
    ui->wmDataEntry->blockSignals(true);
    // Display: fixation cross for 500 ms, balls image for 2000 ms
    ui->stackedWidget->setCurrentIndex(1);  // fixation cross
    ui->stackedWidget->repaint();
    this->repaint();
    QApplication::processEvents();
    qSleep(500);
    ui->stackedWidget->setCurrentIndex(2);  // balls
    ui->wmScene->setNumberOfBalls(numberOfBalls);
    ui->stackedWidget->repaint();
    this->repaint();
    QApplication::processEvents();
    qSleep(2000);
    ui->stackedWidget->setCurrentIndex(4);  // black
    ui->buttonNext->blockSignals(false);
    ui->wmDataEntry->blockSignals(false);
}

void WorkingMemoryTestForm::stateEngine(int newState)
{
    QList<int> balls;
    QString correct;

    switch (newState) {
    case WMTestState::Welcome:
        ui->labelInstructions->setText(
                    "Vous allez voir des écrans avec des points.\n"
                    "Il faut compter les points et mémoriser leur nombre.\n"
                    "Ensuite, vous allez restituer ces nombres dans l'ordre.\n\n"
                    "Vous allez d'abord voir deux exemples, et puis ce sera à votre tour.");
        ui->stackedWidget->setCurrentIndex(0);
        break;
    case WMTestState::Training1_Show3:
        showBalls(3);
        ui->labelInstructions->setText("Nombre de points sur cet écran: 3");
        ui->stackedWidget->setCurrentIndex(0);
        break;
    case WMTestState::Training1_Show5:
        showBalls(5);
        ui->labelInstructions->setText("Nombre de points sur cet écran: 5");
        ui->stackedWidget->setCurrentIndex(0);
        break;
    case WMTestState::Training1_Explain:
        ui->labelInstructions->setText("Vous avez vu les nombres de points suivants: 3 et 5.\n\n"
                                       "Tapez-les dans cet ordre: 35 et tapez sur OK.\n"
                                       "Pour corriger tapez sur C.");
        ui->stackedWidget->setCurrentIndex(0);
        break;
    case WMTestState::Training1_EnterResponse:
        ui->wmDataEntry->reset();
        ui->wmDataEntry->setCorrect("35");
        ui->stackedWidget->setCurrentIndex(3);
        break;
    case WMTestState::Training1_WrongResponse:
        ui->labelInstructions->setText("Oops! Mauvaise réponse!");
        ui->stackedWidget->setCurrentIndex(0);
        break;
    case WMTestState::Training1_CorrectResponse:
        ui->labelInstructions->setText("Très bien.\n\nAppuyez sur une touche pour un deuxième exemple.");
        ui->stackedWidget->setCurrentIndex(0);
        break;
    case WMTestState::Training2_Show7:
        showBalls(7);
        ui->labelInstructions->setText("Nombre de points sur cet écran: 7");
        ui->stackedWidget->setCurrentIndex(0);
        break;
    case WMTestState::Training2_Show2:
        showBalls(2);
        ui->labelInstructions->setText("Nombre de points sur cet écran: 2");
        ui->stackedWidget->setCurrentIndex(0);
        break;
    case WMTestState::Training2_Explain:
        ui->labelInstructions->setText("Vous avez vu les nombres de points suivants: 7 et 2.\n\n"
                                       "Tapez-les dans cet ordre: 72 et tapez sur OK.\n"
                                       "Pour corriger tapez sur C.");
        ui->stackedWidget->setCurrentIndex(0);
        break;
    case WMTestState::Training2_EnterResponse:
        ui->wmDataEntry->reset();
        ui->wmDataEntry->setCorrect("72");
        ui->stackedWidget->setCurrentIndex(3);
        break;
    case WMTestState::Training2_WrongResponse:
        ui->labelInstructions->setText("Oops! Mauvaise réponse!");
        ui->stackedWidget->setCurrentIndex(0);
        break;
    case WMTestState::Training2_CorrectResponse:
        ui->labelInstructions->setText("Vous avez bien compris!\n\n"
                                       "Appuyez sur une touche pour commencer le test.\n"
                                       "Si vous vous êtes trompé ce n'est pas grave, passez au suivant.");
        ui->stackedWidget->setCurrentIndex(0);
        break;
    case WMTestState::Trials:
        balls.clear();
        for (int i = 1; i <= d->cspanLevel; ++i) {
            balls << (1 + (rand() % 9));
        }
        correct.clear();
        foreach (int i, balls) {
            correct.append(QString("%1").arg(i));
        }
        ui->wmDataEntry->reset();
        ui->wmDataEntry->setCorrect(correct);
        foreach (int i, balls) {
            showBalls(i);
        }
        ui->stackedWidget->setCurrentIndex(3);
        break;
    default:
        break;
    }
    d->state = (WMTestState)newState;
    ui->buttonNext->setEnabled(true);
}

void WorkingMemoryTestForm::continueToNextState()
{    
    qDebug() << "continueToNextState" << d->state << sender();
    ui->buttonNext->setEnabled(false);
    ui->buttonNext->blockSignals(true);
    ui->wmDataEntry->blockSignals(true);
    switch (d->state) {
    case WMTestState::Welcome:
        (*d->results) << "<workingmemory>\n";    stateEngine(WMTestState::Training1_Show3);            break;
    case WMTestState::Training1_Show3:           stateEngine(WMTestState::Training1_Show5);            break;
    case WMTestState::Training1_Show5:           stateEngine(WMTestState::Training1_Explain);          break;
    case WMTestState::Training1_Explain:         stateEngine(WMTestState::Training1_EnterResponse);    break;
    case WMTestState::Training1_WrongResponse:   stateEngine(WMTestState::Training1_Show3);            break;
    case WMTestState::Training1_CorrectResponse: stateEngine(WMTestState::Training2_Show7);            break;
    case WMTestState::Training2_Show7:           stateEngine(WMTestState::Training2_Show2);            break;
    case WMTestState::Training2_Show2:           stateEngine(WMTestState::Training2_Explain);          break;
    case WMTestState::Training2_Explain:         stateEngine(WMTestState::Training2_EnterResponse);    break;
    case WMTestState::Training2_WrongResponse:   stateEngine(WMTestState::Training2_Show7);            break;
    case WMTestState::Training2_CorrectResponse: stateEngine(WMTestState::Trials);                     break;
    default:
        break;
    }
    ui->buttonNext->blockSignals(false);
    ui->wmDataEntry->blockSignals(false);
}

void WorkingMemoryTestForm::dataEntryFormCompleted()
{
    qDebug() << "dataEntryFormCompleted" << d->state << sender();
    ui->buttonNext->setEnabled(false);
    ui->buttonNext->blockSignals(true);
    ui->wmDataEntry->blockSignals(true);
    switch (d->state) {
    case Training1_EnterResponse:
        if (ui->wmDataEntry->isResponseCorrect())
            stateEngine(WMTestState::Training1_CorrectResponse);
        else
            stateEngine(WMTestState::Training1_WrongResponse);
        break;
    case Training2_EnterResponse:
        if (ui->wmDataEntry->isResponseCorrect())
            stateEngine(WMTestState::Training2_CorrectResponse);
        else
            stateEngine(WMTestState::Training2_WrongResponse);
        break;
    case Trials:
        if (!ui->wmDataEntry->isResponseCorrect())
            d->wrongAnswers++;
        if (d->wrongAnswers > 3) {
            finish();
            return;
        }
        d->repetition++;
        if (d->repetition > 3) {
            d->repetition = 1;
            d->cspanLevel++;
        }
        if (d->cspanLevel > 8) {
            finish();
            return;
        }
        stateEngine(WMTestState::Trials);
    default:
        break;
    }
    ui->buttonNext->blockSignals(false);
    ui->wmDataEntry->blockSignals(false);
}

void WorkingMemoryTestForm::keyPressEvent(QKeyEvent* event)
{

    if (event->modifiers().testFlag(Qt::ShiftModifier) && event->modifiers().testFlag(Qt::ControlModifier) && event->key() == Qt::Key_D) {
        (*d->results) << QString("<event type=\"break\" timer=\"%1\" />\n").arg(d->timer->elapsed());
        finish();
    }
    QWidget::keyPressEvent(event);
}

void WorkingMemoryTestForm::finish()
{
    (*d->results) << "</workingmemory>\n";
    d->results->flush();
    emit finished();
}
