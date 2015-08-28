名称： Arsenal

目标组件：
  * 可配置的词法分析器
  * 可配置的LR-Parser
  * 相关设计，测试工具

已完成组件：
  * 可配置的词法分析器
  * 可配置的LR-Parser，支持SLR(1),LALR(1)
  * 文法设计工具
    1. BNF Compiler,生成SLR(1),LALR(1)分析表
    1. 内建错误处理
    1. 实时观测语法及其分析树
    1. 实时报告分析表，冲突，First Follow集合以及左递归，左因子
    1. ![http://arsenalcomp.googlecode.com/files/y1.jpg](http://arsenalcomp.googlecode.com/files/y1.jpg)
    1. ![http://arsenalcomp.googlecode.com/files/yy.jpg](http://arsenalcomp.googlecode.com/files/yy.jpg)


目录结构：
  1. ./Arsenal          : Arsenal库代码
  1. ./Tools/Win        : Windows版工具
  1. ./Tools/Mac        : Mac版工具
  1. ./Prj              : 各种编译器的工程
  1. ./misc             : 一些测试用的文法以及工具和一部分语言的yacc版.
  1. ./binary/x86       : 编译后，生成的x86平台下的dll release版，其余binary下结构与其类似,
  1. ./temp             : 编译器生成的临时文件，例如obj等.

词法分析部分：
  1. 例子    ：请参考./misc/grammar/中的例子.
  1. 技术细节: 基于NFA的正则表达式引擎，支持正向预搜索，逆向预搜索，贪婪，非贪婪，循环等操作，支持SINGLELINE IGNORECASE模式，不支持MULTILINE，"^" "$"永远匹配整个输入的首尾，但是扩展了关键字"\B" "\E"以便匹配行首尾，"."在默认不匹配"\n",SINGLELINE下匹配所有字符。

语法分析部分：

  * 目标： 在最开始我并没有想把这两样东西通用化，仅仅想做一个比较精简的基于递归下降技术的类C解释器（应该会去掉很多晦涩的C语法支持），但是在实践过程中，我发现需要一些工具，例如便于实时设计和修正文法以及观测此文法接受的语言和其具体语法树（包括错误处理之后的）等等，因此才决定实现一个可配置的LR分析器。另外，一个被常问到的问题是，自己写一个类似yacc的库有什么用处？我只能说，这个东西可以轻易的改装成任何我需要的东西，不论是对UI的友好程度还是线程安全性以及其执行时的解释性质，至少很难拿yacc做个像样的设计工具出来。

  * 实现：
    1. 实现简单的说就是个LR理论的瘦封装，生成Action和Goto表，之后每个产生式注册一个语义动作，收集节点，自底向上最终生成一颗AST。

  * 错误处理：我采用的是和yacc相当类似的一种技术--error token，首先error作为保留终结符，当有错误发生时，直接检索当前Action Table是否有可以移入error，如果有，则移入，否则，向上pop\_stack，直到可以接受终结符error为止，此时parser状态进入error状态，在此状态下任何非法的输入符号均被丢弃，直到移入了一个在error上合法的符号（这里并非是终结符，请看例子），此时恢复到正常状态。如果任何错误发生时直到parser-stack空为止都不能接受终结符error，则分析失败，否则带error的产生式会被当作正常产生式处理，可以在规约时处理这条错误产生式。例如A -> error ';'；当有任何错误发生时，parser会移入error直到移入';'后才会恢复到正常状态。同样的道理， A -> error TERM; TERM -> ';';同样可以接受，实际上和上面的error -> ';'相同，只不过先做一次';'到TERM的规约动作，当然 A->error同样可以接受，只是错误状态被传递到A之后的符号，也可以在分析中清除Error状态，例如在某个ActionHandler中调用PSR\_ClearError(...);

  * 优先级，结合性：同样和yacc的准则类似，优先选择移入，每条产生式的优先级默认是其最右终结符号，也可以单独指定产生式的优先级以及结合性。例如：
```

%left '+', '-';

%left '*', '/', '%';

%right UMINUS;

E -> E '+' E | E '-' E | E '*' E | E '/' E | E '%' E | '-' E %prec UMINUS | '(' E ')';

```

> 这里，配置起来的 前两条产生式的优先级结合性是其最右终结符，也就是`'+' '-'`, 之后三条是`'*' '/' '%'` ,在
```
E -> '-' E %prec UMINUS
```

> 中，其优先级结合性取自符号UMINUS，而最后一个
```
E->'(' E')'
```

> 的优先级为默认的，默认为0和无结合性。

  * 实例：先简单说下具体用法，因为采用的是表驱动的Shift-Reduce Parser，所以Parser的接口提供了一个名为PSR\_AddToken的函数，每次调用处理一个词法值，直到移入它，此函数返回，当EOI被接受后，此Parser状态为接受状态，所以实用中是词法分析器调用语法分析器。关于语法树的构建实际上是靠针对每条产生式注册一个相关的Action-Handler来完成的，详情请参考实例：./Arsenal/Tools/grammar\_config.c,里面实现一个完整的基于Arsenal的语法配置程序.一个完整的应用Arsenal的例子是./Tools/GrammarDesigner/,一个基于MFC的文法设计工具，可以用其观测大部分文法的具体语法树。基于时间关系，文档也不可能写的相当详细，而且代码还处于不断的修改中，所以有人对其有兴趣的话可以联系我。

  * 错误报告： 这是个不太好设计的部分，暂时权宜之计是在AR\_CommonInit中注册统一的error以及print handler，并由统一的AR\_error和AR\_printf调用

```
 
typedef void	(AR_STDCALL *AR_error_func_t)(int_t level, const wchar_t *msg, void *ctx);
typedef void	(AR_STDCALL *AR_print_func_t)(const wchar_t *msg, void *ctx);


typedef struct __arsenal_io_context_tag
{
		AR_error_func_t	on_error;
		AR_print_func_t	on_print;
		void			*ctx;
}arIOCtx_t;

void foo()
{
   byte_t *p = AR_NEWARR(byte_t, 1024);
   if(p == NULL)
   {
       AR_error(AR_ERR_FATAL, L"%ls\r\n", L"mem low");
   }     
}

```

另外各个模块有其自身的错误报告回调接口。


  * 移植：
    1. 因为此类程序对系统调用几乎没有依赖，所以移植起来相对简单，这里比较棘手的问题是在某些编译器下缺乏对于wchar\_t以及相关CRT库函数的支持，所以所有被引用到的CRT函数已被隔离在common.h中，以AR\_为前缀，如果移植的平台编译器等不支持相关函数，在必要时可能要单独实现一套。未来可能会把所有IO部分的CRT都重写为独立的版本，因为类似sprintf一族的函数缺乏统一标准，暂时是移植方面(甚至是编译器间)最大的麻烦。
    1. thread.c中提供了靠CAS指令实现的spinlock，以提供Arsenal库在多线程环境下的安全初始化以及某些数据结构池的分配。
    1. 请留意基本数据类型的字长，例如int\_t被定义为和目标平台字长相同，而并非32bit，而其它相关数值类型都由其后缀决定，例如int\_8\_t int\_16\_t int\_32\_t int\_64\_t等。wchar\_t尤其相关编译器决定，所以永不少于16bit。
    1. 我的主要开发工具是VS2K8和xcode3.2.6，Arsenal由ANSI C编写，在VS2K8以及xcode3.2.6下可以完美的编译为x86,X64并通过测试,另外Prj目录中有其它平台及编译器的工程。支持gcc4.x版本，在ubuntu 11.04下编译通过。

  * 一些问题：
    1. 这种可配置的词法和语法分析器本质上都是通过模式构建一个状态机，就像不良的程序导致错误一样，这里有些不良的文法会导致它们生成死循环，很遗憾我没办法在编译时检测它们，一个词法模式的例子是比如（{name1}|{name2}|^)+这种模式导致的死循环，很难在对表达式做语法分析时检测并报告它们。而语法分析器部分相对要好一些，但是存在一种可能，导致在用error 尝试容错的时候变成无穷reduce空Rule：首先尝试shift error，之后reduce Epsilon,但是shift Epsilon之后的状态并不能shift error，这里会弹出Epsilon，继续尝试shift error，这就是又一个死循环了。因为中间不一定存在多少reduce操作，所以几乎没办法知道如何从这个循环跳出去。
    1. 另一个被常提及的问题是，例如如何描述C语言的声明和typedef等，例如typedef int int\_t; int\_t x;诸如此类，当type\_specifier -> ID的会产生冲突的问题，这里的答案是没有办法，必须借助符号表，因为C是上下文相关语言，typedef 改变的是作用域内一个符号的词法值，所以即使GLR也一定在某个时刻按照一定的优先级剪枝。从这个例子可以看到，类似C的声明，必须在shift token前执行一次符号表的查找。

联系方式:
  1. Email：[SolidusSnakeEx@gmail.com]
  1. GTalk: [SolidusSnakeEx@gmail.com]
  1. QQ   : 2070341












