#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, QOverload<>::of(&MainWindow::runOnce));
}

MainWindow::~MainWindow()
{
    delete timer;

    delete ui;
}

bool MainWindow::readInputFile()
{
    QString inputFileStr = ui->inputFileEdit->toPlainText();
    inputFile.setFileName(inputFileStr);
    if (!inputFile.exists())
    {
        reportMessage("does not exist");
        return false;
    }
    else
    {
        if (inputFile.isOpen())
        {
            reportMessage("is already opened");
            return false;
        }
        else if (!inputFile.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            reportMessage("does not open");
            return false;
        }
        else
        {
            while(!inputFile.atEnd())
            {
                inputText = inputFile.readAll();
                reportMessage("read");
            }
            inputFile.close();
        }
    }
    return true;
}

void MainWindow::writeOutputFile()
{
    QString outputFileStr = ui->outputFileEdit->toPlainText();
    outputFile.setFileName(outputFileStr);
    if (!outputFile.exists())
        reportMessage("does not exist");
    else
    {
        if (!outputFile.open(QIODevice::Append | QIODevice::Text))
            reportMessage("does not open");
        else
        {
            QByteArray modificationArray;
            for (int i = 0; i < 4; i++)
            {
                modificationArray.append((char)(modificationNumber << (i * 8)));
            }
            QByteArray outputText;
            for (int i = 0; i < inputText.size(); i++)
            {
                outputText.append(inputText[i] ^ modificationArray[i % 4]);
            }
            outputFile.write(outputText);
            outputFile.close();
        }
    }
}

void MainWindow::reportMessage(QString message)
{
    //ui->tempTextBrowser->clearHistory();
    ui->tempTextBrowser->setText(message);
}

void MainWindow::readIfDeleteDialog()
{
    if (ifDeleteInFile)
    {
        if (inputFile.exists())
        {
            if (inputFile.isOpen())
                inputFile.close();
            inputFile.remove();
            reportMessage("deleted");
        }
        else
            reportMessage("does not exist => not deleted");
    }
    else
        reportMessage("choosen not to delete");
}

void MainWindow::readIfOverwiteDialog()
{

}

void MainWindow::readModificationRule()
{
    modificationNumber = ui->modificationBox->value();
}

void MainWindow::stopTimer()
{
    if (ifTimer)
    {
        if (timer->isActive())
            timer->stop();
    }
    clean();
}

void MainWindow::clean()
{
    ifDeleteInFile = false;
    ifTimer = false;
    ifOverwriteOutFile = overwrite;
    inputText.clear();
    modificationNumber = 0;
}

void MainWindow::runOnce()
{
    if (readInputFile())
    {
        readIfDeleteDialog();
        readIfOverwiteDialog();
        readModificationRule();
        writeOutputFile();
        clean();
    }
    else
        reportMessage("could not run");
}

void MainWindow::on_deleteInFileButton_clicked()
{
    ifDeleteInFile = true;
}

void MainWindow::on_notDeleteInFileButton_clicked()
{
    ifDeleteInFile = false;
}

void MainWindow::on_overwriteOutFileButton_clicked()
{
    ifOverwriteOutFile = overwrite;
}

void MainWindow::on_modifyOutFileButton_clicked()
{
    ifOverwriteOutFile = modify;
}

void MainWindow::on_runByTimerButton_clicked()
{
    if (!ifTimer)
    {
        reportMessage("timer is not configuried");
        return;
    }

    stopTimer();
    runOnce();
    timer->start(ui->timerSpinBox->value() * 1000);
}

void MainWindow::on_runOnceButton_clicked()
{
    stopTimer();
    runOnce();
}

void MainWindow::on_stopButton_clicked()
{
    stopTimer();
}

void MainWindow::on_timerCheckBox_stateChanged(int arg1)
{
    ifTimer = ui->timerCheckBox->isChecked();

    ui->multipleTimerLabel->setEnabled(ifTimer);
    ui->timerSpinBox->setEnabled(ifTimer);
}
