/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2020 urShadow
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

#include "../ptl.h"

#include "Pawn.Example.inc"

class Script : public ptl::AbstractScript<Script> {
 public:
  const char *VarVersion() { return "_pawn_example_version"; }

  bool OnLoad() {
    // your code
    return true;
  }
};

class Plugin : public ptl::AbstractPlugin<Plugin, Script> {
 public:
  const char *Name() { return "Pawn.Example"; }

  int Version() { return PAWN_EXAMPLE_VERSION; }

  bool OnLoad() {
    RegisterNative<n_PrintText>("PrintText");

    Log("plugin loaded");

    return true;
  }

  void OnUnload() { Log("plugin unloaded"); }

  static cell n_PrintText(Script *script, std::string text) {
    Plugin::Instance().Log("PrintText('%s');", text.c_str());

    return 1;
  }
};

PLUGIN_EXPORT unsigned int PLUGIN_CALL Supports() {
  return SUPPORTS_VERSION | SUPPORTS_AMX_NATIVES;
}

PLUGIN_EXPORT bool PLUGIN_CALL Load(void **ppData) {
  return Plugin::Instance().DoLoad(ppData);
}

PLUGIN_EXPORT void PLUGIN_CALL Unload() { Plugin::Instance().DoUnload(); }

PLUGIN_EXPORT void PLUGIN_CALL AmxLoad(AMX *amx) {
  Plugin::Instance().DoAmxLoad(amx);
}

PLUGIN_EXPORT void PLUGIN_CALL AmxUnload(AMX *amx) {
  Plugin::Instance().DoAmxUnload(amx);
}
