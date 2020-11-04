#ifndef SERVER_H
#define SERVER_H

#include "esb.h"
#include<stdbool.h>

extern int process_esb_request(char* bmd_file_path);
int make_named_socket(const char *socket_file, bool is_client);
void log_msg(const char *msg, bool terminate);

#endif