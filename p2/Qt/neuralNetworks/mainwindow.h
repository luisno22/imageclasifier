#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>

#include "opennn/opennn.h"
using namespace OpenNN;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void generateDataset();
    void trainNetwork();
    void testNetwork();

private:
    Ui::MainWindow *ui;
    const int IMG_WIDTH = 32;
    const int IMG_HEIGTH = 32;

    const float TRAINING_RATIO = 0.6;
    const float SELECITON_RATIO = 0.2;
    const float TESTING_RATIO = 0.2;

};

#endif // MAINWINDOW_H
