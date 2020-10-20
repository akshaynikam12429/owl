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




 // Print the output of BMD file
 void print(BMD *bmd)
 {
 	printf("%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n",
     
    bmd->bmd_envelope->MessageID,
    bmd->bmd_envelope->MessageType,
    bmd->bmd_envelope->Sender,
    bmd->bmd_envelope->Destination,
    bmd->bmd_envelope->CreationDateTime,
    bmd->bmd_envelope->Signature,
    bmd->bmd_envelope->ReferenceID,
    bmd->bmd_payload->data
     
     );
 }

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
  
        char* arrayValue[9];
        int i=-0;
        first_child = root->children;
        for (node = first_child; node; node = node->next) {
           
            if(node->type==1)
            {
                arrayValue[i]=(char*)xmlNodeGetContent(node);
                i++;
            }
        }

    root=xmlLastElementChild(temp);
    first_child = root;
        for (node = first_child; node; node = node->next) {
           
 
            if(node->type==1)
            {
              arrayValue[i]=(char*)xmlNodeGetContent(node);
                i++;
            }
        }
     }
    
    
    bmd->bmd_envelope->MessageID=(unsigned char*)arrayValue[0];
    bmd->bmd_envelope->MessageType=(unsigned char*)arrayValue[1];
    bmd->bmd_envelope->Sender=(unsigned char*)arrayValue[2];
    bmd->bmd_envelope->Destination=(unsigned char*)arrayValue[3];
    bmd->bmd_envelope->CreationDateTime=(unsigned char*)arrayValue[4];
    bmd->bmd_envelope->Signature=(unsigned char*)arrayValue[5];
    bmd->bmd_envelope->ReferenceID=(unsigned char*)arrayValue[6];
    bmd->bmd_payload->data=(unsigned char*)arrayValue[8];
    printf("\n\n%s is mesg id\n", arrayValue[0]);
    return(bmd);
}

int _main()
 {
  	printf("In the main\n");
    BMD *bmd = (BMD *)(malloc(sizeof(BMD *)));
    
    bmd=processXML("/home/mukesh/nho2020/esb_app/bmd.xml");

    print(bmd);
 }
