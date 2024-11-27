# 操作流程

1.使用XCOM.exe打开串口读取数据，点击保存窗口

2.复制内容到example.txt中，注意example.txt中的内容需要过滤掉部分初始化字符，只剩下Pitch:60.1,Roll:10.2,Yaw:3.1内容

3.确保自己在`F:\pid-proj\pid-proj\data-process`目录下

3.使用python .\process.py处理