HOME=/home/pi/open62541/
CC=gcc
CFLAGS1=-g -std=c99 -v
#CC=g++
#CFLAGS1=-x c++ -std=c++98 -v -fpermissive -Wno-literal-suffix -Wno-write-strings -Wno-long-long -Wno-return-type
CFLAGS2=-I$(HOME) -I$(HOME)include/ -I$(HOME)plugins/ -I$(HOME)src/ -I$(HOME)build/src_generated/ \
-I$(HOME)arch/ -I$(HOME)deps/ -I$(HOME)plugins/include/ -I/usr/local/include/
CFLAGS3=-g -pass-exit-codes
DEPS=
LIBS=-lm -lrt -lwebsockets -ljson-c \
-lmariadbclient -lpthread -lxml2 -lcrypto -lssl -lmbedtls -lmbedx509 -lmbedcrypto
OBJ=SV_Misc.o SV_ExtractXMLElementNames.o SV_Alarm.o SV_Event.o SV_Historizing.o \
SV_Monitor.o SV_Method.o SV_PopulateOPCUANodes.o SV_CreateOPCUANodes.o \
SV_StartOPCUAServer.o json_checker.o SV_PubSub.o SV_mainOPCUAServer.o

ODIR=obj
LDIR1=-L/usr/local/lib/
LDIR2=-L/usr/lib/
LDFLAGS=-L$(HOME)build/bin/ -l:libopen62541.a

all: myNewServer

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS1) $(CFLAGS2) $(CFLAGS3) -c  $< -o $@

myNewServer: $(OBJ)
	$(CC) $(LDIR1) $(LDIR2) -o $@ $^ $(LDFLAGS) $(LIBS)

#g++ -g -x c++ -std=c++98 -v -fpermissive -Wno-literal-suffix -Wno-write-strings -Wno-long-long -Wno-return-type -c ua_pubsub_amqp.c -o ua_pubsub_amqp.o
#LIBS=-lm -lrt -lmodbus -lqpid-proton-core -lqpid-proton-proactor -lqpid-proton -lwebsockets -ljson-c \
#LIBS=-lopen62541 -lm -lrt -lwebsockets -ljson-c \
#CFLAGS2=-I/home/pi/open62541/ -I/home/pi/open62541/include/ -I/home/pi/open62541/plugins/ -I/home/pi/open62541/src/ \
#-I/home/pi/open62541/build/src_generated/ -I/home/pi/open62541/src/ -I/home/pi/open62541/arch/ -I/home/pi/open62541/deps/ -I/home/pi/open62541/plugins/include/ -I/usr/local/include/

