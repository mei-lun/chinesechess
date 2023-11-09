// 公用的预定义文件
enum EVENT_TYPE
{
    NEW_CLIENT_CONNECT = 1,             // 当有新的客户端连入时
    NETWORK_RECEIVE_NEW_MESSAGE = 2,    // 网络层有新的消息过来了,通知上层取走
};