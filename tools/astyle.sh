# 参考网站 http://astyle.sourceforge.net/astyle.html
# 作者 kiraskyler@qq.com
# 版本 V0.0.5.20210611

astyle "*.c,*.cpp,*.h"\
	--recursive\
	--style=bsd\
	--convert-tabs\
	--indent=spaces=4\
	--attach-closing-while\
	--indent-switches\
	--indent-namespaces\
	--indent-continuation=4\
	--indent-preproc-block\
	--indent-preproc-define\
	--indent-preproc-cond\
	--indent-col1-comments\
	--pad-oper\
	--unpad-paren\
	--pad-comma\
	--align-pointer=type\
	--align-reference=name\
	--break-one-line-headers\
	--remove-braces

# --recursive						遍历，如果是指定文件不选带上这个
# --style=bsd\						花括号换行，不换行的都是邪教！
# --convert-tabs\					把tab换成空格
# --indent=spaces=4\				一个tab换4个空格
# --attach-closing-while\			do-while循环时，while紧跟花括号		
# --indent-switches\				switch 中的 case 相比 switch 缩进靠后一个tab
# --indent-namespaces\				namespaces 源码有 namespaces 字样时，其他代码
#										在这个字段后缩进一个tab，编程语言没有 namespaces  概念的就不要凑热闹了
# --indent-continuation=4\			一行未完，使用 '|' 等衔接时，向后缩进几个tab
# --indent-preproc-block\			在预编译字段，通常是 #define 中使用tab缩进
# --indent-preproc-define\			在一行未完成的 #define 中使用tab缩进每行，不至于显得像一行的内容
# --indent-preproc-cond\		    在代码中插入的 宏定义 使用tab与代码对齐，而不是左对齐
# --indent-col1-comments\			代码中的注释，与代码缩进一致，而不是左对齐
# --pad-oper\						操作符两边加空格，不加空格的都是斗鸡眼吗
# --unpad-paren\				    括号周围的空格去掉
# --pad-comma\						逗号后加空格
# --align-pointer=type\				指针变量 '*' 和 '&' name对齐到名字，而不是变量后，type 对齐到变量
# --align-reference=name\			指针变量 '&' 与名字对齐
# --break-one-line-headers\			花括号放到下一行去
# --remove-braces 					删除if后为单行时括号
