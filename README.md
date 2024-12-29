# PanoramicTracking
Real-time panoramic video stitching and object tracking

## TODO

- [x] 完善`base`库，加入线程安全的队列等常用的数据结构
- [x] 完善`log`库，对于每次运行的结果，不要用`imshow`，而是根据运行时间创个文件夹，把所有中间结果以图片的形式保存下来
- [ ] 学习常用的图像融合算法
  - [x] 加权融合
  - [x] 羽化融合
  - [x] 最佳拼接缝融合
  - [ ] Multiband融合
- [x] 跑通`openCV`里的sitch的例子，看看Bundle Adjustment和Wave Correct到底作用大不大
- [x] 比较自己实现的拼接和`openCV`里的sitch的例子的区别
- [ ] 弄明白Wave Correct、Bundle Adjustment的原理
- [ ] 仔细看一下`cv::detail`里的各个拼接模块怎么用，能够做到自由地组合并添加新的算法进去
- [x] 试一下kaggle、colab、aistudio之类的免费云服务器
- [ ] 找出图像拼接近几年的所有顶刊顶会论文
- [ ] **开发图像采集模块**



## 常见的图像融合算法

1.加权平均融合：I-fused(x,y) = α⋅I1(x,y) + (1−α)⋅I2(x,y)

- 该融合算法2张图片的权重是固定的，相加为1



2.羽化融合（也叫渐入检出的融合）：I-fused(x,y) = w(x,y)⋅I1(x,y) + (1−w(x,y))⋅I2(x,y)

- 加权平均融合的改进版，重叠区不同位置，2张图所占的权重不一样



3.最佳拼接缝（Optimal Seam）融合（也叫最佳缝合线）

- 在两幅图像的重叠区域内，通过特定的算法找到一条“最佳”分割线，分割线是2张图片差异最小的一条路径（重叠区的内容不同时Seam可能需要动态调整）。分割线的两侧分别保留对应图像的像素，从而尽量避免颜色、亮度或内容的不一致

- 分割线的寻找有多种方式，比如：逐点法、动态规划法、图割法、深度学习......
- 最佳拼接缝可以与别的图像融合算法结合，比如在缝左右小范围内的像素使用羽化融合

> Optimal Seam在一些论文里被看为Image Alignment的方法，可以单独用，也可以和全局、局部透视变换结合
>



## 改进思路

1.用深度学习替代传统的类似SIFT那些的特征点检测算法，唯一要确认的是深度学习找的特征点和用传统算法找的，在数据结构上有没有区别

- 能用的论文：SuperPoint、LF-Net、D2-Net（这都只是找特征点的论文，不晓得有没有人把他们加入拼接的框架能参考一下）

2.针对监控视频晚上照度低、特征点难以提取的缺点，引入图像增强算法来对低照度图片进行拼接

3.用基于深度学习的image warping（比如单应性估计）替代基于特征点的方法

4.结合语义分割技术，用以确定特征点搜索的ROI，从而减少前景的影响



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

- 全局优化包括Bundle Adjustment、Wave Correct、矩形化...很多论文里貌似都没有全局优化，但是这2个步骤非常重要，**很影响观感**



6.到时候论文里实验部分怎么设计？用什么指标？该对比多少算法？怎么选择要对比的算法？

> 看《研究生自救指南》

- 实验还是和别的CV领域一样，一般得包括：**对比实验**、**消融实验**、**实例分析**。参考UDIS这篇论文，这些实验可以分为以下2类：
  - 定量实验：数值指标的分析
  - 定性实验：主要是看可视化拼接结果的细节，比如相同图片拼接，不同算法对于鬼影之类的处理效果
- **定量**实验**指标的选择**主要还是看改进是什么方面
  - 图像配准算法：特征点匹配对数、匹配准确率
  - 图像配准+对齐：峰值信噪比（PSNR）、结构相似性（SSIM）
  - 图像融合算法：峰值信噪比（PSNR）、结构相似性（SSIM）
  - 其他：运行速度
- 消融实验怎么设计：比如我的模型是在UDIS的基础上改进的，加了A、B、C 3个模块，那么消融实验就需要UDIS、UDIS+A、UDIS+A+B、UDIS+A+B+C来验证改进的有效性

- 对比实验到时候选择**4个**（参考UDIS这篇论文的数量）左右的开源算法就行了，具体选什么算法后面再看吧...



7.实验到底是定性分析还是定量分析还是都进行呢？

- 最优方案肯定是都进行，参考UDIS这篇论文，对比实验至少要都进行，消融实验可以只定性分析



8.论文中提到的“基线”是什么意思？比如“大基线深度单应性网络”

- “基线”指的是2个相机之间的**相对位置**和**角度**之间的差异。大基线就说明2个相机的位置和角度差异比较大



9.什么是视差？

- 视差指的是由两个不同视角拍摄的图像中**同一物体的对应点**在水平或垂直方向上的**像素差异**。产生原因：2个相机有一定的基线
- 视差与物体的深度有直接的关系：2者大小成反比



## 参考

- [duchengyao/gpu-based-image-stitching: A simple version of "GPU based parallel optimization for real time panoramic video stitching".](https://github.com/duchengyao/gpu-based-image-stitching)
- [suncle1993/VideoStitching: solve real time video stitching problem： 4 camera example by opencv surf](https://github.com/suncle1993/VideoStitching)
- https://www.bilibili.com/video/BV1ri4y1s72t
- [AutoStitch笔记1 - 知乎](https://zhuanlan.zhihu.com/p/56633416)
- [文献汇总1](https://github.com/DoongLi/awesome-homography-estimation-and-image-alignment)
- [文献汇总2](https://github.com/MelodYanglc/Survey)
