# Qt 登录界面设计文档

**日期：** 2026-06-10  
**状态：** 已批准  
**项目：** QtLoginDemo

## 概述

开发一个 macOS 原生风格的 Qt 登录界面，包含用户名输入框、密码输入框、登录按钮和状态提示标签。使用本地模拟验证，正确账号为 `admin` / `123456`。登录成功后状态标签显示提示，窗口保持打开。

## 需求摘要

| 项目 | 决定 |
|------|------|
| 技术栈 | Qt Widgets + C++ |
| 构建系统 | CMake |
| 目标平台 | macOS（原生控件风格） |
| UI 语言 | 中文 |
| 登录成功后 | 状态标签显示成功提示，窗口保持打开 |
| 验证方式 | 本地硬编码，无网络请求 |

## 架构

```
QtLoginDemo/
├── CMakeLists.txt
├── src/
│   ├── main.cpp              # 应用入口，macOS 原生风格
│   ├── LoginWindow.h/.cpp    # 登录窗口 UI + 交互
│   └── AuthValidator.h/.cpp  # 本地账号验证
└── build/                    # 构建输出（gitignore）
```

### 职责划分

- **main.cpp** — 创建 `QApplication`，不强制跨平台主题，让 macOS 使用系统原生外观；实例化并显示 `LoginWindow`
- **LoginWindow** — 构建 UI 布局，处理用户交互，调用验证器，更新状态标签
- **AuthValidator** — 纯静态验证逻辑，比对用户名和密码

## UI 布局

### 窗口属性

- 标题：`登录`
- 固定尺寸：360×200，不可缩放
- 启动时居中显示

### 布局结构

使用 `QVBoxLayout` 包裹 `QFormLayout`（表单区）+ 按钮 + 状态标签，自上而下排列：

```
┌─────────────────────────────┐
│           登录              │
├─────────────────────────────┤
│  用户名  [____________]     │
│  密码    [••••••••••••]     │
│                             │
│         [   登  录   ]      │
│                             │
│  请输入用户名和密码          │
└─────────────────────────────┘
```

### 控件明细

| 控件 | 类型 | 配置 |
|------|------|------|
| 用户名 | `QLineEdit` | placeholder「请输入用户名」 |
| 密码 | `QLineEdit` | `EchoMode::Password`，placeholder「请输入密码」 |
| 登录 | `QPushButton` | 默认按钮（`setAutoDefault(true)`） |
| 状态 | `QLabel` | 初始灰色提示文字 |

### 交互行为

- 点击「登录」触发验证
- 密码框按 Enter 同样触发验证
- 验证中：按钮文字变为「验证中...」，按钮禁用
- 验证完成：按钮恢复为「登录」，显示结果
- 登录成功或失败后，输入框内容均保留

### 状态反馈

| 场景 | 标签文字 | 颜色 |
|------|----------|------|
| 初始 | 请输入用户名和密码 | 灰色 `#888888` |
| 用户名为空 | 请输入用户名 | 橙色 |
| 密码为空 | 请输入密码 | 橙色 |
| 验证成功 | 登录成功！欢迎，admin | 绿色 |
| 验证失败 | 用户名或密码错误 | 红色 |

### macOS 原生风格

- 不调用 `QApplication::setStyle()` 强制 Fusion 等跨平台主题
- 使用标准 `QWidget` 控件，不添加 QSS 自定义样式
- 依赖 Qt 在 macOS 上的默认原生控件渲染

## 验证逻辑

### AuthValidator 接口

```cpp
class AuthValidator {
public:
    enum class Result { Success, WrongCredentials };

    static Result validate(const QString& username, const QString& password);
};
```

### 验证规则

- 硬编码账号：`admin` / `123456`
- 精确匹配，区分大小写
- 不自动 trim 首尾空格

### 验证流程

```
点击登录 / 按 Enter
    │
    ▼
用户名是否为空？ ──是──→ 「请输入用户名」（橙色）
    │否
    ▼
密码是否为空？   ──是──→ 「请输入密码」（橙色）
    │否
    ▼
按钮禁用，文字「验证中...」
    │
    ▼
AuthValidator::validate()
    │
    ├─ Success ──→ 「登录成功！欢迎，admin」（绿色）
    └─ WrongCredentials ──→ 「用户名或密码错误」（红色）
    │
    ▼
按钮恢复「登录」
```

### 边界情况

| 情况 | 处理 |
|------|------|
| 用户名含首尾空格 | 按原样匹配，不自动 trim |
| 连续快速点击 | 验证中按钮禁用 |
| 用户名和密码均为空 | 优先提示「请输入用户名」 |

## 构建配置

### CMakeLists.txt 要点

- C++17 标准
- `CMAKE_AUTOMOC ON`
- 依赖 `Qt6::Widgets`
- 源文件：`main.cpp`、`LoginWindow.cpp`、`AuthValidator.cpp`

### 构建命令

```bash
cd QtLoginDemo
cmake -B build -DCMAKE_PREFIX_PATH=/opt/homebrew/opt/qt@6
cmake --build build
./build/QtLoginDemo
```

`build/` 目录已在项目 `.gitignore` 中。

## 测试策略

### 手动验证清单

| 场景 | 输入 | 期望 |
|------|------|------|
| 正确登录 | admin / 123456 | 绿色「登录成功！欢迎，admin」 |
| 错误密码 | admin / wrong | 红色「用户名或密码错误」 |
| 错误用户名 | user / 123456 | 红色「用户名或密码错误」 |
| 空用户名 | （空）/ 123456 | 橙色「请输入用户名」 |
| 空密码 | admin / （空） | 橙色「请输入密码」 |
| Enter 键 | 填好后按 Enter | 与点击按钮相同 |

### 单元测试（可选）

`AuthValidator` 可用 Qt Test 覆盖核心用例（正确凭证、错误凭证）。实现阶段根据复杂度决定是否添加。

## 不在范围内

- 真实后端 API 对接
- 记住密码 / 自动登录
- 登录后跳转主界面
- 多用户账号管理
- 密码加密存储
