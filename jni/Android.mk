LOCAL_PATH := $(call my-dir)


include $(CLEAR_VARS)

# define binary name
LOCAL_MODULE    := testapp

# local source files
LOCAL_SRC_FILES := testapp.c

# remote STUB library wrapper code
# (it is assumed that projects are on the same level)
LOCAL_SRC_FILES += ../../testlib/src/testlib_stub.c

# LOL!!! there are some issues with : symbol from the file path!
# LOCAL_SRC_FILES += C:\Qualcomm\Hexagon_SDK\1.1.0\lib\common\rpcmem\rpcmem_android.c
# this is waaaay to much up, but it does not matter... what a shame...
LOCAL_SRC_FILES += ../../../../../Qualcomm/Hexagon_SDK/1.1.0/lib/common/rpcmem/rpcmem_android.c

# remote STUB library header file contains function declaration
LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../testlib/src
# hexagon includes
LOCAL_C_INCLUDES += ${HEXAGON_SDK_ROOT}/lib/common/rpcmem
LOCAL_C_INCLUDES += ${HEXAGON_SDK_ROOT}/inc/stddef
LOCAL_C_INCLUDES += ${HEXAGON_SDK_ROOT}/inc/qlist
LOCAL_C_INCLUDES += ${HEXAGON_SDK_ROOT}/lib/common/remote/android_ReleaseG
LOCAL_C_INCLUDES += ${HEXAGON_SDK_ROOT}/lib/common/adspmsgd/android_ReleaseG

# hexagon libraries + logging library (for debugs)
LOCAL_LDFLAGS := -L${HEXAGON_SDK_ROOT}/lib/common/remote/android_ReleaseG
LOCAL_LDFLAGS += -ladsprpc -llog

# this is fucked! because this lib has not 'lib' prefixed name, 
# we can not specify dir with -L and lib with -l as was done for adsprpc
LOCAL_LDLIBS := -l${HEXAGON_SDK_ROOT}/lib/common/adspmsgd/android_Debug/adspmsgd.lib

LOCAL_C_FLAGS += -g -O0 -fno-inline

# we usually build shared_libraries that are accessed from java
# but in this case, we want standalone executable
include $(BUILD_EXECUTABLE)
