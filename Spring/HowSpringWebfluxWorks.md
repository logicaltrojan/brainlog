
Webflux supported Webservers 


Servlet 3.1+ container : Tomcat, Jetty 
Non Servlet Runtimes : Netty, Undertow 


All Servers adapted Low-level, common API (server features(connection, read, write) ) so that higher-level programming models(reactor) can supported across servers

Spring webflux doesn't have any webserver, but spring boot automates Netty.

Tomcat and Jetty can be used with both Spring MVC and WebFlux. Keep in mind, however, that the way they are used is very different. Spring MVC relies on Servlet blocking I/O and lets applications use the Servlet API directly if they need to. Spring WebFlux relies on Servlet 3.1 non-blocking I/O and uses the Servlet API behind a low-level adapter. It is not exposed for direct use.

For Undertow, Spring WebFlux uses Undertow APIs directly without the Servlet API.


![IMAGE](quiver-image-url/811944F1CE33FBBBF9D611CE7006658B.jpg =798x408)

| ServerName | Server API used | ReactiveStreamsSupport
-----|------|----|
Netty | NettyAPI | Reactor Netty
Undertow | UndertowAPI | spring-web : Undertow to ReactiveStreams Bridge
Tomact| Servlet 3.1 non-blocking I/O Tomacat API to read and write ByteBuffers vs byt[] | spring-web:Servlet 3.1 non-blocking I/O to Reactive Streams Bridge
Jetty |  Servlet 3.1 non-blocking I/O Tomacat API to read and write ByteBuffers vs byt[] | spring-web:Servlet 3.1 non-blocking I/O to Reactive Streams Bridge
Servlet 3.1 container | Servlet 3.1 non-blocking I/O | spring-web:Servlet 3.1 non-blocking I/O to Reactive Streams Bridge



Concurrency model

Spring MVC : assumed that applications can block the current thread, so servlet containers use a large thread pool to absorb potential blocking during request handling

Spring WebFlux :  it is assumed that applications do not block. Therefore, non-blocking servers use a small, fixed-size thread pool (event loop workers) to handle requests.

Threading Model
What threads should you expect to see on a server running with Spring WebFlux?

Vanilla Spring Webflux server : One thread for the server + Event loop workers thread * CPU core
if Reactor Netty is used for both client and server, the two share event loop resources by default.

Servlet Containers : 
Servlet containers, however, may start with more threads (for example, 10 on Tomcat), in support of both servlet (blocking) I/O and servlet 3.1 (non-blocking) I/O usage.


Reactor and RxJava provide thread pool abstractions, called schedulers, to use with the publishOn operator that is used to switch processing to a different thread pool. The schedulers have names that suggest a specific concurrency strategy — for example, “parallel” (for CPU-bound work with a limited number of threads) or “elastic” (for I/O-bound work with a large number of threads). If you see such threads, it means some code is using a specific thread pool Scheduler strategy.

Data access libraries and other third party dependencies can also create and use threads of their own.



1.2 Reactive Core

The spring-web module contains the following foundational support for reactive web applications:

- For server request processing there are two levels of support.

  - HttpHandler: Basic contract for HTTP request handling with non-blocking I/O and Reactive Streams back pressure, along with adapters for Reactor Netty, Undertow, Tomcat, Jetty, and any Servlet 3.1+ container.
    - HandlerAdpater for each server implementation included 
    - ex) Netty : new ReactorHttpHandlerAdapter(handler)
    - ex) Undertow : new UndertowHttpHandlerAdapter(handler)

  - WebHandler API: Slightly higher level, general-purpose web API for request handling, on top of which concrete programming models such as annotated controllers and functional endpoints are built.

- For the client side, there is a basic ClientHttpConnector contract to perform HTTP requests with non-blocking I/O and Reactive Streams back pressure, along with adapters for Reactor Netty, reactive Jetty HttpClient and Apache HttpComponents. The higher level WebClient used in applications builds on this basic contract.


1.2.1 HttpHandler
- simple contract with a single method to handle a req/re 

```java
HttpHandler handler = ...
ReactorHttpHandlerAdapter adapter = new ReactorHttpHandlerAdapter(handler);
HttpServer.create().host(host).port(port).handle(adapter).bind().block();
```

1.2.2 WebHandler
- org.springframework.web.server package builds on the HttpHandler contract 
- chain of WebExceptionHandler, Mulitple WebFilter + singleWebHandler  

```java
/**
 * This builder has two purposes:
 *
 * <p>One is to assemble a processing chain that consists of a target {@link WebHandler},
 * then decorated with a set of {@link WebFilter WebFilters}, then further decorated with
 * a set of {@link WebExceptionHandler WebExceptionHandlers}.
 *
 * <p>The second purpose is to adapt the resulting processing chain to an {@link HttpHandler}:
 * the lowest-level reactive HTTP handling abstraction which can then be used with any of the
 * supported runtimes. The adaptation is done with the help of {@link HttpWebHandlerAdapter}.
 *
 * <p>The processing chain can be assembled manually via builder methods, or detected from
 * a Spring {@link ApplicationContext} via {@link #applicationContext}, or a mix of both.
 *
 * @author Rossen Stoyanchev
 * @author Sebastien Deleuze
 * @since 5.0
 * @see HttpWebHandlerAdapter
 *
```


1.3 DispatcherHandler

- discovers the delegate component it need from Spring Configuration 
- Inject ApplicationContextAware



<img src='https://img1.daumcdn.net/thumb/R1280x0/?scode=mtistory2&fname=https%3A%2F%2Fblog.kakaocdn.net%2Fdn%2FV4Dt9%2FbtqwP7nPdZ5%2FwLcboq07U3b8AQVq6ST421%2Fimg.png'>



HandlerMapping 

1.RequestMappingHandlerMapping - @RequestMapping 어노테이션 메소드 맵핑
2.RouterFunctionMapping - functional endpoint 방식의 메소드 맵핑
3.SimpleUrlHandlerMapping - 명확한 URI path pattern으로 등록된 메소드 맵핑

HandlerAdapter

HandlerResultHandler



HandlerResultHandler 

|Result Handler Type | Return Values | Default Order|
|---|---|---|
| ResponseEntityResultHandler | ResponseEntity, typically from @Controller instances. | 0
| ServerResponseResultHandler | ServerResponse, typically from functional endpoints.| 0
| ResponseBodyResultHandler | Handle return values from @ResponseBody methods or @RestController classes.|100
|ViewResolutionResultHandler | CharSequence, View, Model, Map, Rendering, or any other Object is treated as a model attribute.| Integer.MAX_VALUE