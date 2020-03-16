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
const int N=200;//�ŵ�����
//����
struct PCB{
    int id; //����id
    string name; //��������
    int size;  //ռ���ڴ��С
}pcb[1000];
int PCBsum;//��������
//deque��c++ STL���һ������:˫�˶���
deque<PCB> running;//����̬
deque<PCB> ready; //����̬
deque<PCB> blocked; //����̬

//�豸���Ʊ�
struct DCT{
    int id;//�豸id
    string name;//�豸����
    int state; //�豸״̬��ռ��״̬Ϊ1����ռ��״̬Ϊ0��
    string COCTname;//�豸���ӵĿ�����������
}dct[100];
int DCTsum;//�豸����

//���������Ʊ�
struct COCT{
    int id;//������id
    string name;//�豸����
    int state;//������״̬
    string CHCTname;//���������ӵ�ͨ��������
    string DCTname[100];//���������ӵ��豸����
    int DCTnum;//���������ӵ��豸����
}coct[100];
int COCTsum;//����������

//ͨ�����Ʊ�
struct CHCT{
    int id; //ͨ��id
    string name;//ͨ������
    int state;//ͨ��״̬
    string COCTname[100];//ͨ�����ӵĿ���������
    int COCTnum;//ͨ�����ӵĿ�����������
}chct[100];
int CHCTsum;//ͨ������

//���̷����
struct Form{
    int PCBid;//����id
    string DCTname;//�豸����
}form[100];
int Formsum;//���̷�����е���Ϣ����

//���˵�
void menu(){
    printf("    ****************************************\n");
    printf("    *       ����Linux���豸���估���̵���  *\n");
    printf("    *        1.���̹���                    *\n");
    printf("    *        2.�豸����ϵͳ                *\n");
    printf("    *        3.���̵���                    *\n");
    printf("    *        4.�˳�ϵͳ                    *\n");
    printf("    ****************************************\n");
}

//�豸����˵�
void menu1(){
    printf("    *************************************\n");
    printf("    **********�豸����ϵͳ***************\n");
    printf("    *      1.����豸��Ϣ               *\n");
    printf("    *      2.�����������Ϣ             *\n");
    printf("    *      3.���ͨ����Ϣ               *\n");
    printf("    *      4.����豸                   *\n");
    printf("    *      5.ɾ���豸                   *\n");
    printf("    *      6.��ӿ�����                 *\n");
    printf("    *      7.ɾ��������                 *\n");
    printf("    *      8.���ͨ��                   *\n");
    printf("    *      9.ɾ��ͨ��                   *\n");
    printf("    *      10.ģ������                  *\n");
    printf("    *      11.ģ�����                  *\n");
    printf("    *      12.�豸����                  *\n");
    printf("    *      13.�ͷ���Դ                  *\n");
    printf("    *      14.����豸�����            *\n");
    printf("    *      15.�������˵�                *\n");
    printf("    *************************************\n");
}

//���̵��Ȳ˵�
void menu2(){
    printf("    *************************************\n");
    printf("    *******���̵����㷨******************\n");
    printf("    *      1.�����ȷ����㷨             *\n");
    printf("    *      2.���Ѱ��ʱ�������㷨       *\n");
    printf("    *      3.���ݵ����㷨               *\n");
    printf("    *      4.ѭ��ɨ������㷨           *\n");
    printf("    *      5.�������˵�                 *\n");
    printf("    *************************************\n");
}

//���̹���˵�
void menu3(){
    printf("    *************************************\n");
    printf("    *******���̹���ϵͳ******************\n");
    printf("    *      1.��������                   *\n");
    printf("    *      2.�л�����                   *\n");
    printf("    *      3.�������,����,����̬�Ľ��� *\n");
    printf("    *      4.�������˵�                 *\n");
    printf("    *************************************\n");
}

//��ʼ���豸����������ͨ��
void init(){
    //��ʼ��ͨ��������3
    CHCTsum=3;
    chct[1].name="chct1";
    chct[2].name="chct2";
    chct[3].name="chct3";
    //��ʼ��������������4
    COCTsum=4;
    coct[1].name="coct1";
    coct[2].name="coct2";
    coct[3].name="coct3";
    coct[4].name="coct4";
    //��ʼ���豸������5
    DCTsum=5;//�豸����
    dct[1].name="keyboard"; //����
    dct[2].name="mouse"; //���
    dct[3].name="screen";//��Ļ
    dct[4].name="input2";
    dct[5].name="output2";
    //����ͨ���Ϳ�����
    chct[1].COCTnum=2;
    chct[1].COCTname[1]="coct1";chct[1].COCTname[2]="coct2";
    chct[2].COCTnum=1;
    chct[2].COCTname[1]="coct3";
    chct[3].COCTnum=1;
    chct[3].COCTname[1]="coct4";
    coct[1].CHCTname=coct[2].CHCTname="chct1";
    coct[3].CHCTname="chct2";
    coct[4].CHCTname="chct3";
    //���ӿ��������豸
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

//��������
void buildPCB(){
    PCBsum++;
    printf("���������id\n");
    cin>>pcb[PCBsum].id;
    printf("�������������\n");
    cin>>pcb[PCBsum].name;
    printf("����������ڴ��С\n");
    cin>>pcb[PCBsum].size;
    ready.push_back(pcb[PCBsum]);
    //���û������̬�Ľ��̣���ô����̬��һ�����̽�������̬
    if(running.size()==0){
        PCB p=ready.front();
        ready.pop_front();
        running.push_back(p);
    }
    puts("���̴����ɹ�");
}

//�����л�
void changePCB(){
    /*
        ����̬��һ������ʱ��Ƭ���꣬������̬������̬������̬��һ�����̽�������̬
    */
    if(running.size()>0){
        PCB x=running.front();//�õ����ж�ͷԪ��
        running.pop_front();//��ͷԪ�س�����
        ready.push_back(x);//���뵽����ĩβ
    }
    if(ready.size()>0){
        PCB x=ready.front();
        ready.pop_front();
        running.push_back(x);
    }
    puts("�л��ɹ�");
}

//������������У�����̬�Ľ���
void showPCB(){
    //�������̬�Ľ���
    printf("����̬�Ľ���:\n");
    printf("����id �������� ռ���ڴ��С\n");
    for(int i=0;i<running.size();i++){
        cout<<running[i].id<<"  "<<running[i].name<<"  "<<running[i].size<<endl;
    }
    //�������̬�Ľ���
    printf("����̬�Ľ���:\n");
    printf("����id �������� ռ���ڴ��С\n");
    for(int i=0;i<ready.size();i++){
        cout<<ready[i].id<<"  "<<ready[i].name<<"  "<<ready[i].size<<endl;
    }
    //�������̬�Ľ���
    printf("����̬�Ľ���:\n");
    printf("����id �������� ռ���ڴ��С\n");
    for(int i=0;i<blocked.size();i++){
        cout<<blocked[i].id<<"  "<<blocked[i].name<<"  "<<blocked[i].size<<endl;
    }
}

//���̹���
void process_manager(){
    while(true){
        menu3();
        printf("    ��ѡ��:\n");
        int x;
        cin>>x;
        switch(x){
            case 1:buildPCB();break;//��������
            case 2:changePCB();break;//�����л�
            case 3:showPCB();break;//������������У�����̬�Ľ���
            case 4:return;//�������˵�
            default:puts("�������");
        }
    }

}


//����豸��Ϣ
void showDCT(){
    printf("DCT:\n");
    printf("�豸����    �豸״̬     �豸���ӵĿ�����\n");
    for(int i=1;i<=DCTsum;i++){
        cout<<dct[i].name<<"      "<<dct[i].state<<"       "<<dct[i].COCTname<<endl;
    }
}

//�����������Ϣ
void showCOCT(){
    printf("COCT��\n");
    printf("����������   ������״̬   ���������ӵ�ͨ��\n");
    for(int i=1;i<=COCTsum;i++){
        cout<<coct[i].name<<"       "<<coct[i].state<<"      "<<coct[i].CHCTname<<endl;
    }
}

//���ͨ����Ϣ
void showCHCT(){
    printf("CHCT:\n");
    printf("ͨ������   ͨ��״̬\n");
    for(int i=1;i<=CHCTsum;i++){
        cout<<chct[i].name<<"       "<<chct[i].state<<endl;
    }
}

//����豸�����
void showForm(){
    printf("�豸�����:\n");
    printf("����id    ռ���豸\n");
    for(int i=1;i<=Formsum;i++){
        cout<<form[i].PCBid<<"    "<<form[i].DCTname<<endl;
    }
}
//�����豸���ҵ���Ӧ���豸�����±�
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

//����ͨ�����ҵ�ͨ�������±�
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

//���ݿ��������ҵ������������±�
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


//ɾ���±�Ϊp���ڵ��豸
void moveDCT(int p){
    for(int i=p;i<DCTsum;i++){
        dct[i]=dct[i+1];
    }
    DCTsum--;
}

//ɾ���±�Ϊp���ڵĿ�����
void moveCOCT(int p){
    //ɾ���������µ��豸
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

//ɾ���±�Ϊp���ڵ�ͨ��
void moveCHCT(int p){
    //ɾ�����ͨ�������ӵĿ�����
    for(int i=1;i<=chct[p].COCTnum;i++){
        moveCOCT(findCOCT(chct[p].COCTname[i]));
    }
    for(int i=p;i<CHCTsum;i++){
        chct[i]=chct[i+1];
    }
    CHCTsum--;
}

//����豸
void addDCT(){
    printf("������Ҫ��ӵ��豸����\n");
    string name;
    cin>>name;
    if(findDCT(name)!=-1){
        puts("�Ѵ�������豸�����ʧ��");
        return;
    }
    printf("�������豸�����ӵĿ���������\n");
    string coctname;
    cin>>coctname;
    int p=findCOCT(coctname);
    if(p==-1){
        puts("û�ҵ���������������ʧ��");
    }else{
        coct[p].DCTnum++;  //���������ӵ��豸������1
        coct[p].DCTname[coct[p].DCTnum]=name;

        DCTsum++;
        dct[DCTsum].name=name;
        dct[DCTsum].COCTname=coctname;
        puts("��ӳɹ�");
    }
}

//ɾ���豸
void deleteDCT(){
    printf("������Ҫɾ�����豸����\n");
    string name;
    cin>>name;
    int p=findDCT(name);
    if(p==-1){
        puts("û���ҵ����豸��ɾ��ʧ��\n");
    }else{
        puts("ɾ���ɹ�");
        string coctname=dct[p].COCTname;
        moveDCT(p);
        //�޸��豸�����ӵĿ�������Ϣ
        p=findCOCT(coctname);
        for(int i=1;i<=coct[p].DCTnum;i++){
            if(coct[p].DCTname[i]==name){
                //�ڿ�������Ϣ��ɾ������豸
                for(int j=i;j<coct[p].DCTnum;j++){
                    coct[p].DCTname[j]=coct[p].DCTname[j+1];
                }
            }
        }
        coct[p].DCTnum--;//���������ӵ��豸������һ
    }
}

//��ӿ�����
void addCOCT(){
    string name;
    printf("���������������\n");
    cin>>name;
    if(findCOCT(name)!=-1){
        puts("�Ѵ�����������������ʧ��");
        return;
    }
    string chctname;
    printf("��������������ӵ�ͨ������\n");
    cin>>chctname;
    int p=findCHCT(chctname);
    if(p==-1){
        puts("û���ҵ����ͨ�������ʧ��");
    }else{
        chct[p].COCTnum++;
        chct[p].COCTname[chct[p].COCTnum]=name;
        COCTsum++;
        coct[COCTsum].name=name;
        coct[COCTsum].CHCTname=chctname;
        puts("��ӳɹ�");
    }
}

//ɾ��������
void deleteCOCT(){
    printf("������ɾ���Ŀ�����������\n");
    string name;
    cin>>name;
    int p=findCOCT(name);
    if(p==-1){
        puts("û�ҵ����������");
    }else{
        puts("ɾ���ɹ�");
        //ɾ������������µ��豸
        string chctname=coct[p].CHCTname;
        moveCOCT(p);
        //�޸�������������ӵ�ͨ������Ϣ
        p=findCHCT(chctname);//�ҵ�����豸���ӵ�ͨ���������е�λ��
        for(int i=1;i<chct[p].COCTnum;i++){
            if(chct[p].COCTname[i]==name){
                //��ͨ����Ϣ��ɾ�����������
                for(int j=i;j<chct[p].COCTnum;j++){
                    chct[p].COCTname[j]=chct[p].COCTname[j+1];
                }
            }
        }
        chct[p].COCTnum--;
    }

}

//���ͨ��
void addCHCT(){
    printf("��������ӵ�ͨ��\n");
    string name;
    cin>>name;
    if(findCHCT(name)!=-1){
        puts("�Ѵ������ͨ�������ʧ��");
    }else{
        CHCTsum++;
        chct[CHCTsum].name=name;
        puts("��ӳɹ�");
    }
}

//ɾ��ͨ��
void deleteCHCT(){
    printf("������ɾ����ͨ������\n");
    string name;
    cin>>name;
    int p=findCHCT(name);
    if(p==-1){
        puts("û���ҵ���ͨ����ɾ��ʧ��");
    }else{
        puts("ɾ���ɹ�");
        moveCHCT(p);
    }
}

//�豸����
void deviceAllocation(){
    /*
        ����̬�Ľ��������豸Ȼ���������̬�������Դ��������̬
    */
    if(running.size()==0){
        puts("û������̬�Ľ���");
        return;
    }
    //���̴�����̬������̬�����Ҿ���̬��һ�����̽�������̬
    PCB x=running.front();
    running.pop_front();
    blocked.push_back(x);
    if(ready.size()>0){
        PCB y=ready.front();
        ready.pop_front();
        running.push_back(y);
    }
    printf("������������豸����\n");
    string name;
    cin>>name;
    bool flag=false;
    for(int i=1;i<=DCTsum;i++){
        //�����豸
        if(dct[i].name==name&&dct[i].state==0){
            string coctname=dct[i].COCTname;
            //���ҿ�����
            for(int j=1;j<=COCTsum;j++){
                if(coct[j].name==coctname&&coct[j].state==0){
                    string chctname=coct[j].CHCTname;
                    //����ͨ��
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
        puts("������Դʧ��");
        return;
    }
    //��Դ����ɹ�,����x������̬������̬
    x=blocked.back();
    blocked.pop_back();
    ready.push_back(x);
    if(ready.size()>0&&running.size()==0){
        PCB y=ready.front();
        ready.pop_front();
        running.push_back(y);
    }
    //�豸����������һ������
    Formsum++;
    form[Formsum].PCBid=x.id;
    form[Formsum].DCTname=name;
    puts("�豸����ɹ�");
}

//�ͷ���Դ
void  releaseEquipment(){
/*
    ����̬��һ���������н������ͷ����������ռ�õ��豸�����Ҵ�������ɾ��������̡�
    ����̬��һ�����̽�������̬��
*/
    if(running.size()==0){
        puts("û������̬�Ľ���");
        return;
    }
    //������̬ɾ��
    PCB x=running.front();
    running.pop_front();
    //����̬��һ�����̽�������̬
    if(ready.size()>0){
        PCB y=ready.front();
        ready.pop_front();
        running.push_back(y);
    }
    //��������ɾ������x
    for(int i=1;i<=PCBsum;i++){
        if(pcb[i].id==x.id){
            //ɾ���±�i������̣���ߵ���ǰ����
            for(int j=i;j<PCBsum;j++){
                pcb[j]=pcb[j+1];
            }
        }
    }
    PCBsum--;
    //����������ռ����ĳЩ�豸������ռ���˶���豸����ô�ͷ������豸
    while(true){
        int index=-1;//��¼��������ڽ��̷�����е�λ��
        string name;//ռ�õ��豸����
        //��ѯ�豸��������Ƿ�ռ���豸
        for(int i=1;i<=Formsum;i++){
            if(form[i].PCBid==x.id){
                name=form[i].DCTname;
                index=i;
                break;
            }
        }
        if(index!=-1){
            //�ͷ��豸����������ͨ��
            for(int i=1;i<=DCTsum;i++){
                //�����豸
                if(dct[i].name==name){
                    string coctname=dct[i].COCTname;
                    for(int j=1;j<=COCTsum;j++){
                        //���ҿ�����
                        if(coct[j].name==coctname){
                            string chctname=coct[j].CHCTname;
                            //����ͨ��
                            for(int z=1;z<=CHCTsum;z++){
                                if(chct[z].name==chctname){
                                    //�豸����������ͨ�����ҵ���
                                    dct[i].state=coct[j].state=chct[z].state=0;
                                }
                            }
                        }
                    }
                }
            }
            //�ӽ��̷������ɾ�������¼
            for(int i=index;i<Formsum;i++){
                form[i]=form[i+1];
            }
            Formsum--;//������е���������һ
        }else{
            break;
        }
    }
    puts("�豸�ͷųɹ�");
}

//д�ļ�
void writefile(){
    //��a.txt�ļ�
    int fd=open("a.txt",O_WRONLY|O_CREAT);
    char buffer[10000];
    printf("������һ���ַ���\n");
    scanf("%s",buffer);//�Ӽ�������һ���ַ���
    write(fd,buffer,sizeof buffer);//���ַ���д��a.txt�ļ�
    close(fd);
}

//ģ������
void in(){
    /*
        һ������̬�Ľ��̣�������Դ����������̬�����뵽��Դ��������̬������̬�ٵ�����̬��
        ���н������ͷ���Դ������ɾ��������̡�
    */
    if(running.size()==0){
        puts("û�����еĽ��̣�ģ��ʧ��");
        return;
    }
    //����ϵͳ��Դ�����̣�������̬������̬
    PCB x=running.front();
    running.pop_front();
    blocked.push_back(x); //��������̬
    string divicename="keyboard";//�豸���ƣ�����
    bool flag=false;
    int i,j,z;//��¼�豸����������ͨ���������е��±�
    for(i=1;i<=DCTsum;i++){
        if(dct[i].name==divicename){
            if(dct[i].state==0){
                string coctname=dct[i].COCTname;
                //�鿴�������Ƿ�ռ��
                for(j=1;j<=COCTsum;j++){
                    if(coct[j].name==coctname){
                        if(coct[j].state==0){
                            string chctname=coct[j].CHCTname;
                            //�鿴ͨ���Ƿ�ռ��
                            for(z=1;z<=CHCTsum;z++){
                                if(chct[z].name==chctname){
                                    if(chct[z].state==0){
                                        flag=true;
                                        dct[i].state=coct[j].state=chct[z].state=1;
                                        goto loop; //��������ѭ��
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
        puts("������Դʧ��");
        return;
    }
    //�豸����ɹ���������̬������̬
    x=blocked.back();
    blocked.pop_back();
    ready.push_back(x);
    //����̬������̬
    x=ready.back();
    ready.pop_back();
    running.push_back(x);
    //Ȼ������
    writefile();
    running.pop_front();//������̬ɾ��
    if(ready.size()&&running.size()==0){
        //����н��̴��ھ���̬����ô�Ӿ���̬�ó�һ�����̽�������̬
        x=ready.front();
        ready.pop_front();
        running.push_back(x);//��������̬
    }
    dct[i].state=coct[j].state=chct[z].state=0;//�ͷ��豸
    //ɾ������
    for(int k=i;k<DCTsum;k++) dct[k]=dct[k+1];
    DCTsum--;//����������һ
    puts("����ɹ���������a.txt�ļ���");
}

//������ļ�
void readfile(){
    //��a.txt�ļ�
    int fd=open("a.txt",O_RDONLY);
    char buffer[1000];
    int size=read(fd,buffer,sizeof buffer);//��a.txt�ļ��е����ݶ���buffer������
    printf("%s\n",buffer);
    close(fd);//�ر��ļ�
}

//ģ�����
void out(){
    if(running.size()==0){
        puts("û�н��̣�ģ��ʧ��");
        return;
    }
     /*
        һ������̬�Ľ��̣�������Դ����������̬�����뵽��Դ��������̬������̬�ٵ�����̬��
        ���н������ͷ���Դ������ɾ��������̡�
    */
    //����ϵͳ��Դ�����̣�������̬������̬
    PCB x=running.front();
    running.pop_front();
    blocked.push_back(x); //��������̬
    string divicename="screen";   //��Ļ
    bool flag=false;
    int i,j,z; //��¼�豸����������ͨ���������е��±�
    for(i=1;i<=DCTsum;i++){
        if(dct[i].name==divicename){
            if(dct[i].state==0){
                string coctname=dct[i].COCTname;
                //�鿴�������Ƿ�ռ��
                for(j=1;j<=COCTsum;j++){
                    if(coct[j].name==coctname){
                        if(coct[j].state==0){
                            string chctname=coct[j].CHCTname;
                            //�鿴ͨ���Ƿ�ռ��
                            for(z=1;z<=CHCTsum;z++){
                                if(chct[z].name==chctname){
                                    if(chct[z].state==0){
                                        flag=true;  //������Դ�ɹ�
                                        dct[i].state=coct[j].state=chct[z].state=1;
                                        goto loop; //��������ѭ��
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
        puts("������Դʧ��");
        return;
    }
    //�豸����ɹ���������̬������̬
    x=blocked.back();
    blocked.pop_back();
    ready.push_back(x);
    //����̬������̬
    x=ready.back();
    ready.pop_back();
    running.push_back(x);
    //Ȼ������
    readfile();
    running.pop_front();//������̬ɾ��
    dct[i].state=coct[j].state=chct[z].state=0;//�ͷ��豸
    if(ready.size()&&running.size()==0){
        //����н��̴��ھ���̬����û������̬�Ľ��̣���ô�Ӿ���̬�ó�һ�����̽�������̬
        x=ready.front();
        ready.pop_front();
        running.push_back(x);//��������̬
    }
    //ɾ������
   for(int i=1;i<=PCBsum;i++){
        if(x.id==pcb[i].id){
            for(int j=i;j<PCBsum;j++){
                pcb[j]=pcb[j+1];
            }
        }
   }
   PCBsum--;
}

//�豸����ϵͳ
void deviceManager(){
    while(true){
        menu1();
        printf("       ��ѡ��:");
        int x;
        cin>>x;
        switch(x){
            case 1:showDCT();break;//��������豸��Ϣ
            case 2:showCOCT();break;//�����������Ϣ
            case 3:showCHCT();break;//���ͨ����Ϣ
            case 4:addDCT();break;    //����豸
            case 5:deleteDCT();break;  //ɾ���豸
            case 6:addCOCT();break;  //��ӿ�����
            case 7:deleteCOCT();break; //ɾ��������
            case 8:addCHCT();break;   //���ͨ��
            case 9:deleteCHCT();break; //ɾ��ͨ��
            case 10:in();break; //ģ������
            case 11:out();break;//ģ�����
            case 12:deviceAllocation();break;//�豸����
            case 13:releaseEquipment();break;//�ͷ���Դ
            case 14:showForm();break;//����豸�����
            case 15:return; //�������������˵�
            default:printf("�������\n");break;
        }
    }
}

//1.�����ȷ����㷨
void FCFS(int *arr ,int n,int x){
    printf("�����������ڵĴŵ�λ��:\n");
    for(int i=1;i<=n;i++){
        cout<<arr[i]<<" ";
    }
    cout<<endl;
    printf("��ͷ��ǰ����λ�ã�%d\n",x);

    printf("����˳��Ϊ��\n");
    for(int i=1;i<=n;i++){
        cout<<arr[i]<<" ";
    }
    cout<<endl;
    //abs��������һ�������ľ���ֵ
    int sum=abs(arr[1]-x);
    for(int i=2;i<=n;i++){
        sum+=abs(arr[i]-arr[i-1]);
    }
    double avg=1.0*sum/n;
    printf("ƽ��Ѱ������:%.2f\n",avg);
}

//2.���Ѱ��ʱ�������㷨
void SSTF(int *arr,int n,int x){
    printf("�����������ڵĴŵ�λ��:\n");
    for(int i=1;i<=n;i++){
        cout<<arr[i]<<" ";
    }
    cout<<endl;
    printf("��ͷ��ǰ����λ�ã�%d\n",x);
   bool use[30];
   for(int i=1;i<=n;i++) use[i]=false;
   int p=x;//��ǰ��ͷ����λ�á�
   int sum=0;

   printf("����˳��\n");
   for(int i=1;i<=n;i++){
        //�ҵ�ǰ��p���������λ��
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
   printf("ƽ��Ѱ���ܳ�:%.2f\n",1.0*sum/n);
}

//3.���ݵ����㷨
void SCAN(int* arr,int n,int p){
    int x;
    while(true){
        printf("��ѡ���ͷ�ƶ��ķ���1���ŵ�����ķ���2���ŵ���С�ķ���\n");
        cin>>x;
        if(x!=1&&x!=2) puts("���������������");
        else break;
    }
    printf("�����������ڵĴŵ�λ��:\n");
    for(int i=1;i<=n;i++){
        cout<<arr[i]<<" ";
    }
    cout<<endl;
    printf("��ͷ��ǰ����λ�ã�%d\n",p);

    printf("����˳��\n");
    int sum=0;
    int cnt[N+2];//��¼ÿ���ŵ��ϵ���������
    for(int i=0;i<=N;i++) cnt[i]=0;
    for(int i=1;i<=n;i++) cnt[arr[i]]++;
    int num=0;//��¼�Ѿ�����˶��ٸ�����
    if(x==1){
        //�Ȱ���ͷ������ɨ��
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
        //�Ȱ���ͷ��С����ɨ��
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
    printf("ƽ��Ѱ������:%.2f\n",1.0*sum/n);
}

//4.ѭ��ɨ������㷨
void CSCAN(int *arr,int n,int p){
    printf("�����������ڵĴŵ�λ��:\n");
    for(int i=1;i<=n;i++){
        cout<<arr[i]<<" ";
    }
    cout<<endl;
    printf("��ͷ��ǰ����λ�ã�%d\n",p);

    printf("����˳��\n");
    int cnt[N+2];//��¼ÿ���ŵ��ϵ���������
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
    printf("ƽ��Ѱ������%.2f\n",1.0*sum/n);

}

//���̵����㷨
void diskScheduling(){
    int n=20;//������ʵ�����
    int arr[30];
    srand(time(0)); //���������
    //�������20�����ʴŵ���λ��
    for(int i=1;i<=20;i++){
        arr[i]=rand()%N;   //���������0��N֮�������
    }
    int p=rand()%N;//��ͷ��ǰ���ڴŵ�
    while(true){
        menu2();
        printf("     ��ѡ��");
        int x;
        scanf("%d",&x);
        switch(x){
            case 1:FCFS(arr,n,p);break;//�����ȷ����㷨
            case 2:SSTF(arr,n,p);break;//���Ѱ��ʱ�������㷨
            case 3:SCAN(arr,n,p);break;//���ݵ����㷨(ɨ���㷨)
            case 4:CSCAN(arr,n,p);break;//ѭ��ɨ������㷨
            case 5:return;//�������˵�
        }
    }
}

int main(){
    init();//��ʼ���豸����������ͨ��
    while(true){
        menu();
        printf("      ��ѡ��:");
        int  x;
        scanf("%d",&x);
        switch(x){
            case 1:process_manager();break; //���̹���
            case 2:deviceManager();break;  //�豸����
            case 3:diskScheduling();break; //���̵���
            case 4:execlp("clear","clear",NULL);exit(0);//�����ں˺���������Ȼ�����
            //execlp����ʧ���з���ֵ�����гɹ�û�з���ֵ�����гɹ���ǰ���̽�����
            default:printf("�������\n");
        }
    }
    return 0;
}
