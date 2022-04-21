#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <QTimer>
#include <fnmatch.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_deleteInFileButton_clicked();
    void on_notDeleteInFileButton_clicked();
    void on_overwriteOutFileButton_clicked();
    void on_modifyOutFileButton_clicked();
    void on_runByTimerButton_clicked();
    void on_runOnceButton_clicked();
    void on_stopButton_clicked();
    void on_timerCheckBox_stateChanged(int arg1);

private:
    Ui::MainWindow *ui;
    QFile inputFile;
    QFile outputFile;
    bool ifDeleteInFile = false;
    bool ifTimer = false;
    enum e_ifOverwriteOutFile
    {
        overwrite,
        modify
    } ifOverwriteOutFile = overwrite;
    QByteArray inputText;
    QTimer *timer;
    int modificationNumber;

    bool readInputFile();
    void writeOutputFile();
    void reportMessage(QString message);
    void readIfDeleteDialog();
    void readIfOverwiteDialog();
    void readModificationRule();
    QString getFileByMask(QString inputMask);
    void stopTimer();
    void clean();
    void runOnce();
};

#endif // MAINWINDOW_H
