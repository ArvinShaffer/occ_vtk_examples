# occ_vtk_examples · OCC & VTK 示例集

## 项目简介 · Project Overview
基于 Qt 5 + VTK 9 的跨平台示例集合，用于演示 OpenCascade（OCC）与 VTK 渲染集成、体绘制、标注、光照以及数据管线等高级特性。应用通过 QVTKOpenGLNativeWidget 呈现 3D 视图，并提供可视化导航、搜索、参数化运行与日志输出，便于教学与快速验证。  
This repository contains a Qt 5 + VTK 9 showcase that demonstrates how to combine OpenCascade (OCC) geometry and VTK rendering techniques. The GUI hosts a QVTKOpenGLNativeWidget, navigation tree, searchable example list, parameter panes, and logs so you can explore rendering tricks, data IO, and OCC primitives interactively.

## 主要特性 · Key Features
- **模块化示例**：每个示例实现 `Example` 抽象类，具备元数据、参数定义与 `run()` 实现，可热插拔。  
  **Modular examples**: each demo inherits from `Example`, exposing metadata, parameter schemas, and a `run()` body.
- **Manifest 驱动**：`data/examples_manifest.json` 描述分类、顺序与素材，启动时自动加载。  
  **Manifest driven**: categories and examples are configured via `data/examples_manifest.json`.
- **UI 导航体验**：左侧树+卡片组合，支持搜索、双击运行与参数即时刷新。  
  **Rich navigation**: tree + card based explorer with search, double-click run, and live parameter panes.
- **OCC & VTK 并行示范**：涵盖 OCC 盒体、VTK 几何/光照/体绘制/标注/视口布局等。  
  **Broad coverage**: includes OCC primitives plus VTK geometry, lighting, volume rendering, annotation, and viewports.
- **示例脚手架**：`tools/create_example.py` 一键生成模板及 manifest 项。  
  **Scaffolding**: `tools/create_example.py` seeds new example classes and manifest entries.

## 环境依赖 · Requirements
- Qt 5.15.x (tested with 5.15.2) · Qt build with QtWidgets + OpenGL
- VTK 9.3（含 RenderingOpenGL2, GUISupportQt, RenderingVolume 等组件）  
- OpenCascade 7.9.1 头文件与库（可自定义路径）  
- CMake ≥ 3.16, Ninja 或 Make, C++17 编译器

## 构建与运行 · Build & Run
```bash
# 配置 · Configure
cmake -S . -B build \
  -DQt5_DIR=/path/to/Qt/5.15.2/gcc_64/lib/cmake/Qt5 \
  -DVTK_DIR=/path/to/lib/cmake/vtk-9.3

# 构建 · Build
cmake --build build -j$(nproc)

# 运行 · Launch (make sure LD_LIBRARY_PATH points to the same Qt used at link time)
cd build && ./occvtkex
```

## 示例系统 · Example System
- **数据目录**：应用启动时自动定位 `data/` 目录；若自定义安装，可将该目录复制到可执行文件同级或 `$prefix/share/occvtkex/data`。  
- **分类与示例**：`examples_manifest.json` 由 `categories[]` 与 `examples[]` 组成，示例项至少包含 `id`、`class`、`category`。  
- **参数面板**：每个示例返回 `ExampleParameter` 列表（类型包含 Double/Integer/Boolean/Choice/FilePath），UI 自动生成对应控件。  
- **日志**：底部“日志”页签记录运行时间与结果，便于调试。

## 添加新示例 · Add a New Example
1. 运行脚手架：  
   ```bash
   tools/create_example.py \
     --class-name MyCoolExample \
     --example-id vtk_my_cool_example \
     --category vtk_rendering \
     --title "酷炫示例" \
     --description "自定义描述"
   ```
2. 在生成的 `examples/<group>/MyCoolExample.{h,cpp}` 中补充渲染逻辑、参数。  
3. 按需编辑 `data/examples_manifest.json`（脚手架已添加基础条目，可扩展 tags、dataPath 等字段）。  
4. 重新构建运行，示例会自动出现在 UI 对应分类中。

## 目录结构 · Directory Layout
```
├── CMakeLists.txt          # 项目配置
├── data/                   # Manifest 与 VTK 示例数据
├── examples/               # Example 基类 + OCC/VTK 示例实现
├── src/main.cpp            # Qt 应用入口
├── mainwindow.{h,cpp,ui}   # 主界面、导航、参数逻辑
├── tools/create_example.py # 示例脚手架
└── build/                  # (可选) CMake 构建输出
```

## 常见问题 · Troubleshooting
- **Qt 版本不匹配**：确保链接与运行的 Qt 版本一致，可通过 `LD_LIBRARY_PATH` 或 `Qt5_DIR` 调整。  
- **Manifest 未找到**：确认 `data/examples_manifest.json` 与可执行文件相对位置正确，或设置 `data/` 到安装位置。

## 许可 · License
若原仓库未附带许可证，请参考上游许可或根据使用场景补充。  
If the original repository does not provide a license, please refer to upstream licensing terms before redistribution.
