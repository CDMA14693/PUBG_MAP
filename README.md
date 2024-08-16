# PUBG-
本项目基于2点之间的像素数量，计算游戏内地图2点之间的距离  
地图必须基于某个固定的缩放才能精确设置，推荐地图放大2次。此时可以达到迫击炮最大射程700米  
软件会自动创建与读取配置文件。文件为 config.dat  
如果软件出现错误后删除 配置文件 config.dat，软件会自动生成新的  
  
本项目不会造成游戏封号，完全基于屏幕绘图以及像素测量  

# 演示视频
https://www.bilibili.com/video/BV1nKYueiEhY
# 基本功能

通过设置游戏地图的缩放次数,来让软件与游戏地图同步不同缩放等级,以求让软件适配所有的游戏

托盘图标  
![image](https://github.com/user-attachments/assets/1af5f99a-1dc5-4ab6-b7cd-866256eb2efc)  
右键托盘呼出菜单  
![image](https://github.com/user-attachments/assets/a95084b8-4a38-423b-8156-8540bd76afb1)  
进入设置
## 快捷键设置菜单 与 功能显示菜单
![image](https://github.com/user-attachments/assets/f9c5344f-e28b-4c96-87a5-c545cb377c0b)

![image](https://github.com/user-attachments/assets/9122fd08-2e0d-4f4f-a905-712e99a02ca4)

渲染标记点可以让软件在绘图空间内进行标点显示,关闭状态下,标点功能不会启用
### 功能分别是
标记点渲染 的开启与关闭 && 功能显示菜单中 功能状态  开启与关闭  
按下某个键后 点击鼠标左键 标记点1 !需要标记点渲染开启  
按下某个键后 点击鼠标左键 标记点2 !需要标记点渲染开启  
标记中心点, 快捷将点1 或者 点2 标记到屏幕中心  
地图开关按键, 与游戏地图开关按键同步,只有这个功能开启后 软件才会处理地图的缩放, 显示菜单也会更新 软件认为地图缩放次数  
地图开关2 解决游戏地图与软件认为状态冲突的情况下,让软件去同步游戏地图状态的功能  
如果遇到游戏内,地图缩放与软件不匹配的情况,可以通过滚轮让游戏与软件同时到到达 地图放大0 的状态来解决    
#### 设置地图不同的缩放下不同的100M像素数量  
![image](https://github.com/user-attachments/assets/1c0c30d0-e246-4c89-aee8-56baf9ebdd07)  
通过下拉框选择放大次数,如果不使用这个放大次数可以不设置,或者为0  
地图从不缩放到最大缩放需要都设置,不然容易造成游戏缩放与软件认为缩放不匹配的情况.  
##### 添加&删除放大
![image](https://github.com/user-attachments/assets/8d377792-285d-44a5-9956-17312e41bde2)
添加 还是 删除 都会去删减增加最后的选项 ,也就是说如果此时设置能放大5次  
删除就是 删除第5次放大  
增加就是 第5次后面增加第6次放大  
#  使用方法
## 首先确定游戏地图从开打不缩放到最大缩放的次数
滚轮没滚动一次视为一次缩放  
将这个缩放次数设置给软件,通过添加与删除放大次数  
## 获取不同缩放 100M 像素数量
使用软件的标点或则其他的截图软件来获取不同缩放下100M内含有的像素数量  
将测得的值设置给软件不同放大下对于的值  
![N5T5N_)}IJOG}NU)Y}K9B}G](https://github.com/user-attachments/assets/c28c9600-c77d-41f0-9ff1-c031e514da18)  
之后只要在这个地图放大倍率下，就可以测得距离

## 技巧
PUBG 中按下空格可以将人物当前的位置放在地图中间。  
所以 将某个点使用快捷键设置到屏幕中心就可以和人物位置重叠。
