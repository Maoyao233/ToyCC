# ToyCC

## ��Ŀ���

����Ŀ����[Maoyao233](https://github.com/Maoyao233)��[lhl322001](https://github.com/lhl322001)��[Amazonite](https://gitee.com/amazonite)������ɵ�ͬ�ô�ѧ����ԭ��CS100395���γ̴���ҵ�ĺ�˲��֣����C���Ե�һ���Ӽ���ʵ���˼��׵ı�����ǰ�ˣ������ʷ��������﷨����������������м�������ɲ��衣�����������`LLVM IR`��Ҳ���Ե�����ʾ�ʷ��������ֻ���json��ʽ���������﷨����AST������������ҵǰ��ʹ��`electron`ʵ�֣���ҪΪ�������Ŀ��ӻ���

## ��ʾ

![help](readme_resources/help.png)

����ʹ�ü򵥵Ĳ��Գ���`test.c`��

```C
int gcd(int a, int b)
{
    if(b==0)
        return a;
    return gcd(b, a % b);
}
```

�ʷ�������

![tokens](readme_resources/tokens.png)

�����﷨����
![ast](readme_resources/ast.png)

�м���룺
![ir](readme_resources/ir.png)

��һ���أ����ǿ���ʹ��`llc`��`LLVM IR`����ΪĿ���ļ������У��Լ�������ȷ�ԡ�

����ʹ��`main.c`��

```C
#include <stdio.h>
int gcd(int, int);
int main()
{
    int a, b;
    scanf("%d%d", &a, &b);
    printf("%d\n", gcd(a, b));
}
```

![test](readme_resources/test.png)

## ����

�Ƽ���Linux��MacOS�±��뱾��Ŀ�������Windows�±��룬��Ҫ��Դ�����LLVM����ʱ�ϳ��Ҵ���ʧ�ܵĿ����ԡ�

�Ƽ�ʹ��[CMake](https://cmake.org/)��[vcpkg](https://github.com/microsoft/vcpkg)������Ŀ����

��δ��װ`vcpkg`������Ӧ��װ`vcpkg`��

Linux:
```bash
git clone https://github.com/microsoft/vcpkg.git
cd vcpkg
sh bootstrap-vcpkg.sh
```
Windows:

��`powershell`��ִ�У�
```powershell
git clone https://github.com/microsoft/vcpkg.git
cd vcpkg
.\bootstrap-vcpkg.bat
```

Ȼ��ͨ��`vcpkg`��װ����Ŀ��������
```bash
./vcpkg install nlohmann-json
```

LLVM������Windows��װ�汾��ȱ�ٱ���Ŀ������ͷ�ļ�����ļ�����˱����Դ����롣����ʱ����`vcpkg`��ɣ�
```powershell
.\vcpkg install llvm[core,target-x86]
```

ע�⣬�˹��̿��ܷǳ������������ĵȴ���ͬʱ��Debugģʽ������ڴ���������ܴ����ýϲ�ĵ����¿��ܻ��ڴ�ľ�������ʧ�ܡ���ʱ���Կ���[�ֶ���Դ�����](https://llvm.org/docs/GettingStarted.html)��

�����Linux��MacOS�£�����ֱ��ʹ��LLVM��pre-built�汾�Ӷ�ʡ����һ���Ӽ��ѵĹ��̡�������`Ubuntu`�£�ֻ��ʹ�ã�

```bash
sudo apt install llvm
```

����ʹ�á�

����Ŀ��Ӧ��`vcpkg`�ķ�����ο�[�ٷ��ĵ�](https://github.com/microsoft/vcpkg/blob/master/README_zh_CN.md#%E5%9C%A8-cmake-%E4%B8%AD%E4%BD%BF%E7%94%A8-vcpkg)���Ƽ���`vcpkg`��Ϊ��Ŀ��ģ�飬���ڱ���Ŀ�� [CMakeLists.txt](CMakeLists.txt) �е�`project()`����ǰ�����������ݣ�

```CMake
set(CMAKE_TOOLCHAIN_FILE path/to/vcpkg/scripts/buildsystems/vcpkg.cmake
  CACHE STRING "Vcpkg toolchain file")
```

����`path/to/vcpkg`Ϊ`vcpkg`��װλ�á�

������ɺ󣬼���ʹ��`CMake`��

Visual Studio:
```bash
mkdir build
cd build
cmake .. -G "Visual Studio 16 2019" 
```

MinGW:
```bash
mkdir build
cd build
cmake .. -G "MinGW Makefiles" 
```

UNIX:
```bash
mkdir build
cd build
cmake .. -G "Unix Makefiles" 
```

## ֧�ֵ��﷨

Ŀǰ����Ŀ��ʵ����C���Ժ�С��һ���Ӽ���

֧�ֵĹؼ��ְ�����
+ int
+ void
+ if
+ else
+ while
+ return

֧�ֵ������������
+ һԪ/��Ԫ�����������+��-��*��/��%�ȣ�
+  ��ϵ�������==��!=��<��<=��>��>=��
+ �߼��������&&��||��!��
+ λ�������&��|��^��
+ ��λ�����
+ ��ֵ�������=��
+ ���ϸ�ֵ�������+=��&=��<<=�ȣ�
+ ����

�������������磺

+ ����/����ע��
+ ������ʼ��
+ ����
+ �ݹ����

������ҵ���ʱ���Լ�����ˮƽ����Ŀ�����ط�ʵ�ֿ��ܲ��������д��ڼ����Ľ���