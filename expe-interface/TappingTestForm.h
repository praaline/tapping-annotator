#ifndef TAPPINGTESTFORM_H
#define TAPPINGTESTFORM_H

#include <QString>
#include <QList>
#include <QWidget>
#include <QMediaPlayer>
class QTextStream;
class QElapsedTimer;

namespace Ui {
class TappingTestForm;
}

struct TappingTestFormData;

class TappingTestForm : public QWidget
{
    Q_OBJECT

public:
    explicit TappingTestForm(QElapsedTimer *timer, QTextStream *resultsStream,
                             QWidget *parent = 0);
    ~TappingTestForm();

    void setStimuli(const QList<QString> &stimuli);
    void shuffleStimuli();
    void setTitle(const QString &title);
    void setInstructions(const QString &instructions);

signals:
    void finished();

protected:
    bool event(QEvent* event);

private slots:
    void buttonInstructionsNextClicked();
    void buttonRepeatStimulusClicked();
    void buttonNextStimulusClicked();
    void mediaPlayerPositionChanged(qint64 position);
    void mediaPlayerStateChanged(QMediaPlayer::State state);

private:
    Ui::TappingTestForm *ui;
    TappingTestFormData *d;

    void playCurrentStimulus();
};

#endif // TAPPINGTESTFORM_H
