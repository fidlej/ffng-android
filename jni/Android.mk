LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

FFNG_CFLAGS 	  := -O2 -Wall
FFNG_SRC_FILES :=	\
					jni.cpp \
					\
					ff/gengine/ResSoundPack.cpp \
					ff/level/LevelStatus.cpp \
					ff/game/Application.cpp \
					ff/gengine/Name.cpp \
					ff/gengine/BaseListener.cpp \
					ff/gengine/Log.cpp \
					ff/gengine/Path.cpp \
					ff/gengine/Random.cpp \
					ff/gengine/ExInfo.cpp \
					ff/gengine/AgentPack.cpp \
					ff/gengine/MessagerAgent.cpp \
					ff/gengine/VideoAgent.cpp \
					ff/gengine/InputAgent.cpp \
					ff/gengine/TimerAgent.cpp \
					ff/game/GameAgent.cpp \
					ff/gengine/SoundAgent.cpp \
					ff/gengine/SDLSoundAgent.cpp \
					ff/gengine/ScriptAgent.cpp \
					ff/gengine/OptionAgent.cpp \
					ff/plan/SubtitleAgent.cpp \
					ff/gengine/OptionParams.cpp \
					ff/effect/Font.cpp \
					ff/gengine/StringMsg.cpp \
					ff/gengine/BaseAgent.cpp \
					ff/gengine/MultiDrawer.cpp \
					ff/gengine/BaseException.cpp \
					ff/gengine/IntMsg.cpp \
					ff/gengine/StringTool.cpp \
					ff/gengine/BaseMsg.cpp \
					ff/gengine/Scripter.cpp \
					ff/gengine/Dialog.cpp \
					ff/gengine/FsPath.cpp \
					ff/gengine/ImgException.cpp \
					ff/gengine/SDLException.cpp \
					ff/gengine/SysVideo.cpp \
					ff/gengine/KeyBinder.cpp \
					ff/gengine/MouseStroke.cpp \
					ff/gengine/KeyStroke.cpp \
					ff/gengine/InputHandler.cpp \
					ff/plan/StateManager.cpp \
					ff/menu/WorldMap.cpp \
					ff/plan/GameState.cpp \
					ff/gengine/MixException.cpp \
					ff/gengine/ScriptState.cpp \
					ff/gengine/options-script.cpp \
					ff/gengine/Environ.cpp \
					ff/plan/Title.cpp \
					ff/effect/TTFException.cpp \
					ff/effect/Outline.cpp \
					ff/gengine/def-script.cpp \
					ff/menu/LevelNode.cpp \
					ff/menu/NodeDrawer.cpp \
					ff/gengine/ResImagePack.cpp \
					ff/menu/WorldInput.cpp \
					ff/menu/WorldBranch.cpp \
					ff/gengine/ResDialogPack.cpp \
					ff/menu/LevelDesc.cpp \
					ff/level/Level.cpp \
					ff/menu/Pedometer.cpp \
					ff/effect/LayeredPicture.cpp \
					ff/option/MenuOptions.cpp \
					ff/state/DemoMode.cpp \
					ff/state/PosterScroller.cpp \
					ff/state/MovieState.cpp \
					ff/state/GameInput.cpp \
					ff/effect/Picture.cpp \
					ff/plan/Planner.cpp \
					ff/state/PosterState.cpp \
					ff/plan/StateInput.cpp \
					ff/effect/PixelTool.cpp \
					ff/effect/SurfaceTool.cpp \
					ff/effect/SurfaceLock.cpp \
					ff/plan/Keymap.cpp \
					ff/plan/KeyDesc.cpp \
					ff/menu/worldmap-script.cpp \
					ff/level/PhaseLocker.cpp \
					ff/level/LevelInput.cpp \
					ff/level/LevelScript.cpp \
					ff/level/LevelLoading.cpp \
					ff/level/LevelCountDown.cpp \
					ff/plan/CommandQueue.cpp \
					ff/level/Room.cpp \
					ff/level/View.cpp \
					ff/level/StepDecor.cpp \
					ff/level/StatusDisplay.cpp \
					ff/gengine/DialogStack.cpp \
					ff/level/RoomAccess.cpp \
					ff/level/Cube.cpp \
					ff/level/ModelList.cpp \
					ff/level/Goal.cpp \
					ff/level/Dir.cpp \
					ff/menu/PedoInput.cpp \
					ff/menu/SolverDrawer.cpp \
					ff/widget/VBox.cpp \
					ff/widget/WiBox.cpp \
					ff/widget/IWidget.cpp \
					ff/widget/HBox.cpp \
					ff/widget/WiPicture.cpp \
					ff/widget/WiButton.cpp \
					ff/widget/WiStatusBar.cpp \
					ff/widget/Slider.cpp \
					ff/option/SelectLang.cpp \
					ff/widget/Labels.cpp \
					ff/option/OptionsInput.cpp \
					ff/widget/WiContainer.cpp \
					ff/option/RadioBox.cpp \
					ff/state/DemoInput.cpp \
					ff/state/demo-script.cpp \
					ff/option/MenuHelp.cpp \
					ff/plan/ScriptCmd.cpp \
					ff/plan/dialog-script.cpp \
					ff/plan/KeyConsole.cpp \
					ff/level/Unit.cpp \
					ff/level/game-script.cpp \
					ff/level/level-script.cpp \
					ff/level/KeyControl.cpp \
					ff/effect/WavyPicture.cpp \
					ff/level/Field.cpp \
					ff/level/FinderAlg.cpp \
					ff/level/Controls.cpp \
					ff/level/Rules.cpp \
					ff/level/Landslip.cpp \
					ff/level/MouseControl.cpp \
					ff/level/FinderField.cpp \
					ff/level/Anim.cpp \
					ff/gengine/PlannedDialog.cpp \
					ff/level/Shape.cpp \
					ff/effect/EffectDisintegrate.cpp \
					ff/widget/WiPara.cpp \
					ff/option/HelpInput.cpp \
					ff/plan/FishDialog.cpp \
					ff/plan/ConsoleInput.cpp \
					ff/level/ModelFactory.cpp \
					ff/level/RopeDecor.cpp \
					ff/level/Shapebuilder.cpp \
					ff/level/MarkMask.cpp \
					ff/effect/EffectNone.cpp \
					ff/effect/EffectMirror.cpp \
					ff/effect/EffectReverse.cpp \
					ff/effect/EffectZx.cpp \
					ff/widget/WiLabel.cpp \
					\
					FFNGApp.cpp \
					FFNGFont.cpp \
					FFNGKey.cpp \
					FFNGMouse.cpp \
					FFNGMusic.cpp \
					FFNGSurface.cpp \
					FFNGVideo.cpp
					
include $(LOCAL_PATH)/lua/Android.mk

LOCAL_C_INCLUDES := $(LOCAL_PATH) \
					$(LOCAL_PATH)/lua \
					$(LOCAL_PATH)/ff/game \
					$(LOCAL_PATH)/ff/gengine \
					$(LOCAL_PATH)/ff/plan \
					$(LOCAL_PATH)/ff/effect \
					$(LOCAL_PATH)/ff/menu \
					$(LOCAL_PATH)/ff/level \
					$(LOCAL_PATH)/ff/option \
					$(LOCAL_PATH)/ff/state \
					$(LOCAL_PATH)/ff/widget
LOCAL_MODULE    := ffng
LOCAL_ARM_MODE  := arm
LOCAL_SRC_FILES := $(FFNG_SRC_FILES)
LOCAL_CFLAGS    := $(FFNG_CFLAGS)
LOCAL_LDLIBS := -llog

include $(BUILD_SHARED_LIBRARY)

