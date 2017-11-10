#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "mannschaft.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

private:
  Ui::MainWindow *ui;

  void createMenusAndActions();

  Mannschaft a;
  Mannschaft b;

public slots:
  void showFileOpenDialogMannschaftA();
  void showFileOpenDialogMannschaftB();
};

#endif // MAINWINDOW_H
