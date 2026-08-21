# gipIOS

iOS platform plugin for [GlistEngine](https://github.com/GlistEngine/GlistEngine). It provides the iOS windowing, app lifecycle and prebuilt dependencies (assimp, freetype, curl, openssl) needed to build a GlistEngine app for iOS.

## Setup

Clone this plugin under `glist/glistplugins`, next to the engine:

- `glist/GlistEngine`
- `glist/glistplugins/gipIOS`
- `glist/myglistapps/GlistApp`

That's all, GlistEngine adds `gipIOS` automatically for iOS builds, so you don't need to list it in `PLUGINS`.

The plugin also ships a prebuilt host copy of the engine's `ShaderToHeader` tool in `tools/`, which the app's `CMakeLists.txt` uses so shader headers can be generated while cross-compiling for iOS.

## Building

iOS builds are driven from the app project, not from this plugin. See [GlistApp's `_apple/README.md`](https://github.com/glistengine/glistapp/blob/main/_apple/README.md) for the build, signing and troubleshooting steps.

## Vulkan (MoltenVK)

GlistEngine can render through Vulkan on iOS. There is no Vulkan driver on the platform, so MoltenVK translates it to Metal; this section is about getting that library in place and telling the engine to use it. Without it the plugin simply builds without the Vulkan backend, and an app asking for Vulkan falls back to OpenGL.

**1. Get MoltenVK.** It is a few dozen megabytes of static archives, so it is not part of this repository and has to be fetched once per machine. Take `MoltenVK-all.tar` from the [MoltenVK releases](https://github.com/KhronosGroup/MoltenVK/releases) - the smaller `MoltenVK-ios.tar` carries only the `ios-arm64` device slice and cannot build for the simulator.

**2. Put it where CMake looks**, under `libs/moltenvk/<version>`:

```text
libs/moltenvk/1.4.2/include/vulkan/vulkan.h
libs/moltenvk/1.4.2/lib/ios-arm64/libMoltenVK.a
libs/moltenvk/1.4.2/lib/ios-arm64_x86_64-simulator/libMoltenVK.a
```

From the extracted tarball that is `MoltenVK/MoltenVK/include` for the headers and `MoltenVK/MoltenVK/static/MoltenVK.xcframework/<slice>/libMoltenVK.a` for each library:

```bash
tar -xf MoltenVK-all.tar
MVK=~/dev/glist/glistplugins/gipIOS/libs/moltenvk/1.4.2
mkdir -p "$MVK/lib"
cp -R MoltenVK/MoltenVK/include "$MVK/include"
for slice in ios-arm64 ios-arm64_x86_64-simulator; do
    cp -R "MoltenVK/MoltenVK/static/MoltenVK.xcframework/$slice" "$MVK/lib/$slice"
done
```

CMake picks the slice that matches the platform being built: `ios-arm64` for a device, `ios-arm64_x86_64-simulator` for the simulator. If you already have MoltenVK elsewhere - the LunarG Vulkan SDK for macOS ships one - point at it with `VULKAN_SDK` or `MOLTENVK_HOME`, or configure with `-DGLIST_MOLTENVK_LIBRARY=` and `-DGLIST_MOLTENVK_INCLUDE_DIR=`. Those take precedence over `libs/moltenvk`.

**3. Check the configure output.** It prints exactly one of these:

```text
-- iOS Vulkan enabled with MoltenVK: <path to libMoltenVK.a>
-- iOS MoltenVK not configured; Vulkan backend disabled
```

The second line means the app is being built OpenGL-only and will stay on OpenGL no matter what it asks for at runtime.

**4. Ask for the renderer** where your app passes the render engine in `main.cpp`: `G_RENDERER_VK` instead of `G_RENDERER_GL`.

Two things the platform does not give you. On the simulator the swapchain only offers FIFO presentation, so the frame rate is capped at the display refresh rate and `disableVsync()` cannot lift it - use the simulator to check that a scene renders, and a device to judge cost. And there are no Vulkan validation layers on iOS; Xcode's Metal frame capture is the equivalent tool, and MoltenVK's own `MVK_CONFIG_*` environment variables turn up its logging.
