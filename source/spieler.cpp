#include "spieler.h"

Spieler::Spieler(QString aPfad)
{
  this->mSpielername = aPfad;
}

QString Spieler::getName()
{
  return this->mSpielername;
}

void Spieler::displayData()
{
  qDebug() << "void Spieler::displayData()";
  qDebug() << this->mSpielername;
}
