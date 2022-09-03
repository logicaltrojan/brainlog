

## Transaction 이란 무엇인가?  


> Database Transcation 은 데이터베이스 관리 시스템 또는 유사한 시스템에서 상호작용의 단위이다. 
> Data 에 대한 하나의 논리적 실행단계



## Transaction - ACID Principle


### Atomicity  

- 원자성은 트랜잭션과 관련된 작업들이 부분적으로 실행되다가 중단되지 않는 것을 보장하는 능력이다. 
- 즉 성공하면 transcation 내부의 모든 논리적 절차가 accept 되고, 아니면 전 상태로 회귀하여 유지한다. 
- No Partial Fail / Success 

### Consistency 

- 일관성은 트랜잭션이 실행을 성공하면 언제나 일관성 있는 데이터베이스 상태로 유지하는 것을 의미한다. 
- 예를 들어 제약조건에 대해 변경을 가하거나, Transaction이 db의 constraint를 어기는 경우의 transaction은 중단됩니다.
- 때때로 transaction 수행 중 db inconsistency의 발생이 가능하지만, 결과적으로 db는 consistency 상태를 유지해야합니다. 
- Data is in a consistent state when a transaction starts and when it ends.
- For example, in an application that transfers funds from one account to another, the consistency property ensures that the total value of funds in both the accounts is the same at the start and end of each transaction.

### Isolation 

- The intermediate state of a transaction is invisible to other transactions. As a result, transactions that run concurrently appear to be serialized.
- For example, in an application that transfers funds from one account to another, the isolation property ensures that another transaction sees the transferred funds in one account or the other, but not in both, nor in neither.


### Durability

- After a transaction successfully completes, changes to data persist and are not undone, even in the event of a system failure.
- For example, in an application that transfers funds from one account to another, the durability property ensures that the changes made to each account will not be reversed.



## Transaction Isolation 

- Simulataneous Transcation Request
- 여러 트랜잭션이 다른 트랜잭션에서 변경하거나 조회하는 데이터를 볼 수 있도록 허용할지 말지 결정하는 것 


Transaction Isolation level

- READ UNCOMMITED
- READ COMMITED
- REPEATABLE READ
- SERIALIZABLE

https://nesoy.github.io/articles/2019-05/Database-Transaction-isolation

#### READ UNCOMMITED

- Transaction can read value regardless of other transaction commmit/rollback 

<img src="https://nesoy.github.io/assets/posts/img/2019-05-08-21-09-02.png" width="50%" height="50%">

##### Problem - Dirty Read

Transaction2 can read uncommited changes, if transaction1 fails and rollback, the transaction2 will be affected. 
(Transaction Isolation is not guranteed)

#### READ COMMITED 

- While transaction, dbms retrieves UNDO db value which LAST successfully commited. 

<img src="https://nesoy.github.io/assets/posts/img/2019-05-08-21-18-08.png" width="50%" height="50%">

##### Problem 2 - Non-Reapeatable Read

Transaction has different value when repeatable read, which differs by transaction1's commit / rollback 

#### REPEATABLE READ

- 트랜잭션마다 ID 를 부여함
- ID가 작은 트랜잭션 번호에서 변경한 것만 READ 가능 (따라서 transaction 안에서 read 된 값이 동일하다)
- MVCC (Multi Version Cuncurrency Control) 

<img src="https://nesoy.github.io/assets/posts/img/2019-05-08-21-52-08.png" width="50%" height="50%">

##### Problem 3 - Phantom Read

<img src="https://nesoy.github.io/assets/posts/img/2019-05-08-22-14-18.png" width="50%" height="50%">

- 다른 트랜잭션에서 update 가 아닌 write를 하게 되면 레코드가 추가된 부분이 보였다가 안보였다가 한다. 
- write lock을 걸어야 한다.



## Transaction in Spring


JDBC Transactional Code
```java

import java.sql.Connection; Connection connection = dataSource.getConnection(); // (1) 

try (connection) { 
    connection.setAutoCommit(false); // (2) // execute some SQL statements... connection.commit(); // (3) 
} 
catch (SQLException e) { 
    connection.rollback(); // (4) 
}

```
https://jeong-pro.tistory.com/228




Using @Transactional

    @Transactional
    public boolean insertUser(User user){
    }







