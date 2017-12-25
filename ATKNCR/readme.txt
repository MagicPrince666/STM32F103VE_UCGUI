本数字字母识别程序由ALIENTEK提供,我们提供2个LIB,供大家使用

ATKNCR_M_Vx.x.lib和ATKNCR_N_Vx.x.lib的唯一区别是是否使用动态内存分配.
其中:M,代表需要用到malloc的版本,必须实现alientek_ncr_malloc和alientek_ncr_free两个函数
     N,代表普通版本,不需要实现alientek_ncr_malloc和alientek_ncr_free两个函数
     Vx.x,代表当前识别程序的版本.	
	 	  
功能:支持数字/小写字母/大写字母/混合四种识别模式.		  
本识别程序使用起来相当简单.					   
第一步:调用alientek_ncr_init函数,初始化识别程序
第二步:获取输入的点阵数据(必须有2个及以上的不同点阵数据输入)
第三步:调用alientek_ncr函数,得到识别结果.
第四步:如果不需要再识别,则调用alientek_ncr_stop函数,终止识别.如果还需要继续,则重复2,3步即可.	


                                              版本:V2.0
                                              正点原子@ALIENTEK
                                              Copyright(C) 广州市星翼电子科技有限公司 2009-2019
                                              All rights reserved

地址：广州市白云区北太路1633号民营科技园科盛路1号白云火炬大厦1801 
传真：020-36773971
电话：020-38271790
网址：www.alientek.com
论坛：www.openedv.com	