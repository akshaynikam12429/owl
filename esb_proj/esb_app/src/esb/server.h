#ifndef SERVER_H
#define SERVER_H

#include "esb.h"

int make_named_socket(const char *socket_file, bool is_client);
void log_msg(const char *msg, bool terminate);

#endif