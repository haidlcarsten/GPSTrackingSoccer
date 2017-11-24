#include "mainwindow.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);

  this->mMannschaftHeim.setChartWidget(ui->widget);
  this->mMannschaftHeim.setSlider(ui->horizontalSlider);

  this->mMannschaftGegner.setChartWidget(ui->widget);
  this->mMannschaftGegner.setSlider(ui->horizontalSlider);

  createMenusAndActions();

  connect(&mMannschaftHeim, &Mannschaft::playerChanged, this, &MainWindow::reDrawSpielerList);
  connect(&mMannschaftGegner, &Mannschaft::playerChanged, this, &MainWindow::reDrawSpielerList);
}

MainWindow::~MainWindow()
{
  delete ui;
}


void MainWindow::createMenusAndActions()
{
  // File menu
  QMenu* fileMenu = menuBar()->addMenu(tr("&File"));
  fileMenu->addAction("&Heim-Mannschaft laden...", this, SLOT(showFileOpenDialogMannschaftHeim()));
  fileMenu->addAction("&Gegner-Mannschaft laden...", this, SLOT(showFileOpenDialogMannschaftGegner()));
  fileMenu->addSeparator();
  fileMenu->addAction("&Spieler laden...", this, SLOT(showFileOpenDialogAddPlayer()));
  fileMenu->addSeparator();
  fileMenu->addAction("Exit", this, SLOT(close()));

  QMenu* helpMenu = menuBar()->addMenu(tr("&Hilfe"));
  helpMenu->addAction("Hilfe...", this, &MainWindow::showHelpMenuDialog);
  helpMenu->addSeparator();
  helpMenu->addAction("Information", this, &MainWindow::showInformationDialog);
}

QStringList MainWindow::showFileOpenDialog()
{
  QFileDialog openDialog(this);
  openDialog.setFileMode(QFileDialog::ExistingFiles);

  QStringList fileNames;
  if (openDialog.exec())
    fileNames = openDialog.selectedFiles();

  return fileNames;
}

void MainWindow::showFileOpenDialogMannschaftHeim()
{
  QStringList fileNames = this->showFileOpenDialog();

  this->mMannschaftHeim.neueSpieler(fileNames);


  this->mMannschaftHeim.t_average_heartRate();
  this->mMannschaftHeim.get_Team_HeartRate();
  this->mMannschaftHeim.t_average_speed();
  this->mMannschaftHeim.get_Team_Speed();
  this->mMannschaftHeim.meanCornePoint();

  this->reDrawSpielerList();
}

void MainWindow::showFileOpenDialogMannschaftGegner()
{
  QStringList fileNames = this->showFileOpenDialog();

  this->mMannschaftGegner.neueSpieler(fileNames);

  this->mMannschaftGegner.t_average_heartRate();
  this->mMannschaftGegner.get_Team_HeartRate();
  this->mMannschaftGegner.t_average_speed();
  this->mMannschaftGegner.get_Team_Speed();
  this->mMannschaftGegner.meanCornePoint();

  this->reDrawSpielerList();
}

void MainWindow::showFileOpenDialogAddPlayer()
{
  QMessageBox msgBox;
  msgBox.setText("Sollen der Heim-Mannschaft Spieler hinzugefügt werden?");
  msgBox.setInformativeText("Bei Wahl von \"Ja\" werden der Heim-Mannschaft Spieler hinzugefügt, bei der Wahl von \"Nein\" werden der Gegner-Mannschaft Spieler hinzugefügt.");
  msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Abort);
  msgBox.setDefaultButton(QMessageBox::Yes);
  msgBox.setButtonText(QMessageBox::Yes, "Ja");
  msgBox.setButtonText(QMessageBox::No, "Nein");
  msgBox.setButtonText(QMessageBox::Abort, "Abbrechen");
  msgBox.setDefaultButton(QMessageBox::Yes);

  int ret = msgBox.exec();

  switch (ret) {
    case QMessageBox::Yes:
      // Yes was clicked
      this->showFileOpenDialogMannschaftHeim();
      break;
    case QMessageBox::No:
      // No was clicked
      this->showFileOpenDialogMannschaftGegner();
      break;
    default:
      // should never be reached
      break;
  }
}

void MainWindow::reDrawSpielerList()
{
  auto  widget = this->mMannschaftHeim.displaySpieler();
  ui->dockWidgetContentsHeim->setChartWidget(widget);

  widget = this->mMannschaftGegner.displaySpieler();
  ui->dockWidgetContentsGegner->setChartWidget(widget);
}

void MainWindow::showHelpMenuDialog()
{
  QMessageBox::question(this,
                           "GPS Tracking Visualisation Hilfe",
                           "Folgt noch",
                           QMessageBox::Ok);
}

void MainWindow::showInformationDialog()
{
  QMessageBox::information(this,
                           "GPS Tracking Visualisation",
                           "Programmiert für MEDIT im Rahmen eines Seminars im Master Elektrotechnik\nPogrammiert von Kotaro Kurokawa und Carsten Haidl.",
                           QMessageBox::Ok);
}
