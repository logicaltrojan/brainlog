


Thread : unit of scheduling / execution

- CPU 이용의 기본 단위이다.
- It is a "thread of execution"
- The smallest unit of processing that can be performed in an OS
- The smallest sequence of programmed instructions that can be managed independently by a schdule
- Is a abstraction concept 
- Which means implementation of threads differ from opreating system


- Threads can't exist outside any process
- Each treads belongs to excactly one process 
- Because it is smallest unit of processing, it has to have requirements for execution
- It is registers / stack / counter 



Process : unit of resources

- 실행 중인 프로그램 
- Program that is in execution
- Contolled by PCB(Process Control Block)




## Thread / Process 의 차이점이 뭐냐고 묻는다면, 그건 OS implementation에 따라 다르다.

그래서 큰 concept적인 개념은 아래와 같은 차이점이 있지만, implementation 상에서 상세하게 다릅니다. 

그냥 thread랑 process는 같은데, thread가 조금 더 lighweight하게 생성됩니다. 라고 하면 면접에서 떨어지겠지, 하지만 linux에서는 맞다.(공룡책 4.7.2 Linux 스레드, Linux는 프로세스와 스레드를 구분하지 않는다)

Linux는 프로그램 내 제어의 흐름을 나타내기 위하여 task라는 용어를 사용한다. 

clone()이 호출될 때(clone() -> make a new task) parameter로 parent task / chid task가 어떤 부분을 공유할 것인지 결정할 수 있다

| flag | meaning
|-|- 
CLONE_FS | Share File system info
CLONE_VM | Share memory 
CLONE_SIGHAND | Share Signal handler
CLONE_FILES | Share Opened Files

Task를 생성시 fork()를 호출할 경우, 모든 자료구조를 복사하여 사용, clone()으로 share를 할경우 일부는 부모 태스크의 자료구조를 가리키게 됨

따라서 이렇게 호출할 경우 자식 task가 부모 task에 있는 거의 모든 자원을 공유하면서, thread를 생성하는 것과 같은 결과를 지니게 됨.

```cpp
clone( ... 0 | CLONE_FS|CLONE_VM|CLONE_SIGHAND|CLONE_FILES );
//clone에서 shared option을 넣을 수 있음 
//https://man7.org/linux/man-pages/man2/clone.2.html
//https://www.baeldung.com/linux/fork-vfork-exec-clone
```

결론적으로 Linux에서는 thread/process를 생성하는 한다는 개념보다는, task의 공유수준에 따라, thread의 성격을 지니지는지, process의 성격을 지니는지 구분할 수 있음 


