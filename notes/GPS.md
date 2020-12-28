### GPS数据采集及写入

#### exiftool 写入GPS信息

```c++
system("exiftool -p -GPSLongitudeRef=E -GPSLongitude=111.123456 -GPSLatitudeRef=N -GPSLatitude=33.23456 -GPSAltitudeRef=Above -GPSAltitude=357 image");
```

#### exiftool读取GPS信息

```c++
system("exiftool -s -GPSLatitude DSC00244.jpg");//经度
system("exiftool -s -GPSLongitude DSC00244.jpg");//纬度
system("exiftool -s -GPSPosition DSC00244.jpg");//经纬度
system("exiftool -s -GPSAltitude DSC00244.jpg");//高度
system("exiftool -s -n -GPSAltitude DSC00244.jpg");//小数形式
```

读取时默认格式为 "%d deg %d' %.2f"  即  54 deg 59' 22.80" 

拷贝时默认格式为"%d %d %.8f"            即 54 59 22.80000000

https://www.rmnof.com/article/exiftool-introduction/

https://www.jianshu.com/p/d76457799de1

官方文档：https://exiftool.org/exiftool_pod.html

#### 传入程序中(GPS完整信息为例)

```c++
#include<iostream>
#include<stdio.h>
#include<stdlib.h>

using namespace std;
int execmd(const char* cmd, char* result) {
    char buffer[128];                      //定义缓冲区                     
    FILE* pipe = _popen(cmd, "r");          //打开管道，并执行命令 
    if (!pipe) return 0;                  //返回0表示运行失败 

    while (!feof(pipe)) {
        if (fgets(buffer, 128, pipe)) {           //将管道输出到result中 
            strcat(result, buffer);
        }
    }
    _pclose(pipe);                        //关闭管道 
    return 1;                            //返回1表示运行成功 
}

int main() {
    char result[1024 * 4] = "";                //定义存放结果的字符串数组 
    if (1 == execmd("exiftool -s -GPSPosition DSC00244.jpg", result)) {
        printf(result);
    }
    system("pause");                      //暂停以查看结果 
}
```

http://blog.sina.com.cn/s/blog_3fe961ae0100uwv5.html

##### 出现secure warning：

项目 - 属性 -c/c++ - 预处理器 - 预处理器定义 - 加上 _CRT_SECURE_NO_WARNING

#### 使用geotag(不一定用得上)

-geotag TRKFILE                  从指定的GPS日志对图像进行地理标记

https://exiftool.org/geotag.html