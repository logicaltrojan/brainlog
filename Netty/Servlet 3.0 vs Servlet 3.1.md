https://dzone.com/articles/servlet-31spring-mvc-non-blocking-io
https://hadev.tistory.com/29


## Servlet 3.0 

- Request Thread는 바로 반환되는것이 ok
- but background thread can be blocked since I/O opreation is blocking
- Prevents Request Threads busy, but Active Thread is still same

```java

@WebServlet(name="myServlet", urlPatterns={"/asyncprocess"}, asyncSupported=true)
public class MyServlet extends HttpServlet {

    public void doGet(HttpServletRequest request, HttpServletResponse response) {
    
        OutputStream out = response.getOutputStream();
        AsyncContext aCtx = request.startAsync(request, response); 
        doAsyncREST(request).
          thenAccept(json -> { 
            out.write(json);   // BLOCKING! 
            ctx.complete();
          });
    }

```