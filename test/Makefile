HELPER = ../helper
SOCKET_VALIDATE = socket_validate.c
MESSAGE_HANDLER = message_handler.c
TRANSFER_HANDLER = transfer_handler.c
FILE_HANDLER = file_handler.c
SERVER_RQ_HANDLER = server_request_handler.c
ACCOUNT_HANDLER = account_handler.c 
MIGRATE_DB = migrate_db.c 

all:
	make compile

compile:
	gcc -o account_handler_test account_handler_test.c $(HELPER)/$(ACCOUNT_HANDLER) $(HELPER)/$(FILE_HANDLER) -lsqlite3 

test:
	./account_handler_test