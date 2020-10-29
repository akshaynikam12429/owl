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
    while((ch=getc(fp)!=EOF))
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

int main(int argc, char **argv) {
	FILE *fp;
	char buffer[1024];
    
	struct json_object *parsed_json;
	struct json_object *payload;

	fp = fopen("xmlOutput.json","r");
	fread(buffer, 1024, 1, fp);
	fclose(fp);

	parsed_json = json_tokener_parse(buffer);

	json_object_object_get_ex(parsed_json, "Payload", &payload);
	printf("Payload: %s\n", json_object_get_string(payload));
    return 0;
}