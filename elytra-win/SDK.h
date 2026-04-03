#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <cstddef>
#include <cstdint>
#include <map>
#include <string>
#include <unordered_map>
#include <vector>

// ==================== IL2CPP Types ====================
typedef void *Il2CppString;
typedef void *Il2CppArray;
typedef void *Il2CppDomain;
typedef void *Il2CppThread;
typedef void *Il2CppClass;
typedef void *Il2CppImage;
typedef void *Il2CppAssembly;

// ==================== MoonSharp Types ====================
typedef void *MsScript;
typedef void *MsTable;
typedef void *MsDynValue;

// ==================== IL2CPP API Typedefs ====================
typedef Il2CppString *(__cdecl *il2cpp_string_new_t)(const char *str);
typedef Il2CppDomain *(__cdecl *il2cpp_domain_get_t)();
typedef Il2CppThread *(__cdecl *il2cpp_thread_attach_t)(Il2CppDomain *domain);
typedef void(__cdecl *il2cpp_thread_detach_t)(Il2CppThread *thread);
typedef Il2CppAssembly **(__cdecl *il2cpp_domain_get_assemblies_t)(
    Il2CppDomain *domain, size_t *count);
typedef Il2CppImage *(__cdecl *il2cpp_assembly_get_image_t)(
    Il2CppAssembly *assembly);
typedef Il2CppClass *(__cdecl *il2cpp_class_from_name_t)(Il2CppImage *image,
                                                         const char *namespaze,
                                                         const char *name);
typedef void *(__cdecl *il2cpp_class_get_static_field_data_t)(
    Il2CppClass *klass);
typedef void *(__cdecl *il2cpp_object_new_t)(void *klass);

// ==================== Game Function Typedefs ====================
typedef Il2CppArray *(__fastcall *Instance_GetChildren_t)(void *instance);
typedef Il2CppString *(__fastcall *Instance_get_Name_t)(void *instance);
typedef void(__fastcall *Instance_set_Name_t)(void *instance,
                                              Il2CppString *value);
typedef Il2CppString *(__fastcall *Instance_get_ClassName_t)(void *instance);
typedef void(__fastcall *Instance_Destroy_t)(void *instance, float time);
typedef void *(__fastcall *Instance_Clone_t)(void *instance);
typedef void *(__fastcall *Instance_New_t)(Il2CppString *className,
                                           void *parent);

typedef void(__fastcall *BaseScript_set_Source_t)(void *script,
                                                  Il2CppString *source);
typedef void(__fastcall *BaseScript_Run_t)(void *script);
typedef Il2CppString *(__fastcall *BaseScript_get_Source_t)(void *script);

typedef void *(__cdecl *ScriptService_get_Instance_t)();
typedef void(__fastcall *ScriptService_RunScript_t)(void *scriptService,
                                                    void *script);

typedef void(__fastcall *NetworkEvent_InvokeServer_t)(void *networkEvent,
                                                      void *msg);
typedef void(__fastcall *Plr_CmdSetHealth_t)(void *player, float health);
typedef uint32_t(__fastcall *NB_get_netId_t)(void *self);

// NetworkWriter typedefs
typedef void(__fastcall *NW_ctor_t)(void *self);
typedef void(__fastcall *NW_WriteByte_t)(void *self, uint8_t value);
typedef void(__fastcall *NW_WriteBlittableFloat_t)(void *self, float value);
typedef void(__fastcall *NW_WriteBlittableUInt_t)(void *self, uint32_t value);
typedef void(__fastcall *NW_WriteBlittableUShort_t)(void *self, uint16_t value);
typedef void(__fastcall *NW_WriteBool_t)(void *self, bool value);
typedef void(__fastcall *NW_WriteVector3_t)(void *self, void *vec3ptr);
typedef void(__fastcall *NW_WriteString_t)(void *self, void *str);
typedef void(__fastcall *SendCommandInternal_t)(void *self,
                                                Il2CppString *funcName,
                                                int functionHash, void *writer,
                                                int channelId,
                                                bool requireAuthority);

// KCP Typedefs
typedef void(__fastcall *KcpPeer_SendData_t)(void *self, void *segment,
                                             int channel);
typedef void(__fastcall *KcpClient_RawInput_t)(void *self, void *segment);
typedef void(__fastcall *KcpPeer_Disconnect_t)(void *self);
typedef void(__fastcall *KcpPeer_SendDisconnect_t)(void *self);
typedef void(__fastcall *KcpPeer_HandleTimeout_t)(void *self, uint32_t time);
typedef void(__fastcall *KcpPeer_HandleDeadLink_t)(void *self);
typedef void(__fastcall *KcpPeer_HandleChoked_t)(void *self);
typedef void(__fastcall *KcpTransport_ClientLateUpdate_t)(void *self);

// AntiKick Typedefs
typedef void(__fastcall *GenericVoid_t)(void *self);
typedef void(__fastcall *GenericVoidStr_t)(void *self, void *str);
typedef void(__fastcall *DelayedDisconnect_t)(void *self, void *conn,
                                              float delay);
typedef void(__fastcall *DeserializeClient_t)(void *self, void *reader,
                                              bool initialState);
typedef bool(__fastcall *ConnIsAlive_t)(void *self, float timeout);
typedef void(__fastcall *HandleRemoteCall_t)(void *self, uint8_t componentIndex,
                                             uint16_t functionHash,
                                             int remoteCallType, void *reader,
                                             void *conn);

// RPC Resolver
typedef bool(__cdecl *GetFunctionMethodName_t)(uint16_t hash,
                                               Il2CppString **outName);
typedef bool(__cdecl *CommandRequiresAuthority_t)(uint16_t hash);
typedef bool(__cdecl *GetInvoker_t)(uint16_t, int, void *);

// ==================== Offsets ====================
// Instance
constexpr uintptr_t INSTANCE_GETCHILDREN_OFFSET = 0x44AEF0;
constexpr uintptr_t INSTANCE_GET_NAME_OFFSET = 0x417800;
constexpr uintptr_t INSTANCE_SET_NAME_OFFSET = 0x44F800;
constexpr uintptr_t INSTANCE_GET_CLASSNAME_OFFSET = 0x2FDB30;
constexpr uintptr_t INSTANCE_DESTROY_OFFSET = 0x44A620;
constexpr uintptr_t INSTANCE_CLONE_OFFSET = 0x44A350;
constexpr uintptr_t INSTANCE_NEW_OFFSET = 0x44BE30;

// Scripts
constexpr uintptr_t BASESCRIPT_SET_SOURCE_OFFSET = 0x4238B0;
constexpr uintptr_t BASESCRIPT_RUN_OFFSET = 0x422EF0;
constexpr uintptr_t BASESCRIPT_GET_SOURCE_OFFSET = 0x417820;
constexpr uintptr_t SCRIPTSERVICE_GET_INSTANCE_OFFSET = 0x4B3B70;
constexpr uintptr_t SCRIPTSERVICE_RUNSCRIPT_OFFSET = 0x4B30B0;
constexpr uintptr_t BASESCRIPT_SOURCE_FIELD_OFFSET = 0x108;
constexpr uintptr_t BASESCRIPT_SCRIPT_OFFSET =
    0x110; // ScriptInstance -> MoonSharp Script* (PROBABLY)
constexpr uintptr_t SCRIPT_GLOBALTABLE_OFFSET =
    0x28; // Script -> Globals Table* (PROBABLY)

// Player & Net
constexpr uintptr_t PLR_CMDSETHEALTH_OFFSET = 0x470310;
constexpr uintptr_t PLAYERS_LOCALPLAYER_OFFSET = 0x108;
constexpr uintptr_t NETEVENT_INVOKESERVER_OFFSET = 0x45D800;
constexpr uintptr_t NB_GET_NETID_OFFSET = 0x1321FB0;
constexpr uintptr_t SEND_CMD_INTERNAL_OFFSET = 0x13200C0;

// NetworkWriter
constexpr uintptr_t NW_CTOR_OFFSET = 0x136B920;
constexpr uintptr_t NW_WRITEBYTE_OFFSET = 0x136B7E0;
constexpr uintptr_t NW_WRITE_FLOAT_OFFSET = 0x757A30;
constexpr uintptr_t NW_WRITE_INT_OFFSET = 0x755CD0;
constexpr uintptr_t NW_WRITE_UINT_OFFSET = 0x7578B0;
constexpr uintptr_t NW_WRITE_USHORT_OFFSET = 0x757860;
constexpr uintptr_t NW_WRITE_BOOL_OFFSET = 0x7557A0;
constexpr uintptr_t NW_WRITE_VEC3_OFFSET = 0x758150;
constexpr uintptr_t NW_WRITE_STRING_OFFSET = 0x757D20;
constexpr uintptr_t NW_BUFFER_OFFSET = 0x10;
constexpr uintptr_t NW_POSITION_OFFSET = 0x18;

// KCP
constexpr uintptr_t KCP_CLIENT_RAWINPUT_OFFSET = 0x1164330;
constexpr uintptr_t KCP_CLIENT_RAWSEND_OFFSET = 0x11648A0;
constexpr uintptr_t KCP_CLIENT_RAWRECV_OFFSET = 0x1164730;
constexpr uintptr_t KCP_PEER_SENDDATA_OFFSET = 0x1166160;
constexpr uintptr_t KCP_PEER_TICKINCOMING_OFFSET = 0x1167040;
constexpr uintptr_t KCP_PEER_DISCONNECT_OFFSET = 0x1164DA0;
constexpr uintptr_t KCP_PEER_HANDLEPING_OFFSET = 0x11653E0;
constexpr uintptr_t KCP_PEER_SENDPING_OFFSET = 0x11666B0;
constexpr uintptr_t KCP_PEER_HANDLETIMEOUT_OFFSET = 0x1165430;
constexpr uintptr_t KCP_PEER_TICKOUTGOING_OFFSET = 0x1167140;
constexpr uintptr_t KCP_PEER_SENDDISCONNECT_OFFSET = 0x1166290;
constexpr uintptr_t KCP_PEER_HANDLEDEADLINK_OFFSET = 0x1165310;
constexpr uintptr_t KCP_PEER_HANDLECHOKED_OFFSET = 0x1164E80;
constexpr uintptr_t KCP_TRANSPORT_CLIENT_EARLYUPDATE_OFFSET = 0x1342950;
constexpr uintptr_t KCP_TRANSPORT_CLIENT_LATEUPDATE_OFFSET = 0x13429A0;

// Telepathy
constexpr uintptr_t TELE_RECV_ENQUEUE_OFFSET = 0x18D4070;
constexpr uintptr_t TELE_SEND_ENQUEUE_OFFSET = 0x18D4FF0;
constexpr uintptr_t TELE_CLIENT_DISCONNECT_OFFSET = 0x18D2E90;
constexpr uintptr_t TELE_CLIENT_SEND_OFFSET = 0x18D3430;
constexpr uintptr_t TELE_CLIENT_TICK_OFFSET = 0x18D3740;
constexpr uintptr_t TELE_CLIENT_CONNECTED_OFFSET = 0x18D38F0;

// AntiKick / Disconnect
constexpr uintptr_t USERCODE_DISCONNECT_RPC_OFFSET = 0x480EA0;
constexpr uintptr_t PLAYER_DELAYED_DISCONNECT_OFFSET = 0x4708D0;
constexpr uintptr_t PTNETMGR_CLIENT_DISCONNECT_OFFSET = 0x432920;
constexpr uintptr_t SHOW_DISCONNECT_MSG_OFFSET = 0x50FE40;
constexpr uintptr_t PT_AUTH_DELAYED_DISC_OFFSET = 0x431CB0;
constexpr uintptr_t PLAYER_KICK_OFFSET = 0x476040;
constexpr uintptr_t PLAYERPROXY_KICK_OFFSET = 0x4CBDE0;
constexpr uintptr_t HANDLE_REMOTE_CALL_OFFSET = 0x132FFF0;
constexpr uintptr_t CONN_DISCONNECT_OFFSET = 0x132E7A0;
constexpr uintptr_t CONN_ISALIVE_OFFSET = 0x132EB70;
constexpr uintptr_t DESERIALIZE_CLIENT_OFFSET = 0x132FCB0;

// RPC Info
constexpr uintptr_t GET_FUNC_NAME_OFFSET = 0x136BF40;
constexpr uintptr_t CMD_REQUIRES_AUTH_OFFSET = 0x136BDB0;
constexpr uintptr_t GET_DELEGATE_OFFSET = 0x136BE90;
constexpr uintptr_t GET_INVOKER_OFFSET = 0x136C080;

// NetMessage
constexpr uintptr_t NETMSG_STRINGS_OFFSET = 0x10;
constexpr uintptr_t NETMSG_INTS_OFFSET = 0x18;
constexpr uintptr_t NETMSG_NUMBERS_OFFSET = 0x20;
constexpr uintptr_t NETMSG_BOOLS_OFFSET = 0x28;
constexpr uintptr_t NETMSG_VEC2S_OFFSET = 0x30;
constexpr uintptr_t NETMSG_VEC3S_OFFSET = 0x38;

// Common IL2CPP Array/String
constexpr uintptr_t ARRAY_LENGTH_OFFSET = 0x18;
constexpr uintptr_t ARRAY_FIRST_ELEMENT_OFFSET = 0x20;
constexpr uintptr_t STRING_LENGTH_OFFSET = 0x10;
constexpr uintptr_t STRING_CHARS_OFFSET = 0x14;

// ==================== Globals (Extern) ====================
extern HMODULE g_GameAssembly;
extern HMODULE g_Module;

// IL2CPP
extern il2cpp_string_new_t il2cpp_string_new;
extern il2cpp_domain_get_t il2cpp_domain_get;
extern il2cpp_thread_attach_t il2cpp_thread_attach;
extern il2cpp_thread_detach_t il2cpp_thread_detach;
extern il2cpp_domain_get_assemblies_t il2cpp_domain_get_assemblies;
extern il2cpp_assembly_get_image_t il2cpp_assembly_get_image;
extern il2cpp_class_from_name_t il2cpp_class_from_name;
extern il2cpp_class_get_static_field_data_t il2cpp_class_get_static_field_data;
extern il2cpp_object_new_t il2cpp_object_new;

// Game Functions
extern Instance_GetChildren_t Instance_GetChildren;
extern Instance_get_Name_t Instance_get_Name;
extern Instance_set_Name_t Instance_set_Name;
extern Instance_get_ClassName_t Instance_get_ClassName;
extern Instance_Destroy_t Instance_Destroy;
extern Instance_Clone_t Instance_Clone;
extern Instance_New_t Instance_New;
extern BaseScript_set_Source_t BaseScript_set_Source;
extern BaseScript_get_Source_t BaseScript_get_Source;
extern BaseScript_Run_t BaseScript_Run;
extern ScriptService_get_Instance_t ScriptService_get_Instance;
extern ScriptService_RunScript_t ScriptService_RunScript;
extern Plr_CmdSetHealth_t Plr_CmdSetHealth;
extern NB_get_netId_t NB_get_netId;
extern SendCommandInternal_t SendCmdInternal;

// NetworkWriter
extern NW_ctor_t NW_ctor;
extern NW_WriteByte_t NW_WriteByte;
extern NW_WriteBlittableFloat_t NW_WriteFloat;
extern NW_WriteBlittableUInt_t NW_WriteUInt;
extern NW_WriteBlittableUShort_t NW_WriteUShort;
extern NW_WriteBool_t NW_WriteBool;
extern NW_WriteVector3_t NW_WriteVector3;
extern NW_WriteString_t NW_WriteString;

// RPC Info
extern GetFunctionMethodName_t GetFunctionMethodName;
extern CommandRequiresAuthority_t CommandRequiresAuthority;

// Global Objects
extern Il2CppDomain *g_Domain;
extern Il2CppThread *g_Thread;
extern void *g_GameSingleton;
extern void *g_ScriptService;
extern void *g_PlayersInstance;
extern void *g_NetworkWriterClass;

bool InitIL2CPP();
bool AttachIL2CPP();
bool SafeReadString(Il2CppString *str, char *out, int size);
bool SafeGetName(void *inst, char *out, int size);
bool SafeGetClassName(void *inst, char *out, int size);
int SafeGetChildren(void *inst, void **out, int max);
void *SafeGetLocalPlayer();
void *SafeFindGame(Il2CppDomain *dom);
void *SafeGetScriptService();
void *FindClassInAssemblies(const char *ns, const char *name);
void *CreateNetworkWriter();
int GetWriterPosition(void *writer);
bool ManualWriteBytes(void *writer, const void *data, int size);