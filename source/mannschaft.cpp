#include "mannschaft.h"
#include <QCheckBox>
#include <QFormLayout>
#include <QPushButton>
#include <QLabel>
#include <QDebug>

Mannschaft::Mannschaft(QObject *parent) : QObject(parent)
{

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
