

## GC 

- JVM Spec에 GC implementation에 관해서는 정의하지 않는다. 
- GC는 trade-off 의 싸움


- 중단 시간
- GC 시간 / 어플리케이션 런타임 ( 처리율)
- 중단 빈도 
- 회수 효율
- 중단 일관성

## Why STW Happens? 

- 안정된 객체 그래프를 유지하기 위해 
- GC 대상을 선정할 때, 변경이 일어나면 다시 선정해야한다. 

## How STW Happens? 

- JVM Safe Point 
- JVM 이 가지고 있는 특별한 실행점 
- JVM이 스레드를 강제로 중단시켜 safe point화 할 수 는 없음, OS thread가 아니기 때문에,
- 다만 이미 safe point로 들어간 thread를 실행을 안시키고 벗어나지 못하게 할 수 있음 


1. JVM이 전역 flag setting
2. application thread는 일정 execution마다 flag 확인
3. flag가 setting 되어 있으면 safe point(대기)

보통 2바이트 코드 단위, 루프 회귀 점 등에다가 넣어놈 
각 스레드 마다 safe point에 들어가는 시점이 다름 - 먼저 들어가서 대기하는 thread가 존재할 수 있음

## Serial GC 

- Young / Old 모두 m/s algorithm 


## ParallelGC / ParallelOld


## Tri-color Marking Algorithm 

- 회색 : 해당 객체를 처리해야 함
- 검은색: 해당 객체가 ref하고 있는 모든 객체를 식별함, 즉 처리함 
- 흰색 : 처리가 안됨(즉 나중에 가서는 GC대상이 됨)

1. GC root -> grey
2. 다른 객체는 모두 흰색 표시 
3. marking thread가 회색 객체를 돌아다니면서 회색이 참조하고 있는 객체를 전부 회색으로 마킹
4. 만약 3에서 참조하고 있는 객체를 전부 마킹완료 했다면, 자신을 검은색으로 마킹 
5. 회색이 없을때까지 3-4 반복
6. 흰색을 수거 


## Concurrent Mark And Sweep (Using tri - color )

- Snapshot at the begginng 
- 수집사이을 시작할때 접근 가능 / 수집중 신규 생성 -> 살아 있다고 가정(그 짧은 사이에  죽지 않았을 꺼야)

- 만약 marking 중에 새로운 객체가 생성되고, ref된다면? 

![image](https://perfectacle.github.io/2019/05/11/jvm-gc-advanced/tri-color-marking-issue-01.png)
![image](https://perfectacle.github.io/2019/05/11/jvm-gc-advanced/tri-color-marking-issue-02.png)

1. A는 검은색으로 marking 되어 있어서 더이상 mark thread가 travel하지 않는데, ref가 추가되어 버림
2. C는 회색으로 marking 되어있는데, 죽은 객체임

문제를 해결하기 위해 

1. 마크 중 변경 사항을 queue에 기록해서 조정함 
2. 마크 중 변경되면 다시 회색으로 변경시킴 


## CMS GC 

OLD 공간 전용 수집기 
STW 시간을 줄이기 위해 application thread가 돌아가는 중 많은 일들을 한다.
compaction / evacluation이 수행되지 않아 fragmaentation problem 존재 

1. initial marking(STW)

- root->old  / young -> old 를 찾아 root marking(grey in tri - color)

2. concurrent mark / preclean 
 
- old gen 순회 
- application thread가 돌고 있기 때문에, 변경이 생김
- 2,3,5 에서 생긴 object들은 Live로 mark

3. remark(STW)

- old gen remark
- application thread가 update한 object를 찾아서 mark
- 이 단계에서 stw가 일어나기 때문에.. 완전한 marking 가능 

4. concurrent sweep 

- free white

5. concurrent reset

- marking reset


## G1
https://www.oracle.com/technetwork/tutorials/tutorials-1876574.html

- CMS collector 와 같이, application thread와 concurrently하게 work할 수 있음
- compact free space without length GC pause (how?)
- CMS replcation purpose로 개발

Java 9 default GC ( - java 17 default gc )

- 연속된 메모리 공간 아님
- 반구형 레이아웃 아님 



기존 heap layout

![image](https://www.oracle.com/webfolder/technetwork/tutorials/obe/java/G1GettingStarted/images/HeapStructure.png)


G1 Heap layout 
![image](https://www.oracle.com/webfolder/technetwork/tutorials/obe/java/G1GettingStarted/images/slide9.png)

- region으로 구성 
- 1mb default(differ from heap size, heap size /2048(4096)) 
- Global Marking phase를 진행(similar to CMS)
- Marking 이후에 어떤 region이 garbage 가 많은지 알 수 있음
- 선별된 region을 GC 하면, 많은 free space 확보 가능
- G1 은 STW가 존재한다.(not real-time collector)

![image](https://www.oracle.com/webfolder/technetwork/tutorials/obe/java/G1GettingStarted/images/slide9.png)

- Eden / Survivor / Old 가 존재 
- Humongous region - region size의 50% 넘어가면 region을 묶어서 할당 

G1 Algorithm

- Concurrent mark and sweep 

1. 초기 마킹
2. 동시 루트 탐색 
3. 동시 마킹 
4. 재마킹
5. 정리


#### Young GC in G1

![image](https://www.oracle.com/webfolder/technetwork/tutorials/obe/java/G1GettingStarted/images/slide11.png)

- STW 
- Live object evacuation (copy / move) to one (or more survivor region)
- 살아 있는 객체들은 survivor region으로 evacuated 됨, 몇개의 survivor region으로 evacuate 가능
- Age count가 다 찬 객체들은 old로 이동
- Eden/Survivor size calculation
- 전부 evalucated 된 후 Young GC end

![image](https://www.oracle.com/webfolder/technetwork/tutorials/obe/java/G1GettingStarted/images/slide12.png)


- YoungGC 는 parallel(multi thread) 진행



Full GC 


1. Initial Mark (STW)

- YoungGC 단계
- Eden -> Survivor

```sh
(young)(inital -mark)
```

![image](https://www.oracle.com/webfolder/technetwork/tutorials/obe/java/G1GettingStarted/images/slide13.png)

2. Root Region Scanning

- old gen 에 reference를 가지고 있는 survivor region(gc root이 된다) 을 선별
- eden에서 살아남은 객체는 모두 survivor로 옮겨 왔기 때문에 eden -> old는 없음 (stack -> old는 ?)
- 다음 Young GC 탐색 전까지 끝나야 함 

3. Cocurrent Marking

- old gen marking 수행
- This happens while the application is running. This phase can be interrupted by young generation garbage collections.
- 만약 region내 살아있는 객체가 없다면, remark phase에서 free하도록 Region 전체를 mark

![image](https://www.oracle.com/webfolder/technetwork/tutorials/obe/java/G1GettingStarted/images/slide14.png)

4. Remark (STW)

- 3에서 region mark가 되어있는 region free (comple)
- SATB를 사용하기 때문에, remark 작업은 오래걸리지 않는다.

![image](https://www.oracle.com/webfolder/technetwork/tutorials/obe/java/G1GettingStarted/images/slide15.png)

5. CleanUp (STW, concurrent)

Performs accounting on live objects and completely free regions. (Stop the world)
Scrubs the Remembered Sets. (Stop the world)
Reset the empty regions and return them to the free list. (Concurrent)

* Copying(space reclamation) - evacuation progress - STW

evacuate or copy live objects to new unused regions. 
This can be done with young generation regions which are logged as [GC pause (young)]. 
Or both young and old generation regions which are logged as [GC Pause (mixed)]


## Garbage Collection Cycle


Young Only Phase 

- 지속적으로 minor GC 를 수행한다. (eden -> survivor , survivor -> old)
- old gen 비율이(XX:InitiatingHeapOccupancyPercent(Old gen)) threshold가 넘는 순간, Major GC init
- Initial Mark : MajorGC가 init되면, MinorGC 와 동시에 marking phase 수행 -> marking root (STW) + minor GC(STW) 라서 STW가 크다 
- 지속적으로 concurrent marking 수행 + minor GC 
- Remark : STW , region 별 살아있는 객체 확정, SATB 사용 
- CleanUP : region 이 전부 회수 가능하면 회수, 남아 있는 region을 보고 space reclamination 진행 여부 결정, 이 단계에서는 어떤 region이 회수율이 좋을지 안다. 

Space-reclamation phase 

- 회수율이 높은 region부터 Major GC( evacuation + compaction 수행 )



![image](https://docs.oracle.com/javase/9/gctuning/img/jsgct_dt_001_grbgcltncyl.png)
![image](./g1_cycle.png)


- 사이클 중 모든 원은 STW 
- 파랑 원 : minor GC STW 
- 주황 원 : marking STW
- 빨강 원 : mixed GC STW




Rset : Heap 외부에서 내부를 참조하는 레퍼런스 관리 

- old -> young은 거의 없다 
- 하지만 있음
- 그래서 카드테이블을 유지해서 이 객체가 old->young에 속하는지 확인 
- 카드 테이블을 유지하는 이유 -> minor gc를 수행하기 위해 old 영역의 참조를 전부 뒤질 필요 없게 하기 위해서 
- Rset을 유지해서 외부에서 내부를 참조하는 reference를 표시 
- Rset 이 전부 0 면, 참조가 없다는 뜻 

![image](https://www.oracle.com/webfolder/technetwork/tutorials/obe/java/G1GettingStarted/images/slide16.png)
![image](https://www.oracle.com/webfolder/technetwork/tutorials/obe/java/G1GettingStarted/images/slide17.png)

