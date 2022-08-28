#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QApplication>
#include <QMessageBox>
#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QTextCharFormat>
#include <QtPrintSupport/QPrinter>
#include <QtPrintSupport/QPrintDialog>
#include <QDebug>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QString FileName, file_path, currentFile, fileContent;

private slots:

    void on_actionNew_triggered();
    void on_actionSave_triggered();
    void on_actionPrint_triggered();
    void on_actionExit_triggered();
    void on_actionUndo_triggered();
    void on_actionRedo_triggered();
    void on_actionAbout_triggered();
    void on_actionAbout_Me_triggered();
    void on_actionOpen_triggered();
    void on_actionBold_triggered(bool bold);
    void on_actionItalic_triggered(bool checked);
    void on_actionUnderline_triggered(bool checked);
    void on_actionSuperscript_triggered(bool checked);
    void on_actionSubscript_triggered(bool checked);
};
#endif // MAINWINDOW_H
