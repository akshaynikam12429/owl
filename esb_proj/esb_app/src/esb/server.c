/*
To compile:
    gcc   server.c esb.c ../adapter/email.c ../adapter/xmltojson.c ftpp.c -o ipc -Wall $(xml2-config --cflags) 
    $(xml2-config --libs) -lxml2  $(mysql_config --cflags) $(mysql_config --libs) -lpthread -ljson-c -lcurl

All in a single line

to see output: ./ipc

*/


#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <pthread.h>
#include <sys/un.h>
#include <stddef.h>
#include "esb.h"
#include<stdlib.h>
#include<mysql/mysql.h>
#include <string.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include "server.h"
#include "../adapter/transform.h"
#include "../adapter/transform_to_csv.h"
#include "ftp.h"
#include "http.h"

extern TD process_esb_request(char* bmd_file_path);
bool create_worker_thread(int fd);
void log_msg(const char *msg, bool terminate) {
    printf("%s\n", msg);
    if (terminate) exit(-1); /* failure */
}
int make_named_socket(const char *socket_file, bool is_client) {
    printf("Creating AF_LOCAL socket at path %s\n", socket_file);
    if (!is_client && access(socket_file, F_OK) != -1) {
        log_msg("An old socket file exists, removing it.", false);
        if (unlink(socket_file) != 0) {
            log_msg("Failed to remove the existing socket file.", true);
        }
    }
    struct sockaddr_un name;
    /* Create the socket. */
    int sock_fd = socket(AF_LOCAL, SOCK_STREAM, 0);
    if (sock_fd < 0) {
        log_msg("Failed to create socket.", true);
    }

    /* Bind a name to the socket. */
    name.sun_family = AF_LOCAL;
    strncpy (name.sun_path, socket_file, sizeof(name.sun_path));
    name.sun_path[sizeof(name.sun_path) - 1] = '\0';

    /* The size of the address is
       the offset of the start of the socket_file,
       plus its length (not including the terminating null byte).
       Alternatively you can just do:
       size = SUN_LEN (&name);
   */
    size_t size = (offsetof(struct sockaddr_un, sun_path) +
                   strlen(name.sun_path));
    if (is_client) {
        if (connect(sock_fd, (struct sockaddr *) &name, size) < 0) {
            log_msg("connect failed", 1);
        }
    } else {
        if (bind(sock_fd, (struct sockaddr *) &name, size) < 0) {
            log_msg("bind failed", 1);
        }
    }
    return sock_fd;
}

void thread_function(int sock_fd) {
    log_msg("SERVER: thread_function: starting", false);
    char buffer[5000];
    memset(buffer, '\0', sizeof(buffer));
    
    int count = read(sock_fd, buffer, sizeof(buffer));
    if (count > 0) {
        printf("SERVER: Received from client: %s\n", buffer);
        write(sock_fd, buffer, sizeof(buffer)); /* echo as confirmation */
    }
    
    char pth[50];
    strcpy(pth,"../.");             /* The file path is modified to locate the file properly in correct folder */   
    strcat(pth,buffer);             /* Buffer contains the file path from client */
    printf("path==>%s\n",pth);
    TD st = process_esb_request(pth);

   if(st.val==1)
   {
       printf("BMD file succesfully processed and stored\n");
   }
   else
   {
        close(sock_fd); /* break connection */
    	log_msg("SERVER: thread_function: Done. Worker thread terminating.", false);
    	pthread_exit(NULL);
   }
   /*storing the content of structure in the string literals*/
    char *transport_key = malloc(strlen(st.Transport_key)+1);
    strcpy(transport_key,st.Transport_key);
    char *transport_value = malloc(strlen(st.Transport_value)+1);
    strcpy(transport_value,st.Transport_value);
    char *transform_value = malloc(strlen(st.Transform_value)+1);
    strcpy(transform_value,st.Transform_value);
    char *transform_key = malloc(strlen(st.Transform_key)+1);
    strcpy(transform_key,st.Transform_key);
    printf("\n\n\nline 105 %s	%s	%s	%s\n\n\n",transport_key,transport_value,transform_value,transform_key);
    
    char * filp;                           //declaraing the string variable
    /*checking the transform value and respective transformation (xml to json or xml to csv) is done*/ 
    if(!(strcmp(transform_value,"Json")))         //checking for Json as transform value
    {   
        filp = transformjson(pth);
        printf("\n\nfilp is : %s\n\n\n",filp);

    }
    
   if(!(strcmp(transform_value,"CSV")))              //checking for CSV as transform value
    {    
         BMD * bmd1 =  parse_bmd_xml(pth);
         char * data=bmd1->bmd_payload->data;
         printf("data is %s\n",data);
         char * content = transformCSV(data);         
         printf("\n\nfilename is : %s\n\n\n",content);
         
         FILE * fp=fopen(content,"r"); /* opening the local file and reading the content of it and storing it into the string*/
         int c,i=0;
         char buff[20]={0};
         while((c=fgetc(fp))!= EOF){
             buff[i]=c;
             i++;
         }
         filp=buff;
         printf("\n\nfilp is : %s\n\n\n",filp);
         
    
    }
   
    
  

    /* sending file content via email*/  
    if(!(strcmp(transport_value,"email")))
    {
        printf("payload=%s\n",filp);
        int emailsent = emailsender(transport_key,filp);           /* email id and payload data is passed to email sender function */
        if(emailsent == 1)
        {
            printf("Email sent to %s successfully\n",transport_key);
        }

    }

   /* sending file content via ftp*/
   if(!(strcmp(transport_value,"ftp")))
   {
        char * URL =transport_key;
        printf("url is %s\n",URL);
        int ftpst = send_ftp_file(URL);         //url is obtained from db table
        if(ftpst==1)
        {
            printf("The file has been succesfully transported via FTP server %s\n",URL);
        }
   }
   
   /* sending file content via http*/
   if(!(strcmp(transport_value,"http")))
   {
       const char * url_http = transport_key;
       
       int resp = http_request(url_http, filp);
       if(resp==1)
       {
           printf("%s is successfully sent via HTTP %s\n",filp, url_http);
       }
       
   }
    
    close(sock_fd); /* break connection */
    log_msg("SERVER: thread_function: Done. Worker thread terminating.", false);
    pthread_exit(NULL); // Must be the last statement
}

/**
 * This function launches a new worker thread.
 * @param sock_fd
 * @return Return true if thread is successfully created, otherwise false.
 */
bool create_worker_thread(int sock_fd) {
    log_msg("SERVER: Creating a worker thread.", false);
    pthread_t thr_id;
    int rc = pthread_create(&thr_id,
            /* Attributes of the new thread, if any. */
                            NULL,
            /* Pointer to the function which will be
             * executed in new thread. */
                            thread_function,
            /* Argument to be passed to the above
             * thread function. */
                            (void *) sock_fd);
    if (rc) {
        log_msg("SERVER: Failed to create thread.", false);
        return false;
    }
    return true;
}

/**
 * Sends a message to the server socket.
 * @param msg Message to send
 * @param socket_file Path of the server socket on localhost.
 */
_Noreturn void start_server_socket(char *socket_file, int max_connects) {
    int sock_fd = make_named_socket(socket_file, false);

    /* listen for clients, up to MaxConnects */
    if (listen(sock_fd, max_connects) < 0) {
        log_msg("Listen call on the socket failed. Terminating.", true); /* terminate */
    }
    log_msg("Listening for client connections...\n", false);
    /* Listens indefinitely */
    while (1) {
        struct sockaddr_in caddr; /* client address */
        int len = sizeof(caddr);  /* address length could change */

        printf("Waiting for incoming connections...\n");
        int client_fd = accept(sock_fd, (struct sockaddr *) &caddr, &len);  /* accept blocks */
        

        if (client_fd < 0) {
            log_msg("accept() failed. Continuing to next.", 0); /* don't terminate, though there's a problem */
            continue;
        }
        /* Start a worker thread to handle the received connection. */
        if (!create_worker_thread(client_fd)) {
            log_msg("Failed to create worker thread. Continuing to next.", 0);
            continue;
        }

    }  
}


int main() {
  
        start_server_socket("./my_sock", 10);           /* start the sever before kodev build and kodev run */
        return 0;
    
}

