#include "spieler.h"
#include <QLabel>
#include <QFormLayout>

Spieler::Spieler(QString aPfad)
{
  this->mSpielername = aPfad;

}

QString Spieler::getName()
{
  return this->mSpielername;
}

void Spieler::displayData(bool aDisplay)
{
  connect(this->mSlider, &QSlider::valueChanged, this, &Spieler::displayFrameData);

  qDebug() << "void Spieler::displayData()";
  qDebug() << this->mSpielername;

  QWidget* test;
  if(aDisplay)
  {
    test = new QWidget;
    QLabel* lbl = new QLabel(this->mSpielername);

    QVBoxLayout* layout = new QVBoxLayout(test);
    layout->addWidget(lbl);

    this->setSliderValues(0, 200, 100);

  }
  else
  {
    test = this->mChartWidget->getDefaultWidget();
    this->setSliderValues(0, 0, 0);
  }

  this->mChartWidget->setChartWidget(test);
}

void Spieler::setSliderValues(int aMin, int aMax, int aValue)
{
  if(aValue )
  this->mSlider->setMinimum(aMin);
  this->mSlider->setMaximum(aMax);
  this->blockSignals(true);
  this->mSlider->setValue(aValue);
  this->blockSignals(false);

}

void Spieler::displayFrameData(int aTime)
{
  qDebug() << aTime;
}
