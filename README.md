# ProcessReader
欢迎使用ProcessReader库！

该库中提供的Reader类可以帮助您读取进程，获取进程的初始静态地址。

# API与使用方法
## 基本使用
库中的类通过reader命名空间进行一层封装，因此在使用之前，您需要进行命名空间的声明。例如：

`reader::Reader CSGO_Reader("Counter-Strike: Global Offensive - Direct3D 9", "Valve001");`

命名空间    类名        自定义的名称(进程的标题，进程的类);

从而完成对CSGO进程的读取。

## 如何获取该进程的标题和类
此处以读取CSGO进程为例：

通过Visual Studio自带的

“工具” -> 

“Spy++(+)” -> 

按住Alt+F3 -> 

“查找程序工具”旁边的准心，可以任意拖动，将其拖动到CSGO的窗口上，便会自动读取出CSGO的标题全程和类。

将读取内容输入到类的初始化参数中，即可完成对该目标进程的读取。

## Reader类中的属性与方法
### 属性
```
int pid;	                        //进程里面有记录ProcessID的，这个就是用来存的
HANDLE target_Process_Recorder;		//用来记录进程的句柄
uintptr_t target_address_Recorder = 0x0;	//用来记录进程的静态地址的起始地址（都是从这里开始偏移）
```
其中使用最多的是：target_address_Recorder

因为大多数游戏逆向、进程读取中都会使用到静态基址：client.dll，此处保存的就是client.dll的地址。

您可以使用`CSGO_Reader.target_address_Recorder`来随时调用、查看这个地址。
### 方法
1. read\<T>(uintptr_t address)
    
    该方法主要用于读取某个特定地址的内容，传入的地址往往是基址+偏移（游戏逆向）

    例如我需要读取人物的血量地址，就有：

    `int health = CSGO_Reader.Read(CSGO_Reader.target_address_Recorder + health_offset);`

    其中health_offset就是血量地址相对于client.dll的偏移，需要使用者自行通过CE、OD等工具进行寻址。具体方法此处不再赘述。

2. read_list\<T>(uintptr_t address, T* t, SIZE_T count)

    第一个参数是要读取的地址

    第二个参数是需要读取的数据的被存储的地址（一般通过xx.data()方法来获取）
    比如我需要将数据存储到一个数组中，这里就可以是该数组的首地址。

    第三个参数是需要读取的大小

    read_list()通常用于读取人物的视角矩阵、人物列表等。可以通过：

    ```
    CSGO_Reader.read_list<player_44_matrix>(
		CSGO_Reader.target_address_Recorder + entity_list_offset, 
		player_address_list.data(), 
		player_address_list.size()
		);
    ```

    entity_list_offset是队伍地址的偏移，player_address_list是存储的数组的名字，此处只是举例。

3. read_chain\<T>(uintptr_t address, std::vector<uintptr_t> vec, T &output)

    该方法用于读取一串基址，在逆向过程中，经常会发现有一些地址存储的内容恰恰也是地址，比如CSGO的人物信息地址头，会被单独提出来，统一放置。该函数便是用于进行地址链表提取的。

    第一个参数传入client.dll即可。

    第二个参数是一个存地址的数组。

    第三个参数是输出的位置。

    该函数会依照地址数组中的每个元素挨个进行偏移读取。