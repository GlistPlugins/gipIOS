# gipIOS

iOS platform plugin for [GlistEngine](https://github.com/GlistEngine/GlistEngine). It provides the iOS windowing, app lifecycle and prebuilt dependencies (assimp, freetype, curl, openssl) needed to build a GlistEngine app for iOS.

## Setup

1. Clone this plugin under `glist/glistplugins`, next to the engine:
   - `glist/GlistEngine`
   - `glist/glistplugins/gipIOS`
   - `glist/myglistapps/GlistApp`
2. Add `gipIOS` to the `PLUGINS` variable in your app's root `CMakeLists.txt`:
   ```cmake
   set(PLUGINS gipIOS)
   ```

The plugin also ships a prebuilt host copy of the engine's `ShaderToHeader` tool in `tools/`, which the app's `CMakeLists.txt` uses so shader headers can be generated while cross-compiling for iOS.

## Building

iOS builds are driven from the app project, not from this plugin. See [GlistApp's `_macos/README.md`](https://github.com/glistengine/glistapp/blob/main/_macos/README.md) for the build, signing and troubleshooting steps.
