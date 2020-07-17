#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QLabel>
#include <QFrame>
#include "client.h"

static int f;
static char buf[BUFFER_SIZE];
void clienthandler(int client);

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void slotGetNumber();  // СЛОТ для получения номера нажатой динамической кнопки
    void getQuestion();
    void on_pushButton_clicked();
    int change(int i);

private:
    Ui::MainWindow *ui;
    Client cl;
    std::thread t;
    std::vector<QFrame *> frames;
    std::vector<QLabel *> labels;
    int num_frame_1;
    int num_frame_2;
    bool flag;
};

#endif // MAINWINDOW_H
