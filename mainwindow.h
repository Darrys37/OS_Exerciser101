#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <QVariant>

class QTableWidget;
class QTabWidget;

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

private:
    Ui::MainWindow *ui;
    QTabWidget *tabs_ = nullptr;

    QTableWidget *tblOs_ = nullptr;
    QTableWidget *tblCpuRam_ = nullptr;
    QTableWidget *tblStorage_ = nullptr;
    QTableWidget *tblPower_ = nullptr;

private:
    void setupUi();

public:
    void fillOs();
    void fillCpuRam();
    void fillStorage();
    void fillPower();
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
};

#endif // MAINWINDOW_H
