# occ_vtk_examples 系统结构 / System Architecture

## 1. 概览 Overview
应用以 Qt Widgets 为基础，整合 QVTKOpenGLNativeWidget、VTK 9.3 渲染管线与 OpenCascade 几何构造。系统按照“Manifest → Registry → UI → Example → VTK/OCC”链路组织，确保示例可配置、可扩展且 UI 与渲染逻辑解耦。

## 2. 高层架构 High-level Architecture
```
┌────────────────────────────────────────────────┐
│                    Manifest                    │
│ data/examples_manifest.json                    │
└──────────────┬─────────────────────────────────┘
               │ loadManifest()
┌──────────────▼──────────────┐        ┌───────────────────────────────┐
│        ExampleRegistry       │        │         MainWindow UI         │
│ - factories (class→factory) │◄──────►│ - Category tree / example list│
│ - ExampleDefinition cache   │        │ - Parameter form / log tabs   │
└──────────────┬──────────────┘        │ - QVTKOpenGLNativeWidget      │
               │ createExample(id)     └──────────────┬────────────────┘
┌──────────────▼──────────────┐                     run()
│        Example (virtual)    │─────────────────────────────────────────┐
│ - metadata()                │                                         │
│ - parameters()              │                                         │
│ - run(params, context)      │                                         │
└──────────────┬──────────────┘                                         │
               │ ExampleContext                                        │
┌──────────────▼──────────────┐        ┌───────────────────────────────┐
│ VTK / OCC Rendering Objects │        │ QVTKOpenGLNativeWidget + UI   │
│ Renderer, Actors, Mappers   │◄──────►│ update(), interactor(), etc.  │
└─────────────────────────────┘        └───────────────────────────────┘
```

## 3. Example 框架 Framework
- **Example 基类** (`examples/Example.h`): 定义 `ExampleMetadata`（标题、分类、标签、数据路径）、`ExampleParameter`（类型、标签、默认值、范围、可选项）以及 `run()` 接口。  
- **ExampleContext**：封装 `vtkRenderer`、`vtkGenericOpenGLRenderWindow`、`QVTKOpenGLNativeWidget` 以及数据路径解析方法，提供 `clearScene()`、`resolveDataPath()` 等便捷方法，确保示例代码无需直接操作 UI。
- **实现类**：位于 `examples/occ/` 与 `examples/vtk/`，通过 `ExampleRegistrar` 注册工厂 lambda（参见例如 `examples/vtk/VtkVolumeRenderingExample.cpp`）。示例仅负责创建 source/mapper/actor 并向上下文渲染器添加内容。

### 参数系统
`ExampleParameterType` 支持 Double / Integer / Boolean / Choice / FilePath。MainWindow 根据参数类型生成 QDoubleSpinBox、QSpinBox、QCheckBox、QComboBox 或带浏览按钮的 QLineEdit 容器。执行时通过 `gatherParameters()` 收集参数值传递给示例。

## 4. Manifest 设计 Manifest Schema
文件：`data/examples_manifest.json`
```jsonc
{
  "categories": [
    {"id": "vtk_basics", "title": "VTK 基础", "description": "…"},
    ...
  ],
  "examples": [
    {
      "id": "vtk_volume",
      "class": "VtkVolumeRenderingExample",
      "category": "vtk_rendering",
      "title": "体绘制",
      "description": "…",
      "tags": ["vtk", "volume"],
      "dataPath": "Ex04.vtk",
      "thumbnail": "...",
      "documentation": "https://..."
    }
  ]
}
```
- **categories**：用于构建 UI 树；若 manifest 未声明但示例指定了新的分类，Registry 会创建“合成”分类。  
- **examples**：最小字段 `id`/`class`/`category`。其它字段覆盖 `ExampleMetadata` 默认值。`dataPath` 定义示例默认资源。

## 5. UI 布局与交互 UI Layout
- **Splitter**：左侧为导航列，右侧为 3D 视图 + Tab 面板。  
- **导航列**：包含搜索框 (`QLineEdit`)、分类树 (`QTreeWidget`)、示例列表 (`QListWidget`) 和“运行示例”按钮。  
- **内容区**：上方标题标签，下方 `QVTKOpenGLNativeWidget`，底部为 Tab（参数、说明、日志）。参数 Tab 由 `QScrollArea + QFormLayout` 构成，自动填入各参数的编辑器。  
- **交互流程**：选中分类 → 列表展示该分类示例 → 选中后刷新标题、说明和参数 → 点击“运行”或双击列表项即执行，对应日志写入时间与状态。

## 6. 渲染流程 Rendering Flow
1. **选择示例**：MainWindow 从 `ExampleRegistry` 获取 `ExampleDefinition`，存储参数描述。  
2. **参数采集**：UI 控件写入 `QVariantMap`。  
3. **ExampleContext 初始化**：封装主 renderer、渲染窗口与 widget。  
4. **执行 `run()`**：示例调用 `context.clearScene()` 清空画布，构造数据源、Mapper、Actor/Volume/Widget，添加到 renderer，并触发渲染。  
5. **更新视图**：MainWindow 调用 `renderWindow->Render()` 与 `openGLWidget->update()`，确保 UI 刷新。  
6. **日志记录**：运行结果追加到 `txtLog`，包含 HH:mm:ss 时间戳。

## 7. 数据与资源 Data Resources
- 体绘制、ScalarBar 等示例依赖 `data/*.vtk`。`ExampleContext::resolveDataPath()` 会在运行时将相对路径转换为可执行文件附近的实际路径。  
- MainWindow 在构造时通过 `locateDataDirectory()` 搜索多个候选位置（`./data`, `../data`, `$prefix/share/occvtkex/data` 等），便于安装部署。

## 8. 工具与扩展 Tooling & Extensibility
- **脚手架**（`tools/create_example.py`）：自动生成头/源文件与 manifest 条目，减少模板重复；生成的类预置 `ExampleRegistrar` 与参数模板。  
- **目录结构**：`examples/` 下可新增 `occ` / `vtk` / 其他子目录，CMake 通过显式列表编译；如需动态扩展，可在未来引入 `file(GLOB)` 或插件式加载。  
- **UI 扩展点**：可添加截图、标签筛选、参数预设保存等；参数系统已支持 Choice/FilePath，可进一步扩展到颜色、向量等自定义控件。

## 9. 未来规划 Suggested Enhancements
1. **示例参数持久化**：保存最近一次参数配置，重启后恢复。  
2. **运行结果缩略图**：在 manifest 中添加缩略图路径并在列表中显示。  
3. **GPU 体绘制/Shader 示例**：利用 VTK GPU Volume 与自定义 shader 展示更复杂效果。  
4. **多语言 UI**：当前文案为中文，可通过 Qt 翻译文件提供多语言支持。  
5. **单元测试**：针对 Registry 和参数构建逻辑编写 Qt Test/GoogleTest，确保 manifest 解析稳定。
