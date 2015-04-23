/** 
 * @file:       mysql_conn.c
 * @brief:      posix thread specified mysql connection implemention
 * @author:     retton
 * @date:       2015-04-23
 * @version:    V1.0.0
 * @note:       History:
 * @note:       <author><time><version><description>
 * @warning:    
 */
#include "mysql_conn.h"

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

static pthread_key_t my_key;
static pthread_once_t key_once = PTHREAD_ONCE_INIT;
static pthread_mutex_t my_lock = PTHREAD_MUTEX_INITIALIZER;

void free_my(void *arg)
{
    mysql_close((MYSQL *)arg);
    free((MYSQL *)arg);
}

static void mk_key()
{
    pthread_key_create(&my_key, free_my);
}

int lc_my_connect(struct my_serv_info *info, MYSQL *mysql)
{
    int mysql_timeout = 10;

    mysql_init(mysql);
    mysql_options(mysql, MYSQL_OPT_CONNECT_TIMEOUT, &mysql_timeout);
    if (NULL == mysql_real_connect(mysql, 
                info->serv_host, 
                info->serv_user, 
                info->serv_passwd,
                info->serv_database,
                info->serv_port,
                NULL, 0)) {
        printf("cannot connect to mysql server!\n");
        return -1;
    }
    return 0;
}

MYSQL *get_mysql(struct my_serv_info *info)
{
    MYSQL *mysql = NULL;
    pthread_once(&key_once, mk_key);
    pthread_mutex_lock(&my_lock);
    if ((mysql = pthread_getspecific(my_key)) == NULL) {
        mysql = (MYSQL *)malloc(sizeof(MYSQL));
        if (NULL == mysql) {
            printf("malloc error!\n");
            pthread_mutex_unlock(&my_lock);
            exit(-1);
        }
        pthread_setspecific(my_key, mysql);
        if (0 != lc_my_connect(info, mysql)) {
            pthread_mutex_unlock(&my_lock);
            return NULL;
        }
    } else {
        //if (0 != mysql_query(mysql, "DO 1")) {
        //use mysql_ping to check the connection ok or not
        if (0 != mysql_ping(mysql)) {
            printf("mysql disconnect!\n");
            if (0 != lc_my_connect(info, mysql)) {
                pthread_mutex_unlock(&my_lock);
                return NULL;
            }
        }
    }
    pthread_mutex_unlock(&my_lock);
    return mysql;
}
