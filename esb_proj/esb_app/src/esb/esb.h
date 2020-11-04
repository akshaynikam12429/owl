#ifndef ESB_H
#define ESB_H



/*typedef struct {
    char* sender_id;
    char* destination_id;
    char* message_type;
    // TODO: Other fields
} bmd_envelop;

typedef struct {
    bmd_envelop envelop;
    char* payload;
} bmd;
*/
typedef struct {
    const unsigned char * key;
    void * value;
}
pair;

typedef struct {
    const unsigned char * MessageID;
    const unsigned char * MessageType;
    const unsigned char * Sender;
    const unsigned char * Destination;
    const unsigned char * CreationDateTime;
    const unsigned char * Signature;
    pair * UserProperties[50];
    const unsigned char * ReferenceID;

}
envelope;

typedef struct {
    const unsigned char * data;
}
payload;

typedef struct {
    envelope * bmd_envelope;
    payload * bmd_payload;
}
BMD;

int process_esb_request(char* bmd_file_path);
BMD * parse_bmd_xml(char * );
BMD * process_xml(char * );
int check_if_string_is_guid(const unsigned char * value);
envelope * extract_envelop(char * bmd_xml);
payload * extract_payload(char * bmd_xml);


#define INSERT_IN_ESB_REQUEST "INSERT INTO esb_request(sender_id,dest_id,message_type,reference_id,message_id,received_on,status) VALUES ('%s','%s','%s','%s','%s','%s','%s');"

#endif