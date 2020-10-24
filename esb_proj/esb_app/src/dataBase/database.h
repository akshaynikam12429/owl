/*
THE QUERY COMMANDS ARE WRITTEN HERE... 
*/
#ifndef DATABASE
#define DATABASE

#define INSERT_IN_ESB_REQUEST "INSERT INTO esb_request(sender_id,dest_id,message_type,reference_id,message_id,received_on,status) VALUES ('%s','%s','%s','%s','%s','%s','%s');"

#define server "localhost"  //MYSQL SERVER
#define user "root"         //USER NAME IN MYSQL SERVER
#define password "my sql"   //PASSWORD FOR MYSQL SERVER
#define database "esb"      //SELECTED DATABASE
#define port 3306           //PORT NAME CAN BE FOUND BY... mysql> SHOW global variables WHERE Variable_name = 'port';

int insert_in_esb_request(BMD * bmd);


#endif