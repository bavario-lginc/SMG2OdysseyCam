#include "revolution.h"
#include "kamek/hooks.h"
#include "Game/Util.h"

typedef void (*Func)();
extern Func __ctor_loc;
extern Func __ctor_end;

namespace {
    // ----------------------------------------------------------------------------------------------------------------
    // Calls all static initializers after MR::initAcosTable. This is necessary to initialize static variables and 
    // constants in the BSS segment. Custom Nerve instances will be stored there, for example.

    void init() {
        for (Func* f = &__ctor_loc; f < &__ctor_end; f++) {
            (*f)();
        }
    }

    // Called after initAcosTable
    kmBranch(0x8003B344, init);


    // ----------------------------------------------------------------------------------------------------------------
    // Events to be handled after GameScene::start

    void handleGameSceneStart() {
        
    }

    kmBranch(0x80451888, handleGameSceneStart);


    // ----------------------------------------------------------------------------------------------------------------
    // Events to be handled after any scene gets destroyed

    void handleAnySceneDestroyed() {
        
    }

    double OdysseyLikeCamera (double a, double b) {
        // Calculate normal result and then subtract 40°-ish
        double result = fmod(a, 6.283185482025146) + (MR::testCorePadTriggerLeft(0) || MR::testCorePadButtonLeft(0) ? 0.698132 : -0.698132);
        if (result < 0) {
            result = 6.283185482025146 + result;
            OSReport("CameraPos < 0, underflowing to %g\n", result);
        }
        else if (result > 6.283185482025146) {
            result -= 6.283185482025146;
            OSReport("CameraPos > 2π, overflowing to %g\n", result);
        }
        return result;
    }
    bool disableSFX () {
        return false; // Lol
    }
    int holdCameraBtnLeft () {
        // Normally, the camera functions call MR::testCorePadTriggerLeft,
        // but that only returns true once while MR::testCorePadButtonLeft
        // returns true on every frame Left is held.
        return (MR::testCorePadTriggerLeft(0) || MR::testCorePadButtonLeft(0));
    }
    int holdCameraBtnRight () {
        // Same here.
        return (MR::testCorePadTriggerRight(0) || MR::testCorePadButtonRight(0));
    }
    kmCall(0x801180C8, OdysseyLikeCamera); // fmod in CameraFollow::startRound
    //kmCall(0x80115930, disableSFX); Crashes the game
    kmCall(0x8011E118, holdCameraBtnLeft); // return in CameraUtil::testCameraPadTriggerLeft
    kmCall(0x8011E168, holdCameraBtnRight); // return in CameraUtil::testCameraPadTriggerRight
}
