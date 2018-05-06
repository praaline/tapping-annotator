#ifndef PARTICIPANTIDENTIFACTIONFORM_H
#define PARTICIPANTIDENTIFACTIONFORM_H

#include <QWidget>
class QTextStream;
class QElapsedTimer;

namespace Ui {
class ParticipantIdentifactionForm;
}

struct ParticipantIdentifactionFormData;

class ParticipantIdentifactionForm : public QWidget
{
    Q_OBJECT

public:
    explicit ParticipantIdentifactionForm(QElapsedTimer *timer, QTextStream *resultsStream,
                                          QWidget *parent = 0);
    ~ParticipantIdentifactionForm();

signals:
    void finished();

private slots:
    void finishButtonClicked();

private:
    Ui::ParticipantIdentifactionForm *ui;
    ParticipantIdentifactionFormData *d;
};

#endif // PARTICIPANTIDENTIFACTIONFORM_H
