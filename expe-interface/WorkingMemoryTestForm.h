#ifndef WORKINGMEMORYTESTFORM_H
#define WORKINGMEMORYTESTFORM_H

#include <QWidget>
class QTextStream;
class QElapsedTimer;

namespace Ui {
class WorkingMemoryTestForm;
}

struct WorkingMemoryTestFormData;

class WorkingMemoryTestForm : public QWidget
{
    Q_OBJECT

public:
    explicit WorkingMemoryTestForm(QElapsedTimer *timer, QTextStream *resultsStream,
                                   QWidget *parent = 0);
    ~WorkingMemoryTestForm();

protected:
    void keyPressEvent(QKeyEvent* event);

signals:
    void finished();

private slots:
    void continueToNextState();
    void dataEntryFormCompleted();

private:
    Ui::WorkingMemoryTestForm *ui;
    WorkingMemoryTestFormData *d;


    void stateEngine(int newState);
    void showBalls(int numberOfBalls);
    void finish();
};

#endif // WORKINGMEMORYTESTFORM_H
