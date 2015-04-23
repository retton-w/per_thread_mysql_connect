/** 
 * @file:       mysql_conn.h
 * @brief:      posix thread specified mysql connection  
 * @author:     retton
 * @date:       2015-04-23
 * @version:    V1.0.0
 * @note:       History:
 * @note:       <author><time><version><description>
 * @warning:    
 */
#ifndef __MYSQL_CONN_H__
#define __MYSQL_CONN_H__

#include <mysql/mysql.h>

/** 
 * @name:   my_serv_info
 * @brief:  required information to connect mysqld
 */
struct my_serv_info {
    char    serv_host[NAME_LEN];
    char    serv_user[NAME_LEN];
    char    serv_passwd[NAME_LEN];
    char    serv_database[NAME_LEN];
    int     serv_port;
};

MYSQL *get_mysql(struct my_serv_info *info);

#endif ///< __MYSQL_CONN_H__
