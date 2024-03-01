GCC=${MINGW64_DIR}/bin/gcc.exe
CFLAGS=-Wall -Wextra -Werror

GXX=${MINGW64_DIR}/bin/g++.exe
CXXFLAGS=${CFLAGS} -DALLEGRO_STATICLINK

ALLEGRO_DIR=C:/SDK/Allegro5
INCLUDE_PATH=${ALLEGRO_DIR}/allegro/include
LIB_PATH=${ALLEGRO_DIR}/allegro/lib
LIBS=-lallegro \
	-lallegro_main \
	-lallegro_primitives \
	-lallegro_color \
	-lallegro_font \
	-lallegro_ttf \
	-lallegro_audio \
	-lallegro_acodec

INCLUDE_PATH_STATIC=${ALLEGRO_DIR}/allegro-static/include
LIB_PATH_STATIC=${ALLEGRO_DIR}/allegro-static/lib
LIBS_STATIC=-lallegro-static \
	-lallegro_main-static \
	-lallegro_primitives-static \
	-lallegro_color-static \
	-lallegro_font-static \
	-lallegro_ttf-static \
	-lallegro_audio-static \
	-lallegro_acodec-static -static \
	-ljpeg \
	-ldumb \
	-lFLAC \
	-ltheoradec \
	-lvorbisfile \
	-lvorbis \
	-logg \
	-lphysfs \
	-lfreetype \
	-lopusfile \
	-lopus \
	-lpng16 \
	-lzlib \
	-ldsound \
	-lgdiplus \
	-luuid \
	-lkernel32 \
	-lwinmm \
	-lpsapi \
	-lopengl32 \
	-lglu32 \
	-luser32 \
	-lcomdlg32 \
	-lgdi32 \
	-lshell32 \
	-lole32 \
	-ladvapi32 \
	-lws2_32 \
	-lshlwapi \
	-static-libstdc++ \
	-static-libgcc

.PHONY: build build-static

build:
	$(GCC) $(CFLAGS) -g src/*.c -o build/blasteroids.exe -I$(INCLUDE_PATH) -L$(LIB_PATH) $(LIBS)

build-static:
	$(GXX) $(CXXFLAGS) -g src/*.c -o build/blasteroids-static.exe -I$(INCLUDE_PATH_STATIC) -L$(LIB_PATH_STATIC) $(LIBS_STATIC)

all: build build-static

clean:
	rm build/*
