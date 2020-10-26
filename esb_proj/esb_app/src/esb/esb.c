#include<stdio.h>
#include "email.h"
#include "esb.h"
#include<stdlib.h>
#include<mysql/mysql.h>
#include <string.h>
#include <libxml/parser.h>
#include <libxml/tree.h>

static char *server = "localhost";
static char *user = "root";
static char *password = "my sql";
static char *database = "esb";

unsigned int port = 3306;
static char *unix_socket = NULL;

unsigned int flag = 0;


static envelope *get_envelope_struct()
{
    envelope *bmd_envelope = (envelope *)malloc(sizeof(envelope));
    return bmd_envelope;
}

static payload *get_payload_struct()
{
    payload *bmd_payload = (payload *)malloc(sizeof(payload));
    return bmd_payload;
}


/**
 * TODO: This is to be implemented separately.
 */
BMD * parse_bmd_xml(char* bmd_file_path) {
    xmlDoc         *document;
    xmlNode        *root, *first_child, *node,*temp;
    char           *filename;
    
    BMD *bmd = (BMD *)(malloc(sizeof(BMD *)));

    bmd->bmd_envelope = get_envelope_struct();
    bmd->bmd_payload = get_payload_struct();




    filename = bmd_file_path;
    
    document = xmlReadFile(filename, NULL, 0);
    temp = xmlDocGetRootElement(document);
    root=xmlFirstElementChild(temp);
  
        char* arrayName[9];
        char* arrayValue[9];
        int i=0;
        first_child = root->children;
        for (node = first_child; node; node = node->next) {

            if(node->type==1)
            {         
                arrayName[i]=(char*)node->name;
                arrayValue[i]=(char*)xmlNodeGetContent(node);
                i++;
            }
        }

    root=xmlLastElementChild(temp);
    first_child = root;
        for (node = first_child; node; node = node->next) {
            if(node->type==1)
            {            
                arrayName[i]=(char*)node->name;            
                arrayValue[i]=(char*)xmlNodeGetContent(node);
                i++;
            }
        }
   
    for(int x=0; x<9; x++){
        if(strcmp(arrayName[x], "MessageID")==0)
            bmd->bmd_envelope->MessageID=(unsigned char*)arrayValue[x];
        if(strcmp(arrayName[x], "MessageType")==0)
            bmd->bmd_envelope->MessageType=(unsigned char*)arrayValue[x];
        if(strcmp(arrayName[x], "Sender")==0)   
            bmd->bmd_envelope->Sender=(unsigned char*)arrayValue[x];
         if(strcmp(arrayName[x], "Destination")==0) 
            bmd->bmd_envelope->Destination=(unsigned char*)arrayValue[x];
        if(strcmp(arrayName[x], "CreationDateTime")==0)
            bmd->bmd_envelope->CreationDateTime=(unsigned char*)arrayValue[x];
        if(strcmp(arrayName[x], "Signature")==0)
            bmd->bmd_envelope->Signature=(unsigned char*)arrayValue[x];
        if(strcmp(arrayName[x], "ReferenceID")==0)    
            bmd->bmd_envelope->ReferenceID=(unsigned char*)arrayValue[x];
    //bmd->bmd_envelope->UserProperties=(unsigned char*)arrayValue[7];
        if(strcmp(arrayName[x], "Payload")==0) 
            bmd->bmd_payload->data=(unsigned char*)arrayValue[x];
    }

    printf("%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n",
     
    bmd->bmd_envelope->MessageID,
    bmd->bmd_envelope->MessageType,
    bmd->bmd_envelope->Sender,
    bmd->bmd_envelope->Destination,
    bmd->bmd_envelope->CreationDateTime,
    bmd->bmd_envelope->Signature,
    bmd->bmd_envelope->ReferenceID,
    //bmd->bmd_envelope->UserProperties,
    bmd->bmd_payload->data
     
     );
    return bmd;
}

 int is_bmd_valid(BMD * bmd)
{
    int valid = 1; // 1 => vaild, -1 => invalid
    // TODO: Implement the validation logic here
     
      if(*(bmd->bmd_envelope->MessageID)==0)
    {
        printf("MessageId is mandatory (missing in BMD file)\n");
        valid = 0;
    }
     if(*(bmd->bmd_envelope->MessageType)==0)
    {
        printf("MessageType is mandatory (missing in BMD file)\n");
        valid = 0;

    } if(*(bmd->bmd_envelope->Sender)==0)
    {
        printf("Sender Id is mandatory (missing in BMD file)\n");
        valid = 0;

    } if(*(bmd->bmd_envelope->Destination)== 0)
    {      
        printf("Destination Id is mandatory (missing in BMD file)\n");
        valid = 0;

    } if(*(bmd->bmd_envelope->CreationDateTime)== 0)
    {
        printf("CreationDateTime is mandatory (missing in BMD file)\n");
        valid = 0;

    } if(*(bmd->bmd_envelope->Signature)== 0)
    {
        printf("Signature is mandatory (missing in BMD file)\n");
        valid = 0;

    } if(*(bmd->bmd_envelope->ReferenceID)== 0)
    {
        printf("ReferenceID is mandatory (missing in BMD file)\n");
        valid = 0;
    }

    return valid;
}

int queue_the_request(BMD * bmd)
{
    int success = 1; // 1 => OK, -1 => Error cases
    /** 
     * TODO: Insert the envelop data into esb_requests table,
     * and implement other logic for enqueueing the request
     * as specified in Theory of Operation.
     */

    MYSQL * conn;
    MYSQL_RES * res;
    MYSQL_ROW row;

    int8_t received_time[100]; // eg: "2020-08-12T05:18:00+0000";
    int8_t received_on[100];
    strcpy(received_time, bmd -> bmd_envelope -> CreationDateTime);

    /*Changing DateTime format*/
    int n = strlen(received_time);
    int j = 0;
    for (int i = 0; i < n-5 ; i++) {
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

    //success = insert_in_esb_request(bmd);

    return success;
}

/**
 * This is the main entry point into the ESB. 
 * It will start processing of a BMD received at the HTTP endpoint.
 */
 int process_esb_request(char* bmd_file_path) {
    int status = 1; // 1 => OK, -ve => Errors
    printf("Handling the BMD %s\n", bmd_file_path);
    /** TODO: 
     * Perform the steps outlined in the Theory of Operation section of
     * the ESB specs document. Each major step should be implemented in
     * a separate module. Suitable unit tests should be created for all
     * the modules, including this one.
     */
    // Step 1:
   BMD * bmd;
   bmd = parse_bmd_xml(bmd_file_path);

   // Step 2:
   if (!is_bmd_valid(bmd))
    {
        //TODO: Process the error case
        printf("BMD is invalid!\n");
        status = -2;
    }
    else
    {
        // Step 3:
        status = queue_the_request(bmd);
    }
    
    return status;
}
