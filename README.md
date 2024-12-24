# PanoramicTracking
Real-time panoramic video stitching and object tracking

## TODO

- [x] 完善`base`库，加入线程安全的队列等常用的数据结构
- [x] 完善`log`库，对于每次运行的结果，不要用`imshow`，而是根据运行时间创个文件夹，把所有中间结果以图片的形式保存下来
- [ ] 学习常用的图像融合算法
  - [x] 加权融合
  - [x] 羽化融合
  - [ ] 最佳分割线融合
  - [ ] Multiband融合

- [ ] 学习投影算法
  
- [ ] 跑通`openCV`里的sitch的例子，看看Bundle Adjustment和Wave Correct到底作用大不大
- [ ] 比较自己实现的拼接和`openCV`里的sitch的例子的区别



## 常见的图像融合算法

1.加权平均融合：I-fused(x,y) = α⋅I1(x,y) + (1−α)⋅I2(x,y)

- 该融合算法2张图片的权重是固定的，相加为1



2.羽化融合（也叫渐入检出的融合）：I-fused(x,y) = w(x,y)⋅I1(x,y) + (1−w(x,y))⋅I2(x,y)

- 加权平均融合的改进版，重叠区不同位置，2张图所占的权重不一样



3.最佳拼接缝（Optimal Seam）融合

- 在两幅图像的重叠区域内，通过特定的算法找到一条“最佳”分割线，分割线的两侧分别保留对应图像的像素，从而尽量避免颜色、亮度或内容的不一致

- 分割线的寻找有多种方式，比如：逐点法、动态规划法、图割法、深度学习......
- 最佳拼接缝可以与别的图像融合算法结合，比如在缝左右小范围内的像素使用羽化融合

> Optimal Seam在一些论文里被看为Image Alignment的方法，可以单独用，也可以和全局、局部透视变换结合
>



## 一些疑问

1.多图拼接时，哪张图片为基准图像？

- 之前的想法，包括参考代码2的实现都是假设从左到右有N个相机，以最左或最右的相机为得到的图像为基准。但是实际上OpenCV的图像拼接模块并不是这样实现的，它会选择一张与其他图像重叠区域最大、匹配特征点最多的图像作为基准图像，可以通过打印`estimate`得到的各相机的外参来知道谁到底是基准（R为单位阵）



2.为什么要进行柱面/球面之类的图像投影?

- 多个相机拍摄时，相机离物体的距离不同（各相机不是在一个平面上的），导致图像中物体的尺度不同。通过图像投影，可以把各相机的成像画面统一到一个坐标系下，通过在图像投影之后再进行图像的对齐和融合。



3.OpenCV的图像拼接模块`cv::detail`里面为什么用相机的外参来描述2张图像之间的几何变换关系而不用单应矩阵，并且为什么它可以在不要标定板的情况下求出相机的内外参？

- 首先，如果要准确的求出相机的内外参，肯定是需要标定板的，`cv::detail::Esitmator`主要是为了求相机的外参，内参是随便设的一个固定的值（光心为图像的中心，焦距为）
- 单应矩阵可以拆成2个相机内外参的乘积：H10=K1·R1·R0^-1·K0^-1，所以在**内参固定**的情况下，用单应矩阵和用外参来表示2个图像之间的几何变换关系是等价的

[Opencv2.4.9源码分析——Stitching（三）_opencv stitching-CSDN博客](https://blog.csdn.net/zhaocj/article/details/78809143)



4.mosaic拼接和panoramic拼接的区别是什么？

- 主要区别在于场景和相机的运动：
  - mosaic拼接主要针对的是拼接场景近似**平面**（例如墙面、地面、文档、远处物体），且相机的移动主要是**平移**（也可小角度的旋转）
  - panoramic拼接主要是针对**三维**的拼接场景（例如室内场景、近处物体），且相机的运动主要是是**旋转运动**（绕固定点的水平或垂直旋转），或者视角覆盖范围较大

> 只有panoramic拼接才需要柱面、球面等投影，mosaic拼接则不需要
>



5.图像拼接的步骤是什么？

(1)图像获取 → (2) 图像校准（可选） → (3) 图像配准 → (4) 图像对齐 → (5) 图像融合 → (6) 全局优化（可选） 

- 全局优化包括Bundle Adjustment、Wave Correct...



## 参考

- [duchengyao/gpu-based-image-stitching: A simple version of "GPU based parallel optimization for real time panoramic video stitching".](https://github.com/duchengyao/gpu-based-image-stitching)
- [suncle1993/VideoStitching: solve real time video stitching problem： 4 camera example by opencv surf](https://github.com/suncle1993/VideoStitching)
- https://www.bilibili.com/video/BV1ri4y1s72t
- [AutoStitch笔记1 - 知乎](https://zhuanlan.zhihu.com/p/56633416)
