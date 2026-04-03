#include "SDK.h"

HMODULE g_GameAssembly = nullptr;
HMODULE g_Module = nullptr;

// IL2CPP
il2cpp_string_new_t il2cpp_string_new = nullptr;
il2cpp_domain_get_t il2cpp_domain_get = nullptr;
il2cpp_thread_attach_t il2cpp_thread_attach = nullptr;
il2cpp_thread_detach_t il2cpp_thread_detach = nullptr;
il2cpp_domain_get_assemblies_t il2cpp_domain_get_assemblies = nullptr;
il2cpp_assembly_get_image_t il2cpp_assembly_get_image = nullptr;
il2cpp_class_from_name_t il2cpp_class_from_name = nullptr;
il2cpp_class_get_static_field_data_t il2cpp_class_get_static_field_data =
    nullptr;
il2cpp_object_new_t il2cpp_object_new = nullptr;

// Game
Instance_GetChildren_t Instance_GetChildren = nullptr;
Instance_get_Name_t Instance_get_Name = nullptr;
Instance_set_Name_t Instance_set_Name = nullptr;
Instance_get_ClassName_t Instance_get_ClassName = nullptr;
Instance_Destroy_t Instance_Destroy = nullptr;
Instance_Clone_t Instance_Clone = nullptr;
Instance_New_t Instance_New = nullptr;
BaseScript_set_Source_t BaseScript_set_Source = nullptr;
BaseScript_get_Source_t BaseScript_get_Source = nullptr;
BaseScript_Run_t BaseScript_Run = nullptr;
ScriptService_get_Instance_t ScriptService_get_Instance = nullptr;
ScriptService_RunScript_t ScriptService_RunScript = nullptr;
Plr_CmdSetHealth_t Plr_CmdSetHealth = nullptr;
NB_get_netId_t NB_get_netId = nullptr;
SendCommandInternal_t SendCmdInternal = nullptr;

// NetworkWriter
NW_ctor_t NW_ctor = nullptr;
NW_WriteByte_t NW_WriteByte = nullptr;
NW_WriteBlittableFloat_t NW_WriteFloat = nullptr;
NW_WriteBlittableUInt_t NW_WriteUInt = nullptr;
NW_WriteBlittableUShort_t NW_WriteUShort = nullptr;
NW_WriteBool_t NW_WriteBool = nullptr;
NW_WriteVector3_t NW_WriteVector3 = nullptr;
NW_WriteString_t NW_WriteString = nullptr;

// RPC Info
GetFunctionMethodName_t GetFunctionMethodName = nullptr;
CommandRequiresAuthority_t CommandRequiresAuthority = nullptr;

// Objects
Il2CppDomain *g_Domain = nullptr;
Il2CppThread *g_Thread = nullptr;
void *g_GameSingleton = nullptr;
void *g_ScriptService = nullptr;
void *g_PlayersInstance = nullptr;
void *g_NetworkWriterClass = nullptr;

bool InitIL2CPP() {
  g_GameAssembly = GetModuleHandleA("GameAssembly.dll");
  if (!g_GameAssembly)
    return false;

  il2cpp_string_new =
      (il2cpp_string_new_t)GetProcAddress(g_GameAssembly, "il2cpp_string_new");
  il2cpp_domain_get =
      (il2cpp_domain_get_t)GetProcAddress(g_GameAssembly, "il2cpp_domain_get");
  il2cpp_thread_attach = (il2cpp_thread_attach_t)GetProcAddress(
      g_GameAssembly, "il2cpp_thread_attach");
  il2cpp_thread_detach = (il2cpp_thread_detach_t)GetProcAddress(
      g_GameAssembly, "il2cpp_thread_detach");
  il2cpp_domain_get_assemblies = (il2cpp_domain_get_assemblies_t)GetProcAddress(
      g_GameAssembly, "il2cpp_domain_get_assemblies");
  il2cpp_assembly_get_image = (il2cpp_assembly_get_image_t)GetProcAddress(
      g_GameAssembly, "il2cpp_assembly_get_image");
  il2cpp_class_from_name = (il2cpp_class_from_name_t)GetProcAddress(
      g_GameAssembly, "il2cpp_class_from_name");
  il2cpp_class_get_static_field_data =
      (il2cpp_class_get_static_field_data_t)GetProcAddress(
          g_GameAssembly, "il2cpp_class_get_static_field_data");
  il2cpp_object_new =
      (il2cpp_object_new_t)GetProcAddress(g_GameAssembly, "il2cpp_object_new");

  if (!il2cpp_string_new || !il2cpp_domain_get || !il2cpp_thread_attach)
    return false;

  uintptr_t b = (uintptr_t)g_GameAssembly;
  Instance_GetChildren =
      (Instance_GetChildren_t)(b + INSTANCE_GETCHILDREN_OFFSET);
  Instance_get_Name = (Instance_get_Name_t)(b + INSTANCE_GET_NAME_OFFSET);
  Instance_set_Name = (Instance_set_Name_t)(b + INSTANCE_SET_NAME_OFFSET);
  Instance_get_ClassName =
      (Instance_get_ClassName_t)(b + INSTANCE_GET_CLASSNAME_OFFSET);
  Instance_Destroy = (Instance_Destroy_t)(b + INSTANCE_DESTROY_OFFSET);
  Instance_Clone = (Instance_Clone_t)(b + INSTANCE_CLONE_OFFSET);
  Instance_New = (Instance_New_t)(b + INSTANCE_NEW_OFFSET);
  BaseScript_set_Source =
      (BaseScript_set_Source_t)(b + BASESCRIPT_SET_SOURCE_OFFSET);
  BaseScript_Run = (BaseScript_Run_t)(b + BASESCRIPT_RUN_OFFSET);
  BaseScript_get_Source =
      (BaseScript_get_Source_t)(b + BASESCRIPT_GET_SOURCE_OFFSET);
  ScriptService_get_Instance =
      (ScriptService_get_Instance_t)(b + SCRIPTSERVICE_GET_INSTANCE_OFFSET);
  ScriptService_RunScript =
      (ScriptService_RunScript_t)(b + SCRIPTSERVICE_RUNSCRIPT_OFFSET);
  Plr_CmdSetHealth = (Plr_CmdSetHealth_t)(b + PLR_CMDSETHEALTH_OFFSET);
  SendCmdInternal = (SendCommandInternal_t)(b + SEND_CMD_INTERNAL_OFFSET);
  NB_get_netId = (NB_get_netId_t)(b + NB_GET_NETID_OFFSET);

  NW_ctor = (NW_ctor_t)(b + NW_CTOR_OFFSET);
  NW_WriteByte = (NW_WriteByte_t)(b + NW_WRITEBYTE_OFFSET);
  NW_WriteFloat = (NW_WriteBlittableFloat_t)(b + NW_WRITE_FLOAT_OFFSET);
  NW_WriteUInt = (NW_WriteBlittableUInt_t)(b + NW_WRITE_UINT_OFFSET);
  NW_WriteUShort = (NW_WriteBlittableUShort_t)(b + NW_WRITE_USHORT_OFFSET);
  NW_WriteBool = (NW_WriteBool_t)(b + NW_WRITE_BOOL_OFFSET);
  NW_WriteVector3 = (NW_WriteVector3_t)(b + NW_WRITE_VEC3_OFFSET);
  NW_WriteString = (NW_WriteString_t)(b + NW_WRITE_STRING_OFFSET);

  GetFunctionMethodName = (GetFunctionMethodName_t)(b + GET_FUNC_NAME_OFFSET);
  CommandRequiresAuthority =
      (CommandRequiresAuthority_t)(b + CMD_REQUIRES_AUTH_OFFSET);

  return true;
}

bool AttachIL2CPP() {
  g_Domain = il2cpp_domain_get();
  g_Thread = il2cpp_thread_attach(g_Domain);
  return g_Thread != nullptr;
}

bool SafeReadString(Il2CppString *str, char *out, int size) {
  if (!str || !out)
    return false;
  try {
    int len = *(int *)((uintptr_t)str + STRING_LENGTH_OFFSET);
    if (len <= 0 || len >= size)
      return false;
    wchar_t *chars = (wchar_t *)((uintptr_t)str + STRING_CHARS_OFFSET);
    for (int i = 0; i < len; i++)
      out[i] = (char)chars[i];
    out[len] = 0;
    return true;
  } catch (...) {
    return false;
  }
}

bool SafeGetName(void *inst, char *out, int size) {
  if (!inst || !Instance_get_Name)
    return false;
  try {
    return SafeReadString(Instance_get_Name(inst), out, size);
  } catch (...) {
    return false;
  }
}

bool SafeGetClassName(void *inst, char *out, int size) {
  if (!inst || !Instance_get_ClassName)
    return false;
  try {
    return SafeReadString(Instance_get_ClassName(inst), out, size);
  } catch (...) {
    return false;
  }
}

int SafeGetChildren(void *inst, void **out, int max) {
  if (!inst || !Instance_GetChildren)
    return 0;
  try {
    Il2CppArray *arr = Instance_GetChildren(inst);
    if (!arr)
      return 0;
    int cnt = *(int *)((uintptr_t)arr + ARRAY_LENGTH_OFFSET);
    if (cnt <= 0 || cnt > max)
      return 0;
    void **elems = (void **)((uintptr_t)arr + ARRAY_FIRST_ELEMENT_OFFSET);
    int n = 0;
    for (int i = 0; i < cnt && n < max; i++) {
      if (elems[i])
        out[n++] = elems[i];
    }
    return n;
  } catch (...) {
    return 0;
  }
}

void *SafeGetLocalPlayer() {
  if (!g_PlayersInstance)
    return nullptr;
  try {
    return *(void **)((uintptr_t)g_PlayersInstance +
                      PLAYERS_LOCALPLAYER_OFFSET);
  } catch (...) {
    return nullptr;
  }
}

void *SafeFindGame(Il2CppDomain *dom) {
  if (!il2cpp_domain_get_assemblies || !il2cpp_assembly_get_image ||
      !il2cpp_class_from_name || !il2cpp_class_get_static_field_data)
    return nullptr;
  try {
    size_t cnt = 0;
    Il2CppAssembly **asms = il2cpp_domain_get_assemblies(dom, &cnt);
    for (size_t i = 0; i < cnt; i++) {
      Il2CppImage *img = il2cpp_assembly_get_image(asms[i]);
      if (!img)
        continue;
      Il2CppClass *cls =
          il2cpp_class_from_name(img, "Polytoria.Datamodel", "Game");
      if (!cls)
        continue;
      void *data = il2cpp_class_get_static_field_data(cls);
      if (!data)
        continue;
      void *s = *(void **)data;
      if (s)
        return s;
    }
  } catch (...) {
  }
  return nullptr;
}

void *SafeGetScriptService() {
  if (ScriptService_get_Instance) {
    try {
      return ScriptService_get_Instance();
    } catch (...) {
    }
  }
  return nullptr;
}

void *FindClassInAssemblies(const char *ns, const char *name) {
  size_t cnt = 0;
  auto asms = il2cpp_domain_get_assemblies(g_Domain, &cnt);
  for (size_t i = 0; i < cnt; i++) {
    auto img = il2cpp_assembly_get_image(asms[i]);
    if (!img)
      continue;
    auto cls = il2cpp_class_from_name(img, ns, name);
    if (cls)
      return cls;
  }
  return nullptr;
}

void *CreateNetworkWriter() {
  if (!g_NetworkWriterClass) {
    g_NetworkWriterClass = FindClassInAssemblies("Mirror", "NetworkWriter");
    if (!g_NetworkWriterClass)
      return nullptr;
  }
  void *writer = il2cpp_object_new(g_NetworkWriterClass);
  if (!writer)
    return nullptr;
  try {
    NW_ctor(writer);
  } catch (...) {
    return nullptr;
  }
  return writer;
}

int GetWriterPosition(void *writer) {
  try {
    return *(int *)((uintptr_t)writer + NW_POSITION_OFFSET);
  } catch (...) {
    return -1;
  }
}

bool ManualWriteBytes(void *writer, const void *data, int size) {
  try {
    void *buffer = *(void **)((uintptr_t)writer + NW_BUFFER_OFFSET);
    int pos = *(int *)((uintptr_t)writer + NW_POSITION_OFFSET);
    int capacity = *(int *)((uintptr_t)buffer + 0x18);
    if (pos + size > capacity)
      return false;
    memcpy((void *)((uintptr_t)buffer + 0x20 + pos), data, size);
    *(int *)((uintptr_t)writer + NW_POSITION_OFFSET) = pos + size;
    return true;
  } catch (...) {
    return false;
  }
}