#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QStandardItemModel"
#include "QStandardItem"
#include "QDebug"

void clienthandler(int client) {
        while (true) {
            while (f == 1 && f != -1) {}
            if (f == -1) {
                break;
            }
            if (f == 0) {
                buf[0] = 0;
            }
            recv(client, buf, sizeof(buf), NULL);
            f = 1;

        }
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ad.create();
    f = 1;
    int client = ad.getadmin();
    t = std::thread(clienthandler, client);

    send(client, "~", BUFFER_SIZE, 0);
    char buffer[BUFFER_SIZE];
    buffer[0] = 0;
    f = 0;
    while (f == 0) {}
    strcat(buffer, buf);
    std::cout << buffer <<std::endl;

    QStandardItemModel *model = new QStandardItemModel;

    QStringList horizontalHeader;
    horizontalHeader.append("Вопрос");
    horizontalHeader.append("Ответы");
    horizontalHeader.append("Номер правильного ответа");

    QStringList verticalHeader;

    char n[BUFFER_SIZE];
    int n_s = 0;
    char q[BUFFER_SIZE];
    int q_s = 0;
    char a[BUFFER_SIZE];
    int a_s = 0;
    char r[BUFFER_SIZE];
    int r_s = 0;
    int i = 1;
    int cnt = 0;
    char amount[BUFFER_SIZE];
    amount[0] = 0;
    int amount_size = 0;
    while (buffer[i] != '%') {
        amount[amount_size] = buffer[i];
        i++;
        amount_size++;
    }
    amount[amount_size] = 0;
    int count = atoi(amount);
    for (int j = 1; j <= count; ++j) {
        char tmp[BUFFER_SIZE];
        if (j > 9) {
            tmp[0] = '0' + j / 10;
            tmp[1] = '0' + j % 10;
            tmp[2] = 0;
        } else {
            tmp[0] = '0' + j;
            tmp[1] = 0;
        }
        verticalHeader.append(tmp);
    }
    i++;
    while (buffer[i] != 0) {
        QStandardItem *item;
        n[0] = 0;
        q[0] = 0;
        a[0] = 0;
        r[0] = 0;
        q_s = 0;
        a_s = 0;
        r_s = 0;
        while(buffer[i] != '%') {
            n[n_s] = buffer[i];
            n_s++;
            i++;
        }
        n[n_s] = 0;
        i++;
        while (buffer[i] != '%') {
            q[q_s] = buffer[i];
            q_s++;
            i++;
        }
        q[q_s] = 0;
        i++;
        item = new QStandardItem(QString(q));
        model->setItem(cnt, 0, item);
        while (buffer[i] != '%') {
            a[a_s] = buffer[i];
            a_s++;
            i++;
        }
        a[a_s] = 0;
        i++;
        item = new QStandardItem(QString(a));
        model->setItem(cnt, 1, item);
        while (buffer[i] != '%') {
            r[r_s] = buffer[i];
            r_s++;
            i++;
        }
        r[r_s] = 0;
        i++;
        i++;
        item = new QStandardItem(QString(r));
        model->setItem(cnt, 2, item);

        cnt++;
    }

    model->setHorizontalHeaderLabels(horizontalHeader);
    model->setVerticalHeaderLabels(verticalHeader);

    ui->tableView->setModel(model);

    ui->tableView->resizeRowsToContents();
    ui->tableView->resizeColumnsToContents();

}

MainWindow::~MainWindow()
{
    int client = ad.getadmin();
    f = -1;
    send(client, "#", BUFFER_SIZE, 0);
    t.join();
    delete ui;
    ad.drop();
}

void MainWindow::on_pushButton_clicked()
{
    QString q_ = ui->textEdit->toPlainText();
    char q[BUFFER_SIZE];
    q[0] = 0;
    strcat(q, q_.toUtf8().constData());
    QString a_ = ui->textEdit_2->toPlainText();
    char a[BUFFER_SIZE];
    a[0] = 0;
    strcat(a, a_.toUtf8().constData());
    QString r_ = ui->textEdit_3->toPlainText();
    char r[BUFFER_SIZE];
    r[0] = 0;
    strcat(r, r_.toUtf8().constData());
    char buffer[BUFFER_SIZE];
    buffer[0] = 0;
    strcat(buffer, "+");
    int buffer_s = 1;
    for (int i = 0; i < strlen(q); ++i) {
        buffer[buffer_s] = q[i];
        buffer_s++;
    }
    buffer[buffer_s] = '%';
    buffer_s++;
    for (int i = 0; i < strlen(a); ++i) {
        buffer[buffer_s] = a[i];
        buffer_s++;
    }
    buffer[buffer_s] = '%';
    buffer_s++;
    for (int i = 0; i < strlen(r); ++i) {
        buffer[buffer_s] = r[i];
        buffer_s++;
    }
    buffer[buffer_s] = 0;
    int client = ad.getadmin();
    send(client, buffer, BUFFER_SIZE, 0);
    update_table();
}

void MainWindow::on_pushButton_2_clicked() {
    QString n_ = ui->textEdit_4->toPlainText();
    char n[BUFFER_SIZE];
    n[0] = 0;
    strcat(n, n_.toUtf8().constData());
    char buffer[BUFFER_SIZE];
    buffer[0] = 0;
    strcat(buffer, "-");
    strcat(buffer, n);
    int client = ad.getadmin();
    send(client, buffer, BUFFER_SIZE, 0);
    update_table();
}

void MainWindow::update_table() {
    char buffer[BUFFER_SIZE];
    buffer[0] = 0;
    f = 0;
    while (f == 0) {}
    strcat(buffer, buf);
    QStandardItemModel *model = new QStandardItemModel;

    QStringList horizontalHeader;
    horizontalHeader.append("Вопрос");
    horizontalHeader.append("Ответы");
    horizontalHeader.append("Номер правильного ответа");

    QStringList verticalHeader;


    char n[BUFFER_SIZE];
    int n_s = 0;
    char q[BUFFER_SIZE];
    int q_s = 0;
    char a[BUFFER_SIZE];
    int a_s = 0;
    char r[BUFFER_SIZE];
    int r_s = 0;
    int i = 1;
    int cnt = 0;

    char amount[BUFFER_SIZE];
    amount[0] = 0;
    int amount_size = 0;
    while (buffer[i] != '%') {
        amount[amount_size] = buffer[i];
        i++;
        amount_size++;
    }
    amount[amount_size] = 0;
    int count = atoi(amount);
    for (int j = 1; j <= count; ++j) {
        char tmp[BUFFER_SIZE];
        if (j > 9) {
            tmp[0] = '0' + j / 10;
            tmp[1] = '0' + j % 10;
            tmp[2] = 0;
        } else {
            tmp[0] = '0' + j;
            tmp[1] = 0;
        }
        verticalHeader.append(tmp);
    }
    i++;

    while (buffer[i] != 0) {
        QStandardItem *item;
        n[0] = 0;
        q[0] = 0;
        a[0] = 0;
        r[0] = 0;
        q_s = 0;
        a_s = 0;
        r_s = 0;
        while(buffer[i] != '%') {
            n[n_s] = buffer[i];
            n_s++;
            i++;
        }
        n[n_s] = 0;
        i++;
        while (buffer[i] != '%') {
            q[q_s] = buffer[i];
            q_s++;
            i++;
        }
        q[q_s] = 0;
        i++;
        item = new QStandardItem(QString(q));
        model->setItem(cnt, 0, item);
        while (buffer[i] != '%') {
            a[a_s] = buffer[i];
            a_s++;
            i++;
        }
        a[a_s] = 0;
        i++;
        item = new QStandardItem(QString(a));
        model->setItem(cnt, 1, item);
        while (buffer[i] != '%') {
            r[r_s] = buffer[i];
            r_s++;
            i++;
        }
        r[r_s] = 0;
        i++;
        i++;
        item = new QStandardItem(QString(r));
        model->setItem(cnt, 2, item);

        cnt++;
    }

    model->setHorizontalHeaderLabels(horizontalHeader);
    model->setVerticalHeaderLabels(verticalHeader);

    ui->tableView->setModel(model);

    ui->tableView->resizeRowsToContents();
    ui->tableView->resizeColumnsToContents();


}
