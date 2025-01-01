CC := gcc
AR := ar
AR_ARGS = rcs
CFLAGS := -Wall
lib_name := cutils_lib
lib_dir  := ../src/libs
LDFLAGS := 
DEPENDFLAGS := -MMD -MP
OBJS_DIR := objs
FILES_DIR := src
SOURCEFILES := $(shell find $(FILES_DIR) -name '*.c')
OBJECTFILES := $(patsubst %.c, %.o, $(addprefix $(OBJS_DIR)/, $(SOURCEFILES)))
TARGET := ejemplo
EXISTO_OBJS := $(shell find $(OBJS_DIR) -name '*.o')

all : $(lib_name).a

.PHONY : install

install : $(lib_dir)/$(lib_name).a
	@echo libreria creada exitosamente!!
$(lib_dir)/$(lib_name).a : $(OBJECTFILES)
	@echo instalando libreria $^
	${AR} ${AR_ARGS} $@ $^
	@echo '\n'

.PHONY : tests

tests : $(TARGET) 

$(lib_name).a : $(OBJECTFILES)
	@echo creando libreria ${lib_name} '\n'
	@echo "Los objetos incluidos son : "'\n' $(OBJECTFILES) '\n''\n'
	${AR} ${AR_ARGS} $@ $^


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

