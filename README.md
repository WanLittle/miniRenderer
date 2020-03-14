# miniRenderer
3D 软件光栅化渲染器



## 特性

- 跨平台
- 模型文件支持：obj
- 左手坐标系
- 详细注释



## 构建

https://github.com/Microsoft/vcpkg

- ```c
  vcpkg install sdl
  ```

- cmake



## 参考

- 3D数学（矩阵变换、齐次坐标等）

  - 《3D游戏与计算机图形学中的数学方法》
  - 投影矩阵：http://www.songho.ca/opengl/gl_projectionmatrix.html

- 图形学基础和图形渲染管线：

  - LearnOpenGL 教程：https://learnopengl.com/
  - 细说图形学渲染管线：https://zhuanlan.zhihu.com/p/79183044
  - Bresenham 直线绘制算法：https://zh.wikipedia.org/wiki/%E5%B8%83%E9%9B%B7%E6%A3%AE%E6%BC%A2%E5%A7%86%E7%9B%B4%E7%B7%9A%E6%BC%94%E7%AE%97%E6%B3%95
  - 扫描线算法：https://zh.wikipedia.org/wiki/%E6%89%AB%E6%8F%8F%E7%BA%BF%E6%B8%B2%E6%9F%93

- SDL 相关：

  - https://www.willusher.io/sdl2%20tutorials/2013/08/17/lesson-1-hello-world
  - http://sdl.beuc.net/sdl.wiki/Pixel_Access

  