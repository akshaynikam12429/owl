#include<stdio.h>
#include<string.h>


void  emailsender(char *to,  char *text)
{
     char cmd[100];  // to hold the command.
     int len = strlen(text);
        char tempFile[len];     // name of tempfile.

        strcpy(tempFile,tempnam("/tmp","sendmail")); // generate temp file name.

        FILE *fp = fopen(tempFile,"w"); // open it for writing.
        
        fprintf(fp,"Subject:payload\n");  
        fprintf(fp,"From:TeamOwl\r\n");      // write body to it.
        //fclose(fp);

        //FILE *fp = fopen(tempFile,"w"); // open it for writing.
        fprintf(fp,"%s\n",text);        // write body to it.
        fclose(fp);             // close it.
         printf("received=%s\n",text);
        sprintf(cmd,"ssmtp %s < %s",to,tempFile); // prepare command.
         system(cmd);     // execute it.
        

}