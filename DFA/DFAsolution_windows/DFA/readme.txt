*DFA使用说明**/
 
1，规则表达式说明：
 元符号说明：
    [] 0个或1个
    <> 1个或多个
    {} 0个或多个
    $* 变量 （注意变量名后需要有空格）
    () 整体优先
    |  或
    \ 为转义字符(须要转义的有 空格,tab,$,-,(,),{},<>,[],|,@)
    //为注释行
 最后一行为命令行，须以类似($email:100|$ipadd:101)格式，表示识别$email和$ipadd,返回值分别是100和101
 括号中不能包含非变量，其中:\d+为返回值
  
  具体参见default.txt的格式
 
2,程序使用说明(linux版本)
  (/home/zhangbo/finitemachine/bin)
  1,./dfabuild  <规则文件_in> <自动机文件_out>
  
  2,./find  <自动机文件_in> <文本文件_multi_in> <结果文件>
 
3,源码使用

CNFABuilder类提供
(/home/zhangbo/finitemachine/dfabuilder)
 
  //生成自动机文件程序
 输入资源文件：/home/zhangbo/finitemachine/bin/default.txt
  
  LoadRuleFile(...); //调入规则文件,并初始化NFA
  
  CreateNFA2DFA(...);//将NFA转换成DFA
  
  SaveDFAtoFile(...);//DFA存成二进制文件：存储格式为【头结点】【-1】【..自动机结构体】
  
  Release();//释放空间
  
  具体使用代码参见/home/zhangbo/finitemachine/dfabuilder/dfabuild.cpp
  
  //查找接口文件
  (/home/zhangbo/finitemachine/demo)
   输入资源文件：/home/zhangbo/finitemachine/bin/mydfa.net

  CSegRule * NFAB=CreateSegRuleInstance()//创建自动机实例
  
  int len=NFAB->Search(&nBuf[j],nCovlen-j,&retnode);//查找并返回结果长度以及结果结构体
  
  Release(NFAB);//空间释放
  
  具体使用代码参见/home/zhangbo/finitemachine/demo/find.cpp
  
4,程序效率
  80M新闻语料,查找所有的数字,英文,电话,邮箱,ip地址,空格,共查得2761378个结果，平均耗时7s
  机器2G内存,查找速度约为 10M/s