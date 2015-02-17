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
EventHandler.o \
Bullet.o \
Walls.o \
Vortex.o \


FLAGS=-march=armv6 -mfpu=vfp -mfloat-abi=hard

TARGET=space

%.o: %.cpp Makefile
	${CXX} ${FLAGS} -W -Wall -c ${INCLUDE} $< -o $@


${TARGET}: ${OBJECTS} 
	${CXX} -o ${TARGET} ${OBJECTS} ${LIBDIR} ${LIBRARIES}


clean:
	rm ${OBJECTS}
	rm ${TARGET}
