#necesitamos las variables que va a conocer el usuario
-include data.mk 
CC := gcc
AR := ar
AR_ARGS := rcs#queria usar libtool pero aparentemente no viene por defecto
#con el set de devtools de C a si queeeeee si u sistema no funciona
#con ran lib. Instala libtool y modifica esto, dejo comentado el uso de
#lib tool
RAN     := ranlib
RANARGS := -t 
C_FLAGS := -Wall #Por ahora solo nos interesa tener todas las warnings
C_LINKS := #Aqui irin librerias que deban incluirse (solo usare las 
#librerias estandar, a si que con excepcion de math.h no creo agregar nada
#mas a futuro, pero me sirve para recordar la estructura de libtool
export target_dir
DEPENDFLAGS := -MMD -MP #para que make tome en cuenta los .h
OBJS_DIR := objs#carpeta para objetos
FILS_DIR := src #Carpeta para codigo fuente
SOURCES := $(shell find ${FILS_DIR} -name '*.c')
OBJECTS := $(patsubst %.c, %.o, $(addprefix ${OBJS_DIR}/, $(SOURCES)))

#custom install que es lo que casi siempre se usara
#no se quien llamaria cinstall a un archivo, pero por si acaso lo hago asi
.PHONY : cinstall 

print : 
	@echo $(OBJECTS)

#cinstall : $(target_dir)/$(LIB_NAME).la
cinstall : $(target_dir)/$(LIB_NAME).a

#$(target_dir)/$(LIB_NAME).la : $(OJBECTS)
$(target_dir)/$(LIB_NAME).a : $(OBJECTS)
ifeq ($(target_dir),)
	@echo No se especifico directorio, no se creara nada
#creamos la libreria
else
	@echo Instalando libreria ${LIB_NAME}
	@echo 
	${AR} ${AR_ARGS} ${target_dir}/${LIB_NAME}.a $(OBJECTS)
	${RAN} ${RANARGS} ${target_dir}/${LIB_NAME}.a
	#libtool --mode=link $(CC) -g -O -o $(LIB_NAME).la $(OBJECTS) ${target_dir}/ $(C_LIBS)
endif
	@echo
#Compilamos todos los objetos
#%.lo : %.c
$(OBJS_DIR)/%.o : %.c
	if ! test -d $(dir $@) ; then mkdir -p $(dir $@); fi
	$(CC) -c $(CFLAGS) $< -o $@ $(DEPENDFLAGS)
	#libtool --mode=compile $(CC) -I. -g -O -c $<

-include $(OBJECTS:.o=.d)
