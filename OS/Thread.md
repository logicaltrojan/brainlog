

# Thread

---

## What is Thread

- CPU 이용의 기본 단위 
- Thread id, pc, register set, stack 
- Thread는 같은 프로세스에 속한 thread와 code, data, opened file, signal 을 공유한다.
- Process가 Multi thread를 같는 다면 하나 이상의 작업을 수행가능하다.



## Why Thread

- 원래는 process로 함
- 예를 들어 새로운 요청이 올 경우에, fork()로 새로운 프로세스를 만들었음
- 만들고 보니 비효율적, 코드도 같아, 열린 파일도 같아 
- 실행의 조금 더 작은 단위를 쓰면 효율적


### Benefits

1. 응답성 - 일부 긴 작업이 있는 프로세스의 경우, 긴 작업과 별개로 사용자의 응답을 처리할 수 있다
2. 자원 공유 - 프로세스도 공유가 가능하지만, 프로그래머가 명시적으로 해줘야 함. Thread는 자동적으로 프로세스의 자원을 공유함. 코드와 데이터를 공유한다는 것은 같은 주소공간에서 다른 작업을 할 수 있다는 뜻임(물론 대재앙이 될 수 있다)
3. 경제성 - 메모리와 자원은 비용이다. Thread는 단위가 작아서 context switching에도 유리함
4. 확장성 (scailbilty) : 멀티 프로세서 환경에서 더욱 빠른 처리가 가능함(프로세서가 사용하는 단위가 Thread이기 때문)



### Concurrency(병행성) vs Parallism(병렬성) 

<img src="https://devopedia.org/images/article/339/6574.1623908671.jpg">


- Concurrency : 여러 작업을 switching을 통해 병행으로 진행되게 만든다.
- Parallel : 둘 이상의 작업을 동시에 수행한다.
- Single Core에서는 concurrency를 통해 여러 Process의 실행을 지원하였다 

### Programming Challenges of using thread

1. 태스크 인식(identifying tasks) : application의 task를 독립적인 task로 나눌 수 있는 영역을 찾는 작업이 필요
2. 균형(balance) : 전체작업에 실제 균등한 부분으로 task를 나눠야 한다. 어떤 task에 작업이 몰리면 unbalance가 일어난다
3. 데이터 분리(data splitting) : 분리된 task가 접근하는 data도 서로 개별적이고 독립적으로 관리해야한다.
4. 데이터 종속성(data dependency) : 태스크가 접근하는 데이터는 둘 이상의 태스크 사이에 종속성이 없는지 검토되어야 한다. 데이터가 종속적일 경우에는, 태스크가 기다려야 하거나, 아예 나중에 실행되거나 해야함.. 
5. 시험 및 디버깅(testing and debugging) : 병렬 프로그램의 debugging 을 단일 thread의 디버깅보다 어렵다. 일관적으로 나오지 않고 다양한 실행경로를 거치기 때문이다.

### Types of Parellism


- 데이터 병렬 실행
    - 1-100까지의 원소를 더하는 task를 parallel하게 나눌 수 있다
    - 1 - 50 까지 더하는 연산 + 51 + 100까지 더하는 연산
    - 동일한 데이터의 부분집합을 다수의 계산 코어에 분배

- 태스크 병렬 실행
    - 1-100까지의 원소를 더하는 task + 1-100까지의 곱을 구하는 task의 경우 
    - 1-100까지 원소를 더하는 task + 1-100 곱하는 task 
    - 병렬로 실행되지만 같은 데이터를 사용할 수 도 , 사용안 할 수도 

보통 같이 쓴다(1-100까지 더하는 연산 = 1-50 + 51 - 100 ) + (1-100까지 곱하는 연산 = 1-50 * 51-100) => 4개의 task

## Mulitthreading Model 

UserThread Vs KernelThread

<img src="http://www.it.uu.se/education/course/homepage/os/vt18/images/module-4/user-level-and-kernel-level-threads.png?width=777px">


#### Kernel Level Threads 

- The kernel knows about and manages all threads.
- One process control block (PCP) per process.
- One thread control block (TCB) per thread in the system.
- Provide system calls to create and manage threads from user space.

Pros

- Kernel has full knowledge of all threads
- Scheduler may decide to give more cpu time if process have much threads

Cons

- Kernel manage and schedule all threads
- Significant overhead / increase kernel complexity
- Thread operations slow compare to user-level thread


#### User level Threads

User level thread는process 내(library를 사용할 경우) thread scheduler, thread table 등이 존재한다.

- Threads managed entirely by the run-time system (user-level library).
- Ideally, thread operations should be as fast as a function call.
- The kernel knows nothing about user-level threads and manage them as if they where single-threaded processes.

Pros

- Can be implemented on an OS that does not suport kernel-level thread
- Simple representation : PC, register, stack, TCB, **all stored in user-level process memory**
- Simple management : Creating / Switching /synchronzing threads done in user-space with out kernel intervention
- Fast : switching threads not much more expensive than a function call

Cons

- Os may make poor decision
    - scheduling a process with idle threads
    - blocking a process due to a blocking thread even though the process has other threads that can run
    - giving a process as a whole one time slice irrespective of whether the process has 1 or 1000 threads
    - unschedule a process with a thread holding a lock.

http://www.it.uu.se/education/course/homepage/os/vt18/module-4/implementing-threads/

### MuliThreading Model

#### Many to One 

- all user thread가 process에 할당된 하나의 kernel thread에 Mapping
- thread blocking call이 일어날 경우, process 가 block 될 수 있음
- parallel 실행될 수 없음, 한 kernel thread를 여러 user level thread가 돌려씀


#### One to One

- one user thread - one kernel thread mapping
- parallel 가능, blocking 되어도 다른 thread에 영향 없음
- user thread를 많이 만들면 kernel thread가 늘어나기 때문에 주의
- Linux / Windows thread

#### Many to Many

- n user thread : m kernel thread ( n >= m )
- 많은 수의 user thread를 생성하더라도, 특정 개수의 kernel thread에 할당하면 되기 때문에 one-one 겪는 문제 해결
- blocking call 상관 없고/ parallel 도 가능 
- 구현하기가 어려움

**How n:m mapping**

<img src="http://www.it.uu.se/education/course/homepage/os/vt18/images/module-4/scheduler-activations-3-7.png?height=444px">

<img src="http://www.it.uu.se/education/course/homepage/os/vt18/images/module-4/scheduler-activations-3-7.png?height=444px">


- user 3 : 1 kernel mapping
- if blocking call occurs, kernel scheduler allocate new kernel thread to the process
- Upcall : kernel notifies thread manager(user space) 
- User lvele thread manager move the other threads to the new kernel thread and resumes

## Thread Library

- 프로그래머에게 스레드를 생성하고 관리하기 위한 API 제공

    1. 커널의 지원 없이 완전히 사용자 공간에서만 라이브러리 제공
    - 라이브러리 콜은 시스템 콜과 무관하게 lib function call이다
    - POSIX / JAVA 
    - JVM의 thread 구현은 host가 어떤 라이브러리 API를 구현했는지에 따라 달라진다.
    2. 운영체제에 의해 지원되는 커널 수준 라이브러리 구현
    - 라이브러리 콜은 시스템 콜
    - 코드 /자료구조 커널에 존재
    - WINDOW 

- JVM 이 window 구현체에서 thread 구현시 WINDOWS API 이용
- LINUX/MAC / UNIX use POSIX , JVM 도 그럼 posix를 wrapping ? 

**synchronized vs asynchronized**

synchronzied

- 부모가 자식의 실행을 모두 기다리고 다시 재개
- 부모가 생성한 자식들은 병행이지만 부모는 자식이 모두 종료될때까지 대기

asynchronzied

- 부모는 자식과 상관없이 서로 독립적으로 실행



### Pthread

### Java Thread

- Java thread를 사용하면, JVM 이 구현하는 어느 host에서도 돌아간다 (당연..)
- Java 에서 thread 구현의 두가지 방법
    - extend Thread class
    - implements Runnable interface

```java
class Task implements Runnable {
    public void run(){
	Systme.out.println("I am a thread.");
    }
}

Thread worker = new Thread(new Task());
worker.start();
// 1. memory 할당 , JVM 새로운 스레드 초기화
// 2. run()을 호출하면 스레드가 JVM 에 의해 실행될 자격을 갖추게 함
// start() -> run() 호출함
```

join()을 사용하면 thread의 완료를 기다릴 수 있음
```java
try { 
worker.join()
}
catch(InterruptedException e){

}
```

**Excecutor Framework**

```java
/**
*An Executor that provides methods to manage termination and methods 
that can produce a Future for tracking progress of one or more asynchronous tasks.

An ExecutorService can be shut down, which will cause it to reject new tasks. 
Two different methods are provided for shutting down an ExecutorService. 
The shutdown() method will allow previously submitted tasks to execute before terminating, 
while the shutdownNow() method prevents waiting tasks from starting and attempts to stop currently executing tasks. Upon termination, an executor has no tasks actively executing, no tasks awaiting execution, and no new tasks can be submitted. An unused ExecutorService should be shut down to allow reclamation of its resources.

Method submit extends base method Executor.execute(java.lang.Runnable) by creating and returning a Future that can be used to cancel execution and/or wait for completion. Methods invokeAny and invokeAll perform the most commonly useful forms of bulk execution, executing a collection of tasks and then waiting for at least one, or all, to complete. (Class ExecutorCompletionService can be used to write customized variants of these methods.)


*/

public interface Executor
{
    void execute(Runnable command);
}

Executor service = new ExecutorImple();
service.execute(new Task());
```
- java 1.5 , java.util.concurrent
- An Executor that provides methods to manage termination and methods that can produce a Future for tracking progress of one or more asynchronous tasks.
- Future를 return할 수도 있는(return 안해도 됨) 비동기적인 task 처리를 위해, progress를 tracking하고, termination의 method를 지원함
- ExecutorService 는 shutdown 될 수 있음, 새로운 task를 받지 않음
    - shutdown()은 전에 제출된 task를 terminate 전에 실행할 수 있음
    - shutdownNow() 는 모든 task를 실행하지 않고, 현재것도 멈춤
- 사용하지 않는 ExecutorService는 shutdown 되어야 함 (resource)


- submit()은 Executro.execute(Runnable) 을 사용함,
- submit()에서 Future를 생성해서 execution을 취소하거나, wait for compeletion 하도록 함
```java
    public <T> Future<T> submit(Runnable task, T result) {
        if (task == null) throw new NullPointerException();
        RunnableFuture<T> ftask = newTaskFor(task, result);
        execute(ftask);
        return ftask;
    }

    public <T> Future<T> submit(Callable<T> task) {
        if (task == null) throw new NullPointerException();
        RunnableFuture<T> ftask = newTaskFor(task);
        execute(ftask);
        return ftask;
    }


```
https://docs.oracle.com/javase/7/docs/api/java/util/concurrent/ExecutorService.html


- Executor method는 provider - consumer model 
    - provider : Programmer implements Runnable(Callable)
    - consumer : Executing Thread
- Executor는 스레드의 생성 / 실행을 분리하고, command(terminate)등도 제공함
    - 생성을 분리한다는 것은 task가 생길때 생성을 꼭 해야할 필요가 없다는 뜻 
    - 미리 만들어 둘 수 있음
    - 생성하고도 실행을 나중에 하는것도 가능

```java
class Summation implements Callable<Integer> {

    private final int upper;
    public Summation(int upper){
	this.upper = upper;
    }

    public Integer call(){
	int sum = 0;
	for(int i = 1; i <= upper ; i++){
	    sum += i;
	}
	return new Integer(sum);
    }
}

public class Sample {
    public static void main(String[] args){
	int upper = 3; 
	ExecutorService pool = Executors.newSingleThreadExecutor();
	Future<Integer> result = pool.submit(new Summation(upper))

	System.out.println(result.get());

    }

}

```







