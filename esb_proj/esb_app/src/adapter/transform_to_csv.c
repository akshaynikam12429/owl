#include<stdio.h>
#include<stdlib.h>
#include<string.h>

char * transformCSV(char *s){

printf("\n Creating payload_data.csv file");

FILE *fp;


//filename=strcat(filename,".csv");

fp=fopen("payload_data.csv","w+");

fprintf(fp,"Payload\n");
fprintf(fp,"%s\n",s);



fclose(fp);
/*
    FILE * fl,*f;
    fp=fopen("payload_data.csv","r");
    int c;
    int i=0;
    while((c=fgetc(fl))!= EOF){    //determine the size of buffer 
	i++;              
    }
    fclose(fl); //close the local file
    f=fopen("payload_data.csv","r");
    char buff[i];
    int j=0;
    while((c=fgetc(fp))!= EOF){
	buff[j]=c;  
	j++;
    }
    char * const str=buff;  //storing the content of the file in the string
    fclose(f);*/

printf("\n payload_data.csv file created\n");
return "payload_data.csv";
}




/*
int main(){
           
     // char *s=(char*)malloc(50*sizeof(char));
     // char *str=(char*)malloc(50*sizeof(char));
      
    char * s="Hello";
    
   char * filename=to_csv(s);
   printf("filename is %s\n",filename);

return 0;

}
*/
