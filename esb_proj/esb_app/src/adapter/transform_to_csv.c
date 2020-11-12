#include<stdio.h>
#include<stdlib.h>
#include<string.h>




/* function to save data in CSV file*/
char * transformCSV(char *s){
    printf("\n Creating payload_data.csv file");

    FILE *fp;                               //declaring the file pointer

    fp=fopen("payload_data.csv","w+");          //creating a new file and writing in it
    fprintf(fp,"Payload\n");
    fprintf(fp,"%s\n",s);
    fclose(fp);                   //closing the local file
    printf("\n payload_data.csv file created\n");
        
    return "payload_data.csv";             //returning the filename
}




