/*

IT EXTRACTS THE DATA PRESENT IN BMD(XML) FILE..
AND STORES IN THE DATABASE TABLE "esb_request"..

TO COMPILE: gcc -o output -Wall $(xml2-config --cflags) $(xml2-config --libs) $(mysql_config --cflags) database.c  $(mysql_config --libs) -lxml2

*/


#include <stdio.h>
#include <string.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include "../parsing/bmd_parser.h"
#include "../parsing/parse.c"
#include <mysql/mysql.h>
#include "database.h"



int insert_in_esb_request(BMD * bmd) {
    int success = 1; //1=>success , -ve for errors

    MYSQL * conn;
    MYSQL_RES * res;
    MYSQL_ROW row;

    int8_t received_time[100]; // eg: "2020-08-12T05:18:00+0000";
    int8_t received_on[100];
    strcpy(received_time, bmd -> bmd_envelope -> CreationDateTime);

    /*Changing DateTime format*/
    int n = strlen(received_time)-6;
    int j = 0;
    for (int i = 0; i <= n; i++) {
        received_on[j] = received_time[i];
        if (received_on[i] == 'T') //test for character
        {
            received_on[i] = ' '; // change T to space
        }
        j++;
    }

    conn = mysql_init(NULL);

    /* Connect to database */
    if (!mysql_real_connect(conn, server, user, password, database, 0, NULL, 0)) {
        printf("Failed to connect MySQL Server %s. Error: %s\n", server, mysql_error(conn));
        success = -1;
        return success;
    }

    /*sql query to insert in table*/
    char * status = "available";
    char query1[5000];

    sprintf(query1, INSERT_IN_ESB_REQUEST,
        bmd -> bmd_envelope -> Sender,
        bmd -> bmd_envelope -> Destination,
        bmd -> bmd_envelope -> MessageType,
        bmd -> bmd_envelope -> ReferenceID,
        bmd -> bmd_envelope -> MessageID,
        received_on, status);

    printf("\n\n%s\n\n", query1);

    if (mysql_query(conn, query1)) {
        printf("Failed to execute query.Error: %s\n", mysql_error(conn));
        success = -1;
        return success;
    }
    mysql_query(conn, "SELECT * FROM esb_request");

    res = mysql_use_result(conn);

    /*free the result*/

    mysql_free_result(res);
    mysql_close(conn);
    return success;
}

int main()
 {
     int dbcon=0;
    BMD *bmd = (BMD *)(malloc(sizeof(BMD *)));
    
    bmd=processXML("/home/amruth/work/nho2020/bmd.xml");
	dbcon = insert_in_esb_request(bmd);
    
    printf("\n%d is the query status\n SUCCESS=1\n FAIL=-ve value\n",dbcon);
    return 0;
 }
