#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Notepad");
    ui->actionBold->setCheckable(true);
    ui->actionItalic->setCheckable(true);
    ui->actionUnderline->setCheckable(true);
    ui->actionSuperscript->setCheckable(true);
    ui->actionSubscript->setCheckable(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_actionNew_triggered()
{
    QMessageBox SaveFileMessage, SaveAsFileMessage;

    SaveFileMessage.setMinimumSize(5000, 200);
    SaveFileMessage.setWindowTitle("Save file");
    SaveFileMessage.setText("Wanted to Save the File");
    SaveFileMessage.setInformativeText("It seem that the current workspace is not empty press yes to save else no to Clear workspace.");
    SaveFileMessage.setIcon(QMessageBox::Question);
    SaveFileMessage.setStandardButtons(QMessageBox::Yes | QMessageBox::No);

    SaveAsFileMessage.setMinimumSize(5000, 200);
    SaveAsFileMessage.setWindowTitle("Save file");
    SaveAsFileMessage.setText("Wanted to Save As the File");
    SaveAsFileMessage.setInformativeText("It seem that the current workspace is not empty press yes to save as else no to Clear workspace.");
    SaveAsFileMessage.setIcon(QMessageBox::Question);
    SaveAsFileMessage.setStandardButtons(QMessageBox::Yes | QMessageBox::No);

    if (!ui->textEdit->document()->isEmpty())
    {
        if (ui->textEdit->document()->isModified())
        {
            // qDebug() << "modified " << FileName;
            if (FileName.isEmpty())
            {
                int saveas = SaveAsFileMessage.exec();
                if (saveas == QMessageBox::Yes)
                {
                    on_actionSave_triggered();
                    file_path = "";
                    ui->textEdit->setText("");
                }
                if (saveas == QMessageBox::No)
                {
                    file_path = "";
                    ui->textEdit->setText("");
                }
            }
            else
            {
                int save = SaveFileMessage.exec();
                if (save == QMessageBox::Yes)
                {
                    on_actionSave_triggered();
                    file_path = "";
                    ui->textEdit->setText("");
                }
                if (save == QMessageBox::No)
                {
                    file_path = "";
                    ui->textEdit->setText("");
                }
            }
        }
        else
        {
            on_actionExit_triggered();
        }
    }
}

void MainWindow::on_actionSave_triggered()
{
    if (currentFile.isEmpty())
    {
        FileName = QFileDialog::getSaveFileName(this, "Save");
        if (FileName.isEmpty())
            return;
        currentFile = FileName;
    }
    else
    {
        FileName = currentFile;
    }
    QFile file(FileName);
    if (!file.open(QIODevice::WriteOnly | QFile::Text))
    {
        QMessageBox::warning(this, "Warning", "Cannot save file: " + file.errorString());
        return;
    }
    QTextStream out(&file);
    QString text = ui->textEdit->toPlainText();
    out << text;
    file.close();
}


void MainWindow::on_actionOpen_triggered()
{
    if (ui->textEdit->document()->isModified())
    {
        QMessageBox SaveFileMessage;

        SaveFileMessage.setMinimumSize(5000, 200);
        SaveFileMessage.setWindowTitle("Save file");
        SaveFileMessage.setText("Wanted to Save the File");
        SaveFileMessage.setInformativeText("It seem that the current workspace is not empty press yes to save else no to Clear workspace.");
        SaveFileMessage.setIcon(QMessageBox::Question);
        SaveFileMessage.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);

        int save = SaveFileMessage.exec();
        if (save == QMessageBox::Yes)
        {
            on_actionSave_triggered();
        }
        if (save == QMessageBox::Cancel)
        {
            return;
        }
        if (save == QMessageBox::No)
        {
            ui->textEdit->document()->setModified(false);
        }
    }

    // QTextCharFormat
    // save the file to disk

    QString filename = QFileDialog::getOpenFileName(this, "Open File");

    if (filename.isEmpty())
    {
        return;
    }
    QFile file(filename);
    if (!file.open(QIODevice::ReadWrite | QIODevice::Text))
    {
        return;
    }

    QTextStream in(&file);
    QString line;
    line = in.readLine();

    while (!line.isNull())
    {
        fileContent.append(line);
        line = in.readLine();
    }

    // fileSize = file.size();

    QFileInfo fi(file);
    FileName = fi.fileName();
    setWindowTitle("Notepad -- " + FileName);

    qDebug() << "opened" << FileName;
    ui->textEdit->setPlainText(fileContent);
    fileContent.clear();

    ui->textEdit->document()->setModified(false);
    setWindowTitle("Notepad -- " + FileName);

    file.close();

    connect(ui->textEdit, SIGNAL(textChanged()), this, SLOT(textModified()));
}


void MainWindow::on_actionExit_triggered()
{
    if (ui->textEdit->document()->isModified())
    {
        QMessageBox SaveFileMessage;

        SaveFileMessage.setMinimumSize(5000, 200);
        SaveFileMessage.setWindowTitle("Save file");
        SaveFileMessage.setText("Wanted to Save the File");
        SaveFileMessage.setInformativeText("It seem that the current workspace is not empty press yes to save else no to Clear workspace.");
        SaveFileMessage.setIcon(QMessageBox::Question);
        SaveFileMessage.setStandardButtons(QMessageBox::Yes | QMessageBox::No);

        int save = SaveFileMessage.exec();
        if (save == QMessageBox::Yes)
        {
            on_actionSave_triggered();
        }
        if (save == QMessageBox::No)
        {
            QFile file(FileName);
            file.close();
            ui->textEdit->clear();
            ui->textEdit->document()->setModified(false);
            qDebug() << "Closed " << FileName;
            setWindowTitle("Notepad");
        }
    }
    else
    {
        QFile file(FileName);

        file.close();
        ui->textEdit->clear();
        ui->textEdit->document()->setModified(false);
        qDebug() << "Closed " << FileName;
        FileName = "";
        setWindowTitle("Notepad");
    }
}


void MainWindow::on_actionPrint_triggered()
{
    // Allows for interacting with printer
    QPrinter printer;

    // You'll put your printer name here
    printer.setPrinterName("Printer Name");

    // Create the print dialog and pass the name and parent
    QPrintDialog pDialog(&printer, this);

    if (pDialog.exec() == QDialog::Rejected)
    {
        QMessageBox::warning(this, "Warning", "Cannot Access Printer");
        return;
    }

    // Send the text to the printer
    ui->textEdit->print(&printer);
}


void MainWindow::on_actionUndo_triggered()
{
    ui->textEdit->undo();
}


void MainWindow::on_actionRedo_triggered()
{
    ui->textEdit->redo();
}


void MainWindow::on_actionAbout_triggered()
{
    QApplication::aboutQt();
}


void MainWindow::on_actionAbout_Me_triggered()
{
    QMessageBox::about(this, "About", "Hi This is Arpan Kumar and I'm a Qt Developer.");
}


void MainWindow::on_actionBold_triggered(bool bold)
{
    bold ? ui->textEdit->setFontWeight(QFont::Bold) : ui->textEdit->setFontWeight(QFont::Normal);
}


void MainWindow::on_actionItalic_triggered(bool italic)
{
    italic ? ui->textEdit->setFontItalic(italic) : ui->textEdit->setFontItalic(false);
}


void MainWindow::on_actionUnderline_triggered(bool underline)
{
    underline ? ui->textEdit->setFontUnderline(underline) : ui->textEdit->setFontUnderline(false);
}


void MainWindow::on_actionSuperscript_triggered(bool superscript)
{
    QTextCharFormat format1, format2;
    format1.setVerticalAlignment(QTextCharFormat::AlignSuperScript);
    format2.setVerticalAlignment(QTextCharFormat::AlignNormal);
    if(superscript)
        ui->textEdit->mergeCurrentCharFormat(format1);
    else
        ui->textEdit->mergeCurrentCharFormat(format2);
}


void MainWindow::on_actionSubscript_triggered(bool subscript)
{
    QTextCharFormat format1, format2;
    format1.setVerticalAlignment(QTextCharFormat::AlignSubScript);
    format2.setVerticalAlignment(QTextCharFormat::AlignNormal);
    if(subscript)
        ui->textEdit->mergeCurrentCharFormat(format1);
    else
        ui->textEdit->mergeCurrentCharFormat(format2);
}

