# 验证记录

## 已完成

- 从移除预编译产物后的源码构建，在 Windows MSYS2 MinGW32 环境完成 `make all`，生成 90,112 字节内核与 10,321,920 字节磁盘镜像。
- 构建环境：GNU Make 4.4.1、GCC 16.1.0、QEMU 11.0.1；目标为 32 位 MinGW。
- QEMU 无窗口启动镜像，画面确认已进入 Shell；`ls` 和 `date` 返回结果，`malloc` 输出分配/扩堆结果后返回 Shell；`forks` 输出多个父子进程 PID 后返回 Shell。
- `docs/images/qemu-shell.png` 保存本次运行画面。普通 `fork` 样例仅输出 PID，未将其误作为子进程创建验证。
- 镜像、对象文件、运行库、Shell 与用户程序二进制均移到仓库外；必要目录保留 `.gitkeep` 并可由 Makefile 自动创建。
- Python 镜像工具语法、README 本地链接和提交清单检查通过。
- 最后从 Git 暂存区导出实际待上传源码到全新目录，再次执行 `make all` 成功，确认提交清单包含必要构建文件。

## 为干净构建完成的修正

- Makefile 的 include 使用正斜线；自动创建输出目录，串行构建并确保 deploy 依赖 build。
- C 运行库中补充缺失声明，并为 32 位地址/指针转换添加显式类型转换，以适配当前 GCC。
- `time.c` 补充 string/stdio 头文件，Shell 执行模块补充 file 头文件；运行库 clean 使用 MSYS 可用的 `rm -f`。

## 验证边界

原教学代码仍有编译和链接警告；此次只验证上述命令和启动流程，没有执行所有内核测试或证明内存管理完全正确。原实验报告是历史说明，本文件描述本次实际执行情况。
