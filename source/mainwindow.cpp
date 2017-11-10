#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  createMenusAndActions();
  statusBar()->hide();

}

MainWindow::~MainWindow()
{
  delete ui;
}


void MainWindow::createMenusAndActions()
{
  // File menu
  QMenu* fileMenu = menuBar()->addMenu(tr("&File"));
  fileMenu->addAction("&Mannschaft A...", this, SLOT(showFileOpenDialogMannschaftA()), Qt::CTRL + Qt::Key_O);
  fileMenu->addAction("&Mannschaft B...", this, SLOT(showFileOpenDialogMannschaftB()), Qt::CTRL + Qt::Key_O);
  fileMenu->addSeparator();
  fileMenu->addAction("Exit", this, SLOT(close()));

}
void MainWindow::showFileOpenDialogMannschaftA()
{
  QFileDialog openDialog(this);
  openDialog.setFileMode(QFileDialog::ExistingFiles);

  QStringList fileNames;
  if (openDialog.exec())
    fileNames = openDialog.selectedFiles();

  if (fileNames.count() > 0)
  {
    // save last used directory with QPreferences
    QString filePath = fileNames.at(0);
    filePath = filePath.section('/', 0, -2);
  }

 a.neueSpieler(fileNames);
}

void MainWindow::showFileOpenDialogMannschaftB()
{
  QFileDialog openDialog(this);
  openDialog.setFileMode(QFileDialog::ExistingFiles);

  QStringList fileNames;
  if (openDialog.exec())
    fileNames = openDialog.selectedFiles();

  if (fileNames.count() > 0)
  {
    // save last used directory with QPreferences
    QString filePath = fileNames.at(0);
    filePath = filePath.section('/', 0, -2);
  }

  b.neueSpieler(fileNames);
}
