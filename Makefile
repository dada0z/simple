CROSS_COMPILE :=
CC := $(CROSS_COMPILE)gcc
CFLAGS := -Wall -Werror -pedantic

INCLUDE_PATH = include
SOURCE_PATH = source
OBJ_PATH = obj
SOURCE = $(wildcard $(SOURCE_PATH)/*.c)
OBJS = $(patsubst %.c,$(OBJ_PATH)/%.o,$(notdir $(SOURCE)))

TARGET = simple

all:$(TARGET)

$(TARGET):$(OBJS)
	$(CC) $(CFLAGS) -I$(INCLUDE_PATH) -o $@ $^
$(OBJ_PATH)/%.o:$(SOURCE_PATH)/%.c
	if [ ! -d $(OBJ_PATH) ]; then mkdir -p $(OBJ_PATH); fi;\
	$(CC) $(CFLAGS) -I$(INCLUDE_PATH) -c $< -o $@
test:
	gcc -g -I$(INCLUDE_PATH) $(filter-out $(SOURCE_PATH)/main.c, $(SOURCE)) $(wildcard tests/*.c) -o AllTests
.PHONY:clean

clean:
	rm -rf $(TARGET) $(OBJ_PATH) AllTests