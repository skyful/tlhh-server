# tlhh

#### 介绍
    打算基于c++11标准做一个跨平台的通用服务器框架，其中将要集成socket、mysql、redis以及多进程通信，项目正在一步步完善中。现在已经实现了socket处理，mysql处理。数据库和socket都是用了asio异步模型，逻辑处理部分使用单线程是为了避免多线程导致的线程安全的问题，将来实现了进程间通信之后可以实现单个线程多个进程的模式，提高运行效率。
    
    如果有幸哪位大牛看到了我的项目，并且更荣幸的是大牛看了我的代码并且发现设计不足以及代码的bug，可以给我提些建议，非常感谢！

    生命不息，学习不止！共勉

    socket通信参考了https://github.com/YuWenHaiBo/brink
    


#### 软件架构
软件架构说明


#### 安装教程
windows：
使用vs2015编译，需要自己修改mysql和boost库的路径

linux：
编译实用cmake，本人cmake也不熟，所以使用cmake将就能编译通过，同样需要配置mysql和boost库

#### 使用说明

1.  
2.  
3.  

#### 参与贡献

1.  Fork 本仓库
2.  新建 Feat_xxx 分支
3.  提交代码
4.  新建 Pull Request


#### 特技

1.  使用 Readme\_XXX.md 来支持不同的语言，例如 Readme\_en.md, Readme\_zh.md
2.  Gitee 官方博客 [blog.gitee.com](https://blog.gitee.com)
3.  你可以 [https://gitee.com/explore](https://gitee.com/explore) 这个地址来了解 Gitee 上的优秀开源项目
4.  [GVP](https://gitee.com/gvp) 全称是 Gitee 最有价值开源项目，是综合评定出的优秀开源项目
5.  Gitee 官方提供的使用手册 [https://gitee.com/help](https://gitee.com/help)
6.  Gitee 封面人物是一档用来展示 Gitee 会员风采的栏目 [https://gitee.com/gitee-stars/](https://gitee.com/gitee-stars/)
