#include "settingsdialog.h"
#include "ui_settingsdialog.h"
#include <cfloat>

SettingsDialog::SettingsDialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::SettingsDialog)
{
  ui->setupUi(this);

  // only numbers should be avaible
  ui->edCornerBottomLeft->setValidator(new QDoubleValidator(0, DBL_MAX, 10));
  ui->edCornerBottomRight->setValidator(new QDoubleValidator(0, DBL_MAX, 10));
  ui->edCornerTopLeft->setValidator(new QDoubleValidator(0, DBL_MAX, 10));
  ui->edCornerTopRight->setValidator(new QDoubleValidator(0, DBL_MAX, 10));
}

SettingsDialog::~SettingsDialog()
{
  delete ui;
}
