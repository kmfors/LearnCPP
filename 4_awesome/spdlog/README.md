
wincolor_stdout_sink_mt 与 ansicolor_stdout_sink_mt 区别：

二者都是用于在 Windows 操作系统上为控制台输出添加颜色支持的日志处理器。它们的主要区别在于它们使用的ANSI转义序列不同，因此适用于不同的终端类型。

- wincolor_stdout_sink_mt：这个处理器使用 Windows 特有的 ANSI 转义序列，这些序列在旧版本的 Windows控制台中可能无法正常工作。然而，从 Windows10 开始，Windows 控制台已经支持ANSI转义序列，因此这个处理器可以在新版本的 Windows 控制台中正常工作。
- ansicolor_stdout_sink_mt：这个处理器使用标准的 ANSI 转义序列，这些序列在所有支持 ANSI 转义序列的控制台中都可以正常工作，包括 Linux 和 macOS 上的终端。

总的来说，如果你的目标平台是 Windows10 或更高版本，建议使用 wincolor_stdout_sink_mt，因为它可以直接利用Windows控制台的支持。如果你的目标平台是 Linux 或 macOS，建议使用 ansicolor_stdout_sink_mt，因为它在这些平台上有更好的兼容性。


debug与info的实验发现，只有在创建日志时的等级级别才起到作用，而不受项目是debug还是release的影响。

即使没有定义全局变量，依旧可以使用MLOGR，因为它里面会判断有没有生成这样的一个日志对象。
