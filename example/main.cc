/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2020-2021 katursis
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

#define _GLIBCXX_USE_CXX11_ABI 0

#include "../ptl.h"

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
  const char *Name() { return "samp-ptl-example-plugin"; }

  bool OnLoad() {
    RegisterNative<&Script::n_ExampleNative>("ExampleNative");

    Log("plugin loaded");

    return true;
  }
};

PLUGIN_EXPORT bool PLUGIN_CALL Load(void **ppData) {
  return Plugin::DoLoad(ppData);
}

PLUGIN_EXPORT void PLUGIN_CALL Unload() { Plugin::DoUnload(); }

PLUGIN_EXPORT void PLUGIN_CALL AmxLoad(AMX *amx) { Plugin::DoAmxLoad(amx); }

PLUGIN_EXPORT void PLUGIN_CALL AmxUnload(AMX *amx) { Plugin::DoAmxUnload(amx); }

PLUGIN_EXPORT unsigned int PLUGIN_CALL Supports() {
  return SUPPORTS_VERSION | SUPPORTS_AMX_NATIVES;
}
