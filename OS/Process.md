

# Process

- 실행중인 프로그램



### Process Memory 

<img src="https://i.stack.imgur.com/DkUbU.png">


Fixed 

Text : Code
Data : Global Variable

Not Fixed(Dynamically grow / shrink)

Heap : Dynamic memory allocation
Stack : Temporary memory, when function called ( arguments, local variable, return address )



### Process State ( Process Lifecycle )

<img src="https://www.researchgate.net/publication/332546783/figure/fig3/AS:749937696464896@1555810488119/Process-state-transition-diagram.png">


- 운영체제마다 이름이 다를 수 있음
- State는 기본적으로 가질 것임

new : 프로세스 생성중
ready : 프로세스가 처리기에 할당되기를 기다림
running : 프로세스가 실행중
waiting : 프로세스가 어떤 이벤트(입출력 완료 / signal)이 일어나길 기다리는중
terminated : 프로세스 실행 종료



### PCB (Process Control Block / TCB Task Control Block)

다음과 같은 data를 가지고 있음

- Process State : new / ready/ running ...
- Program Counter (PC) : 다음 실행할 명령어주소
- CPU Register : accumulator, stack register, index register 같은state 정보 
- CPU Scheduling Info
- Memory Management Info : Base/ Limit 
- Accounting
- I/O : I/O hardware that is opened to this process, including opened file


#### Linux Task Struct

```cpp
long state; // process state
struct sched_endtity se; // scheduling info
struct task_struct *parent; // parent task
struct list_head children; //child task
struct fileds_struct *files; // opened fileds
struct mm_strut *mm; // memory info pointer 

```


### Process Scheduling 

코어 당 하나의 프로세스가 실행 가능하다 
(정확히는 thread겠지만, 어쨋든 하나의 프로세스의 쓰레드가 실행가능ㅎ)


 1. Process 가 New상태에서 실행 될 준비가 되면 Ready state로 전환되고, 이는 Ready queue에 들어간다. 
 2. 실행 중, 어떤 특정 이벤트가 발생하기를 기다리게 되면 , wait queue 에 삽입된다.
 - I/O request
 - Child Process Fork and waiting to get end sig from child 
 - Interrupt(not time slice interrupt, somekind of wait interrupt)

 3. 실행 중, 다음 특정 이벤트가 발생하면, ready queue로 돌아간다.
 - Time Slice 가 만료되어 ready queue 로 돌아감

2와 같은 경우에는 wait -> ready로 전환된다.

<img src="https://www.guru99.com/images/1/122319_0900_ProcessSche1.png">

### Context Switching

Interrupt -> CPU core를 현재 작업에서 뺏어 내어 커널 루틴을 실행시킬 수 있음 
interrupt를 하면 기존에 수행하던 작업 상황을 저장해야지, 후에 다시 restore 시켜서 재개할 수 있음
즉 context를 저장해야함

Context는 PCB에 표현됨 (register, PC, memory state . .)

Context Switching 
- CPU core를 다른 프로세스로 교환하기 위해, state save/ state restore를 하는 작업
- context switching 동안은 다른 작업을 수행할 수 없음
- 순수한 overhead
- Hardware 지원에 성능이 좌우
    - 예를 들어 register set을 지원하는 hw의 경우 register를 전부 복사하는 것이 아닌, Register의 pointer를 바꾸는 것으로 할 수있음 
    - 이 작업도.. 어쨋든 process가 많아지면, memory로 왔다갔다 해야하기 때문에 overhead가 커짐

<img src="https://www.tutorialandexample.com/wp-content/uploads/2020/04/What-is-Context-Switching-1.png">


### Process Creation

- process idenitified by unique PID
- Linux에서는 pid가 1인 systemd 프로세스가 모든 프로세스의 root로 존재
- Mac도 마찬가지로 /sbin/launchd 프로세스가 root로 존재

새로운 process 생성의 두가지 방법
- 부모는 자식과 병행하게 실행을 계속한다.
- 부모는 일부 또는 모든 자식이 실행을 종료할 때까지 기다린다.(안 기다릴 수도? )

새로운 process는 다음과 같이 두가지로 구분
- 자식 프로세스는 부모 프로세스의 복사본
- 자식 프로세스가 자신에게 적재될 새로운 프로그램을 가지고 있음(서로 다른 프로그램을 실행함)


fork()

- pid = fork()
- pid == 0 -> child process
- pid == ${child process number} -> parent process


exec()

- if exec() called, destroy current program memory
- and load new binary program, and start program


so when fork() + exec() -> whole new program starts in another process


### Process Termination

- exit(int status) system call 
- child can return integer state to parent process

> The exit() function causes normal process termination and the least significant byte of status (i.e., status & 0xFF) is returned to the parent (see wait(2)).
https://man7.org/linux/man-pages/man3/exit.3.html

- other process can terminate process with system call (TerminateProcess(pid))
- only parent can terminate child 
- or else the crazy process can kill every process
- parent needs pid to kill child, so fork() returns pid of child
- several systems allow child without parent, but some are allowed
- when child without paretn is not allowed, and parent terminates, cascading termination occurs(연쇄식 종료)


### Zombie Process

> 종료되었지만, 부모 프로세스가 wait() 호출을 하지 않는 프로세스

부모 프로세스는 wait()를 통해 자식 프로세스가 종료될 때를 기다릴 수 있음

```cpp
pid_t pid;
int status;

pid = wait(&status);

```

- 프로세스가 종료하면 사용하던 자원은 운영체제가 회수한다.
- 그러나 프로세스의 종료상태가 저장되는 프로세스 테이블은 부모 프로세스가 wait()을 호출할 때까지 남아있게 된다. (부모에게 이 process가 종료되었다는 것을 알리려면, process state를 finished로 어딘가에 저장하고 있어야 한다. 언제까지? wait()으로 확인할때까지 )
- 모든 프로세스가 종료되면 아주 짧은 시간동안 좀비 프로세스가 되지만, 바로 wait()로 PID / PCB가 회수된다.


### Orphan Process

> 자식이 종료되기 전, 부모가 종료된 process

- wait()이 호출이 안되어 PCB 등의 자원이 회수가 되지 않음
- 보통 운영체제가 init(systemd)등의 프로세스로 부모를 지정하여 주기적으로 wait()을 호출하여 해결함



### InterProcess Communication

#### Shared Memory vs Message passing

SharedMemory : 공유하는 메모리 영역을 통하여 통신
Message Passing : 메시지를 통해서 통신

- Message Passing은 race condition 에 구애 받지 않으므로 구현이 쉽다.
- 적은 메모리에 유리함
- 분산시스템에서도 구현하기 용이함
- 시스템 콜을 추가적으로 이용하므로, 상대적으로 공유 메모리 보다 느림



#### Shared Memory 


- 각 통신하는 프로세스들이 공유 메모리를 구축해야한다
- 통상적으로 데이터를 생성하는 쪽의 메모리 공간에 유지한다. 
- 이 데이터를 생성하는 프로세스와 통신하고자 하는 프로세는 이 메모리 공간을 자신 주소공간에 추가해야한다
- 각 프로세스는 동시에 동일한 위치에 쓰이지 않도록 각자가 취급을 잘해야한다.

생산자와 소비자

- 생산자와 소비자는 반드시 동기화되어야 한다.
- 생산하지 않은 영역을 사용하려고 하지 않게 해야한다.

무한 버퍼 vs 유한 버퍼

- 원형의 무한버퍼는 생산자는 무제한의 생산이 가능하게 하고, 소비자는 기다려야 할 수 도 있다.(실제로 소비자가 소비한 부분을 check할 경우 무제한의 생산은 아님)
- 유한 버퍼는 , 만약 다 찰경우 생산자가, 하나도 없을 경우 소비자가 대기해야한다.


#### Message passing

- send(message)/ receive(message)를 구현해야 한다.
- 고정길이/가변길이로 구현할 수 있는데, 고정길이가 더 쉽다.
- 구현하는 방법은 여러가지가 있으나, 다음과 같은 쟁점을 고려해야 한다.

- 직접 vs 간접 통신
- 동기식 vs 비동기식 통신
- 자동 vs 명시적 버퍼링

### 직접 vs 간접 - 명명 

#### 직접 통신

- 수신자/ 송신자 이름 명시 필요
- 통신을 원하는 각 프로세스가 자동으로 구축됨, 필요한건 상대 id
- 연결은 정확히 두 프로세스 사이에서만
- 통신하는 프로세스끼리는 정확하게 하나의 연결만

send(P, message) - process P에 메시지 전달
receive(Q, message) - process Q로 부터 메시지 전달 받음

- 수신/송신이 정확히 서로를 가리키는것을 대칭성이라고 부름
- 송신 받는 쪽이 주소를 제시 하지 않으면 비대칭성이라고 부름

send(P, message) - process P에 메시지 전달
receive(id, message) - 어떤 process 부터 메시지 전달 받음, 송신자의 id가 argument로 들어옴

#### 간접 통신

- 각 process id를 명시하기 보다는 mailbox / port를 명시한다
- 따라서 송신할때도 mailbox id / port id를 명시한다.
- 공유 메일박스를 가져야만 한다
- 메일 박스에 다수의 프로세스가 송신 /수신이 가능하다.
- p1 -> A -> p2 / p3 같이 송신할 때 어느 프로세스가 수신하는지는 결정하기 나름
    - 하나의 박스가 오직 2개의 프로세스와 연결되게 한다
    - receive()가 하나의 프로세스에서만 실행되도록 보장
    - 라운드로빈 같은 방식을 정해서 임의로 수신되게 함
    - 아니면 전부 줘도 상관없고..
- 메일 박스에 소유자를 정할 수 있게 해야한다.
- 메일 박스가 사라질 경우, 이 메일 박스에서 수신하는 모든 프로세스에게 알려야한다.

send(A, message) - mailbox A에 메시지 전달
receive(A, message) - mailbox B로 부터 메시지 전달 받음


### 동기식 vs 비동기식 - 동기화 synchornization


blocking send : 송신하는 프로세스는 수신/ 멩일박스에 의해 수신될때까지 block
non-blocking send : 메시지 보내고 작업 재시작
blocking receive : 메시지가 이용가능할 때까지 block
non-blocking receive : 송신하는 프로세스가 유효한 메시지 또는 null을 받음


### 자동 vs 명시적 버퍼링-  Buffering

zero capcity : 큐길이 0, 대기 메시지 없음, 송신자는 수신자가 메시지를 수신할 때까지 기다림
bounded capacity : n 개의 큐, 송신자는 큐가 full이면 대기, 수신자는 큐가 empty면 대기
unbounded capacity : 무한, 송신자는 대기하지 않는다.


### Communication in Client-Server Systems 

#### Socket
 
#### Remote Procedure Calls (RPC)

- IPC 와 유사함
- RPC 통신에서의 메시지는 구조화되어있음
    - 원격지 포트에서 listen중인 RPC daemon의 주소
    - 실행되어야 할 함수의 식별자
    - 함수에게 전달되어야 할 매개변수

- 원격 프로세스가 어떤 서비스를 받고자 하면, 그 서비스에 대응하는 포트 주소로 메시지를 보내면 됨
- 서비스를 제공하는 쪽은 RPC를 제공하는 daemon을 해당 port에 등록해놓음
- 원격 호스트의 프로세스 내 프로시져를 호출한는 것이지만, 마치 자기의 프로시져를 호출하는 것 처럼 해준다

- 이걸 해주는 것이 RPC 서비스 
    - Client 가 원격 프로시져를 호출하면 RPC는 해당하는 스텁을 호출하고, 프로시저가 필요한 매개변수를 전달함
    - 그러면 원격 서버의 포트를 찾아 매개변수를 marshall 한뒤 서버에 메시지를 전송
	- marshall 이 필요한 이유는 32bit/64bit big-endian / little-endian 의 차이가 있기 때문
	- 그래서 중립적인 방식인 XDR 방식으로 변환해서 보냄
    - 그러면 서버쪽 스텁이 메시지를 수신한 후 적절한 서버의 프로시저를 호출
    - RPC 서비스가 고려해야할 사항
	- local 서비스와 다르게, network failure로 인한 failure/ duplicated execution hazard
	- TimeStamp / Ack need to prevent 
    - RPC procedure는 서로의 process 정보를 procedure binding time에 알 수 없다
	- 고정된 포트 주소 사용
	- 고정 RPC 포트를 통해 matchmaker라는 랑데부용 포트에 질의 
	    - RPC id를 주면 port return 
	


#### GRPC 

//@TODO 






