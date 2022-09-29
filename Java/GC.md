

# GC

## Important rule of GC 

1. 살아 있는 객체는 절대 수집해선 안된다.
2. 1에 위배되지 않게, 반드시 모든 가비지를 수집해야 한다. 

## Mark And Sweep 

1. 모든 할당된 객체의 Mark bit 을 0으로 초기화 
2. GC root으로 부터 살아 있는 객체를 찾음
3. 2에서 찾은 살아있는 객체에 mark bit을 set 
    - 살아 있는 객체를 root으로 다시 recursive하게 mark 
    - dfs 결과를 live object graph라고 한다
4. 모든 할당된 객체를 순회하며 mark bit을 check
    - 힙에서 메모리 회수 
    - 할당 리스트에서 객체 삭제 


## Terminology 


STW : Stop the World 

- GC가 일어날때, heap state를 valid하게 유지하기 위해, 모든 application thread가 중단되는 것 

Concurrency  - 병렬

- GC Thread는 application thread와 병렬 실행 될 수 있음
- 그러나 완전한 병렬은 없음
- mark - sweep GC 과정에서 이미 mark한 객체가 무효화되는 경우가 있을 경우, GC 가 불가능해지기 때문 
- 따라서 어느 순간에는 STW가 필요하게 됨 


Compression - 압착

- 살아남은 객체들은 연속된 메모리로 배열되며, 빈 공간의 시작점을 가리키는 포인터가 존재 



## 객체를 runtime에 표현하는 방법 

Hotspot jvm : ordinary object pointer 구조체를 이용 


## GC Root / Arena 

https://d2.naver.com/helloworld/329631
https://perfectacle.github.io/2019/05/07/jvm-gc-basic/

그럼 mark & sweep을 한다고 치고, 시작점을 어떻게 할 것인가? 
시작점은 단일한 지점이 아니라, 다음과 같은 GC Root들을 전부 확인해야 한다. 
heap 외부에서 access 할 수 있는 point라고 하는데, 객체의 생성이 heap 외부에서 시작(class loader, thread stack)
heap에 있는 객체들의 참조는 아래 4가지 중 한개 

- 힙 객체 -> 힙 객체  
- stack local var / param ->  heap 
- JNI stack -> heap
- method static var -> heap

2,3,4 가 외부 참조에 의한 root set
<img src="./helloworld-329631-1.png">

Class: Classes loaded by a system class loader; contains references to static variables as well
Stack Local: Local variables and parameters to methods stored on the local stack
Active Java Threads: All active Java threads
JNI References: Native code Java objects created for JNI calls; contains local variables, parameters to JNI methods, and global JNI references


<img src="./helloworld-329631-2.png">


## Weak Generation 


젊은 객체를 빠르게 수집할 수 있도록 설계해야한다.
늙은 객체와 단명 객체를 떼어놓는 게 가장 좋다.
Hotspot VM은 Weak Generational 가설을 활용해 아래와 같은 규칙을 만들었다.

- 객체마다 generational count(객체가 지금까지 무사통과한 가비지 컬렉션 횟수)를 센다.
- 새로운 객체는 Young Generation이라는 공간에 생성된다.
- 장수했다고 할 정도로 충분히 오래 살아남은 객체들은 별도의 메모리 영역(Old Generation 또는 Tenured Generation)에 보관된다.
- 또한 Weak Generational 가설 중에 **’늙은 객체가 젊은 객체를 참조할 일은 거의 없다.’**는 내용도 있는데 아예 없는 건 아니므로 Hotspot VM에서는 카드 테이블(JVM이 관리하는 바이트 배열로 각 요소는 Old Generation 공간의 512 바이트 영역을 가리킨다.)이라는 자료구조에 늙은 객체가 젊은 객체를 참조하는 정보를 기록한다.  따라서 Young Generation의 GC가 수행될 때 늙은 객체가 젊은 객체를 참조하는지도 확인해봐야한다.  하지만 이 때는 늙은 객체를 전부 뒤져보는 게 아니라 카드 테이블만 뒤져보면 돼서 GC의 수행 속도를 높여준다.


## Young Generation / Old Generation

- GC 를 특정 영역에서만 수행할 수 있어서 GC 시간이 줄어든다.
- raw address를 직접 사용하는 경우, young->old로 넘어갔을 때 raw address가 달라지는 문제가 발생하지만, JVM 의 경우 raw address를 사용하지 않아서 


### Young Generation 

- Most 객체들이 소멸하는 지역 
- GC 자주일어남 
- 객체마다 generation count 
- 객체 생성시 eden space에 생성
- eden space GC 시(minor GC) survivor 영역으로 모든 객체 반출

<img src="https://perfectacle.github.io/2019/05/07/jvm-gc-basic/survivor-space-01.png">
<img src="https://perfectacle.github.io/2019/05/07/jvm-gc-basic/survivor-space-02.png">


## TLAB - ThreadLocal Allocation Buffer

- Eden을 여러 buffer로 나눔 
- 이 buffer를 thread 마다 할당 - TLAB
- thread끼리 buffer를 침범할까 관리할 필요 없음 
- TLAB은 정해진 size가 있는데 , 만약 더 큰 할당이 필요하다면 
- TALB을 재할당 하거나 , eden space의 공용 공간을 할당함(synchonization이 필요해짐)

## 반구형 수집 - (방출 수집기 evacuating collectior) 

- 수집기가 라이브 영역을 수집할때, 객체들은 다른 반구로 압착시켜 옮기고, 수집된 반구는 비워서 사용 
- 절반의 공간은 완전 비워둠 (for extraction)
- eden-> survivor 
- survivor from <-> survivor to 

<img src="https://perfectacle.github.io/2019/05/07/jvm-gc-basic/minor-gc-02.png">

객체를 할당하기 위해 minor gc 수행

<img src="https://perfectacle.github.io/2019/05/07/jvm-gc-basic/minor-gc-03.png">

GC root에서 mark

<img src="https://perfectacle.github.io/2019/05/07/jvm-gc-basic/minor-gc-04.png">

mark 객체 survivor space 복사 

<img src="https://perfectacle.github.io/2019/05/07/jvm-gc-basic/minor-gc-05.png">

generation count를 1증가 

<img src="https://perfectacle.github.io/2019/05/07/jvm-gc-basic/minor-gc-06.png">

eden space sweep

<img src="https://perfectacle.github.io/2019/05/07/jvm-gc-basic/minor-gc-07.png">

<img src="https://perfectacle.github.io/2019/05/07/jvm-gc-basic/minor-gc-09.png">

minor gc시eden + survivor 영역을 mark

<img src="https://perfectacle.github.io/2019/05/07/jvm-gc-basic/minor-gc-12.png">

sweep 후 eden-> survivor , generation count 증가

<img src="https://perfectacle.github.io/2019/05/07/jvm-gc-basic/minor-gc-13.png">
<img src="https://perfectacle.github.io/2019/05/07/jvm-gc-basic/minor-gc-13.png">
<img src="https://perfectacle.github.io/2019/05/07/jvm-gc-basic/minor-gc-14.png">
<img src="https://perfectacle.github.io/2019/05/07/jvm-gc-basic/minor-gc-15.png">
<img src="https://perfectacle.github.io/2019/05/07/jvm-gc-basic/minor-gc-16.png">

eden space gc 수행중 ,survivor space에 메모리 할당이 불가능 (memory fragmentation problem)
https://stackoverflow.com/questions/10695298/java-gc-why-two-survivor-regions

<img src="https://perfectacle.github.io/2019/05/07/jvm-gc-basic/minor-gc-17.png">
<img src="https://perfectacle.github.io/2019/05/07/jvm-gc-basic/minor-gc-18.png">

survivor from -> survivor to , eden-> survivor 수행

<img src="https://perfectacle.github.io/2019/05/07/jvm-gc-basic/minor-gc-19.png">


### Card Table

<img src="https://perfectacle.github.io/2019/05/07/jvm-gc-basic/minor-gc-25.png">

- OldGeneration의 모든 객체에서 mark & sweep을 수행하면 시간이 오래걸린다.
- OldGeneration의 참조형 필드값이 변경되면 old 객체가 객체를 참조하고 있다는 flag만 기록 
- mark할 old generation 객체를 선별할 수 있음 


### Promotion 

- Survivor에서 일정 generation count가 지난 객체는 old generation 영역으로 이동 

### Premature Promiton 

- Eden -> Survivor 이동에서 survivor에 충분한 공간이 없을 경우, 바로 old gen이동
- Eden 용량 보다 할당해야할 용량이 큰 경우 old gen에 바로 할당하는 경우도 있다 


GC 

https://perfectacle.github.io/2019/05/11/jvm-gc-advanced/

