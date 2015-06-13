基于nlpbamboo的(简体)中文人名识别工具

# 介绍 #

关于findscn的用法
  * -c 开启高亮
  * -n 显示行号
  * -i 只输出人名
![http://www.yakergong.net/blog/attachments/2011/03/findscn.png](http://www.yakergong.net/blog/attachments/2011/03/findscn.png)

# 用法举例 #

获取代码后

cd findscn

make

cat example1.txt | ./findscn

./findscn -c -n example1.txt

./findscn -c -n -i `*`.txt