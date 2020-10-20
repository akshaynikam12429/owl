#ifndef bmd_parser
#define bmd_parser

typedef struct {
    const unsigned char * key;
    void * value;
}
pair;

//structure for envelope
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

//structure for payload
typedef struct {
    const unsigned char * data;
}
payload;

//structure for BMD
typedef struct {
    envelope * bmd_envelope;
    payload * bmd_payload;
}
BMD;

BMD * parse_bmd_xml(char * );
BMD * process_xml(char * );
int check_if_string_is_guid(const unsigned char * value);
envelope * extract_envelop(char * bmd_xml);
payload * extract_payload(char * bmd_xml);

#endif /* bmd_parser.h */
