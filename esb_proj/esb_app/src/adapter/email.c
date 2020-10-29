#include<stdio.h>

void send(char *to, char *text) {
    printf("Sending to %s\n", to);
    /*char cmd[100];  // to hold the command.
      
        char tempFile[100];     // name of tempfile.

        strcpy(tempFile,tempnam("/tmp","sendmail")); // generate temp file name.

        FILE *fp = fopen(tempFile,"w"); // open it for writing.
        fprintf(fp,"%s\n",text);        // write body to it.
        fclose(fp);             // close it.

        sprintf(cmd,"ssmtp %s < %s",to,tempFile); // prepare command.
        system(cmd);     // execute it.*/
}
