# miniRenderer
3D 软件光栅化渲染器



## 特性

- [x] 跨平台

- [ ] 交互

- [ ] 渲染管线
  - [x] 局部空间→世界空间→观察空间→裁剪空间→ NDC → 屏幕空间
  - [x] 线框模式、填充模式
  - [ ] 透视校正插值
  - [ ] 背面剔除
  - [x] 顶点着色器、片段着色器
  - [ ] 几何着色器
  - [ ] Aplha 测试
  - [ ] 模板测试
  - [x] 深度测试
  - [ ] 混合
  - [ ] 延迟着色
  - [ ] 分块延迟着色
  
- [ ] 经典光照模型

- [x] Normal Mapping

- [ ] PBR

- [ ] IBL

- [ ] Tone Mapping

- [ ] Gamma 校正

- [ ] 点光源、方向光、聚光灯

- [ ] SSAO

- [ ] Shadow Map、PCF、CSM

- [ ] 屏幕拾取

- [ ] 骨骼动画

- [ ] 天空盒

  

- [ ] Relief Mapping

- [x] 模型文件 obj 支持

- [x] 左手坐标系（参照OpenGL）

- [x] 详细注释





## 构建

- 安装 vcpkg: https://github.com/Microsoft/vcpkg
- 安装 SDL: `vcpkg install sdl`

- cmake with vcpkg



## 参考

- 3D数学（矩阵变换、齐次坐标等）：
- 《3D游戏与计算机图形学中的数学方法》
  - http://www.songho.ca/opengl/gl_projectionmatrix.html
- 图形学基础和图形渲染管线：

  - LearnOpenGL：https://learnopengl.com/
  - 细说图形学渲染管线：https://zhuanlan.zhihu.com/p/79183044
  - Bresenham ：https://zh.wikipedia.org/wiki/%E5%B8%83%E9%9B%B7%E6%A3%AE%E6%BC%A2%E5%A7%86%E7%9B%B4%E7%B7%9A%E6%BC%94%E7%AE%97%E6%B3%95
  - 扫描线算法：https://zh.wikipedia.org/wiki/%E6%89%AB%E6%8F%8F%E7%BA%BF%E6%B8%B2%E6%9F%93
- SDL ：

  - https://www.willusher.io/sdl2%20tutorials/2013/08/17/lesson-1-hello-world
  - http://sdl.beuc.net/sdl.wiki/Pixel_Access
- tinyrenderer：https://github.com/ssloy/tinyrenderer/wiki