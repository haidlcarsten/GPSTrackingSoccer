#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{

  QApplication a(argc, argv);
  MainWindow w;

  QString versionString = "0.01-alpha";

  QApplication::setApplicationName("GPS Tracking Visualisation");
  QApplication::setApplicationVersion(versionString);
  QApplication::setOrganizationName("Lehrstuhl für Medizinische Informationstechnik (MedIT)");
  QApplication::setOrganizationDomain("medit.hia.rwth-aachen.de");

  w.setWindowTitle("GPS Tracking Visualisation");

  w.show();
  return a.exec();
}
