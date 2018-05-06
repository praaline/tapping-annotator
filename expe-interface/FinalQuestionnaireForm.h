#ifndef FINALQUESTIONNAIREFORM_H
#define FINALQUESTIONNAIREFORM_H

#include <QWidget>
class QTextStream;
class QElapsedTimer;

namespace Ui {
class FinalQuestionnaireForm;
}

struct FinalQuestionnaireFormData;

class FinalQuestionnaireForm : public QWidget
{
    Q_OBJECT

public:
    explicit FinalQuestionnaireForm(QElapsedTimer *timer, QTextStream *resultsStream,
                                    QWidget *parent = 0);
    ~FinalQuestionnaireForm();

signals:
    void finished();

private slots:
    void finishButtonClicked();

private:
    Ui::FinalQuestionnaireForm *ui;
    FinalQuestionnaireFormData *d;
};

#endif // FINALQUESTIONNAIREFORM_H
