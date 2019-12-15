HELPER = ../helper
SOCKET_VALIDATE = socket_validate.c
MESSAGE_HANDLER = message_handler.c
FILE_HANDLER = file_handler.c
TRANSFER_HANDLER = transfer_handler.c

all:
	make th-cmd

th-cmd:
	gcc -o th-app th-app.c $(HELPER)/$(SOCKET_VALIDATE) $(HELPER)/$(MESSAGE_HANDLER) $(HELPER)/$(FILE_HANDLER)
	gcc -o th-init th-init.c $(HELPER)/$(FILE_HANDLER)
	gcc -o th-commit th-commit.c $(HELPER)/$(FILE_HANDLER)
	gcc -o th-log th-log.c $(HELPER)/$(FILE_HANDLER)
	gcc -o th-push th-push.c $(HELPER)/$(TRANSFER_HANDLER) $(HELPER)/$(MESSAGE_HANDLER) $(HELPER)/$(FILE_HANDLER)  $(HELPER)/$(SOCKET_VALIDATE)
	gcc -o th-pull th-pull.c $(HELPER)/$(TRANSFER_HANDLER) $(HELPER)/$(MESSAGE_HANDLER) $(HELPER)/$(FILE_HANDLER)  $(HELPER)/$(SOCKET_VALIDATE)