#include "mainwindow.h"
#include "ui_mainwindow.h"

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
    f = 1;
    ui->setupUi(this);
    cl.create();
    int client = cl.getclient();
    t = std::thread(clienthandler, client);

    frames.push_back(ui->frame);
    frames.push_back(ui->frame_2);
    frames.push_back(ui->frame_3);
    frames.push_back(ui->frame_4);
    frames.push_back(ui->frame_5);
    frames.push_back(ui->frame_6);
    frames.push_back(ui->frame_7);
    frames.push_back(ui->frame_8);
    frames.push_back(ui->frame_9);
    frames.push_back(ui->frame_10);
    frames.push_back(ui->frame_11);
    frames.push_back(ui->frame_12);
    frames.push_back(ui->frame_13);
    frames.push_back(ui->frame_14);
    frames.push_back(ui->frame_15);
    frames.push_back(ui->frame_16);
    frames.push_back(ui->frame_17);
    frames.push_back(ui->frame_18);
    frames.push_back(ui->frame_19);
    frames.push_back(ui->frame_20);

    labels.push_back(ui->label);
    labels.push_back(ui->label_2);
    labels.push_back(ui->label_3);
    labels.push_back(ui->label_4);
    labels.push_back(ui->label_5);
    labels.push_back(ui->label_6);
    labels.push_back(ui->label_7);
    labels.push_back(ui->label_8);
    labels.push_back(ui->label_9);
    labels.push_back(ui->label_10);
    labels.push_back(ui->label_11);
    labels.push_back(ui->label_12);
    labels.push_back(ui->label_13);
    labels.push_back(ui->label_14);
    labels.push_back(ui->label_15);
    labels.push_back(ui->label_16);
    labels.push_back(ui->label_17);
    labels.push_back(ui->label_18);
    labels.push_back(ui->label_19);
    labels.push_back(ui->label_20);

    for (int i = 0; i < frames.size(); ++i) {
        frames[i]->setStyleSheet("background-color:pink;");
    }

    ui->frame_21->setStyleSheet("background-color:azure;");
    ui->frame_22->setStyleSheet("background-color:azure;");

    QPushButton *button = new QPushButton(this);
    button->setText("играть");
    ui->verticalLayout->addWidget(button);
    connect(button, SIGNAL(clicked()), this, SLOT(on_pushButton_clicked()));
    num_frame_1 = 0;
    num_frame_2 = 0;
    flag = false;
}

MainWindow::~MainWindow()
{
    int client = cl.getclient();
    f = -1;
    send(client, "#", BUFFER_SIZE, 0);
    t.join();
    delete ui;
    cl.drop();
}



void MainWindow::slotGetNumber()
{
    QPushButton *button = (QPushButton*) sender();
    char b_[BUFFER_SIZE];

    int i = 1;
    for(i; i < ui->verticalLayout->count(); i++){
        QPushButton *j = qobject_cast<QPushButton*>(ui->verticalLayout->itemAt(i)->widget());
        if(button == j){
            break;
        }
    }
    b_[0] = 0;
    b_[0] = '^';
    b_[1] = '0' + i;
    b_[2] = 0;

    char buffer[BUFFER_SIZE];

    int client = cl.getclient();

    send(client, b_, BUFFER_SIZE, 0);

    buffer[0] = 0;
    f = 0;
    while (f == 0) {}
    strcat(buffer, buf);


    i = ui->verticalLayout->count() - 1;
    while (i >= 1){
        auto ob = ui->verticalLayout->itemAt(i)->widget();
        ob->hide();
        delete ob;
        i = ui->verticalLayout->count() - 1;
    }

    char msg[BUFFER_SIZE];
    msg[0] = 0;
    int s = 0;
    for (int i = 2; i < strlen(buffer); ++i) {
        if (buffer[i] == '$') {
            strcat(msg, "\n");
            s = strlen(msg);
        } else {
            msg[s] = buffer[i];
            s++;
        }
    }
    msg[s] = 0;
    ui->label_22->clear();
    ui->label_22->setText(msg);
    ui->label_22->setAlignment(Qt::AlignHCenter);

    QPushButton *b = new QPushButton(this);
    b->setText("Вопрос");
    ui->verticalLayout->addWidget(b);
    connect(b, SIGNAL(clicked()), this, SLOT(getQuestion()));

    if (buffer[1] == '1') {
        int res = change(0);
        if (res == 1) {
            b->hide();
            delete b;
            ui->label_22->clear();
            ui->label_22->setText("Вы выиграли!");
            ui->label_22->setWordWrap(true);
            ui->label_22->setAlignment(Qt::AlignHCenter);
        }
    }

}

void MainWindow::getQuestion() {
    int k = ui->verticalLayout->count() - 1;
    while (k >= 1){
        auto ob = ui->verticalLayout->itemAt(k)->widget();
        ob->hide();
        delete ob;
        k = ui->verticalLayout->count() - 1;
    }

    int client = cl.getclient();
    send(client, "@", BUFFER_SIZE, 0);

    char buffer[BUFFER_SIZE];
    int i = 1;

    int res = 0;

    if (!flag) {
        ui->label_21->clear();
        ui->label_21->setText("Пожалуйста, подождите... Ожидается ответ другого игрока");
        ui->label_21->setAlignment(Qt::AlignHCenter);
        this->repaint();
        buffer[0] = 0;
        f = 0;
        while (f == 0) {}
        strcat(buffer, buf);

        if (buffer[0] == '!') {
            if (buffer[1] == '1') {
                res = change(1);
            }
        }
    } else {
        flag = false;
    }
    buffer[0] = 0;
    f = 0;
    while (f == 0) {}
    strcat(buffer, buf);
    if (res != -1) {
        char q[BUFFER_SIZE];
        q[0] = 0;
        int q_s = 0;
        while (buffer[i] != '$') {
            q[q_s] = buffer[i];
            q_s++;
            i++;
        }
        q[q_s] = 0;
        ui->label_22->clear();
        ui->label_22->setText(q);
        ui->label_22->setWordWrap(true);
        ui->label_22->setAlignment(Qt::AlignHCenter);

        char a[BUFFER_SIZE];
        int a_s = 0;
        i++;
        for(i; i < strlen(buffer); ++i) {
            if (buffer[i] == '$') {
                a[a_s] = 0;
                QPushButton *button = new QPushButton(this);
                button->setText(a);
                ui->verticalLayout->addWidget(button);
                connect(button, SIGNAL(clicked()), this, SLOT(slotGetNumber()));
                a[0] = 0;
                a_s = 0;
             } else {
                a[a_s] = buffer[i];
                a_s ++;
             }
        }
        a[a_s] = 0;
        QPushButton *button = new QPushButton(this);
        button->setText(a);
        ui->verticalLayout->addWidget(button);
        connect(button, SIGNAL(clicked()), this, SLOT(slotGetNumber()));

        ui->label_21->clear();

    } else {
        ui->label_21->clear();
        ui->label_22->clear();
        ui->label_22->setText("Вы проиграли!");
        ui->label_22->setWordWrap(true);
        ui->label_22->setAlignment(Qt::AlignHCenter);
    }

}


void MainWindow::on_pushButton_clicked()
{
    int client = cl.getclient();
    send(client, "=", BUFFER_SIZE, 0);

    char buffer[BUFFER_SIZE];
    buffer[0] = 0;
    f = 0;
    while (f == 0) {}
    strcat(buffer, buf);

    if (buffer[0] == '!') {
        ui->label_21->clear();
        ui->label_21->setText("Пожалуйста, подождите... Ожидается подключение другого игрока");
        ui->label_21->setAlignment(Qt::AlignHCenter);
        this->repaint();
        buffer[0] = 0;
        f = 0;
        while (f == 0) {}
        strcat(buffer, buf);
    }
    if (buffer[1] == '0' && flag == false) {
        flag = true;
    }
    QPushButton *button = new QPushButton(this);
    button->setText("Вопрос");
    ui->verticalLayout->addWidget(button);
    connect(button, SIGNAL(clicked()), this, SLOT(getQuestion()));

    ui->frame->setStyleSheet("background-color:green;");
    ui->label->setText("я");
    ui->label->setAlignment(Qt::AlignHCenter);

    auto ob = ui->verticalLayout->itemAt(1)->widget();
    ob->hide();
    delete ob;

    ui->label_21->clear();
    this->repaint();
}

int MainWindow::change(int i) {
    int res = 0;
    int num_frame;
    int num_frame_opponent;
    char s[BUFFER_SIZE];
    char s_opponent[BUFFER_SIZE];
    char name[BUFFER_SIZE];
    char name_opponent[BUFFER_SIZE];
    name_opponent[0] = 0;
    name[0] = 0;
    s_opponent[0] = 0;
    s[0] = 0;
    if (i == 0) {
        num_frame = num_frame_1;
        num_frame_1++;
        strcat(s, "background-color:green;");
        num_frame_opponent = num_frame_2;
        strcat(s_opponent, "background-color:red;");
        strcat(name, "я");
        strcat(name_opponent, "соперник");
    } else {
        num_frame = num_frame_2;
        num_frame_2++;
        strcat(s, "background-color:red;");
        num_frame_opponent = num_frame_1;
        strcat(s_opponent, "background-color:green;");
        strcat(name, "соперник");
        strcat(name_opponent, "я");
    }
    if (num_frame_1 == num_frame_2) {
        strcat(s, "background-color:blue;");
        name[0] = 0;
        strcat(name, "вместе");
    }
    num_frame++;
    if (num_frame < 20) {
        labels[num_frame - 1]->clear();
        labels[num_frame]->clear();
        if (num_frame - 1 == num_frame_opponent) {
            frames[num_frame - 1]->setStyleSheet(s_opponent);
            labels[num_frame - 1]->setText(name_opponent);
        } else {
            frames[num_frame - 1]->setStyleSheet("background-color:pink;");
            labels[num_frame]->setText("");
        }
        labels[num_frame - 1]->setAlignment(Qt::AlignHCenter);
        frames[num_frame]->setStyleSheet(s);
        labels[num_frame]->setText(name);
        labels[num_frame]->setAlignment(Qt::AlignHCenter);
    } else {
        if (strlen(name) == 2) {
            res = 1;
        } else {
            res = -1;
        }
        labels[num_frame - 1]->clear();
        frames[num_frame - 1]->setStyleSheet("background-color:pink;");
        labels[num_frame_opponent]->clear();
        frames[num_frame_opponent]->setStyleSheet("background-color:pink;");

    }
    return res;
}

