#include "mannschaft.h"

Mannschaft::Mannschaft(QObject *parent) : QObject(parent)
{

}

void Mannschaft::neuerSpieler(QString aPfad)
{
  Spieler person(aPfad);

  this->mListSpieler.append(&person);
}
