#include<stdio.h>
#include "../parsing/bmd_parser.h"
#include "email.h"
#include "esb.h"
#include "../dataBase/database.h"

/**
 * TODO: This is to be implemented separately.
 */
BMD * parse_bmd_xml(char* bmd_file_path) {
    BMD * bmd;
    //BMD *bmd = (BMD *)(malloc(sizeof(BMD *)));
    bmd=processXML(bmd_file_path);
    return bmd;
}

 int is_bmd_valid(BMD * bmd)
{
    int valid = 1; // 1 => vaild, -1 => invalid
    // TODO: Implement the validation logic here

    return valid;
}

int queue_the_request(BMD * bmd)
{
    int success = 1; // 1 => OK, -1 => Error cases
    /** 
     * TODO: Insert the envelop data into esb_requests table,
     * and implement other logic for enqueueing the request
     * as specified in Theory of Operation.
     */
    success = insert_in_esb_request(bmd);

    return success;
}

/**
 * This is the main entry point into the ESB. 
 * It will start processing of a BMD received at the HTTP endpoint.
 */
 int process_esb_request(char* bmd_file_path) {
    int status = 1; // 1 => OK, -ve => Errors
    printf("Handling the BMD %s\n", bmd_file_path);
    /** TODO: 
     * Perform the steps outlined in the Theory of Operation section of
     * the ESB specs document. Each major step should be implemented in
     * a separate module. Suitable unit tests should be created for all
     * the modules, including this one.
     */
    // Step 1:
   BMD * bmd;
   bmd = parse_bmd_xml(bmd_file_path);

   /* // Step 2:
   if (!is_bmd_valid(bmd))
    {
        //TODO: Process the error case
        printf("BMD is invalid!\n");
        status = -2;
    }
    else
    {
        // Step 3:
        status = queue_the_request(bmd);
    }*/
    //status = queue_the_request(bmd);
    return status;
}