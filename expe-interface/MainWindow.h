#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

struct MainWindowData;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void startExperimentClicked();
    void moveToNextPage();
    void endExperiment();

private:
    Ui::MainWindow *ui;
    MainWindowData *d;

    void updateFilenameResults();
};

#endif // MAINWINDOW_H
