#
# Makefile for Unix V6++ Mini Clib.
#

#鍛戒护瀹氫箟
CC = gcc
LD = ld

#鍙傛暟瀹氫箟
CFLAGS = -w -O0 -g -nostdlib -nostdinc -nostartfiles -fno-builtin -fno-exceptions -I"$(SRC)"

#鐩綍瀹氫箟
TARGET = ./objs
INCLUDE = ./include
SRC = ./src

.PHONY : all

LIB_OBJS =	$(TARGET)/file.o	\
			$(TARGET)/sys.o		\
			$(TARGET)/stdio.o	\
			$(TARGET)/stdlib.o	\
			$(TARGET)/string.o	\
			$(TARGET)/double.o	\
			$(TARGET)/float.o	\
			$(TARGET)/sprintf.o	\
			$(TARGET)/print_parse.o	\
			$(TARGET)/time.o \
			$(TARGET)/malloc.o

			
			
all	:	$(LIB_OBJS)
	ar -crv Lib_V6++.a $(LIB_OBJS)
	
	
$(TARGET)/file.o :	$(SRC)/file.c $(INCLUDE)/file.h
	$(CC) $(CFLAGS) -I"$(INCLUDE)" -c $< -o $@
	
$(TARGET)/sys.o	:	$(SRC)/sys.c $(INCLUDE)/sys.h
	$(CC) $(CFLAGS) -I"$(INCLUDE)" -c $< -o $@
	
$(TARGET)/stdio.o	:	$(SRC)/stdio.c $(INCLUDE)/stdio.h
	$(CC) $(CFLAGS) -I"$(INCLUDE)" -c $< -o $@
	
$(TARGET)/stdlib.o	:	$(SRC)/stdlib.c $(INCLUDE)/stdlib.h
	$(CC) $(CFLAGS) -I"$(INCLUDE)" -c $< -o $@
	
$(TARGET)/string.o	:	$(SRC)/string.c $(INCLUDE)/string.h
	$(CC) $(CFLAGS) -I"$(INCLUDE)" -c $< -o $@
	
$(TARGET)/double.o	:	$(SRC)/double.c $(SRC)/double.h
	$(CC) $(CFLAGS) -I"$(INCLUDE)" -c $< -o $@
	
$(TARGET)/float.o	:	$(SRC)/float.c $(SRC)/float.h
	$(CC) $(CFLAGS) -I"$(INCLUDE)" -c $< -o $@

$(TARGET)/sprintf.o	:	$(SRC)/sprintf.c
	$(CC) $(CFLAGS) -I"$(INCLUDE)" -c $< -o $@
	
$(TARGET)/print_parse.o	:	$(SRC)/print_parse.c $(SRC)/print_parse.h
	$(CC) $(CFLAGS) -I"$(INCLUDE)" -c $< -o $@
	
$(TARGET)/time.o	:	$(SRC)/time.c $(INCLUDE)/time.h
	$(CC) $(CFLAGS) -I"$(INCLUDE)" -c $< -o $@	

$(TARGET)/malloc.o	:	$(SRC)/malloc.c $(INCLUDE)/malloc.h
	$(CC) $(CFLAGS) -I"$(INCLUDE)" -c $< -o $@	
	
clean:
	rm -f $(TARGET)/*.o
	rm -f Lib_V6++.a
	
	
#榛樿缂栬瘧瑙勫垯
.c.s:
	$(CC) $(CFLAGS) \
	-S -o $*.s $<
.s.o:
	$(AS) -o $*.o $<
.c.o:
	$(CC) $(CFLAGS) \
	-c -o $*.o $<
