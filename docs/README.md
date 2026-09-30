**简体中文** | [English](README_EN.md)

# NanSU

<img src="https://kernelsu.org/logo.png" style="width: 96px;" alt="logo">

一个 Android 上基于内核的 root 方案，由 [`tiann/KernelSU`](https://github.com/tiann/KernelSU) 分叉而来，并移植了 SukiSU Ultra 的 KernelPatch Module 功能，并且优化了它。

[![Latest release](https://img.shields.io/github/v/release/Guxin12/NANSU?label=Release&logo=github)](https://github.com/Guxin12/NANSU/releases/latest)
[![License: GPL v2](https://img.shields.io/badge/License-GPL%20v2-orange.svg?logo=gnu)](https://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html)
[![GitHub License](https://img.shields.io/github/license/Guxin12/NANSU?logo=gnu)](/LICENSE)


## 特性

- 基于内核的 `su` 和权限管理。
- 基于 [metamodules](https://kernelsu.org/zh_CN/guide/metamodule.html) 的模块系统：可插拔的模块架构。
- [App Profile](https://kernelsu.org/zh_CN/guide/app-profile.html): 把 Root 权限关进笼子里。
- KPM 支持 （移植自 SukiSU Ultra）。

## 兼容状态

KernelSU 官方支持 GKI 2.0 的设备（内核版本5.10以上）；旧内核也是兼容的（最低4.14+），不过需要自己编译内核。

WSA, ChromeOS 和运行在容器上的 Android 也可以与 KernelSU 一起工作。

目前支持 `arm64-v8a` 和 `x86_64` 架构。

> [!CAUTION]
> 最近的内核版本引入了一项破坏性更改，导致 KernelSU 在 `x86_64` 上运行失败，甚至可能引发内核恐慌 (kernel panic)！请查看网站获取更多信息！

## 使用方法

- [安装教程](https://kernelsu.org/zh_CN/guide/installation.html)
- [如何构建？](https://kernelsu.org/zh_CN/guide/how-to-build.html)
- [官方网站](https://kernelsu.org/zh_CN/)

## KPM 支持

- 基于 KernelPatch 开发，移除了与 KernelSU 重复的功能，仅保留 KPM 支持。
- 正在进行（WIP）：通过集成附加功能来扩展 APatch 兼容性，以确保跨不同实现的兼容性。

**开源仓库**：[https://github.com/Guxin12/SukiSU_KernelPatch](https://github.com/Guxin12/SukiSU_KernelPatch)

**KPM 模板**：[https://github.com/udochina/KPM-Build-Anywhere](https://github.com/udochina/KPM-Build-Anywhere)

> [!Note]
>
> 1. 需要 `CONFIG_KPM=y`
> 2. Non-GKI 设备需要 `CONFIG_KALLSYMS=y` 和 `CONFIG_KALLSYMS_ALL=y`
> 3. 对于低于 `4.19` 的内核，需要从 `4.19` 的 `set_memory.h` 进行反向移植。

## 安全性

有关报告 KernelSU 安全漏洞的信息，请参阅 [SECURITY.md](/SECURITY.md)。

## 许可证

- 目录 `kernel` 下所有文件为 [GPL-2.0-only](https://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html)。
- 除 `kernel` 目录的其他部分均为 [GPL-3.0-or-later](https://www.gnu.org/licenses/gpl-3.0.html)。

## 鸣谢

- [KernelSU](https://github.com/tiann/KernelSU)：上游
- [SukiSU-Ultra](https://github.com/SukiSU-Ultra/SukiSU-Ultra)：KPM 功能移植来源
- [KernelPatch](https://github.com/bmax121/KernelPatch)：KernelPatch 是内核模块 APatch 实现的关键部分

## KernelSU 原项目鸣谢

- [kernel-assisted-superuser](https://git.zx2c4.com/kernel-assisted-superuser/about/)：KernelSU 的灵感。
- [Magisk](https://github.com/topjohnwu/Magisk)：强大的 root 工具箱。
- [genuine](https://github.com/brevent/genuine/)：apk v2 签名验证。
- [Diamorphine](https://github.com/m0nad/Diamorphine)：一些 rootkit 技巧。
