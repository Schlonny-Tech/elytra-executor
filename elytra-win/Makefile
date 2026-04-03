# do not use its just for me schlonny mainly to perfectly compile it for windows on linux!
CXX = x86_64-w64-mingw32-g++
CXXFLAGS = -std=c++20 -O2 -Wall -m64 -shared -static-libgcc -static-libstdc++ -Wno-unknown-pragmas -fpermissive
DEFINES = -DNDEBUG -DINTERNALDLLMHDX11_EXPORTS -D_WINDOWS -D_USRDLL -DWIN32_LEAN_AND_MEAN -DNOMINMAX -D_WIN32_WINNT=0x0A00 -D_CRT_SECURE_NO_WARNINGS
INCLUDES = -I. -I./thirdparty/imgui -I./thirdparty/kiero -I./thirdparty/minhook/include -I/usr/x86_64-w64-mingw32/include
LIBS = -ld3d11 -ldxgi -ldxguid -luuid -lwinmm -lgdi32 -limm32 -lversion -lwininet -lcomdlg32 -ldwmapi -ld3dcompiler -static-libgcc -static-libstdc++

BUILD_DIR = build

SOURCES = \
    dllmain.cpp \
    Globals.cpp \
    LuaEngine.cpp \
    menu/dx11hook.cpp \
    NetHooks.cpp \
    SDK.cpp \
    TextEditor.cpp \
    UI.cpp \
    utility/utility.cpp \
    thirdparty/imgui/imgui.cpp \
    thirdparty/imgui/imgui_demo.cpp \
    thirdparty/imgui/imgui_draw.cpp \
    thirdparty/imgui/imgui_impl_dx11.cpp \
    thirdparty/imgui/imgui_impl_win32.cpp \
    thirdparty/imgui/imgui_tables.cpp \
    thirdparty/imgui/imgui_widgets.cpp \
    thirdparty/kiero/kiero.cpp \
    thirdparty/minhook/src/buffer.c \
    thirdparty/minhook/src/hde/hde32.c \
    thirdparty/minhook/src/hde/hde64.c \
    thirdparty/minhook/src/hook.c \
    thirdparty/minhook/src/trampoline.c

OBJECTS = $(addprefix $(BUILD_DIR)/, $(notdir $(SOURCES:.cpp=.o)))
OBJECTS := $(OBJECTS:.c=.o)
TARGET = $(BUILD_DIR)/InternalDLL.dll

all: $(BUILD_DIR) $(TARGET)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(DEFINES) $(INCLUDES) -o $@ $^ $(LIBS)

$(BUILD_DIR)/%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(DEFINES) $(INCLUDES) -c $< -o $@

$(BUILD_DIR)/%.o: menu/%.cpp
	$(CXX) $(CXXFLAGS) $(DEFINES) $(INCLUDES) -c $< -o $@

$(BUILD_DIR)/%.o: utility/%.cpp
	$(CXX) $(CXXFLAGS) $(DEFINES) $(INCLUDES) -c $< -o $@

$(BUILD_DIR)/%.o: thirdparty/imgui/%.cpp
	$(CXX) $(CXXFLAGS) $(DEFINES) $(INCLUDES) -c $< -o $@

$(BUILD_DIR)/%.o: thirdparty/kiero/%.cpp
	$(CXX) $(CXXFLAGS) $(DEFINES) $(INCLUDES) -c $< -o $@

$(BUILD_DIR)/%.o: thirdparty/minhook/src/%.c
	$(CXX) $(CXXFLAGS) $(DEFINES) $(INCLUDES) -c $< -o $@

$(BUILD_DIR)/%.o: thirdparty/minhook/src/hde/%.c
	$(CXX) $(CXXFLAGS) $(DEFINES) $(INCLUDES) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean