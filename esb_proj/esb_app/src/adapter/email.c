#include<stdio.h>
#include<string.h>


void  emailsender(char *to,  char *text)
{
     char cmd[1000];  // to hold the command.
     int len = strlen(text);
        char tempFile[len];     // name of tempfile.

        //strcpy(tempFile,tempnam("/tmp","sendmail")); // generate temp file name.

        FILE *fp = fopen(tmpfile,"w"); // open it for writing.
        fprintf(fp,"From: Team Owl\r\r\n");
        fprintf(fp,"Subject:Payload\r\n");  
         fprintf(fp,"\r\n");  
        fprintf(fp,"%s\n",text);        // write body to it.
        fclose(fp);             // close it.
         printf("received=%s\n",text);
        sprintf(cmd,"ssmtp %s < %s",to,tmpfile); // prepare command.
         system(cmd);     // execute it.
        
}


