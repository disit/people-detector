#Recommended compiler warnings
CFLAGS += -Wall
# Application Name
TARGET := disit_app
OBJECTS := main.cpp

#file that store information about libs directory, name, dependency, includedir, libdir, description
PKGS = gio-2.0 vdostream gio-unix-2.0 glib-2.0 monkey


LDFLAGS += -L$(SDKTARGETSYSROOT)/usr/lib
LDLIBS += $(shell PKG_CONFIG_PATH=$(PKG_CONFIG_PATH) pkg-config --libs $(PKGS)) -ldl

#-Os Optimize for size
CXXFLAGS += -Os -pipe -std=c++14
CXXFLAGS += $(shell PKG_CONFIG_PATH=$(PKG_CONFIG_PATH) pkg-config --cflags $(PKGS))
CXXFLAGS += -I$(TARGET_ROOT)/usr/include/opencv4
CXXFLAGS += -I$(TARGET_ROOT)/libtorch/include
CXXFLAGS += -I$(TARGET_ROOT)/paho_mqtt/include
CXXFLAGS += -I$(TARGET_ROOT)/libtorch/include/torch/csrc/api/include

SDK_TARGET_LIBS=$(TARGET_ROOT)/usr/lib

SHLIB_DIR = ./lib

#if the linker is being invoked indirectly, via a compiler driver (e.g. ‘gcc’) then all the linker command-line options should be prefixed by ‘-Wl,’
#--no-as-needed links all library even not used
LDFLAGS = -L$(TARGET_ROOT)/paho_mqtt/lib -L$(TARGET_ROOT)/libtorch/lib -L$(TARGET_ROOT)/openblas/lib -L$(SHLIB_DIR) -Wl,--no-as-needed,-rpath,'$$ORIGIN/lib' -Wl,--no-as-needed,-rpath,'/var/spool/storage/SD_DISK/lib'
SHLIBS += -lm -lopencv_video -lopencv_imgcodecs -lopencv_imgproc -lopencv_core -lopenblas -lpthread -lmonkey -lgomp -lc10 -lshm -ltorch_global_deps -ltorch -ltorch_cpu -lpaho-mqtt3a -lpaho-mqtt3c
LIBCORE_FILES = $(wildcard $(SDK_TARGET_LIBS)/libopencv_core.so*)
LIBPROC_FILES = $(wildcard $(SDK_TARGET_LIBS)/libopencv_imgproc.so*)
LIBVIDEO_FILES = $(wildcard $(SDK_TARGET_LIBS)/libopencv_video.so*)
LIBIMG_FILES = $(wildcard $(SDK_TARGET_LIBS)/libopencv_imgcodecs.so*)

.PHONY: all clean

all: $(TARGET)

#The _GLIBCXX_USE_CXX11_ABI macro (see Macros) controls whether the declarations in the library headers use the old or new ABI. So the decision of which ABI to use can be made separately for each source file being compiled. Using the default configuration options for GCC the default value of the macro is 1 which causes the new ABI to be active, so to use the old ABI you must explicitly define the macro to 0 before including any library headers. (Be aware that some GNU/Linux distributions configure GCC 5 differently so that the default value of the macro is 0 and users must define it to 1 to enable the new ABI.)

$(TARGET): $(OBJECTS) libscopy
	$(CXX) $< $(CXXFLAGS) $(LDFLAGS) -D_GLIBCXX_USE_CXX11_ABI=1 $(SHLIBS) $(LDLIBS) imgprovider.cpp base64.cpp -o $@ && $(STRIP) --strip-unneeded $@

libscopy:
	mkdir -p $(SHLIB_DIR)
	cp $(LIBCORE_FILES) $(SHLIB_DIR)
	cp $(LIBPROC_FILES) $(SHLIB_DIR)
	cp $(LIBVIDEO_FILES) $(SHLIB_DIR)
	cp $(LIBIMG_FILES) $(SHLIB_DIR)


clean:
	$(RM) *.o $(TARGET)
