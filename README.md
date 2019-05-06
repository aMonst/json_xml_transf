json_xml_transf
===============
使用cjson库写的一个json和xml互相转化的例子
## 使用说明
直接复制其中的cJson.h cJson.cpp Json.h Json.cpp，调用里面的json2xml 和 xml2json来进行相互转化
## 暂时存在的问题
这个库暂时没有考虑某些情况：
   1. xml中存在标签的问题
   2. 
   ```xml
    <label>123<lab>123</lab></label>
  ```
  这种标签到子标签中存在值的情况
## 更新日志
 ### 20170424：
  在网上找了份代码，重写了它里面的xml转json部分
  ### 20170505：
  重写了所有代码，修复了一个bug。
  在将其放到实际项目中进行运行的时候发现如果在xml标签中"<>"作为值而不是标签的时候，解析会出现错误
  例如：里面如果存在这样的项
```xml
<banner>"HTTP/1.1 200\ncontent-length:81\naccept-ranges:bytes\nexpires:Tue, 23 Feb 2016 13:37:17 GMT\nserver:Apache\nlast-modified:Tue, 12 Jan 2010 13:48:00 GMT\nconnection:Close\netag:51-47cf7e6ee8400\ncache-control:max-age=86400\ndate:Mon, 22 Feb 2016 13:37:17 GMT\ncontent-type:text/html\n\n\n<html>\n<meta http-equiv=refresh content=0;url=http://www.baidu.com/>\n</html>\n\"</banner>
```
  它会将里面的html标签作为一个新的xml项，而不是作为banner中的字符串
### 20170517
修改可能造成内存泄露的bug
