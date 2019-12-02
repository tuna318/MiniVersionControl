HELPER = ../helper
SOCKET_VALIDATE = socket_validate.c
MESSAGE_HANDLER = message_handler.c

all:
	make th-cmd

th-cmd:
	gcc -o th-app th-app.c $(HELPER)/$(SOCKET_VALIDATE) $(HELPER)/$(MESSAGE_HANDLER)
