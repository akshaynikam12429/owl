#include <stdio.h>
#include <string.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include "bmd_parser.h"

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




 
 /*void print(BMD *bmd)
 {
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
 }*/

BMD* processXML(char* nameXML)
{
    xmlDoc         *document;
    xmlNode        *root, *first_child, *node,*temp;
    char           *filename;
    
    BMD *bmd = (BMD *)(malloc(sizeof(BMD *)));

    bmd->bmd_envelope = get_envelope_struct();
    bmd->bmd_payload = get_payload_struct();




    filename = nameXML;
    
    document = xmlReadFile(filename, NULL, 0);
    temp = xmlDocGetRootElement(document);
    root=xmlFirstElementChild(temp);
  
        //char* arrayName[8];
        char* arrayValue[9];
        int i=-0;
        first_child = root->children;
        for (node = first_child; node; node = node->next) {
            // if(node->type==1)
            // fprintf(stdout, "\t Child is <%s>  (%i) value: \n", node->name ,node->type);
            // // temp=node;

            if(node->type==1)
            {
            // printf("%d\n",checkNamespace("MessageID",node->name));
                //arrayName[i]=(char*)node->name;
                arrayValue[i]=(char*)xmlNodeGetContent(node);
                //fprintf(stdout, "attributes:<%s>\tvalue: %s\n", node->name,xmlNodeGetContent(node));
                //printf("%d: name->%s value->%s\n",i,arrayName[i],arrayValue[i]);
                i++;
            }
        }

    root=xmlLastElementChild(temp);
    first_child = root;
        for (node = first_child; node; node = node->next) {
            // if(node->type==1)
            // fprintf(stdout, "\t Child is <%s>  (%i) value: \n", node->name ,node->type);
            // // temp=node;

            if(node->type==1)
            {
            // printf("%d\n",checkNamespace("MessageID",node->name));
                //arrayName[i]=(char*)node->name;
                arrayValue[i]=(char*)xmlNodeGetContent(node);
                //fprintf(stdout, "attributes:<%s>\tvalue: %s\n", node->name,xmlNodeGetContent(node));
                //printf("%d: name->%s value->%s\n",i,arrayName[i],arrayValue[i]);
                i++;
            }
        }
    // }
    //int size = sizeof arrayName / sizeof arrayName[0];
    // for(int i=0;i<=size;i++)
    // {
    //     printf("%d: name->%s value->%s\n",i,arrayName[i],arrayValue[i]);
    // } 
    
    
    bmd->bmd_envelope->MessageID=(unsigned char*)arrayValue[0];
    bmd->bmd_envelope->MessageType=(unsigned char*)arrayValue[1];
    bmd->bmd_envelope->Sender=(unsigned char*)arrayValue[2];
    bmd->bmd_envelope->Destination=(unsigned char*)arrayValue[3];
    bmd->bmd_envelope->CreationDateTime=(unsigned char*)arrayValue[4];
    bmd->bmd_envelope->Signature=(unsigned char*)arrayValue[5];
    bmd->bmd_envelope->ReferenceID=(unsigned char*)arrayValue[6];
    //bmd->bmd_envelope->UserProperties=(unsigned char*)arrayValue[7];
    bmd->bmd_payload->data=(unsigned char*)arrayValue[8];
    //printf("\n\n%s is mesg id\n", arrayValue[0]);
    //xml2json(bmd->bmd_payload->data);
    //print(bmd);
    return(bmd);
}

/*int ___main()
 {
     //int dbcon=0;
	printf("In the main\n");
    BMD *bmd = (BMD *)(malloc(sizeof(BMD *)));
    
    bmd=processXML("/home/amruth/work/nho2020/bmd.xml");
	//dbcon = insert_in_esb_request(bmd);
    
    //printf("\n%d is the db status\n",dbcon);

    //print(bmd);
 }*/