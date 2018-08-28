LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE		:= gm_mpegh_iis_dec

include $(LOCAL_PATH)/base.mk

LOCAL_C_INCLUDES 	+= $(LOCAL_PATH)/../../../../extra_lib/include/
LOCAL_LDLIBS    += -lmpegh

LOCAL_SRC_FILES := ../../../../modules/mpegh_iis_dec/mpegh_iis_dec.c

include $(BUILD_SHARED_LIBRARY)
