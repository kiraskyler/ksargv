# git工具

V0.0.3.20210526

------

## 项目简介

## 文件目录

* 这是单独的一篇文章，要什么文件目录

## git_keep_ignore.sh

* 用于在当前或指定的文件夹中生成.gitignore和.gitkeep文件

* 在当前目录中执行，直接直接即可，会在当前目录下拷贝一份.gitignore文件，在每个空目录下生成一份.gitkeep文件

```
[kira@kira-virtual-machine git]$ ll
总用量 28
drwxrwxr-x 3 kira kira 4096 5月  26 09:02 ./
drwxrwxr-x 8 kira kira 4096 5月  26 08:49 ../
drwxrwxr-x 2 kira kira 4096 5月  26 09:02 emputydir/
-rw-rw-r-- 1 kira kira  322 5月  26 08:47 .gitignore
-rw-rw-r-- 1 kira kira  195 5月  26 08:44 .gitkeep
-rwxrwxr-x 1 kira kira 1226 5月  26 09:01 git_keep_ignore.sh*
-rw-rw-r-- 1 kira kira  657 5月  26 08:54 Readme.md
[kira@kira-virtual-machine git]$ ./git_keep_ignore.sh
[v] emputydir 
[kira@kira-virtual-machine git]$ ll ./emputydir/
总用量 12
drwxrwxr-x 2 kira kira 4096 5月  26 09:02 ./
drwxrwxr-x 3 kira kira 4096 5月  26 09:02 ../
-rw-rw-r-- 1 kira kira  195 5月  26 09:02 .gitkeep
[kira@kira-virtual-machine git]$ ll
总用量 28
drwxrwxr-x 3 kira kira 4096 5月  26 09:02 ./
drwxrwxr-x 8 kira kira 4096 5月  26 08:49 ../
drwxrwxr-x 2 kira kira 4096 5月  26 09:02 emputydir/
-rw-rw-r-- 1 kira kira  322 5月  26 08:47 .gitignore
-rw-rw-r-- 1 kira kira  195 5月  26 08:44 .gitkeep
-rwxrwxr-x 1 kira kira 1226 5月  26 09:01 git_keep_ignore.sh*
-rw-rw-r-- 1 kira kira  657 5月  26 08:54 Readme.md
```

* 对指定目录执行，执行命令时额外指定文件夹即可

```
[kira@kira-virtual-machine git]$ ./git_keep_ignore.sh -d ../
[x] git 
[x] template 
[x] tools 
[x] typora 
	[x] themes 
		[x] gitbook 
[x] 程序模板 
	[x] doc 
	[x] File_bak 
	[x] hardware 
	[x] img 
	[x] licenses 
	[x] program 
	[x] 参考文档 
	[x] 测试工具 
	[x] 发布版本 
[kira@kira-virtual-machine git]$ 
```

## 安装说明

## 使用说明

## 维护说明

## 注意

## 关于作者

>Author: KiraSkyler
>Email: kiraskyler@outlook.com / kiraskyler@qq.com

## 贡献者/贡献组织

## 鸣谢

## 版权信息

> 当前项目为私有项目，如果您在任何地方看到此项目，请联系作者确认合法性，未经作者允许对该文件除删除外所有操作均视为非法
>

## 更新日志

* V0.0.0.20210101