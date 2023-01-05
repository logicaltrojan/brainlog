

## Connection Vs Request 

### Connection 

- Socket Connection 
- A connection is a TCP-based reliable pipeline between two endpoints. Each connection requires keeping track of both endpoint addresses/ports, sequence numbers, and which packets are unacknowledged or presumed missing. There may be zero or more connections per session
- TCP 에서 three way hand shake 이후의 pipe line 
- keep -alive 등으로 계속 연결된다 


### Request

- Connection이 맺어진 이후, client가 server에게 보내는 요청 
- 한개의 Connection에서 여러개의 Request를 보낼 수 있다. 


## Connector in Tomcat

- Socket Connection을 accept 함 
- Parse 후 ServletRequest로 변환하여 Servlet으로 forwarding

## BIO connector vs  NIO Connector

### BIO Connector  - Thread per connection

- 1 connection 당 1 thread 를 사용한다. 
- connection을 맺고 client가 10초 간격으로 request를 보내도, thread는 계속 할당되어 있다 
- user측에서 connection을 놓아주던가.. timeout으로 돌아오는 수 밖에..

<img src="https://velog.velcdn.com/images%2Fjihoson94%2Fpost%2F21e95eac-84c7-4933-8af4-f33717fc3a8d%2F%E1%84%89%E1%85%B3%E1%84%8F%E1%85%B3%E1%84%85%E1%85%B5%E1%86%AB%E1%84%89%E1%85%A3%E1%86%BA%202021-05-21%20%E1%84%8B%E1%85%A9%E1%84%92%E1%85%AE%202.37.50.png">

- JIOEndpoint object가 request를 처리함
- JIOEndpoint는 Acceptor 와 Worker를 관리함
- Acceptor는 single thread가 할당된, socket accept 담당
- Worker는 request를 처리할 thread pool임
    - Worker에 thread pool 은 어떻게 설정가능함 - server.tomcat.thread.max  /min-spare 등으로 설정가능
- Acceptor는 socket connection을 받으면 idle한 worker thread가 있는지 확인하고, 없으면 block함


- https://topic.alibabacloud.com/a/details-about-tomcat-connection-font-classtopic-s-color00c1decountfont-and-font-classtopic-s-color00c1dethreadfont-pool-and-details-about-tomcat-font-classtopic-s-color00c1dethreadfont-pool_1_27_32615492.html

### NIO Connector - Theread per request

- 1 request 당 1 thread 사용
- connection마다 selector가 존재하고, 이를 순회하는 poller가 존재함

<img src="https://velog.velcdn.com/images%2Fjihoson94%2Fpost%2Ffa4fe719-c515-45d8-b170-570ecd0051d2%2F%E1%84%89%E1%85%B3%E1%84%8F%E1%85%B3%E1%84%85%E1%85%B5%E1%86%AB%E1%84%89%E1%85%A3%E1%86%BA%202021-05-21%20%E1%84%8B%E1%85%A9%E1%84%92%E1%85%AE%204.09.26.png">

- NIOEndpoint가request를 처리함 
- Acceptor 와 Worker를 가지고 있지만, Poller라는게 추가되었음
- 마찬가지로 Acceptor는 single thread가 할당된, socket accept 담당
- Socket에서 connection을 받으면, 바로 worker에게 할당ㄹ하는 것이 아니라, poller queue에다 넣음 
    - SocketChannel -> NioChannel -> PollerEvent로 바꿔서 넣음
- Socket 마다 Selector 객체가 유지됨 (https://www.baeldung.com/java-nio-selector)
- Poller는 Selector들을 순회하면서, readable socket이 있는지 찾고, 있으면 worker thread를 할당함 
- 마찬가지로 Worker는 request를 처리할 thread pool임
    - Worker에 thread pool 은 어떻게 설정가능함 - server.tomcat.thread.max  /min-spare 등으로 설정가능

from : https://velog.io/@jihoson94/BIO-NIO-Connector-in-Tomcatse


## Thread per connection vs Thread per request


Client-to-Server Communication
In HTTP 1.1, all connections between the browser and the server are considered persistent unless declared otherwise. Persistence, in this context, means to use a single TCP connection to send and receive multiple HTTP requests/responses, as opposed to opening a new connection for every single request/response pair.



### Thread per connection

Until Tomcat 6, which is the implementation of Java EE 5 (and hence servlet 2.5), the default HTTP connector is asynchronous blocking and follows a one thread per connection model. This means that, in order to serve 100 concurrent users, it requires 100 active threads. We end up wasting resources (the thread), because connections may not be used heavily, but just enough to avoid a timeout.

- Servlet 2.5(tomcat 6) 까지 http connector = async blocking 
- 100 concurrent user = 100 active thread 
- 많은 수의 connection 들이 사용된다. 

In order to configure tomcat to use the non-blocking NIO connector instead of the default asynchronous blocking BIO, one simply needs to change the value of the protocol attribute of the connector tag in the server.xml from HTTP/1.1 to org.apache.coyote.http11.Http11NioProtocol.


<img scr="https://docs.google.com/drawings/d/sySb0q7iCcywVOX6uaJBH7Q/image?w=579&h=252&rev=139&ac=1&parent=1s0ScFUZs9bqbMqXakRKzt4ASAj5eZXefYpq1or9klSc">

### Request per connection

Opposed to this is the relatively new NIO or non-blocking connector (based on J2SE 1.4). This connector has a couple of poller threads (usually depends upon the number of cores) used to keep the connection alive for all connected users while worker threads are called whenever data (a new HTTP request) is available. Threads can be allocated to connections only when requests are being processed. When a connection is idle between requests, the thread can be recycled and the connection is placed in a centralized NIO select set to detect new requests without consuming a separate thread. This model, called a thread per request, potentially allows web servers to handle a growing number of user connections with a fixed number of threads. This model leads to a much better sharing of resources (threads) and a larger number of concurrent users can be served from the same server. Java NIO API can be referenced for a better understanding of Channel, Selectors, and other related stuff.

From Tomcat 8.x, NIO is the default connector, and BIO support is dropped altogether from 8.5x.
![IMAGE](resources/6A69A712E9CBA789278F0286FE2DE2FA.jpg =579x270)


https://dzone.com/articles/understanding-spring-reactive-servlet-async

https://dzone.com/articles/servlet-31spring-mvc-non-blocking-io


## Servlet Async 

![IMAGE](resources/322D4FA0BB6DF9E70FBC15E5D7297283.jpg =719x295)
![IMAGE](resources/7E4646DE289D0D5DAF608DBAEF6E207F.jpg =768x510)

Nio connector flow

1. Few threads (1-4 depending on the number of cores) polling the selector, looking for IO activity on a channel on a connection.
2.  When the selector sees the IO activity, it calls a handle method on the connection and a thread from pool is allocated to process.
3. Thread will attempt to read the connection and parse it and for HTTP connection. If the request headers are complete, the thread goes on to call the handling of the request (eventually this gets to the servlet) without waiting for any content.
4. Once a thread is dispatched to a Servlet, it looks to it like the Servlet IO is blocking, and hence, any attempt to read/write data from the  HttpInputStream / HttpOutputStream should block. But, as we are using the NIO connector underneath, the IO operations using HttpInputStream and HttpOutputStream are async with callbacks. Due to the blocking nature of the Servlet API, it uses a special blocking callback to achieve blocking.

//nio connector -> a lot of request + a short time in servlet  
//but , a lot of request + lot time in servlet 
// blocking 
// 서블릿은 비동기 i/o api로 동작했기 때문에 ...


Prior to Servlet 3.0 

- Actual request processing, which is blocking in nature, is done by a thread
- Default thread pool size is 200, which implies that only 200 requests can be served concurrently
- Synchronous processing of request resulted run out of threads due to long running requests


![IMAGE](resources/7A321ACE48B63E221760C2CAB7E5389B.jpg =556x301)

 
Async Servlet in 3.0

- A given HTTP request thread handles an incoming request and then passes the request to another background thread, which will be responsible for processing the request and sends the response back to client
- Initial HTTP request thread will return to the HTTP thread pool as soon as it passed the request to the background thread
![IMAGE](resources/51FE0079E3F771CD6433542B49CDADA5.jpg =551x406)

```java
@WebServlet(name="myServlet", urlPatterns={"/asyncprocess"}, asyncSupported=true)
public class MyServlet extends HttpServlet {
    public void doGet(HttpServletRequest request, HttpServletResponse response) {
        OutputStream out = response.getOutputStream();
        AsyncContext aCtx = request.startAsync(request, response);
        //process your request in a different thread
        Runnable runnable = new Runnable() {
            @Override
            public void run() {
                String json ="json string";
                out.write(json);
                ctx.complete();
            }
        };
        //use some thread pool executor
        poolExecutor.submit(runnable);
    }
}
```

- When asyncSupported is set true, the response object is not commited on method exit
- Without any delay, doGet returns, and the original request thread is recycled
- Can configure ThreadPoolExecutor on server start up
- HttpServeltResponse.getOutputStream().write(), and complete() will commit the reponse(or forward to direct to jsp)

This whole behavior is defined above for serlvets can be achieved by returning callable, DefferedResult or CompletableFuture from Spring Controller

This may solve the problem of HTTP thread pool exhaustion, but it will not solve the problem of system resources comsumption

**After all, another background thread was created for processing the request, so the number of simultaneously active threads will not decrease and the system resource consumption will not be improved**



## Comparison blocking Request Processing vs AsyncBlockingRequestProcessing

```java
 @GetMapping(value = "/blockingRequestProcessing")

    public String blockingRequestProcessing() {

        logger.debug("Blocking Request processing Triggered");

        String url = "http://localhost:8090/sleep/1000";

        new RestTemplate().getForObject(url, Boolean.TYPE);
        //sleep for 1 second

        return "blocking...";

    }

```

```java
    @GetMapping(value = "/asyncBlockingRequestProcessing")

    public CompletableFuture<String> asyncBlockingRequestProcessing(){

        return CompletableFuture.supplyAsync(() -> {

            logger.debug("Async Blocking Request processing Triggered");

            String url = "http://localhost:8090/sleep/1000";

            new RestTemplate().getForObject(url, Boolean.TYPE);
            //sleep for 1 second

            return "Async blocking...";

        },asyncTaskExecutor);

    }
```



Nio Connector Count : 100000
Thread Pool Count : 10
20 request/sec  * 60 sec 


Sync Blocking Request Processing
![IMAGE](resources/EFC29936DA49D1E3FCC9049ADBD7D9B3.jpg =779x193)

- Every second, 10 request is queued in
- Error is 50%, since timeout is 60 second


Async Blokcing Request Processing


![IMAGE](resources/8C08D807B43EAEC4E1F000F73DFFD88F.jpg =972x191)

- Returning Callabel release the Tomcat thread, and processing is executed on another thread
- The another thread will be managed by the Spring MVC task executor configured

Request to sleeping-service is still blocking, but it is blocking a different thread





