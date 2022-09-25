


# Hello World 

다음과 같은 HelloWorld를 실행시키면 다음과 같은 결과가 나온다

```sh
Hello, World
```
```java

public class HelloWorld { 

    public static void main(String[] args){

	MyString myString = new MyString();
	System.out.println(myString.getHelloWorld());
	

    }

    public static class MyString {

	public String getHelloWorld(){

	    String helloWorldString = "Hello, World";

	    return helloWorldString;

	}
	 
    }

}

```

이 Hello World가 어떻게 찍히는지 알아보자 

## Java Compile

JVM은 class file을 load하기 때문에, java file을 class file로 compile 해줘야 한다.
class file은 byte code 이다. bytecode 라 해서 기계코드는 아니다. 당연히 01로 이루어져있는것은 같지만(겉모습은) class file의 bytecode 파일은 일반적인 compiler가 생성하는 exec code와 차이가 있다. 

1. 최적화가 이루어져 있지 않다. decompile이 바로 가능함
2. instruction set이 cpu용이 아니라, jvm 용 


class file의 구조는 다음과 같다 
 
```cpp
ClassFile {
    u4			magic;
    u2			minor_version;
    u2			major_version;
    u2			constant_pool_count;
    cp_info		contant_pool[constant_pool_count – 1];
    u2			access_flags;
    u2			this_class;
    u2			super_class;
    u2			interfaces_count;
    u2			interfaces[interfaces_count];
    u2			fields_count;
    field_info		fields[fields_count];
    u2			methods_count;
    method_info		methods[methods_count];
    u2			attributes_count;
    attribute_info	attributes[attributes_count];
}

```

위 java 파일을 컴파일하면 아래와 같이 class file이 구성된것을 확인할 수 있다.
```shell
javac HelloWorld.java
ls | grep .class
```

inner class도 별도의 class file이 생성됨을 알 수 있다.

```sh
HelloWorld$MyString.class
HelloWorld.class
```

class file의 byte representation은 다음과 같이 확인 할 수 있다.

확인해보면 cafe babe라는 magic number ( this file is class file 명시 )
003d -> 16*3 = 48 + d (13) = 61이라는 버전이 명시되어 있음을 알 수 있다. 
```sh
xxd HelloWorld.class
00000000: cafe babe 0000 003d 0025 0a00 0200 0307  .......=.%......
00000010: 0004 0c00 0500 0601 0010 6a61 7661 2f6c  ..........java/l
00000020: 616e 672f 4f62 6a65 6374 0100 063c 696e  ang/Object...<in
00000030: 6974 3e01 0003 2829 5607 0008 0100 1348  it>...()V......H
00000040: 656c 6c6f 576f 726c 6424 4d79 5374 7269  elloWorld$MyStri
00000050: 6e67 0a00 0700 0309 000b 000c 0700 0d0c  ng..............
00000060: 000e 000f 0100 106a 6176 612f 6c61 6e67  .......java/lang
00000070: 2f53 7973 7465 6d01 0003 6f75 7401 0015  /System...out...
00000080: 4c6a 6176 612f 696f 2f50 7269 6e74 5374  Ljava/io/PrintSt
00000090: 7265 616d 3b0a 0007 0011 0c00 1200 1301  ream;...........
000000a0: 000d 6765 7448 656c 6c6f 576f 726c 6401  ..getHelloWorld.
000000b0: 0014 2829 4c6a 6176 612f 6c61 6e67 2f53  ..()Ljava/lang/S
000000c0: 7472 696e 673b 0a00 1500 1607 0017 0c00  tring;..........
000000d0: 1800 1901 0013 6a61 7661 2f69 6f2f 5072  ......java/io/Pr
000000e0: 696e 7453 7472 6561 6d01 0007 7072 696e  intStream...prin
000000f0: 746c 6e01 0015 284c 6a61 7661 2f6c 616e  tln...(Ljava/lan
00000100: 672f 5374 7269 6e67 3b29 5607 001b 0100  g/String;)V.....
00000110: 0a48 656c 6c6f 576f 726c 6401 0004 436f  .HelloWorld...Co
00000120: 6465 0100 0f4c 696e 654e 756d 6265 7254  de...LineNumberT
00000130: 6162 6c65 0100 046d 6169 6e01 0016 285b  able...main...([
00000140: 4c6a 6176 612f 6c61 6e67 2f53 7472 696e  Ljava/lang/Strin
00000150: 673b 2956 0100 0a53 6f75 7263 6546 696c  g;)V...SourceFil
00000160: 6501 000f 4865 6c6c 6f57 6f72 6c64 2e6a  e...HelloWorld.j
00000170: 6176 6101 000b 4e65 7374 4d65 6d62 6572  ava...NestMember
00000180: 7301 000c 496e 6e65 7243 6c61 7373 6573  s...InnerClasses
00000190: 0100 084d 7953 7472 696e 6700 2100 1a00  ...MyString.!...
000001a0: 0200 0000 0000 0200 0100 0500 0600 0100  ................
000001b0: 1c00 0000 1d00 0100 0100 0000 052a b700  .............*..
000001c0: 01b1 0000 0001 001d 0000 0006 0001 0000  ................
000001d0: 0003 0009 001e 001f 0001 001c 0000 0033  ...............3
000001e0: 0002 0002 0000 0013 bb00 0759 b700 094c  ...........Y...L
000001f0: b200 0a2b b600 10b6 0014 b100 0000 0100  ...+............
00000200: 1d00 0000 0e00 0300 0000 0700 0800 0800  ................
00000210: 1200 0b00 0300 2000 0000 0200 2100 2200  ...... .....!.".
00000220: 0000 0400 0100 0700 2300 0000 0a00 0100  ........#.......
00000230: 0700 1a00 2400 09                        ....$..

```



```sh
public class HelloWorld
  minor version: 0
  major version: 61
  flags: (0x0021) ACC_PUBLIC, ACC_SUPER
  this_class: #26                         // HelloWorld
  super_class: #2                         // java/lang/Object
  interfaces: 0, fields: 0, methods: 2, attributes: 3
Constant pool:
   #1 = Methodref          #2.#3          // java/lang/Object."<init>":()V
   #2 = Class              #4             // java/lang/Object
   #3 = NameAndType        #5:#6          // "<init>":()V
   #4 = Utf8               java/lang/Object
   #5 = Utf8               <init>
   #6 = Utf8               ()V
   #7 = Class              #8             // HelloWorld$MyString
   #8 = Utf8               HelloWorld$MyString
   #9 = Methodref          #7.#3          // HelloWorld$MyString."<init>":()V
  #10 = Fieldref           #11.#12        // java/lang/System.out:Ljava/io/PrintStream;
  #11 = Class              #13            // java/lang/System
  #12 = NameAndType        #14:#15        // out:Ljava/io/PrintStream;
  #13 = Utf8               java/lang/System
  #14 = Utf8               out
  #15 = Utf8               Ljava/io/PrintStream;
  #16 = Methodref          #7.#17         // HelloWorld$MyString.getHelloWorld:()Ljava/lang/String;
  #17 = NameAndType        #18:#19        // getHelloWorld:()Ljava/lang/String;
  #18 = Utf8               getHelloWorld
  #19 = Utf8               ()Ljava/lang/String;
  #20 = Methodref          #21.#22        // java/io/PrintStream.println:(Ljava/lang/String;)V
  #21 = Class              #23            // java/io/PrintStream
  #22 = NameAndType        #24:#25        // println:(Ljava/lang/String;)V
  #23 = Utf8               java/io/PrintStream
  #24 = Utf8               println
  #25 = Utf8               (Ljava/lang/String;)V
  #26 = Class              #27            // HelloWorld
  #27 = Utf8               HelloWorld
  #28 = Utf8               Code
  #29 = Utf8               LineNumberTable
  #30 = Utf8               main
  #31 = Utf8               ([Ljava/lang/String;)V
  #32 = Utf8               SourceFile
  #33 = Utf8               HelloWorld.java
  #34 = Utf8               NestMembers
  #35 = Utf8               InnerClasses
  #36 = Utf8               MyString
{
  public HelloWorld();
    descriptor: ()V
    flags: (0x0001) ACC_PUBLIC
    Code:
      stack=1, locals=1, args_size=1
         0: aload_0
         1: invokespecial #1                  // Method java/lang/Object."<init>":()V
         4: return
      LineNumberTable:
        line 3: 0

  public static void main(java.lang.String[]);
    descriptor: ([Ljava/lang/String;)V
    flags: (0x0009) ACC_PUBLIC, ACC_STATIC
    Code:
      stack=2, locals=2, args_size=1
         0: new           #7                  // class HelloWorld$MyString
         3: dup
         4: invokespecial #9                  // Method HelloWorld$MyString."<init>":()V
         7: astore_1
         8: getstatic     #10                 // Field java/lang/System.out:Ljava/io/PrintStream;
        11: aload_1
        12: invokevirtual #16                 // Method HelloWorld$MyString.getHelloWorld:()Ljava/lang/String;
        15: invokevirtual #20                 // Method java/io/PrintStream.println:(Ljava/lang/String;)V
        18: return
      LineNumberTable:
        line 7: 0
        line 8: 8
        line 11: 18
}
SourceFile: "HelloWorld.java"
NestMembers:
  HelloWorld$MyString
InnerClasses:
  public static #36= #7 of #26;           // MyString=class HelloWorld$MyString of class HelloWorld

```


그럼 다음과 같은 class file을 java 로 실행시키면 어떤 일이 일어나는가 

```sh
java HelloWorld
```

#### JVM stats with Class loader loads class to memory 

BootStrap claas loader가 먼저 java core한 부분의 class 를 loading 
Bootstrap class loader는 native 한 Method로 작성되어 있음

Bootstrap -> Extension -> Application (System) -> Application 2( User defined )로 class loader가 hierarchy적으로 생성됨

Loading -> Linking( Verifying-> Preparing->Resolving ) -> Initialization 으로 이루어짐 

Loading : finding file , read to byte array, parsed, confirmed (version check), direct super class loaded(Object) , object representation created from binary

Linking - Verifying : check structure, semantic requirement (final method not override, variable init)
Linking - Preparing : allocation of memory for static stroage, static value init 
Linking - Resolving : checking referenced class 


Initialization : 


MainClass 가 initilze 되고 난 뒤에, main method를 실행하면서 main thread + system thread 가 invoke



#### Main thread + Background Thread

- main method()를 찾은 후, main thread + background thread를 만든다. 
- background thread는 다음과 같다.

VM thread 
Periodic task thread
GC thread
Compiler thread
signal dispatcher thread


```sh
jps -v # get pid
jstack pid 
```

thread dump를 보면 12개의 thread가 생성되었음을 알 수 있다.


```sh
Full thread dump OpenJDK 64-Bit Server VM (17.0.2+8-86 mixed mode, sharing):

Threads class SMR info:
_java_thread_list=0x00006000016500a0, length=12, elements={
0x00007fbd8680d400, 0x00007fbd8580a200, 0x00007fbd85808800, 0x00007fbd75008200,
0x00007fbd8500d000, 0x00007fbd8700ae00, 0x00007fbd8580a800, 0x00007fbd86921200,
0x00007fbd87808a00, 0x00007fbd86923a00, 0x00007fbd85879200, 0x00007fbd76008200
}

"main" #1 prio=5 os_prio=31 cpu=24.80ms elapsed=9.65s tid=0x00007fbd8680d400 nid=0x2503 waiting on condition  [0x000070000af24000]
   java.lang.Thread.State: TIMED_WAITING (sleeping)
	at java.lang.Thread.sleep(java.base@17.0.2/Native Method)
	at HelloWorld.main(HelloWorld.java:9)

"Reference Handler" #2 daemon prio=10 os_prio=31 cpu=0.11ms elapsed=9.63s tid=0x00007fbd8580a200 nid=0x4003 waiting on condition  [0x000070000b639000]
   java.lang.Thread.State: RUNNABLE
	at java.lang.ref.Reference.waitForReferencePendingList(java.base@17.0.2/Native Method)
	at java.lang.ref.Reference.processPendingReferences(java.base@17.0.2/Reference.java:253)
	at java.lang.ref.Reference$ReferenceHandler.run(java.base@17.0.2/Reference.java:215)

"Finalizer" #3 daemon prio=8 os_prio=31 cpu=0.18ms elapsed=9.63s tid=0x00007fbd85808800 nid=0x3c03 in Object.wait()  [0x000070000b73c000]
   java.lang.Thread.State: WAITING (on object monitor)
	at java.lang.Object.wait(java.base@17.0.2/Native Method)
	- waiting on <0x000000061fc02f40> (a java.lang.ref.ReferenceQueue$Lock)
	at java.lang.ref.ReferenceQueue.remove(java.base@17.0.2/ReferenceQueue.java:155)
	- locked <0x000000061fc02f40> (a java.lang.ref.ReferenceQueue$Lock)
	at java.lang.ref.ReferenceQueue.remove(java.base@17.0.2/ReferenceQueue.java:176)
	at java.lang.ref.Finalizer$FinalizerThread.run(java.base@17.0.2/Finalizer.java:172)

"Signal Dispatcher" #4 daemon prio=9 os_prio=31 cpu=0.40ms elapsed=9.63s tid=0x00007fbd75008200 nid=0xa603 waiting on condition  [0x0000000000000000]
   java.lang.Thread.State: RUNNABLE

"Service Thread" #5 daemon prio=9 os_prio=31 cpu=0.05ms elapsed=9.63s tid=0x00007fbd8500d000 nid=0xa403 runnable  [0x0000000000000000]
   java.lang.Thread.State: RUNNABLE

"Monitor Deflation Thread" #6 daemon prio=9 os_prio=31 cpu=0.36ms elapsed=9.63s tid=0x00007fbd8700ae00 nid=0x5a03 runnable  [0x0000000000000000]
   java.lang.Thread.State: RUNNABLE

"C2 CompilerThread0" #7 daemon prio=9 os_prio=31 cpu=3.22ms elapsed=9.63s tid=0x00007fbd8580a800 nid=0x5b03 waiting on condition  [0x0000000000000000]
   java.lang.Thread.State: RUNNABLE
   No compile task

"C1 CompilerThread0" #15 daemon prio=9 os_prio=31 cpu=1.26ms elapsed=9.63s tid=0x00007fbd86921200 nid=0x5c03 waiting on condition  [0x0000000000000000]
   java.lang.Thread.State: RUNNABLE
   No compile task

"Sweeper thread" #19 daemon prio=9 os_prio=31 cpu=0.04ms elapsed=9.63s tid=0x00007fbd87808a00 nid=0x9f03 runnable  [0x0000000000000000]
   java.lang.Thread.State: RUNNABLE

"Notification Thread" #20 daemon prio=9 os_prio=31 cpu=0.04ms elapsed=9.62s tid=0x00007fbd86923a00 nid=0x9e03 runnable  [0x0000000000000000]
   java.lang.Thread.State: RUNNABLE

"Common-Cleaner" #21 daemon prio=8 os_prio=31 cpu=0.10ms elapsed=9.62s tid=0x00007fbd85879200 nid=0x9d03 in Object.wait()  [0x000070000c15d000]
   java.lang.Thread.State: TIMED_WAITING (on object monitor)
	at java.lang.Object.wait(java.base@17.0.2/Native Method)
	- waiting on <0x000000061fc191f8> (a java.lang.ref.ReferenceQueue$Lock)
	at java.lang.ref.ReferenceQueue.remove(java.base@17.0.2/ReferenceQueue.java:155)
	- locked <0x000000061fc191f8> (a java.lang.ref.ReferenceQueue$Lock)
	at jdk.internal.ref.CleanerImpl.run(java.base@17.0.2/CleanerImpl.java:140)
	at java.lang.Thread.run(java.base@17.0.2/Thread.java:833)
	at jdk.internal.misc.InnocuousThread.run(java.base@17.0.2/InnocuousThread.java:162)

"Attach Listener" #22 daemon prio=9 os_prio=31 cpu=0.64ms elapsed=0.10s tid=0x00007fbd76008200 nid=0x5807 waiting on condition  [0x0000000000000000]
   java.lang.Thread.State: RUNNABLE

"VM Thread" os_prio=31 cpu=0.72ms elapsed=9.64s tid=0x00007fbd861117d0 nid=0x3a03 runnable

"GC Thread#0" os_prio=31 cpu=0.11ms elapsed=9.64s tid=0x00007fbd86108a40 nid=0x4c03 runnable

"G1 Main Marker" os_prio=31 cpu=0.05ms elapsed=9.64s tid=0x00007fbd861090e0 nid=0x2f03 runnable

"G1 Conc#0" os_prio=31 cpu=0.02ms elapsed=9.64s tid=0x00007fbd86109960 nid=0x4903 runnable

"G1 Refine#0" os_prio=31 cpu=0.04ms elapsed=9.64s tid=0x00007fbd8610d5c0 nid=0x3303 runnable

"G1 Service" os_prio=31 cpu=1.54ms elapsed=9.64s tid=0x00007fbd8610de50 nid=0x4603 runnable

"VM Periodic Task Thread" os_prio=31 cpu=6.70ms elapsed=9.62s tid=0x00007fbd84f06210 nid=0x5f03 waiting on condition

JNI global refs: 14, weak refs: 0
```

<img src="https://blog.jamesdbloom.com/images_2013_11_17_17_56/JVM_Internal_Architecture_small.png">

<img src="./JVM_Internal_Architecture_small.png">

