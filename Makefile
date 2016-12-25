## $Id: Makefile,v 1.2 2001/05/05 22:51:48 hpa Exp $
## -----------------------------------------------------------------------
##   
##   Copyright 2001 H. Peter Anvin - All Rights Reserved
##
##   This program is free software; you can redistribute it and/or modify
##   it under the terms of the GNU General Public License as published by
##   the Free Software Foundation, Inc., 675 Mass Ave, Cambridge MA 02139,
##   USA; either version 2 of the License, or (at your option) any later
##   version; incorporated herein by reference.
##
## -----------------------------------------------------------------------

PROGS = mmind

CC	= g++ 
CFLAGS	= -Wall -pedantic -DBUILDDATE="\"`date +%Y%m%d`\""
LDFLAGS = 
#LDFLAGS = -lncurses

all:	$(PROGS)

clean:
	rm -fv $(PROGS)
	rm -fv *~ \#* core

mmind: mastermind.cpp
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $< philipw.cpp
