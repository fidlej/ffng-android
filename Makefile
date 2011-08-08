CFLAGS	= -O2
LIBS	= 
LIBNAME = libffng.so

SOURCES =	$(wildcard jni/*.cpp) \
			$(wildcard jni/lua/*.c) \
			\
			$(wildcard jni/ff/game/*.cpp) \
			$(wildcard jni/ff/gengine/*.cpp) \
			$(wildcard jni/ff/plan/*.cpp) \
			$(wildcard jni/ff/effect/*.cpp) \
			$(wildcard jni/ff/menu/*.cpp) \
			$(wildcard jni/ff/level/*.cpp) \
			$(wildcard jni/ff/option/*.cpp) \
			$(wildcard jni/ff/state/*.cpp) \
			$(wildcard jni/ff/widget/*.cpp)
INCLUDES =	-Ijni \
			-Ijni/lua \
			\
			-Ijni/ff/game \
			-Ijni/ff/gengine \
			-Ijni/ff/plan \
			-Ijni/ff/effect \
			-Ijni/ff/menu \
			-Ijni/ff/level \
			-Ijni/ff/option \
			-Ijni/ff/state \
			-Ijni/ff/widget
OBJS_FOO	= $(SOURCES:jni/%.cpp=obj/%.o)
OBJS	= $(OBJS_FOO:jni/%.c=obj/%.o)
SYMS	= obj/$(LIBNAME)
LIB		= libs/armeabi/$(LIBNAME)

all:	$(LIB)

ifeq ($(shell uname),Linux)
ANDROID_NDK_BASE = $(HOME)/android/android-ndk-r4-crystax
else # windows
ANDROID_NDK_BASE = /cygdrive/c/dan/android/android-ndk-r4-crystax

ANDROID_PLAT=windows
endif
ANDROID_TOOL_PREFIX = $(ANDROID_NDK_BASE)/build/prebuilt/$(ANDROID_PLAT)/arm-eabi-4.2.1/bin/arm-eabi-
ANDROID_CFLAGS = -march=armv5te -mtune=xscale -msoft-float -fpic -mthumb-interwork \
	-ffunction-sections -funwind-tables -fstack-protector -fno-short-enums \
	-D__ARM_ARCH_5__ -D__ARM_ARCH_5T__ -D__ARM_ARCH_5E__ -D__ARM_ARCH_5TE__ -DANDROID -O2 -DNDEBUG -g \
	-I$(ANDROID_NDK_BASE)/build/platforms/android-4/arch-arm/usr/include

#ANDROID_CFLAGS = -march=armv5te -mtune=xscale -msoft-float -fpic -mthumb-interwork \
	-ffunction-sections -funwind-tables -fstack-protector -fno-short-enums \
	-D__ARM_ARCH_5__ -D__ARM_ARCH_5T__ -D__ARM_ARCH_5E__ -D__ARM_ARCH_5TE__ -DANDROID -O0 -g \
	-I$(ANDROID_NDK_BASE)/build/platforms/android-4/arch-arm/usr/include

$(LIB):	$(SYMS)
	@mkdir -p $(dir $@)
	$(ANDROID_TOOL_PREFIX)strip --strip-debug $^ -o $@
	#cp $^ $@

$(SYMS):	$(OBJS)
	@mkdir -p $(dir $@)
	$(ANDROID_TOOL_PREFIX)ld \
		-o $(SYMS) $(OBJS) $(LIBS) \
		-nostdlib -shared -Bsymbolic --no-undefined \
		-L$(ANDROID_NDK_BASE)/build/platforms/android-4/arch-arm/usr/lib \
		-llog -lc -lm \
		$(ANDROID_NDK_BASE)/build/prebuilt/$(ANDROID_PLAT)/arm-eabi-4.2.1/lib/gcc/arm-eabi/4.2.1/interwork/libgcc.a \
		$(ANDROID_NDK_BASE)/build/prebuilt/$(ANDROID_PLAT)/arm-eabi-4.2.1/arm-eabi/lib/interwork/libstdc++.a \

obj/%.o:	jni/%.c
	@mkdir -p $(dir $@)
	$(ANDROID_TOOL_PREFIX)gcc $(CFLAGS) $(ANDROID_CFLAGS) $(INCLUDES) -MD -o $@ -c $<

obj/%.o:	jni/%.cpp
	@mkdir -p $(dir $@)
	$(ANDROID_TOOL_PREFIX)g++ $(CFLAGS) $(ANDROID_CFLAGS) $(INCLUDES) -MD -o $@ -c $<

-include $(OBJS:%.o=%.d)

clean:
	rm -fr $(OBJS) $(SYMS) $(LIB)
