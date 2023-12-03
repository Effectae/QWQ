#include <iostream>
#include <stdio.h>
#include <sys/fcntl.h>
#include <sys/ioctl.h>
// 这里是结构体头文件
#include "copy_memory.h"
#include "module_base.h"
// 这里是结构体头文件
using namespace std;

// 宏定义区域
#define DEV_PATH "/dev/"              // 这条是dev路径
#define DEV_NAME "wanbai"             // 这条是节点名称
#define DEV_CONNET_PATH "/dev/wanbai" // 这个是连接路径
#define OP_INIT_KEY 0x800
#define OP_READ_MEM 0x801
#define OP_WRITE_MEM 0x802
#define OP_MODULE_BASE 0x803
// 宏定义区域

// 驱动类
class ReadDriver
{
private:
    string connetDevPath;
    int fd;
    int pid;

public:
    ReadDriver()
    {
        string devPath = DEV_PATH;
        string devName = DEV_NAME;
        this->connetDevPath = devPath + devName;
        cout << "驱动路径是" << this->connetDevPath << endl;
        fd = open(DEV_CONNET_PATH, O_RDWR);
        if (fd == -1)
        {
            printf("[-] open driver failed\n");
        }
    }
    ReadDriver(char *devName, int pid)
    {
        string devPath = DEV_PATH;
        this->connetDevPath = devPath + devName;
        cout << "驱动路径是" << this->connetDevPath << endl;
        fd = open(devName, O_RDWR);
        if (fd == -1)
        {
            printf("[-] open driver failed\n");
        }
        cout << "您使用的是有参构造" << endl;
    }
    ~ReadDriver()
    {
        if (fd > 0)
        {
            close(fd);
        }
        cout << "析构函数已调用 您本次的使用驱动已结束 作者吴迪:忘川" << endl;
    }
    template <class T>
    T getPid()
    {
        if (sizeof(T) == sizeof(int))
        {
            return this->pid;
        }
        else
        {
            cout << "pid是int类型的请用int类型变量接收" << endl;
            return -1;
        }
    }
    template <class T>
    T getConnetDevPath()
    {
        if (sizeof(T) == sizeof(string))
        {
            return this->getConnetDevPath;
        }
        else
        {
            return -1;
        }
    }
    void getProcessPid(char *name)
    {
        FILE *fp;
        int pid;
        char cmd[0x100] = "pidof ";

        strcat(cmd, name);
        fp = popen(cmd, "r");
        fscanf(fp, "%d", &pid);
        pclose(fp);
        this->pid = pid;
    }
    uintptr_t getModuleBase(char *name)
    {
        MODULE_BASE mb;
        char buf[0x100];
        strcpy(buf, name);
        mb.pid = this->pid;
        mb.name = buf;

        if (ioctl(this->fd, OP_MODULE_BASE, &mb) != 0)
        {
            return 0;
        }
        return mb.base;
    }

    template <class T>
    T read(uintptr_t addr)
    {
        T res; // 创建一个跟你变量类型相同的变量 最后会返回这个
        driverRead(addr, &res, sizeof(T));
        return res;
    }

    long int lsp32(long int addr)
    {
        long int res = 0;
        driverRead(addr, &res, 4);
        return res;
    }

private:
    template <class T>
    void driverRead(uintptr_t addr, T *res, size_t size)
    {
        COPY_MEMORY cm;
        cm.pid = this->pid;
        cm.addr = addr;
        cm.buffer = res; // 这里是一个指针 他需要赋予res实际的值 所以需要把地址取给这个指针
        cm.size = size;
        ioctl(this->fd, OP_READ_MEM, &cm);
    }
};