Basic I/O operations (bind(), connect(), read(), and write()) depend on primitives supplied by the underlying network transport. In Java-based networking, the funda- mental construct is class Socket. Netty’s Channel interface provides an API that greatly reduces the complexity of working directly with Sockets. Additionally, Channel is the root of an extensive class hierarchy having many predefined, specialized implementa- tions, of which the following is a short list:

- EmbeddedChannel : 
  - transports data without teh need for a real network connection
  - can simulate incoming messages by writing data on the inbound channels and also check the generated response on the outbound channels
  - can test each individual ChannelHandler  
- LocalServerChannel
  - data transport vm communication
- NioDataGramChannel
  - udp transportation
- NioSctpChannel
  - Stream Control Transport Protoco
- NioSocketChannel



EventLoop 

- EventLoopGroup 1 : 1 - N EventLoop
- EventLoop is bound to single thread
- All I/O evnets processed by an EventLoop are handled on dedicated thread
- All Channel is registered for its lifetime with a single EventLoop
- EventLoop 1  : N Channel



![IMAGE](resources/5BED9F5EBA08DCAD9A5A0FCB5B2D3A01.jpg =439x253)




ChannelFuture

- registers a ChannelFutureListener to be notified when an operation has completed (whether or not successfully).


ChannelHandler 
- triggered by events
- ChannelInboundHandler 


ChannelPipeline
- container for a chain of ChannelHandlers 
- defines an API for propogation the flow of inbound and outbound event
- when Channel created, automatically assigned its own ChannelPipeline
  -  ChannelInitializer implementation is registered with a ServerBootstrap. (this is why we didnt register handler by ourselves)
  - When ChannelInitializer.initChannel() is called, the ChannelInitializer
installs a custom set of ChannelHandlers in the pipeline.
  - The ChannelInitializer removes itself from the ChannelPipeline.
  
 
 
Each channel has its own pipeline and it is created automatically when a new channel is created.

```java
   ServerBootstrap b = new ServerBootstrap();
        b.group(eventLoopGroup)
                .channel(NioServerSocketChannel.class)
                .localAddress(new InetSocketAddress(8080))
                .childHandler(new ChannelInitializer<SocketChannel>() {
                    @Override
                    protected void initChannel(SocketChannel ch) throws Exception {
                        System.out.println("Pipeline Size :  " + ch.pipeline().toMap().size()); // Pipeline Size : 1
                        ch.pipeline().addLast(echoServerHandler);
                        ch.pipeline().addLast(echoServerHandler);
                    }
                });
```

```java

    }

    @Override
    public void channelReadComplete(ChannelHandlerContext ctx) throws Exception {

        int size = ctx.pipeline().toMap().size();
        System.out.println("Pipeline Size x : " + size);// Pipeline Size x : 2 
        ctx.writeAndFlush(Unpooled.EMPTY_BUFFER);
    }

```

- ChannelHandlers receive event, execute the processing logic for which they have been implemented, and pass the data to the next handler in the chain.



![IMAGE](resources/69EF7C9B18F26E26E54742824E99E00F.jpg =488x112)


![IMAGE](resources/AA8280958C68A3D45480BD09C18CF116.jpg =473x150)

- Inbound / Outbound handlers are installed in same pipeline
- if a message or any other inbound event is read, it will start from the head of the pipeline and be passed to the first ChannelInboundHandler.
- data will be passed to the next ChannelInboundHandler in the chain. Finally, the data will reach the tail of the pipeline, at which point all processing is terminated.
-  The outbound movement of data (that is, data being written) is identical in concept.
- data flows from the tail through the chain of ChannelOutboundHandlers until it reaches the head. Beyond this point, outbound data will reach the network transport, shown here as a Socket. Typically, this will trigger a write operation


Why use ChannelHandlerAdapter

- ChannelHandler in a pipeline is responsible for forwarding events to the next handler in the chain. These adapter classes (and their subclasses) do this automatically
- default handler implementations in the form of adapter classes, which are intended to simplify the development of an application’s processing logic

- ChannelHandlerAdapter
- ChannelInboundHandlerAdapter 
- ChannelOutboundHandlerAdapter
- ChannelDuplexHandlerAdapter



Encoders and decoders

- inbound message : decoded 
- outbound message : encoded

byte -> format -> byte

- ByteToMessageDecoder 
- MessageToByteDecoder

- Strictly all decoder/encoder are ChannelHandler
- but don't make the wheels again



Bootstrapping

```java
//Client.java
  EventLoopGroup eventLoopGroup = new NioEventLoopGroup();
        Bootstrap b = new Bootstrap()
                .group(eventLoopGroup)
                .channel(NioSocketChannel.class)
                .remoteAddress(new InetSocketAddress(host, port))
                .handler(new ChannelInitializer<SocketChannel>() {
                    @Override
                    protected void initChannel(SocketChannel ch) throws Exception {
                       ch.pipeline().addLast(new EchoClientHandler());
                    }
                });
```

```java
//server.java
        EventLoopGroup eventLoopGroup = new NioEventLoopGroup();
        ServerBootstrap b = new ServerBootstrap();
        b.group(eventLoopGroup)
                .channel(NioServerSocketChannel.class)
                .localAddress(new InetSocketAddress(8080))
                .childHandler(new ChannelInitializer<SocketChannel>() {
                    @Override
                    protected void initChannel(SocketChannel ch) throws Exception {
                        System.out.println("Pipeline Size :  " + ch.pipeline().toMap().size());
                        ch.pipeline().addLast(echoServerHandler);
                        ch.pipeline().addLast(echoServerHandler);
                    }
                });

```


![IMAGE](resources/A2903005A09127360D123B9BBB1FAE17.jpg =502x104)

- ServerBootStrap requires two EventLoopGroup(which can be the same instance) 
- A server needs two distinct sets of Channels 
  - First set will contain single ServerChannle representing the server's own listening socket, bound to local port
  - First set is responsible for creating Channels for incoming connection requests
  - Second  set will contain all of the Channels that have been created to handle incoming client connections - one for each connections the server has accepted 
  
![IMAGE](resources/0A4F9680D8CC52BEC8680776C7B546E2.jpg =446x269)