

## 1.4 성능 분류

- 처리율 - throughput
- 지연 - latency
- 용량 - capacity
- 사용률 - utilization
- 효율 - erfficiency
- 확장성 - scalability
- 저하 - degradation


### 처리율 - throughput

시스템이 수행 가능한 작업 비율을 나타내는 지표
- 작업량 / 일정 시간 으로 보통 표시


### 지연 - latency

하나의 트랜잭션이 결과가 나올때까지 걸리는 시간
- 1초에 100리터 흘러보내는 수도관 
- 100L/ 초 가 처리율
- 지연 : 수도관 자체의 길이


### 용량 - capacity

시스템이 보유한 작업 병렬성의 총량 
시스템이 동시 처리 가능한 작업 단위 


### 사용률 - utilization

시스템 리소스의 사용률 

워크로드 종류에 따라서 리소스가 다를 수 있다 
계산 집약적 , IO 집약적에 따라 CPU/ MEM 사용량은 달라진다

### 효율 - erfficiency

처리율 / 리소스 사용률

### 확장성 - scalability

리소스 대비 처리율이 변경되는 형태 

- 궁극적으로는 linear 하게 증가하는 형태를 지향

### 저하 - degradation

시스템이 특정 시점에서 처리율이 더 늘어나지 않는, 지연만 늘어나는 현상



