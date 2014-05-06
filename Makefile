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
SDL2GLTest.o\
ShaderUtil.o\
Ship.o\


TARGET=sdl2gl

%.o: %.cpp Makefile
	${CXX} ${DEBUGFLAGS} -W -Wall -c ${INCLUDE} $< -o $@


${TARGET}: ${OBJECTS} 
	${CXX} -o ${TARGET} ${OBJECTS} ${LIBDIR} ${LIBRARIES}


clean:
	rm ${OBJECTS}
	rm ${TARGET}
