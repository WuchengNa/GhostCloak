# GhostCloak - Windows 视觉夹层工具

**让任何窗口"隐身"于屏幕之下 | 真正的视觉欺骗工具**

## 📋 目录

1. [项目简介](#项目简介)
2. [核心特点](#核心特点)
3. [快速开始](#快速开始)
4. [项目结构](#项目结构)
5. [构建指南](#构建指南)
6. [使用方法](#使用方法)
7. [开发文档](#开发文档)
8. [系统要求](#系统要求)

---

## 项目简介

GhostCloak 是一款极轻量、极纯净的 Windows 视觉欺骗工具，专为需要"表面只显示一个程序，实际上同时运行多个程序"的场景设计。

它在你的屏幕上创造一个肉眼均难以察觉的"夹层"：
- **最上层** - GhostCloak（几乎完全透明、可精确操作）
- **中间层** - 你想隐藏的目标程序（正常运行、可接收全部鼠标键盘）
- **底层** - 你希望展示给别人的画面（IDE、桌面、办公软件……）

当目标程序最小化或隐藏时，GhostCloak 瞬间抓取底层画面进行像素级完美补位，视觉上完全看不出任何异常，仿佛屏幕上永远只有最底层的那个程序在运行。

### 适用场景

- 🎮 游戏多开隐身
- 📹 直播隐藏脚本/聊天框
- 💼 工作摸鱼伪装
- 🔒 隐私窗口保护
- 📊 防肩窥、防录屏检测

---

## 核心特点

✅ **极轻极纯**
- 单文件 < 90KB，无依赖、无安装
- 纯原生 Win32 + GDI+ 实现

✅ **完美适配**
- 任意 DPI 下像素 1:1 对齐（125%~300% 完美）
- 启动快、内存占用极低

✅ **易用交互**
- 可拖拽、可缩放的透明交互孔，孔内完全点击穿透
- 右键抓取背景、滚轮调节透明度、ESC 一键退出
- 背景图自动缓存，下次启动直接恢复伪装状态

✅ **现代化工程**
- 模块化代码结构（8 个功能明确的类）
- CMake 构建系统 + CMakePresets 预设配置
- 支持 x86/x64 多架构输出
- 支持多种 IDE 开发

---

## 快速开始

### VS Code 开发（推荐）

安装 **CMake Tools** 扩展后：
1. Ctrl+Shift+P → **CMake: Select Configure Preset** → 选择预设（如 `win x64 Release`）
2. Ctrl+Shift+P → **CMake: Configure**
3. Ctrl+Shift+P → **CMake: Build** 或 Ctrl+Shift+B
4. F5 → 调试运行

### 命令行

```bash
# 使用 CMake Presets 构建（推荐）
cmake --preset "win x64 Release"      # 配置 x64 Release
cmake --build --preset "x64 Release"   # 编译

cmake --preset "win x86 Debug"         # 配置 x86 Debug
cmake --build --preset "x86 Debug"     # 编译
```

### Visual Studio 集成

1. Visual Studio 2022 → **文件** → **打开** → **文件夹**
2. 选择 GhostCloak 项目文件夹
3. 等待 CMake 自动配置完成（自动识别 CMakePresets.json）
4. 点击 **生成** 菜单编译

---

## 项目结构

### 文件组织

```
GhostCloak/
├── include/                       # 头文件（8个类）
│   ├── AppState.h                # 全局应用状态
│   ├── ConfigManager.h           # 配置管理
│   ├── BackgroundManager.h       # 背景截图管理
│   ├── UIManager.h               # UI菜单管理
│   ├── AutoClickManager.h        # 自动连点管理
│   ├── GDIPlusHelper.h           # GDI+绘制助手
│   ├── WindowHelper.h            # 窗口辅助功能
│   └── DragManager.h             # 拖拽管理
│
├── src/                           # 源文件（8个类 + main）
│   ├── main.cpp                  # 主程序入口
│   ├── ConfigManager.cpp
│   ├── BackgroundManager.cpp
│   ├── UIManager.cpp
│   ├── AutoClickManager.cpp
│   ├── GDIPlusHelper.cpp
│   ├── WindowHelper.cpp
│   └── DragManager.cpp
│
├── CMakeLists.txt                # 主 CMake 配置
├── CMakePresets.json             # CMake 预设（x86/x64 + Debug/Release）
├── .gitignore                    # Git 忽略列表
│
├── build_win64/                  # x64 构建缓存（自动生成）
├── build_win64_debug/            # x64 Debug 构建缓存
├── build_win32/                  # x86 构建缓存
├── build_win32_debug/            # x86 Debug 构建缓存
│
└── output/                       # 编译输出（自动生成）
    ├── bin/                      # x86 可执行文件
    │   ├── Debug/GhostCloak.exe
    │   └── Release/GhostCloak.exe
    ├── bin_x64/                  # x64 可执行文件
    │   ├── Debug/GhostCloak.exe
    │   └── Release/GhostCloak.exe
    ├── lib/                      # x86 库文件
    └── lib_x64/                  # x64 库文件
```

### 核心类说明

| 类名 | 职责 | 主要方法 |
|------|------|----------|
| **AppState** | 全局应用状态 | 状态字段集合 |
| **ConfigManager** | 配置加载/保存 | LoadConfig(), SaveConfig() |
| **BackgroundManager** | 背景截图管理 | GrabBackground(), RestoreBackground(), ResetBackground() |
| **UIManager** | 右键菜单交互 | ShowContextMenu() |
| **AutoClickManager** | 自动连点功能 | StartTimer(), PerformClick(), ResetTimer() |
| **DragManager** | 拖拽操作管理 | StartDrag(), UpdateDrag(), EndDrag() |
| **GDIPlusHelper** | 图形绘制 | RenderWindow() 静态方法 |
| **WindowHelper** | 窗口辅助操作 | InitDPI(), PtInRect(), SetTransparency() |

---

## 构建指南

### 系统要求

| 项目 | 要求 | 备注 |
|------|------|------|
| 操作系统 | Windows 10+ | 仅 Windows 支持 |
| Visual Studio | 2022 或更新 | Community 及以上版本 |
| CMake | 3.16+ | [下载链接](https://cmake.org/download/) |
| Windows SDK | 10.0.22621+ | 通常随 VS 安装 |
| C++ 编译器 | MSVC 2022 | 支持 C++17 |

### CMake Presets 预设

项目通过 `CMakePresets.json` 提供了 4 个配置预设：

| 配置预设 | 架构 | 类型 | 构建缓存目录 | 输出目录 |
|----------|------|------|-------------|----------|
| `win x64 Release` | x64 | Release | `build_win64/` | `output/bin_x64/Release/` |
| `win x64 Debug` | x64 | Debug | `build_win64_debug/` | `output/bin_x64/Debug/` |
| `win x86 Release` | x86 | Release | `build_win32/` | `output/bin/Release/` |
| `win x86 Debug` | x86 | Debug | `build_win32_debug/` | `output/bin/Debug/` |

### 编译输出位置

成功编译后的可执行文件按架构和配置分目录输出：

```
output/
  bin/                    ← x86 输出
    Debug/GhostCloak.exe
    Release/GhostCloak.exe
  bin_x64/                ← x64 输出
    Debug/GhostCloak.exe
    Release/GhostCloak.exe
```

### 构建命令

```bash
# x64 Release（推荐）
cmake --preset "win x64 Release"
cmake --build --preset "x64 Release"

# x64 Debug
cmake --preset "win x64 Debug"
cmake --build --preset "x64 Debug"

# x86 Release
cmake --preset "win x86 Release"
cmake --build --preset "x86 Release"

# x86 Debug
cmake --preset "win x86 Debug"
cmake --build --preset "x86 Debug"
```

### 清理构建文件

```bash
# 删除所有构建缓存和编译输出
rmdir /s build_win64
rmdir /s build_win64_debug
rmdir /s build_win32
rmdir /s build_win32_debug
rmdir /s output
```

### 依赖库

项目自动链接以下 Windows SDK 库（无需手动安装）：

| 库文件 | 用途 |
|--------|------|
| `gdiplus.lib` | GDI+ 图形绘制 |
| `user32.lib` | Windows 用户界面 |
| `gdi32.lib` | GDI 库 |
| `shlwapi.lib` | Shell 轻量级实用程序 |
| `shell32.lib` | Shell 库 |

### CMake 配置选项

```bash
# 自定义架构后缀（默认 x64 预设为 "_x64"，x86 预设为空）
cmake --preset "win x64 Release" -DARCH_SUFFIX="_custom"

# 设置 C++ 标准版本（默认 C++17）
cmake --preset "win x64 Release" -DCMAKE_CXX_STANDARD=20
```

---

## 使用方法

### 快捷操作

| 操作 | 功能 |
|------|------|
| **右键点击** | 打开菜单（抓取背景、重置、自动连点开关、退出） |
| **滚轮向上** | 增加窗口透明度（+5%） |
| **滚轮向下** | 降低窗口透明度（-5%） |
| **ESC 键** | 立即退出应用 |
| **拖拽孔洞** | 移动交互区域位置 |
| **拖拽边角** | 调整交互区域大小 |
| **拖拽点** | 移动自动连点位置 |

### 工作流

1. **启动应用** - 创建透明覆盖层
2. **调整位置和大小** - 拖拽交互孔洞到目标窗口位置
3. **抓取背景** - 右键 → "抓取背景"，捕获底层屏幕
4. **最小化目标程序** - 隐藏要隐藏的窗口
5. **完成伪装** - 屏幕显示干净的背景，实际可通过孔洞操作隐藏程序

### 自动连点功能

- 在孔洞内随机选取坐标
- 定时间隔（1-20秒随机）
- 支持启用/禁用切换
- 通过右键菜单控制

---

## 开发文档

### 项目演进历程

```
单文件 main.cpp (1300+行)
    ↓
按功能拆分成 8 个模块化类
    ↓
修复编译问题和依赖
    ↓
集成 CMake 构建系统
    ↓
CMakePresets 预设 + 多架构输出 ✓
```

### 模块化优势

✅ **清晰职责** - 每个类专注于一个业务模块
✅ **高内聚** - 相关功能集中在同一类中
✅ **低耦合** - 模块间依赖最小化
✅ **易维护** - 修改或新增功能无需影响其他模块
✅ **易测试** - 每个模块可独立测试
✅ **代码复用** - 各模块可在其他项目中复用

### 构建系统变化

| 方面 | 旧系统 | 新系统 (CMake + Presets) |
|------|--------|------------------------|
| 配置方式 | 批处理脚本 | CMakePresets.json 预设 |
| 架构支持 | 仅 x64 | x86 / x64 自动区分输出 |
| IDE 集成 | 仅命令行 | VS / VS Code / CLion 等 |
| 依赖管理 | 手动指定 Findgdiplus.cmake | 直接链接 Windows SDK 库 |
| 输出组织 | build/bin/Debug|Release | output/bin|bin_x64/Debug|Release |
| 编译优化 | 固定参数 | 灵活配置 |

---

## 故障排除

### CMake 找不到编译器

**症状**：CMake Error: No CMAKE_CXX_COMPILER could be found

**解决**：确保已安装 Visual Studio 2022 且 CMake 能检测到，或手动指定生成器：
```bash
cmake --preset "win x64 Release"
```

### 找不到 GDI+ 头文件

**症状**：fatal error C1083: Cannot open include file: 'gdiplus.h'

**解决**：确保已安装 Windows SDK（通常随 Visual Studio 安装），在 Visual Studio Installer 中勾选 "Windows 10/11 SDK" 组件。

### 链接错误

**症状**：error LNK2001: 无法解析的外部符号

**解决**：检查 CMakeLists.txt 中的 `target_link_libraries` 是否包含所有必需库

### 编译警告关于编码

**原因**：某些源文件包含中文字符

**解决**：VS 会自动处理，或将文件另存为 UTF-8 with BOM

---

## 进阶配置

### 开发模式推荐

```bash
# 配置 x64 Debug 预设
cmake --preset "win x64 Debug"

# 快速增量编译
cmake --build --preset "x64 Debug" --parallel 4
```

### 发布模式推荐

```bash
# 配置 x64 Release 预设
cmake --preset "win x64 Release"

# 完整优化编译
cmake --build --preset "x64 Release"
```

### 与版本控制集成

项目已配置 `.gitignore`，包含：
- CMake 缓存和生成文件（`build_*/`）
- 编译输出目录（`output/`）
- IDE 配置和用户文件
- 编译产物和调试符号

---

## 后续改进计划

### 即期目标
- [ ] 在 Visual Studio 2022 中完整测试编译
- [ ] 验证所有功能正常运行
- [ ] 检查并消除编译警告

### 中期目标
- [ ] 添加单元测试框架
- [ ] 集成代码质量检查工具（CppCheck、Clang-Tidy）
- [ ] 配置 CI/CD 流程（GitHub Actions）
- [ ] 自动文档生成（Doxygen）

### 长期目标
- [ ] 生成安装程序（CPack）
- [ ] 版本号自动管理（Git 标签）
- [ ] 跨平台适配（Linux/macOS）
- [ ] 更多高级功能

---

## 相关资源

- 📚 [CMake 官方文档](https://cmake.org/cmake/help/latest/)
- 📚 [CMake Presets 文档](https://cmake.org/cmake/help/latest/manual/cmake-presets.7.html)
- 📚 [Visual Studio 开发文档](https://docs.microsoft.com/en-us/visualstudio/)
- 📚 [C++ 参考资源](https://en.cppreference.com/)
- 📚 [GDI+ 官方文档](https://docs.microsoft.com/en-us/windows/win32/gdiplus/)
- 🐙 [CMake 最佳实践](https://github.com/cpp-best-practices/cppbestpractices)

---

## 许可和版权

- **原始代码**：main.cpp（已备份）
- **备份文件**：bak.zip
- **当前版本**：v1.0

---

## 技术栈

- **语言**：C++17
- **框架**：Win32 API + GDI+
- **构建**：CMake 3.16+ / CMakePresets
- **编译器**：MSVC 2022
- **IDE 支持**：Visual Studio、VS Code、CLion

---

## 总结

GhostCloak 项目现在是一个**现代化、模块化、易维护**的 C++ Windows 应用程序：

✨ **优雅的代码结构** - 8 个功能明确的类  
🔨 **灵活的构建系统** - CMake Presets + 多架构输出  
📚 **完善的文档** - 详细的指南和说明  
🚀 **开发友好** - 支持多种 IDE 和工作流  

---

**GhostCloak，让你想藏的窗口彻底消失在所有人视野里，却依然在你指尖完美运行。**

*最后更新：2025年11月26日*
