#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <Windows.h>
#include <stdio.h>
#include <tchar.h>
#include <psapi.h>
#include <QMessageBox>
#include <vector>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


class Process {
    public:
        int PID;
        QString status;
        HANDLE processHandle;
        HANDLE processThread;
        Process(int PID, QString status, HANDLE processHandle, HANDLE processThread) {
            this->PID = PID;
            this->status = status;
            this->processHandle = processHandle;
            this->processThread = processThread;
        }
        QString getStatus() {

        }
        void makeZombie() {
            ::TerminateProcess(processHandle, 100);
            ::CloseHandle(processThread);

//            ::CloseHandle(processHandle);
            DWORD exitCode;
            GetExitCodeProcess(processHandle, &exitCode);
            if (exitCode == STILL_ACTIVE )
                {
                  status = "running";
                } else
                {
                  status = "zombie";
                }
        }
        void kill() {
            ::TerminateProcess(processHandle, 100);
            ::CloseHandle(processThread);
            ::CloseHandle(processHandle);
            DWORD exitCode;
            GetExitCodeProcess(processHandle, &exitCode);
            if (exitCode == STILL_ACTIVE )
                {
                  status = "running";
                } else
                {
                  status = "not running";
                }
        }
};

std::vector<Process> handleVector;

void MainWindow::on_pushButton_clicked()
{
    STARTUPINFO si = { sizeof(si) };
    PROCESS_INFORMATION pi = { 0 };

    std::wstring appName = ui->textEdit->toPlainText().toStdWString();

    std::wstring ApplicationName(appName);
    LPWSTR lpwAppName = &ApplicationName[0];

    int i = 0;
        auto created = ::CreateProcess(lpwAppName,
                                       NULL,
                                       NULL,
                                       NULL,
                                       FALSE,
                                       0,
                                       NULL,
                                       NULL,
                                       &si,
                                       &pi);
        printf("Index: %6d PID: %u\n", i + 1, pi.dwProcessId);
        QMessageBox::information(this, "Info", "Process is running");
        DWORD exitCode;
        QString exitCodeStr = "";
        GetExitCodeProcess(pi.hProcess, &exitCode);
        if (exitCode == STILL_ACTIVE )
            {
              exitCodeStr = "running";
            } else
            {
              exitCodeStr = "not running";
            }
//        ui->listWidget->addItem(QString::number(GetProcessId(pi.hProcess)) + " " + exitCodeStr);
        Process *newProcess = new Process(GetProcessId(pi.hProcess), exitCodeStr, pi.hProcess, pi.hThread);
        handleVector.push_back(*newProcess);
    printf("Total: %d\n", i);

}



int MainWindow::on_pushButton_2_clicked()
{
   QString fileName = QFileDialog::getOpenFileName(this, "Open a file", "C://");

   ui->textEdit->setText(fileName);
}











