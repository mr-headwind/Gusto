
#  Copyright (C) 2023 Anthony Buckley
# 
#  This file is part of Gusto.
# 
#  Gusto is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
# 
#  Gusto is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
# 
#  You should have received a copy of the GNU General Public License
#  along with Gusto.  If not, see <http://www.gnu.org/licenses/>.

CC=cc
CFLAGS=-I. `pkg-config --cflags gtk+-3.0 gstreamer-1.0` 
CFLAGS2=-Wno-deprecated-declarations
DEPS = defs.h main.h user_data.h version.h
OBJ = gusto.o callbacks.o main_ui.o utility.o convert.o css.o
LIBS = `pkg-config --libs gtk+-3.0 gstreamer-1.0 gstreamer-video-1.0 gstreamer-pbutils-1.0 libpng`
#LIBS2 = -ljpeg -lpthread -lmsvcrt
LIBS2 = -ljpeg -lpthread -lc

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS) $(CFLAGS2)

Gusto: $(OBJ)
	$(CC) -o $@ $^ $(LIBS) $(LIBS2)

clean:
	rm -f $(OBJ)
