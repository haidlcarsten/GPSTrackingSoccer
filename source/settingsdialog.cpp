#include "settingsdialog.h"
#include "ui_settingsdialog.h"
#include "consts.h"
#include "typedef.h"
#include <QMessageBox>
#include <QSettings>
#include <qglobal.h>

SettingsDialog::SettingsDialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::SettingsDialog)
{
  ui->setupUi(this);

  // only numbers should be avaible
  ui->edCornerBottomLeftLatitude->setValidator(new QDoubleValidator(0, DBL_MAX, 10));
  ui->edCornerBottomRightLatitude->setValidator(new QDoubleValidator(0, DBL_MAX, 10));
  ui->edCornerTopLeftLatitude->setValidator(new QDoubleValidator(0, DBL_MAX, 10));
  ui->edCornerTopRightLatitude->setValidator(new QDoubleValidator(0, DBL_MAX, 10));

  ui->edCornerBottomLeftLongitude->setValidator(new QDoubleValidator(0, DBL_MAX, 10));
  ui->edCornerBottomRightLongitude->setValidator(new QDoubleValidator(0, DBL_MAX, 10));
  ui->edCornerTopLeftLongitude->setValidator(new QDoubleValidator(0, DBL_MAX, 10));
  ui->edCornerTopRightLongitude->setValidator(new QDoubleValidator(0, DBL_MAX, 10));

  //setting the correct tab-order
  QWidget::setTabOrder(ui->edCornerBottomLeftLatitude, ui->edCornerBottomLeftLongitude);
  QWidget::setTabOrder(ui->edCornerBottomLeftLongitude, ui->edCornerBottomRightLatitude);
  QWidget::setTabOrder(ui->edCornerBottomRightLatitude, ui->edCornerBottomRightLongitude);
  QWidget::setTabOrder(ui->edCornerBottomRightLongitude, ui->edCornerTopLeftLatitude);
  QWidget::setTabOrder(ui->edCornerTopLeftLatitude, ui->edCornerTopLeftLongitude);
  QWidget::setTabOrder(ui->edCornerTopLeftLongitude, ui->edCornerTopRightLatitude);
  QWidget::setTabOrder(ui->edCornerTopRightLatitude, ui->edCornerTopRightLongitude);


  // define the textes
  ui->grbCoordinats->setTitle(STD_GRBBOX_COORDINATES);
  ui->lblLaengengrad->setText(STD_LBL_LONGITUDE);
  ui->lblBreitengrad->setText(STD_LBL_LATITUDE);
  ui->lblCornerBottomLeft->setText(STD_LBL_CORNER_BOTTOM_LEFT);
  ui->lblCornerBottomRight->setText(STD_LBL_CORNER_BOTTOM_RIGHT);
  ui->lblCornerTopLeft->setText(STD_LBL_CORNER_TOP_LEFT);
  ui->lblCornerTopRight->setText(STD_LBL_CORNER_TOP_RIGHT);

  ui->edCornerBottomLeftLatitude->setToolTip(STD_CORNER_BOTTOM_LEFT_LAT_TOOLTIP);
  ui->edCornerBottomLeftLongitude->setToolTip(STD_CORNER_BOTTOM_LEFT_LONG_TOOLTIP);
  ui->edCornerBottomRightLatitude->setToolTip(STD_CORNER_BOTTOM_RIGHT_LAT_TOOLTIP);
  ui->edCornerBottomRightLongitude->setToolTip(STD_CORNER_BOTTOM_RIGHT_LONG_TOOLTIP);
  ui->edCornerTopLeftLatitude->setToolTip(STD_CORNER_TOP_LEFT_LAT_TOOLTIP);
  ui->edCornerTopLeftLongitude->setToolTip(STD_CORNER_TOP_LEFT_LONG_TOOLTIP);
  ui->edCornerTopRightLatitude->setToolTip(STD_CORNER_TOP_RIGHT_LAT_TOOLTIP);
  ui->edCornerTopRightLongitude->setToolTip(STD_CORNER_TOP_RIGHT_LONG_TOOLTIP);

#ifdef QT_DEBUG
  // if we have the debug and we dont have a config-file, we set default-values
  ui->edCornerBottomLeftLatitude->setText("50,5");
  ui->edCornerBottomLeftLongitude->setText("50,5");
  ui->edCornerBottomRightLatitude->setText("50,5");
  ui->edCornerBottomRightLongitude->setText("50,5");
  ui->edCornerTopLeftLatitude->setText("50,5");
  ui->edCornerTopLeftLongitude->setText("50,5");
  ui->edCornerTopRightLatitude->setText("50,5");
  ui->edCornerTopRightLongitude->setText("50,5");
#endif

#ifdef Q_OS_LINUX
  QSettings settings("." + QApplication::applicationDirPath().left(1) + SETTINGS_FILE_PATH, QSettings::IniFormat);
#elif Q_OS_WIN
  QSettings settings(QApplication::applicationDirPath().left(1) + SETTINGS_FILE_PATH, QSettings::IniFormat);
#endif

  if(settings.value(SETTINGS_COORDINATES_EXISTS, false).toBool())
  {
    ui->edCornerBottomLeftLatitude->setText(QString::number(settings.value(SETTINGS_COORDINATES_BOTTOM_LEFT_LATITUDE).toDouble()));
    ui->edCornerBottomLeftLongitude->setText(QString::number(settings.value(SETTINGS_COORDINATES_BOTTOM_LEFT_LONGITUDE).toDouble()));

    ui->edCornerBottomRightLatitude->setText(QString::number(settings.value(SETTINGS_COORDINATES_BOTTEM_RIGHT_LATITUDE).toDouble()));
    ui->edCornerBottomRightLongitude->setText(QString::number(settings.value(SETTINGS_COORDINATES_BOTTOM_RIGHT_LONGITUDE).toDouble()));

    ui->edCornerTopLeftLatitude->setText(QString::number(settings.value(SETTINGS_COORDINATES_TOP_LEFT_LATITUDE).toDouble()));
    ui->edCornerTopLeftLongitude->setText(QString::number(settings.value(SETTINGS_COORDINATES_TOP_LEFT_LONGITUDE).toDouble()));

    ui->edCornerTopRightLatitude->setText(QString::number(settings.value(SETTINGS_COORDINATES_TOP_RIGHT_LATITUDE).toDouble()));
    ui->edCornerTopRightLongitude->setText(QString::number(settings.value(SETTINGS_COORDINATES_TOP_RIGHT_LONGITUDE).toDouble()));
  }
}

SettingsDialog::~SettingsDialog()
{
  delete ui;
}

void SettingsDialog::accept()
{
  if(ui->edCornerBottomLeftLatitude->text() == "" || ui->edCornerBottomLeftLongitude->text() == "")
  {
    QMessageBox::information(this,
                             APPLICATION_NAME,
                             STD_INFO_CORNER_BOTTOM_LEFT,
                             QMessageBox::Ok);
    return;
  }


  if(ui->edCornerBottomRightLatitude->text() == "" || ui->edCornerBottomRightLongitude->text() == "")
  {
    QMessageBox::information(this,
                             APPLICATION_NAME,
                             STD_INFO_CORNER_BOTTOM_RIGHT,
                             QMessageBox::Ok);
    return;
  }

  if(ui->edCornerTopLeftLatitude->text() == "" || ui->edCornerTopLeftLongitude->text() == "")
  {
    QMessageBox::information(this,
                             APPLICATION_NAME,
                             STD_INFO_CORNER_TOP_LEFT,
                             QMessageBox::Ok);
    return;
  }

  if(ui->edCornerTopRightLatitude->text() == "" || ui->edCornerTopRightLongitude->text() == "")
  {
    QMessageBox::information(this,
                             APPLICATION_NAME,
                             STD_INFO_CORNER_TOP_RIGHT,
                             QMessageBox::Ok);
    return;
  }

#ifdef Q_OS_LINUX
  QSettings settings("." + QApplication::applicationDirPath().left(1) + SETTINGS_FILE_PATH, QSettings::IniFormat);
#elif Q_OS_WIN
  QSettings settings(QApplication::applicationDirPath().left(1) + SETTINGS_FILE_PATH, QSettings::IniFormat);
#endif

  settings.setValue(SETTINGS_COORDINATES_EXISTS, true);

  settings.setValue(SETTINGS_COORDINATES_BOTTOM_LEFT_LATITUDE, ui->edCornerBottomLeftLatitude->text().replace(',',".").toDouble());
  settings.setValue(SETTINGS_COORDINATES_BOTTOM_LEFT_LONGITUDE, ui->edCornerBottomLeftLongitude->text().replace(',',".").toDouble());

  settings.setValue(SETTINGS_COORDINATES_BOTTEM_RIGHT_LATITUDE, ui->edCornerBottomRightLatitude->text().replace(',',".").toDouble());
  settings.setValue(SETTINGS_COORDINATES_BOTTOM_RIGHT_LONGITUDE, ui->edCornerBottomRightLongitude->text().replace(',',".").toDouble());

  settings.setValue(SETTINGS_COORDINATES_TOP_LEFT_LATITUDE, ui->edCornerTopLeftLatitude->text().replace(',',".").toDouble());
  settings.setValue(SETTINGS_COORDINATES_TOP_LEFT_LONGITUDE, ui->edCornerTopLeftLongitude->text().replace(',',".").toDouble());

  settings.setValue(SETTINGS_COORDINATES_TOP_RIGHT_LATITUDE, ui->edCornerTopRightLatitude->text().replace(',',".").toDouble());
  settings.setValue(SETTINGS_COORDINATES_TOP_RIGHT_LONGITUDE, ui->edCornerTopRightLongitude->text().replace(',',".").toDouble());

  settings.sync();

  QDialog::accept();
}

void SettingsDialog::reject()
{
  // do nothing;
  QDialog::reject();
}



