# SA:MP Plugin Template Library

C++17 template library that allows you to create your own plugins for **SA:MP** server very easy and fast

## Main features
* Safe C++ AMX API with errors handling
* Queue of AMX scripts (gamemode at the end)
* Easy executing the callbacks (publics) with optional caching
* Easy registration of natives: auto-conversion parameters from cell type to common C++ types. You may also define your own conversions by extending Script::NativeParam struct
* Logging
* Optional checking for a version match between the plugin and scripts

## Example

```cpp
#include "samp-ptl/ptl.h"

class Script : public ptl::AbstractScript<Script> {
 public:
  // native ExampleNative(int_number, Float:float_number, &ref, const text[]);
  cell n_ExampleNative(int int_number, float float_number, cell *ref,
                       std::string text) {
    Log("int_number = %d, float_number = %.2f, text = '%s'", int_number,
        float_number, text.c_str());

    *ref = 23;

    return 1;
  }
};

class Plugin : public ptl::AbstractPlugin<Plugin, Script> {
 public:
  const char *Name() { return "samp-example-plugin"; }

  bool OnLoad() {
    RegisterNative<&Script::n_ExampleNative>("ExampleNative");

    Log("plugin loaded");

    return true;
  }
};

PLUGIN_EXPORT unsigned int PLUGIN_CALL Supports() {
  return SUPPORTS_VERSION | SUPPORTS_AMX_NATIVES;
}

PLUGIN_EXPORT bool PLUGIN_CALL Load(void **ppData) {
  return Plugin::DoLoad(ppData);
}

PLUGIN_EXPORT void PLUGIN_CALL Unload() {
  Plugin::DoUnload();
}

PLUGIN_EXPORT void PLUGIN_CALL AmxLoad(AMX *amx) {
  Plugin::DoAmxLoad(amx);
}

PLUGIN_EXPORT void PLUGIN_CALL AmxUnload(AMX *amx) {
  Plugin::DoAmxUnload(amx);
}
```

## More examples
[Simple plugin](https://github.com/katursis/samp-ptl/tree/master/example)

[Pawn.RakNet](https://github.com/katursis/Pawn.RakNet)

[Pawn.CMD](https://github.com/katursis/Pawn.CMD)

[Pawn.Regex](https://github.com/katursis/Pawn.Regex)
