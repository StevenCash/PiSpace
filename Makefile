INCLUDE=\
-I/usr/local/include \
-I/opt/vc/include

LIBDIR=\
-L/opt/vc/lib \
-L/usr/local/lib \

LIBRARIES=\
-lSDL2 \
-lSDL2_mixer \
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
Ships.o \
EventHandler.o \
Bullet.o \
Walls.o \
Vortex.o \
ContactListener.o \
SoundBank.o \
Stars.o \
ConnectionThread.o \


FLAGS=-march=armv6 -mfpu=vfp -mfloat-abi=hard -DRPI

TARGET=space

%.o: %.cpp Makefile
	${CXX} ${FLAGS} -W -Wall -c ${INCLUDE} $< -o $@


${TARGET}: ${OBJECTS} 
	${CXX} -o ${TARGET} ${OBJECTS} ${LIBDIR} ${LIBRARIES}


clean:
	rm ${OBJECTS}
	rm ${TARGET}
