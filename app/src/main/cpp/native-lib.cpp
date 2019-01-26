#include <jni.h>
#include <string>
#include <android/log.h>
#include <android_native_app_glue.h>

#include <vulkan_wrapper.h>

void android_main(struct android_app* state) {
    InitVulkan();
    __android_log_print(ANDROID_LOG_INFO, "native-activity", "Party Time! + Vulkan");
}

