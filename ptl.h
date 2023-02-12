/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2020-2023 katursis
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef PTL_H_
#define PTL_H_

#include <algorithm>
#include <functional>
#include <list>
#include <memory>
#include <sstream>
#include <tuple>

#include "amx/amx.h"
#include "plugincommon.h"

#define PACK_PLUGIN_VERSION(major, minor, patch) \
  (((major) << 16) | ((minor) << 8) | (patch))

namespace ptl {  // Plugin Template Library
using LogPrintf = void (*)(const char *fmt, ...);

class Amx {
 public:
  Amx(AMX *amx, void *amx_functions, bool log_amx_errors, LogPrintf logprintf,
      const std::string &plugin_name)
      : amx_{amx},
        amx_functions_{amx_functions},
        log_amx_errors_{log_amx_errors},
        logprintf_{logprintf},
        plugin_name_{plugin_name} {}

  uint16_t *Align16(uint16_t *v) {
    return Call<PLUGIN_AMX_EXPORT_Align16, false, uint16_t *>(v);
  }

  uint32_t *Align32(uint32_t *v) {
    return Call<PLUGIN_AMX_EXPORT_Align32, false, uint32_t *>(v);
  }

#if defined _I64_MAX || defined HAVE_I64
  uint64_t *Align64(uint64_t *v) {
    return Call<PLUGIN_AMX_EXPORT_Align64, false, uint64_t *>(v);
  }
#endif

  template <bool raise_error = true>
  int Allot(int cells, cell *amx_addr, cell **phys_addr) {
    return Call<PLUGIN_AMX_EXPORT_Allot, raise_error>(amx_, cells, amx_addr,
                                                      phys_addr);
  }

  template <bool raise_error = true>
  int Callback(cell index, cell *result, cell *params) {
    return Call<PLUGIN_AMX_EXPORT_Callback, raise_error>(amx_, index, result,
                                                         params);
  }

  template <bool raise_error = true>
  int Cleanup() {
    return Call<PLUGIN_AMX_EXPORT_Cleanup, raise_error>(amx_);
  }

  template <bool raise_error = true>
  int Clone(AMX *amx_clone, void *data) {
    return Call<PLUGIN_AMX_EXPORT_Clone, raise_error>(amx_clone, amx_, data);
  }

  template <bool raise_error = true>
  int Exec(cell *retval, int index, const std::string &debug_args_values = "") {
    int result = Call<PLUGIN_AMX_EXPORT_Exec, false>(amx_, retval, index);

    if constexpr (raise_error) {
      if (log_amx_errors_ && result != AMX_ERR_NONE) {
        Log(StrError(result) + " in public " + GetPublicName(index) + "(" +
            debug_args_values +
            ") - please note that the AMX error is not related with the "
            "plugin, but your script");
      }
    }

    return result;
  }

  template <bool raise_error = true>
  int FindNative(const char *name, int *index) {
    return Call<PLUGIN_AMX_EXPORT_FindNative, raise_error>(amx_, name, index);
  }

  template <bool raise_error = true>
  int FindPublic(const char *funcname, int *index) {
    return Call<PLUGIN_AMX_EXPORT_FindPublic, raise_error>(amx_, funcname,
                                                           index);
  }

  template <bool raise_error = true>
  int FindPubVar(const char *varname, cell *amx_addr) {
    return Call<PLUGIN_AMX_EXPORT_FindPubVar, raise_error>(amx_, varname,
                                                           amx_addr);
  }

  template <bool raise_error = true>
  int FindTagId(cell tag_id, char *tagname) {
    return Call<PLUGIN_AMX_EXPORT_FindTagId, raise_error>(amx_, tag_id,
                                                          tagname);
  }

  template <bool raise_error = true>
  int Flags(uint16_t *flags) {
    return Call<PLUGIN_AMX_EXPORT_Flags, raise_error>(amx_, flags);
  }

  template <bool raise_error = true>
  int GetAddr(cell amx_addr, cell **phys_addr) {
    return Call<PLUGIN_AMX_EXPORT_GetAddr, raise_error>(amx_, amx_addr,
                                                        phys_addr);
  }

  template <bool raise_error = true>
  int GetNative(int index, char *funcname) {
    return Call<PLUGIN_AMX_EXPORT_GetNative, raise_error>(amx_, index,
                                                          funcname);
  }

  template <bool raise_error = true>
  int GetPublic(int index, char *funcname) {
    return Call<PLUGIN_AMX_EXPORT_GetPublic, raise_error>(amx_, index,
                                                          funcname);
  }

  template <bool raise_error = true>
  int GetPubVar(int index, char *varname, cell *amx_addr) {
    return Call<PLUGIN_AMX_EXPORT_GetPubVar, raise_error>(amx_, index, varname,
                                                          amx_addr);
  }

  template <bool raise_error = true>
  int GetString(char *dest, const cell *source, int use_wchar,
                std::size_t size) {
    return Call<PLUGIN_AMX_EXPORT_GetString, raise_error>(dest, source,
                                                          use_wchar, size);
  }

  template <bool raise_error = true>
  int GetTag(int index, char *tagname, cell *tag_id) {
    return Call<PLUGIN_AMX_EXPORT_GetTag, raise_error>(amx_, index, tagname,
                                                       tag_id);
  }

  template <bool raise_error = true>
  int GetUserData(long tag, void **ptr) {
    return Call<PLUGIN_AMX_EXPORT_GetUserData, raise_error>(amx_, tag, ptr);
  }

  template <bool raise_error = true>
  int Init(void *program) {
    return Call<PLUGIN_AMX_EXPORT_Init, raise_error>(amx_, program);
  }

  template <bool raise_error = true>
  int InitJIT(void *reloc_table, void *native_code) {
    return Call<PLUGIN_AMX_EXPORT_InitJIT, raise_error>(amx_, reloc_table,
                                                        native_code);
  }

  template <bool raise_error = true>
  int MemInfo(long *codesize, long *datasize, long *stackheap) {
    return Call<PLUGIN_AMX_EXPORT_MemInfo, raise_error>(amx_, codesize,
                                                        datasize, stackheap);
  }

  template <bool raise_error = true>
  int NameLength(int *length) {
    return Call<PLUGIN_AMX_EXPORT_NameLength, raise_error>(amx_, length);
  }

  AMX_NATIVE_INFO *NativeInfo(const char *name, AMX_NATIVE func) {
    return Call<PLUGIN_AMX_EXPORT_NativeInfo, false, AMX_NATIVE_INFO *>(name,
                                                                        func);
  }

  template <bool raise_error = true>
  int NumNatives(int *number) {
    return Call<PLUGIN_AMX_EXPORT_NumNatives, raise_error>(amx_, number);
  }

  template <bool raise_error = true>
  int NumPublics(int *number) {
    return Call<PLUGIN_AMX_EXPORT_NumPublics, raise_error>(amx_, number);
  }

  template <bool raise_error = true>
  int NumPubVars(int *number) {
    return Call<PLUGIN_AMX_EXPORT_NumPubVars, raise_error>(amx_, number);
  }

  template <bool raise_error = true>
  int NumTags(int *number) {
    return Call<PLUGIN_AMX_EXPORT_NumTags, raise_error>(amx_, number);
  }

  template <bool raise_error = true>
  int Push(cell value) {
    return Call<PLUGIN_AMX_EXPORT_Push, raise_error>(amx_, value);
  }

  template <bool raise_error = true>
  int PushArray(cell *amx_addr, cell **phys_addr, const cell array[],
                int numcells) {
    return Call<PLUGIN_AMX_EXPORT_PushArray, raise_error>(
        amx_, amx_addr, phys_addr, array, numcells);
  }

  template <bool raise_error = true>
  int PushString(cell *amx_addr, cell **phys_addr, const char *string, int pack,
                 int use_wchar) {
    return Call<PLUGIN_AMX_EXPORT_PushString, raise_error>(
        amx_, amx_addr, phys_addr, string, pack, use_wchar);
  }

  template <bool raise_error = true>
  int RaiseError(int error) {
    return Call<PLUGIN_AMX_EXPORT_RaiseError, raise_error>(amx_, error);
  }

  template <bool raise_error = true>
  int Register(const AMX_NATIVE_INFO *nativelist, int number) {
    return Call<PLUGIN_AMX_EXPORT_Register, raise_error>(amx_, nativelist,
                                                         number);
  }

  template <bool raise_error = true>
  int Release(cell amx_addr) {
    return Call<PLUGIN_AMX_EXPORT_Release, raise_error>(amx_, amx_addr);
  }

  template <bool raise_error = true>
  int SetCallback(AMX_CALLBACK callback) {
    return Call<PLUGIN_AMX_EXPORT_SetCallback, raise_error>(amx_, callback);
  }

  template <bool raise_error = true>
  int SetDebugHook(AMX_DEBUG debug) {
    return Call<PLUGIN_AMX_EXPORT_SetDebugHook, raise_error>(amx_, debug);
  }

  template <bool raise_error = true>
  int SetString(cell *dest, const char *source, int pack, int use_wchar,
                std::size_t size) {
    return Call<PLUGIN_AMX_EXPORT_SetString, raise_error>(dest, source, pack,
                                                          use_wchar, size);
  }

  template <bool raise_error = true>
  int SetUserData(long tag, void *ptr) {
    return Call<PLUGIN_AMX_EXPORT_SetUserData, raise_error>(amx_, tag, ptr);
  }

  template <bool raise_error = true>
  int StrLen(const cell *cstring, int *length) {
    return Call<PLUGIN_AMX_EXPORT_StrLen, raise_error>(cstring, length);
  }

  template <bool raise_error = true>
  int UTF8Check(const char *string, int *length) {
    return Call<PLUGIN_AMX_EXPORT_UTF8Check, raise_error>(string, length);
  }

  template <bool raise_error = true>
  int UTF8Get(const char *string, const char **endptr, cell *value) {
    return Call<PLUGIN_AMX_EXPORT_UTF8Get, raise_error>(string, endptr, value);
  }

  template <bool raise_error = true>
  int UTF8Len(const cell *cstr, int *length) {
    return Call<PLUGIN_AMX_EXPORT_UTF8Len, raise_error>(cstr, length);
  }

  template <bool raise_error = true>
  int UTF8Put(char *string, char **endptr, int maxchars, cell value) {
    return Call<PLUGIN_AMX_EXPORT_UTF8Put, raise_error>(string, endptr,
                                                        maxchars, value);
  }

  inline AMX *GetPtr() const { return amx_; }

  template <PLUGIN_AMX_EXPORT func, bool raise_error = true, typename Ret = int,
            typename... Args>
  inline Ret Call(Args... args) {
    Ret result = reinterpret_cast<Ret(AMXAPI **)(Args...)>(
        amx_functions_)[func](args...);

    if constexpr (raise_error && std::is_same<int, Ret>::value) {
      if (log_amx_errors_ && result != AMX_ERR_NONE) {
        Log(StrError(result) + " in amx_" + StrFunction(func) + "(" +
            DumpArgs(args...) + ")");
      }
    }

    return result;
  }

  inline std::string GetPublicName(int index) {
    int len{};
    NameLength(&len);

    std::unique_ptr<char[]> name{new char[len + 1]{}};
    GetPublic(index, name.get());

    return name.get();
  }

  template <typename T, typename... Args>
  inline std::string DumpArgs(T arg1, Args... args) {
    std::stringstream ss;

    ss << AddQuotesIfStr(arg1);
    ((ss << ", " << AddQuotesIfStr(args)), ...);

    return ss.str();
  }

  template <typename T>
  inline auto AddQuotesIfStr(T arg) {
    if constexpr (std::is_same<T, const char *>::value ||
                  std::is_same<T, char *>::value) {
      return "\"" + std::string(arg) + "\"";
    } else if constexpr (std::is_same<typename std::decay<T>::type,
                                      std::string>::value) {
      return "\"" + arg + "\"";
    } else {
      return arg;
    }
  }

  inline const std::string StrFunction(PLUGIN_AMX_EXPORT func) {
    static const std::string functions[] = {
        "Align16",      "Align32",    "Align64",     "Allot",
        "Callback",     "Cleanup",    "Clone",       "Exec",
        "FindNative",   "FindPublic", "FindPubVar",  "FindTagId",
        "Flags",        "GetAddr",    "GetNative",   "GetPublic",
        "GetPubVar",    "GetString",  "GetTag",      "GetUserData",
        "Init",         "InitJIT",    "MemInfo",     "NameLength",
        "NativeInfo",   "NumNatives", "NumPublics",  "NumPubVars",
        "NumTags",      "Push",       "PushArray",   "PushString",
        "RaiseError",   "Register",   "Release",     "SetCallback",
        "SetDebugHook", "SetString",  "SetUserData", "StrLen",
        "UTF8Check",    "UTF8Get",    "UTF8Len",     "UTF8Put",
    };

    if (func < 0 || func >= (sizeof(functions) / sizeof(functions[0]))) {
      return "(unknown function, " + std::to_string(func) + ")";
    }

    return functions[func];
  }

  inline const std::string StrError(int errnum) {
    static const std::string messages[] = {
        "(none)",                                          // AMX_ERR_NONE
        "Forced exit",                                     // AMX_ERR_EXIT
        "Assertion failed",                                // AMX_ERR_ASSERT
        "Stack/heap collision (insufficient stack size)",  // AMX_ERR_STACKERR
        "Array index out of bounds",                       // AMX_ERR_BOUNDS
        "Invalid memory access",                           // AMX_ERR_MEMACCESS
        "Invalid instruction",                             // AMX_ERR_INVINSTR
        "Stack underflow",                                 // AMX_ERR_STACKLOW
        "Heap underflow",                                  // AMX_ERR_HEAPLOW
        "No (valid) native function callback",             // AMX_ERR_CALLBACK
        "Native function failed",                          // AMX_ERR_NATIVE
        "Divide by zero",                                  // AMX_ERR_DIVIDE
        "(sleep mode)",                                    // AMX_ERR_SLEEP
        "Invalid state for this access",                   // AMX_ERR_INVSTATE
        "(reserved)",                                      // 14
        "(reserved)",                                      // 15
        "Out of memory",                                   // AMX_ERR_MEMORY
        "Invalid/unsupported P-code file format",          // AMX_ERR_FORMAT
        "File is for a newer version of the AMX",          // AMX_ERR_VERSION
        "File or function is not found",                   // AMX_ERR_NOTFOUND
        "Invalid index parameter (bad entry point)",       // AMX_ERR_INDEX
        "Debugger cannot run",                             // AMX_ERR_DEBUG
        "AMX not initialized (or doubly initialized)",     // AMX_ERR_INIT
        "Unable to set user data field (table full)",      // AMX_ERR_USERDATA
        "Cannot initialize the JIT",                       // AMX_ERR_INIT_JIT
        "Parameter error",                                 // AMX_ERR_PARAMS
    };

    if (errnum < 0 || errnum >= (sizeof(messages) / sizeof(messages[0]))) {
      return "(unknown error, " + std::to_string(errnum) + ")";
    }

    return messages[errnum];
  }

  template <typename... Args>
  void Log(const std::string &fmt, Args... args) {
    if (!logprintf_) {
      throw std::runtime_error{"logprintf_ is null"};
    }

    if (plugin_name_.empty()) {
      logprintf_(fmt.c_str(), args...);
    } else {
      logprintf_(("[%s] " + fmt).c_str(), plugin_name_.c_str(), args...);
    }
  }

 private:
  AMX *amx_{};
  void *amx_functions_{};

  LogPrintf logprintf_{};
  std::string plugin_name_;
  bool log_amx_errors_{};
};

class Public {
 public:
  Public(const std::string &name, const std::shared_ptr<Amx> &amx,
         bool use_caching = false)
      : amx_{amx}, name_{name}, use_caching_{use_caching} {
    exists_ = amx_->FindPublic<false>(name_.c_str(), &index_) == AMX_ERR_NONE &&
              index_ >= 0;
  }

  template <typename... Args>
  inline cell Exec(Args... args) {
    cell retval{};

    if (use_caching_) {
      if (!cached_) {
        amx_->FindPublic(name_.c_str(), &index_);

        cached_ = true;
      }
    } else {
      amx_->FindPublic(name_.c_str(), &index_);
    }

    std::string debug_args_values = "";

    if constexpr (sizeof...(Args) != 0) {
      Push(args...);

      debug_args_values = amx_->DumpArgs(args...);
    }

    amx_->Exec(&retval, index_, debug_args_values);

    if (amx_addr_to_release_) {
      amx_->Release(amx_addr_to_release_);

      amx_addr_to_release_ = 0;
    }

    return retval;
  }

  inline bool Exists() const { return exists_; }

  inline const std::string &GetName() { return name_; }

  template <typename T, typename... Args>
  inline void Push(T arg1, Args... args) {
    Push(args...);

    Push(arg1);
  }

  template <typename T>
  inline void Push(T arg) {
    if constexpr (std::is_pointer<T>::value) {
      if constexpr (std::is_same<T, const char *>::value ||
                    std::is_same<T, char *>::value) {
        cell amx_addr{};

        amx_->PushString(&amx_addr, nullptr, arg, 0, 0);

        if (!amx_addr_to_release_) {
          amx_addr_to_release_ = amx_addr;
        }
      } else {
        amx_->Push(reinterpret_cast<cell>(arg));
      }
    } else if constexpr (std::is_floating_point<T>::value) {
      amx_->Push(amx_ftoc(arg));
    } else if constexpr (std::is_same<typename std::decay<T>::type,
                                      std::string>::value) {
      Push(arg.c_str());
    } else {
      amx_->Push(static_cast<cell>(arg));
    }
  }

 private:
  std::shared_ptr<Amx> amx_;
  std::string name_;
  int index_{};
  bool exists_{};
  bool cached_{};
  bool use_caching_{};

  cell amx_addr_to_release_{};
};

template <typename ScriptT>
class AbstractScript {
 public:
  struct NativeParam {
    operator cell() { return raw_value; }

    operator cell *() { return script.GetPhysAddr(raw_value); }

    operator float() { return amx_ctof(raw_value); }

    operator float *() {
      return reinterpret_cast<float *>(script.GetPhysAddr(raw_value));
    }

    operator std::string() { return script.GetString(raw_value); }

    cell raw_value{};
    ScriptT &script{};
  };

  template <typename T>
  inline T PrepareNativeParam(cell value) {
    return T{value, static_cast<ScriptT &>(*this)};
  }

  bool IsGamemode() const { return is_gamemode_; }

  std::string GetPublicName(int index) { return amx_->GetPublicName(index); }

  template <typename T = cell>
  T GetPublicVarValue(const char *name) {
    cell amx_addr{};
    amx_->FindPubVar(name, &amx_addr);

    return static_cast<T>(*GetPhysAddr(amx_addr));
  }

  bool PublicVarExists(const char *name) {
    cell unused{};
    return amx_->FindPubVar<false>(name, &unused) == AMX_ERR_NONE;
  }

  std::string GetString(cell amx_addr) {
    cell *addr = GetPhysAddr(amx_addr);

    int len{};
    amx_->StrLen(addr, &len);

    std::unique_ptr<char[]> str{new char[len + 1]{}};
    amx_->GetString(str.get(), addr, 0, len + 1);

    return str.get();
  }

  void SetString(cell *dest, const std::string &src, std::size_t size) {
    amx_->SetString(dest, src.c_str(), 0, 0, size);
  }

  cell *GetPhysAddr(cell amx_addr) {
    cell *phys_addr{};

    amx_->GetAddr(amx_addr, &phys_addr);

    return phys_addr;
  }

  void RegisterNative(const char *name, AMX_NATIVE func) {
    amx_->Register<false>(amx_->NativeInfo(name, func), 1);
  }

  auto MakePublic(const std::string &name, bool use_caching = false) {
    return std::make_shared<Public>(name, amx_, use_caching);
  }

  const char *VarVersion() { return nullptr; };

  const char *VarIsGamemode() { return nullptr; }

  bool OnLoad() { return true; }

  void Init(AMX *amx, void *amx_functions, bool log_amx_errors,
            LogPrintf logprintf, const std::string &plugin_name) {
    impl_ = static_cast<ScriptT *>(this);

    logprintf_ = logprintf;

    plugin_name_ = plugin_name;

    amx_ = std::make_shared<Amx>(amx, amx_functions, log_amx_errors, logprintf,
                                 plugin_name);

    if (impl_->VarIsGamemode() && PublicVarExists(impl_->VarIsGamemode())) {
      is_gamemode_ = GetPublicVarValue<bool>(impl_->VarIsGamemode());
    }
  }

  const auto &GetAmx() const { return amx_; }

  bool HasVersion() {
    return impl_->VarVersion() && PublicVarExists(impl_->VarVersion());
  }

  int GetVersion() { return GetPublicVarValue<int>(impl_->VarVersion()); }

  inline void AssertParams(std::size_t count, cell *params) const {
    if (static_cast<ucell>(params[0]) != (count * sizeof(cell))) {
      throw std::runtime_error{"Number of parameters must be equal to " +
                               std::to_string(count)};
    }
  }

  inline void AssertMinParams(std::size_t min_count, cell *params) const {
    if (static_cast<ucell>(params[0]) < (min_count * sizeof(cell))) {
      throw std::runtime_error{"Number of parameters must be >= " +
                               std::to_string(min_count)};
    }
  }

  template <typename... Args>
  void Log(const std::string &fmt, Args... args) {
    if (!logprintf_) {
      throw std::runtime_error{"logprintf_ is null"};
    }

    if (plugin_name_.empty()) {
      logprintf_(fmt.c_str(), args...);
    } else {
      logprintf_(("[%s] " + fmt).c_str(), plugin_name_.c_str(), args...);
    }
  }

  inline bool operator==(AMX *amx) { return amx_->GetPtr() == amx; }

 protected:
  std::shared_ptr<Amx> amx_;
  bool is_gamemode_{};

  LogPrintf logprintf_{};
  std::string plugin_name_;

 private:
  ScriptT *impl_{};
};

class DummyScript : public AbstractScript<DummyScript> {};

template <typename PluginT, typename ScriptT = DummyScript,
          typename NativeParamT = typename ScriptT::NativeParam>
class AbstractPlugin {
 public:
  static PluginT &Instance() {
    static PluginT instance;

    return instance;
  }

  static bool DoLoad(void **ppData) { return Instance().DoLoadImpl(ppData); }

  static void DoUnload() { Instance().DoUnloadImpl(); }

  static void DoAmxLoad(AMX *amx) { Instance().DoAmxLoadImpl(amx); }

  static void DoAmxUnload(AMX *amx) { Instance().DoAmxUnloadImpl(amx); }

  static void DoProcessTick() { Instance().DoProcessTickImpl(); }

  static ScriptT &GetScript(AMX *amx) { return Instance().GetScriptImpl(amx); }

  static bool EveryScript(
      std::function<bool(const std::shared_ptr<ScriptT> &)> func) {
    return Instance().EveryScriptImpl(func);
  }

  static std::string GetNativeName(AMX_NATIVE func) {
    return Instance().GetNativeNameImpl(func);
  }

  template <typename... Args>
  static void Log(const std::string &fmt, Args... args) {
    Instance().LogImpl(fmt, args...);
  }

  static std::tuple<int, int, int> VersionToTuple(int version) {
    return Instance().VersionToTupleImpl(version);
  }

  int Version() { return PACK_PLUGIN_VERSION(1, 0, 0); };  // 1.0.0

  const char *Name() { return typeid(PluginT).name(); };

  bool LogAmxErrors() { return true; };

  bool OnLoad() {
    Log("plugin v%s loaded", VersionAsString().c_str());

    return true;
  }

  void OnUnload() { Log("plugin unloaded"); }

  void OnProcessTick() {}

  std::string VersionAsString() const { return VersionToString(version_); }

  template <auto func, bool expand_params = true>
  void RegisterNative(const char *name) {
    if constexpr (std::is_member_function_pointer<decltype(func)>::value) {
      natives_[name] =
          NativeGenerator<decltype(func), func, expand_params>::Native;
    } else {
      natives_[name] = NativeGenerator<
          typename std::add_pointer<
              typename std::remove_pointer<decltype(func)>::type>::type,
          func, expand_params>::Native;
    }
  }

 protected:
  template <typename Sig, Sig, bool>
  struct NativeGenerator;

  template <typename... Args, auto func, bool expand_params>
  struct NativeGenerator<cell (*)(ScriptT &, Args...), func, expand_params> {
    template <std::size_t... index>
    inline static cell Call(ScriptT &script, cell *params,
                            std::index_sequence<index...>) {
      return func(script, script.template PrepareNativeParam<NativeParamT>(
                              params[index + 1])...);
    }

    static cell AMX_NATIVE_CALL Native(AMX *amx, cell *params) {
      try {
        auto &script = PluginT::GetScript(amx);

        if constexpr (expand_params) {
          script.AssertParams(sizeof...(Args), params);

          return Call(script, params,
                      std::make_index_sequence<sizeof...(Args)>{});
        } else {
          return func(script, params);
        }
      } catch (const std::exception &e) {
        PluginT::Log("%s: %s", PluginT::GetNativeName(Native).c_str(),
                     e.what());
      }

      return 0;
    }
  };

  template <typename... Args, auto func, bool expand_params>
  struct NativeGenerator<cell (ScriptT::*)(Args...), func, expand_params> {
    template <std::size_t... index>
    inline static cell Call(ScriptT &script, cell *params,
                            std::index_sequence<index...>) {
      return (script.*func)(script.template PrepareNativeParam<NativeParamT>(
          params[index + 1])...);
    }

    static cell AMX_NATIVE_CALL Native(AMX *amx, cell *params) {
      try {
        auto &script = PluginT::GetScript(amx);

        if constexpr (expand_params) {
          script.AssertParams(sizeof...(Args), params);

          return Call(script, params,
                      std::make_index_sequence<sizeof...(Args)>{});
        } else {
          return (script.*func)(params);
        }
      } catch (const std::exception &e) {
        PluginT::Log("%s: %s", PluginT::GetNativeName(Native).c_str(),
                     e.what());
      }

      return 0;
    }
  };

  AbstractPlugin() = default;
  AbstractPlugin(const AbstractPlugin &) = delete;
  AbstractPlugin(AbstractPlugin &&) = delete;
  AbstractPlugin &operator=(const AbstractPlugin &) = delete;
  AbstractPlugin &operator=(AbstractPlugin &&) = delete;

  inline bool DoLoadImpl(void **ppData) {
    plugin_data_ = ppData;

    logprintf_ =
        reinterpret_cast<LogPrintf>(plugin_data_[PLUGIN_DATA_LOGPRINTF]);

    impl_ = static_cast<PluginT *>(this);

    try {
      name_ = impl_->Name();
      version_ = impl_->Version();

      Log("plugin v%s loading...", VersionAsString().c_str());

      bool loaded = impl_->OnLoad();

      log_amx_errors_ = impl_->LogAmxErrors();

      return loaded;
    } catch (const std::exception &e) {
      Log("%s: %s", __func__, e.what());
    }

    return false;
  }

  inline void DoUnloadImpl() {
    try {
      impl_->OnUnload();
    } catch (const std::exception &e) {
      Log("%s: %s", __func__, e.what());
    }
  }

  inline void DoAmxLoadImpl(AMX *amx) {
    try {
      auto script = std::make_shared<ScriptT>();

      script->Init(amx, plugin_data_[PLUGIN_DATA_AMX_EXPORTS], log_amx_errors_,
                   logprintf_, name_);

      if (script->HasVersion() && script->GetVersion() != version_) {
        throw std::runtime_error{"Mismatch between the plugin (" +
                                 VersionToString(version_) + ") and include (" +
                                 VersionToString(script->GetVersion()) +
                                 ") versions"};
      }

      for (auto &[native_name, native_func] : natives_) {
        script->RegisterNative(native_name.c_str(), native_func);
      }

      if (!script->OnLoad()) {
        return;
      }

      bool gamemode_exists =
          std::any_of(scripts_.begin(), scripts_.end(),
                      [](const auto &script) { return script->IsGamemode(); });

      if (script->IsGamemode()) {
        scripts_.push_back(script);

        if (gamemode_exists) {
          throw std::runtime_error{
              "Warning! You probably forgot to define FILTERSCRIPT in one "
              "of your filterscripts"};
        }
      } else {
        if (scripts_.size() && gamemode_exists) {
          scripts_.insert(std::prev(scripts_.end()), script);
        } else {
          scripts_.push_back(script);
        }
      }
    } catch (const std::exception &e) {
      Log("%s: %s", __func__, e.what());
    }
  }

  inline void DoAmxUnloadImpl(AMX *amx) {
    auto script = std::find_if(scripts_.begin(), scripts_.end(),
                               [amx](auto &script) { return *script == amx; });

    if (script != scripts_.end()) {
      scripts_.erase(script);
    }
  }

  inline void DoProcessTickImpl() {
    try {
      impl_->OnProcessTick();
    } catch (const std::exception &e) {
      Log("%s: %s", __func__, e.what());
    }
  }

  inline ScriptT &GetScriptImpl(AMX *amx) {
    auto script = std::find_if(scripts_.begin(), scripts_.end(),
                               [amx](auto &script) { return *script == amx; });

    if (script == scripts_.end()) {
      throw std::runtime_error{"Script not found"};
    }

    return **script;
  }

  inline bool EveryScriptImpl(
      std::function<bool(const std::shared_ptr<ScriptT> &)> func) {
    for (const auto &script : scripts_) {
      try {
        if (!func(script)) {
          return false;
        }
      } catch (const std::exception &e) {
        LogImpl("%s: %s", __func__, e.what());
      }
    }

    return true;
  }

  inline std::string GetNativeNameImpl(AMX_NATIVE func) {
    for (auto &[native_name, native_func] : natives_) {
      if (func == native_func) {
        return native_name;
      }
    }

    return "(unknown native)";
  }

  template <typename... Args>
  inline void LogImpl(const std::string &fmt, Args... args) {
    if (!logprintf_) {
      throw std::runtime_error{"logprintf_ is null"};
    }

    if (name_.empty()) {
      logprintf_(fmt.c_str(), args...);
    } else {
      logprintf_(("[%s] " + fmt).c_str(), name_.c_str(), args...);
    }
  }

  inline std::string VersionToString(int version) const {
    auto [major, minor, patch] = VersionToTupleImpl(version);

    std::stringstream ss;

    ss << major << "." << minor << "." << patch;

    return ss.str();
  }

  inline std::tuple<int, int, int> VersionToTupleImpl(int version) const {
    int major = (version >> 16) & 0xFF;
    int minor = (version >> 8) & 0xFF;
    int patch = version & 0xFF;

    return std::make_tuple(major, minor, patch);
  }

  std::list<std::shared_ptr<ScriptT>> scripts_;
  std::unordered_map<std::string, AMX_NATIVE> natives_;

  void **plugin_data_{};
  LogPrintf logprintf_{};
  bool log_amx_errors_{};

  std::string name_;
  int version_{};

 private:
  PluginT *impl_{};
};
}  // namespace ptl
#endif  // PTL_H_
