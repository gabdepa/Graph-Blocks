COMMON_FLAGS = -pipe \
	       -ggdb3 -Wstrict-overflow=5 -fstack-protector-all \
	       -W -Wall -Wextra \
	       -Wcast-align \
	       -Wcast-qual \
	       -Wconversion \
	       -Wfloat-equal \
	       -Wformat-y2k \
	       -Winit-self \
	       -Winline \
	       -Winvalid-pch \
	       -Wmissing-declarations \
	       -Wmissing-field-initializers \
	       -Wmissing-format-attribute \
	       -Wmissing-include-dirs \
	       -Wmissing-noreturn \
	       -Wnormalized=nfc \
	       -Woverlength-strings \
	       -Wpacked \
	       -Wpointer-arith \
	       -Wredundant-decls \
	       -Wshadow \
	       -Wsign-compare \
	       -Wstack-protector \
	       -Wstrict-aliasing=2 \
	       -Wundef \
	       -Wunsafe-loop-optimizations \
	       -Wvolatile-register-var

CFLAGS  = $(COMMON_FLAGS) \
          -std=c99 \
	  -Wbad-function-cast \
	  -Wmissing-prototypes \
	  -Wnested-externs \
	  -Wold-style-definition \
	  -Wstrict-prototypes \
	  -Wwrite-strings

CPPFLAGS = $(COMMON_FLAGS)

# Target names
TARGET = findBlocks

# Source files
SOURCES = findBlocks.c

# Object files
OBJECTS = $(SOURCES:.c=.o)

# Dependency files
DEPS = findBlocks.h

#------------------------------------------------------------------------------
.PHONY : all clean

#------------------------------------------------------------------------------
all : $(TARGET)

# Compile .c files into .o files
%.o : %.c $(DEPS)
	$(CC) -c $(CFLAGS) -o $@ $<

# Link object files into the final executable
$(TARGET) : $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^
	$(RM) $(OBJECTS)

#------------------------------------------------------------------------------
clean :
	$(RM) $(TARGET) $(OBJECTS)
