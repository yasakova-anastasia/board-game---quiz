#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "admin.h"

#include <QMainWindow>

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
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void update_table();

private:
    Ui::MainWindow *ui;
    admin ad;
    std::thread t;
};

#endif // MAINWINDOW_H
