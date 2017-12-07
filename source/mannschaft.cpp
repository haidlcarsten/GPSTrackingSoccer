#include "mannschaft.h"
#include "consts.h"
#include <QCheckBox>
#include <QFormLayout>
#include <QGroupBox>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QDebug>

Mannschaft::Mannschaft(QObject *parent) : QObject(parent)
{
    this->mAvgHeartrate = 0;
    this->mAvgSpeed = 0;
}

void Mannschaft::neuerSpieler(QString aPfad)
{
  Spieler* person = new Spieler(aPfad);
  person->setChartWidget(this->mChartWidget);
  person->setSlider(this->mSlider);
  person->calcAverageSpeed();
  person->calcAverageHeartRate();

  this->mListSpieler.append(person);
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
    // element, we can react to
    QCheckBox* cbSelect = new QCheckBox(person->getPlayerName(), NULL);
    cbSelect->setToolTip(TEAM_PLAYER_INDIVIDUAL_TOOLTIP + person->getPlayerName());
    cbSelect->setChecked(false);

    QLineEdit* edName = new QLineEdit();
    edName->setText(person->getPlayerName());

    // add the elements to the widget
    lyGbrTeam->addRow(cbSelect, edName);

    // connect to handle the event from the checkbox
    connect(cbSelect, &QCheckBox::clicked, person, &Spieler::displayData);

    // connect to change the name
    connect(edName, &QLineEdit::textChanged, person, &Spieler::setPlayerName);
  }


  if(this->mListSpieler.count() > 0)
  {
    // create the layout for the team-data
    QGridLayout* lyGridTeamData = new QGridLayout;

    lyGridTeamData->addWidget(new QLabel(TEAM_SPEED), 0, 0);
    lyGridTeamData->addWidget(new QLabel(QString::number(this->getTeamAverageSpeed())), 0, 1);

    lyGridTeamData->addWidget(new QLabel(TEAM_HEARTRATE), 1, 0);
    lyGridTeamData->addWidget(new QLabel(QString::number(this->getTeamAverageHeartrate())), 1, 1);

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
