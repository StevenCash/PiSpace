INCLUDE=\
-I/usr/local/include \
-I/opt/vc/include

LIBDIR=\
-L/opt/vc/lib \
-L/usr/local/lib \

LIBRARIES=\
-lSDL2 \
-lbcm_host \
-lGLESv2 \
-lcwiid \
-lBox2D


CXX=g++
OBJECTS=\
Wiimote.o\
Space.o\
ShaderUtil.o\
Ship.o\
GenericDrawing.o \
EventHandler.o \
Bullet.o \

DEBUGFLAGS=-DDEBUG

TARGET=space

%.o: %.cpp Makefile
	${CXX} ${DEBUGFLAGS} -W -Wall -c ${INCLUDE} $< -o $@


${TARGET}: ${OBJECTS} 
	${CXX} -o ${TARGET} ${OBJECTS} ${LIBDIR} ${LIBRARIES}


clean:
	rm ${OBJECTS}
	rm ${TARGET}
