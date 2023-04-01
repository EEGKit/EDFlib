#
#
# Author: Teunis van Beelen
#
# email: teuniz@protonmail.com
#
# Linux only for the moment
#
# sudo cp -v libedf.so.1.2.4 /lib64/.
#
# sudo ldconfig
#
# https://tldp.org/HOWTO/Program-Library-HOWTO/shared-libraries.html
#
# nm -D libedf.so.1.2.4
#
# objdump -T libedf.so.1.2.4
#

CC = gcc
CFLAGS = -O2 -fpic -fvisibility=hidden -std=gnu11 -Wall -Wextra -Wshadow -Wformat-nonliteral -Wformat-security \
         -D_LARGEFILE64_SOURCE -D_LARGEFILE_SOURCE -DEDFLIB_SO_DLL -DEDFLIB_BUILD
LDLIBS = -lc
TARGET = libedf.so.1.2.4

objects = edflib.o

all: $(TARGET)

$(TARGET) : $(objects)
	$(CC) -shared -Wl,-soname,libedf.so.1 -o $(TARGET) edflib.o $(LDLIBS)
	strip --strip-unneeded $(TARGET)

edflib.o : ../edflib.h ../edflib.c
	$(CC) $(CFLAGS) -c ../edflib.c -o edflib.o

clean :
	$(RM) $(TARGET) $(objects)

#
#
#
#





