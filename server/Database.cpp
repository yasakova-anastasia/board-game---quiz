#include "Database.h"
#include <my_global.h>
#include <iostream>
#include <cstring>
#include <string>
#include <mysql.h>



Database::Database() {
    MYSQL *con = mysql_init(NULL);
    if (con == NULL) {
        std::cout << "mysql_init() failed\n";
        exit(1);
    }

    if (mysql_real_connect(con, DB_HOST, DB_USER, DP_PASS, DB_NAME, 0, NULL, 0) == NULL) {
        std::cout << mysql_error(con) << std::endl;
        mysql_close(con);
        exit(1);
    }

    if (mysql_query(con, "SELECT * FROM " DB_TABLE)) {
        std::cout << mysql_error(con) << std::endl;
        mysql_close(con);
        exit(1);
    }

    MYSQL_RES *result = mysql_store_result(con);
  
    if (result == NULL) {
        std::cout << mysql_error(con) << std::endl;
        mysql_close(con);
        exit(1);
    }

    int tmp = mysql_num_rows(result);
    int j = 0;
    while (tmp > 0) {
        int m = tmp % 10;
        counter[j] = '0' + m;
        j++;
        tmp /= 10;
    }
    counter[j] = '\0';
    for (int i = 0; i < j / 2; i++) {
        counter[j] = counter[i];
        counter[i] = counter[j - i - 1];
        counter[j - i - 1] = counter[j];
    }
    counter[j] = '\0';
    mysql_free_result(result);
    mysql_close(con);
}

Database::~Database() {
}

void Database::getAmount(char* cnt) const {
    strcpy(cnt, counter);
}

int Database::getAmount() const {
    return atoi(counter);
}

void Database::addStr(const char * q, const char * a, const char * r) {
    MYSQL *con = mysql_init(NULL);
    if (con == NULL) {
        std::cout << "mysql_init() failed\n";
        exit(1);
    }

    if (mysql_real_connect(con, DB_HOST, DB_USER, DP_PASS, DB_NAME, 0, NULL, 0) == NULL) {
        std::cout << mysql_error(con) << std::endl;
        mysql_close(con);
        exit(1);
    }
    char buffer[BUFFER_SIZE];
    buffer[0] = 0;
    strcat(buffer, "INSERT INTO questions VALUES(");
    change_counter(1);
    strcat(buffer, counter);
    strcat(buffer, ", '");
    strcat(buffer, q);
    strcat(buffer, "', '");
    strcat(buffer, a);
    strcat(buffer, "', ");
    strcat(buffer, r);
    strcat(buffer, ")");

    if (mysql_query(con, buffer)) {
        std::cout << mysql_error(con) << std::endl;
        mysql_close(con);
        exit(1);
    }
    mysql_close(con);
}

void Database::delStr(const char * num) {
    MYSQL *con = mysql_init(NULL);
    if (con == NULL) {
        std::cout << "mysql_init() failed\n";
        exit(1);
    }

    if (mysql_real_connect(con, DB_HOST, DB_USER, DP_PASS, DB_NAME, 0, NULL, 0) == NULL) {
        std::cout << mysql_error(con) << std::endl;
        mysql_close(con);
        exit(1);
    }

    if (mysql_query(con, "SELECT * FROM " DB_TABLE)) {
        std::cout << mysql_error(con) << std::endl;
        mysql_close(con);
        exit(1);
    }

    MYSQL_RES *result = mysql_store_result(con);
  
    if (result == NULL) {
        std::cout << mysql_error(con) << std::endl;
        mysql_close(con);
        exit(1);
    }

    int cnt = mysql_num_rows(result);

    int tmp = atoi(num);

    if (tmp > cnt) {
        std::cout << "Такой строки нет.\n";
    } else {
        char buffer[BUFFER_SIZE];
        buffer[0] = 0;
        strcat(buffer, "DELETE FROM questions WHERE Id = ");
        strcat(buffer, num);

        if (mysql_query(con, buffer)) {
            std::cout << mysql_error(con) << std::endl;
            mysql_close(con);
            exit(1);
        }

        if (strcmp(num, counter) != 0) {
            buffer[0] = 0;
            strcat(buffer, "SELECT * FROM questions WHERE Id =");
            strcat(buffer, counter);
            if (mysql_query(con, buffer)) {
                std::cout << mysql_error(con) << std::endl;
                mysql_close(con);
                exit(1);
            }
            result = mysql_store_result(con);
            MYSQL_ROW row = mysql_fetch_row(result);


            buffer[0] = 0;
            strcat(buffer, "DELETE FROM questions WHERE Id = ");
            strcat(buffer, counter);
            if (mysql_query(con, buffer)) {
                std::cout << mysql_error(con) << std::endl;
                mysql_close(con);
                exit(1);
            }

            buffer[0] = 0;
            strcat(buffer, "INSERT INTO questions VALUES(");
            strcat(buffer, num);
            strcat(buffer, ", '");
            strcat(buffer, row[1]);
            strcat(buffer, "', '");
            strcat(buffer, row[2]);
            strcat(buffer, "', ");
            strcat(buffer, row[3]);
            strcat(buffer, ")");

            if (mysql_query(con, buffer)) {
                std::cout << mysql_error(con) << std::endl;
                mysql_close(con);
                exit(1);
            }

        }
        change_counter(-1);
    }
    mysql_free_result(result);
    mysql_close(con);
}

void Database::change_counter(int i) {

    int amount = atoi(counter);
    amount += i;
    if (amount > 9) {
        counter[0] = '0' + amount / 10;
        counter[1] = '0' + amount % 10;
        counter[2] = 0;
    } else {
        counter[0] = '0' + amount;
        counter[1] = 0;
    }
}

void Database::getStr(int num, char * q, char * a, char * r) {
    MYSQL *con = mysql_init(NULL);
    if (con == NULL) {
        std::cout << "mysql_init() failed\n";
        exit(1);
    }

    if (mysql_real_connect(con, DB_HOST, DB_USER, DP_PASS, DB_NAME, 0, NULL, 0) == NULL) {
        std::cout << mysql_error(con) << std::endl;
        mysql_close(con);
        exit(1);
    }

    if (mysql_query(con, "SELECT * FROM " DB_TABLE)) {
        std::cout << mysql_error(con) << std::endl;
        mysql_close(con);
        exit(1);
    }

    MYSQL_RES *result = mysql_store_result(con);
  
    if (result == NULL) {
        std::cout << mysql_error(con) << std::endl;
        mysql_close(con);
        exit(1);
    }

    int cnt = mysql_num_rows(result);

    if (num > cnt) {
        std::cout << "Такой строки нет.\n";
    } else {
        char tmp[BUFFER_SIZE];
        int j = 0;
        while (num > 0) {
            int m = num % 10;
            tmp[j] = '0' + m;
            j++;
            num /= 10;
        }
        tmp[j] = '\0';
        for (int i = 0; i < j / 2; i++) {
            tmp[j] = tmp[i];
            tmp[i] = tmp[j - i - 1];
            tmp[j - i - 1] = tmp[j];
        }
        tmp[j] = '\0';

        char buffer[BUFFER_SIZE];
        buffer[0] = 0;
        strcat(buffer, "SELECT * FROM questions WHERE Id =");
        strcat(buffer, tmp);
        if (mysql_query(con, buffer)) {
            std::cout << mysql_error(con) << std::endl;
            mysql_close(con);
            exit(1);
        }
        result = mysql_store_result(con);
        MYSQL_ROW row = mysql_fetch_row(result);

        q[0] = 0;
        a[0] = 0;
        r[0] = 0;
        strcat(q, row[1]);
        strcat(a, row[2]);
        strcat(r, row[3]);
        
        mysql_free_result(result);
        mysql_close(con);
    }
}

void Database::print_db(char * res) {
    MYSQL *con = mysql_init(NULL);
    if (con == NULL) {
        std::cout << mysql_error(con) << std::endl;
        mysql_close(con);
        exit(1);
    }
    if (mysql_real_connect(con, DB_HOST, DB_USER, DP_PASS, DB_NAME, 0, NULL, 0) == NULL) {
        std::cout << mysql_error(con) << std::endl;
        mysql_close(con);
        exit(1);
    }
    if (mysql_query(con, "SELECT * FROM " DB_TABLE)) {
        std::cout << mysql_error(con) << std::endl;
        mysql_close(con);
        exit(1);
    }
  
    MYSQL_RES *result = mysql_store_result(con);
    if (result == NULL) {
        std::cout << mysql_error(con) << std::endl;
        mysql_close(con);
        exit(1);
    }
    int num_fields = mysql_num_fields(result);
    MYSQL_ROW row;
    strcat(res, "%");
    strcat(res, counter);
    strcat(res, "%");
    while ((row = mysql_fetch_row(result))) { 
        for(int i = 0; i < num_fields; i++) { 
            strcat(res, row[i]);
            strcat(res, "%");
        }
        strcat(res, "%");
    }
    mysql_free_result(result);
    mysql_close(con);
}