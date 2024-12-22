# PanoramicTracking
Real-time panoramic video stitching and object tracking

## TODO

- [x] 完善`base`库，加入线程安全的队列等常用的数据结构
- [x] 完善`log`库，对于每次运行的结果，不要用`imshow`，而是根据运行时间创个文件夹，把所有中间结果以图片的形式保存下来
- [ ] 学习常用的图像融合算法
  - [x] 加权融合
  - [x] 羽化融合
  - [ ] 最佳分割线融合

- [ ] 跑通`openCV`里的sitch的例子，看看Bundle Adjustment和Wave Correct到底作用大不大
- [ ] 比较自己实现的拼接和`openCV`里的sitch的例子的区别



## 常见的图像融合算法

1.加权平均融合：I-fused(x,y) = α⋅I1(x,y) + (1−α)⋅I2(x,y)

- 该融合算法2张图片的权重是固定的，相加为1



2.羽化融合（也叫渐入检出的融合）：I-fused(x,y) = w(x,y)⋅I1(x,y) + (1−w(x,y))⋅I2(x,y)

- 加权平均融合的改进版，重叠区不同位置，2张图所占的权重不一样



## 参考

- [duchengyao/gpu-based-image-stitching: A simple version of "GPU based parallel optimization for real time panoramic video stitching".](https://github.com/duchengyao/gpu-based-image-stitching)
- [suncle1993/VideoStitching: solve real time video stitching problem： 4 camera example by opencv surf](https://github.com/suncle1993/VideoStitching)
