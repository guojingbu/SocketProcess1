#include <jni.h>
#include <string>
#include <string.h>
#include "native-lib.h"
int m_child;
const char *userId;

const char *PATH = "/data/data/com.yesway.socketprocess/my.sock";

void child_do_work() {
    //开启socket 服务端
    if(child_create_channel()){
        child_listen_msg();
    }

}
/**
 * 服务端读取信息
 * @return
 */

void child_listen_msg() {
        fd_set rfds;
    //在子进程中运行
    struct  timeval timout = {3,0};
    while (1){
        //清空内容
        FD_ZERO(&rfds);
        //指定要读取的客户
        FD_SET(m_child,&rfds);
        int r = select(m_child+1,&rfds,NULL,NULL,&timout);
        LOGE("读取消息前 %d  ",r);
        if(r>0){
            int pkg[256] = {0};
            //保证所读到的信息时 指定apk客户端
            if(FD_ISSET(m_child,&rfds)){
                //阻塞式函数什么都不读
                int result = read(m_child,pkg, sizeof(pkg));
                LOGE("user Id =  %s",userId);
                //开启服务
                execlp("am","am","startservice","--user",
                       userId,"com.yesway.socketprocess/com.yesway.socketprocess.ProcessService",(char*)NULL);
                break;
            }
        }
    }
}
/**
 * 创建服务端socket
 */
int child_create_channel() {
    //指明要链接的文件
    int listenfd = socket(AF_LOCAL,SOCK_STREAM,0);
    //清空
    unlink(PATH);
    struct sockaddr_un addr;
    //清空内存
    memset(&addr,0, sizeof(sockaddr_un));
    addr.sun_family = AF_LOCAL;
    int connfd = 0;
     strcpy(addr.sun_path,PATH);
    if(bind(listenfd, (const sockaddr *) &addr, sizeof(sockaddr_un))<0){
        LOGE("绑定错误");
        return 0;
    }
    listen(listenfd,5);
    //保证宿主进程链接成功 百度全家桶
    while (1){
        //返回值为客户端地址
        if((connfd =  accept(listenfd,NULL,NULL))<0){
            if(errno = EINTR){
                continue;
            }else{
                LOGE("读取失败");
                return 0;
            }

        }
        m_child = connfd;
        LOGE("apk 父进程链接上了  %d" ,m_child);
        break;
    }
    return 1;
}
//创建一个socket
extern "C"
JNIEXPORT void JNICALL
Java_com_yesway_socketprocess_Watcher_creatWatcher(JNIEnv *env, jobject instance, jstring userId_) {
    userId = env->GetStringUTFChars(userId_, 0);

    //开启一个进程
    pid_t pid = fork();
    if(pid<0){
        //开启进程失败
    }else if(pid==0){
        //子进程 守护进程
        child_do_work();
    }else if(pid<0){
        //父进程
    }
    env->ReleaseStringUTFChars(userId_, userId);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_yesway_socketprocess_Watcher_connectMonitor(JNIEnv *env, jobject instance) {
    //在apk进程运行
    LOGE("客戶端 父進程開始鏈接了");
    int socked;
    struct sockaddr_un addr;
    while(1){

        socked = socket(AF_LOCAL,SOCK_STREAM,0);
        if(socked<0){
            LOGE("链接失败");
            return ;
        }
        //清空内存
        memset(&addr,0, sizeof(sockaddr_un));
        addr.sun_family = AF_LOCAL;
        strcpy(addr.sun_path,PATH);
        if(connect(socked, (const sockaddr *) &addr, sizeof(sockaddr_un)) < 0){
            LOGE("链接失败");
            close(socked);
            sleep(1);

            continue;
        }
        LOGE("链接成功");
        break;
    }

}