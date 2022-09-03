Client-to-Server Communication
In HTTP 1.1, all connections between the browser and the server are considered persistent unless declared otherwise. Persistence, in this context, means to use a single TCP connection to send and receive multiple HTTP requests/responses, as opposed to opening a new connection for every single request/response pair.


## Thread per connection vs Thread per request

### Thread per connection

Until Tomcat 6, which is the implementation of Java EE 5 (and hence servlet 2.5), the default HTTP connector is asynchronous blocking and follows a one thread per connection model. This means that, in order to serve 100 concurrent users, it requires 100 active threads. We end up wasting resources (the thread), because connections may not be used heavily, but just enough to avoid a timeout.

In order to configure tomcat to use the non-blocking NIO connector instead of the default asynchronous blocking BIO, one simply needs to change the value of the protocol attribute of the connector tag in the server.xml from HTTP/1.1 to org.apache.coyote.http11.Http11NioProtocol.

![IMAGE](resources/C08BEF6B9D7DE1BCD8AE69C07C5913EE.jpg =579x252)

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





