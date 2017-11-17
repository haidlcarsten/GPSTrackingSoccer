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

QLayout* Mannschaft::displaySpieler()
{
  // create the layout, to palce the Spieler-objects
  QFormLayout* resultLayout = new QFormLayout;

  //add every player
  int number = 0;
  foreach (Spieler* person, this->mListSpieler)
  {
    QHBoxLayout* layout = new QHBoxLayout();

    // element, we can react to
    QCheckBox* cbSelect = new QCheckBox(person->getName(), NULL);
    cbSelect->setChecked(false);

    QPushButton* btnDelete = new QPushButton("Entfernen", NULL);
    layout->addWidget(cbSelect);
    layout->addWidget(btnDelete);

    // add the elements to the widget
    resultLayout->addRow(layout);

    // do an connect to react to the click of the checkbox
    connect(btnDelete, &QPushButton::pressed, [=]{
      this->mListSpieler.removeAt(number);
      emit playerChanged();
    });
    number++;

    connect(cbSelect, &QCheckBox::clicked, person, &Spieler::displayData);
  }

  return resultLayout;
}
