#include<stdio.h>
#include<string.h>
#include<bits/stdc++.h>
#include<stdlib.h>
#include<iostream>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
using namespace std;
const int N=200;//磁道数量
//进程
struct PCB{
    int id; //进程id
    string name; //进程名称
    int size;  //占用内存大小
}pcb[1000];
int PCBsum;//进程总数
//deque是c++ STL里的一个容器:双端队列
deque<PCB> running;//运行态
deque<PCB> ready; //就绪态
deque<PCB> blocked; //阻塞态

//设备控制表
struct DCT{
    int id;//设备id
    string name;//设备名称
    int state; //设备状态（占用状态为1，非占用状态为0）
    string COCTname;//设备连接的控制器的名称
}dct[100];
int DCTsum;//设备数量

//控制器控制表
struct COCT{
    int id;//控制器id
    string name;//设备名称
    int state;//控制器状态
    string CHCTname;//控制器连接的通道的名称
    string DCTname[100];//控制器连接的设备名称
    int DCTnum;//控制器连接的设备数量
}coct[100];
int COCTsum;//控制器数量

//通道控制表
struct CHCT{
    int id; //通道id
    string name;//通道名称
    int state;//通道状态
    string COCTname[100];//通道连接的控制器名称
    int COCTnum;//通道连接的控制器的数量
}chct[100];
int CHCTsum;//通道数量

//进程分配表
struct Form{
    int PCBid;//进程id
    string DCTname;//设备名称
}form[100];
int Formsum;//进程分配表中的信息总数

//主菜单
void menu(){
    printf("    ****************************************\n");
    printf("    *       基于Linux的设备分配及磁盘调度  *\n");
    printf("    *        1.进程管理                    *\n");
    printf("    *        2.设备管理系统                *\n");
    printf("    *        3.磁盘调度                    *\n");
    printf("    *        4.退出系统                    *\n");
    printf("    ****************************************\n");
}

//设备分配菜单
void menu1(){
    printf("    *************************************\n");
    printf("    **********设备管理系统***************\n");
    printf("    *      1.输出设备信息               *\n");
    printf("    *      2.输出控制器信息             *\n");
    printf("    *      3.输出通道信息               *\n");
    printf("    *      4.添加设备                   *\n");
    printf("    *      5.删除设备                   *\n");
    printf("    *      6.添加控制器                 *\n");
    printf("    *      7.删除控制器                 *\n");
    printf("    *      8.添加通道                   *\n");
    printf("    *      9.删除通道                   *\n");
    printf("    *      10.模拟输入                  *\n");
    printf("    *      11.模拟输出                  *\n");
    printf("    *      12.设备分配                  *\n");
    printf("    *      13.释放资源                  *\n");
    printf("    *      14.输出设备分配表            *\n");
    printf("    *      15.返回主菜单                *\n");
    printf("    *************************************\n");
}

//磁盘调度菜单
void menu2(){
    printf("    *************************************\n");
    printf("    *******磁盘调度算法******************\n");
    printf("    *      1.先来先服务算法             *\n");
    printf("    *      2.最短寻道时间优先算法       *\n");
    printf("    *      3.电梯调度算法               *\n");
    printf("    *      4.循环扫描调度算法           *\n");
    printf("    *      5.返回主菜单                 *\n");
    printf("    *************************************\n");
}

//进程管理菜单
void menu3(){
    printf("    *************************************\n");
    printf("    *******进程管理系统******************\n");
    printf("    *      1.创建进程                   *\n");
    printf("    *      2.切换进程                   *\n");
    printf("    *      3.输出就绪,运行,阻塞态的进程 *\n");
    printf("    *      4.返回主菜单                 *\n");
    printf("    *************************************\n");
}

//初始化设备，控制器，通道
void init(){
    //初始化通道个数是3
    CHCTsum=3;
    chct[1].name="chct1";
    chct[2].name="chct2";
    chct[3].name="chct3";
    //初始化控制器个数是4
    COCTsum=4;
    coct[1].name="coct1";
    coct[2].name="coct2";
    coct[3].name="coct3";
    coct[4].name="coct4";
    //初始化设备数量是5
    DCTsum=5;//设备数量
    dct[1].name="keyboard"; //键盘
    dct[2].name="mouse"; //鼠标
    dct[3].name="screen";//屏幕
    dct[4].name="input2";
    dct[5].name="output2";
    //连接通道和控制器
    chct[1].COCTnum=2;
    chct[1].COCTname[1]="coct1";chct[1].COCTname[2]="coct2";
    chct[2].COCTnum=1;
    chct[2].COCTname[1]="coct3";
    chct[3].COCTnum=1;
    chct[3].COCTname[1]="coct4";
    coct[1].CHCTname=coct[2].CHCTname="chct1";
    coct[3].CHCTname="chct2";
    coct[4].CHCTname="chct3";
    //连接控制器和设备
    coct[1].DCTnum=2;
    coct[2].DCTnum=1;
    coct[3].DCTnum=2;
    coct[4].DCTnum=0;
    coct[1].DCTname[1]="keyboard";
    coct[1].DCTname[2]="input2";
    coct[2].DCTname[1]="mouse";
    coct[3].DCTname[1]="screen";
    coct[3].DCTname[2]="output2";
    dct[1].COCTname=dct[4].COCTname="coct1";
    dct[2].COCTname="coct2";
    dct[3].COCTname=dct[5].COCTname="coct3";
}

//创建进程
void buildPCB(){
    PCBsum++;
    printf("请输入进程id\n");
    cin>>pcb[PCBsum].id;
    printf("请输入进程名称\n");
    cin>>pcb[PCBsum].name;
    printf("请输入进程内存大小\n");
    cin>>pcb[PCBsum].size;
    ready.push_back(pcb[PCBsum]);
    //如果没有运行态的进程，那么就绪态的一个进程进入运行态
    if(running.size()==0){
        PCB p=ready.front();
        ready.pop_front();
        running.push_back(p);
    }
    puts("进程创建成功");
}

//进程切换
void changePCB(){
    /*
        运行态的一个进程时间片用完，从运行态到就绪态，就绪态的一个进程进入运行态
    */
    if(running.size()>0){
        PCB x=running.front();//拿到队列队头元素
        running.pop_front();//队头元素出队列
        ready.push_back(x);//插入到队列末尾
    }
    if(ready.size()>0){
        PCB x=ready.front();
        ready.pop_front();
        running.push_back(x);
    }
    puts("切换成功");
}

//输出就绪，运行，阻塞态的进程
void showPCB(){
    //输出运行态的进程
    printf("运行态的进程:\n");
    printf("进程id 进程名称 占用内存大小\n");
    for(int i=0;i<running.size();i++){
        cout<<running[i].id<<"  "<<running[i].name<<"  "<<running[i].size<<endl;
    }
    //输出就绪态的进程
    printf("就绪态的进程:\n");
    printf("进程id 进程名称 占用内存大小\n");
    for(int i=0;i<ready.size();i++){
        cout<<ready[i].id<<"  "<<ready[i].name<<"  "<<ready[i].size<<endl;
    }
    //输出阻塞态的进程
    printf("阻塞态的进程:\n");
    printf("进程id 进程名称 占用内存大小\n");
    for(int i=0;i<blocked.size();i++){
        cout<<blocked[i].id<<"  "<<blocked[i].name<<"  "<<blocked[i].size<<endl;
    }
}

//进程管理
void process_manager(){
    while(true){
        menu3();
        printf("    请选择:\n");
        int x;
        cin>>x;
        switch(x){
            case 1:buildPCB();break;//创建进程
            case 2:changePCB();break;//进程切换
            case 3:showPCB();break;//输出就绪，运行，阻塞态的进程
            case 4:return;//返回主菜单
            default:puts("输入错误");
        }
    }

}


//输出设备信息
void showDCT(){
    printf("DCT:\n");
    printf("设备名称    设备状态     设备连接的控制器\n");
    for(int i=1;i<=DCTsum;i++){
        cout<<dct[i].name<<"      "<<dct[i].state<<"       "<<dct[i].COCTname<<endl;
    }
}

//输出控制器信息
void showCOCT(){
    printf("COCT：\n");
    printf("控制器名称   控制器状态   控制器连接的通道\n");
    for(int i=1;i<=COCTsum;i++){
        cout<<coct[i].name<<"       "<<coct[i].state<<"      "<<coct[i].CHCTname<<endl;
    }
}

//输出通道信息
void showCHCT(){
    printf("CHCT:\n");
    printf("通道名称   通道状态\n");
    for(int i=1;i<=CHCTsum;i++){
        cout<<chct[i].name<<"       "<<chct[i].state<<endl;
    }
}

//输出设备分配表
void showForm(){
    printf("设备分配表:\n");
    printf("进程id    占用设备\n");
    for(int i=1;i<=Formsum;i++){
        cout<<form[i].PCBid<<"    "<<form[i].DCTname<<endl;
    }
}
//根据设备名找到对应的设备所在下标
int findDCT(string name){
    int index=-1;
    for(int i=1;i<=DCTsum;i++){
        if(dct[i].name==name){
            index=i;
            break;
        }
    }
    return index;
}

//根据通道名找到通道所在下标
int findCHCT(string name){
    int index=-1;
    for(int i=1;i<=CHCTsum;i++){
        if(chct[i].name==name){
            index=i;
            break;
        }
    }
    return index;
}

//根据控制器名找到控制器所在下标
int findCOCT(string name){
    int index=-1;
    for(int i=1;i<=COCTsum;i++){
        if(coct[i].name==name){
            index=i;
            break;
        }
    }
    return index;
}


//删除下标为p所在的设备
void moveDCT(int p){
    for(int i=p;i<DCTsum;i++){
        dct[i]=dct[i+1];
    }
    DCTsum--;
}

//删除下标为p所在的控制器
void moveCOCT(int p){
    //删除控制器下的设备
    for(int i=1;i<=coct[p].DCTnum;i++){
            moveDCT(findDCT(coct[p].DCTname[i]));
//            string dctname=coct[p].DCTname[i];
//            int pos=findDCT(dctname);
//            if(pos==-1) cout<<dctname<<endl;
//            else moveDCT(pos);
        }
    for(int i=p;i<COCTsum;i++){
        coct[i]=coct[i+1];
    }
    COCTsum--;
}

//删除下标为p所在的通道
void moveCHCT(int p){
    //删除这个通道所连接的控制器
    for(int i=1;i<=chct[p].COCTnum;i++){
        moveCOCT(findCOCT(chct[p].COCTname[i]));
    }
    for(int i=p;i<CHCTsum;i++){
        chct[i]=chct[i+1];
    }
    CHCTsum--;
}

//添加设备
void addDCT(){
    printf("请输入要添加的设备名称\n");
    string name;
    cin>>name;
    if(findDCT(name)!=-1){
        puts("已存在这个设备，添加失败");
        return;
    }
    printf("请输入设备所连接的控制器名称\n");
    string coctname;
    cin>>coctname;
    int p=findCOCT(coctname);
    if(p==-1){
        puts("没找到这个控制器，添加失败");
    }else{
        coct[p].DCTnum++;  //控制器连接的设备数量加1
        coct[p].DCTname[coct[p].DCTnum]=name;

        DCTsum++;
        dct[DCTsum].name=name;
        dct[DCTsum].COCTname=coctname;
        puts("添加成功");
    }
}

//删除设备
void deleteDCT(){
    printf("请输入要删除的设备名称\n");
    string name;
    cin>>name;
    int p=findDCT(name);
    if(p==-1){
        puts("没有找到此设备，删除失败\n");
    }else{
        puts("删除成功");
        string coctname=dct[p].COCTname;
        moveDCT(p);
        //修改设备所连接的控制器信息
        p=findCOCT(coctname);
        for(int i=1;i<=coct[p].DCTnum;i++){
            if(coct[p].DCTname[i]==name){
                //在控制器信息中删除这个设备
                for(int j=i;j<coct[p].DCTnum;j++){
                    coct[p].DCTname[j]=coct[p].DCTname[j+1];
                }
            }
        }
        coct[p].DCTnum--;//控制器连接的设备数量减一
    }
}

//添加控制器
void addCOCT(){
    string name;
    printf("请输入控制器名称\n");
    cin>>name;
    if(findCOCT(name)!=-1){
        puts("已存在这个控制器，添加失败");
        return;
    }
    string chctname;
    printf("请输入控制器连接的通道名称\n");
    cin>>chctname;
    int p=findCHCT(chctname);
    if(p==-1){
        puts("没有找到这个通道，添加失败");
    }else{
        chct[p].COCTnum++;
        chct[p].COCTname[chct[p].COCTnum]=name;
        COCTsum++;
        coct[COCTsum].name=name;
        coct[COCTsum].CHCTname=chctname;
        puts("添加成功");
    }
}

//删除控制器
void deleteCOCT(){
    printf("请输入删除的控制器的名称\n");
    string name;
    cin>>name;
    int p=findCOCT(name);
    if(p==-1){
        puts("没找到这个控制器");
    }else{
        puts("删除成功");
        //删除这个控制器下的设备
        string chctname=coct[p].CHCTname;
        moveCOCT(p);
        //修改这个控制器连接的通道的信息
        p=findCHCT(chctname);//找到这个设备连接的通道在数组中的位置
        for(int i=1;i<chct[p].COCTnum;i++){
            if(chct[p].COCTname[i]==name){
                //在通道信息中删除这个控制器
                for(int j=i;j<chct[p].COCTnum;j++){
                    chct[p].COCTname[j]=chct[p].COCTname[j+1];
                }
            }
        }
        chct[p].COCTnum--;
    }

}

//添加通道
void addCHCT(){
    printf("请输入添加的通道\n");
    string name;
    cin>>name;
    if(findCHCT(name)!=-1){
        puts("已存在这个通道，添加失败");
    }else{
        CHCTsum++;
        chct[CHCTsum].name=name;
        puts("添加成功");
    }
}

//删除通道
void deleteCHCT(){
    printf("请输入删除的通道名称\n");
    string name;
    cin>>name;
    int p=findCHCT(name);
    if(p==-1){
        puts("没有找到该通道，删除失败");
    }else{
        puts("删除成功");
        moveCHCT(p);
    }
}

//设备分配
void deviceAllocation(){
    /*
        运行态的进程申请设备然后进入阻塞态，获得资源后进入就绪态
    */
    if(running.size()==0){
        puts("没有运行态的进程");
        return;
    }
    //进程从运行态到阻塞态，并且就绪态的一个进程进入运行态
    PCB x=running.front();
    running.pop_front();
    blocked.push_back(x);
    if(ready.size()>0){
        PCB y=ready.front();
        ready.pop_front();
        running.push_back(y);
    }
    printf("请输入申请的设备名称\n");
    string name;
    cin>>name;
    bool flag=false;
    for(int i=1;i<=DCTsum;i++){
        //查找设备
        if(dct[i].name==name&&dct[i].state==0){
            string coctname=dct[i].COCTname;
            //查找控制器
            for(int j=1;j<=COCTsum;j++){
                if(coct[j].name==coctname&&coct[j].state==0){
                    string chctname=coct[j].CHCTname;
                    //查找通道
                    for(int z=1;z<=CHCTsum;z++){
                        if(chct[z].name==chctname&&chct[z].state==0){
                            flag=true;
                            dct[i].state=coct[j].state=chct[z].state=1;
                            goto loop;
                        }
                    }
                }
            }
        }
    }
    loop:;
    if(!flag){
        puts("申请资源失败");
        return;
    }
    //资源分配成功,进程x从阻塞态到就绪态
    x=blocked.back();
    blocked.pop_back();
    ready.push_back(x);
    if(ready.size()>0&&running.size()==0){
        PCB y=ready.front();
        ready.pop_front();
        running.push_back(y);
    }
    //设备分配表中添加一个数据
    Formsum++;
    form[Formsum].PCBid=x.id;
    form[Formsum].DCTname=name;
    puts("设备分配成功");
}

//释放资源
void  releaseEquipment(){
/*
    运行态的一个进程运行结束，释放这个进程所占用的设备。并且从数组中删除这个进程。
    就绪态的一个进程进入运行态。
*/
    if(running.size()==0){
        puts("没有运行态的进程");
        return;
    }
    //从运行态删除
    PCB x=running.front();
    running.pop_front();
    //就绪态的一个进程进入运行态
    if(ready.size()>0){
        PCB y=ready.front();
        ready.pop_front();
        running.push_back(y);
    }
    //从数组中删除进程x
    for(int i=1;i<=PCBsum;i++){
        if(pcb[i].id==x.id){
            //删除下标i这个进程，后边的往前覆盖
            for(int j=i;j<PCBsum;j++){
                pcb[j]=pcb[j+1];
            }
        }
    }
    PCBsum--;
    //如果这个进程占用了某些设备，可能占用了多个设备，那么释放所有设备
    while(true){
        int index=-1;//记录这个进程在进程分配表中的位置
        string name;//占用的设备名称
        //查询设备分配表，看是否占用设备
        for(int i=1;i<=Formsum;i++){
            if(form[i].PCBid==x.id){
                name=form[i].DCTname;
                index=i;
                break;
            }
        }
        if(index!=-1){
            //释放设备，控制器，通道
            for(int i=1;i<=DCTsum;i++){
                //查找设备
                if(dct[i].name==name){
                    string coctname=dct[i].COCTname;
                    for(int j=1;j<=COCTsum;j++){
                        //查找控制器
                        if(coct[j].name==coctname){
                            string chctname=coct[j].CHCTname;
                            //查找通道
                            for(int z=1;z<=CHCTsum;z++){
                                if(chct[z].name==chctname){
                                    //设备，控制器，通道都找到了
                                    dct[i].state=coct[j].state=chct[z].state=0;
                                }
                            }
                        }
                    }
                }
            }
            //从进程分配表中删除这个记录
            for(int i=index;i<Formsum;i++){
                form[i]=form[i+1];
            }
            Formsum--;//分配表中的数据量减一
        }else{
            break;
        }
    }
    puts("设备释放成功");
}

//写文件
void writefile(){
    //打开a.txt文件
    int fd=open("a.txt",O_WRONLY|O_CREAT);
    char buffer[10000];
    printf("请输入一个字符串\n");
    scanf("%s",buffer);//从键盘输入一个字符串
    write(fd,buffer,sizeof buffer);//将字符串写到a.txt文件
    close(fd);
}

//模拟输入
void in(){
    /*
        一个运行态的进程，申请资源，进入阻塞态，申请到资源后进入就绪态，就绪态再到运行态。
        运行结束后释放资源，并且删除这个进程。
    */
    if(running.size()==0){
        puts("没有运行的进程，模拟失败");
        return;
    }
    //申请系统资源，键盘，从运行态到阻塞态
    PCB x=running.front();
    running.pop_front();
    blocked.push_back(x); //进入阻塞态
    string divicename="keyboard";//设备名称，键盘
    bool flag=false;
    int i,j,z;//记录设备，控制器，通道在数组中的下标
    for(i=1;i<=DCTsum;i++){
        if(dct[i].name==divicename){
            if(dct[i].state==0){
                string coctname=dct[i].COCTname;
                //查看控制器是否被占用
                for(j=1;j<=COCTsum;j++){
                    if(coct[j].name==coctname){
                        if(coct[j].state==0){
                            string chctname=coct[j].CHCTname;
                            //查看通道是否被占用
                            for(z=1;z<=CHCTsum;z++){
                                if(chct[z].name==chctname){
                                    if(chct[z].state==0){
                                        flag=true;
                                        dct[i].state=coct[j].state=chct[z].state=1;
                                        goto loop; //跳出所有循环
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    loop:;
    if(!flag){
        puts("分配资源失败");
        return;
    }
    //设备分配成功，从阻塞态到就绪态
    x=blocked.back();
    blocked.pop_back();
    ready.push_back(x);
    //就绪态到运行态
    x=ready.back();
    ready.pop_back();
    running.push_back(x);
    //然后输入
    writefile();
    running.pop_front();//从运行态删除
    if(ready.size()&&running.size()==0){
        //如果有进程处于就绪态，那么从就绪态拿出一个进程进入运行态
        x=ready.front();
        ready.pop_front();
        running.push_back(x);//进入运行态
    }
    dct[i].state=coct[j].state=chct[z].state=0;//释放设备
    //删除进程
    for(int k=i;k<DCTsum;k++) dct[k]=dct[k+1];
    DCTsum--;//进程总数减一
    puts("输入成功，保存在a.txt文件中");
}

//输出到文件
void readfile(){
    //打开a.txt文件
    int fd=open("a.txt",O_RDONLY);
    char buffer[1000];
    int size=read(fd,buffer,sizeof buffer);//把a.txt文件中的数据读到buffer数组中
    printf("%s\n",buffer);
    close(fd);//关闭文件
}

//模拟输出
void out(){
    if(running.size()==0){
        puts("没有进程，模拟失败");
        return;
    }
     /*
        一个运行态的进程，申请资源，进入阻塞态，申请到资源后进入就绪态，就绪态再到运行态。
        运行结束后释放资源，并且删除这个进程。
    */
    //申请系统资源，键盘，从运行态到阻塞态
    PCB x=running.front();
    running.pop_front();
    blocked.push_back(x); //进入阻塞态
    string divicename="screen";   //屏幕
    bool flag=false;
    int i,j,z; //记录设备，控制器，通道在数组中的下标
    for(i=1;i<=DCTsum;i++){
        if(dct[i].name==divicename){
            if(dct[i].state==0){
                string coctname=dct[i].COCTname;
                //查看控制器是否被占用
                for(j=1;j<=COCTsum;j++){
                    if(coct[j].name==coctname){
                        if(coct[j].state==0){
                            string chctname=coct[j].CHCTname;
                            //查看通道是否被占用
                            for(z=1;z<=CHCTsum;z++){
                                if(chct[z].name==chctname){
                                    if(chct[z].state==0){
                                        flag=true;  //分配资源成功
                                        dct[i].state=coct[j].state=chct[z].state=1;
                                        goto loop; //结束所有循环
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    loop:;
    if(!flag){
        puts("分配资源失败");
        return;
    }
    //设备分配成功，从阻塞态到就绪态
    x=blocked.back();
    blocked.pop_back();
    ready.push_back(x);
    //就绪态到运行态
    x=ready.back();
    ready.pop_back();
    running.push_back(x);
    //然后输入
    readfile();
    running.pop_front();//从运行态删除
    dct[i].state=coct[j].state=chct[z].state=0;//释放设备
    if(ready.size()&&running.size()==0){
        //如果有进程处于就绪态并且没有运行态的进程，那么从就绪态拿出一个进程进入运行态
        x=ready.front();
        ready.pop_front();
        running.push_back(x);//进入运行态
    }
    //删除进程
   for(int i=1;i<=PCBsum;i++){
        if(x.id==pcb[i].id){
            for(int j=i;j<PCBsum;j++){
                pcb[j]=pcb[j+1];
            }
        }
   }
   PCBsum--;
}

//设备管理系统
void deviceManager(){
    while(true){
        menu1();
        printf("       请选择:");
        int x;
        cin>>x;
        switch(x){
            case 1:showDCT();break;//输出所有设备信息
            case 2:showCOCT();break;//输出控制器信息
            case 3:showCHCT();break;//输出通道信息
            case 4:addDCT();break;    //添加设备
            case 5:deleteDCT();break;  //删除设备
            case 6:addCOCT();break;  //添加控制器
            case 7:deleteCOCT();break; //删除控制器
            case 8:addCHCT();break;   //添加通道
            case 9:deleteCHCT();break; //删除通道
            case 10:in();break; //模拟输入
            case 11:out();break;//模拟输出
            case 12:deviceAllocation();break;//设备分配
            case 13:releaseEquipment();break;//释放资源
            case 14:showForm();break;//输出设备分配表
            case 15:return; //结束，返回主菜单
            default:printf("输入错误\n");break;
        }
    }
}

//1.先来先服务算法
void FCFS(int *arr ,int n,int x){
    printf("所有请求所在的磁道位置:\n");
    for(int i=1;i<=n;i++){
        cout<<arr[i]<<" ";
    }
    cout<<endl;
    printf("磁头当前所在位置：%d\n",x);

    printf("访问顺序为：\n");
    for(int i=1;i<=n;i++){
        cout<<arr[i]<<" ";
    }
    cout<<endl;
    //abs函数返回一个整数的绝对值
    int sum=abs(arr[1]-x);
    for(int i=2;i<=n;i++){
        sum+=abs(arr[i]-arr[i-1]);
    }
    double avg=1.0*sum/n;
    printf("平均寻道长度:%.2f\n",avg);
}

//2.最短寻道时间优先算法
void SSTF(int *arr,int n,int x){
    printf("所有请求所在的磁道位置:\n");
    for(int i=1;i<=n;i++){
        cout<<arr[i]<<" ";
    }
    cout<<endl;
    printf("磁头当前所在位置：%d\n",x);
   bool use[30];
   for(int i=1;i<=n;i++) use[i]=false;
   int p=x;//当前磁头所在位置。
   int sum=0;

   printf("访问顺序：\n");
   for(int i=1;i<=n;i++){
        //找当前离p最近的请求位置
        int L,pos=-1;
        for(int j=1;j<=n;j++){
            if((pos==-1&&use[j]==false)||(pos!=-1&&use[j]==false&&abs(arr[j]-p)>L)){
                pos=j;
                L=abs(p-arr[j]);
            }
        }
        cout<<arr[pos]<<" ";
        sum+=abs(arr[pos]-p);
        use[pos]=true;
        p=arr[pos];
   }
   cout<<endl;
   printf("平均寻道周长:%.2f\n",1.0*sum/n);
}

//3.电梯调度算法
void SCAN(int* arr,int n,int p){
    int x;
    while(true){
        printf("请选择磁头移动的方向，1：磁道增大的方向，2：磁道减小的方向\n");
        cin>>x;
        if(x!=1&&x!=2) puts("输入错误，重新输入");
        else break;
    }
    printf("所有请求所在的磁道位置:\n");
    for(int i=1;i<=n;i++){
        cout<<arr[i]<<" ";
    }
    cout<<endl;
    printf("磁头当前所在位置：%d\n",p);

    printf("访问顺序：\n");
    int sum=0;
    int cnt[N+2];//记录每个磁道上的请求数量
    for(int i=0;i<=N;i++) cnt[i]=0;
    for(int i=1;i<=n;i++) cnt[arr[i]]++;
    int num=0;//记录已经解决了多少个请求
    if(x==1){
        //先按磁头增大方向扫描
        for(int i=p;i<=N;i++){
            while(cnt[i]){
                cout<<i<<" ";
                cnt[i]--;
                num++;
                if(num==n) break;
            }
            sum++;
        }
        for(int i=N-1;i>=0;i--){
            while(cnt[i]){
                cout<<i<<" ";
                cnt[i]--;
                num++;
                if(num==n) break;
            }
            sum++;
        }
    }else{
        //先按磁头减小方向扫描
        for(int i=p;i>=0;i--){
            while(cnt[i]){
                cout<<i<<" ";
                cnt[i]--;
                num++;
                if(num==n) break;
            }
            sum++;
        }
        for(int i=1;i<=N;i++){
            while(cnt[i]){
                cout<<i<<" ";
                cnt[i]--;
                num++;
                if(num==n) break;
            }
            sum++;
        }
    }
    cout<<endl;
    printf("平均寻道长度:%.2f\n",1.0*sum/n);
}

//4.循环扫描调度算法
void CSCAN(int *arr,int n,int p){
    printf("所有请求所在的磁道位置:\n");
    for(int i=1;i<=n;i++){
        cout<<arr[i]<<" ";
    }
    cout<<endl;
    printf("磁头当前所在位置：%d\n",p);

    printf("访问顺序：\n");
    int cnt[N+2];//记录每个磁道上的请求数量
    for(int i=0;i<=N;i++) cnt[i]=0;
    for(int i=1;i<=n;i++) cnt[arr[i]]++;
    int num=0;
    int sum=0;
    for(int i=p;i<=N;i++){
        while(cnt[i]){
            cout<<i<<" ";
            cnt[i]--;
            num++;
            if(num==n) break;
        }
        sum++;
    }
    for(int i=0;i<p;i++){
        while(cnt[i]){
            cout<<i<<" ";
            cnt[i]--;
            num++;
            if(num==n) break;
        }
        sum++;
    }
    cout<<endl;
    printf("平均寻道长度%.2f\n",1.0*sum/n);

}

//磁盘调度算法
void diskScheduling(){
    int n=20;//请求访问的数量
    int arr[30];
    srand(time(0)); //随机数播种
    //随机生成20个访问磁道的位置
    for(int i=1;i<=20;i++){
        arr[i]=rand()%N;   //随机生成在0到N之间的数据
    }
    int p=rand()%N;//磁头当前所在磁道
    while(true){
        menu2();
        printf("     请选择：");
        int x;
        scanf("%d",&x);
        switch(x){
            case 1:FCFS(arr,n,p);break;//先来先服务算法
            case 2:SSTF(arr,n,p);break;//最短寻道时间优先算法
            case 3:SCAN(arr,n,p);break;//电梯调度算法(扫描算法)
            case 4:CSCAN(arr,n,p);break;//循环扫描调度算法
            case 5:return;//返回主菜单
        }
    }
}

int main(){
    init();//初始化设备，控制器，通道
    while(true){
        menu();
        printf("      请选择:");
        int  x;
        scanf("%d",&x);
        switch(x){
            case 1:process_manager();break; //进程管理
            case 2:deviceManager();break;  //设备管理
            case 3:diskScheduling();break; //磁盘调度
            case 4:execlp("clear","clear",NULL);exit(0);//调用内核函数清屏，然后结束
            //execlp运行失败有返回值，运行成功没有返回值，运行成功当前进程结束。
            default:printf("输入错误\n");
        }
    }
    return 0;
}
