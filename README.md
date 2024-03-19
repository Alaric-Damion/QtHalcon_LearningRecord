# Halcon_LearningRecord

#### 介绍
记载一些在编程及图像处理过程中的问题及解决方案，主要包括QT与C++的一些实用小项目，Halcon及Opencv的图像处理相关解决方案，主要是自己学习的一些记录内容，只是用于学习，涉及的相关借鉴内容的我尽量标明出处，如涉及版权问题，请联系删除。

#### 软件环境
Qt：5.9.6
vs2019
Halcon 12.0/17.0


#### 子项目介绍

* 1.ARM_FontSet
**功能：**实现Halcon界面设定的文字字体和大小设置。

![ARM_FontSet](https://images.gitee.com/uploads/images/2021/0817/221328_6315a907_4968621.png "1.ARM_FontSet.PNG")

* 2.CustomView
**功能：**实现超大像素图像读取/显示功能。

![CustomView](https://images.gitee.com/uploads/images/2021/0817/221415_1799756c_4968621.png "2.CustomView.PNG")

* 3.ImageScale
**功能：**实现图像缩放平移功能。

![ImageScale](https://images.gitee.com/uploads/images/2021/0817/221456_5b998351_4968621.png "3.ImageScale.PNG")

* 4.Draw_Item
**功能：**实现QGraphicsView的自定义Item组件。

![Draw_Item](https://images.gitee.com/uploads/images/2021/0817/221520_bf81174f_4968621.png "4.Draw_Item.PNG")

* 5.AutoFacus
**功能：**实现图像自动对焦评价系数参考。

![AutoFacus](snap/5.AutoFacus.PNG)

* 6.LockerTest
**功能：**实现Qt抽屉展开折叠显示功能。

![LockerTest](snap/6.LockerTest.PNG)

* 7.CustomToolBox
**功能：**实现Qt控件操作使用功能。

![CustomToolBox](snap/7.CustomToolBox.PNG)

* 8.AutoRepeat_Test
**功能：**Qt实现QPushButton长按触发事件。

![AutoRepeat](snap/8.AutoRepeat.gif)

* 9.SwitchButton
**功能：**Qt实现开关控件效果。

![控件效果](snap/9.SwitchButton.PNG)

* 10.Balser_GigeDemo
**功能：**Balser工业相机采图及参数设置。

![Balser相机采图](snap/10.Balser_GigeDemo.PNG)

* 11.QtCameraDemo
**功能：**Qt实现USB多摄像头视频、抓图及参数设置。

![多相机采图和参数设置](snap/11.QtCameraDemo.gif)

* 12.VideoTracking
**功能：**Qt+OpenCV实现USB摄像头监测移动物体。

![VideoTracking](snap/12.VideoTracking.PNG)

* 13.GrayTo3channel
**功能：**Qt实现8位灰度图叠加通道转换成24位三通道图像。

![GrayTo3channel](snap/13.GrayTo3channel.gif)

* 14.Code_Encryption
**功能：**Qt实现软件使用次数限制。

![Code_Encryption](snap/14.Code_Encryption.gif)

* 15.Dynamic_setStyle
**功能：**Qt实现控件背景的qss。

![Dynamic_setStyle](snap/15.Dynamic_setStyle.JPG)

* 16.printTest
**功能：**美松打印机打印测试程序。

![printTest](snap/16.printTest.PNG)

* 17.rtsp_Display
**功能：**Qt调用VLC-Qt库实现拉流播放。

![rtsp_Display](snap/17.rtsp_Display.PNG)

* 18.videoPlay
**功能：**基于OPenCV的视频播放器。

![videoPlay](snap/18.videoPlay.PNG)

* 19.transTest
**功能：**Qt的中英文动态翻译。

![transTest](snap/19.transTest.gif)

* 20.SingleApplationTest
  **功能：**Qt程序单例化测试。

![SingleApplationTest](snap/20.SingleApplationTest.gif)

* 21.PixelPos_MouseFollow
  **功能：**Qt实现实时获取图像坐标和像素值。

![PixelPos_MouseFollow](snap/21.PixelPos_MouseFollow.gif)
#### 文件夹说明

1.  snap
此文件夹为各子工程界面截图
2.  File_Info
此文件夹存放工程所需文件，如图像/txt/ini
3.  bin
此文件夹存放工程创建的exe文件
4.  Halcon_StudyProject
此文件存放Halcon学习过程中的一些实际例子
5.  OpenCV_LearningProject
此文件存放OpenCV学习过程中的一些例子

