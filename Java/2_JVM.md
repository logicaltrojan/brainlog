

# JVM 

## 인터프리팅 / 클래스 로딩 

JVM : Stack 기반 interpreting machine
레지스터는 없지만 일부 결과를 스택에 보관하고, 맨 위의 값을 계산함

```java

public static void main(String[] args){
System.out.println("Hello, World!");
}
```

```sh
java HelloWorld
```

1. OS java binary 실행
2. stack machine init
3. BootStrap loader loads core class (Object, Class.Classloader)
    - BootStrap Class loader is not java class
    - It loads pure java class loader 
    - load class rt.jar 
4. Extension Class Loader loads core class 
    - jre/lib/ext
5. Application Class Loader(System class Loader) 
    - CLASSPATH env 
    - classpath / -cp class path option

<img src="https://media.geeksforgeeks.org/wp-content/uploads/jvmclassloader.jpg">


- 자바 실행중 처음보는 class는 dependency에 로드
- 만약 자신이 찾지 못할 경우, 부모 class에 delegate
- bootstrap까지 찾지 못할 경우 ClassNotFoundException
- 똑같은 클래스지만, 다른 classloader가 로드하는 경우도 있음(hierarchy에서 방지하고 있기 떄문에, jvm 구현에 따라서 여러개의 application class loader를 사용할 경우? )
- class = classloader + full class name으로 구별됨 


## ByteCode exec

javac compile

java -> class(bytecode)


컴포넌트 | 설명
-|-
magic num | 0xCAFEBABE
class file format ver | major + minor
constant pool | 클래스 상수들 위치
access flag | abstract, static 
this class | current class name 
super class | super class name 
interface | class implemented interface
field | every fields
mathod | every methods
attribute | class attribute(code file name)


```sh
00000000: cafe babe 0000 003d 001d 0a00 0200 0307  .......=........  # magic num + major + minor 
00000010: 0004 0c00 0500 0601 0010 6a61 7661 2f6c  ..........java/l
00000020: 616e 672f 4f62 6a65 6374 0100 063c 696e  ang/Object...<in
00000030: 6974 3e01 0003 2829 5609 0008 0009 0700  it>...()V.......
00000040: 0a0c 000b 000c 0100 106a 6176 612f 6c61  .........java/la
00000050: 6e67 2f53 7973 7465 6d01 0003 6f75 7401  ng/System...out.
00000060: 0015 4c6a 6176 612f 696f 2f50 7269 6e74  ..Ljava/io/Print
00000070: 5374 7265 616d 3b08 000e 0100 0c48 656c  Stream;......Hel 
00000080: 6c6f 2c20 576f 726c 640a 0010 0011 0700  lo, World.......
00000090: 120c 0013 0014 0100 136a 6176 612f 696f  .........java/io
000000a0: 2f50 7269 6e74 5374 7265 616d 0100 0770  /PrintStream...p
000000b0: 7269 6e74 6c6e 0100 1528 4c6a 6176 612f  rintln...(Ljava/
000000c0: 6c61 6e67 2f53 7472 696e 673b 2956 0700  lang/String;)V..
000000d0: 1601 000a 4865 6c6c 6f57 6f72 6c64 0100  ....HelloWorld..
000000e0: 0443 6f64 6501 000f 4c69 6e65 4e75 6d62  .Code...LineNumb
000000f0: 6572 5461 626c 6501 0004 6d61 696e 0100  erTable...main..
00000100: 1628 5b4c 6a61 7661 2f6c 616e 672f 5374  .([Ljava/lang/St
00000110: 7269 6e67 3b29 5601 000a 536f 7572 6365  ring;)V...Source
00000120: 4669 6c65 0100 0f48 656c 6c6f 576f 726c  File...HelloWorl
00000130: 642e 6a61 7661 0021 0015 0002 0000 0000  d.java.!........
00000140: 0002 0001 0005 0006 0001 0017 0000 001d  ................
00000150: 0001 0001 0000 0005 2ab7 0001 b100 0000  ........*.......
00000160: 0100 1800 0000 0600 0100 0000 0300 0900  ................
00000170: 1900 1a00 0100 1700 0000 2500 0200 0100  ..........%.....
00000180: 0000 09b2 0007 120d b600 0fb1 0000 0001  ................
00000190: 0018 0000 000a 0002 0000 0007 0008 0008  ................
000001a0: 0001 001b 0000 0002 001c                 ..........
```

```sh
Compiled from "HelloWorld.java"
public class HelloWorld {
  public HelloWorld();
    Code:
       0: aload_0
       1: invokespecial #1                  // Method java/lang/Object."<init>":()V
       4: return

  public static void main(java.lang.String[]);
    Code:
       0: getstatic     #7                  // Field java/lang/System.out:Ljava/io/PrintStream;
       3: ldc           #13                 // String Hello, World
       5: invokevirtual #15                 // Method java/io/PrintStream.println:(Ljava/lang/String;)V
       8: return
}
```


## JVM 

https://medium.com/platform-engineer/understanding-jvm-architecture-22c0ddf09722

https://blog.jamesdbloom.com/JVMInternals.html#constant_pool

<img src="https://miro.medium.com/max/720/0*GMXQBZCEpGQMBjy-">


### Class Loader

- runtime시 class를 reference 하는 경우, load / link /initialize ( not compiletime)


#### ClassLoader -  Loading 

- class file을 memory로 load 하는 것이 class loader의 주역할
- static main declaration으로 부터 class loading이 시작됨 
- subsequent class loading은 다음과 같은 상황에서 발생
    - byte code make a static referenc( System.out )
    - bytecode create a class Object ( Person person = new Person )


4 pricinple of class loader

1. visibility 
child can see parent loader loaded class, but parent can't see child loaded class

2. uniqueness
class loaded by parent should not be loaded by child

3. delegation hierarchy
to satisfy principle2, class loader delegates the request to upper class 

4. no unloading
current class lodaer can be deleted and new class loader can be created, but no unloading the loaded class


#### ClassLoader - Linking

verifying and preparing a loaded class or interface(direct superclass / super interface, element type if necessary )


1. Verification - ensure the correctness of .class file

- most complicated test process of the class load processes, and takes the longest time
- If verification fails, it throws runtime errors (java.lang.VerifyError)


checking ...
- consistent and correctly formatted symbol table
- final methods / classes not overridden
- methods respect access control keywords
- methods have correct number and type of parameters
- bytecode doesn’t manipulate stack incorrectly
- variables are initialized before being read
- variables are a value of the correct type


2. Preparation - allocate memory for static storage and any data structures used by the JVM such as method tables

- Static fields are created and initialized to their default values
- however, no initializers or code is executed at this stage as that happens as part of initialization

3. Resolution - replace symbolic ref to direct ref

- searching into method area to locate the referenced entity


#### ClassLoader - Initialization 

the initialization logic of each loaded class or interface will be executed (e.g. calling the constructor of a class)
initialization of a class or interface should happen very carefully with proper synchronization to avoid some other thread from trying to initialize the same class or interface at the same time 

This is the final phase of class loading where all the static variables are assigned with their original values defined in the code and the static block will be executed (if any). 
This is executed line by line from top to bottom in a class and from parent to child in class hierarchy.

```java
protected Class<?> loadClass(String name, boolean resolve) throws ClassNotFoundException {
        synchronized(this.getClassLoadingLock(name)) {
            Class<?> c = this.findLoadedClass(name);
            if (c == null) {
                long t0 = System.nanoTime();

                try {
                    if (this.parent != null) {
                        c = this.parent.loadClass(name, false);
                    } else {
                        c = findBootstrapClassOrNull(name);
                    }
                } catch (ClassNotFoundException var10) {
                }
```


## Runtime Data Area


JVM에게 할당된 메모리 영역 
class loader 가 아래와 같은 정보를 method area에 보관

- Class Fully qualified name / parent class
- class / interface /enum 여부 
- modiifer/ static var / method infol


class file마다 한개의 object 파일을 만들어 heap에 배치 - 차후 access 용 

### Method Area - Thread Shares

- JVM 에는 한개의 method Area 존재
- 모든 threads는 method area 공유 
- method data / dynamic linking은 thread safe여야 한다 

stores 

- ClassLoader reference 
- Runtime constant pool
    - Numeric constants, field reference, method reference, attribut , constant
    -
```java
System.out.println("Hello, World!");

```
```sh
0:   getstatic       #2; //Field java/lang/System.out:Ljava/io/PrintStream;
3:   ldc     #3; //String Hello, world!
5:   invokevirtual   #4; //Method java/io/PrintStream.println:(Ljava/lang/String;)V

```
```sh
public class HelloWorld
  minor version: 0
  major version: 61
  flags: (0x0021) ACC_PUBLIC, ACC_SUPER
  this_class: #21                         // HelloWorld
  super_class: #2                         // java/lang/Object
  interfaces: 0, fields: 0, methods: 2, attributes: 1
Constant pool:
   #1 = Methodref          #2.#3          // java/lang/Object."<init>":()V
   #2 = Class              #4             // java/lang/Object
   #3 = NameAndType        #5:#6          // "<init>":()V
   #4 = Utf8               java/lang/Object
   #5 = Utf8               <init>
   #6 = Utf8               ()V
   #7 = Fieldref           #8.#9          // java/lang/System.out:Ljava/io/PrintStream;
   #8 = Class              #10            // java/lang/System
   #9 = NameAndType        #11:#12        // out:Ljava/io/PrintStream;
  #10 = Utf8               java/lang/System
  #11 = Utf8               out
  #12 = Utf8               Ljava/io/PrintStream;
  #13 = String             #14            // Hello, World
  #14 = Utf8               Hello, World
  #15 = Methodref          #16.#17        // java/io/PrintStream.println:(Ljava/lang/String;)V
  #16 = Class              #18            // java/io/PrintStream
  #17 = NameAndType        #19:#20        // println:(Ljava/lang/String;)V
  #18 = Utf8               java/io/PrintStream
  #19 = Utf8               println
  #20 = Utf8               (Ljava/lang/String;)V
  #21 = Class              #22            // HelloWorld
  #22 = Utf8               HelloWorld
  #23 = Utf8               Code
  #24 = Utf8               LineNumberTable
  #25 = Utf8               main
  #26 = Utf8               ([Ljava/lang/String;)V
  #27 = Utf8               SourceFile
  #28 = Utf8               HelloWorld.java
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
      stack=2, locals=1, args_size=1
         0: getstatic     #7                  // Field java/lang/System.out:Ljava/io/PrintStream;
         3: ldc           #13                 // String Hello, World
         5: invokevirtual #15                 // Method java/io/PrintStream.println:(Ljava/lang/String;)V
         8: return
      LineNumberTable:
        line 7: 0
        line 8: 8
}
```
- 기본적으로 System.out.println 이라는 method name을 symbolic ref #15로 가지고 있음
- #15는 #16 / #17 ( Method Class / String Name And Type #19 , 20 으로 가지고 있음 )
- Field data - per method : name, type, modifiers, attribute
- Method data - per mtehod : name, return type, parameter tyep 
- Method code  - per method : bytecodes, operand stack size, local variable size, local variable table, exception table 



### Heap Area - Thread Shares

- 1 heap area per JVM 
- information of all object / correspoding instance vairable , arrays 
- data stored in Method / Heaps are not thread safe 

### Stack Area - Per Thred 

- not shread 
- for every JVM thread 
- seperate runtime stack created 
- every method call , Stack frame created and pushed to runtime stack 

<img src="https://miro.medium.com/max/640/0*9GyWqgKUyoo-F2_g">

- stack frame is popped if return / mtehod exception 
- stack frame : local var + operand stack + reference to constant pool 
- stack can be dynamic / fix , thred required a larger stack than allowed => stackoverflow, thread required new frame and no memory => OOM 

local var 

- start from 0
- 0 is there reference of a class instance method belong(this)
- from 1, method parameter saved 
- after method parameter, local variable of method are saved

Operand stack 

- runtime workspace 
- method invoke result push / pop 


Reference to constant pool

- symbols related to method 


### PC Registers 

- separate PC register created to hold current executing instruction 
- if current method is native, PC undefined

### Native Method Stack 

- A seperate native stack for store native method info 

## Execution Engine

### Interpreter 

- interpretes byte code 
- executes the instruction one-by-one

### JIT(Just-In-Time) Compiler

- if one method called multiple time, each time interpretion occur, which a redundant
- compiles the entire byt code to native code 
- provides native code to method call 
- native code stored in cache 
- Not all code is compiled, only the worth


### Garbage Collector

 long as an object is being referenced, the JVM considers it alive. Once an object is no longer referenced and therefore is not reachable by the application code, the garbage collector removes it and reclaims the unused memory



## JVM Threads

- 프로그램을 수행하는 데 사용하는 application thread 
- JVM background task 를 수행하기 위한 system thread
-
- application thread는 main thread를 시작으로 만들어짐 (main thread가 전부 만듬)
- system thread는 jvm의해 만들어지며 관리됨

Compiler thread - byte code to native code compiling
GC thread 
Periodic task thread - timer event 
Signal dispatcher thread - receive signal, and handle inside the JVM by calling appropirate JVM method
VM thread - jvm safe point에서 일어나야 하는 operation 을 handle, 예를 들어 stop-the-world 의 경우, Thread 에서 더이상 객체 생성이 일어나지 않는 것이 보장되어야 하므로, 이 thread에서 task를 수행
