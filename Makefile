-include data.mk
CC := gcc
AR := ar
AR_ARGS = rcs
CFLAGS := -Wall
export c_lib_dir
DEPENDFLAGS := -MMD -MP
OBJS_DIR := objs
FILES_DIR := src
SOURCEFILES := $(shell find $(FILES_DIR) -name '*.c')
OBJECTFILES := $(patsubst %.c, %.o, $(addprefix $(OBJS_DIR)/, $(SOURCEFILES)))
TARGET := ejemplo
EXISTO_OBJS := $(shell find $(OBJS_DIR) -name '*.o')

all : $(lib_name).a
	#For custom installs
.PHONY : cinstall
#NOTA: para setear los valores de una variable,
#se sigue esta regla <Nombre variable>=<Valor>
#El orden da igual, pero parece que es comun definirlo
#antes de los argumentos

cinstall : $(c_lib_dir)/$(LIB_NAME).a

$(c_lib_dir)/$(LIB_NAME).a : $(OBJECTFILES)
ifeq ($(c_lib_dir),)
	@echo no valor definido para c_lib_dir no se creo nada
else
	@echo Instalando libreria ${lib_name}
	@echo
	${AR} ${AR_ARGS} $@ $^
endif
	@echo 

.PHONY : install

install : $(lib_dir)/$(lib_name).a
	@echo libreria creada exitosamente!!
$(lib_dir)/$(lib_name).a : $(OBJECTFILES)
	@echo instalando libreria $^
	${AR} ${AR_ARGS} $@ $^
	@echo 

.PHONY : tests

tests : $(TARGET) 

$(lib_name).a : $(OBJECTFILES)
	@echo creando libreria ${LIB_NAME} '\n'
	@echo "Los objetos incluidos son : "'\n' $(OBJECTFILES) '\n''\n'
	${AR} ${AR_ARGS} $@ $^


$(TARGET) : $(OBJECTFILES)
	@echo El programa $(TARGET) esta siendo construido
	@echo usando los headers $(patsubst %.c, %.h, $(SOURCEFILES))
	$(CC) $(CFLAGS) $(OBJECTFILES) -o $(TARGET)

$(OBJS_DIR)/%.o : %.c
	if ! test -d $(dir @) ; then mkdir -p $(dir @); fi
	#mkdir -p $(dir $@)
	$(CC) -c $(CFLAGS) $< -o $@ $(DEPENDFLAGS)


-include $(OBJECTFILES:.o=.d)

.PHONY : clean

clean : 
	rm -f ${EXIST_OBJS} ${TARGET} $(patsubst %.o, %.d, ${EXIST_OBJS})

