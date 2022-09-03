

Writing data to main memory is expensive(compare to cpu cache update)

Whenever a Processor wants to write a word, it checks to see if the address it wants to write the data to, is present in the cache or not. If the address is present in the cache i.e., Write Hit. 

## IF cache hit

### Write Through

![write through](https://media.geeksforgeeks.org/wp-content/uploads/20200122184347/1406-2.png)

- simulatenousely updated to cache & memory
- simple / reliable
- if no frequent write to cache
- no inconsistency problem
- but perfromance -> write cache is redundant, cache is used to only read



### Write Back

![write back](https://media.geeksforgeeks.org/wp-content/uploads/20200122163548/223-1.png)

- data is updated only in the cache and updated in memory latter 
- data is updated when cache is replaced ( by LRU, LFU, FIFO ... )
- Each cache needs to have *DirtyBit* 
- If DirtBit is clean, then just replace cache
- if dirty bit is dirty, then write to memory


## IF cache miss

### Write Allocation

![write allocation](https://media.geeksforgeeks.org/wp-content/uploads/20200122164750/3164.png)

- data is loaded from memory to cache then update
- generally used with write back

### Write Around

![write around](https://media.geeksforgeeks.org/wp-content/uploads/20200122220402/496.png)

- directly updateds to the main memory







