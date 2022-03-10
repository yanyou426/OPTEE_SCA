# 1.24-2.4 《软件分析》课程

## 01 Introduction

静态语言分析：分析一个程序，编译时分析，是否有private leak? null pointers? safe? dead code?但并不能准确判断是否满足这些性质！

sound：误报   complete： 漏报，只要有的肯定是错的

![image-20220124151715238](C:\Users\颜又和瓜蛋\AppData\Roaming\Typora\typora-user-images\image-20220124151715238.png)

尽可能sound，情愿误报多

abstraction+over-approximation

![image-20220124153247027](C:\Users\颜又和瓜蛋\AppData\Roaming\Typora\typora-user-images\image-20220124153247027.png)

![image-20220124153812030](C:\Users\颜又和瓜蛋\AppData\Roaming\Typora\typora-user-images\image-20220124153812030.png)

flow merging☆

## 02 Intermediate Representation

![image-20220124171609499](C:\Users\颜又和瓜蛋\AppData\Roaming\Typora\typora-user-images\image-20220124171609499.png)

三地址码的IR与语言无关，能看出控制流信息，静态分析的基础

![image-20220124214323493](C:\Users\颜又和瓜蛋\AppData\Roaming\Typora\typora-user-images\image-20220124214323493.png)

![image-20220124215227096](C:\Users\颜又和瓜蛋\AppData\Roaming\Typora\typora-user-images\image-20220124215227096.png)



Control flow analysis：

basic block：仅一个入口，仅一个出口

![image-20220124220754739](C:\Users\颜又和瓜蛋\AppData\Roaming\Typora\typora-user-images\image-20220124220754739.png)

![image-20220124222106876](C:\Users\颜又和瓜蛋\AppData\Roaming\Typora\typora-user-images\image-20220124222106876.png)



## 03 Data Flow Analysis I

reaching definitions: may analysis(不放过任何一个可能性。可能有一个都是不安全的，v会被kill掉)

![image-20220125124707385](C:\Users\颜又和瓜蛋\AppData\Roaming\Typora\typora-user-images\image-20220125124707385.png)



![image-20220125154051737](C:\Users\颜又和瓜蛋\AppData\Roaming\Typora\typora-user-images\image-20220125154051737.png)

其中out[B]不一定为空

![image-20220125155137365](C:\Users\颜又和瓜蛋\AppData\Roaming\Typora\typora-user-images\image-20220125155137365.png)

Iteration1：IN[B3] = 1011 0000 OUT[B3] = 00110010

结束后out存在变化，故继续迭代循环

最后的结果，就是可以看出哪些definition可以reach到这些basic block



重点：transfer function:

![image-20220125160643064](C:\Users\颜又和瓜蛋\AppData\Roaming\Typora\typora-user-images\image-20220125160643064.png)

**那么为什么这个算法一定会结束？**

out never shrinks

为什么没变化的时候就可以停止了？

达到不动点（out不变时in就不变，那么因此out也就不会变了）



## 04 Data Flow Analysis II

### Live variables analysis(backword analysis,是个may analysis)

![image-20220125215622764](C:\Users\颜又和瓜蛋\AppData\Roaming\Typora\typora-user-images\image-20220125215622764.png)

第一个公式写错了，是IN[S]；并且use中要看是不是在define之前被用到的，如例6，如果是在define之前用到的，那么就可以算进去



### Available expression analysis（**must analysis**）

![image-20220125221551885](C:\Users\颜又和瓜蛋\AppData\Roaming\Typora\typora-user-images\image-20220125221551885.png)

![image-20220125222017969](C:\Users\颜又和瓜蛋\AppData\Roaming\Typora\typora-user-images\image-20220125222017969.png)



### 总结一下

![image-20220125222943826](C:\Users\颜又和瓜蛋\AppData\Roaming\Typora\typora-user-images\image-20220125222943826.png)



## 05 Data Flow Analysis - Foundations I

### partial order

偏序关系

### Lattice（Semilattice）

一个偏序集的每对元素都有lub和glb

### Complete Lattice

任意子集都有lub和glb

整数集不是，比如所有正整数集没有lub

任意有穷的lattice都是complete lattice

### data flow analysis framework

![image-20220126140832854](C:\Users\颜又和瓜蛋\AppData\Roaming\Typora\typora-user-images\image-20220126140832854.png) 

### fixed-point theorem

到达的不动点为什么一定是最好的？单调性monotonicity 

![image-20220126143059474](C:\Users\颜又和瓜蛋\AppData\Roaming\Typora\typora-user-images\image-20220126143059474.png)

Proof：先证明存在，在证明确实是最小的

​		前提：单调有限全格

![image-20220126143500224](C:\Users\颜又和瓜蛋\AppData\Roaming\Typora\typora-user-images\image-20220126143500224.png)

![image-20220126143705879](C:\Users\颜又和瓜蛋\AppData\Roaming\Typora\typora-user-images\image-20220126143705879.png)



## 06 Data Flow Analysis - Foundations II

算法什么时候能达到不动点？h*k次（每轮都只迭代k个点中的一个0->1，故每轮要走k次，共要h轮）

### may analysis（lattice）

![image-20220126151101829](C:\Users\颜又和瓜蛋\AppData\Roaming\Typora\typora-user-images\image-20220126151101829.png)

越往上越不准，在准度和safe中求个权衡

### must analysis (lattice)

![image-20220126151544251](C:\Users\颜又和瓜蛋\AppData\Roaming\Typora\typora-user-images\image-20220126151544251.png)

### MOP

把每个path的结果join或meet起来

可能会有假path，即动态运行时永远不可能运行，故结果是not fully precise的

![image-20220126161812148](C:\Users\颜又和瓜蛋\AppData\Roaming\Typora\typora-user-images\image-20220126161812148.png)

MOP <= Ours，因为是往上跑，所以越下面越准，MOP更准

当F是distributive，MOP=Ours

之前讲过的bit-vector或gen/kill problem都是distrubutive的

### Constant propagation

must analysis

![image-20220126163223056](C:\Users\颜又和瓜蛋\AppData\Roaming\Typora\typora-user-images\image-20220126163223056.png)



why nondistributive？

![image-20220126163421318](C:\Users\颜又和瓜蛋\AppData\Roaming\Typora\typora-user-images\image-20220126163421318.png)

第一个空是NAC ×  ![image-20220126163552078](C:\Users\颜又和瓜蛋\AppData\Roaming\Typora\typora-user-images\image-20220126163552078.png)

第二个是10? × ![image-20220126163656812](C:\Users\颜又和瓜蛋\AppData\Roaming\Typora\typora-user-images\image-20220126163656812.png)

故我们的<=MOP，所以我们的在must analysis中是在下面，更不准。故MOP是更准的



### Worklist algorithm

只计算有改变的OUT[B]

![image-20220126164319910](C:\Users\颜又和瓜蛋\AppData\Roaming\Typora\typora-user-images\image-20220126164319910.png)



## 第一次作业  Constant propagation





## 07 Interprocedural Analysis

### Dispatch

![image-20220126220922317](C:\Users\颜又和瓜蛋\AppData\Roaming\Typora\typora-user-images\image-20220126220922317.png)

### CHA

![image-20220126221551629](C:\Users\颜又和瓜蛋\AppData\Roaming\Typora\typora-user-images\image-20220126221551629.png)

special call: 构造函数 私有 父类

![image-20220126221645831](C:\Users\颜又和瓜蛋\AppData\Roaming\Typora\typora-user-images\image-20220126221645831.png)

第一个就是c.foo

第二个：a.foo c.foo d.foo

第三个：a.foo **c.foo d.foo**(一个点做dispatch 是需要对他自己本身及所有的子类进行dispatch，他自己本身的是a.foo)

**注意：只考虑申明的类，即最前面的那个类**

不准，因为会有很多假的目标函数

### Call graph

![image-20220126223113493](C:\Users\颜又和瓜蛋\AppData\Roaming\Typora\typora-user-images\image-20220126223113493.png)

![image-20220127113832801](C:\Users\颜又和瓜蛋\AppData\Roaming\Typora\typora-user-images\image-20220127113832801.png)



### ICFG（interprocedural）

 ![image-20220127120158760](C:\Users\颜又和瓜蛋\AppData\Roaming\Typora\typora-user-images\image-20220127120158760.png)

而做过程内分析时，都做最保守假设，即尽量都是NAC



## 08 Pointer Analysis

### problem of CHA

![image-20220127125422940](C:\Users\颜又和瓜蛋\AppData\Roaming\Typora\typora-user-images\image-20220127125422940.png)

### Key factors

![image-20220127131458766](C:\Users\颜又和瓜蛋\AppData\Roaming\Typora\typora-user-images\image-20220127131458766.png)

![image-20220127132220116](C:\Users\颜又和瓜蛋\AppData\Roaming\Typora\typora-user-images\image-20220127132220116.png)

![image-20220127132533609](C:\Users\颜又和瓜蛋\AppData\Roaming\Typora\typora-user-images\image-20220127132533609.png)

### Allocation-site abstraction：

​		对象o在循环中若一直在创建，那么实际只创建一个o

 

### Context sensitivity：

​		同一个方法在上下文中分开分析,更精确

![image-20220127130812386](C:\Users\颜又和瓜蛋\AppData\Roaming\Typora\typora-user-images\image-20220127130812386.png)

### flow sensitivity：

流不敏感（右边）不精确，但是目前没有证明左比右好

![image-20220127131149975](C:\Users\颜又和瓜蛋\AppData\Roaming\Typora\typora-user-images\image-20220127131149975.png)

### analysis scope：

![image-20220127131420503](C:\Users\颜又和瓜蛋\AppData\Roaming\Typora\typora-user-images\image-20220127131420503.png)

### Rules

![image-20220127133338232](C:\Users\颜又和瓜蛋\AppData\Roaming\Typora\typora-user-images\image-20220127133338232.png)



## 09 Pointer Analysis - Foundations I 

### PFG

![image-20220127150616433](C:\Users\颜又和瓜蛋\AppData\Roaming\Typora\typora-user-images\image-20220127150616433.png)

### Algorithm

![image-20220127215803504](C:\Users\颜又和瓜蛋\AppData\Roaming\Typora\typora-user-images\image-20220127215803504.png)

![image-20220127220055960](C:\Users\颜又和瓜蛋\AppData\Roaming\Typora\typora-user-images\image-20220127220055960.png)



![image-20220127221444977](C:\Users\颜又和瓜蛋\AppData\Roaming\Typora\typora-user-images\image-20220127221444977.png)

### 	Example![image-20220127224243683](C:\Users\颜又和瓜蛋\AppData\Roaming\Typora\typora-user-images\image-20220127224243683.png)

### **重点！！！**自己再做一遍！！！



## 10 Pointer Analysis - Foundations II

![image-20220128111826583](C:\Users\颜又和瓜蛋\AppData\Roaming\Typora\typora-user-images\image-20220128111826583.png)

### Algorithm

![image-20220128114524996](C:\Users\颜又和瓜蛋\AppData\Roaming\Typora\typora-user-images\image-20220128114524996.png)

### Example(重要！)：全程序跨函数指针分析

![image-20220128114925061](C:\Users\颜又和瓜蛋\AppData\Roaming\Typora\typora-user-images\image-20220128114925061.png)

![image-20220128124230484](C:\Users\颜又和瓜蛋\AppData\Roaming\Typora\typora-user-images\image-20220128124230484.png)

![image-20220128124506916](C:\Users\颜又和瓜蛋\AppData\Roaming\Typora\typora-user-images\image-20220128124506916.png)

![image-20220128125019370](C:\Users\颜又和瓜蛋\AppData\Roaming\Typora\typora-user-images\image-20220128125019370.png)

![image-20220128125215681](C:\Users\颜又和瓜蛋\AppData\Roaming\Typora\typora-user-images\image-20220128125215681.png)

## 11 Pointer Analysis - Context Sensitivity I

本质就是带个上下文的“行号“

![image-20220128143211657](C:\Users\颜又和瓜蛋\AppData\Roaming\Typora\typora-user-images\image-20220128143211657.png) 



## 12 Pointer Analysis - Context Sensitivity  II

### 3种sensitivity

![image-20220128171704704](C:\Users\颜又和瓜蛋\AppData\Roaming\Typora\typora-user-images\image-20220128171704704.png)

call-site:

每次调用时，加个调用点的行数 k-limit限制



### Algorithm&1-call-site Example

![image-20220128172619860](C:\Users\颜又和瓜蛋\AppData\Roaming\Typora\typora-user-images\image-20220128172619860.png)

![image-20220128212156811](C:\Users\颜又和瓜蛋\AppData\Roaming\Typora\typora-user-images\image-20220128212156811.png)

### Object/call-site/type sensitivity

![image-20220128220924341](C:\Users\颜又和瓜蛋\AppData\Roaming\Typora\typora-user-images\image-20220128220924341.png)

![image-20220128221719872](C:\Users\颜又和瓜蛋\AppData\Roaming\Typora\typora-user-images\image-20220128221719872.png)

call-site中产生假的指向关系

![image-20220128223052448](C:\Users\颜又和瓜蛋\AppData\Roaming\Typora\typora-user-images\image-20220128223052448.png)



## 13 Static Analysis for Security

information leak 



taint analysis:

![image-20220201145938612](C:\Users\颜又和瓜蛋\AppData\Roaming\Typora\typora-user-images\image-20220201145938612.png)

由输出可以看出，可以看到程序中危险的信息流



## 15 CFL-Reachability and IFDS

###   括号匹配

## 16  Soundness and Soundiness

Soundy:我们尽可能的认为是完整的



# 2.15 VPN

保密： 数据加密一层，隐藏了源地址和目标地址

保护数据完整性：hash

认证：RSA

SSL/TLS：使用可以VPN的浏览器



# 2.15 endnote分享

endnote导出文献？

 



# 2.15 开题报告怎么写捏

https://www.bilibili.com/video/BV1Ng411w7Yb?from=search&seid=3090615100357045264&spm_id_from=333.337.0.0

研究背景：1.1研究这个问题是什么，重要性，目前是如何实现的

​					1.2研究意义（理论意义，实践意义）

文献综述

研究的主要内容（用什么方法，研究什么问题）以及关键点（个人理解就是难点）、创新点

研究方案和可行性分析





