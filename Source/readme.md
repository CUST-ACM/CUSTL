# CUSTL Source Dir

## 目录结构

```
.
├── header                  # 源代码
│   ├── allocator.h
│   ├── config.h
│   ├── construct.h
│   ├── deque.h
│   ├── iterator.h
│   ├── list.h
│   ├── tree.h
│   ├── type_traits.h
│   └── vector.h
├── Makefile
├── readme.md
└── test                    # demo 和单元测试
    ├── deque_demo.cpp
    ├── list_demo.cpp
    ├── Rb_tree_test.cpp
    └── vector_demo.cpp

```

## 添加代码说明

1. 源代码放入 `header` 文件夹

2. 单元测试 和 demo 代码放入 `test` 文件夹

## Test Build

1. 进入 `Source` 文件夹

2. 运行 `make && make check` 来编译 `test` 下的代码，并运行对应的二进制文件
