#ifndef ENDEXPERIMENTFORM_H
#define ENDEXPERIMENTFORM_H

#include <QWidget>
class QTextStream;
class QElapsedTimer;

namespace Ui {
class EndExperimentForm;
}

struct EndExperimentFormData;

class EndExperimentForm : public QWidget
{
    Q_OBJECT

public:
    explicit EndExperimentForm(QElapsedTimer *timer, QTextStream *resultsStream,
                               QWidget *parent = 0);
    ~EndExperimentForm();

signals:
    void finished();

private slots:
    void finishButtonClicked();

private:
    Ui::EndExperimentForm *ui;
    EndExperimentFormData *d;
};

#endif // ENDEXPERIMENTFORM_H
