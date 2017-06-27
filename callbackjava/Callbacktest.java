import com.sun.tools.jconsole.JConsoleContext;

/**
 * Created by zhangbo on 17/6/27.
 */
public class Callbacktest {

    public static void main(String[] args){
        Client ob = new Client();
        System.out.println("start synchronized call");
        ob.ConnectServer("test1",new mycallback1());
        ob.ConnectServer("test2",new mycallback2());
        System.out.println("end syn");

        System.out.println("start asynchronized call");
        ob.ConnectServer_async("test3",new mycallback1());
        System.out.println("end asyn");

    }
}

interface callbakinterface{
    void process(String input);
}

class Server{
    private callbakinterface m_callback = null;

    public void setCallBack(callbakinterface ob){
        this.m_callback = ob;
    }

    public void dosome(String input){
        input= input + "_server";
        try {
            Thread.sleep(2000);
        }catch(Exception e){
            e.printStackTrace();
        }
        m_callback.process(input);
    }

}

class Client{
    public void ConnectServer(String input,callbakinterface callbackob){
        Server ob = new Server();
        ob.setCallBack(callbackob);
        ob.dosome(input);
    }

    public void ConnectServer_async(final String input,final callbakinterface callbackob){
        new Thread(new Runnable() {
            public void run() {
                Server ob = new Server();
                ob.setCallBack(callbackob);
                ob.dosome(input);
            }
        }).start();
    }
}

class mycallback1 implements callbakinterface{

    public void process(String input) {
        input += " mycallback1";
        System.out.println(input);
    }
};

class mycallback2 implements callbakinterface{

    public void process(String input) {
        input += " mycallback2";
        System.out.println(input);
    }
};

