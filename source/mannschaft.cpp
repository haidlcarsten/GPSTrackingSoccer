#include "mannschaft.h"
#include "consts.h"
#include <QCheckBox>
#include <QFormLayout>
#include <QGroupBox>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QDebug>
#include <QtCharts>
#include "mainwindow.h"

Mannschaft::Mannschaft(QObject *parent) : QObject(parent)
{
#ifdef Q_OS_LINUX
  this->mSettings = new QSettings("." + QApplication::applicationDirPath().left(1) + SETTINGS_FILE_PATH, QSettings::IniFormat);
#endif

#ifdef Q_OS_WIN
  this->mSettings = new QSettings(QApplication::applicationDirPath().left(1) + SETTINGS_FILE_PATH, QSettings::IniFormat);
#endif

  this->mAvgHeartrate = 0;
  this->mAvgSpeed = 0;
}

void Mannschaft::neuerSpieler(QString aPfad)
{
  Spieler* person = new Spieler(aPfad);
  person->setChartWidget(this->mPlayerDataTab);
  person->setSlider(this->mSlider);
  person->setPlayerNumber(QString::number(this->mListSpieler.count() + 1));

  this->mListSpieler.append(person);

  this->initTeam();

  emit playersChanged();
}

void Mannschaft::initTeam()
{
  this->calcAverageHeartrate();
  this->calcAverageSpeed();
  this->calcMeanCornePoint();
  this->calcSynchPoint();

  this->displayAllPlayerData();
}

void Mannschaft::displayAllPlayerData()
{
  QWidget* widget = new QWidget;
  QFormLayout* lyForm = new QFormLayout;
  foreach (Spieler* player, this->mListSpieler)
    lyForm->addRow(player->generatePlayerDataWidget());

  widget->setLayout(lyForm);

  this->mPlayerDataTab->setWidget(widget);

  // always renew the default-widget
  this->mPlayerDataTab->setDefaultWidget(widget);
}

void Mannschaft::neueSpieler(QStringList aPfade)
{
  foreach (auto element, aPfade)
    this->neuerSpieler(element);
}

float Mannschaft::getTeamAverageHeartrate()
{
  return  this->mAvgHeartrate;
}

float Mannschaft::calcAverageHeartrate()
{
    double t_heartrate_sum = 0;
    foreach (Spieler* player, this->mListSpieler)
    {
     t_heartrate_sum = t_heartrate_sum + player->getHeartRate();
    }

    mAvgHeartrate = t_heartrate_sum / mListSpieler.count();
    return mAvgHeartrate;
}

void Mannschaft::calcMeanCornePoint()
{
    double xMax = 0.0;
    double xMin = 0.0;
    double yMax = 0.0;
    double yMin = 0.0;
    foreach (Spieler* person, mListSpieler)
    {
      xMin += person->getCornerBottomLeft().first;
      yMin += person->getCornerBottomLeft().second;
      xMax += person->getCornerTopRight().first;
      yMax += person->getCornerTopRight().second;
    }
    xMin /=(double) mListSpieler.count();
    yMin /=(double) mListSpieler.count();
    xMax /=(double) mListSpieler.count();
    yMax /=(double) mListSpieler.count();

    mCornerBottomLeft.first = xMin;
    mCornerBottomLeft.second = yMin;

    mCornerTopRight.first = xMax;
    mCornerTopRight.second = yMax;

    mCornerBottomRight.first = xMax;
    mCornerBottomRight.second = yMin;

    mCornerTopLeft.first = xMin;
    mCornerTopLeft.second = yMax;

}

void Mannschaft::calcSynchPoint()
{
  QTime latestTime(0,0,0);
  foreach (Spieler* person, mListSpieler)
  {
    QTime time = person->getStartTime();
    if(latestTime < time)
        latestTime = time;
  }
  mSynchPoint = latestTime;

  foreach (Spieler* person, mListSpieler)
  {
    person->calcSynchTime(mSynchPoint);
  }

}

int Mannschaft::getMaximumTimeStamp()
{
  int result = 0;

  foreach (Spieler* player, this->mListSpieler)
  {
    int max = player->getMaximumTimestamp();
    if(max > result)
      result = max;
  }

  return result;
}

void Mannschaft::recalculateAll()
{
  foreach (auto player, this->mListSpieler)
    player->recalculate();

  this->initTeam();

  emit playersChanged();
}

void Mannschaft::showTeamMap(int aTimeStamp)
{
  double markersize = this->mSettings->value(SETTINGS_MARKERSIZE_PLAYERDATA).toDouble();

  QScatterSeries *seriesdata = new QScatterSeries();
  seriesdata->setMarkerShape(QScatterSeries::MarkerShapeCircle);
  seriesdata->setMarkerSize(markersize);
  QChart *chart = new QChart();

  foreach (Spieler* player, this->mListSpieler)
  {
    auto data = player->getTransformedPlayerData(aTimeStamp);
    seriesdata->append(data.mLatitude, data.mLongitude);

    QScatterSeries *seriesHeartRate = new QScatterSeries();

    seriesHeartRate->setMarkerShape(QScatterSeries::MarkerShapeCircle);
    seriesHeartRate->setMarkerSize(markersize * 2.0);
    seriesHeartRate->append(data.mLatitude, data.mLongitude);

    if(data.mHeartRate >= 160.0)
      seriesHeartRate->setColor(QColor(255, 0, 0)); // red
    else if((data.mHeartRate < 160.0) && (data.mHeartRate >= 120.0))
      seriesHeartRate->setColor(QColor(255, 102, 0)); // orange
    else if((data.mHeartRate < 120.0) && (data.mHeartRate >= 90.0))
      seriesHeartRate->setColor(QColor(255, 255, 0)); // yellow
    else if(data.mHeartRate < 90.0)
      seriesHeartRate->setColor(QColor(51, 255,0)); // green

    chart->addSeries(seriesHeartRate);
  }

  chart->addSeries(seriesdata);

  chart->createDefaultAxes();
  chart->setDropShadowEnabled(false);

  QChartView *chartView = new QChartView(chart);

  this->mTeamDataTab->setWidget(chartView);
}

double Mannschaft::getTeamAverageSpeed()
{
  return this->mAvgSpeed;
}

double Mannschaft::calcAverageSpeed()
{
    double t_speed_sum = 0;
    foreach (Spieler* player, this->mListSpieler)
    {
      t_speed_sum = t_speed_sum + player->getSpeed();
    }

    mAvgSpeed = t_speed_sum / mListSpieler.count();
    return mAvgSpeed;
}

QWidget* Mannschaft::displaySpieler()
{
  QWidget* widget = new QWidget();
  // create the layout, to place the Spieler-objects
  QFormLayout* resultLayout = new QFormLayout;
  QFormLayout* lyGbrTeam = new QFormLayout;

  //add every player
  foreach (Spieler* person, this->mListSpieler)
  {
    QHBoxLayout* lyPerson = new QHBoxLayout;
    // element, we can react to
    QRadioButton* cbSelect = new QRadioButton(person->getPlayerName(), NULL);
    cbSelect->setToolTip(TEAM_PLAYER_INDIVIDUAL_TOOLTIP + person->getPlayerName());
    cbSelect->setChecked(false);

    QLineEdit* edName = new QLineEdit();
    edName->setText(person->getPlayerName());

    QLineEdit* edPlayerNumber = new QLineEdit();
    edPlayerNumber->setText(QString::number(person->getPlayerNumber()));
    edPlayerNumber->setValidator(new QIntValidator(0, INT_MAX));

    // add the elements to the widget
    lyPerson->addWidget(cbSelect);
    lyPerson->addWidget(edName);
    lyPerson->addWidget(edPlayerNumber);

    lyGbrTeam->addRow(lyPerson);

    // connect to handle the event from the checkbox
    connect(cbSelect, &QRadioButton::toggled, person, &Spieler::displayData);

    // connect to change the name
    connect(edName, &QLineEdit::textChanged, person, &Spieler::setPlayerName);

    // connect to change the playernumber
    connect(edPlayerNumber, &QLineEdit::textChanged, person, &Spieler::setPlayerNumber);
  }


  if(this->mListSpieler.count() > 0)
  {
    // create the layout for the team-data
    QGridLayout* lyGridTeamData = new QGridLayout;

    lyGridTeamData->addWidget(new QLabel(TEAM_SPEED), 0, 0);
    lyGridTeamData->addWidget(new QLabel(QString::number(this->getTeamAverageSpeed()) + " m/s"), 0, 1);

    lyGridTeamData->addWidget(new QLabel(TEAM_HEARTRATE), 1, 0);
    lyGridTeamData->addWidget(new QLabel(QString::number(this->getTeamAverageHeartrate()) + " bpm"), 1, 1);

    // getting a groupbox for the player of a team
    QGroupBox* grbTeam = new QGroupBox(TEAM_GRB_TEAM);
    grbTeam->setLayout(lyGbrTeam);

    // getting a second groubbox for the team-data
    QGroupBox* grbTeamData = new QGroupBox(TEAM_GRB_DATA);
    grbTeamData->setLayout(lyGridTeamData);

    // add the groupboxes to the basic layout
    resultLayout->addRow(grbTeam);
    resultLayout->addRow(grbTeamData);
  }

  widget->setLayout(resultLayout);
  return widget;
}
