# miniRenderer

3D 软件光栅化渲染器



## 特性

- [x] 渲染管线
  - [x] 右手坐标系，视图变换、透视&正交投影变换、视口变换
  - [x] 线框模式：Bresenham直线算法
  - [x] 填充模式：Scanline填充算法、重心坐标插值填充算法、透视校正插值
  - [x] 顶点着色器、片段着色器
  - [x] 背面剔除、深度测试
- [x] 经典光照模型、Normal Mapping
- [x] 模型文件 obj 支持



## 构建

- 安装 vcpkg: https://github.com/Microsoft/vcpkg
- 安装 SDL: `vcpkg install sdl`

- cmake with vcpkg
