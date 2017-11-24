#include "mannschaft.h"
#include <QCheckBox>
#include <QFormLayout>
#include <QPushButton>
#include <QLabel>
#include <QDebug>

Mannschaft::Mannschaft(QObject *parent) : QObject(parent)
{
    this->t_heartrate = 0;
    this->t_speed = 0;
}

void Mannschaft::neuerSpieler(QString aPfad)
{
  Spieler* person = new Spieler(aPfad);
  person->setChartWidget(this->mChartWidget);
  person->setSlider(this->mSlider);
  person->average_speed();
  person->getSpeed();
  person->average_heartRate();
  person->getHeartRate();

  this->mListSpieler.append(person);
}

void Mannschaft::neueSpieler(QStringList aPfade)
{
  foreach (auto element, aPfade)
    this->neuerSpieler(element);
}

float Mannschaft::get_Team_HeartRate()
{
  return  this->t_heartrate;
}

float Mannschaft::t_average_heartRate()
{
    double t_heartrate_sum = 0;
    foreach (Spieler* player, this->mListSpieler)
    {
     t_heartrate_sum = t_heartrate_sum + player->getHeartRate();
    }

    t_heartrate = t_heartrate_sum / mListSpieler.length();
    return t_heartrate;
}

double Mannschaft::get_Team_Speed()
{
  return   this->t_speed;
}

double Mannschaft::t_average_speed()
{
    double t_speed_sum = 0;
    foreach (Spieler* player, this->mListSpieler)
    {
     t_speed_sum = t_speed_sum + player->getSpeed();
    }

    t_speed = t_speed_sum / mListSpieler.length();
    return t_speed;
}

QWidget* Mannschaft::displaySpieler()
{
  QWidget* widget = new QWidget();
  // create the layout, to palce the Spieler-objects
  QFormLayout* resultLayout = new QFormLayout;

  //add every player
  foreach (Spieler* person, this->mListSpieler)
  {
    // element, we can react to
    QCheckBox* cbSelect = new QCheckBox(person->getName(), NULL);
    cbSelect->setChecked(false);

    // add the elements to the widget
    resultLayout->addRow(cbSelect);

    connect(cbSelect, &QCheckBox::clicked, person, &Spieler::displayData);
  }

  widget->setLayout(resultLayout);
  return widget;
}
