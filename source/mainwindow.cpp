#include "mainwindow.h"
#include "consts.h"

#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <QSettings>
#include "settingsdialog.h"

#include <QtCharts>


MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);

  // setting the label-text
  ui->dockWidgetHeim->setWindowTitle(MW_TEAM_HOME_DOCK);
  ui->dockWidgetGegner->setWindowTitle(MW_TEAM_ENEMY_DOCK);

  ui->btnStepUp->setText(MW_BTN_STEP_NEXT);
  ui->btnStepDown->setText(MW_BTN_STEP_PRE);
  ui->btnPlay->setText(MW_BTN_STEP_PlAY);
  ui->btnStop->setText(MW_BTN_STEP_STOP);
  ui->lblGoToMinute->setText(MW_LBL_GO_TO_MINUTE);

  ui->tabWidget->setTabText(0, MW_TAB_PLAYER_DATA);
  ui->tabWidget->setTabText(1, MW_TAB_TEAM_DATA_HOME);
  ui->tabWidget->setTabText(2, MW_TAB_TEAM_DATA_ENEMY);
  ui->tabWidget->setTabText(3, MW_TAB_HEATMAP);

  // show always the first one
  ui->tabWidget->setCurrentIndex(0);

  // for more information see: http://doc.qt.io/archives/qt-4.8/qlineedit.html#inputMask-prop
  ui->edGoToMinute->setInputMask("999:99");

  this->mMannschaftHeim.setUI(this);
  this->mMannschaftGegner.setUI(this);

  this->mMannschaftHeim.setPlayerDataTab(ui->tabPlayerData);
  this->mMannschaftHeim.setTeamDataTab(ui->tabTeamHomeData);
  this->mMannschaftHeim.setHeatmapTab(ui->tabHeatmap);
  this->mMannschaftHeim.setSlider(ui->horizontalSlider);

  this->mMannschaftGegner.setPlayerDataTab(ui->tabPlayerData);
  this->mMannschaftGegner.setTeamDataTab(ui->tabTeamEnemyData);
  this->mMannschaftGegner.setHeatmapTab(ui->tabHeatmap);
  this->mMannschaftGegner.setSlider(ui->horizontalSlider);

  createMenusAndActions();

  connect(&mMannschaftHeim, &Mannschaft::playersChanged, this, &MainWindow::reDrawSpielerList);
  connect(&mMannschaftGegner, &Mannschaft::playersChanged, this, &MainWindow::reDrawSpielerList);

  connect(ui->horizontalSlider, &QSlider::valueChanged, &mMannschaftHeim, &Mannschaft::showTeamMap);
  connect(ui->horizontalSlider, &QSlider::valueChanged, &mMannschaftGegner, &Mannschaft::showTeamMap);

  connect(ui->btnStepUp, &QPushButton::clicked, this, &MainWindow::incrementSlider);
  connect(ui->btnStepDown, &QPushButton::clicked, this, &MainWindow::decrementSlider);

  connect(ui->edGoToMinute, &QLineEdit::editingFinished, this, &MainWindow::goToMinute);

  connect(ui->btnPlay, &QPushButton::clicked, this, &MainWindow::startPlayback);
  connect(ui->btnStop, &QPushButton::clicked, this, &MainWindow::stopPlayback);

  loadSettings();
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::createMenusAndActions()
{
  // File menu
  QMenu* fileMenu = menuBar()->addMenu(MW_FILE_MENU);
  fileMenu->addAction(MW_FILE_MENU_LOAD_TEAM_HOME, this, &MainWindow::showFileOpenDialogMannschaftHeim);
  fileMenu->addAction(MW_FILE_MENU_LOAD_TEAM_ENEMY, this, &MainWindow::showFileOpenDialogMannschaftGegner);
  fileMenu->addSeparator();
  fileMenu->addAction(MW_FILE_MENU_LOAD_PLAYER, this, &MainWindow::showFileOpenDialogAddPlayer);
  fileMenu->addSeparator();
  fileMenu->addAction(MW_FILE_MENU_LOAD_SETTINGS, this, &MainWindow::showSettingsDialog);
  fileMenu->addSeparator();
  fileMenu->addAction(MW_FILE_MENU_EXIT, this, SLOT(close()));

  QMenu* helpMenu = menuBar()->addMenu(MW_HELP_MENU);
  helpMenu->addAction(MW_HELP_MENU_HELP, this, &MainWindow::showHelpMenuDialog);
  helpMenu->addSeparator();
  helpMenu->addAction(MW_HELP_MENU_INFORMATION, this, &MainWindow::showInformationDialog);
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

  this->determineSliderRange();
  this->mMannschaftHeim.showTeamMap(0);

  this->generateMap();
}

void MainWindow::showFileOpenDialogMannschaftGegner()
{
  QStringList fileNames = this->showFileOpenDialog();

  this->mMannschaftGegner.neueSpieler(fileNames);

  this->determineSliderRange();
  this->mMannschaftGegner.showTeamMap(0);
}

void MainWindow::showFileOpenDialogAddPlayer()
{
  QMessageBox msgBox;
  msgBox.setText(MW_LOAD_PLAYER_QUESTION);
  msgBox.setInformativeText(MW_LOAD_PLAYER_QUESTION_INFORMATION);
  msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Abort);
  msgBox.setDefaultButton(QMessageBox::Yes);
  msgBox.setButtonText(QMessageBox::Yes, BUTTON_TEXT_YES);
  msgBox.setButtonText(QMessageBox::No, BUTTON_TEXT_NO);
  msgBox.setButtonText(QMessageBox::Abort, BUTTON_TEXT_ABORT);
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
  ui->dockWidgetContentsHeim->setWidget(widget);

  widget = this->mMannschaftGegner.displaySpieler();
  ui->dockWidgetContentsGegner->setWidget(widget);
}

void MainWindow::incrementSlider()
{
  if(this->mTimer.isActive())
    this->mTimer.stop();

  ui->horizontalSlider->setValue(ui->horizontalSlider->value() + 1);
}

void MainWindow::decrementSlider()
{
  if(this->mTimer.isActive())
    this->mTimer.stop();

  ui->horizontalSlider->setValue(ui->horizontalSlider->value() - 1);
}

void MainWindow::goToMinute()
{
  if(this->mTimer.isActive())
    this->mTimer.stop();


  if(ui->horizontalSlider->maximum() > 0)
  {
    QStringList parts = ui->edGoToMinute->text().split(":");
    QString secondtenpart = "" + parts[1][0];
    if(secondtenpart.toInt() >= 6)
    {
      QMessageBox::information(this,
                               APPLICATION_NAME,
                               MW_WRONG_TIME,
                               QMessageBox::Ok);
      return;
    }

    int minute = parts.at(0).toInt();
    int seconds = parts[1].toInt();

    int step = minute * 60 + seconds;
    int maximum = ui->horizontalSlider->maximum();

    if(step <= maximum)
      ui->horizontalSlider->setValue(step);
    else
    {
      int realminute = maximum / 60;
      int realsecond = maximum % 60;

      QString messagetime = " (0:00 - " + QString::number(realminute) + ":" + QString::number(realsecond) + ")";

      QString message = MW_WRONG_TIME;
      message = message.append(messagetime);

      QMessageBox::information(this,
                               APPLICATION_NAME,
                               message,
                               QMessageBox::Ok);
      return;
    }
  }
}

void MainWindow::startPlayback()
{
  if(this->mTimer.isActive())
    this->mTimer.stop();

  this->mTimer.start(100, this);
}

void MainWindow::stopPlayback()
{
  if(this->mTimer.isActive())
    this->mTimer.stop();
}

void MainWindow::timerEvent(QTimerEvent* event)
{
  if(event->timerId() == this->mTimer.timerId())
    ui->horizontalSlider->setValue(ui->horizontalSlider->value() + 1);
}

void MainWindow::showHelpMenuDialog()
{
  QMessageBox::question(this,
                           APPLICATION_NAME_HELP,
                           MW_HELP_TEXT,
                           QMessageBox::Ok);
}

void MainWindow::showInformationDialog()
{
  QMessageBox::information(this,
                           APPLICATION_NAME,
                           MW_ABOUT_TEXT,
                           QMessageBox::Ok);
}

void MainWindow::showSettingsDialog()
{
  SettingsDialog dlg;
  dlg.exec();

  this->mMannschaftHeim.recalculateAll();
  this->mMannschaftGegner.recalculateAll();
}

void MainWindow::loadSettings()
{
#ifdef Q_OS_LINUX
  QSettings settings("." + QApplication::applicationDirPath().left(1) + SETTINGS_FILE_PATH, QSettings::IniFormat);
#endif

#ifdef Q_OS_WIN
  QSettings settings(QApplication::applicationDirPath().left(1) + SETTINGS_FILE_PATH, QSettings::IniFormat);
#endif

  while(!settings.value(SETTINGS_COORDINATES_EXISTS, false).toBool())
  {
    QMessageBox::information(this,
                             APPLICATION_NAME,
                             MW_NO_SETTINGS,
                             QMessageBox::Ok);

    this->showSettingsDialog();
  }
}

void MainWindow::generateMap()
{
#ifdef Q_OS_LINUX
  QSettings settings("." + QApplication::applicationDirPath().left(1) + SETTINGS_FILE_PATH, QSettings::IniFormat);
#endif

#ifdef Q_OS_WIN
  QSettings settings(QApplication::applicationDirPath().left(1) + SETTINGS_FILE_PATH, QSettings::IniFormat);
#endif

  // creates an widget to show the information to an player
    QWidget* widgetToDisplay; // just a holder

    // if we should display the player or not
      QGridLayout* lyForm = new QGridLayout;

      QScatterSeries *seriesdata = new QScatterSeries();

      double bottomLeftLong = settings.value(SETTINGS_COORDINATES_BOTTOM_LEFT_LONGITUDE).toDouble();
      double bottomLeftLat = settings.value(SETTINGS_COORDINATES_BOTTOM_LEFT_LATITUDE).toDouble();

      double bottomrightLong = settings.value(SETTINGS_COORDINATES_BOTTOM_RIGHT_LONGITUDE).toDouble();
      double bottomrightLat = settings.value(SETTINGS_COORDINATES_BOTTEM_RIGHT_LATITUDE).toDouble();

      double toprightLong = settings.value(SETTINGS_COORDINATES_TOP_RIGHT_LONGITUDE).toDouble();
      double toprightLat = settings.value(SETTINGS_COORDINATES_TOP_RIGHT_LATITUDE).toDouble();

      double topleftLong = settings.value(SETTINGS_COORDINATES_TOP_LEFT_LONGITUDE).toDouble();
      double topleftLat = settings.value(SETTINGS_COORDINATES_TOP_LEFT_LATITUDE).toDouble();

      seriesdata->append(bottomLeftLong, bottomLeftLat);
      seriesdata->append(bottomrightLong, bottomrightLat);
      seriesdata->append(topleftLong, topleftLat);
      seriesdata->append(toprightLong, toprightLat);

      QScatterSeries *seriesdata2 = new QScatterSeries();
      Spieler* person = this->mMannschaftHeim.at(0);

      seriesdata2->append(person->getTransformedPlayerData(0).cLeftBottomLong, person->getTransformedPlayerData(0).cLeftBottomLat);
      seriesdata2->append(person->getTransformedPlayerData(0).cRightBottomLong, person->getTransformedPlayerData(0).cRightBottomLat);
      seriesdata2->append(person->getTransformedPlayerData(0).cLeftTopLong, person->getTransformedPlayerData(0).cLeftTopLat);
      seriesdata2->append(person->getTransformedPlayerData(0).cRightTopLong, person->getTransformedPlayerData(0).cRightTopLat);

      seriesdata2->setColor(QColor(0,255,0));

      QChart *chart = new QChart();
      //chart->addSeries(seriesdata);
      chart->addSeries(seriesdata2);

      chart->createDefaultAxes();
      chart->setDropShadowEnabled(false);

      QChartView *chartView = new QChartView(chart);

      lyForm->addWidget(chartView, 0, 0);

      widgetToDisplay = new QWidget;
      widgetToDisplay->setLayout(lyForm);

      ui->tabHeatmap->setWidget(widgetToDisplay);
}

void MainWindow::determineSliderRange()
{
  int maxHome = this->mMannschaftHeim.getMaximumTimeStamp();
  int maxEnemy = this->mMannschaftGegner.getMaximumTimeStamp();

  if(maxHome > maxEnemy)
    ui->horizontalSlider->setRange(0, maxHome);
  else
    ui->horizontalSlider->setRange(0, maxEnemy);

  ui->horizontalSlider->blockSignals(true);
  ui->horizontalSlider->setValue(0);
  ui->horizontalSlider->blockSignals(false);
}
