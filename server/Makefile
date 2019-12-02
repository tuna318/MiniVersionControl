HELPER = ../helper
SOCKET_VALIDATE = socket_validate.c
MESSAGE_HANDLER = message_handler.c
FILE_HANDLER = file_handler.c
SERVER_RQ_HANDLER = server_request_handler.c

all:
	make server

server:
	gcc -o th-server th-server.c $(HELPER)/$(SERVER_RQ_HANDLER) $(HELPER)/$(SOCKET_VALIDATE) $(HELPER)/$(MESSAGE_HANDLER)
