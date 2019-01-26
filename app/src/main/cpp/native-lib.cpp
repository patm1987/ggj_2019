#include <jni.h>
#include <string>
#include <android_native_app_glue.h>
#include <sstream>

#include "Game.h"
#include "Logger.h"

void handle_cmd(android_app* pApp, int32_t cmd);

void android_main(struct android_app *pApp) {
    Logger::Info("Party Time + Vulkan + Logger");
    pApp->onAppCmd = handle_cmd;

    int events;
    android_poll_source *pSource;
    do {
        if (ALooper_pollAll(10, nullptr, &events, (void **) &pSource) > 0) {
            if (pSource) {
                pSource->process(pApp, pSource);
            }
        }
    } while (!pApp->destroyRequested);
}

void handle_cmd(android_app* pApp, int32_t cmd) {
    switch (cmd) {
        case APP_CMD_INIT_WINDOW:
            // TODO: create the game at app entry, just create a renderer here
            pApp->userData = new Game();
            break;
        case APP_CMD_TERM_WINDOW:
            // TODO: keep the game, just recreate the renderer
            if (pApp->userData) {
                Game* pGame = reinterpret_cast<Game*>(pApp->userData);
                delete(pGame);
                pApp->userData = nullptr;
            }
            break;
        default:
            std::stringstream stream;
            stream << "Event not handled: " << cmd;
            Logger::Info(stream.str());
    }
}
