/*#include<stdio.h>
#include<stdlib.h>
#include<string.h>



int main()
{
    int i=0;
    char str[50],ch;
    FILE * fp;
    fp=fopen("xmlOutput.json","r");
   if(fp==NULL)
   {
       printf("File open error\n");
   }
    while((ch=fgetc(fp)!=EOF))
    {
        printf("%c",ch);
        str[i++]=ch;
        
    }
    fclose(fp);
    printf("%s\n",str);
    



    return 0;
}*/


#include<stdio.h>
#include<json-c/json.h>
#include "js.h"

char *  getstr(void) {
	FILE *fp;
	char ch;
    int i=0;
    
	struct json_object *parsed_json;
	struct json_object *payload;
    char * info;


    fp = fopen("xmlOutput.json","r");
	//fread(buffer, 1024, 1, fp);
    while(ch=(fgetc(fp)!=EOF))
    {
        i++;
    }
	fclose(fp);

    char buffer[i];
    fp = fopen("xmlOutput.json","r");
	fread(buffer, i, 1, fp);
	fclose(fp);

	parsed_json = json_tokener_parse(buffer);

	json_object_object_get_ex(parsed_json, "Payload", &payload);
    info = json_object_get_string(payload);
	printf("In String format: %s\n", info);
    return info;
}