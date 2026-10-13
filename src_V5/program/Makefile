#
# Makefile for all Unix V6++ programs
#

# 榛樿鏋勫缓鐩爣銆?
.DEFAULT_GOAL := all

#鍛戒护瀹氫箟
CC = gcc
LD = ld
OBJDUMP = objdump

#鍙傛暟瀹氫箟-nostartfiles
CFLAGS = -w -O0 -g -nostdlib -nostartfiles -nostdinc -fno-builtin -fno-exceptions

#鐩綍瀹氫箟
TARGET = ./objs
V6++LIB = "../lib/Lib_V6++.a"
INCLUDE = .
LIB_INCLUDE = ../lib/include

#MAKEIMAGEPATH = ../../tools/MakeImage/bin/Debug/programs
MAKEIMAGEPATH = ../../tools/v6pp-fs-edit-2022/workspace/programs
#DEMO = demo
BIN = bin

.PHONY : all build prepare clean

SHELL_OBJS =$(TARGET)/cat.exe	\
			$(TARGET)/cp.exe	\
			$(TARGET)/ls.exe	\
			$(TARGET)/mkdir.exe	\
			$(TARGET)/rm.exe	\
			$(TARGET)/perf.exe	\
			$(TARGET)/sig.exe \
			$(TARGET)/copyfile.exe	\
			$(TARGET)/shutdown.exe	\
			$(TARGET)/test.exe	\
			$(TARGET)/forks.exe	\
			$(TARGET)/trace.exe	\
			$(TARGET)/echo.exe	\
			$(TARGET)/date.exe \
			$(TARGET)/newsig.exe \
			$(TARGET)/sigTest.exe \
			$(TARGET)/stack.exe	\
			$(TARGET)/malloc.exe \
			$(TARGET)/testSTDOUT.exe \
			$(TARGET)/fork.exe
			
#$(TARGET)/performance.exe
			


build : prepare $(SHELL_OBJS)

prepare:
	rm -f $(TARGET)/*.exe
	rm -f $(MAKEIMAGEPATH)/$(BIN)/*

$(TARGET)/cat.exe :	cat.c
	$(CC) $(CFLAGS) -I"$(INCLUDE)" -I"$(LIB_INCLUDE)"  $< -e _main1 $(V6++LIB) -o $@
	cp -f $(TARGET)/cat.exe $(MAKEIMAGEPATH)/$(BIN)/cat
	
$(TARGET)/cp.exe :	cp.c
	$(CC) $(CFLAGS) -I"$(INCLUDE)" -I"$(LIB_INCLUDE)"  $< -e _main1 $(V6++LIB) -o $@
	cp -f $(TARGET)/cp.exe $(MAKEIMAGEPATH)/$(BIN)/cp
	
$(TARGET)/ls.exe :	ls.c
	$(CC) $(CFLAGS) -I"$(INCLUDE)" -I"$(LIB_INCLUDE)"  $< -e _main1 $(V6++LIB) -o $@
	cp -f $(TARGET)/ls.exe $(MAKEIMAGEPATH)/$(BIN)/ls
	
$(TARGET)/mkdir.exe :	mkdir.c
	$(CC) $(CFLAGS) -I"$(INCLUDE)" -I"$(LIB_INCLUDE)"  $< -e _main1 $(V6++LIB) -o $@
	cp -f $(TARGET)/mkdir.exe $(MAKEIMAGEPATH)/$(BIN)/mkdir
	
$(TARGET)/perf.exe :	performance.c
	$(CC) $(CFLAGS) -I"$(INCLUDE)" -I"$(LIB_INCLUDE)"  $< -e _main1 $(V6++LIB) -o $@
	cp -f $(TARGET)/perf.exe $(MAKEIMAGEPATH)/$(BIN)/perf
	
$(TARGET)/rm.exe :	rm.c
	$(CC) $(CFLAGS) -I"$(INCLUDE)" -I"$(LIB_INCLUDE)"  $< -e _main1 $(V6++LIB) -o $@
	cp -f $(TARGET)/rm.exe $(MAKEIMAGEPATH)/$(BIN)/rm
	
$(TARGET)/copyfile.exe :	copyfile.c
	$(CC) $(CFLAGS) -I"$(INCLUDE)" -I"$(LIB_INCLUDE)"  $< -e _main1 $(V6++LIB) -o $@
	cp -f $(TARGET)/copyfile.exe $(MAKEIMAGEPATH)/$(BIN)/copyfile
	
$(TARGET)/shutdown.exe :	shutdown.c
	$(CC) $(CFLAGS) -I"$(INCLUDE)" -I"$(LIB_INCLUDE)"  $< -e _main1 $(V6++LIB) -o $@
	cp -f $(TARGET)/shutdown.exe $(MAKEIMAGEPATH)/$(BIN)/shutdown

$(TARGET)/test.exe :	test.c
	$(CC) $(CFLAGS) -I"$(INCLUDE)" -I"$(LIB_INCLUDE)"  $< -e _main1 $(V6++LIB) -o $@
	cp -f $(TARGET)/test.exe $(MAKEIMAGEPATH)/$(BIN)/test
	
$(TARGET)/forks.exe :		forks.c
	$(CC) $(CFLAGS) -I"$(INCLUDE)" -I"$(LIB_INCLUDE)"  $< -e _main1 $(V6++LIB) -o $@
	cp -f $(TARGET)/forks.exe $(MAKEIMAGEPATH)/$(BIN)/forks
	
$(TARGET)/sig.exe :		sig.c
	$(CC) $(CFLAGS) -I"$(INCLUDE)" -I"$(LIB_INCLUDE)"  $< -e _main1 $(V6++LIB) -o $@
	cp -f $(TARGET)/sig.exe $(MAKEIMAGEPATH)/$(BIN)/sig

$(TARGET)/sigTest.exe :		sigTest.c
	$(CC) $(CFLAGS) -I"$(INCLUDE)" -I"$(LIB_INCLUDE)"  $< -e _main1 $(V6++LIB) -o $@
	cp -f $(TARGET)/sigTest.exe $(MAKEIMAGEPATH)/$(BIN)/sigTest
	
$(TARGET)/trace.exe :		trace.c
	$(CC) $(CFLAGS) -I"$(INCLUDE)" -I"$(LIB_INCLUDE)"  $< -e _main1 $(V6++LIB) -o $@
	cp -f $(TARGET)/trace.exe $(MAKEIMAGEPATH)/$(BIN)/trace
	
$(TARGET)/echo.exe :		echo.c
	$(CC) $(CFLAGS) -I"$(INCLUDE)" -I"$(LIB_INCLUDE)"  $< -e _main1 $(V6++LIB) -o $@
	cp -f $(TARGET)/echo.exe $(MAKEIMAGEPATH)/$(BIN)/echo
	
$(TARGET)/date.exe :		date.c
	$(CC) $(CFLAGS) -I"$(INCLUDE)" -I"$(LIB_INCLUDE)"  $< -e _main1 $(V6++LIB) -o $@
	cp -f $(TARGET)/date.exe $(MAKEIMAGEPATH)/$(BIN)/date
	
$(TARGET)/newsig.exe :		newsig.c
	$(CC) $(CFLAGS) -I"$(INCLUDE)" -I"$(LIB_INCLUDE)"  $< -e _main1 $(V6++LIB) -o $@
	cp -f $(TARGET)/newsig.exe $(MAKEIMAGEPATH)/$(BIN)/newsig
	
$(TARGET)/stack.exe :		stack.c
	$(CC) $(CFLAGS) -I"$(INCLUDE)" -I"$(LIB_INCLUDE)"  $< -e _main1 $(V6++LIB) -o $@
	cp -f $(TARGET)/stack.exe $(MAKEIMAGEPATH)/$(BIN)/stack
	
$(TARGET)/malloc.exe :		malloc.c
	$(CC) $(CFLAGS) -I"$(INCLUDE)" -I"$(LIB_INCLUDE)"  $< -e _main1 $(V6++LIB) -o $@
	cp -f $(TARGET)/malloc.exe $(MAKEIMAGEPATH)/$(BIN)/malloc

$(TARGET)/fork.exe :		fork.c
	$(CC) $(CFLAGS) -I"$(INCLUDE)" -I"$(LIB_INCLUDE)"  $< -e _main1 $(V6++LIB) -o $@
	cp -f $(TARGET)/fork.exe $(MAKEIMAGEPATH)/$(BIN)/fork

$(TARGET)/testSTDOUT.exe :	testSTDOUT.c
	$(CC) $(CFLAGS) -I"$(INCLUDE)" -I"$(LIB_INCLUDE)"  $< -e _main1 $(V6++LIB) -o $@
	cp -f $(TARGET)/testSTDOUT.exe $(MAKEIMAGEPATH)/$(BIN)/testSTDOUT

clean:
	rm -f $(TARGET)/*.exe
	rm -f $(MAKEIMAGEPATH)/$(BIN)/*
	
#all	:	build
all	:	build
	
