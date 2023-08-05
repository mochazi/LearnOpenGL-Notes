# OpenGL学习笔记
## 参考视频
[LearnOpenGL文档学习【完结】](https://www.bilibili.com/video/BV11Z4y1c7so)

## 参考仓库

[start-learning-opengl](https://github.com/yocover/start-learning-opengl)

[LearnOpenGL](https://github.com/JoeyDeVries/LearnOpenGL)

## 编译环境
- [**请自行安装msys2的msys64-mingw64**](https://packages.msys2.org/package)
- **低版本环境没测试，估计没问题**

### msys2一键安装

- 由于手动编译`assimp`耗时比较久，默认使用[msys2的mingw-w64-x86_64-assimp](https://packages.msys2.org/base/mingw-w64-assimp)
- 如果`assimp`无法使用，可考虑`ext/CMakeLists.txt`开启手动编译

```bash
pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-assimp mingw-w64-x86_64-cmake mingw-w64-x86_64-bzip2 mingw-w64-x86_64-glm mingw-w64-x86_64-glfw
```

---

| **编译环境**  |                             **项目使用版本**                             |
|:---------:|:------------------------------------------------------------------:|
|  **gcc**  |                             **13.1.0**                             |
|  **g++**  |    **13.1.0**    |
| **cmake** | **3.24**  |
| **bzip2** |  **1.0.8**  |

## 第三方库
|   **库名**   |                                       **项目使用版本**                                       |
|:----------:|:--------------------------------------------------------------------------------------:|
|  **glad**  |                        **[gl4.6-core](https://glad.dav1d.de)**                         |
|  **glfw**  |              **[3.3.8](https://github.com/glfw/glfw/releases/tag/3.3.8)**              |
|  **glm**   |           **[0.9.9.8](https://github.com/g-truc/glm/releases/tag/0.9.9.8)**            |
| **imgui**  |            **[v1.78](https://github.com/ocornut/imgui/releases/tag/v1.78)**            |
| **assimp** |            **[v5.2.5](https://github.com/assimp/assimp/releases/tag/v5.2.5)**            |

---

## 克隆仓库

```bash
git clone --recurse-submodules git@github.com:mochazi/LearnOpenGL-Notes.git
```

## 查看子模块

```bash
git submodule
```