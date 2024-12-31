CC := gcc
CFLAGS := -Wall
LDFLAGS := 
DEPENDFLAGS := -MMD -MP
OBJS_DIR := objs
FILES_DIR := src
SOURCEFILES := $(shell find $(FILES_DIR) -name '*.c')
OBJECTFILES := $(patsubst %.c, %.o, $(addprefix $(OBJS_DIR)/, $(SOURCEFILES)))
TARGET := ejemplo
EXISTO_OBJS := $(shell find $(OBJS_DIR) -name '*.o')

all : $(TARGET)

$(TARGET) : $(OBJECTFILES)
	@echo El programa $(TARGET) esta siendo construido
	@echo usando los headers $(patsubst %.c, %.h, $(SOURCEFILES))
	$(CC) $(CFLAGS) $(OBJECTFILES) -o $(TARGET) $(LDFLAGS)

$(OBJS_DIR)/%.o : %.c
	mkdir -p $(dir $@)
	$(CC) -c $(CFLAGS) $(LDFLAGS) $< -o $@ $(DEPENDFLAGS)

-include $(OBJECTFILES:.o=.d)

.PHONY : clean

clean : 
	rm -f ${EXIST_OBJS} ${TARGET} $(patsubst %.o, %.d, ${EXIST_OBJS})

