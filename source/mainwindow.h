#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "mannschaft.h"
#include "ui_mainwindow.h"
#include "chartwidget.h"


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
  ChartWidget* mChartWidget;

  void createMenusAndActions();
  QStringList showFileOpenDialog();

  Mannschaft mMannschaftHeim;
  Mannschaft mMannschaftGegner;

public slots:  
  void showFileOpenDialogMannschaftHeim();
  void showFileOpenDialogMannschaftGegner();
  void showFileOpenDialogAddPlayer();
  void reDrawSpielerList();


};

#endif // MAINWINDOW_H
